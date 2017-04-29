
## Instructor feedback

Your program is very good. 
I appreciate the fact that you have made an effort to make the program look cleaner, 
modular and overall more presentable. 
For this reason, I am awarding you the 20% extra credit for conformance with the coding standard.

That said, there is one functional mistake (i.e. memory leak) in your program.﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿﻿

You have done two allocations against 'sentence[2]' in your program. 
However, before the second allocation against this pointer on line-94, 
you haven't freed the memory that was previously allocated to the pointer which is causing a memory leak. (-10 Points)

Regarding the question you have raised in the main() function about why the crash occurs, 
note that the value you are freeing, is pointing to a variable in the stack whereas free() only works on dynamically allocated memory in heap. 
Doing  it on a stack variable will result in undefined-behavior i.e. it might crash on some machines and it might run perfectly on others.

You lost 10 points in your submission and 12 points in your report. So your total score is:

Score = 78

Scaled Score = round_to_100(1.2 * 78) = 93

## Postmortem

Time to investigate! My primary language is C++, so I'm not familiar with
which C functions allocate memory, so I need to take note.

### Double allocation:

Line 94, second allocation:

	(*bug_info).sentence[2] = strdup("colorful");

alright, so **strdup** apparently allocates memory. (This is why we use classes, constructors, and destructors in C++, yo. :)

### Why's this crash?

In my program:

	/*
	I don't know *why* this causes a crash -- this pointer is
	still pointing to a memory address, so it seems like freeing it
	would be OK.
	My guess is that free() should only be used opposite to malloc(),
	but this string was assigned directly.
	(I have a lot of exp in C++, but not C.)
	*/
	// free(bug_info.useless_bug);

bug_info.useless_bug was declared like this:

	struct Storage {
	  intptr_t num_bugs_on_mars;
	  const char* scary_bug;
	  char* sentence[7];
	  const char* colorful_bug;
	  intptr_t num_bugs_on_earth;
	  intptr_t num_bugs_on_venus;
	  char* useless_bug;
	};

and useless_bug is set to...

	(*bug_info).useless_bug = "mosquito";
	
Instructor's note: *is pointing to a variable in the stack*

Is this string literal what he was referring to? I will ask...

**Response:**

	Whenever you declare a string literal like this in c:

	char *my_string = "Hello World!";

	It points to a location in read-only data section. This is done by the compiler for you and you don't need to free this memory because it is automatically managed by c-runtime. Note that since the value resides in the read-only memory, you cannot modify its content i.e. doing something like this:

	/* Change the string from "Hello World!" to "Yello World!" */
	my_string[0] = 'Y';

	will result in an error. Another way to declare a string is like this:

	char my_string[ ] = "Hello World!";

	In this case, the string is declared on the stack and you can modify its content. However, you still don't need to free this memory because stack space is also automatically managed by c-runtime.

	Finally, you can assign a string by explicitly allocating a buffer for its content:

	char *my_string = (char *)malloc (32);
	sprintf (my_string, "%s", "Hello World!");

	or more conveniently, you can use strdup() which is going to allocate adequate memory on your behalf, copy the desired string in that memory and return the memory pointer to you:

	char *my_string = strdup ("Hello World!");

	In the last two cases, my_str is going to point to a memory in heap and will have to be explicitly freed.

	Now lets come to the case at hand:

	bug_info.useless_bug = "Mosquito";

		​The bug-info structure is in stack and hence all its fields are in stack too
		The address of the field bug_info.useless_bug lies in stack
		The memory pointed to by field bug_info.useless_bug lies in read-only data section of the address space and hence it should not be freed.

	I guess my comment on your report was a bit ambiguous regarding the placement of bug_info.useless_bug field. I hope that the explanation above clarifies this concept for you.
