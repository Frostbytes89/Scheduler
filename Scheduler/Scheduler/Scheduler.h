#pragma once

#include <functional>
#include <queue>
#include <vector>
#include <stdint.h>

namespace sf
{
	class Clock;
	class Time;
}

typedef std::function<void()> WorkFunction;
struct TimedFunc
{
	TimedFunc(WorkFunction func, char identifier);
	~TimedFunc();
	char m_identifier;
	int m_interval;
	int32_t m_nextTimeToCall;
	int32_t m_timeLastCall;
	WorkFunction m_doWork;
};

struct Comparator {
	bool operator() (TimedFunc rhs, TimedFunc lhs)
	{
		return lhs.m_nextTimeToCall < rhs.m_nextTimeToCall;
	}
};

typedef std::priority_queue<TimedFunc, std::vector<TimedFunc>,
	Comparator> TimedFuncPriorityQueue;

struct Scheduler
{
public:
	Scheduler();
	~Scheduler();

	sf::Clock* m_clock;
	void RunTasks();
	TimedFuncPriorityQueue m_funcQueue;
	void InsertIntoQueue(TimedFunc func);

	void operator()() {
		while (true)
		{
			RunTasks();
		}
	}
};
