#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <memory>
#include <string>
#include <queue>

struct Task {
	Task(int _id, double _t)
		: id(_id)
		, arrived_time(_t) {}

	int id;
	double arrived_time;
};

class Device {
public:
	Device();
	Device(std::string, double);

	std::string GetType() const { return type; }
	double GetDelay() const { return delay; }
	double GetLastCompletedTaskTime() const { return lct_time; }
	int GetCompletedTasksCount() const { return ct_counter; }
	const std::queue<Task> & GetTaskQueue() const { return taskQueue; }

	void IncreaseCompletedTasksCounter() { ++ct_counter; }
	void IncreaseProgressTime(double _t) { progress_time += _t; }

	bool Progress();

	void Dequeue() { taskQueue.pop(); }
	void Enqueue(Task _task) { taskQueue.push(_task); }

private:
	std::string type;
	double delay;
	double progress_time;
	double lct_time;
	int ct_counter;

	std::queue<Task> taskQueue;
};

#endif
