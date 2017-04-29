# Chapter 16: Virtual machines

Rachel's summary notes

* [Official book slides - Chapter 16](http://codex.cs.yale.edu/avi/os-book/OS9/slide-dir/PPT-dir/ch16.ppt)

---

# Notes

---

# Vocabulary

* Host
* Virtual Machine Manager VMM (aka Hypervisor)
* Guest
* Type 0 hypervisors
* Type 1 hypervisors
* Type 2 hypervisors
* Paravirtualization
* Programming-environment virtualization
* Emulators
* Application containment
* History
	* Minidisks
	* Xen, VMWare
* Benefits and Features
	* Suspend
	* Snapshots
	* Resume
	* Clone
	* System-development time
	* Consolidation
	* Templating
	* Live migration
	* Cloud computing
	* RDP
* Building Blocks
	* Virtual CPU (VCPU)
	* Trap-and-Emulate
	* Binary Translation
		* Nested page tables (NPTs)
	* Hardware Assistance
		* Virtual Machine Control Structures (VMCSs)
		* Protection domains
* Types of Virtual Machines and their Implementations
	* The Virtual Machine Life Cycle
		* Virtual Machine sprawl
	* Type 0 Hypervisor
		* Control partition
	* Type 1 Hypervisor
	* Type 2 Hypervisor
	* Paravirtualization
	* Programming-Environment Virtualization
		* Interpreted languages
	* Emulation
	* Application Containment
* Virtualization and Operating-System Concepts
	* CPU Scheduling
		* Overcommitment
	* Memory management
		* Pseudo-device driver
		* Balloon
	* I/O
		* Bridging
		* Network address translation (NAT)
	* Storage management
		* Disk image
		* Physical-to-virtual (P-to-V)
		* Virtual-to-physical (V-to-P)
	* Live migration
* Examples
	* VMWare
	* The Java Virtual Machine
