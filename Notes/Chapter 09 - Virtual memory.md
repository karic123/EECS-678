# Chapter 9: Virtual Memory

Rachel's summary notes

* [Official book slides - Chapter 9](http://codex.cs.yale.edu/avi/os-book/OS9/slide-dir/PPT-dir/ch9.ppt)

---

# Notes



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
