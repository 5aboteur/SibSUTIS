#include <algorithm>
#include <iostream>
#include <iomanip>
#include <string>
#include <queue>

#include "schedule.h"

void Schedule::Sort()
{
	schedule.sort([](const Event & _e1, const Event & _e2)
	{
		return _e1.execution_time < _e2.execution_time;
	});
}

void Schedule::PrintSchedule()
{
	std::cout << "\n   dev | task | arrived | executed \n";
	std::cout << "-------------------------------------\n";

	for (const auto & e : schedule)
	{
		std::cout << std::fixed << "  " << e.device_type << " | "
			<< std::setw(4) << e.task_id << " | "
			<< std::setw(7) << std::setprecision(2) << e.arrive_time << " | "
			<< std::setw(7) << std::setprecision(2) << e.execution_time << "\n";
	}
}
