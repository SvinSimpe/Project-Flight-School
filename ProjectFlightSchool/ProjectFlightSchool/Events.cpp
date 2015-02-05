//========================================================================
// Events.cpp : defines common game events
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
//    http://code.google.com/p/gamecode4/6
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

#include "Events.h"

// To define a new event - you need a 32-bit GUID. 
// In Visual Studio, go to Tools->Create GUID and grab the first bit.
const EventType Event_Player_Update::GUID( 0 );
const EventType Event_Remote_Player_Update::GUID( 1 );
const EventType Event_Local_Player_Joined::GUID( 2 );
const EventType Event_Remote_Player_Joined::GUID( 3 );
const EventType Event_Remote_Player_Left::GUID( 4 );
const EventType Event_Change_State::GUID( 5 );
const EventType Event_Start_Server::GUID( 6 );
const EventType Event_Game_Started::GUID( 7 );
const EventType Event_Game_Ended::GUID( 8 );
const EventType Event_Start_Client::GUID( 9 );
const EventType Event_Player_Died::GUID( 10 );
const EventType Event_Remote_Player_Died::GUID( 11 );
const EventType Event_Player_Damaged::GUID( 12 );
const EventType Event_Remote_Player_Damaged::GUID( 13 );
const EventType Event_Player_Spawned::GUID( 14 );
const EventType Event_Remote_Player_Spawned::GUID( 15 );
const EventType Event_Projectile_Fired::GUID( 16 );
const EventType Event_Remote_Projectile_Fired::GUID( 17 );
const EventType Event_Player_Update_HP::GUID( 18 );
const EventType Event_Remote_Player_Update_HP::GUID( 19 );
const EventType Event_Sync_Enemy::GUID( 20 );
const EventType Event_Enemy_List_Synced::GUID( 21 );
const EventType Event_Server_Initialized::GUID( 22 );
const EventType Event_Connection_Failed::GUID( 23 );
const EventType Event_Player_Melee_Hit::GUID( 24 );
const EventType Event_Remote_Player_Melee_Hit::GUID( 25 ); 
const EventType Event_Sync_Spawn::GUID( 26 );
const EventType Event_Update_Enemy_Position::GUID( 27 );
const EventType	Event_Add_Point_Light::GUID( 28 );
const EventType	Event_Remove_Point_Light::GUID( 29 );
const EventType Event_Sync_Enemy_Type::GUID( 30 );
const EventType Event_Sync_Enemy_State::GUID( 31 );
const EventType Event_Player_Attack::GUID( 32 );
const EventType Event_Remote_Player_Attack::GUID( 33 );
const EventType Event_Set_Enemy_State::GUID( 34 );
const EventType Event_Set_Remote_Enemy_State::GUID( 35 );
const EventType Event_Projectile_Damage_Enemy::GUID( 36 );
const EventType Event_Enemy_Died::GUID( 37 );
const EventType Event_Enemy_Attack_Player::GUID( 38 );
const EventType Event_Remote_Player_Down::GUID( 39 );
const EventType Event_Player_Down::GUID( 40 );
const EventType Event_Remote_Player_Up::GUID( 41 );
const EventType Event_Player_Up::GUID( 42 );
const EventType Event_Remote_Player_Revive::GUID( 43 );
const EventType Event_Player_Revive::GUID( 44 );