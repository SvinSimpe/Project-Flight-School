#ifndef TIMER_H
#define TIMER_H

#include <Windows.h>

class Timer
{
	private:
		__int64		mCntsPerSec;
		double		mSecsPerCnt;
		__int64		mPrevTimeStamp;
		float		mFps;

	protected:
	public:

	private:
	protected:
	public:
		float	GetDeltaTime();
		float		GetFPS();

		HRESULT	Initialize();
		void	Release();
				Timer();
		virtual	~Timer();
};
#endif