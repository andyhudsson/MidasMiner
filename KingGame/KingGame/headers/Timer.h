#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>
#include <iostream>

class Timer
{
	Uint32 _time;
	Uint32 _startTime;
	Uint32 _leftTime;

	char _strTime[10];

public:

	Timer();
	~Timer();

	bool Finished();

	Uint32 GetTime();

	char * GetTimeStr();

	void SetTime(Uint32 time);

	void Start();

	void Update();
};

#endif
