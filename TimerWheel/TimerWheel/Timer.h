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

#pragma once
#ifndef __TIMER_H_
#define __TIMER_H_
#include<atomic>
#include<mutex>
#include<chrono>
#include<iostream>
#include<condition_variable>
#include<functional>

class Timer
{
	using Task = std::function<void()>;
public:
	Timer();
	~Timer();
	Timer(Timer&&) = delete;
	void start(Task t, std::chrono::duration<int, std::milli> span);     /* start task */
	void cancel();														 /* cancel a task */
	bool isStopping();													 /* whether stopping */
	bool threadisRunning();
private:
	std::shared_ptr<std::thread>		 p_thread;
	std::atomic<bool>	                 stopping;						 /* stop this timer */
	std::mutex				             mtx;
	std::atomic<bool>					 isRunning;
	std::atomic<bool>				     isTerminate{ false };
	std::condition_variable              is_quit;
};



#endif //__TIMER_H_