#include "BattleLogEntry.h"

BattleLogEntry::BattleLogEntry( float time )
{
	mTime		= time;
	mFade		= 1.0f;
	mFadeTime	= 3.0f;
}

BattleLogEntry::BattleLogEntry( const BattleLogEntry &origOb )
{
	mTime			= origOb.mTime;
	mFirstUnitID	= origOb.mFirstUnitID;
	mActionID		= origOb.mActionID;
	mSecondUnitID	= origOb.mSecondUnitID;
	mFade			= origOb.mFade;
	mFadeTime		= origOb.mFadeTime;
}	

BattleLogEntry::~BattleLogEntry()
{
}

bool BattleLogEntry::TimeOut( float time )
{
	mTime -= time;
	if( mTime <= 3.0f )
	{
		mFade = (float)( mTime / mFadeTime );
		if( mTime <= 0.0f )
			return false;
	}
	return true;
}
