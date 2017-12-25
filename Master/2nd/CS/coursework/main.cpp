#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>

#include "device_logic.h"

#define TASKS_COUNT	100

bool JOB_IS_DONE = false;

double exp_distr(double ksi, double lambda)
{
	auto number = -1.0 * (log(1.0 - ksi) / lambda);
	return number;
}

double uni_distr(double a, double b)
{
	auto number = ((double)rand() / (RAND_MAX)) * (b - a) + a;
	return number;
}

void DeviceRoutine(std::shared_ptr<Device> device, double device_time);

int main()
{
	srand((unsigned)time(NULL));

	int k = 2;
	size_t n = 6;
	double lambda = 1.0;

	auto dev1 = std::make_shared<Device>("fast");
	auto dev2 = std::make_shared<Device>("slow");

	std::thread fastDeviceHandler(DeviceRoutine, dev1, lambda);
	std::thread slowDeviceHandler(DeviceRoutine, dev2, lambda * k);

	for (int i = 0; i < TASKS_COUNT; ++i)
	{
		double tau = uni_distr(0.7, 1.1);
		Task task(i, tau);

		double ksi = uni_distr(0.0, 1.0);
		double whatsThis = exp_distr(ksi, lambda);	// ???

		if (dev1->GetTaskQueue()->size() < n)
		{
			dev1->Enqueue(task);
		}
		else
		{
			dev2->Enqueue(task);
		}
	}

	while (!dev1->GetTaskQueue()->empty() && !dev2->GetTaskQueue()->empty())
	{
		if (dev1->GetTaskQueue()->size() < n)
		{
			auto task = dev2->Dequeue();
			dev1->Enqueue(task);
		}
	}

	JOB_IS_DONE = true;

	fastDeviceHandler.join();
	slowDeviceHandler.join();

	std::cout << "\nJob is done!\n\n";
	std::cout << "[Dev1] uptime: " << dev1->GetUptime() << ", tasks completed: " << dev1->GetCompletedTasksCount()
		<< ", avg time in queue: " << dev1->GetUptime() / dev1->GetCompletedTasksCount() << " ms\n";

	std::cout << "[Dev2] uptime: " << dev2->GetUptime() << ", tasks completed: " << dev2->GetCompletedTasksCount()
		<< ", avg time in queue: " << dev2->GetUptime() / dev2->GetCompletedTasksCount() << " ms\n";

	getchar();
	return 0;
}

void DeviceRoutine(std::shared_ptr<Device> device, double device_time)
{
	while (1)
	{
		if (device->GetTaskQueue()->size() > 0) 
		{
			int task_id = device->GetTaskQueue()->front().id;
			double task_time = device->GetTaskQueue()->front().t;
			long wait_time = (long)(task_time * device_time * 100);

			device->IncreaseUptime(wait_time);
			device->IncreaseCompletedTasks();
			device->Dequeue();

			std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));

			// compact
			std::cout << std::setw(10) << device->GetType() << " - ["
				<< std::setw(5) << device->GetTaskQueue()->size() << "], #"
				<< std::setw(5) << task_id << " done in "
				<< std::setw(5) << wait_time << " ms\n";

			// detailed
			//std::cout << "\nDevice type: " << device->GetType() << ", queue size: " << device->GetTaskQueue()->size() << ", task " << task_id << " resolved after " << device_time << " ms.\n";
		}
		else if (JOB_IS_DONE)
		{
			break;
		}
	}
}
