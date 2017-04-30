# Chapter 9: Virtual Memory

Rachel's summary notes

* [Official book slides - Chapter 9](http://codex.cs.yale.edu/avi/os-book/OS9/slide-dir/PPT-dir/ch9.ppt)

---

# Slide notes

These are the topics covered in the slides, but finding those topics
covered elseware on the internet, such as Wikipedia.

## Goals

Virtual memory allows us to isolate processes, so that they do not
corrupt each other. It achieves this by abstracting memory out to
virtual memory, and all processes share the same virtual memory space.
This also helps us utilize our limited physical memory more efficiently.

> The primary benefits of virtual memory include freeing applications from having to manage a shared memory space, increased security due to memory isolation, and being able to conceptually use more memory than might be physically available, using the technique of paging.
[Wikipedia](https://en.wikipedia.org/wiki/Virtual_memory)

## Concepts to learn

* Virtual address translation
* Paging and TLB
* Page table management
* Swap

## Virtual memory VM

> In computing, virtual memory is a memory management technique that is implemented using both hardware and software. It maps memory addresses used by a program, called virtual addresses, into physical addresses in computer memory. Main storage as seen by a process or task appears as a contiguous address space or collection of contiguous segments. The operating system manages virtual address spaces and the assignment of real memory to virtual memory.
[Wikipedia](https://en.wikipedia.org/wiki/Virtual_memory)

**Hardware support:**

>  Address translation hardware in the CPU, often referred to as a **memory management unit or MMU**, automatically translates virtual addresses to physical addresses.
[Wikipedia](https://en.wikipedia.org/wiki/Virtual_memory)

> A **Translation lookaside buffer (TLB)** is a memory cache that is used to reduce the time taken to access a user memory location.[1][2] It is a part of the chip’s memory-management unit (MMU). The TLB stores the recent translations of virtual memory to physical memory and can be called an address-translation cache.
[Wikipedia](https://en.wikipedia.org/wiki/Translation_lookaside_buffer)

**Operating system support:**

* The OS manages the MMU, and sometimes the TLB.
* The OS determines the address mapping

**Alternatives:**

* Many real-time operating systems don't have virtual memory.

![Virtual memory](https://upload.wikimedia.org/wikipedia/commons/thumb/6/6e/Virtual_memory.svg/620px-Virtual_memory.svg.png)
*By Ehamberg - Own work, CC BY-SA 3.0, https://commons.wikimedia.org/w/index.php?curid=8352077*



## Virtual address

## MMU

## Fragmentation

## Virtual Address Translation

## Paging

## Translation Lookaside Buffer TLB

## Multi-level paging

## Two level address translation

## Demand paging

## Page Table Entry PTE

## Partial memory mapping

## Page fault

## Anonymous page

---

# Vocabulary

* virtual memory
* virtual address space
* sparse
* demand paging
	* lazy swapper
	* pager
	* basis concepts
		* memory resident
		* page fault
		* pure demand paging
		* locality of reference
		* swap space
	* performance of demand paging
		* effective access time
		* page-fault rate
		* anonymous memory
* copy-on-write
	* pool
	* zero-fill-on-demand
	* virtual memory fork
* page replacement
	* over-allocating
	* page replacement
	* basic page replacement
		* victim frame
		* modify bit / dirty bit
		* frame-allocation algorithm
		* page-replacement algorithm
		* reference string
	* FIFO page replacement
		* Belady's anomaly
	* optimal page replacement
		* optimal page-replacement algorithm
	* LRU page replacement
		* least recently used (LRU) algorithm
	* stack algorithms
	* LRU-approximation page replacement
		* reference bit
		* additional-reference-bits algorithm
			* second-chance page-replacement algorithm
		* second-chance algorithm
			* clock
		* enhanced second-chance algorithm
	* counting-based page replacement
		* least frequently used (LFU)
		* most frequently used (MFU)
	* page-buffering algorithms
	* applications and page replacement
		* raw disk
* allocation of frames
	* minimum number of frames
	* allocation algorithms
		* equal allocation
		* proportional allocation
	* global vs. local allocation
		* global replacement
		* local replacement
	* non-uniform memory access (NUMA)
* thrashing
	* cause of thrashing
		* local replacement algorithm / priority replacement algorithm
		* locality model
	* working-set model
		* working-set window
		* working set
	* page-fault frequency (PFF)
* memory-mapped files
	* basic mechanism
	* shared memory in the Windows API
		* file mapping
		* view
		* named shared-memory object
	* memory-mapped I/O
		* port
		* programmed I/O (PIO)
		* interrupt driven
* allocating kernel memory
	* buddy system
		* power-of-2 allocator
		* buddies
		* coalescing
	* slab allocation
		* slab
		* cache
		* objects
* other considerations
	* prepaging
	* page size
		* resolution
	* TLB reach
		* hit ratio
	* inverted page tables
	* program structure
	* I/O interlock and page locking
		* pinning
* operating-system examples
	* Windows
		* clustering
		* working-set minimum
		* working-set maximum
		* automatic working-set trimming
	* Solaris
		* pageout
		* priority paging
