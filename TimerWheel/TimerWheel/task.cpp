

#include"Task.h"


oaho::task::task(const task& other)
{
	copy(other);
	setFunc(const_cast<task&>(other).func);
}
oaho::task::task(task&& t) noexcept
{
	copy(t);
	this->func = std::move(t.func);
	t.taskid = -1;
}

unsigned long long oaho::task::getTaskId() const
{
	return this->taskid;
}

bool oaho::task::operator == (const oaho::task& other)
{
	return this->getTaskId() == other.getTaskId();
}

oaho::ReturnType oaho::task::operator()()
{
	this->func();
}
oaho::task& oaho::task::operator = (const task& other)
{
	if (this == &other)
	{
		return *this;
	}
	copy(other);
	setFunc(const_cast<task&>(other).func);
	return *this;
}


void oaho::task::copy( const oaho::task& other)
{
	this->delayTime = other.delayTime;
	this->isloop = other.isloop;
	this->func = other.func;  /* std::move(other.func)*/
	this->taskid = other.taskid;
}




/* 设置任务是否循环 */
/* 此函数用于循环任务 */
void oaho::task::setLoop(bool loop)
{
	this->isloop = loop;
}

/* 标志任务是否循环 */
bool oaho::task::Isloop()
{
	return this->isloop;
}
/* 标志任务执行的延时时间 */
void oaho::task::setDelayTime(unsigned long long delaytime)
{
	this->delayTime = delaytime;
}
unsigned long long oaho::task::getDelayTime()
{
	return this->delayTime;
}