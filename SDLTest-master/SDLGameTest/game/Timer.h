#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <SDL.h>
#include <SDL_image.h>
#elif defined(__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#endif

class Timer
{
public:
	Timer();
	void start();
	float getDeltaTime();
	Uint32 getTimeElapsed() const;
	void restart();
	void pause();
	void resume();
	bool isStarted() const;
	bool isPaused() const;

private:
	Uint32 mStartTime, mPausedTime, mLastTime;
	bool mStarted, mPaused;
};
