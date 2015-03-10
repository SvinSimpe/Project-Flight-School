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

EventType counter = 0;

const EventType Event_Exit_Game::GUID( counter++ );
const EventType Event_Toggle_Fullscreen::GUID( counter++ );
const EventType Event_Client_Joined::GUID( counter++ );
const EventType Event_Client_Left::GUID( counter++ );
const EventType Event_Local_Joined::GUID( counter++ );
const EventType Event_Remote_Joined::GUID( counter++ );
const EventType Event_Remote_Left::GUID( counter++ );
const EventType Event_Client_Update::GUID( counter++ );
const EventType Event_Remote_Update::GUID( counter++ );
const EventType Event_Change_State::GUID( counter++ );
const EventType Event_Start_Server::GUID( counter++ );
const EventType Event_Start_Client::GUID( counter++ );
const EventType Event_Game_Started::GUID( counter++ );
const EventType Event_Game_Ended::GUID( counter++ );
const EventType Event_Client_Died::GUID( counter++ );
const EventType Event_Remote_Died::GUID( counter++ );
const EventType Event_Client_Removed_Projectile::GUID( counter++ );
const EventType Event_Remote_Removed_Projectile::GUID( counter++ );
const EventType Event_Client_Damaged::GUID( counter++ );
const EventType Event_Remote_Damaged::GUID( counter++ );
const EventType Event_Client_Spawned::GUID( counter++ );
const EventType Event_Remote_Spawned::GUID( counter++ );
const EventType Event_Client_Fired_Projectile::GUID( counter++ );
const EventType Event_Remote_Fired_Projectile::GUID( counter++ );
const EventType Event_Client_Update_HP::GUID( counter++ );
const EventType Event_Remote_Update_HP::GUID( counter++ );
const EventType Event_Server_Create_Enemy::GUID( counter++ );
const EventType Event_Server_Enemies_Created::GUID( counter++ );
const EventType Event_Client_Melee_Hit::GUID( counter++ );
const EventType Event_Remote_Melee_Hit::GUID( counter++ );
const EventType Event_Client_Attack::GUID( counter++ );
const EventType Event_Remote_Attack::GUID( counter++ );
const EventType Event_Server_Sync_Spawn::GUID( counter++ );
const EventType Event_Add_Point_Light::GUID( counter++ );
const EventType Event_Remove_Point_Light::GUID( counter++ );
const EventType Event_Server_Update_Enemy::GUID( counter++ );
const EventType Event_Server_Sync_Enemy_State::GUID( counter++ );
const EventType Event_Set_Enemy_State::GUID( counter++ );
const EventType Event_Remote_Set_Enemy_State::GUID( counter++ );
const EventType Event_Client_Projectile_Damage_Enemy::GUID( counter++ );
const EventType Event_Server_Enemy_Died::GUID( counter++ );
const EventType Event_Server_Enemy_Attack_Player::GUID( counter++ );
const EventType Event_Client_Down::GUID( counter++ );
const EventType Event_Remote_Down::GUID( counter++ );
const EventType Event_Client_Up::GUID( counter++ );
const EventType Event_Remote_Up::GUID( counter++ );
const EventType Event_Client_Attempt_Revive::GUID( counter++ );
const EventType Event_Remote_Attempt_Revive::GUID( counter++ );
const EventType Event_Load_Level::GUID( counter++ );
const EventType Event_Create_Player_Name::GUID( counter++ );
const EventType Event_Client_Enemy_Attack::GUID( counter++ );
const EventType Event_Remote_Enemy_Attack::GUID( counter++ );
const EventType Event_Shutdown_Server::GUID( counter++ );
const EventType Event_Shutdown_Client::GUID( counter++ );
const EventType Event_Reset_Game::GUID( counter++ );
const EventType Event_Connect_Server_Success::GUID( counter++ );
const EventType Event_Connect_Client_Success::GUID( counter++ );
const EventType Event_Connect_Server_Fail::GUID( counter++ );
const EventType Event_Connect_Client_Fail::GUID( counter++ );
const EventType Event_Server_Spawn_Ship::GUID( counter++ );
const EventType Event_Server_Change_Buff_State::GUID( counter++ );
const EventType Event_Server_Sync_Energy_Cell::GUID( counter++ );
const EventType Event_Client_Sync_Energy_Cell::GUID( counter++ );
const EventType Event_Tell_Server_Enemy_Attack_Player::GUID( counter++ );
const EventType Event_Client_Update_Ship::GUID( counter++ );
const EventType Event_Server_Update_Ship::GUID( counter++ );
const EventType Event_Client_Change_Ship_Levels::GUID( counter++ );
const EventType Event_Server_Change_Ship_Levels::GUID( counter++ );
const EventType Event_Remote_Win::GUID( counter++ );
const EventType Event_Client_Win::GUID( counter++ );
const EventType Event_Upgrade_Player::GUID( counter++ );
const EventType Event_Request_Player_Spawn_Position::GUID( counter++ );
const EventType Event_New_Player_Spawn_Position::GUID( counter++ );
const EventType Event_Server_XP::GUID( counter++ );
const EventType Event_XP::GUID( counter++ );
const EventType Event_Server_Update_Turret::GUID( counter++ );
const EventType Event_Turret_Fired_Projectile::GUID( counter++ );
const EventType Event_Server_Turret_Fired_Projectile::GUID( counter++ );
const EventType Event_Client_Initialize_LobbyPlayer::GUID( counter++ );
const EventType Event_Server_Initialize_LobbyPlayer::GUID( counter++ );
const EventType Event_Client_Lobby_Finished::GUID( counter++ );
const EventType Event_Server_Lobby_Finished::GUID( counter++ );
const EventType Event_Client_Switch_Team::GUID( counter++ );
const EventType Event_Server_Switch_Team::GUID( counter++ );
const EventType Event_Trigger_Client_Fired_Projectile::GUID( counter++ );
const EventType Event_Trigger_Client_Update::GUID( counter++ );
const EventType Event_Unlock_Player::GUID( counter++ );
const EventType Event_Client_Reach_Server::GUID( counter++ );
const EventType Event_Server_Reach_Client::GUID( counter++ );
const EventType Event_Change_Weapon::GUID( counter++ );
const EventType Event_Client_Change_Weapon::GUID( counter++ );
const EventType Event_Server_Change_Weapon::GUID( counter++ );
const EventType Event_Reset_Turret_Targets::GUID( counter++ );
const EventType Event_Client_Change_Ready_State::GUID( counter++ );
const EventType Event_Server_Change_Ready_State::GUID( counter++ );
const EventType Event_Server_Start_Game_Countdown::GUID( counter++ );
const EventType Event_Host_Start_Game_Countdown::GUID( counter++ );
const EventType Event_Enemy_Fired_Projectile::GUID(counter++);
const EventType Event_Server_Enemy_Fired_Projectile::GUID(counter++);