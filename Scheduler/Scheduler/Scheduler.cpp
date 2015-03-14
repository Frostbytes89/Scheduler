//------------------------------------------------------------------------------
//
// AnimatedSprite.cpp
// Joshua Sanders
//

#include "Scheduler.h"
#include <SFML/System/Clock.hpp>
#include <iostream>

TimedFunc::TimedFunc(WorkFunction func)
  : m_interval(0),
	m_nextTimeToCall(0),
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

void Scheduler::RunTasks()
{
	while (!m_funcQueue.empty())
	{
		TimedFunc currentFunction = m_funcQueue.top();
		int64_t currentTime = m_clock->getElapsedTime().asMilliseconds();

		// First call
		if (currentFunction.m_nextTimeToCall == 0 )
		{
			currentFunction.m_timeLastCall = currentTime;
			m_funcQueue.top().m_doWork();
			currentFunction.m_nextTimeToCall = currentFunction.m_timeLastCall + currentFunction.m_interval;
			m_funcQueue.pop();
			m_funcQueue.push(currentFunction);
			continue;
		}
		
		else if (m_clock->getElapsedTime().asMilliseconds() >= currentFunction.m_nextTimeToCall)
		{
			// when were we supposed to be called
			// what time is it now?
			// store the time now as the time last called
			// store the difference in an offset
			// call the function
			//set the next time to call to the time this was called, plus (the interval minus the offset)

			int64_t supposedtoBeCalled = currentFunction.m_timeLastCall;		
			int64_t offset = currentTime - supposedtoBeCalled;

			currentFunction.m_timeLastCall = currentTime;
			currentFunction.m_doWork();
			currentFunction.m_nextTimeToCall = currentTime + (currentFunction.m_interval - offset);
			m_funcQueue.pop();
			m_funcQueue.push(currentFunction);
		}
	}
}

void Scheduler::InsertIntoQueue(TimedFunc func)
{
	m_funcQueue.push(func);
}
