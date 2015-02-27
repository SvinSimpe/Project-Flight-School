#include "Timer.h"

float Timer::GetDeltaTime()
{
	__int64 currTimeStamp = 0;
	QueryPerformanceCounter( (LARGE_INTEGER*)&currTimeStamp );

	float dt = (float)( ( currTimeStamp - mPrevTimeStamp ) * mSecsPerCnt );
	
	mPrevTimeStamp = currTimeStamp;

	

	mFps = mFps * 0.1f + 0.9f / dt;

	return dt;
}

float Timer::GetFPS()
{
	return mFps;
}

HRESULT Timer::Initialize()
{
	mCntsPerSec = 0;
	QueryPerformanceFrequency( (LARGE_INTEGER*)&mCntsPerSec );

	mSecsPerCnt = 1.0 / (double)mCntsPerSec;

	mPrevTimeStamp = 0;
	QueryPerformanceCounter( (LARGE_INTEGER*)&mPrevTimeStamp );

	mFps = 60.0f;
	
	return S_OK;
}

void Timer::Release()
{

}

Timer::Timer()
{
	mCntsPerSec		= 0;
	mSecsPerCnt		= 0.0f;
	mPrevTimeStamp	= 0;
	mFps			= 0.0f;
}

Timer::~Timer()
{
}