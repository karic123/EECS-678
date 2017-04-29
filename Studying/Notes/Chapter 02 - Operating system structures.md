# Chapter 2: Operating system structures

Rachel's summary notes

* [Official book slides - Chapter 2](http://codex.cs.yale.edu/avi/os-book/OS9/slide-dir/PPT-dir/ch2.ppt)

## Vocabulary

* Operating System services
	* User interface (UI)
		* Command-line Interface (CLI)
		* Graphical User Interface (GUI)
		* Batch interface
	* Program execution
	* I/O operations
	* File system manipulation
	* Communication
	* Error detection
	* Resource allocation
	* Accounting
	* Protection and security
* User and Operating System interface
	* Command interpreters
		* Shell
		* GUI
		* Shell scripts
* System calls
	* Application programming interface (API)
	* System-call interface
* Types of system calls
	* Process control
		* Handles:
			* end, abort
			* load, execute
			* create process, terminate process
			* get process attributes, set process attributes
			* wait for time
			* wait event, signal event
			* allocate and free memory
	* File manipulation
		* Handles:
			* create file, delete file
			* open, close
			* read, write, reposition
			* get file attributes, set file attributes
	* Device manipulation
		* Handles:
			* request device, release device
			* read, write, reposition
			* get device attributes, set device attributes
			* logically attach or detach devices
	* Information maintenance
		* Handles:
			* get time or date, set time or date
			* get system data, set system data
			* get process, file, or device attributes
			* set process, file, or device attributes
	* Communications
		* Message-passing model...
		* Shared-memory model...
		* Handles:
			* create, delete communication connection
			* send, receive messages
			* transfer status information
			* attach or detact remote devices
	* Protection
* System programs aka system utilities
	* File management
	* Status information
	* File modification
	* Programming language support
	* Program loading and execution
	* Communications
	* Background services
* Operating System design and implementation
	* Design goals
		* User goals
		* System goals
	* Mechanisms and policies
	* Implementation
* Operating System structures
	* Simple structure
	* Layered approach
	* Microkernels
	* Modules
	* Hybrid systems
	* Types:
		* Monolithic
		* Simple
		* Layered
		* Microkernel
		* Hybrid
* Operating System debugging
	* Failure analysis
		* Log file
		* Core dump
		* Crash
		* Crash dump
	* Performance tuning
	* DTrace
		* Profiling
		* Providers and consumers of probes
		* Enabling control blocks (ECBs)
* Operating System generation
	* System generation SYSGEN
* System boot
	* Bootstrap program/bootstrap loader
	* Read-only memory (ROM)
	* Erasable programmable read-only memory (EPROM)
	* Firmware
	* Boot block
	* GRUB
	* Boot disk/system disk

* System calls
* Application programming interface (API)
* Debugger
* Lock shared data
* Single step (CPU mode)
* Daemons/services/subsystems
* Client, server
* System programs/system utilities
* Registry
* Loadable kernel modules
* Performance tuning
* Bottlenecks


---

# Notes
