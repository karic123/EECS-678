# Rachel's Notes - Lab 3 - EECS 678, Spring 2017

## Reference material

### Reference

* [CS 702 - Operating Systems - Spring 2005](http://www.cs.loyola.edu/~jglenn/702/S2005/Examples/dup2.html)

### man pages

* [man dup2](https://linux.die.net/man/2/dup2)
* [man execv](https://linux.die.net/man/3/execv)

## Difficulties

* How the crap do you get output from an exec if it takes over the process?
	* Have to duplicate the STDOUT to a pipe using **dup2**
* I can send data between two processes via one pipe, but how do I
use a second process as an intermediate between the first and the third,
so that the *write* from the first ends up being a *read* for the third?
* execv was ignoring my arguments list... because we're not SUPPOSED to use
execv, but the instructions SOUNDED like you were supposed to use execv...
but NNOOOOOO we use execl. Thanks for hours and hours of FRUSTRATION! ARG!


## Pipeline

With this project, for my in-between pipes, I was wanting to
duplicate OUTPUT from one pipe ("pipeA") to the INPUT of another pipe
("pipeB"), but ?? can't do this ??

In an exaple program with 3 processes and 2 pipes, there were these:

* catToGrep[0] = read end    of cat->grep pipe   (read by grep)
* catToGrep[1] = write end   of cat->grep pipe   (written by cat)
* grepToCut[0] = read end    of grep->cut pipe   (read by cut)
* grepToCut[1] = write end   of grep->cut pipe   (written by grep)

### Child 1:

	// Replace process's stdout with the write of the 1st pipe
	dup2( catToGrep[ 1 ], STDOUT_FILENO );

	// Close pipes
	close( catToGrep[0] );
	close( catToGrep[1] );
	close( grepToCut[0] );
	close( grepToCut[1] );

	execv( *cat_args, cat_args );

	return 0;

### Child 2:

	// replace grep's stdin with read end of 1st pipe
	dup2( catToGrep[0], STDIN_FILENO );

	// replace grep's stdout with write end of 2nd pipe
	dup2( grepToCut[1], STDOUT_FILENO );

	// Close pipes
	close( catToGrep[0] );
	close( catToGrep[1] );
	close( grepToCut[0] );
	close( grepToCut[1] );

	execvp(*grep_args, grep_args);

	return 0;

### Child 3:

	// replace cut's stdin with input read of 2nd pipe
	dup2( grepToCut[0], STDIN_FILENO );

	// Close pipes
	close( catToGrep[0] );
	close( catToGrep[1] );
	close( grepToCut[0] );
	close( grepToCut[1] );

	execvp( *cut_args, cut_args );

	return 0;
