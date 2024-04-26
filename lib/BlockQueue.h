#pragma once
#include <mutex>
#include <condition_variable>
#include <queue>
#include <exception>
#include "Logger.h"
#include <cassert>

//maxSize is a variable that makes sure you don't more than needed items in the queue.
template<typename E>
class BlockQueue
{
private:
   std::mutex mtx;
   std::condition_variable condVar;
   size_t maxSize{}; //maxSize is a number that sets the limit of thread pop and push operation each time for each object
   std::queue<E> blockQueue;

public:
    BlockQueue(const size_t& maxSize) : maxSize(maxSize) {
        assert(maxSize > 0);
    }
    void push(const E& item) {
        std::unique_lock<std::mutex> lock(mtx);
        condVar.wait(lock, [&](){ return blockQueue.size() < maxSize; });
        blockQueue.push(item);
        lock.unlock();
        condVar.notify_one();
        Logger::INFO("The item is added to the Queue.\n");
    }
    size_t getSize() {
        std::lock_guard<std::mutex> lock(mtx);
        return blockQueue.size();
    }
    E front(){
        std::unique_lock<std::mutex> lock(mtx);
        condVar.wait(lock, [&](){ return !blockQueue.empty(); });
        if (!blockQueue.empty()) {
            auto item = blockQueue.front();
            lock.unlock();
            return item;
        }
        else {
            throw std::runtime_error("The Queue is empty!\n");
        }
    }
    void pop(){
        std::unique_lock<std::mutex> lock(mtx);
        condVar.wait(lock, [&](){ return !blockQueue.empty(); });
        if (!blockQueue.empty()) {
            blockQueue.pop();
            lock.unlock();
            condVar.notify_one();
            Logger::INFO("The item is moved from the Queue.\n");
        }
        else {
            throw std::runtime_error("The queue is empty!\n");
        }
    }
    ~BlockQueue() = default;
};
