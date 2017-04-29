# Chapter 14: Protection

Rachel's summary notes

* [Official book slides - Chapter 16](http://codex.cs.yale.edu/avi/os-book/OS9/slide-dir/PPT-dir/ch16.ppt)

---

# Slide notes

These are the topics covered in the slides, but finding those topics
covered elseware on the internet, such as Wikipedia.

## Examples of OS protection

* Memory protection - between user processes, between user and kernel
* File protection - prevent unauthorized access
* Privileged instructions - page table, cache/TLB updates

## Principles of protection

> In information security, computer science, and other fields, the principle of least privilege (also known as the principle of minimal privilege or the principle of least authority) requires that in a particular abstraction layer of a computing environment, every module (such as a process, a user, or a program, depending on the subject) must be able to access only the information and resources that are necessary for its legitimate purpose.
[Wikipedia](https://en.wikipedia.org/wiki/Principle_of_least_privilege)

## Protection domains

> In computer science, hierarchical protection domains,[1][2] often called protection rings, are mechanisms to protect data and functionality from faults (by improving fault tolerance) and malicious behaviour (by providing computer security). This approach is diametrically opposite to that of capability-based security
[Wikipedia](https://en.wikipedia.org/wiki/Protection_ring)

![ring image](https://upload.wikimedia.org/wikipedia/commons/thumb/2/2f/Priv_rings.svg/633px-Priv_rings.svg.png) (CC-BY-SA 3.0, by Hertzsprung at English Wikipedia)

## Access Control Matrix

> In computer science, an Access Control Matrix or Access Matrix is an abstract, formal security model of protection state in computer systems, that characterizes the rights of each subject with respect to every object in the system.

> An access matrix can be envisioned as a rectangular array of cells, with one row per subject and one column per object. The entry in a cell - that is, the entry for a particular subject-object pair - indicates the access mode that the subject is permitted to exercise on the object. Each column is equivalent to an access control list for the object; and each row is equivalent to an access profile for the subject.
[Wikipedia](https://en.wikipedia.org/wiki/Access_Control_Matrix)

## Access control list

> An access control list (ACL), with respect to a computer file system, is a list of permissions attached to an object. An ACL specifies which users or system processes are granted access to objects, as well as what operations are allowed on given objects.[1] Each entry in a typical ACL specifies a subject and an operation. For instance, if a file object has an ACL that contains (Alice: read,write; Bob: read), this would give Alice permission to read and write the file and Bob to only read it.
[Access Control List](https://en.wikipedia.org/wiki/Access_control_list)

## Capabiliy list

> In capability-based computer security, a C-list is an array of capabilities, usually associated with a process and maintained by the kernel. The program running in the process does not manipulate capabilities directly, but refers to them via C-list indexes—integers indexing into the C-list.
[Wikipedia](https://en.wikipedia.org/wiki/C-list_(computer_security))

## Summary

* Protection - prevent unauthorized access
* Protection domains 
* Access control matrix

---

# Vocabulary

* 14.1 Goals of protection

* 14.2 Principles of protection
	* Principle of least privilege

* 14.3 Domains of protection
	* Hardware objects
	* Software objects
	* Need-to-know principle
	* Domain structure
		* Protection domain
		* Access right
		* Static or dynamic
		* Domain switching

* 14.4 Access matrix
	* Confinement problem

* 14.5 Implementation of the access
	* Global table
	* Access lists for objects
	* Capability lists for domains
		* Capability
		* Possession
		* Tag
	* A lock-key mechanism
		* Locks, keys
	* Comparison

* 14.6 Access control
	* Role-based access control RBAC
	* Roles

* 14.7 Revocation of access rights
	* Questions about revocation:
		* Immediate vs delayed?
		* Selective vs general?
		* Partial vs total?
		* Temporary vs permanent?
	* Schemes that implement revocation:
		* Reacquisition
		* Back-pointers
		* Indirection
		* Keys

* 14.8 Capability-based systems

* 14.9 Language-based protection
	* Compiler-based enforcement
	* Protection in Java

* 14.10 Summary
