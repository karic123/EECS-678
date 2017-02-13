
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
