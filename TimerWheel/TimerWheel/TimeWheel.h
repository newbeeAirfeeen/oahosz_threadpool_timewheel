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
#pragma once
#ifndef __TIME_WHEEL_H
#define __TIME_WHEEL_H
/*
	时间轮可以抽象出一个父类。子类分 秒轮, 分轮, 时轮。
	共同有添加任务，删除任务等的共同抽象的不同实现。
*/
#include"threadpool.h"
#include<queue>
#include<vector>
#include<atomic>
#include<mutex>
#include<condition_variable>
#include<utility>
#include"Timer.h"

class TimeWheel 
{
public:
	TimeWheel():mtx_lock{ mtx, std::defer_lock } {}
	virtual ~TimeWheel() {}
	virtual void addTask(oaho::Task t, unsigned int position, bool loop) = 0;			/* 添加任务 */
	virtual void run() = 0;																/* 转动刻度 */

protected:
	std::shared_ptr<TimeWheel>							timeWheel;						/* 指向下一个时间轮 */
	std::vector<std::deque<oaho::Task>>					quarter_task;					/* 任务队列 */
	std::atomic<int>									quarter_pointer;				/* 刻度指针 */
	std::atomic<bool>									completed;						/* 是否初步完成初始化 */
	unsigned int										max_quarter;					/* 最大刻度 */
	std::mutex											notify_init;					/* 初步初始化完成的通知 */
	std::mutex											mtx;
	std::condition_variable								cv_notify;
	std::shared_ptr<Timer>								timer;							/* 定时拨动刻度 */
	oaho::threadpool									excutePool{ 50 };				/* 执行任务的线程池 */
	oaho::threadpool									notify_pool{ 10 };
	std::unique_lock<std::mutex>						mtx_lock;
	//std::recursive_mutex          						recursive_lock;
};

class HoursTimeWheel :public TimeWheel
{

};

class MinutesTimeWheel :public TimeWheel
{
public:
	MinutesTimeWheel();
	~MinutesTimeWheel();
public:
	virtual void addTask(oaho::Task t, unsigned int position, bool loop) override;
	virtual void run() override;
};
class SecondsTimeWheel:public TimeWheel                             /* 秒轮 */
{																	/* 每一秒移动一个刻度,到60刻度的时候归0.在通知分轮去转 */
public:
	SecondsTimeWheel();
	~SecondsTimeWheel();
	

public:
	virtual void addTask(oaho::Task t, unsigned int position, bool loop) override;
	virtual void run() override;
};



#endif // !__TIME_WHEEL_H






