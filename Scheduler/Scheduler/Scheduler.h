//------------------------------------------------------------------------------
//
// Scheduler.h
// Joshua Sanders
//

#pragma once

#include <functional>
#include <queue>
#include <vector>
#include <stdint.h>
#include <memory>

namespace sf
{
	class Clock;
	class Time;
}

typedef std::function<void()> WorkCallback;
class TimedCallback
{
public:
	template<class T>
	TimedCallback(const T& t, WorkCallback& callback)
		: m_internalBase(std::make_unique<InternalDerived<T>>(t, callback)),
		m_interval(0),
		m_lastTimeCalled(0),
		m_hasDelayedTick(false),
		m_identifier('A')
	{
	}

	TimedCallback(TimedCallback&& rhs)
		: m_internalBase(std::move(rhs.m_internalBase)),
		m_interval(rhs.m_interval),
		m_lastTimeCalled(rhs.m_lastTimeCalled),
		m_hasDelayedTick(rhs.m_hasDelayedTick),
		m_identifier(rhs.m_identifier)
	{
	}
	
	TimedCallback& operator=(TimedCallback&& rhs)
	{
		m_internalBase = std::move(rhs.m_internalBase);
		m_interval = rhs.m_interval;
		m_lastTimeCalled = rhs.m_lastTimeCalled;
		m_hasDelayedTick = rhs.m_hasDelayedTick;
		m_identifier = rhs.m_identifier;

		return *this;
	}

	TimedCallback(const TimedCallback&) = delete;
	TimedCallback& operator=(const TimedCallback&) = delete;

	bool ExecuteCallback() { m_internalBase->ExecuteCallback(); }
	int32_t GetInterval() { return m_interval; }
	int32_t GetNextTimeToCall() { return m_nextTimeToCall; }
	int32_t GetLastTimeCalled() { return m_lastTimeCalled; }
	bool GetHasDelayedTick() { return m_hasDelayedTick; }

	void SetInterval(int32_t interval) { m_interval = interval; }
	void SetNextTimeToCall(int32_t nextTimeToCall) { m_nextTimeToCall = nextTimeToCall; }
	void SetLastTimeCalled(int32_t lastTimeCalled) { m_lastTimeCalled = lastTimeCalled; }
	void SetHasDelayedTick(bool hasDelayedTick) { m_hasDelayedTick = hasDelayedTick; }

	friend struct Comparator;

private:
	//--------------------------------------------------------

	class InternalBase
	{
	public:
		virtual ~InternalBase();
		virtual bool ExecuteCallback() = 0;
	};

	//--------------------------------------------------------

	template <class T>
	class InternalDerived : public InternalBase
	{
	public:
		InternalDerived(const T& t, WorkCallback& callback)
			: m_functionCreator(t),
			m_callback(callback)
		{
		}

		virtual bool ExecuteCallback() override
		{
			if (auto m_functionCreator.lock())
			{
				// call the creator's function.
				m_callback();
				return true;
			}
			return false;
		}

		// Data necessary to execute the callback after determining that it is still
		// valid to do so.
		std::weak_ptr<typename T::element_type> m_functionCreator;
		WorkCallback m_callback;
	};

	//-------------------------------------------------------

	std::unique_ptr<InternalBase> m_internalBase;

	// Time management attributes.
	int32_t m_interval;
	int32_t m_nextTimeToCall;
	int32_t m_lastTimeCalled;

	// Indicates whether we should wait a full interval before executing the first time.
	bool m_hasDelayedTick;

	// For debugging. TODO: Remove
	char m_identifier;
};

struct Comparator {
	bool operator() (const TimedCallback& rhs, const TimedCallback& lhs)
	{
		return lhs.m_lastTimeCalled < rhs.m_lastTimeCalled;
	}
};

typedef std::priority_queue<TimedCallback, std::vector<TimedCallback>,
	Comparator> TimedFuncPriorityQueue;
struct Scheduler
{
public:
	Scheduler();
	~Scheduler();

	sf::Clock* m_clock;
	void RunTasks();
	TimedFuncPriorityQueue m_funcQueue;
	int32_t m_currentTime;

	void InsertIntoQueue(const TimedCallback& func);
	bool HandleDelayedTick(TimedCallback& func);
	void operator()() {
		while (true)
		{
			RunTasks();
		}
	}
};
