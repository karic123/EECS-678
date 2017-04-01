# Chapter 9: Virtual Memory

Rachel's summary notes

* [Official book slides - Chapter 9](http://codex.cs.yale.edu/avi/os-book/OS9/slide-dir/PPT-dir/ch9.ppt)

---

# Notes

## Basic hardware information

* we care about efficiency, speed, and correctness
* we need to care about protection
* processes will need their own individual memory spaces

[registers](https://en.wikipedia.org/wiki/Processor_register)

* contained within the processor
* processor can access registers directly
* fast access - usually within one clock cycle

[Main memory](https://en.wikipedia.org/wiki/Computer_data_storage#Primary_storage)

* aka RAM
* processor can also access main memory directly
* data must be moved into memory in order for CPU to work with it
* main memory cannot be accessed by the CPU as quickly as registers can
* if the processor has to wait for memory access, then the processor
will **stall** since it is waiting on info that it needs to process.
	* need to design around this to make sure CPU usage is efficient
	* cache is a solution
	
[Cache](https://en.wikipedia.org/wiki/CPU_cache)

* a cache is used "in-between" the CPU and the main memory.

### Splitting up memory between processes

* Split up memory so that each process has its own memory space
* **base register** - smallest physical memory address
* **limit register** - size of the range
* base and limit registers are used to protect operating-system memory,
and can only be loaded by the OS.
* The OS has access to all memory though, including process memory.



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
