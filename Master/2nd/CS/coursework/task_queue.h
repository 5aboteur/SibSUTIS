#ifndef _TASK_QUEUE_H_
#define _TASK_QUEUE_H_

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class TaskQueue
{
public:
	bool empty()
	{
		std::unique_lock<std::mutex> mlock(_mutex);
		size_t size = _queue.size();
		return size == 0 ? true : false;
	}

	size_t size() 
	{
		std::unique_lock<std::mutex> mlock(_mutex);
		size_t size = _queue.size();
		return size;
	}

	T front()
	{
		std::unique_lock<std::mutex> mlock(_mutex);

		while (_queue.empty())
		{
			_cond.wait(mlock);
		}

		auto item = _queue.front();
		return item;
	}

	T back()
	{
		std::unique_lock<std::mutex> mlock(_mutex);

		while (_queue.empty())
		{
			_cond.wait(mlock);
		}

		auto item = _queue.back();
		return item;
	}

	T pop()
	{
		std::unique_lock<std::mutex> mlock(_mutex);

		while (_queue.empty())
		{
			_cond.wait(mlock);
		}

		auto item = _queue.front();
		_queue.pop();

		return item;
	}

	void push(const T& item)
	{
		std::unique_lock<std::mutex> mlock(_mutex);
		_queue.push(item);
		mlock.unlock();
		_cond.notify_one();
	}

	void push(T&& item)
	{
		std::unique_lock<std::mutex> mlock(_mutex);
		_queue.push(std::move(item));
		mlock.unlock();
		_cond.notify_one();
	}

private:
	std::queue<T> _queue;
	std::mutex _mutex;
	std::condition_variable _cond;
};

#endif