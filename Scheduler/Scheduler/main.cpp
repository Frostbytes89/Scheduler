#include "Scheduler.h"
#include "Worker.h"
#include <iostream>
#include <thread>

int main()
{
	std::shared_ptr<Worker> workerPtr(new Worker{});

	Scheduler scheduler;
	TimedCallback tcb(workerPtr, [&workerPtr] ()
	{
		workerPtr->AdvanceCurrentData();
		workerPtr->PrintData();
	});
	tcb.SetInterval(1000);
	scheduler.InsertIntoQueue(tcb);

	Scheduler schedulerA;
	TimedCallback deleter(workerPtr, [&workerPtr] {
		workerPtr.reset();
		std::cout << "\ndeleting\n";
	});

	deleter.SetInterval(5000);
	deleter.SetHasDelayedTick(true);
	schedulerA.InsertIntoQueue(deleter);

	std::thread t1(std::move(scheduler));

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
