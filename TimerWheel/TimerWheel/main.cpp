#include"Timer.h"
#include<iostream>
#include<ratio>
#include<chrono>
#include <functional>
#include"threadpool.h"
using namespace std;


void print2()
{
	std::this_thread::sleep_for(std::chrono::seconds{ 2 });
	cout << "Hello,world" << endl;
}

void print3()
{
	std::this_thread::sleep_for(std::chrono::seconds{ 3 });
	cout << "Hello,shen" << endl;
}
void print4(int i)
{
	std::this_thread::sleep_for(std::chrono::seconds{ 1 });
	cout << i << "  shen,world" << endl;
}








int main()
{

	//定时器部分
	using msecond = std::chrono::duration<int, std::milli>;
	using secondtype = std::chrono::duration<int>;
	


	oaho::Task t;
	t.setPreviousTask(print4, 5);
	//t.setAfterTask(&A::print, 456);
	t.setFunc(print2);
	



	oaho::threadpool pool;
	pool.commit(t);




	return 0;
}