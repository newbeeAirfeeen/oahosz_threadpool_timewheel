/*
	Copyright (c) hzsz
	All rights reserved.

	FileName    : threadpool.h
	tip		    : implements a simple thread pool.
	version	    : 2.4

	author      : oaho
	date	    : 2020-11-22
	desciprtion : progess flow: user define a variable with threadpool, threadpool could initialize ifself.
				  it initialize itself with 20 couple of thread, you could revise it by youself.after completed.
				  you can commit a task to thread pool, once task completed, process exit. otherwise, thread pool 
				  will wait the task completed.
*/

#pragma once
#ifndef __THREAD_POOL_H_
#define __THREAD_POOL_H_

#include<vector>
#include<thread>
#include<atomic>
#include<mutex>
#include<condition_variable>
#include<algorithm>
#include<functional>
#include<queue>
#include<random>
#include<utility>
#include<iostream>
namespace oaho
{
	using ReturnType = void;
	using std::default_random_engine;


	class PerviousAfterTask
	{
	public:
		PerviousAfterTask() {}
		inline bool is_set_previous() { return _is_set_previos; }
		inline bool is_set_after() { return _is_set_after; }
		template<typename T, typename...Args>
		inline int setPerviousTask(T&& t, Args&&... args)
		{
			this->_PreviousTask = std::bind(
				std::forward<T>(t),
				std::forward<Args>(args)...);
			this->_is_set_previos = true;
			return 0;
		}
		template<typename T, typename...Args>
		inline int setAfterTask(T&& t, Args&&... args)
		{
			this->_AfterTask = std::bind(
				std::forward<T>(t),
				std::forward<Args>(args)...);
			this->_is_set_after = true;
			return 0;
		}
		inline int runPrevious()
		{
			_PreviousTask();
			return 0;
		}
		inline int runAfterTask()
		{
			_AfterTask();
			return 0;
		}										//ater
	private:
		bool						_is_set_previos{ false };
		bool						_is_set_after{ false };
		std::function<ReturnType()> _PreviousTask{nullptr};
		std::function<ReturnType()> _AfterTask{nullptr};
	};


	using Task = class task{
	public:
		task();
		task(const task& other);
		task(task&&) noexcept;
		template<typename T, typename...Args>
		inline void setFunc(T&& tt, Args&&... args)
		{
			//set task id
			default_random_engine e;
			int task_id = e();
			this->taskid = task_id;
			//commit task
			//bind 函数的理解
			std::function<ReturnType()> func_cmit = std::bind(
				std::forward<T>(tt), std::forward<Args>(args)...);
			this->func = std::move(func_cmit);
		}
		template<typename T, typename...Args>
		inline void setPreviousTask(T&& tt, Args&&... args)									    //previous task
		{
			this->taskptr->setPerviousTask(std::forward<T>(tt), std::forward<Args>(args)...);
		}
		template<typename T, typename...Args>
		inline void setAfterTask(T&& tt, Args&&... args)										//after task
		{
			this->taskptr->setAfterTask(std::forward<T>(tt), std::forward<Args>(args)...);
		}
		unsigned long long getFuncId() const;													//get task id
		bool operator == (const task& other);
		task& operator = (const task& other);
		ReturnType operator()();

	protected:
		std::function<ReturnType()>				func{nullptr};									//excute function
		unsigned long long 						taskid{ (unsigned long long) - 1 };				//taskid
		std::shared_ptr<PerviousAfterTask>		taskptr{ nullptr };
	};

	class threadpool 
	{
	public:
		threadpool( unsigned int poolsize = 10 );
		~threadpool();
		inline void commit(oaho::Task& ttt)
		{
			if (isStopped.load())
				throw std::runtime_error("thread pool is notified stopping!!");

			// if pool is full, wait 200 ms
			while (thread_use_couple == pool.size())
			{
				std::cout << "thread pool is full, please wait...\n";
				std::this_thread::sleep_for(std::chrono::duration<int, std::milli>{200});
			}

			std::unique_lock<std::mutex> mtx_lock{ Mutex };
			//add task
			this->task_que.emplace_back(ttt);
			//notify a thread excute
			excuteTask.store(true);
			cv.notify_one();
		}
		int getTaskSize();																//get size fo task
		void terminate();																//quit the threads


	private:
		std::vector<std::shared_ptr<std::thread>>				pool;				    //thread pool
		std::atomic<unsigned int>								thread_use_couple;		//thread use size
		std::deque<Task>										task_que;				//task que
		std::atomic<bool>										atomic_completed;		//notify the constructor , init success.
		std::atomic<bool>										isStopped;				//threadpool whether exit.
		std::atomic<bool>										excuteTask;				//task_que is not empty();
		std::mutex												Mutex;					
		std::mutex												completed_mutex;	    //pre the init
		std::condition_variable									cv;		
		std::condition_variable									cv_notify;
		bool												    all_exit;
	};


};
#endif // !__THREAD_POOL_H_
