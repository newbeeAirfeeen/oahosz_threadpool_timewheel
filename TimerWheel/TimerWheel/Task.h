//#pragma once
//#ifndef __TASK_H__
//#define __TASK_H__
//
//#include<functional>
//#include<random>
//namespace oaho
//{
//
//	using ReturnType = void;
//	using std::default_random_engine;
//
//
//	using Task = class task {
//	public:
//		task() {};
//		task(const task& other);
//		task(task&&) noexcept;
//
//		/*
//			设置函数执行体,不支持成员函数
//		*/
//		template<typename T, typename...Args>
//		inline void setFunc(T&& tt, Args&&... args)
//		{
//			//commit task
//			//bind 函数的理解
//			std::function<ReturnType()> func_cmit = std::bind(
//				std::forward<T>(tt), std::forward<Args>(args)...);
//			this->func = std::move(func_cmit);
//		}
//
//
//		/* 标识每个任务 任务ID */
//		unsigned long long getTaskId() const;				
//
//		/* 设置任务是否循环 */
//		/* 此函数用于循环任务 */
//		void setLoop(bool loop);
//
//		/* 标志任务是否循环 */
//		bool Isloop();
//
//		/* 标志任务执行的延时时间 */
//		void setDelayTime(unsigned long long time);
//		unsigned long long getDelayTime();
//
//		bool operator == (const task& other);
//		task& operator = (const task& other);
//		ReturnType operator()();
//
//	protected:
//		std::function<ReturnType()>				func{ nullptr };								/* 执行函数体 */
//		unsigned long long 						taskid{ (unsigned long long) - 1 };				/* 任务ID */
//		unsigned long long						delayTime{0};									/* 执行的时间 */
//		bool									isloop{ false };								/* 是否循环执行 */
//	private:
//		void copy(const oaho::task& tt);
//	};
//}
//
//
//
//
//
//#endif
