
//========================================================================
// EventMangerImpl.cpp : implementation side of the event system
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
#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "IEventManager.h"
#include <list>
#include <map>
#include <Windows.h>

const unsigned int EVENTMANAGER_NUM_QUEUES = 2;

class EventManager : public IEventManager
{
	CRITICAL_SECTION lock;
	typedef std::list<IEventPtr> EventQueue;
	typedef std::list<EventListenerDelegate> EventListenerList;
	typedef std::map<EventType, EventListenerList> EventListenerMap;

	EventListenerMap m_eventListeners;
	EventQueue m_queues[EVENTMANAGER_NUM_QUEUES];
	int m_activeQueue;  // index of actively processing queue; events enque to the opposing queue

	public:
		virtual void Release();
		virtual ~EventManager();

		virtual bool TriggerEvent( const IEventPtr& pEvent ) const;
		virtual bool QueueEvent( const IEventPtr& pEvent );
		virtual bool AbortEvent( const EventType& type, bool allOfType = false );

		virtual bool Update( unsigned long maxMillis = kINFINITE );

		template<typename T, typename U> 
		bool AddListener( T functionPtr, U instancePtr, const EventType& type ){
			EventListenerDelegate eventDelegate = 
			std::bind	(	functionPtr,
							instancePtr,
							std::placeholders::_1
						);

			return AddListener( eventDelegate, type );
		}
		template<typename T, typename U>
		bool RemoveListener( T functionPtr, U instancePtr, const EventType& type ){
			EventListenerDelegate eventDelegate =
			std::bind	(	functionPtr,
							instancePtr,
							std::placeholders::_1
						);
			return RemoveListener( eventDelegate, type );
		}
		static EventManager* GetInstance();

	private:
		static EventManager* instance;
		explicit EventManager();
		virtual bool AddListener( const EventListenerDelegate& eventDelegate, const EventType& type );
		virtual bool RemoveListener( const EventListenerDelegate& eventDelegate, const EventType& type );
};
#endif
