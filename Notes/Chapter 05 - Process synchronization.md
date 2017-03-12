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




**CONS:**

* They are a low-level primitive
* They are used for both mutual exclusion and for scheduling
* They are very easy to get wrong


### Monitor



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
