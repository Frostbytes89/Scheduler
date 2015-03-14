#include <iostream>
#include "Scheduler.h"

int main()
{
	Scheduler scheduler;

	TimedFunc a([] {
		std::cout << "From Function A\n";
	});
	a.m_interval = 1000;

	TimedFunc b([] {
		std::cout << "From Function B\n";
	});
	b.m_interval = 3000;

	TimedFunc c([] {
		std::cout << "From Function C\n";
	});
	c.m_interval = 2000;

	scheduler.InsertIntoQueue(a);
	scheduler.InsertIntoQueue(b);
	scheduler.InsertIntoQueue(c);

	scheduler.RunTasks();

	system("pause");
	return 0;
}
