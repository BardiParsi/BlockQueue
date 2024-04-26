#include "lib/BlockQueue.h"
#include "lib/Logger.h"
#include <thread>
#include <cstdlib>
#include <future>
#include <vector>
#include <iostream>
#include <chrono>
#include <random>
#include <cassert>
#include <exception>
/*
- This project requires c++20 or above
- Asserts are removable and it's only for debugging purposes and should be removed for release version
- This is a Block Queue pattern used in multithreading programs. This makes sure that the queue prevents data racing
and dead locks.
*/


using std::cout;
using std::endl;

const size_t CPUThreadNum = std::thread::hardware_concurrency(); //The number of CPU Threads in the system

//generating random numbers to push pack to the queue
int generateRandNum(){
    static std::mt19937_64 mtRand(std::random_device{}());
    std::uniform_int_distribution<int> randDist(1, 100);
    return randDist(mtRand);
}

//This function generates shared future object which contains random integers from 1 to 100 for the queue object
std::shared_future<int> generateShrFut(){
    return std::async(std::launch::async, [](){ return generateRandNum(); });
}

//function that pushes the shared future object to the queue
void pushToQueue(BlockQueue<std::shared_future<int>>& queue){
    auto fut = generateShrFut();
    assert(fut.valid());
    try {
        queue.push(fut);
    }
    catch (const std::exception& e) {
        std::cerr << "Exception is caught! " << e.what() << endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

//function that read the front item of queue and pop it out for testing demostration
void popFromQueue(BlockQueue<std::shared_future<int>>& queue) {
    try {
        auto fut = queue.front();
        assert(fut.valid());
        int result = fut.get();
        {
            std::lock_guard<std::mutex> lock(Logger::logMtx);
            cout << "The front value of Queue is: " << result << endl;
        }

        queue.pop();
    }
    catch(const std::exception& e) {
        std::cerr << "Exception is caught! " << e.what() << endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}
/*
The maxSize can be any size that you want to grow the queue. It does not necessary needs to be equal to CPU Thread
numbers. In this case we just made it the same for the sake of observation and example.
*/
int main() {
    const size_t maxSize = CPUThreadNum; //maxSize can be any number you want to set push and pop process limitation.
    assert (maxSize > 1); //all the asserts should be removed for release version
    BlockQueue<std::shared_future<int>> queue(maxSize); //the queue can contain any sort of item
    std::vector<std::jthread> threads;
    //using maxSize for the loop limits helps to prevent data racing.
    for (size_t i = 0; i < maxSize; i++) {
        threads.emplace_back(pushToQueue, std::ref(queue));
    }
    for (size_t i = 0; i < maxSize; i++) {
        threads.emplace_back(popFromQueue, std::ref(queue));
    }

    for (auto& t : threads) {
        t.join();
    }

    std::system("pause"); //this line is for vscode to keep the console on. This may be removed according to the project

    return 0;
}
