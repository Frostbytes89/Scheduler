//------------------------------------------------------------------------------
//
// AnimatedSprite.cpp
// Joshua Sanders
//

#include "Scheduler.h"
#include <SFML/System/Clock.hpp>
#include <iostream>
#include <fstream>
#include <chrono>

TimedFunc::TimedFunc(WorkFunction func)
  : m_interval(0),
	m_nextTimeToCall(0),
	m_timeLastCall(0),
	m_doWork(func)
{
}

TimedFunc::~TimedFunc()
{
}

//------------------------------------------------------------------------------------

Scheduler::Scheduler()
  : m_clock(new sf::Clock())
{
}

Scheduler::~Scheduler()
{

}

// For removing from the priority queue
// scheduler maintains a list of tasks queued for deletion
// before each task is done, we just check that list and remove the task from
// the queue if we're in that list.
std::ofstream os ("log.txt", std::ios::out);

void Scheduler::RunTasks()
{
	if (!os.is_open())
	{
		std::cout << "could not open";
	}

	TimedFunc currentFunction = m_funcQueue.top();
	
	// If I wanted to use std::chrono
	//int32_t currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
	//	 std::chrono::high_resolution_clock::now().time_since_epoch()).count();

	int32_t currentTime = m_clock->getElapsedTime().asMilliseconds();

	if (currentTime >= currentFunction.m_nextTimeToCall)
	{
		int64_t supposedtoBeCalled = currentFunction.m_nextTimeToCall;
		os << "When we were supposed to be called " << supposedtoBeCalled << "\n";
		os << "Time Calling: " << currentTime << "\n";
		int64_t offset = 0;
		// If this isn't the first time calling this function.
		if (supposedtoBeCalled != 0)
		{
			int64_t offset = currentTime - supposedtoBeCalled;
			os << "offset for next time " << offset << "\n";
		}
		currentFunction.m_timeLastCall = currentTime;
		currentFunction.m_doWork();
		currentFunction.m_nextTimeToCall = currentTime + (currentFunction.m_interval - offset);
		os << "Next Time to call " << currentFunction.m_nextTimeToCall << "\n";
		m_funcQueue.pop();
		m_funcQueue.push(currentFunction);

		os << "===============================================\n";
	}
	os.flush();
}

void Scheduler::InsertIntoQueue(TimedFunc func)
{
	m_funcQueue.push(func);
}
