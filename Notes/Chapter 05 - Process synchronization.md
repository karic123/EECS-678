# Chapter 5: Process synchronization

Rachel's summary notes

* [Official book slides - Chapter 5](http://codex.cs.yale.edu/avi/os-book/OS9/slide-dir/PPT-dir/ch5.ppt)

---

# Notes

## Mutual exclusion


### Race condition

A **race condition** is a problem where two or more threads attempt to
access shared data at the same time. (read/write at same time).

Whether the result is correct depends on the order of execution of the threads.

To prevent this problem, we first need to identify our **critical sections**
in the program - locations where shared data is being accessed by
the thread.

We need to make sure that there is **mutual exclusion**, so that when
one thread is accessing the data in a critical section, no other thread
can access it at the same time.

If the critical section isn't currently being accessed, then a thread
should be able to work with that data.

Also, there must be a limit on the amount of time a thread is left waiting.(?)


### Critical sections

A **critical section** is the portion of code where shared variables/resources may
be modified. We need to make sure that only one thread at a time can access
this data.

To do this, we think about how we have the threads request entry into
the critical section - the requesting part of code is the **entry section**.
There also needs to be a way for the thread to leave the critical section -
the **exit section**.

In order to solve critical section problems, we must satisfy these three requirements:

1. **Mutual exclusion** - We must protect the critical region and make sure
that only one thread can access it at a time.
2. **Progress** - We must ensure that if nothing is currently in the critical region,
then any thread can enter it.
3. **Bounded waiting** - We need to make sure that there is a limit on
the amount of times that processes are able to enter their critical sections
after some other process has made a request to enter that critical section
and before that request is granted.


A **Preemptive Kernel** is where a process running in kernel mode may be preempted.

**PROS:** A preemptive kernel can be more responsive, and is more suitable
for real-time programming.

A **Nonpreemptive Kernel** is where a process running in kernel mode may not be preempted.





## Synchronization mechanisms

We can utilize various locking mechanisms to protect critical regions
in our software. There are several different ways to achieve this.

The textbook uses general functions *test_and_set()* and *compare_and_swap()*
in some examples. The idea behind both of these is that they are executed atomically.

	// I guess *target will always be true, and the idea is that
	// it will return either true or false for what the *target
	// value originally was, and it will "lock" the *target value
	// as true (if going from false -> true), otherwise no change
	// (if going from true -> true)
	bool test_and_set( bool * target )
	{
		bool rv = *target;
		*target = true;
		return *rv;
	}

	// Check if it is the expected value. If so, replace with new value.
	in compare_and_swap( int * value, int expected, int new_value )
	{
		int temp = *value;
		
		if ( *value == expected )
		{
			*value = new_value;
		}
		
		return temp;
	}







### Mutex

A mutex lock is a way to protect a critical section. Any process wanting
to get to the critical section must acquire the lock first, and then
release the lock once it is done.
If a region is locked, then other processes are not allowed in.

For a mutex lock, the *acquire* and *release* functions must be *atomic*!
Or else we'll all dieeeee!!!

Acquire():

	acquire()
	{
		while ( !available )
		{
			; // wait
		}
		available = false;
	}

**PROS:** No context switch happens when a process is waiting on a lock.
If locks are only held for a short amount of time, then spinlocks may be useful.

**CONS:** This implementation makes a thread sit around and wait while
it wants access to a section. This is **busy waiting**. This type of
mutex is called a spinlock.


#### Spinlocks

> In software engineering, a spinlock is a lock which causes a thread trying to acquire it to simply wait 
in a loop ("spin") while repeatedly checking if the lock is available. Since the thread remains active but 
is not performing a useful task, the use of such a lock is a kind of busy waiting. Once acquired, spinlocks
will usually be held until they are explicitly released, although in some implementations they may be 
automatically released if the thread being waited on (that which holds the lock) blocks, or "goes to sleep".

> Because they avoid overhead from operating system process rescheduling or context switching, 
spinlocks are efficient if threads are likely to be blocked for only short periods.

[From Spinlocks - Wikipedia](https://en.wikipedia.org/wiki/Spinlock)


### Semaphore

A Semaphore is another kind of lock, but it is an integer valeu instead of
a boolean. The semaphore is accessed via two atomic operations *wait()*
and *signal()*.

Note: Sometimes *wait()* is referred to as *P()*, and *signal()* is referred to as *V()*.
Because Dutch words, and anyone who uses single-letter function names is 
**A TERPOMANTO**.

Eble mi devas skribi miajn tutajn notojn en Esperanto do nur la Esperantistoj
povas legi ĝin?! BONIDEO, ĈU NE?!?!?!?!

Fuck.

Anyway.

Here's a wait and signal.

wait():

	wait( int semaphore )
	{
		while ( semaphore <= 0 )
		{
			// Busy wait
		}
		semaphore--;
	}
	
signal():

	signal( int semaphore )
	{
		semaphore++;
	}

wait() and signal() should not be updating *semaphore* at the same time.

...

* **Counting semaphore:** can have any value.
* **Binary semaphore:** can be 0 or 1. Similar to a mutex lock.

With a counting semaphore, we can keep track of resources. We can initialize
the value of the *semaphore* to the amount of resources available,
and increment/decrement as the resources become available/unavailable.

Instead of sitting around in a **busy waiting** state, when a semaphore
finds that the resources is unavailable, it can block itself and put
itself in a waiting queue.

If the semaphore becomes negative, then |semaphore| is the amount
of processes waiting on the semaphore lock.

Note that if we use a waiting queue with our semaphores, a case could
arise where multiple processes wait indefinitely, and therefore starve.
This is known as a **deadlocked** state.


**CONS:**

* They are a low-level primitive
* They are used for both mutual exclusion and for scheduling
* They are very easy to get wrong


#### Priority inversion

Sometimes, a lower priority process is holding up the line and a 
higher priority job needs to get done. This is known as **priority inversion**.

The problem can be solved by **priority-inheritance protocol**, where 
processes inherit the higher priority when dealing with high priority resources,
then go back to normal once finished. This helps with preemption, or something.



### Monitor

It can be difficult to implement semaphores correctly, and it can be
difficult to detect errors that arise from using semaphores.

A Monitor type can be used to help with this problem.

	In concurrent programming, a monitor is a synchronization construct that allows 
	threads to have both mutual exclusion and the ability to wait (block) for a 
	certain condition to become true. Monitors also have a mechanism for signalling 
	other threads that their condition has been met.
	
	From https://en.wikipedia.org/wiki/Monitor_(synchronization)

RACHEL DO MORE READING ON MONITORS LATER OK?


## Synchronization problems

What could possibly go wrong?

### The bounded-buffer problem (aka the producer-consumer problem)

	In computing, the producer–consumer problem[1][2] (also known as the bounded-buffer problem) 
	is a classic example of a multi-process synchronization problem. The problem describes two processes, 
	the producer and the consumer, who share a common, fixed-size buffer used as a queue. The producer's 
	job is to generate data, put it into the buffer, and start again. At the same time, the consumer is 
	consuming the data (i.e., removing it from the buffer), one piece at a time. The problem is to make 
	sure that the producer won't try to add data into the buffer if it's full and that the consumer won't 
	try to remove data from an empty buffer.

	The solution for the producer is to either go to sleep or discard data if the buffer is full. 
	The next time the consumer removes an item from the buffer, it notifies the producer, who starts
	to fill the buffer again. In the same way, the consumer can go to sleep if it finds the buffer to 
	be empty. The next time the producer puts data into the buffer, it wakes up the sleeping consumer. 
	The solution can be reached by means of inter-process communication, typically using semaphores. 
	An inadequate solution could result in a deadlock where both processes are waiting to be awakened. 
	The problem can also be generalized to have multiple producers and consumers.

	From https://en.wikipedia.org/wiki/Producer%E2%80%93consumer_problem


### The readers-writers problem

	In computer science, the readers-writers problems are examples of a common computing problem 
	in concurrency. There are at least three variations of the problems, which deal with situations 
	in which many threads try to access the same shared resource at one time. Some threads may read
	and some may write, with the constraint that no process may access the shared resource for either 
	reading or writing while another process is in the act of writing to it. (In particular, it is 
	allowed for two or more readers to access the share at the same time.) A readers-writer lock is 
	a data structure that solves one or more of the readers-writers problems.
	
	From https://en.wikipedia.org/wiki/Readers%E2%80%93writers_problem



### The dining-philosophers problem

I think this problem is stupid.

	Five silent philosophers sit at a round table with bowls of spaghetti. Forks are placed between 
	each pair of adjacent philosophers.

	Each philosopher must alternately think and eat. However, a philosopher can only eat spaghetti when 
	they have both left and right forks. Each fork can be held by only one philosopher and so a philosopher 
	can use the fork only if it is not being used by another philosopher. After an individual philosopher 
	finishes eating, they need to put down both forks so that the forks become available to others. A 
	philosopher can take the fork on their right or the one on their left as they become available, but 
	cannot start eating before getting both forks.

	Eating is not limited by the remaining amounts of spaghetti or stomach space; an infinite supply and 
	an infinite demand are assumed.

	The problem is how to design a discipline of behavior (a concurrent algorithm) such that no philosopher 
	will starve; i.e., each can forever continue to alternate between eating and thinking, assuming that no 
	philosopher can know when others may want to eat or think.

	From https://en.wikipedia.org/wiki/Dining_philosophers_problem#Problem_statement
	


---

# Vocabulary

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
