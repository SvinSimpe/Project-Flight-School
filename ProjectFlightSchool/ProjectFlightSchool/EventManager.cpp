//========================================================================
// EventManagerImpl.cpp : implementation side of the event system
//
// Part of the GameCode4 Application
//
// GameCode4 is the sample application that encapsulates much of the source code
// discussed in "Game Coding Complete - 4th Edition" by Mike McShaffry and David
// "Rez" Graham, published by Charles River Media. 
// ISBN-10: 1133776574 | ISBN-13: 978-1133776574
//
// If this source code has found it's way to you, and you think it has helped you
// in any way, do the authors a favor and buy a new copy of the book - there are 
// detailed explanations in it that compliment this code well. Buy a copy at Amazon.com
// by clicking here: 
//    http://www.amazon.com/gp/product/1133776574/ref=olp_product_details?ie=UTF8&me=&seller=
//
// There's a companion web site at http://www.mcshaffry.com/GameCode/
// 
// The source code is managed and maintained through Google Code: 
//    http://code.google.com/p/gamecode4/
//
// (c) Copyright 2012 Michael L. McShaffry and David Graham
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser GPL v3
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See 
// http://www.gnu.org/licenses/lgpl-3.0.txt for more details.
//
// You should have received a copy of the GNU Lesser GPL v3
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//========================================================================


#include "EventManager.h"
#include <windows.h>
#include <windowsx.h>

EventManager* EventManager::instance = nullptr;
//---------------------------------------------------------------------------------------------------------------------
// EventManager::VTick
//---------------------------------------------------------------------------------------------------------------------
bool EventManager::Update( unsigned long maxMillis )
{
	unsigned long currMs = GetTickCount();
	unsigned long maxMs = ( ( maxMillis == IEventManager::kINFINITE ) ? ( IEventManager::kINFINITE ) : ( currMs + maxMillis ) );

	// swap active queues and clear the new queue after the swap
    int queueToProcess = m_activeQueue;
	m_activeQueue = ( m_activeQueue + 1 ) % EVENTMANAGER_NUM_QUEUES;
	m_queues[m_activeQueue].clear();


	// Process the queue
	while ( !m_queues[queueToProcess].empty() )
	{
        // pop the front of the queue
		IEventPtr pEvent = m_queues[queueToProcess].front();
        m_queues[queueToProcess].pop_front();

		const EventType& eventType = pEvent->GetEventType();

        // find all the delegate functions registered for this event
		auto findIt = m_eventListeners.find( eventType );
		if ( findIt != m_eventListeners.end() )
		{
			const EventListenerList& eventListeners = findIt->second;

            // call each listener
			for ( auto it = eventListeners.begin(); it != eventListeners.end(); ++it )
			{
				EventListenerDelegate listener = ( *it );
				listener( pEvent );
			}
		}

        // check to see if time ran out
		currMs = GetTickCount();
		if ( maxMillis != IEventManager::kINFINITE && currMs >= maxMs )
        {
			break;
        }
	}
	
	// If we couldn't process all of the events, push the remaining events to the new active queue.
	// Note: To preserve sequencing, go back-to-front, inserting them at the head of the active queue
	bool queueFlushed = ( m_queues[queueToProcess].empty() );
	if ( !queueFlushed )
	{
		while ( !m_queues[queueToProcess].empty() )
		{
			IEventPtr pEvent = m_queues[queueToProcess].back();
			m_queues[queueToProcess].pop_back();
			m_queues[m_activeQueue].push_front( pEvent );
		}
	}
	
	return queueFlushed;
}
//---------------------------------------------------------------------------------------------------------------------
// EventManager::VAddListener
//---------------------------------------------------------------------------------------------------------------------

bool EventManager::AddListener( const EventListenerDelegate& eventDelegate, const EventType& type )
{

	EventListenerList& eventListenerList = m_eventListeners[type];  // this will find or create the entry
	eventListenerList.push_back( eventDelegate );
	
	return true;
}


//---------------------------------------------------------------------------------------------------------------------
// EventManager::VRemoveListener
//---------------------------------------------------------------------------------------------------------------------
bool EventManager::RemoveListener( const EventListenerDelegate& eventDelegate, const EventType& type )
{
	bool success = false;

    auto findIt = m_eventListeners.find( type );
    if ( findIt != m_eventListeners.end() )
    {
        EventListenerList& listeners = findIt->second;
        for ( auto it = listeners.begin(); it != listeners.end(); ++it )
        {
            if ( (void*)&eventDelegate == (void*)&(*it) )
            {
                listeners.erase( it );
                success = true;
                break;  // we don't need to continue because it should be impossible for the same delegate function to be registered for the same event more than once
            }
        }
    }

    return success;
}


//---------------------------------------------------------------------------------------------------------------------
// EventManager::VTrigger
//---------------------------------------------------------------------------------------------------------------------
bool EventManager::TriggerEvent( const IEventPtr& pEvent ) const
{
    bool processed = false;

	auto findIt = m_eventListeners.find( pEvent->GetEventType() );
	if ( findIt != m_eventListeners.end() )
    {
		const EventListenerList& eventListenerList = findIt->second;
	    for ( EventListenerList::const_iterator it = eventListenerList.begin(); it != eventListenerList.end(); ++it )
	    {
		    EventListenerDelegate listener = ( *it );
		    listener( pEvent );  // call the delegate
            processed = true;
	    }
    }

	return processed;
}


//---------------------------------------------------------------------------------------------------------------------
// EventManager::VQueueEvent
//---------------------------------------------------------------------------------------------------------------------
bool EventManager::QueueEvent( const IEventPtr& pEvent )
{


    // make sure the event is valid
    if ( !pEvent )
    {
        return false;
    }

	EnterCriticalSection( &lock );
	auto findIt = m_eventListeners.find( pEvent->GetEventType() );
	if ( findIt != m_eventListeners.end() )
	{
		m_queues[m_activeQueue].push_back( pEvent );
		LeaveCriticalSection( &lock );
		return true;
	}
	else
	{	
		LeaveCriticalSection( &lock );
		return false;
	}

}

//---------------------------------------------------------------------------------------------------------------------
// EventManager::VAbortEvent
//---------------------------------------------------------------------------------------------------------------------
bool EventManager::AbortEvent( const EventType& inType, bool allOfType )
{
    bool success = false;
	EventListenerMap::iterator findIt = m_eventListeners.find( inType );
	
	EnterCriticalSection( &lock );
	if ( findIt != m_eventListeners.end() )
    {

        EventQueue& eventQueue = m_queues[m_activeQueue];
        auto it = eventQueue.begin();
        while ( it != eventQueue.end() )
        {
            // Removing an item from the queue will invalidate the iterator, so have it point to the next member.  All
            // work inside this loop will be done using thisIt.
            auto thisIt = it;
            ++it;

	        if ( (*thisIt)->GetEventType() == inType )
	        {
		        eventQueue.erase( thisIt );
		        success = true;
		        if ( !allOfType )
			        break;
	        }
        }
    }
	LeaveCriticalSection( &lock );
	return success;
}

EventManager* EventManager::GetInstance()
{
	if( instance == nullptr )
	{
		instance = new EventManager();
	}
	return instance;
}
//---------------------------------------------------------------------------------------------------------------------
// EventManager::EventManager
//---------------------------------------------------------------------------------------------------------------------
EventManager::EventManager()
	: IEventManager()
{
    m_activeQueue = 0;
	InitializeCriticalSection( &lock );
}

void EventManager::Release()
{
	DeleteCriticalSection( &lock );
	if(instance != nullptr)
	{
		delete instance;
	}
}
//---------------------------------------------------------------------------------------------------------------------
// EventManager::~EventManager
//---------------------------------------------------------------------------------------------------------------------
EventManager::~EventManager()
{
}