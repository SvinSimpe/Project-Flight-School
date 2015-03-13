#include "HeadLineEntry.h"

HeadLineEntry::HeadLineEntry( float displayTime )
{
	mFadeTime	= 3.0f;
	mFade		= 1.0f;
}

HeadLineEntry::HeadLineEntry( const HeadLineEntry &origOb )
{
	mTeamID		= origOb.mTeamID;
	mActionID	= origOb.mActionID;
	mGoalID		= origOb.mGoalID;
	mTime		= origOb.mTime;
	mFade		= origOb.mFade;
	mFadeTime	= origOb.mFadeTime;
}

HeadLineEntry::~HeadLineEntry()
{
}

bool HeadLineEntry::TimeOut( float time )
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
