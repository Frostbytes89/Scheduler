//------------------------------------------------------------------------------
//
// Worker.cpp
// Joshua Sanders
//

#include "Worker.h"

Worker::Worker()
: m_currentData(0)
{
	m_data.push_back(0);
	m_data.push_back(1);
	m_data.push_back(2);
	m_data.push_back(3);
	m_data.push_back(4);
}

Worker::~Worker()
{
}

void Worker::AdvanceCurrentData()
{
	if (++m_currentData >= m_data.size())
	{
		m_currentData = 0;
	}
}

void Worker::PrintData()
{
	std::cout << m_data[m_currentData];
}
