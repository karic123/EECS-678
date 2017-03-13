# Chapter 

Rachel's summary notes

* [Official book slides - Chapter 7](http://codex.cs.yale.edu/avi/os-book/OS9/slide-dir/PPT-dir/ch7.ppt)



---

# Notes

## Deadlocks

	In concurrent computing, a deadlock is a state in which each member 
	of a group of actions, is waiting for some other member to release 
	a lock.

	From https://en.wikipedia.org/wiki/Deadlock

For a deadlock to occur, all four of the following conditions must be met:

1. **Mutual exclusion**
2. **Hold and wait**
3. **No preemption**
4. **Circular wait** - Process A may be waiting for a region to be freed by
Process B, and vice versa, meaning that neither ever gain access to the area
that they want.

### Resource-allocation graphs



## Managing deadlock problems

### Handling deadlocks




### Deadlock prevention




### Deadlock avoidance




### Deadlock detection

Deadlock detection is required if we do not utilize deadlock avoidance or prevention.

We will need to be able to figue out whether a deadlock has occured by
examening the state of the system.

Being able to detect and recover from deadlocks also means additional overhead.
We have the cost of having to store information to be able to detect a deadlock,
but also the time it takes to analyze and resolve deadlocks.

#### Single instance of each resource type

#### Several instances of a resource type

#### Detection-algorithm usage

When do we use our detection algorithm? Well, how frequently might a 
deadlock occur? And, when it does happen, how many processes will it affect?





### Recovery from deadlock

Once we have detected that a deadlock has occurred, we need some manner to
resolve the problem. This is known as **recovery**.

#### Process termination

We can terminate a process in order to alieviate a deadlock. The system
will reclaim the resources used by the process in order to help other
processes continue.

* **Method 1: Abort all deadlocked processes** - This is expensive,
though it will definitely break the deadlock. Any works-in-progress are lost
and will be to be restarted.

* **Method 2: Abort one process at a time until the deadlock is elimiated**
- This will cost additional overhead since each time a process is terminated
the detector has to re-analyze the processes to look for deadlocks.

Additionally, terminating a process could corrupt data, such as a file
that was only partially written.

If we're only terminating one process at a time, we must also figure out
*which* process we want to kill. We may need to take into account the
process' priority, how long it has been running, what resources the
process is utilizing, what resources it needs, and whether the process
is foreground or background.


#### Resource preemption

For this method, we alieviate the deadlocks by preempting resources
that are needed by some of the deadlocked processes. We do this until
the deadlock is cleared.

This also has some challenges: Which process do we preempt and take its
resources from? If we preempt that process, do we restart the process or
otherwise roll it back to a safe state? And how do we ensure that
starvation does not occur, such as if a process is constantly being
preempted due to deadlocks.

---

# Vocabulary

* Deadlock
* System model
* Deadlock characterization
	* Necessary conditions
	* Resource-allocation graph
		* System resource-allocation graph
		* Request edge
		* Assignment edge
* Methods for handling deadlocks
	* Deadlock prevention
	* Deadlock avoidance
* Deadlock prevention
	* Mutual exclusion
	* Hold and wait
	* No preemption
	* Circular wait
* Deadlock avoidance
	* Safe state
	* Resource-Allocation-Graph algorithm
	* Banker's algorithm
* Deadlock detection
	* Single instance of each resource type
	* Several instances of each resource type
	* Detection-algorithm usage
* Recovery from deadlock
	* Process termination
	* Resource preemption
