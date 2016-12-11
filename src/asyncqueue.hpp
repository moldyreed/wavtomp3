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

    bool hasMoreItems()
    {
        std::lock_guard<std::mutex> lock(*mutex);
        return !queue->empty();
    }

    void push(const T& object)
    {
        {
            std::lock_guard<std::mutex> lock(*mutex);
            queue->push(object);
        }
        cond->notify_one();
    }

    void push(T&& object)
    {
        {
            std::lock_guard<std::mutex> lock(*mutex);
            queue->push(object);
        }
        cond->notify_one();
    }

    T pop()
    {
        std::unique_lock<std::mutex> lock(*mutex);

        while (queue->empty()) {
            cond->wait(lock);
        }

        T object = std::move(queue->front());
        queue->pop();
        return std::move(object);
    }
};
