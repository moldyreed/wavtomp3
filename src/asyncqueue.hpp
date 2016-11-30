#pragma once

#include<memory>
#include<queue>
#include<mutex>
#include<condition_variable>

template<typename T>
class AsyncQueue
{
private:
	std::shared_ptr<std::queue<T>> queue;
	std::shared_ptr<std::mutex> mutex;
	std::shared_ptr<std::condition_variable> cond;

public:
	/* Big 5 */
	AsyncQueue(const AsyncQueue& other) = default;
	AsyncQueue(AsyncQueue&& other) = default;
	AsyncQueue& operator=(const AsyncQueue& other) = default;
	AsyncQueue& operator=(AsyncQueue&& other) = default;
	virtual ~AsyncQueue() = default;

	/**
	 * @brief Default constructor
	 */
	AsyncQueue() :
		queue(new std::queue<T>()),
		mutex(new std::mutex),
		cond(new std::condition_variable())
	{
	}

	/**
	 * @brief Push a value to the async queue
	 */
	void push(const T& object)
	{
		{
			std::lock_guard<std::mutex> lock(*mutex);
			queue->push(object);
		}
		cond->notify_one();
	}

	/**
	 * @brief Push a value to the async queue (move variant)
	 */
	void push(T&& object)
	{
		{
			std::lock_guard<std::mutex> lock(*mutex);
			queue->push(object);
		}
		cond->notify_one();
	}

	/**
	 * @brief Pop a value from the queue
	 */
	T pop()
	{
		std::unique_lock<std::mutex> lock(*mutex);

		while (queue->empty())
		{
			cond->wait(lock);
		}

		T object = std::move(queue->front());
		queue->pop();
		return std::move(object);
	}
};