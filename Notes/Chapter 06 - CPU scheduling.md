# Chapter 6: CPU Scheduling

Rachel's summary notes

* [Official book slides - Chapter 6](http://codex.cs.yale.edu/avi/os-book/OS9/slide-dir/PPT-dir/ch6.ppt)


---

# Notes

With CPU scheduling, our intent is to make our system work more efficiently
for systems that allow **multiprogramming**.

With a single processor system, only one process can run at a time,
and we use multiprogramming to run a secondary process while the 
"current" process is waiting on the completion of an I/O request.
This design is to minimize the amount of time that the CPU sits
around idle, resulting in wasted time with no work being executed.

Part of the design of our CPU scheduling deals with deciding which
thread to run next, how long to run a thread for, and at what point
to schedule the next thread to run.


## The CPU-I/O Burst Cycle

The execution of a process follows two states: **CPU Burst** and **I/O Burst**.

A **CPU Burst** is when the process is currently working, and **I/O Burst** is
the period of time that some Input or Output task is being handled.
A process jumps between these two states throughout its lifetime.


## The CPU Scheduler

The CPU Scheduler is responsible for choosing a "ready" process waiting
to be executed when the CPU is idle.

The CPU Scheduler is also known as the **short-term scheduler**.

There is a queue of processes in the **ready** state, though the design
of this queue is also a design challenge - is it simply a First-In, First-Out
Queue, a Priority Queue, or something else?


### Performance metrics for a CPU scheduler

There are several asepects of a CPU scheduler that one may take into account
when deciding how efficient a scheduling scheme is...:

* **CPU utilization** - The amount of time that the CPU is working.
* **Throughput** - The amount of jobs performed per some unit time.
* **Response-time (turn-around time)** - The time it takes to complete
a task, from the *ready* state to the *complete*/*terminated* state.
Another way of looking at this is the *sum* of the periods where the 
process was waiting to get into memory, waiting in the ready queue, executing,
and waiting on I/O.
* **Waiting time** - The amount of time a process spends in the
*ready queue* waiting to be processed.
* **Scheduling latency** - The time it takes to schedule a task,
going from the *ready* state to being scheduled for the first time.

*CPU utilization* and *Throughput* should preferably be high, while
*turnaround time*, *waiting time*, and *response time* should preferably be low.

However, we cannot min/max these aspects always; we may need to make
tradeoffs in order to make sure the CPU scheduler's average times are
adequate.


### Process lifetime

While a process is running in the CPU, its turn may end for one of several
reasons...:

* An **I/O request** is made
* Its **time slice** expires
* It **forks a child**
* It begins **waiting for an interrupt**

Once the reason has been resolved, the process returns to the **ready queue**
and waits its turn to be processed again.

The states of a process are as follows:

* **new**
* **ready**
* **running**
* **waiting**
* **terminated**

![Process states](images/process_states.png)


### CPU Decisions

The CPU scheduler will need to make a decision under the following scenarios:

1. Process goes from **running** -> **waiting**
	* No choice; a new process must be selected if any are in the ready queue.
2. Process goes from **running** -> **ready**
	* Decide whether to run that process again, or a different one.
3. Process goes from **waiting** -> **ready**
	* Decide whether to run that process, or a different one.
4. Process terminates
	* No choice; a new process must be selected if any are in the ready queue.

The way we design our scheduler will be **preemptive** or **nonpreemptive (cooperative)**
depending on its function: If we only schedule new tasks when a process goes
from *running* to *waiting*, or when a process *terminates*, then the scheduling
scheme is **nonpreemptive**. 

In the **nonpreemptive** case, the process maintains hold of the CPU
until it is either blocked (waiting on I/O or an event) or is finished;
it does not share its processing time with others while it is ready to run.

If our scheme is **preemptive**, where we share processing time between
multiple processes, we can run into **race conditions**, making
the scheduling more complex.


### The Dispatcher

The dispatcher is a component of the CPU scheduler. Once the short-term
scheduler has selected a process, the dispatcher gives control of the CPU
to this process.

The dispatcher has the following functionaliy:

* Switching contexts
* Switching to user mode
* Resuming a program by jumping to the location in the program where it left off.

The Dispatcher comes into play every time there is a process switch. Therefore,
we are also concerned with **dispatch latency**.


## Types of scheduling algorithms

There are different types of scheduling algorithms to consider.

### The Gantt chart

The Gantt chart is a way that we can illustrate a schedule.


### First-Come, First-Served Scheduling

* [Wikipedia article](https://en.wikipedia.org/wiki/Scheduling_(computing)#First_come.2C_first_serve)

The FCFS scheduling scheme is easy to implement, but not very efficient.

The first process that requests the CPU is the first process that receives
processing time. This means that for a set of multiple tasks passed in,
the resulting wait time can vary widely based on the order of these tasks.

A **Convoy Effect** may also occur, where one big process "leads the pack"
of smaller processes, as it is the resource hog and ends up blocking
the CPU while the smaller processes wait.

Also the FCFS scheduling scheme is **nonpreemptive**.

**PROS:** Simple to implement, low overhead, and good for batch jobs.

**CONS:** The average response time sucks, and is also unreliable! Short jobs
may end up becoming stuck behind longer jobs.


### Shortest-Job-First Scheduling

For this scheme, the process with the smallest next CPU burst
is scheduled. If two processes have the same CPU burst size,
then FCFS (first come first serve) is used to select which one goes.

The challenge here is *how to figure* the length of each process.

SJF scheduling is used more commonly with long-term job scheduling,
rather than as short-term CPU scheduling, as we cannot know how long
of a CPU burst a process will need.

**PROS:** Good average response time. Shortest job is performed first.

**CONS:** Can't predict the future. Long jobs may be starved by
the short jobs.

We can try to approximate this scheduling for short-term by estimating
the CPU burst length of a process. This might mean looking at how long
its last CPU burst took, and assume subsequent ones will be of similar length.
We use an **exponential weighted moving average** of the past CPU bursts
to estimate for future ones.

(And I really hope that we don't have to memorize the formula stuff.)

The SJF algorithm can be **preemptive** or **nonpreemptive**.
Preemptive SJF is also known as "Shortest-remaining-time-first scheduling".

We might also have to account for processes not arriving at the same time.
So it might be better to think of this as "shortest remaining time first",
and allow the scheduling to be preemptive so that processes can be scheduled
more efficiently.


### Priority Scheduling

Priority scheduling is simply selecting the next process based on its
given *priority*. The SJF algorithm is a *type of* priority scheduling.

In the textbook we use for class, they use **low numbers** to represent
items of **high priority**.

Priority of a process can be defined either internally or externally.

Internal priority may come from # of files open by the process,
average I/O burst time : average CPU burst time, a time limit, or
memory requirements.

External priority may come from the user-defined importance of the process.

The textbook seems a bit salty about acadmic politics and funding here.

Additionally, priority scheduling can be preemptive or nonpreemptive.

**CONS:** A problem that occurs with priority scheduling is the threat of
starvation. This means that a process is constantly preempted by higher-priority
items, and it never receives any processing time.

One way to combat this is by **aging** the process - increasing its
priority the longer it lives, so that it will eventually get some processing time.


### Round-Robin Scheduling

* [Wikipedia article](https://en.wikipedia.org/wiki/Scheduling_(computing)#Round-robin_scheduling)

Round-Robin Scheduling is essentially First-Come, First-Served but with
the ability to preempt. It was designed especially for time-sharing systems.

For Round-Robin, each job is executed for some fixed time slice,
known as a **time quantum**. The Ready Queue is now a circular queue.

The scheduler "travels" around the queue, giving each process a bit
of time to work, up to one *time quantum*.

The ready queue is essentially still *First-In, First-Out*, but a process
doesn't get to spend the entire time processing, blocking others
while it does so. New processes are added to the tail of the queue.

**CONS:** The average wait time AND turnaround time is still pretty long 
on average, though
the performance relies greatly on the length of the time quantum.

If the time quantum is very small, then overhead is added in the form of
*context switching*.

If the time quantum is large, then the response time is bad.

**PROS:** Better interactivity, low average scheduling latency.

**CONS:** Performance depends on the quantum size.


### Multilevel Queue Scheduling

* [Wikipedia article](https://en.wikipedia.org/wiki/Scheduling_(computing)#Multilevel_queue_scheduling)

It may be handy to divide our processes in groups, such as **Foreground** and **Background** processes.
Foreground processes are interactive, and background processes are batch.

With the **Multilevel Queue** scheme, we maintain separate queues for
the separate groups, and the separate queues have separate algorithms
they follow.

Additionally, different groups may overall have different priorities,
like the foreground queue might have priority over the background queue.
This is known as *fixed-priority preemptive scheduling.*

Another alternative is to use time-slices for each of the queues.

**PROS:** Low scheduling overhead

**CONS:** Inflexible


### Multilevel Feedback Queue Scheduling

With a Multilevel Feedback Queue, processes are allowed to move between
the queues.

This might be useful if a process takes a lot of CPU time, and it
can be moved to a lower-priority queue. Likewise, if a process uses
a lot of I/O time, it can go in a high-priority queue.

We can also implement aging by allowing a process move from low-priority
to higher-priority if it sits in the low-priority queue for too long.

A Multilevel Feedback Queue scheduler is defined by the following:

* The number of queues it has
* The scheduling algorithm used for each queue
* The means by which a decision is made to move a process to a higher-priority queue
* The means by which a decision is made to move a process to a lower-priority queue
* The way it decides which queue a process will be assigned to.

**PROS:** Most general algorithm. Most flexibility.

**CONS:** Most complex algorithm.


### Completely Fair Scheduler

* [Wikipedia article](https://en.wikipedia.org/wiki/Completely_Fair_Scheduler)

Linux uses CFS, the Completely Fair Scheduler. For this scheme,
each task owns a fraction of the CPU time share.

For the scheduling algorithm, each task keeps track of its "virtual runtime",
which is the **amount of time executed * weight**.

The scheduler chooses the task that has the *smallest* virtual runtime,
and tasks are sorted according to the virtual runtimes.

The CFS is not based on run queues, but on a red-black tree.

Edge-cases ? For a new task, what do we set the virtual time to?

#### Red-Black Tree

A Red-Black Tree is used in CFS to implement a "timeline" of task
executions to be done. This type of tree structure is self-balancing,
as well as a binary search tree.

#### Weighted Fair Sharing


### Comparisons

What has the smallest average wait time? -- **SRTF/SFJ**.

What has better interactivity? **RR with a small time quantum, or SRTF**.

What is best for minimizing overhead? **FCFS**


## Real-time Scheduling

* [Wikipedia article](https://en.wikipedia.org/wiki/Real-time_computing)

	In computer science, real-time computing (RTC), 
	or reactive computing describes hardware and software 
	systems subject to a "real-time constraint", 
	for example from event to system response.[1] 
	Real-time programs must guarantee response within 
	specified time constraints, often referred to as "deadlines".[2] 
	
	The correctness of these types of systems depends on their 
	temporal aspects as well as their functional aspects. 
	Real-time responses are often understood to be in the 
	order of milliseconds, and sometimes microseconds.
	
	(From https://en.wikipedia.org/wiki/Real-time_computing)

For real-time systems, processes are trying to react to, or controlling, events
that are taking place in the real world. These events take place in
"real time" and the system must be able to keep up with these events.
There are timing constraints such as deadlines.

The real-time aspect of events belongs on a spectrum. At the "Hard RT" end,
we might have flight control. In the middle, towards "Soft RT", we
may have internet communication - video, audio, etc. And towards "No RT",
we may have a computer simulation.

The ultimate goal is to meet the deadlines of our tasks. **A soft deadline**
is something less important - video game events, video decoding, etc.
A **hard deadline** is much more important and could result in the loss of
life if something goes wrong - Engine control, anti-lock break system, etc.

Additionally, a **soft real-time system** provides no guarantees for when
a critical real-time process will be scheduled. A **hard real-time system** 
has stricter deadlines, and it must be executed by its deadline, otherwise
afterwards it is useless.

We can also do priority scheduling on real-time systems, which may include
**static priority scheduling** or **dynamic priority scheduling**.

### Latency

**Event latency** is the time it takes from when an event occurs until
when that event is serviced.

**Interrupt latency** is the time it takes from when an interrupt occurs
until the start of the response process.

**Dispatch latency** is the time it takes for the dispatcher to stop
one process and start another process. The best way to keep this latency
low is to allow preemption.

**Interrupt latency** and **Dispatch latency** affect a real-time
system's performance.


### Scheduling for real-time systems

Since real-time systems need to respond as soon as possible to external
information, CPU scheduling must require preemption (allow interrupts)
and the system must also support priority raking of processes.

However, only providing a scheduler that allows for prioritization and
preemption **does not** guarantee hard real-time functionality;
it merely guarantees soft real-time functionality.
To have hard real-time functionality, there must be some guarantee
that tasks will be processed by their deadlines.


#### Characteristics of processes in real-time systems

In a real-time system, the processes must be consudered **periodic**.
This means that they require the CPU at constant time periods.

When a process is created, its deadline may also be included as 
information going to the scheduler. Using an **admission-control algorithm**,
the scheduler will either *accept the process* with the guarantee that it
will be processed by the deadline, or it will *reject the process* if
it is unable to *guarantee* that the process will be taken care of by the deadline.


#### Scheduling methods for real-time systems

**Rate-Monotonic scheduling:** Tasks are prioritized based on their periods.
If a task has a short period, then it is higher priority, and vice versa.

The rate-monotonic scheduling algorithm utilizes a static priority policy
and it allows for preemption.

**Earliest-deadline-first (EDF):** Priorities are assigned based on the
deadline. If the deadline comes sooner, then the task is higher priority.
If the deadline is further away, then the task is lower priority.

EDF scheduling doesn't require periodic processes, or constant CPU burst
time, only that a process provides its deadline to the scheduler.

**Proportional share:** These schedulers will allocate a specific amount
of shares, *T*, to each application. The application will also receive
*N* shares of time, so that it will have *N/T* total time processing.

An **admission-control policy** must also be used so that shares of time
are properly received.


#### Linux scheduling framework

To go over later, FML.


### Multicore Scheduling

FML

### Algorithm Evaluation

FML

---

# Vocabulary

* Basic concepts
	* CPU-I/O Burst Cycle
		* Cycle
		* CPU burst
		* I/O burst
	* CPU scheduler
		* Short-term scheduler
	* Preemptive scheduling
* Scheduling schemes
	* Nonpreemptive
	* Cooperative
	* Preemptive
* Dispatch latency
* Scheduling criteria
* Scheduling algorithms
	* First-Come, First-Served scheduling (FCFS)
		* Gantt chart
		* Convoy effect
	* Shortest-Job-First scheduling (SJF) aka Shortest-Remaining-Time-First
		* Exponential average
	* Priority scheduling
		* Indefinite blocking / starvation
		* Aging
	* Round-Robin scheduling (RR)
		* Time quantum / time slice
	* Multilevel Queue scheduling
		* Foreground (interactive) processes
		* Background (batch) processes
	* Multilevel Feedback Queue scheduling
* Thread scheduling
	* Process-contention scope (PCS)
	* System-contention scope (SCS)
* Multiple-processor scheduling
	* Load sharing
	* Asymmetric multiprocessing
	* Symmetric multiprocessing (SMP)
	* Processor affinity
		* Soft affinity
		* Hard affinity
	* Load balancing
		* Push migration
		* Pull migration
	* Multicore processors
		* Memory stall
		* Coarse-grained
		* Fine-grained
* Real-time CPU Scheduling
	* Soft real-time systems
	* Hard real-time systems
	* Event latency
		* Interrupt latency
		* Dispatch latency
		* Conflict phase
	* Priority-based Scheduling
		* Periodic
		* Rate
		* Admission-control algorithm
	* Rate-monotonic scheduling
	* Earliest-Deadline-First scheduling (EDF)
	* Proportional Share scheduling
	* POSIX Real-Time scheduling
* Operating system examples
	* Linux
		* Completely Fair Scheduler
		* Scheduling classes
		* Nice value
		* Targeted latency
		* Virtual run time
	* Windows
		* Dispatcher
		* Variable class
		* Real-time class
		* Idle thread
		* Foreground process
		* Background process
		* User-mode scheduling (UMS)
		* Fibers
		* Concurrency Runtime (ConcRT)
	* Solaris
* Algorithm evaluation
	* Deterministic modeling
		* Analytic evaluation
		* Deterministic modeling
	* Queueing models
		* Queueing-network analysis
		* Little's formula
	* Simulations
		* Trace tapes
	* Implementation
