#ifndef BATTLELOGENTRY_H
#define BATTLELOGENTRY_H
#pragma once

#include "IEventManager.h"
#include <string>

class BattleLogEntry
{
	private:
		float mTime;
	protected:
	public:
		int mFirstUnitID;

		int mActionID;

		int mSecondUnitID;

		float mFadeTime;
		float mFade;

	private:
	protected:
	public:
		BattleLogEntry( float time = 8.0f );
		BattleLogEntry( const BattleLogEntry &origOb );
		~BattleLogEntry();

		bool TimeOut( float time );
};
#endif