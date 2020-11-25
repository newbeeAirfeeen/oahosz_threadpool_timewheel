/*
	Copyright (c) hzsz
	All rights reserved.

	FileName    : threadpool.h
	tip		    : implements a simple thread pool.
	version		: 2.4

	author      : oaho
	date	    : 2020-11-22
	desciprtion : progess flow: user define a variable with threadpool, threadpool could initialize ifself.
				  it initialize itself with 20 couple of thread, you could revise it by youself.after completed.
				  you can commit a task to thread pool, once task completed, process exit. otherwise, thread pool
				  will wait the task completed.
*/

#include"threadpool.h"

//################################### Task ############################
/// <summary>
///      task
/// </summary>
/// <param name="func">task pointer, function object.... </param>

oaho::task::task() 
{
	taskptr = std::make_shared<PerviousAfterTask>();
}
oaho::task::task(const task& other) 
{

	this->taskptr = other.taskptr;
	setFunc(const_cast<task&>(other).func);
}
oaho::task::task(task&& t) noexcept
{
	this->func = std::move(t.func);
	this->taskid = t.taskid;
	t.taskid = -1;
}
unsigned long long oaho::task::getFuncId() const
{
	return this->taskid;
}
bool oaho::task::operator == (const oaho::task& other)
{
	return this->getFuncId() == other.getFuncId();
}
oaho::ReturnType oaho::task::operator()()
{
	bool fsa = this->taskptr->is_set_previous();
	if (this->taskptr->is_set_previous())
		taskptr->runPrevious();
	this->func();
	if (this->taskptr->is_set_after())
		taskptr->runAfterTask();
}
oaho::task& oaho::task::operator = (const task& other)
{
	if (this == &other)
	{
		return *this;
	}
	this->taskptr = other.taskptr;
	setFunc(const_cast<task&>(other).func);
	return *this;
}


//#################################### thread pool #################################
/// <summary>
///			threadpool
/// </summary>
/// <param name="poolsize">thread pool size</param>
oaho::threadpool::threadpool(unsigned int poolsize) :thread_use_couple{0},
													 atomic_completed{false},
													 isStopped{false},
													 excuteTask{false},
													 all_exit{false}{

	/*
	   线程的运行方式： 线程在等待，此时被通知要求退出等待，并且销毁。此时任务池里应该是没有任务
	*/
	for (int i = 0; i < poolsize; ++i)
	{
		pool.emplace_back(
			std::make_shared<std::thread>([this] {
				while (true)
				{
					//notify : one thread completed
					if (!atomic_completed.load())
					{
						atomic_completed.store(true);
						cv_notify.notify_one();
					}
					//if notified quit , exit
					if (isStopped.load() && task_que.empty())
					{
						break;
					}
					Task t;
					{
						std::unique_lock<std::mutex> mtx_lock{ Mutex };
						//wait the task;
						cv.wait(mtx_lock, [this] {
							return excuteTask.load() || isStopped.load();
						});
						if (task_que.empty())
							break;
						t = task_que.front();
						task_que.pop_front();
					}

					thread_use_couple++;
					t();
					thread_use_couple--;
				}
			}));
	}
	std::unique_lock<std::mutex> lck_mtx{ completed_mutex};
	cv_notify.wait(lck_mtx, [this] {
		return atomic_completed.load();
	});
}
oaho::threadpool::~threadpool()
{
	if(!all_exit)
		terminate();
}
int oaho::threadpool::getTaskSize()
{
	return task_que.size();
}
void oaho::threadpool::terminate()
{
	isStopped.store(true);
	cv.notify_all();
	for (auto ptr : pool)
	{
		if (ptr->joinable())				//join each thread
			ptr->join();
	}
	all_exit = true;
}