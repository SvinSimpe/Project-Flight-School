#pragma once
//========================================================================
// Events.h : defines common game events
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
#include <DirectXMath.h>
using namespace DirectX;

class Event_Client_Joined : public IEvent
{
	private:
		UINT mID;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Joined()
		{
			mID = 0;
		}
		Event_Client_Joined( UINT socket )
		{
			mID = socket;
		}
		~Event_Client_Joined() {}
		int ID() const
		{
			return mID;
		}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::stringstream& out ) const
		{
			out << mID << " ";
		}
		void Deserialize( std::stringstream& in )
		{
			in >> mID;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Joined( mID ) );
		}
};

class Event_Client_Left : public IEvent
{
	private:
		UINT mID;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Left()
		{
			mID = 0;
		}
		Event_Client_Left( UINT socket )
		{
			mID = socket;
		}
		~Event_Client_Left() {}
		int ID() const
		{
			return mID;
		}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::stringstream& out ) const
		{
			out << mID << " ";
		}
		void Deserialize( std::stringstream& in )
		{
			in >> mID;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Left( mID ) );
		}
};

class Event_Text : public IEvent
{
	private:
		int	mSocketID;
		std::string		mText;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Text()
		{
			mSocketID = 0;
			mText	= "";
		}
		Event_Text( int socket, std::string text )
		{
			mSocketID = socket;
			mText = text;
		}
		~Event_Text() {}
		int SocketID() const
		{
			return mSocketID;
		}
		std::string Text() const
		{
			return mText;
		}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::stringstream& out ) const
		{
			out << mSocketID << " ";
			out << mText << " ";
		}
		void Deserialize( std::stringstream& in )
		{
			in >> mSocketID;
			in >> mText;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Text( mSocketID, mText ) );
		}
};