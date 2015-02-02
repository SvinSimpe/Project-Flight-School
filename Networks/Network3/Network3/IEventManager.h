//========================================================================
// EventManager.h : implements a multi-listener multi-sender event system
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
#ifndef IEVENT_MANAGER_H
#define IEVENT_MANAGER_H
#include <functional>
#include <strstream>
#include <memory>
#include <sstream>
#include <map>

//---------------------------------------------------------------------------------------------------------------------
// Forward declaration & typedefs
//---------------------------------------------------------------------------------------------------------------------
class IEvent;

typedef unsigned long EventType;
typedef std::shared_ptr<IEvent> IEventPtr;
typedef std::function<void( IEventPtr )> EventListenerDelegate;

//---------------------------------------------------------------------------------------------------------------------
// IEvent                               - Chapter 11, page 310
// Base type for event object hierarchy, may be used itself for simplest event notifications such as those that do 
// not carry additional payload data. If any event needs to propagate with payload data it must be defined separately.
//---------------------------------------------------------------------------------------------------------------------
class IEvent 
{
	public:
		virtual ~IEvent( void ) {}
		virtual const EventType& GetEventType( void ) const		= 0;
		virtual void Serialize( std::stringstream& out ) const	= 0;
		virtual void Deserialize( std::stringstream& in  )		= 0;
		virtual IEventPtr Copy() const							= 0;
};

class EventFactory
{
	private:
		std::map<EventType, IEvent*> mEvents;
	protected:
	public:

	private:
	protected:
	public:
		template<class SubClass>
		bool Register( EventType id )
		{
			auto findIt = mEvents.find( id );
			if( findIt == mEvents.end() )
			{
				printf( "Registering event with ID: %d!\n", id );
				mEvents[id] = new SubClass;
				return true;
			}
			return false;
		}

	private:
	protected:
	public:
		IEvent* Create( EventType id )
		{
			auto findIt = mEvents.find( id );
			printf( "Trying to retreive event with ID: %d\n", id );
			if( findIt != mEvents.end() )
			{
				IEvent* obj = findIt->second;
				return obj;
			}
			return nullptr;
		}

		EventFactory()
		{
			mEvents = std::map<EventType, IEvent*>();
		}

		virtual ~EventFactory() 
		{
			for( auto& it : mEvents )
			{
				if( it.second )
					delete it.second;
			}
			mEvents.clear();
		}
};

extern EventFactory gEventFactory;
#define REGISTER_EVENT( eventClass ) gEventFactory.Register<eventClass>(eventClass::GUID)
#define CREATE_EVENT( eventType ) gEventFactory.Create(eventType)

//---------------------------------------------------------------------------------------------------------------------
// IEventManager Description                        Chapter 11, page 314
//
// This is the object which maintains the list of registered events and their listeners.
//
// This is a many-to-many relationship, as both one listener can be configured to process multiple event types and 
// of course multiple listeners can be registered to each event type.
//
// The interface to this construct uses smart pointer wrapped objects, the purpose being to ensure that no object 
// that the registry is referring to is destroyed before it is removed from the registry AND to allow for the registry 
// to be the only place where this list is kept ... the application code does not need to maintain a second list.
//
// Simply tearing down the registry (e.g.: destroying it) will automatically clean up all pointed-to objects (so long 
// as there are no other outstanding references, of course).
//---------------------------------------------------------------------------------------------------------------------
class IEventManager
{
public:

	enum eConstants { kINFINITE = 0xffffffff };

	// returns true if all messages ready for processing were completed, false otherwise (e.g. timeout )
	virtual bool Update( unsigned long maxMillis = kINFINITE ) = 0;

    // Registers a delegate function that will get called when the event type is triggered.  Returns true if 
    // successful, false if not.
    
	virtual bool AddListener( const EventListenerDelegate& eventDelegate, const EventType& type ) = 0;

	// Removes a delegate / event type pairing from the internal tables.  Returns false if the pairing was not found.
	virtual bool RemoveListener( const EventListenerDelegate& eventDelegate, const EventType& type ) = 0;

	// Fire off event NOW.  This bypasses the queue entirely and immediately calls all delegate functions registered 
    // for the event.
	virtual bool TriggerEvent( const IEventPtr& pEvent ) const = 0;

	// Fire off event.  This uses the queue and will call the delegate function on the next call to VTick(), assuming
    // there's enough time.
	virtual bool QueueEvent( const IEventPtr& pEvent ) = 0;
	
	// Find the next-available instance of the named event type and remove it from the processing queue.  This 
    // may be done up to the point that it is actively being processed ...  e.g.: is safe to happen during event
	// processing itself.
	//
	// if allOfType is true, then all events of that type are cleared from the input queue.
	//
	// returns true if the event was found and removed, false otherwise
	virtual bool AbortEvent( const EventType& type, bool allOfType = false ) = 0;

	// Allow for processing of any queued messages, optionally specify a processing time limit so that the event 
    // processing does not take too long. Note the danger of using this artificial limiter is that all messages 
    // may not in fact get processed.
	//

	explicit IEventManager();
	virtual ~IEventManager( void );
};

#endif
