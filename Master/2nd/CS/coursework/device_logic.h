#ifndef _DEVICE_LOGIC_H_
#define _DEVICE_LOGIC_H_

#include "task_queue.h"

struct Task {
	Task(int _id, double _t) : id(_id), t(_t) {}

	int	id;
	double t;
};

class Device {
public:
	Device() : type("slow"), uptime(0), completed_tasks_counter(0)
	{
		taskQueue = std::make_shared<TaskQueue<Task>>();
	}

	Device(std::string _t) : type(_t), uptime(0), completed_tasks_counter(0)
	{
		taskQueue = std::make_shared<TaskQueue<Task>>();
	}

	std::shared_ptr<TaskQueue<Task>> GetTaskQueue() { return taskQueue; }
	std::string GetType() { return type; }
	long GetUptime() { return uptime; }
	long GetCompletedTasksCount() { return completed_tasks_counter; }

	void IncreaseCompletedTasks() { ++completed_tasks_counter; }
	void IncreaseUptime(long _t) { uptime += _t; }

	Task Dequeue() { return taskQueue->pop(); }
	void Enqueue(Task _task) { taskQueue->push(_task); }

private:
	std::string	type;
	long uptime;
	long completed_tasks_counter;

	std::shared_ptr<TaskQueue<Task>> taskQueue;
};

#endif
