/*
	Copyright (c) oahosz
	All rights reserved.


	filename   : TimeWheel.h
	tip        : 实现了一个简单的时间轮。
	date	   : 2020/11/28
	version	   : v3.0
	author     : oaho
	descrition : 时间轮分秒轮,分轮,时轮.分层的时间轮设计.当然还有另外一种设计就是单单一个时间轮，每个任务中
				 有个rounds属性,比如,一个任务在时间轮启动后的78秒后执行,秒轮的刻度为1秒，那么要等它转了一圈
				 之后,在8秒这个刻度去执行,rounds属性为1,当指针转到此刻的时候,rounds-1,rounds=0的时候,丢入到
				 时间轮。
				 另外本文件的时间轮设计方法,也有多的抉择,例如在秒指针到达某一个刻度的时候,需要在这把任务丢到
				 线程池里执行, 那么在放置任务的时候本身具有时间消耗,减少误差的方式其一就是计算任务放置过程的
				 时间,之后在加上去。再者就是到达这个刻度的时候,用一个线程池去添加任务,这样也可以减少时间误差，
				 且处理起来相对简单.
*/


#pragma once
#ifndef __SCHDELETASK_H__
#define __SCHDELETASK_H__

#include<array>
#include<list>
#include<mutex>
#include"threadpool.h"
#include"Task.h"
#include"Timer.h"
#include<atomic>
using namespace std;

namespace oaho
{
	class TimeWheel final
	{
		template<int quarter>
		using Wheel = array<list<oaho::Task>, quarter>;

	public:

		TimeWheel();

		/* 用于添加任务 */
		void addTask(unsigned int delay, bool loop, oaho::Task t);

		/* 用于启动和停止时间轮 */
		void run();
		void stop();

	public:
		static const int SEC = 61;
		static const int MIN = 61;
		static const int HOUR = 25;
	private:
		Wheel<SEC>									sec_w;			/* 秒轮 */
		Wheel<MIN>									min_w;			/* 分轮 */
		Wheel<HOUR>									hour_w;			/* 时轮 */
		std::shared_ptr<Timer>						time_pointer;	/* 刻度指针 */
		
		std::atomic<int>							saved_sec_w;	/* 记录秒针的位置 */
		std::atomic<int>							saved_min_w;	/* 记录分针的位置 */
		std::atomic<int>							saved_hour_w;	/* 记录时针的位置 */

		std::atomic<bool>							init_completed;	/* 标志时间轮是否启动 */
		std::mutex									mtx;		    /* 用于添加任务的锁 */
		std::mutex									mtx_init;		/* 用于初始化进度的锁 */
		std::condition_variable						notify_init;	/* 通知run方法可以退出 */

	};




	class ScheduleTask final
	{



	private:
		TimeWheel wheel;
	};
}






#endif // !__SCHDELETASK_H__
