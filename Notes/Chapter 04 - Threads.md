# Chapter 4: Threads

Rachel's summary notes

* [Official book slides - Chapter 4](http://codex.cs.yale.edu/avi/os-book/OS9/slide-dir/PPT-dir/ch4.ppt)

## Vocabulary

* [Threads](https://en.wikipedia.org/wiki/Thread_(computing))
* [Single-threaded process](https://en.wikipedia.org/wiki/Thread_(computing)#Single_threading)
* [Multi-threaded process](https://en.wikipedia.org/wiki/Thread_(computing)#Multithreading)
* Multicore/multiprocessor system
* Data parallelism
* Task parallelism
* User threads
* Kernel threads
* Green threads
* Multithreading models
	* One-to-one model
	* Many-to-many model
	* Many-to-one model
* Thread libraries
	* POSIX
* Pthreads, Windows threads, Java threads
* Implicit threading
* Thread pools
* OpenMP - Compiler directives and API
* Parallel regions
* Grand Central Dispatch - Apple's extentions to C, API
	* Blocks
	* Dispatch queue
	* Main queue
* Threading issues
	* fork() and exec()
	* Signal handling
		* Default signal handler
		* User-defined singal handler
		* Asynchronous procedure calls (APCs)
	* Thread cancellation
		* Target thread
		* Asynchronous cancellation
		* Deferred cancellation
		* Cancellation point
		* Cleanup handler
	* Thread-local storage (TLS)
	* Scheduler activations
		* Lightweight process (LWP)
		* Upcall, upcall handler


---

# Notes

## Threads

![Single-threaded vs Multi-threaded](images/single_multi_threaded.png)

An individual thread has:

* Thread ID
* Counter
* Register set
* Stack

And the threads of a process share:

* Code section
* Data section
* OS resources (open files, signals)



## Producer/consumer

> In computing, the producer–consumer problem[1][2] (also known as the bounded-buffer problem) is a classic example of a multi-process synchronization problem. The problem describes two processes, the producer and the consumer, who share a common, fixed-size buffer used as a queue. The producer's job is to generate data, put it into the buffer, and start again. At the same time, the consumer is consuming the data (i.e., removing it from the buffer), one piece at a time. The problem is to make sure that the producer won't try to add data into the buffer if it's full and that the consumer won't try to remove data from an empty buffer.

> The solution for the producer is to either go to sleep or discard data if the buffer is full. The next time the consumer removes an item from the buffer, it notifies the producer, who starts to fill the buffer again. In the same way, the consumer can go to sleep if it finds the buffer to be empty. The next time the producer puts data into the buffer, it wakes up the sleeping consumer. The solution can be reached by means of inter-process communication, typically using semaphores. An inadequate solution could result in a deadlock where both processes are waiting to be awakened. The problem can also be generalized to have multiple producers and consumers.

[Wikipedia - Producer-consumer problem](https://en.wikipedia.org/wiki/Producer%E2%80%93consumer_problem)

## Deadlocks
