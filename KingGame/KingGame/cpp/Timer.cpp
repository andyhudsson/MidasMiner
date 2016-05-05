
#include "Timer.h"

Timer::Timer() :_time(60), 
				_startTime(SDL_GetTicks()), 
				_leftTime(_time)
{}

Timer::~Timer()
{}

bool Timer::Finished()
{
	return _leftTime == 0;
}

char * Timer::GetTimeStr()
{
	_itoa_s(_leftTime, _strTime, 10, 10);
	return _strTime;
}

void Timer::Start()
{
	_startTime = SDL_GetTicks();
}

void Timer::SetTime(Uint32 time)
{
	_time = time;
	_leftTime = time;
	_startTime = SDL_GetTicks();
}

Uint32 Timer::GetTime()
{
	return _leftTime;
}

void Timer::Update()
{
	_leftTime = _time - (SDL_GetTicks() - _startTime) / 1000;

	if (_leftTime < 0)
		_leftTime = 0;
}
