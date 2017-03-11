# Chapter 3: Processes

Rachel's summary notes

* [Official book slides - Chapter 3](http://codex.cs.yale.edu/avi/os-book/OS9/slide-dir/PPT-dir/ch3.ppt)

## Vocabulary

* Process concepts
	* [Process](https://en.wikipedia.org/wiki/Process_(computing))
		* Parent process
		* Child process
		* Sibling process
	* Jobs
	* User programs / tasks
	* The Process...
		* Text section
		* [Program counter](https://en.wikipedia.org/wiki/Program_counter)
		* [Stack pointer](https://en.wikipedia.org/wiki/Call_stack#STACK-POINTER)
		* Process stack
		* Data section
		* Process heap
	* Process state
		* States
			* New
			* Running
			* Waiting
			* Ready
			* Terminated
	* Process control block (PCB) aka Task control block
		* Process state
		* Program counter
		* CPU registers
		* CPU-scheduling information
		* Memory-management information
		* Accounting information
		* I/O status information
	* Threads
* Process scheduling
	* Process scheduler
	* Scheduling queues
		* Job queue
		* Ready queue
		* Device queue
		* Queueing diagram
		* Dispatched
	* Schedulers
		* [Long-term scheduler / job scheduler](https://en.wikipedia.org/wiki/Scheduling_(computing)#LONG-TERM)
		* [Short-term scheduler / CPU scheduler](https://en.wikipedia.org/wiki/Scheduling_(computing)#Short-term_scheduling)
		* [Medium-term scheduler](https://en.wikipedia.org/wiki/Scheduling_(computing)#Medium-term_scheduling)
		* [I/O-bound process](https://en.wikipedia.org/wiki/I/O_bound)
		* [CPU-bound process](https://en.wikipedia.org/wiki/CPU-bound)
		* Swapping
	* [Context switching](https://en.wikipedia.org/wiki/Context_switch)
		* State save, state restore
* Operations on processes
	* Process creation
		* Tree of processes
		* Process identifier (pid)
	* Process termination
		* Foreground application, background application
		* Cascading termination
		* Zombie process
		* Orphan process
	* [Interprocess communication (IPC)](https://en.wikipedia.org/wiki/Inter-process_communication)
		* Shared memory
			* Producer process
			* Consumer process
			* Unbounded buffer
			* Bounded buffer
		* Message passing
			* Direct communication
			* Blocking (synchronous)
			* Nonblocking (asynchronous)
			* Blocking send
			* Nonblocking send
			* Blocking receive
			* Nonblocking receive
			* Rendezvous
		* Buffering
			* Zero capacity
			* Bounded capacity
			* Unbounded capacity
* Examples of IPC systems
	* POSIX shared memory
	* Mach (for Mac)
	* Windows
		* Advanced local procedure call (ALPC)
		* Connection ports
		* Communication ports
		* Section object
* Communication in client-server systems
	* Socket
		* Connection-oriented sockets (TCP)
		* Connectionless sockets (UDP)
		* Loopback 127.0.0.1
	* Remote procedure calls
		* Port
		* Stub
		* Microsoft Interface Definition Language (MIDL)
		* Big-endian
		* Little-endian
		* External data representation (XDR)
		* Matchmaker
	* [Pipes](https://en.wikipedia.org/wiki/Pipeline_(Unix)#pipe_character)
		* Write-end
		* Read-end
		* [Anonymous pipes](https://en.wikipedia.org/wiki/Anonymous_pipe)
		* [Named pipes](https://en.wikipedia.org/wiki/Named_pipe)

* [Address space](https://en.wikipedia.org/wiki/Address_space)
* [Virtual memory](https://en.wikipedia.org/wiki/Virtual_memory)
* [Process state](https://en.wikipedia.org/wiki/Process_state) - New, running, waiting, ready, terminated
* [Process control block (PCB)](https://en.wikipedia.org/wiki/Process_control_block)
* [fork](https://en.wikipedia.org/wiki/Fork_(system_call)) system call
* [exec](https://en.wikipedia.org/wiki/Exec_(system_call)) system call

---

# Notes

## Processes

Notes from [Process (computing) - Wikipedia](https://en.wikipedia.org/wiki/Process_(computing))

A process is an instance of a computer program that is being executed.

### Program memory layout

![program memory layout](https://upload.wikimedia.org/wikipedia/commons/thumb/5/50/Program_memory_layout.pdf/page1-350px-Program_memory_layout.pdf.jpg)

(From [Wikipedia](https://en.wikipedia.org/wiki/File:Program_memory_layout.pdf))

The program memory layout contains:

* Stack - Typically in the "higher" parts of memory. Automatic (local) variables.
* Heap - Commonly attached to the .bss / .data segments and can grow "upwards". Dynamically allocated memory
* Data - Contains any global or static variables
* BSS - Uninitialized data; global and static variables that are uninitialized or initialized to 0.
* Text

A process generally contains:

* An image of the executable machine code for the program (text?)
* Memory - call stack and heap
* Operating system descriptors
* Security attributes
* Processor state

![Virtual address space](https://upload.wikimedia.org/wikipedia/commons/thumb/3/32/Virtual_address_space_and_physical_address_space_relationship.svg/773px-Virtual_address_space_and_physical_address_space_relationship.svg.png)

(From [Wikipedia](https://en.wikipedia.org/wiki/Address_space))

["Most modern operating systems that support virtual memory also run each process in its own dedicated address space. Each program thus appears to have sole access to the virtual memory."](https://en.wikipedia.org/wiki/Virtual_memory)


## Process Control Block

Notes from [Process control block - Wikipedia](https://en.wikipedia.org/wiki/Process_control_block)

Process Control Block (PCB, also called Task Controlling Block,[1] process table,[2] Task Struct, or Switchframe) is a data structure in the operating system kernel containing the information needed to manage a particular process. The PCB is "the manifestation of a process in an operating system".

General structure of a PCB:

* Process identification data - contains unique process identifier
* Process state data - the status of a process when it is suspended, for later restart ([See: context switching](https://en.wikipedia.org/wiki/Context_switch))
* Process control data

Process control data is used by the OS to manage the process itself.

* The process scheduling state
* Process structuring information
* Interprocess communication information
* Process privileges
* Process state
* Process No (number)
* Program counter
* CPU registers
* CPU scheduling information
* Memory management information
* Accounting information
* IO status information


### Process states

Notes from [Process state - Wikipedia](https://en.wikipedia.org/wiki/Process_state)

![Process states](https://upload.wikimedia.org/wikipedia/commons/thumb/8/83/Process_states.svg/600px-Process_states.svg.png)

(From [Wikipedia](https://en.wikipedia.org/wiki/File:Process_states.svg))

* Created/New
	* When first created, a process is in this state.
	* Awaits admission into ready state.
* Ready
	* A process that has been loaded into main memory and is awaiting exection on the CPU (to be context switched onto the CPU by the dispatcher aka short-term scheduler)
	* Can be many ready processes at one time
	* A ready queue/run queue is used.
* Running
	* A process that has been chosen for execution.
	* Process instructions are executed by one of the CPU cores.
	* At most one process running per CPU/core.
	* Can run in kernel mode or user mode.
* Blocked
	* Once a process cannot carry on without an external change, it is moved to a blocked state. (e.g., waiting on a printer if not available)
* Terminated
	* A process can be explicitly killed, or it will terminate once finishing its execution.

## Message passing

Notes from [Inter-process communication - Wikipedia](https://en.wikipedia.org/wiki/Inter-process_communication)

### Shared memory

Notes from [Shared memory](https://en.wikipedia.org/wiki/Shared_memory)

Notes from book

* Faster than message passing
* System calls only used to establish shared memory regions.
* No assistance is required from the kernel after establishment.
* Has cache consistency issues.

### Message passing

Notes from [Message passing](https://en.wikipedia.org/wiki/Message_passing)

Message passing sends a message to a process. It is used as a way 
for objects in a program to work with each other, and as a way
for objects and systems on different computers to ineract.

Notes from book

* Able to allow processes to communicate and synchronize their actions without sharing address space
* No conflicts need to be avoided
* Useful for exchanging small amounts of data
* Easier to implement in a distributed system
* Typically implemented via system calls, kernel intervention is more time-consuming.
* Messages can be fixed size or variable size


## Pipes

Notes from [Pipeline (Unix)](https://en.wikipedia.org/wiki/Pipeline_(Unix))






