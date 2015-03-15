#include <iostream>
#include "Scheduler.h"
#include <thread>

int main()
{
	Scheduler scheduler;

	TimedFunc a([] {
		std::cout << "From Function A ";
		std::cout << "Thread ID " << __threadid() << "\n";
	}, 'A');
	a.m_interval = 4000;

	TimedFunc b([] {
		std::cout << "From Function B ";
		std::cout << "Thread ID " << __threadid() << "\n";
	}, 'B');
	b.m_interval = 8000;

	TimedFunc c([] {
		std::cout << "From Function C ";
		std::cout << "Thread ID " << __threadid() << "\n";
	}, 'C');
	c.m_interval = 12000;

	TimedFunc d([] {
		std::cout << "From Function D ";
		std::cout << "Thread ID " << __threadid() << "\n";
	}, 'D');
	d.m_interval = 3000;

	TimedFunc e([] {
		std::cout << "From Function E ";
		std::cout << "Thread ID " << __threadid() << "\n";
	}, 'E');
	e.m_interval = 6000;

	TimedFunc f([] {
		std::cout << "From Function F ";
		std::cout << "Thread ID " << __threadid() << "\n";
	}, 'F');

	f.m_interval = 9000;

	scheduler.InsertIntoQueue(a);
	scheduler.InsertIntoQueue(b);
	scheduler.InsertIntoQueue(c);

	Scheduler schedulerA;
	schedulerA.InsertIntoQueue(d);
	schedulerA.InsertIntoQueue(e);
	schedulerA.InsertIntoQueue(f);

	std::thread t1 (scheduler);
	// Join causes main to wait on separate thread
	// Detach will make the new thread free
	//t1.join();
	t1.detach();

	while (true)
	{
		schedulerA.RunTasks();
	}
	
	system("pause");
	return 0;
}
