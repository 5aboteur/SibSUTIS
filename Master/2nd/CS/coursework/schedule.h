#ifndef _SCHEDULE_H_
#define _SCHEDULE_H_

#include <list>
#include <memory>

struct Event {
	Event(std::string _dtype, int _tid, double _at, double _et)
		: device_type(_dtype)
		, task_id(_tid)
		, arrive_time(_at)
		, execution_time(_et) {}

	std::string device_type;
	int task_id;
	double arrive_time;
	double execution_time;
};

class Schedule
{
public:
	Schedule()
	{
		schedule = std::list<Event>();
	}

	const std::list<Event> & Get() { return schedule; }

	void AddEvent(Event _e) { schedule.push_back(_e); }
	void RemEvent() { schedule.pop_back(); }

	void Sort();
	void PrintSchedule();

private:
	std::list<Event> schedule;
};

#endif
