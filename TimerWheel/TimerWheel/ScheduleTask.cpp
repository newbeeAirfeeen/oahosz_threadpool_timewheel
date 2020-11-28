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

#include"ScheduleTask.h"


oaho::TimeWheel::TimeWheel()
{
	saved_sec_w = 0;
	saved_min_w = 0;	
	saved_hour_w = 0;
	time_pointer = std::make_shared<Timer>();
	init_completed = false;
}


void oaho::TimeWheel::addTask(unsigned int delay, bool loop, oaho::Task t)
{

}



void oaho::TimeWheel::run()
{
	std::chrono::duration<int, std::milli> span{ 1000 };		/* 每一秒执行一次 */
																/* 用于启动定时器 */
	time_pointer->start(
		[this] 
		{
			/* 线程创建完成后，通知run方法可以退出 */
			/* 因为线程创建本身需要时间 */
			if (!init_completed.load())
			{
				init_completed.store(true);
				notify_init.notify_one();
			}
			/* 拨动秒指针 */
			/* 这里这么写是为了简化代码 */
			saved_sec_w++;
			if (saved_sec_w == 60)
			{
				saved_sec_w = 0; 
				saved_min_w++;

			}
		}, 
	span);    /* time_pointer */
	std::unique_lock<std::mutex> mtx_lock{ mtx_init };
	notify_init.wait(mtx_lock, [this] { return init_completed.load(); });
}
void oaho::TimeWheel::stop()
{
	/* 停止刻度的转动 */
	time_pointer->cancel();
}


