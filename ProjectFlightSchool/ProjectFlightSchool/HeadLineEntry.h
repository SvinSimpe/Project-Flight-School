#ifndef HEADLINEENTRY_H
#define HEADLINEENTRY_H

#pragma once
#include "IEventManager.h"
#include <string>

class HeadLineEntry
{
	private:
		float mTime;
	protected:
	public:
		int mTeamID;
		int mActionID;
		int mGoalID;

		float mFadeTime;
		float mFade;
	
	private:
	protected:
	public:
		HeadLineEntry( float displayTime = 4.0f );
		HeadLineEntry( const HeadLineEntry &origOb );
		~HeadLineEntry();
		
		bool TimeOut( float time );
};
#endif