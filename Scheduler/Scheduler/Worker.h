//------------------------------------------------------------------------------
//
// Worker.h
// Joshua Sanders
//

#pragma once

#include <vector>
#include <iostream>

class Worker
{
public:
	Worker();
	~Worker();
	void AdvanceCurrentData();
	void PrintData();

private:
	std::vector<int> m_data;
	int m_currentData;
};
