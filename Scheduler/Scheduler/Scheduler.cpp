//------------------------------------------------------------------------------
//
// Scheduler.cpp
// Joshua Sanders
//

#include "Scheduler.h"
#include <SFML/System/Clock.hpp>
#include <iostream>
#include <fstream>
#include <memory>

Scheduler::Scheduler()
: m_clock(new sf::Clock()),
m_currentTime(m_clock->getElapsedTime().asMilliseconds())
{
}

Scheduler::~Scheduler()
{
}

// For removing from the priority queue
// scheduler maintains a list of tasks queued for deletion
// before each task is done, we just check that list and remove the task from
// the queue if we're in that list.
std::ofstream os("log.txt", std::ios::out);
int counter = 0;
void Scheduler::RunTasks()
{
	if (m_funcQueue.empty())
	{
		return;
	}

	TimedCallback currentFunction(std::move(m_funcQueue.top()));
	m_funcQueue.pop();

	m_currentTime = m_clock->getElapsedTime().asMilliseconds();

	// Maybe we want to call function at next interval instead of immediately.
	if (HandleDelayedTick(currentFunction))
	{
		return;
	}

	bool success = true;
	if (m_currentTime >= currentFunction.GetNextTimeToCall())
	{
		int32_t supposedtoBeCalled = currentFunction.GetNextTimeToCall();
		int32_t offset = 0;

		if (supposedtoBeCalled != 0)
		{
			offset = m_currentTime - supposedtoBeCalled;
		}

		currentFunction.SetLastTimeCalled(m_currentTime);

		success = currentFunction.ExecuteCallback();

		int interval = currentFunction.GetInterval();
		int offsetInterval = interval - offset;

		currentFunction.SetNextTimeToCall(m_currentTime + (currentFunction.GetInterval() - offset));		
	}
	if (success)
	{
		m_funcQueue.push(std::move(currentFunction));
	}

	os.flush();
}

void Scheduler::InsertIntoQueue(TimedCallback& cb)
{
	m_funcQueue.push(std::move(cb));
}

bool Scheduler::HandleDelayedTick(TimedCallback& cb)
{
	bool delay = false;

	if (cb.GetHasDelayedTick())
	{
		cb.SetNextTimeToCall(m_currentTime + cb.GetInterval());
		cb.SetHasDelayedTick(false);
		m_funcQueue.push(std::move(cb));

		delay = true;
	}

	return delay;
}
