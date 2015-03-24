#include "BattleLog.h"

BattleLog::BattleLog()
{
	mFont = nullptr;

	mRedTeam		= DirectX::XMFLOAT3( 1.0f, 0.0f, 0.0f );
	mBlueTeam		= DirectX::XMFLOAT3( 0.0f, 0.0f, 1.0f );
	mCyanBlue		= DirectX::XMFLOAT3( 0.0f, 0.55f, 0.85f );
	mActionColor	= DirectX::XMFLOAT3( 1.0f, 1.0f, 1.0f );
}

BattleLog::BattleLog( const BattleLog &origOb )
{
}

BattleLog::~BattleLog()
{
}

void BattleLog::SetUpPlayers( Player* player, std::vector<RemotePlayer*>& remotePlayers )
{
	mPlayerMap[player->GetID()] = PlayerInfo( player->GetName(), player->GetTeam() );
	mPlayer = player;
	
	for( auto it : remotePlayers )
	{
		mPlayerMap[it->GetID()] = PlayerInfo( it->GetName(), it->GetTeam() );
	}
}

void BattleLog::Update( float deltaTime )
{
	auto it = mLogQueue.begin();
	while( it != mLogQueue.end() )
	{
		if( !it->TimeOut( deltaTime ) )
		{
			it = mLogQueue.erase( it );
		}
		else
		{
			it++;
		}
	}

	auto it2 = mHeadLineQueue.begin();
	if( !mHeadLineQueue.empty() )
	{
		if( !it2->TimeOut( deltaTime ) )
		{
			it2 = mHeadLineQueue.erase( it2 );
		}
	}

}

void BattleLog::Render()
{
	float height	= (float)Input::GetInstance()->mScreenHeight;
	float width		= (float)Input::GetInstance()->mScreenWidth;

	float offset = height * 0.03f;
	DirectX::XMFLOAT2 topLeft = XMFLOAT2( width * 0.05f, height * 0.05f );
	DirectX::XMFLOAT2 middleLeft = XMFLOAT2( width * 0.5f, height * 0.2f );

	float scale = 2.0f;
	float headLineScale = 3.0f;
	float x1, x2;

	DirectX::XMFLOAT3 first = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	DirectX::XMFLOAT3 second = XMFLOAT3( 1.0f, 1.0f, 1.0f );

	std::string writeString;

	for( UINT i = 0; i < mLogQueue.size() && i < LOG_QUEUE_DISPLAY_LIMIT; i++ )
	{
		BattleLogEntry* current = &mLogQueue[i];

		mActionString		= mActions[current->mActionID];

		if( mPlayerMap.count( current->mFirstUnitID ) == 1 )
		{
			mFirstUnitString	= mPlayerMap[current->mFirstUnitID].mName + " ";
			first				= mTeams[1].color;

			if( mPlayer->GetTeam() != mPlayerMap[current->mFirstUnitID].mTeamID )
			{

				mFirstUnitString	= mPlayerMap[current->mFirstUnitID].mName + " ";
				first				= mTeams[2].color;
			}
		}

		else if( current->mSecondUnitID == -1 )
		{
			mSecondUnitString = "";
		}

		else
		{
			mFirstUnitString	= mTeams[0].teamName;
			first				= mTeams[0].color;
		}

		if( mPlayerMap.count( current->mSecondUnitID ) == 1 )
		{
			mSecondUnitString	= mPlayerMap[current->mSecondUnitID].mName + " ";
			second				= mTeams[1].color;
			if( mPlayer->GetTeam() != mPlayerMap[current->mSecondUnitID].mTeamID )
			{
				mSecondUnitString	= mPlayerMap[current->mSecondUnitID].mName + " ";
				second				= mTeams[2].color;
			}
		}

		else if( current->mSecondUnitID == -1 )
		{
			mSecondUnitString = "";
		}

		else
		{
			mSecondUnitString	= mTeams[0].teamName;
			second				= mTeams[0].color;
		}

		x1 = mFont->GetMiddleXPoint( mFirstUnitString, scale );
		x2 = mFont->GetMiddleXPoint( mActionString, scale );

		mFont->WriteText( mFirstUnitString, topLeft.x, topLeft.y + offset * i, 2.0f, XMFLOAT4( first.x, first.y, first.z, current->mFade ) );
		mFont->WriteText( mActionString, topLeft.x + x1 * 2.0f, topLeft.y + offset * i, 2.0f, XMFLOAT4( 1.0f, 1.0f, 1.0f, current->mFade ) );
		mFont->WriteText( mSecondUnitString, ( topLeft.x + x1 * 2.0f ) + x2 * 2.0f, topLeft.y + offset * i, 2.0f, XMFLOAT4( second.x, second.y, second.z, current->mFade ) );
	}

	if( !mHeadLineQueue.empty() )
	{
		HeadLineEntry* current = &mHeadLineQueue.front();

		if( current->mActionID == 6 )
		{
			mFirstUnitString	= mObjects[current->mGoalID];
			first				= mCyanBlue;
			mActionString		= mActions[current->mActionID];
			mSecondUnitString	= "";
		}
		else
		{
			if( current->mTeamID == mPlayer->GetTeam() )
			{
				mFirstUnitString	= mTeams[1].teamName;
				first				= mTeams[1].color;
			}
			else
			{
				mFirstUnitString	= mTeams[2].teamName;
				first				= mTeams[2].color;
			}
			mActionString		= mActions[current->mActionID];
			mSecondUnitString	= mObjects[current->mGoalID];
			second				= mCyanBlue;
		}
		
		mMeasureString = mFirstUnitString + mActionString + mSecondUnitString;

		float offsetX = middleLeft.x - mFont->GetMiddleXPoint( mMeasureString, headLineScale );
		x1 = mFont->GetMiddleXPoint( mFirstUnitString, headLineScale );
		x2 = mFont->GetMiddleXPoint( mActionString, headLineScale );

		mFont->WriteText( mFirstUnitString, offsetX, middleLeft.y, headLineScale, XMFLOAT4( first.x, first.y, first.z, current->mFade ) );
		mFont->WriteText( mActionString, ( offsetX + x1 * 2.0f ), middleLeft.y, headLineScale, XMFLOAT4( 1.0f, 1.0f, 1.0f, current->mFade ) );
		mFont->WriteText( mSecondUnitString, ( offsetX + x1 * 2.0f ) + x2 * 2.0f, middleLeft.y, headLineScale, XMFLOAT4( second.x, second.y, second.z, current->mFade ) );
	}
}

HRESULT BattleLog::Initialize()
{
	if( !mFont )
	{
		mFont = new Font();
		mFont->Initialize( "../Content/Assets/GUI/Fonts/final_font/" );
	}

	//Define Actions---------
	mActions[KILLED]			= "KILLED ";
	mActions[REVIVED]			= "REVIVED ";
	mActions[DOWNED]			= " WAS DOWNED!";
	mActions[CELL_PICKED_UP]	= "PICKED UP THE ";
	mActions[DROPPED]			= "DROPPED THE ";
	mActions[CAPTURED]			= "CAPTURED THE ";
	mActions[SPAWNED]			= " HAS SPAWNED!";

	//Define Objects---------
	mObjects[ENERGY_CELL]	= "ENERGY CELL";

	//Define Teams----------
	mTeams[0].teamName	= "WORLD ";
	//mTeams[0].color		= XMFLOAT3( 0.0f, 0.102f, 0.0f );
	mTeams[0].color		= XMFLOAT3( 0.0f, 0.4f, 0.0f );

	mTeams[1].teamName	= "YOUR TEAM ";
	//mTeams[1].color		= XMFLOAT3( 0.214f, 0.167f, 0.240f );
	mTeams[1].color		= XMFLOAT3( COLOR_CYAN.x, COLOR_CYAN.y, COLOR_CYAN.z );
	//mTeams[1].color		= XMFLOAT3( 1.0f, 0.94f, 0.12f );

	//MY TEAM 1, ENMEMY TEAM 2

	mTeams[2].teamName	= "ENEMY TEAM ";
	//mTeams[2].color		= XMFLOAT3( 0.204f, 0.0f, 0.0f );
	//mTeams[2].color		= XMFLOAT3( 0.8f, 0.0f, 0.0f );
	mTeams[2].color		= XMFLOAT3( COLOR_RED.x, COLOR_RED.y, COLOR_RED.z );

	EventManager::GetInstance()->AddListener( &BattleLog::OnHeadlineEvent, this, Event_Server_Headline_Event::GUID );
	EventManager::GetInstance()->AddListener( &BattleLog::OnPlayerActionEvent, this, Event_Remote_Log_Event::GUID );
	EventManager::GetInstance()->AddListener( &BattleLog::OnServerSetName, this, Event_Remote_Set_Name::GUID );

	return S_OK;
}

void BattleLog::Release()
{
	SAFE_RELEASE_DELETE( mFont );
}

void BattleLog::AddLogEntry( BattleLogEntry entry )
{
	if( mLogQueue.size() < LOG_QUEUE_SIZE )
	{
		mLogQueue.push_back( entry );
	}
}

void BattleLog::AddHeadLineEntry( HeadLineEntry entry )
{
	if( mHeadLineQueue.size() < HEADLINE_QUEUE_SIZE )
	{
		if( !mHeadLineQueue.empty() )
		{
			mHeadLineQueue.pop_back();
		}
		mHeadLineQueue.push_back( entry );
	}
}

void BattleLog::OnPlayerActionEvent( IEventPtr e )
{
	if ( e->GetEventType() == Event_Remote_Log_Event::GUID )
	{
		std::shared_ptr<Event_Remote_Log_Event> data = std::static_pointer_cast<Event_Remote_Log_Event>( e );
		BattleLogEntry temp;
		
		temp.mFirstUnitID	= data->GetFirstUnitID();

		temp.mActionID		= data->GetActionID();

		temp.mSecondUnitID	= data->GetSecondUnitID();
		
		AddLogEntry( temp );
	}
}

void BattleLog::OnHeadlineEvent( IEventPtr e )
{
	if ( e->GetEventType() == Event_Server_Headline_Event::GUID )
	{
		std::shared_ptr<Event_Server_Headline_Event> data = std::static_pointer_cast<Event_Server_Headline_Event>( e );

		HeadLineEntry temp;

		temp.mTeamID	= data->GetTeamID();
		temp.mActionID	= data->GetActionID();
		temp.mGoalID	= data->GetGoalID();

		AddHeadLineEntry( temp );
	}
}

void BattleLog::OnServerSetName( IEventPtr e )
{
	if ( e->GetEventType() == Event_Remote_Set_Name::GUID )
	{
		std::shared_ptr<Event_Remote_Set_Name> data = std::static_pointer_cast<Event_Remote_Set_Name>( e );

		if( mPlayerMap.count( data->ID() ) == 1 )
			mPlayerMap[data->ID()].mName = data->Name();
	}
}

void BattleLog::Reset()
{
	mLogQueue.clear();
	mHeadLineQueue.clear();
}