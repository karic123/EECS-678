# Final Review

FML.

1. [FINAL EXAM REVIEW](#final-exam-review)

	1. [Main memory](#main-memory)
		1. [Paging](#paging)
		1. [MMU](#memory-management-unit-mmu)
		1. [TLB](#translation-lookaside-buffer-tlb)
		1. PROBLEMS
			1. [Calculate size of a single page](#application-calculate-size-of-single-page)
			1. [Calculate virtual address space](#application-calculate-virtual-address-space)
			1. [Calculate physical address space](#application-calculate-physical-address-space)
			1. [Address translation](#application-address-translation)
			1. [Calculate page table size](#application-calculate-page-table-size)
			1. [Calculate required page table size](#application-calculate-required-page-table-size)
			1. [Calculate bits for page offset](#application-calculate-bits-for-page-offset)
			1. [Page number given page size and address](#application-page-number-given-page-size-and-address)
			1. [Calculate entries in a page table](#application-entries-in-a-page-table)
		
	1. [Virtual memory](#virtual-memory)
		1. [Demand paging](#demand-paging)
		1. [Page replacement & swapping](#page-replacement--swapping)
		1. [Second-chance algorithm](#second-chance-algorithm)
		1. PROBLEMS
			1. []()
			1. []()
	1. I/O Devices
	1. Disk
	1. I/O Mechanisms
	1. Filesystem
	1. Name resolution
	1. Storage caches
	1. Network
	1. Security & Virtual Machine
		
1. [FINAL EXAM POSTMORTEM](#final-exam-postmortem)

1. [MIDTERM ANALYSIS](#midterm-analysis)

---

# Final Exam review

## Main memory

### Paging

The OS creates the page table and the hardware reads the page table.

Advantages are that there is no external fragmentation; there is
efficient use of memory. Internal fragmentation, however, may still exist.

**Operating system support:**

* The OS manages the MMU, and sometimes the TLB.
* The OS determines the address mapping

> In computing, virtual memory is a memory management technique that is implemented using both hardware and software. It maps memory addresses used by a program, called virtual addresses, into physical addresses in computer memory. Main storage as seen by a process or task appears as a contiguous address space or collection of contiguous segments. The operating system manages virtual address spaces and the assignment of real memory to virtual memory.
[Wikipedia](https://en.wikipedia.org/wiki/Virtual_memory)

**Paging issues:**

* Too big: Waste space, small table size
* Too small: Large table size, wasted space minimization
* Typical size: 4 KB

**Alternatives:**

* Many real-time operating systems don't have virtual memory.



### Memory-management unit (MMU)

> A memory management unit (MMU), sometimes called paged memory management unit (PMMU), is a computer hardware unit having all memory references passed through itself, primarily performing the translation of virtual memory addresses to physical addresses. It is usually implemented as part of the central processing unit (CPU), but it also can be in the form of a separate integrated circuit.

> An MMU effectively performs virtual memory management, handling at the same time memory protection, cache control, bus arbitration and, in simpler computer architectures (especially 8-bit systems), bank switching.
[Wikipedia](https://en.wikipedia.org/wiki/Memory_management_unit)



#### Segmentation MMU (base+limit)

![Dynamic relocation](images/dynamic_relocation.png)

**Simple MMU**, with some ```BaseAddr```, we get the ```PAddr``` with ```PAddr = VAddr + BaseAddr```.

This would be fast, but has no protection and it is wasteful.

The routine is only loaded as-needed, and not when it is not.


**Better MMU**:

* If ```VAddr > limit```:
	* Trap and report error
* Else:
	* ```PAddr = VAddr + BaseAddr```

![MMU with hardware support](images/hardware_realocation.png)

This adds error detection, and we can trap an error.

This also supports variable-sized partitions.

However, it can lead to fragmentation.



#### Paging MMU (page tables)

* Physical memory is divided into fixed-sized blocks, which are called **frames**.
* Logical memory blocks are divided into fixed-sized blocks, which are called **pages**.
* page size = frame size
* A **page table** maps the pages onto frames.

		p: page number		d: page offset (BECAUSE THAT HAS A "d" ANYWHERE IN THE NAME...)

These addresses look like:

![Logical address and physical address diagram](images/logical_and_physical_addresses.png)

And the diagram for this MMU scheme:

![MMU with paging](images/mmu_with_paging.png)

And mapping looks like this:

![MMU Memory View](images/memory_view.png)



#### Translation lookaside buffer (TLB)

> A Translation lookaside buffer (TLB) is a memory cache that is used to reduce
the time taken to access a user memory location.
[Wikipedia](https://en.wikipedia.org/wiki/Translation_lookaside_buffer)

![MMU with paging and TLB](images/mmu_with_paging_and_tlb.png)



### Multi-level paging

#### Two-level paging

![Two-level page table](images/two-level-paging.png)




### Fragmentation

> In computer storage, fragmentation is a phenomenon in which storage space is used inefficiently, reducing capacity or performance and often both
[Wikipedia](https://en.wikipedia.org/wiki/Fragmentation_(computing))

#### Internal fragmentation:

> Due to the rules governing memory allocation, more computer memory is sometimes allocated than is needed. For example, memory can only be provided to programs in chunks divisible by 4, 8 or 16, and as a result if a program requests perhaps 23 bytes, it will actually get a chunk of 32 bytes. When this happens, the excess memory goes to waste. In this scenario, the unusable memory is contained within an allocated region. This arrangement, termed fixed partitions, suffers from inefficient memory use - any process, no matter how small, occupies an entire partition. This waste is called internal fragmentation.

> Unlike other types of fragmentation, internal fragmentation is difficult to reclaim; usually the best way to remove it is with a design change. For example, in dynamic memory allocation, memory pools drastically cut internal fragmentation by spreading the space overhead over a larger number of objects.
[Wikipedia](https://en.wikipedia.org/wiki/Fragmentation_(computing)#Internal_fragmentation)



#### External fragmentation:

> External fragmentation arises when free memory is separated into small blocks and is interspersed by allocated memory. It is a weakness of certain storage allocation algorithms, when they fail to order memory used by programs efficiently. The result is that, although free storage is available, it is effectively unusable because it is divided into pieces that are too small individually to satisfy the demands of the application. The term "external" refers to the fact that the unusable storage is outside the allocated regions.

> For example, consider a situation wherein a program allocates 3 continuous blocks of memory and then frees the middle block. The memory allocator can use this free block of memory for future allocations. However, it cannot use this block if the memory to be allocated is larger in size than this free block.
[Wikipedia](https://en.wikipedia.org/wiki/Fragmentation_(computing)#External_fragmentation)




---

## Virtual memory

How the operating system makes it appear that there's more memory
than there actually is, so that each process thinks it has more memory than
is available.


### Demand paging





### Page replacement & swapping



### Second-chance algorithm




---

## I/O systems


### I/O devices

### Disk

### I/O mechanisms






---

## File-system

### Name resolution

### Storage caches

### Network

---

## Security & virtual machine











---

## Problems: Main Memory

### Application: Calculate size of single page


[View all code](https://github.com/Rachels-studies/EECS-678/blob/main/Studying/Tools/main_memory.py)

```python
#   x bits      y bits      z bits
# ----------------------------------
# | 1st level | 2nd level | offset |
# ----------------------------------

# Virtual address format: x + y + z bits

def get_size_of_single_page( offset_bits ):
    return 2 ** offset_bits # 2^offset_bits
```

The size of a single page is **2<sup>offset</sup>**.



### Application: Calculate virtual address space

[View all code](https://github.com/Rachels-studies/EECS-678/blob/main/Studying/Tools/main_memory.py)

```python
#   x bits      y bits      z bits
# ----------------------------------
# | 1st level | 2nd level | offset |
# ----------------------------------

# Virtual address format: x + y + z bits
# Virtual address space: 2^virtual_address_format

def get_size_of_virtual_address_space_bytes( virtual_address_format_bits ):
    return 2 ** virtual_address_format_bits # 2^virtual_address_format_bits
```

The size of the virtual address space is **2<sup>virtual_address_format</sup>**



### Application: Calculate physical address space

[View all code](https://github.com/Rachels-studies/EECS-678/blob/main/Studying/Tools/main_memory.py)

```python
#   a bits    b bits  c bits
# ------------------------
# | frame # | unused | V |
# ------------------------ 

# Page table entry: a + b + c bits
# Frame: a bits

def get_size_of_physical_address_space_bytes( frame_bits, page_table_entry_bits ):
    return ( 2 ** frame_bits ) * ( 2 ** page_table_entry_bits ) # 2^frame_bits + 2^page_table_entry_bits
```

The size of the physical address space is **2<sup>frame</sup> * 2<sup>page_table_entry_bits</sup>**


### Application: Address translation

Given the following information:

**Page Layout**

<table>
	<tr><td> 8 bits </td><td> 8 bits </td><td> 8 bits </td></tr>
	<tr><th> 1st level </th><th> 2nd level </th><th> offset </th></tr>
</table>

*Virtual address format: 8 + 8 + 8 = 24 bits*

**Frame Layout**

<table>
	<tr><td> 4 bits </td><td> 3 bits </td><td> 1 bit </td></tr>
	<tr><th> frame # </th><th> unused </th><th> Valid Bit </th></tr>
</table>

* Page table entry: 4 + 3 + 1 = 8 bits*

**Page table**

*Page-table base address: 0x100*

<table border="0" cellspacing="0" cellpadding="0" class="ta1"><colgroup><col width="99"/><col width="34"/><col width="34"/><col width="34"/><col width="34"/><col width="34"/><col width="34"/><col width="34"/><col width="34"/><col width="34"/><col width="34"/><col width="36"/><col width="36"/><col width="36"/><col width="36"/><col width="36"/><col width="35"/></colgroup><tr class="ro1"><td style="text-align:left;width:64.01pt; " class="ce1"><p>Addr</p></td><td style="text-align:left;width:21.86pt; " class="ce2"><p>+0</p></td><td style="text-align:left;width:21.86pt; " class="ce2"><p>+1</p></td><td style="text-align:left;width:21.86pt; " class="ce2"><p>+2</p></td><td style="text-align:left;width:21.86pt; " class="ce2"><p>+3</p></td><td style="text-align:left;width:21.86pt; " class="ce2"><p>+4</p></td><td style="text-align:left;width:21.86pt; " class="ce2"><p>+5</p></td><td style="text-align:left;width:21.86pt; " class="ce2"><p>+6</p></td><td style="text-align:left;width:21.86pt; " class="ce2"><p>+7</p></td><td style="text-align:left;width:21.86pt; " class="ce2"><p>+8</p></td><td style="text-align:left;width:21.86pt; " class="ce2"><p>+9</p></td><td style="text-align:left;width:23.41pt; " class="ce2"><p>+A</p></td><td style="text-align:left;width:23.41pt; " class="ce2"><p>+B</p></td><td style="text-align:left;width:23.41pt; " class="ce2"><p>+C</p></td><td style="text-align:left;width:23.41pt; " class="ce2"><p>+D</p></td><td style="text-align:left;width:23.41pt; " class="ce2"><p>+E</p></td><td style="text-align:left;width:22.59pt; " class="ce2"><p>+F</p></td></tr><tr class="ro1"><td style="text-align:left;width:64.01pt; " class="ce1"><p>0x000</p></td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:right; width:21.86pt; " class="ce2"><p>31</p></td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:22.59pt; " class="ce2"> </td></tr><tr class="ro1"><td style="text-align:left;width:64.01pt; " class="ce1"><p>0x010</p></td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:22.59pt; " class="ce2"> </td></tr><tr class="ro1"><td style="text-align:left;width:64.01pt; " class="ce1"><p>0x020</p></td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:right; width:21.86pt; " class="ce2"><p>41</p></td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:22.59pt; " class="ce2"> </td></tr><tr class="ro1"><td style="text-align:left;width:64.01pt; " class="ce1"><p>…</p></td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:22.59pt; " class="ce2"> </td></tr><tr class="ro1"><td style="text-align:left;width:64.01pt; " class="ce1"><p>0x100</p></td><td style="text-align:left;width:21.86pt; " class="ce2"><p>00</p></td><td style="text-align:left;width:21.86pt; " class="ce2"><p>01</p></td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"><p>01</p></td><td style="text-align:left;width:21.86pt; " class="ce2"><p>00</p></td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"><p>01</p></td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:22.59pt; " class="ce2"> </td></tr><tr class="ro1"><td style="text-align:left;width:64.01pt; " class="ce1"><p>…</p></td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:22.59pt; " class="ce2"> </td></tr><tr class="ro1"><td style="text-align:left;width:64.01pt; " class="ce1"><p>0x200</p></td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:21.86pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:23.41pt; " class="ce2"> </td><td style="text-align:left;width:22.59pt; " class="ce2"> </td></tr></table>

* The column headers *+0, +1, ... +F*, correspond to the *level index*.
* The first page begins from row *0x100*, and then subsequent pages begin at row *0x000* at the base.
* Each character in a hex address is 4 bits.

#### Find the ```Paddr```, given that ```Vaddr = 0x0703FE```.

<table>
	<tr><td> 8 bits </td><td> 8 bits </td><td> 8 bits </td></tr>
	<tr><th> 1st level </th><th> 2nd level </th><th> offset </th></tr>
	<tr><th><pre> 07 </pre></th><th> <pre> 03 </pre> </th><th> <pre> FE </pre> </th></tr>
</table>

**First level:**

* Start at row ```0x100```, and move over by the *1st level index* (from the virtual address), ```07```.
* The value in this cell is ```01```.
	* Interpret as: *0 is the frame number*, and *1 is the valid bit*.
	* If the valid bit is *0*, then the address is invalid.

**Second level:**

* Start at row ```0x000```, and move over by the *2nd level index* (from the virtual address), ```03```.
* The value in this cell is ```31```.
	* *3* is the data we will use in our physical address, and *1* again is the valid bit.

**Offset:**

* The offset from the virtual address is also part of the physical address.

Final address: ```0x3FE```.


#### Find the ```Paddr```, given that ```Vaddr = 0x072370```.

1. First level: Start at row ```0x100```, offset by ```07```: Value is *01*. Frame is 0 and valid bit is 1.
1. Second level: Start at row ```0x000```, offset by ```23```:
	1. Go down by 2, and over by 3. This value is ```41```.
1. Keep the offset

Final address: ```0x470```


### Application: Calculate page table size

**How many pages are needed for 4 GB (of physical memory...?) @ 32 bit?**

1. 4 GB / 4 KB = 1 Million Pages

I don't know what the *@ 32 bit* specifically measures,
and I don't know where that 4KB came from.



### Application: Calculate required page table size

**What is the required page table size? for 4 GB (of physical memory...?) @ 32 bit?**

1. 1 Page table entry is 4 bytes...
1. 1 Million (pages) * 4 bytes = 4 MB

```python
def get_page_table_size( pages_needed, something_bits ):
	something_bytes = something_bits / 8
	return pages_needed * something_bytes
```


### Application: Calculate bits for page offset

```python
def get_offset_size_bits( page_size_KB ):
	# This needs to be in terms of bits...
	offset_bits = numpy.log2( page_size_KB )
	return offset_bits
```

In other words, with a page size of ```x```, and the page offset as ```y``` bits...

* 2<sup>y</sup> = x
* log<sub>2</sub>( x ) = y


### Application: Entries in a page table

**How many entries ```y``` are in a page table if we are using ```x``` bits
of a virtual address as the index bits?**

```python
def get_entries_in_page_table( index_bits ):
    return 2 ** index_bits # 2^index_bits
```

* y = 2<sup>x</sup>


### Application: Page number given page size and address

**Given some logical address *(a<sub>)16</sub>* (hexadecimal #)
and a page size of ```p``` bytes, what is the page number?**

1. Convert page size from bytes to bits:
	1. *p = 2<sup>b</sup>* or
	1. *b = log<sub>2</sub>( p )*

Remember that the logical address layout is like:

<table>
<tr> <td colspan="2"> Page </td> 						<td> Offset </td> </tr>
<tr> <td> p<sub>1</sub> </td> <td> p<sub>2</sub> </td> 	<td>d</td> </tr>
</table>

Each digit in a hexadecimal number corresponds to 4 bits

<table>
<tr> <th> Hex </th> 	<td> A </td> 	<td> F </td> </tr>
<tr> <th> Binary </th> 	<td> 1010 </td> <td> 1111 </td> </tr>
<tr> <th> Decimal </th> <td> 10 </td> 	<td> 15 </td> </tr>
</table>

So given the amount of bits *b*, you would take the first *b*
bits from the address. If *b* were 8, then...

<table> 
<tr> <th> Address (Hex) </th> <td> F </td> <td> F </td> <td> F </td> <td> F </td> </tr>
<tr> <th> Address (Binary) </th> <td> 1111 </td> <td> 1111 </td> <td> 1111 </td> <td> 1111 </td> </tr> 
<tr> <td> </td> <td colspan="2"> First 8 </td> <td colspan="2"> </td> </tr>
</table>

---


## Problems: Virtual Memory

### Application: LRU Replacement

---

## Problems: File-system

### Application: Maximum disk size

### Application: Maximum size of a file

### Knowledge: File system layers

### Knowledge: Transfers between memory and disk

### Knowledge: Linux VFS architecture













---

# Final Exam postmortem

---

# Midterm analysis

I got a 78% on the Midterm, partially because I did not study from the
*right* resources. The lesson I learned from the midterm was:

*Don't study by reading the textbook.*

This time around, I'm going to primarily study off the instructor's
slides and the Blackboard quizzes, as a lot of the application problems
came from the quizzes that we only ever covered in *that form* on
the quizzes.

I have my Midterm exam paper, and I can now compare it to the instructor's
Midterm review slides as well, and figure out how closely it was an actual guide
to the exam.

### Midterm review guide vs. Midterm exam

* OS Structure
	* User Mode vs. Kernel Mode
	* System call
	* Monolithic Kernel vs. Micro Kernel
	* Diagram of *API - System Call - Operating System* together
	* UNIX: Monolithic Kernel - *on exam*
		* Pros and cons

* Processes
	* Address space layout
	* Process states - *on exam*
	* Other...
		* PCB
		* Context switch
		* Zombies, orphans
		* Communication overhead, processes & threads
	* Process state diagram - *on exam*
	* Process address space - *on exam*
		* Text, data, heap, stack - *on exam*

* Inter-process communication
	* Shared memory vs. message passing pros/cons - *on exam*

* Threads
	* User threads vs. Kernel threads
	* Benefits?
	* Diagram of single-threaded and multi-threaded processes - *on exam*

* Synchronization
	* Race conditions
	* Synchronization instructions
	* Spinlock
	* Mutex
	* Race condition
	* Bounded buffer problem
	* Deadlocks - *on exam*
		* Conditions
		* Resource allocation graph
		* Banker's algorithm
		* Dining philosophers
	* Starvation

* Scheduling
	* FCFS, SJF,RR - *on exam*
	* Gantt charts - *on exam*
	* Fair scheduling
	* Fixed priority scheduling
	* Multi-level scheduling
	* Load balancing, multicore scheduling
	* Round-Robin Gantt chart, waiting time, schedule latency - *on exam*
	* Pros and Cons - *on exam*

**Types of application problems:**

* Given code that forks a process and runs an exec command, what is the output?
	* Slides had different example without exec; lack of time and not paying attention caused me to get this one partially wrong.
* Spinlock code / Test-and-set with fill-in-the-blank parts to complete it.
	* *On the slides*
* Multithreaded program without locks, possible outcomes.
	* Kind of on slides
* Implementing a monitor and a sempahore for bounded-buffer problem.
	* Subjective question of "Which version is easier to understand?" that I apparently got 1 point off on???
	* Examples were in the slides
* Given tables of processes and resources, figure out if there is a deadlock.
	* *Example on slides*
* Given a table of processes and arrival/burst times, draw Gantt diagrams and calculate averages.
	* *Example on slides*

**Other things not necessarily on the slides:**

* Multiprogramming
* Time sharing
* Dual mode hardware mechanism
* "who" manages the CPU cache?
* What hardware support is required to implement a preemptive CPU scheduler?
* Where CPU register values are stored
* Where global variables are stored
* Anonymous piping
* User-level threading
* Protection of multi-threaded architecture vs. multi-process architecture
