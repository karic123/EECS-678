# amiko-asignilo


Aishwarya & Rachel


## To Do

1. Need to implement data structure in list.h?


---

## Reference material

A.

> Starting off, the size of the smallest possible block is determined, i.e. the smallest memory block that can be allocated. 

> Typically the lower limit would be small enough to minimize the average wasted space per allocation, but large enough to avoid excessive overhead. The smallest block size is then taken as the size of an order-0 block, so that all higher orders are expressed as power-of-two multiples of this size.

> The programmer then has to decide on, or to write code to obtain, the highest possible order that can fit in the remaining available memory space.

> Since the total available memory in a given computer system may not be a power-of-two multiple of the minimum block size, the largest block size may not span the entire memory of the system. For instance, if the system had 2000K of physical memory and the order-0 block size was 4K, the upper limit on the order would be 8, since an order-8 block (256 order-0 blocks, 1024K) is the biggest block that will fit in memory. 

[From Buddy memory allocation (Wikipedia)](https://en.wikipedia.org/wiki/Buddy_memory_allocation)

B.

![Free page block management](https://www.kernel.org/doc/gorman/html/understand/understand-html029.png)

[From Physical Page Allocation, kernel.org](https://www.kernel.org/doc/gorman/html/understand/understand009.html)

C.

> On a memory request, the allocator returns the head of a free-list of the matching size (i.e., smallest block that satisfies the request). If the free-list of the matching block size is empty, then a larger block size will be selected. The selected (large) block is then splitted into two smaller blocks. Among the two blocks, left block will be used for allocation or be further splitted while the right block will be added to the appropriate free-list.

> Whenever a block is freed, the allocator checks its buddy. If the buddy is free as well, then the two buddies are combined to form a bigger block. This process continues until one of the buddies is not free.

[From Buddy Allocator specifications](http://www.ittc.ku.edu/~heechul/courses/eecs678/S17/projects/buddy/doc/html/index.html)

D.

> The common pattern for storing this structure in a linked list is to embed the list pointer in the structure. For example:

```c
struct fox { 
	unsigned long  tail_length;  /* length in centimeters of tail */ 
	unsigned long  weight;       /* weight in kilograms */ 
	bool           is_fantastic; /* is this fox fantastic? */ 
	struct fox     *next;        /* next fox in linked list */ 
	struct fox     *prev;        /* previous fox in linked list */
};
```

> The Linux kernel approach is different. Instead of turning the structure into a linked list, the Linux approach is to embed a linked list node in the structure!
> (...)

```c
struct fox { 
	unsigned long    tail_length;  /* length in centimeters of tail */ 
	unsigned long    weight;       /* weight in kilograms */ 
	bool             is_fantastic; /* is this fox fantastic? */ 
	struct list_head list;         /* list of all fox structures */
};
```

> With this, list.next in fox points to the next element, and list.prev in fox points to the previous.
> (...)
> the list_add() method adds a new node to an existing linked list.These methods, however, are generic:They accept only list_head structures. Using the macro container_of(), we can easily find the par-ent structure containing any given member variable
> (...)
> Armed with list_entry(), the kernel provides routines to create, manipulate, and otherwise manage linked lists—all without knowing anything about the structures that the list_head resides within

Ugh, this is why we have C++ now you guys.

[From Page 88 - 96, Robert Love's Linux Kernel Development, Linux kernel linked list](http://perso.crans.org/segaud/Addison-Wesley%20Professional%20Linux%20Kernel%20Development%203rd.pdf)

---

Notes

Ugh, it really feels like a bunch of code is dumped in our lap without
any guidance on what to do... e.g., wtf goes in the page struct?


