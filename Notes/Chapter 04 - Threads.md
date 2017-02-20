# Chapter 4: Threads

Rachel's summary notes

* [Official book slides - Chapter 4](http://codex.cs.yale.edu/avi/os-book/OS9/slide-dir/PPT-dir/ch4.ppt)

## Vocabulary

* Single-threaded process
* Multi-threaded process
* Multicore/multiprocessor system
* Data parallelism
* Task parallelism
* User threads
* Kernel threads
* Green threads
* Multithreading models
	* One-to-one model
	* Many-to-many model
	* Many-to-one model
* Thread libraries
	* POSIX
* Pthreads, Windows threads, Java threads
* Implicit threading
* Thread pools
* OpenMP - Compiler directives and API
* Parallel regions
* Grand Central Dispatch - Apple's extentions to C, API
	* Blocks
	* Dispatch queue
	* Main queue
* Threading issues
	* fork() and exec()
	* Signal handling
		* Default signal handler
		* User-defined singal handler
		* Asynchronous procedure calls (APCs)
	* Thread cancellation
		* Target thread
		* Asynchronous cancellation
		* Deferred cancellation
		* Cancellation point
		* Cleanup handler
	* Thread-local storage (TLS)
	* Scheduler activations
		* Lightweight process (LWP)
		* Upcall, upcall handler


---

# Notes

