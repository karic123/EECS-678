# Chapter 5: Process synchronization

Rachel's summary notes

* [Official book slides - Chapter 5](http://codex.cs.yale.edu/avi/os-book/OS9/slide-dir/PPT-dir/ch5.ppt)

## Vocabulary

* Cooperating process
* Race condition
* Process synchronization and coordination
* The critical-section problem
	* Critical section
	* Entry section
	* Exit section
	* Remainder section
	* Preemptive kernels, non-preemptive kernels
* Peterson's solution
* Synchronization hardware
	* Locking
	* Atomically
* Mutex locks
	* Mutex stands for "Mutual exclusion"
	* Busy waiting
	* Spinlock
* Semaphores
	* Counting semaphore
	* Binary semaphore
* Deadlocks
* Starvaction/Indefinite blocking
* Priority inversion
* Priority-inheritance protocol
* Classic synchronization problems
	* The bounded-buffer problem
	* The readers-writers problem
	* The dining-philosophers problem
* Monitors
* Abstract data type (ADT)
* Conditional-wait construct
* Priority number
* Synchronization in Windows
	* Dispatcher objects
	* Events
	* Signaled state, nonsignaled state
	* Critical-section object
* Synchronization in Linux
* Synchronization in Solaris
	* Adaptive mutex
	* Turnstile
	* Priority-inheritance protocol
* Pthreads synchronization
	* Named and unnamed semaphores
* Alternatives
	* Transactional memory, Software transactional memory (STM), Hardware transactional memory (HTM)
	* OpenMP
	* Functional programming languages

---

# Notes

## Spinlocks

> In software engineering, a spinlock is a lock which causes a thread trying to acquire it to simply wait in a loop ("spin") while repeatedly checking if the lock is available. Since the thread remains active but is not performing a useful task, the use of such a lock is a kind of busy waiting. Once acquired, spinlocks will usually be held until they are explicitly released, although in some implementations they may be automatically released if the thread being waited on (that which holds the lock) blocks, or "goes to sleep".

> Because they avoid overhead from operating system process rescheduling or context switching, spinlocks are efficient if threads are likely to be blocked for only short periods.

[From Spinlocks - Wikipedia](https://en.wikipedia.org/wiki/Spinlock)

