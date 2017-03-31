# Chapter 8: Main Memory

Rachel's summary notes

* [Official book slides - Chapter 8](http://codex.cs.yale.edu/avi/os-book/OS9/slide-dir/PPT-dir/ch8.ppt)


---

# Notes

Only the processor can directly access main memory and registers (which are part of the processor).


---

# Vocabulary

* basic hardware
	* stall
	* cache
	* base register
	* limit register
* address binding
	* input queue
	* binding
	* absolute code
	* relocatable code
* logical vs. physical address space
	* logical address
	* memory-address register
	* physical address
	* virtual address
	* logical address space
	* physical address space
	* memory management unit (MMU)
	* relocation register
* dynamic loading
* dynamic linking and shared libraries
	* dynamically linked libraries
	* static linking
	* stub
	* shared libraries
* swapping
	* backing store
	* standard swapping
		* ready queue
		* double buffering
	* swapping on mobile systems
		* application state
	* contiguous memory allocation
	* memory protection
		* transient
	* memory allocation
		* partitions
		* multiple-partition method
		* variable-partition
		* hole
		* dynamic storage-allocation problem
		* first-fit
		* best-fit
		* worst-fit
	* fragmentation
		* external fragmentation
		* 50-percent rule
		* internal fragmentation
		* compaction
* segmentation
	* basic method
	* segmentation hardware
		* segment table
		* segment base
		* segment limit
* paging
	* basic method
		* frames
		* pages
		* page number (p)
		* page offset (d)
		* page table
	* hardware support
		* registers
		* page-table base register (PTBR)
		* translation look-aside buffer (TLB)
		* TLB miss
		* wired down
		* address-space identifiers (ASIDs)
		* flushed
		* hit ratio
		* effective memory-access time
	* protection
		* valid-invalid bit
		* page-table length register (PTLR)
	* shared pages
		* reentrant code (pure code)
* structure of the page table
	* hierarchical paging
		* forward-mapped page table
		* Digital Equipment Corporation (DEC)
		* VAX minicomputer
	* hashed page tables
		* clustered page tables
		* sparse
	* inverted page tables
	* Oracle SPARC Solaris
		* Solaris
		* SPARC
		* TLB walk
* example:  Intel 32 and 64-bit architectures
	* IA-32 architecture
		* IA-32 segmentation
			* local descriptor table (LDT)
			* global descriptor table (GDT)
			* linear address
		* IA-32 paging
			* page directory
			* page address extension (PAE)
			* page directory pointer table
	* x86-64
		* Itanium
		* AMD64
		* Intel 64
* example: ARM architecture
	* sections
	* micro TLBs
	* main TLB

