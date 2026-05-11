#include "Utils\Stopwatch.h"
#include <SDL.h>
#include <SDL_stdinc.h>

Stopwatch::Stopwatch()
{
	mStartTicks = 0;
	mPausedTicks = 0;

	mPaused = false;
	mStarted = false;
}

void Stopwatch::start()
{
	mStarted = true;
	mPaused = false;

	mStartTicks = SDL_GetTicks();	
	mPausedTicks = 0;
}
void Stopwatch::stop()
{
	mStarted = false;
	mPaused = false;

	mStartTicks = 0;
	mPausedTicks = 0;
}
void Stopwatch::pause()
{
	if (mStarted && !mPaused)
	{
		mPaused = true;

		mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
	}
}
void Stopwatch::unpause()
{
	if (mStarted && mPaused)
	{
		mPaused = false;

		mStartTicks = SDL_GetTicks() - mPausedTicks;
		mPausedTicks = 0;
	}
}

Uint32 Stopwatch::getTicks()
{
	Uint32 time = 0;

	if (mStarted)
	{
		if (mPaused)
		{
			time = mPausedTicks;
		}
		else
		{
			time = SDL_GetTicks() - mStartTicks;
		}
	}

	return time;
}

bool Stopwatch::isStarted()
{
	return mStarted;
}

bool Stopwatch::isPaused()
{
	return mPaused && mStarted;
}