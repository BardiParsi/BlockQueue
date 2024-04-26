# Block Queue Pattern

The BlockQueue pattern is a thread-safe queue implementation designed for concurrent multi-threaded environments. It ensures synchronized access to the underlying queue, preventing data races and ensuring thread safety. This implementation allows multiple threads to push and pop elements from the queue concurrently.

## Features

- **Thread Safety:** Utilizes mutex and condition variables to ensure safe concurrent access.
- **Blocking Operations:** Supports blocking push and pop operations, ensuring that threads wait when the queue is full or empty.
- **Dynamic Size:** Allows the specification of a maximum size for the queue, providing flexibility in resource utilization.
- **Simple Integration:** Easy-to-use interface for integrating into multi-threaded applications.

# Implementation

BlockQueue.h: Header file containing the implementation of the BlockQueue pattern.
Logger.h and Logger.cpp: Implementation of a simple logging utility for console output.

# Dependencies

C++20 or later

# License

This project is licensed under the GNU General Public License v3.0 - see the LICENSE file for details.

## Usage

```cpp
#include "BlockQueue.h"

// Example usage
BlockQueue<int> queue(10); // Create a queue with a maximum size of 10

// Push elements into the queue
queue.push(10);
queue.push(20);

// Pop elements from the queue
int item = queue.front();
queue.pop();
