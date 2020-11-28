//#pragma once
//#ifndef __POINTER_H__
//#define __POINTER_H__
//#include<atomic>
//此类为了简化针运动的情况 
//class Pointer
//{
//public:
//	Pointer(unsigned int quarter) :pointer{ 0 }, max_quarter{ quarter } {}
//	virtual void run() = 0;
//protected:
//	std::atomic<int>			pointer;
//	int							max_quarter;
//};
//
//
//
//
//class SecPointer :public Pointer
//{
//public:
//	SecPointer() :Pointer(60) {}
//	virtual void run() override ;
//};
//class MinPointer :public Pointer
//{
//public:
//	MinPointer() : Pointer(60) {}
//	virtual void run() override;
//};
//class HourPointer :public Pointer
//{
//public:
//	HourPointer() : Pointer(24) {}
//	virtual void run() override;
//};
//
//#endif // !__POINTER_H__
