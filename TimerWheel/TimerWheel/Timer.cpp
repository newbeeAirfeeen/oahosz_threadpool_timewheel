/*
	Copyright (c) 2020 hzsz
	All rights reserved.

	filename : Timer.h
	tip      : design a timer. span a time to excute task.

	version	 : 1.0
	author   : oohao
	date     : 2020/11/18
	descripttion : every a time span, timer can excute task for while.
				   the Timer::stopping pre the timer whether to stop or run. stopping = 1,
				   timer stop. stoppign = 0,continue.

*/
#include"Timer.h"


Timer::Timer() :p_thread{nullptr},
				stopping{ false }, 
				isRunning{ false }
{
	
}

Timer::	~Timer()
{
	cancel();
}


void Timer::start(Task t, std::chrono::duration<int, std::milli> span)
{
	p_thread = std::make_shared<std::thread>([=]{
			while (true)
			{
				this->isRunning = true;
				if (this->isStopping())
					break;
				std::this_thread::sleep_for(span);
				t();
			}
		});
}

void Timer::cancel()
{
	stopping.store(true);
	if (p_thread != nullptr && p_thread->joinable())
	{
		p_thread->join();
	}
}

bool Timer::isStopping()
{
	return stopping;
}

bool Timer::threadisRunning()
{
	return isRunning;
}


