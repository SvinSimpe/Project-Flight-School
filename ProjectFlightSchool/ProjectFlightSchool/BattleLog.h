#ifndef BATTLELOG_H
#define BATTLELOG_H

#pragma once
#include "IEventManager.h"
#include <vector>
#include "BattleLogEntry.h"
#include "HeadLineEntry.h"
#include "Text.h"
#include "Player.h"

#define LOG_QUEUE_SIZE 20
#define LOG_QUEUE_DISPLAY_LIMIT 5
#define HEADLINE_QUEUE_SIZE 20


static enum Actions
{
	//Player Actions
	KILLED,
	REVIVED,
	DOWNED,

	//Game Actions
	CELL_PICKED_UP,
	DROPPED,
	CAPTURED,
	SPAWNED,

	NR_OF_ACTIONS
};

static enum Objects
{
	ENERGY_CELL,

	NR_OF_OBJECTS
};

struct PlayerInfo
{
	std::string mName;
	int mTeamID;
	PlayerInfo()
	{
		mName = "";
		mTeamID	= 0;
	}
	PlayerInfo( std::string name, int teamID )
	{
		mName = name;
		mTeamID = teamID;
	}
};

struct TeamInfo
{
	DirectX::XMFLOAT3 color;
	std::string teamName;
};
class BattleLog
{
	private:

		std::string mActions[NR_OF_ACTIONS];
		std::string mObjects[NR_OF_OBJECTS];
		TeamInfo mTeams[3];
		std::unordered_map< int, PlayerInfo > mPlayerMap;

		std::vector< BattleLogEntry > mLogQueue;
		std::list< HeadLineEntry > mHeadLineQueue;
		Font* mFont;
		int mTeamID;

		DirectX::XMFLOAT3 mRedTeam;
		DirectX::XMFLOAT3 mBlueTeam;
		DirectX::XMFLOAT3 mCyanBlue;
		DirectX::XMFLOAT3 mGreen;
		DirectX::XMFLOAT3 mActionColor;

		std::string mFirstUnitString;
		std::string mActionString;
		std::string mSecondUnitString;

		std::string mMeasureString;

	protected:
	public:
	
	private:
	protected:
	public:

		void SetUpPlayers( Player* player, std::vector<RemotePlayer*>& remotePlayers );
		void Update( float deltaTime );
		void Render();
		HRESULT Initialize();
		void Release();

		void AddLogEntry( BattleLogEntry entry );
		void AddHeadLineEntry( HeadLineEntry entry );

		void OnPlayerActionEvent( IEventPtr e );
		void OnHeadlineEvent( IEventPtr e );
		void OnServerSetName( IEventPtr e );

		void Reset();

		BattleLog();
		BattleLog( const BattleLog &origOb );
		~BattleLog();
};
#endif

