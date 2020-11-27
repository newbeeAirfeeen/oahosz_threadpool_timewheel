/*
	Copyright (c) oahosz
	All rights reserved.


	filename   : TimeWheel.h
	tip        : 实现了一个简单的时间轮。
	date	   : 2020/11/27
	version	   : v2.0
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

#include"TimeWheel.h"
std::mutex mutex;
//###################### MinuteTimeWheel #####################
MinutesTimeWheel::MinutesTimeWheel()
{

}
MinutesTimeWheel::~MinutesTimeWheel()
{

}
void MinutesTimeWheel::addTask(oaho::Task t, unsigned int position, bool loop)
{

}

void MinutesTimeWheel::run()
{

}


//######################  SecondsTimeWheel ####################
SecondsTimeWheel::SecondsTimeWheel(unsigned int quarter)
{
	max_quarter = quarter;
	timeWheel = std::make_shared<MinutesTimeWheel>();			/* 下一个指针是分轮 */
	quarter_task.resize(quarter);							    /* 调整一个轮的最大刻度 */
	quarter_pointer = 0;
	completed = false;
}
SecondsTimeWheel::~SecondsTimeWheel()
{
	/* 暂停刻度的拨动 */
	timer->cancel();
}
void SecondsTimeWheel::addTask(oaho::Task t, unsigned int position, bool loop)
{		
	t.setExcuteTime(position);
	t.setloop(loop);
	if (position >= max_quarter)								/* 如果大于当前刻度,则提层 */
	{
		timeWheel->addTask(t, position, loop);					/* 通知分轮来添加任务 */
		return;
	}
	/* 同步添加任务 */
	std::unique_lock < std::mutex> mtx_lock{ mutex };
	int poi = (position + quarter_pointer) % max_quarter;
	std::cout << poi << std::endl;
	quarter_task[poi].emplace_back(std::move(t));		/* 当前刻度 + 指定刻度 */
}
void SecondsTimeWheel::run()
{
	std::chrono::duration<int, std::milli> span{ 1000 };		/* 每一秒执行一次 */
	timer = std::make_shared<Timer>();
	timer->start(
		/* 定时器初步初始化好之后，才可以添加任务*/
		[this]
		{
			if (!completed.load())
			{
				completed.store(true);
				cv_notify.notify_one();
			}
			quarter_pointer++;

			//std::cout << "seconds: " << quarter_pointer.load() << std::endl;

			/*如果到60刻度，置为0，通知高层时间轮转动*/
			if (quarter_pointer.load() == 60)
			{
				quarter_pointer = 0;
				timeWheel->run();
			}
			/*通知线程池处理当前刻度的任务*/
			mutex.lock();
			int value = quarter_pointer;
			oaho::Task tt;
			while (!quarter_task[value].empty())
			{
				oaho::Task t = quarter_task[value].front();
				if (t._isloop())
				{
					addTask(t, t.getExcuteTime() , true);
				}
				excutePool.commit(t); 
				quarter_task[value].pop_front();
			}
			quarter_task[value].clear();
			mutex.unlock();
		}, span
	);
	std::unique_lock<std::mutex> mtx_lock{ notify_init };
	cv_notify.wait(mtx_lock, [this] { return completed.load();});
}