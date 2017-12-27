#include <iostream>
#include <queue>

#include "device.h"

Device::Device()
	: type("slow")
	, delay(0.0)
	, progress_time(0)
	, lct_time(0)
	, ct_counter(0) {}

Device::Device(std::string _t, double _delay)
	: type(_t)
	, delay(_delay)
	, progress_time(0)
	, lct_time(0)
	, ct_counter(0) {}

bool Device::Progress()
{
	if (!taskQueue.empty())
	{
		if (progress_time > delay)
		{
			progress_time -= delay;
			//double tau = taskQueue.front().arrived_time / (ct_counter + 1);
			//lct_time = (lct_time > tau * (ct_counter + 1)) ? lct_time + delay : tau;
			lct_time = (lct_time) ? lct_time + delay : taskQueue.front().arrived_time /*delay*/;

			return true;
		}
	}

	return false;
}
