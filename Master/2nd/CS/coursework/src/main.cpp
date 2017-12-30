#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <cmath>
#include <string>
#include <thread>

#include "device.h"
#include "schedule.h"

#define TASKS_COUNT 100

struct Stats {
	Stats(std::string _dtype, double _uptime, double _dt, double _delay, int _n, int _ct, int _id)
		: dtype(_dtype), uptime(_uptime), dt(_dt), delay(_delay), n(_n), ct(_ct), id(_id) {}

	std::string dtype;

	double uptime;
	double dt;
	double delay;

	int n;
	int ct;

	int id;
};

double exp_distr(double, double);
double uni_distr(double, double);

void DoProgress(std::vector<std::shared_ptr<Device>> & devices, double dt, std::shared_ptr<Schedule> schedule);
void PrintStatistics(std::vector<std::shared_ptr<std::pair<Stats, Stats>>> & statistics);

int main()
{
	srand((unsigned)time(NULL));

	int iter = 1;
	int k = 2;
	size_t n = 6;

	auto devices = std::vector<std::shared_ptr<Device>>();
	auto statistics = std::vector<std::shared_ptr<std::pair<Stats, Stats>>>();

	auto tau = std::vector<double>();

	for (int i = 0; i < iter; ++i)
	{
		auto dt = uni_distr(0.7, 1.1);
		tau.push_back(dt);
	}

	std::sort(tau.begin(), tau.end());

	auto lambda = std::vector<double>();

	for (int i = 0; i < iter; ++i)
	{
		auto ksi = uni_distr(0.0, 1.0);
		auto dt = exp_distr(ksi, 0.8);
		lambda.push_back(dt);
	}

	std::sort(lambda.begin(), lambda.end());

	int exp = 0;

	for (auto dt : tau)
	{
		for (auto lm : lambda)
		{
			double uptime = 0.0;

			devices.push_back(std::make_shared<Device>("fast", lm));
			devices.push_back(std::make_shared<Device>("slow", lm * k));

			auto fast = devices[0];
			auto slow = devices[1];

			auto schedule = std::make_shared<Schedule>();

			// Spread tasks
			for (int i = 1; i <= TASKS_COUNT; ++i)
			{
				if (fast->GetTaskQueue().size() < n)
				{
					fast->Enqueue(Task(i, dt * i));
				}
				else
				{
					slow->Enqueue(Task(i, dt * i));
				}

				DoProgress(devices, dt, schedule);

				uptime += dt;
			}

			// Wait 'til devices complete their work
			while (schedule->Get().size() != TASKS_COUNT)
			{
				if (fast->GetTaskQueue().size() < n)
				{
					while (!slow->GetTaskQueue().empty() && fast->GetTaskQueue().size() < n)
					{
						fast->Enqueue(slow->GetTaskQueue().front());
						slow->Dequeue();
					}
				}

				DoProgress(devices, dt, schedule);

				uptime += dt;
			}

			++exp;

			auto fast_stat = Stats(fast->GetType(), uptime, dt, fast->GetDelay(), n, fast->GetCompletedTasksCount(), exp);
			auto slow_stat = Stats(slow->GetType(), uptime, dt, slow->GetDelay(), -1, slow->GetCompletedTasksCount(), exp);
			auto stat = std::make_shared<std::pair<Stats, Stats>>(std::make_pair(fast_stat, slow_stat));
			statistics.push_back(stat);

			schedule->Sort();
			schedule->PrintSchedule();

			devices.clear();
		}
	}

	PrintStatistics(statistics);

	getchar();
	return 0;
}

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

void DoProgress(std::vector<std::shared_ptr<Device>> & devices, double dt, std::shared_ptr<Schedule> schedule)
{
	for (auto & device : devices)
	{
		device->IncreaseProgressTime(dt);

		if (device->Progress())
		{
			schedule->AddEvent(Event(device->GetType(), device->GetTaskQueue().front().id, device->GetTaskQueue().front().arrived_time, device->GetLastCompletedTaskTime()));
			device->Dequeue();
			device->IncreaseCompletedTasksCounter();
		}
	}
}

void PrintStatistics(std::vector<std::shared_ptr<std::pair<Stats, Stats>>> & statistics)
{
	std::cout << "  # |  tau | type | delay |  n  | tasks |  avg  \n";
	std::cout << "------------------------------------------------\n";

	for (const auto & stat : statistics)
	{
		std::cout << std::fixed << std::setw(3) << stat->first.id << " | "
			<< std::setw(4) << std::setprecision(2) << stat->first.dt << " | "
			<< std::setw(4) << stat->first.dtype << " | "
			<< std::setw(5) << std::setprecision(2) << stat->first.delay << " | "
			<< std::setw(3) << stat->first.n << " | "
			<< std::setw(5) << stat->first.ct << " | "
			<< std::setw(5) << std::setprecision(2) << stat->second.uptime / TASKS_COUNT << "\n";

		std::cout << std::fixed << std::setw(3) << " " << " | "
			<< std::setw(4) << std::setprecision(2) << stat->second.dt << " | "
			<< std::setw(4) << stat->second.dtype << " | "
			<< std::setw(5) << std::setprecision(2) << stat->second.delay << " | "
			<< std::setw(3) << "Inf" << " | "
			<< std::setw(5) << stat->second.ct << " | "
			<< std::setw(5) << std::setprecision(2) << "\n";

		std::cout << "------------------------------------------------\n";
	}
}
