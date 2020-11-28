#include<iostream>
#include<ratio>
#include<chrono>
#include <functional>
#include"TimeWheel.h"
using namespace std;

std::mutex mt;
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


	SecondsTimeWheel st;
	oaho::Task t;
	t.setFunc([] {
		std::cout << "Hello,world " << std::endl;
	});
	st.run();
	st.addTask(t, 3, true);

	cin.get();


	return 0;
}
