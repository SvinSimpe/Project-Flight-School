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
#include "IEventManager.h"
#include <DirectXMath.h>
using namespace DirectX;

class TestEvent : public IEvent
{
	private:
	protected:
	public:
		static const EventType GUID;
	

	private:
	public:
		TestEvent()
		{
			//Event Data
		}
	protected:
		const EventType& GetEventType( void ) const
		{
			return GUID;
		}
};

class Event_Player_Moved : public IEvent
{
	private:
		XMFLOAT3 mLowerBodyPos;
		XMFLOAT3 mUpperBodyPos;

	protected:
	public:
		static const EventType GUID;
	
	private:
	public:
		Event_Player_Moved( XMFLOAT3 lowerBodyPos, XMFLOAT3 upperBodyPos )
		{
			mLowerBodyPos	= lowerBodyPos;
			mUpperBodyPos	= upperBodyPos;
		}
		XMFLOAT3 LowerBodyPos()const
		{
			return mLowerBodyPos;
		}
		XMFLOAT3 UpperBodyPos()const
		{
			return mUpperBodyPos;
		}
	protected:
		const EventType& GetEventType( void ) const
		{
			return GUID;
		}
};

class Event_Remote_Player_Update : public IEvent
{
private:
	XMFLOAT3 mLowerBodyPos;
	XMFLOAT3 mUpperBodyPos;

protected:
public:
	static const EventType GUID;

private:
public:
	Event_Remote_Player_Update(XMFLOAT3 lowerBodyPos, XMFLOAT3 upperBodyPos)
	{
		mLowerBodyPos = lowerBodyPos;
		mUpperBodyPos = upperBodyPos;
	}
	XMFLOAT3 LowerBodyPos()const
	{
		return mLowerBodyPos;
	}
	XMFLOAT3 UpperBodyPos()const
	{
		return mUpperBodyPos;
	}
protected:
	const EventType& GetEventType(void) const
	{
		return GUID;
	}
};