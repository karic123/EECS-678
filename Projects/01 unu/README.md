# EECS 678 Project 1, Spring 2017

Aishwarya & Rachel

hhiii

---

## Quash tutorial

### Quash in a nutshell

**quash.c**

	main ( ... ) {
		while (is_running) {
			CommandHolder *script = parse(&state);
			
			run_script (script);
		}
	}

**execute.c**

	run_script (CommandHolder* holders) {
		if ( end_condition_reached 1 ) {
			is_running = false;
		}
		for each holder in holders {
			create_process (holder);
		}
		if (holder_contains_forground_jobs) {
			wait_for_all_the_proceses_in_the_job_to_complete;
		} else {
			push_the_job_in_background_job_queue;
		}
	}

	create_process (CommandHolder holder) {
		Setup_pipes_and_io_redirection_based_on_flags;

		fork_a_child ();

		if (in_child) {
			run_child_command(holder.cmd);
			exit (EXIT_SUCCESS);
		} else {
			run_parent_command(holder.cmd);
		}
	}


	run_*****_command (Command cmd) {
		switch based on command type {
			case command_type:
				run_command_action (command_arguments);
			break;
			...
			default:
				fprintf (stderr, “Unknown Command\n”);
		}
	}
	
### Essential data structures

**command.h**

	struct CommandHolder {
		char* redirect_in;
		char* redirect_out;
		int flags;
		Command cmd;
	}

	union Command {
		SimpleCommand simple;
		GenericCommand generic;
		EchoCommand echo;
		ExportCommand export;
		CDCommand cd;
		KillCommand kill;
		PWDCommand pwd;
		JobsCommand jobs;
		ExitCommand exit;
		EOCCommand eoc;
	} Command;
	
	struct CDCommand {
		CommandType type;
		char* dir;
	} CDCommand;
	
### Quash invocation - Example 1

	>> ./quash
	[<QUASH_PROMPT>] cd/home/
	
**quash.c**

	main ( ... ) {
		while (is_running) {
			CommandHolder *script = parse(&state);
			run_script(script);
		}
	}
	
	----------------------------
	
	After 3 rd line CommandHolder structure array
	pointed to by script looks like the following:
	
	script[0].redirect_in = 0;
	script[0].redirect_out = 0;
	script[0].flags = 0;
	script[0].cmd =>
		script.cmd.type = 5;
		script.cmd.dir = “/home/”;
	
### Quash invocation - Example 2

	[<QUASH_PROMPT>] cd /home/ | ls –ll /home/
	
**quash.c**

	main ( ... ) {
		while (is_running) {
			CommandHolder *script = parse(&state);
			run_script(script);
		}
	}
	
	----------------------------
	
	After 3rd line CommandHolder structure array
	pointed to by script looks like the following:
	
	script[0].redirect_in = 0;
	script[0].redirect_out = 0;
	script[0].flags = 0x10;
	script[0].cmd =>
		script.cmd.type = CD;
		script.cmd.dir = “/home/”;
		
	script[1].redirect_in = 0;
	script[1].redirect_out = 0;
	script[1].flags = 0x10;
	script[1].cmd =>
		script.cmd.type = GENERIC;
		script.cmd.args = [“ls”, “-ll”, “/home/”];
	
**execute.c**

	run_script (CommandHolder* holders) {
		if ( end_condition_reached ) {
			is_running = false;
		}
		
		for each holder in holders {
			create_process( holder );
			----------------------------------
			Iteration-0: Create process for CD
			Iteration-1: Create process for ls
			----------------------------------
		}
		
		if (holder_contains_foreground_jobs) {
			wait_for_all_the_proceses_in_the_job_to_complete;
			----------------------------------------------
			NOTE: You need a queue here which is populated
			by create_process () function; to track the
			pids of created processes and wait for them
			to exit
			-----------------------------------------------
		}
		else {
			push_the_job_in_background_job_queue;
			-----------------------------------------------
			NOTE: Another queue required to accomplish this
			task
			-----------------------------------------------
		}
		
	}
	
	create_process (CommandHolder holder) {
		Setup_pipes_and_io_redirection_based_on_flags;
		
		pid = fork_a_child ();
		
		if (pid == 0) {
			run_child_command(holder.cmd);
			exit (EXIT_SUCCESS);
		} else {
			-----------------------------------------------
			NOTE: This is a good place to populate the pid
			queue
			-----------------------------------------------
			run_parent_command(holder.cmd);
		}
	}
	
### Command handling

* Parent side
	* export
	* cd
	* kill

* Child side
	* generic
	* echo
	* pwd
	* jobs

### Quash milestones

* get_current_directory
* create_process (Step 1) 
	- just uncomment child and parent run process functions
* lookup_env
* run_pwd
* run_cd
* run_export
* run_echo
* run_generic
* create_process (Step 2) 
	- Set up pipes to establish IO redirection among children
* run_script (Step 1) 
	- Implement PID-queue, 
	- update create_process to track the pids of children.
	- After returning from process creation, and if the job is 
	foreground, pop processes one by one from the queue and
	wait for each of them to exit
* create_process (Step 3)
	- Setup file redirection for child process outputs
* run_script (Step 2)
	- Implement background job handling
* run_kill
	- Implement signal handling in quash to process kill signal
	
---

## Notes

**Getting started with the project:**

> Hi Rachel,

> 1) Deque is a double sided que that is implemented through macros. If you open memory_pool.c and look at the top you will see where they used it to implement the memory pools. After they put the two macro lins:

	IMPLEMENT_DEQUE_STRUCT(MemoryPoolDeque, MemoryPool);
	IMPLEMENT_DEQUE(MemoryPoolDeque, MemoryPool);

> they were then able to use all of the functions from deque.h as if they were in their code for their structure.

> MemoryPoolDeque: became a struct that they could initialize to represent a que.

> MemoryPool: was a structure that could be stored in the que.

> You can do this else where in your code to manage your own structures. I personally did not use it, because I thought that I would need to remove elements from the middle of the que. Looking back, you do not need to do that. You need to decide how you want to manage your jobs and processes, and then choose the structures to hold them. If you write your own, you risk it being buggy, but if you use deque you risk using it incorrectly. Also, if you need to gdb step through deque, good luck as you will not be able to debug through the macro functions.

> 2) You do not need to know anything that happens down in the parsing directory, and anything that calls function in it, you should just be able to assume that they work fine. I would say that 90% of the work and things that you need to be familiar with happen in execute.c, unless you decide to make your own structures, then those could take a considerable amount of work also. I would also say that 90% of the things that you need to complete are very spelled out for you. How you decide to implement the project though could effect how much you need to go off coarse and edit other parts. You will need to know a little about how the commands are set up, but almost all of that work is already done. Compiling, and a quick spin through gdb through the skeleton program will be a quick way to learn just how small the area is that you need to work with, again, you do not need to step down into the parsing directory.

> 3) I did not use destroy_parser(), I feel like it is just a function that they have already registered with atexit() that you could put code in to destroy your own data structures that you make. That way, you will be able to call exit() in your program without needing to de-allocate all your global structures every time. I just made a cleanup function and then registered it with atexit() and it worked fine.

> Hope this was useful.

> Good luck.

(From another student)

**Queues:**

> You need queue for processes and for background jobs. Please note the distinction between a job and a process. 
> When you type a command in quash terminal and press enter, that creates a job i.e.

> 		<QUASH BLAH BLAH>] echo "Hello World!" | cat somefile.txt <ENTER>

> creates  a job consisting of two processes. A job is considered a background job if you put ampersand ('&') at the end of it before pressing <ENTER>. If a job is marked as background, all the processes in that job will be marked as background and hence, they should not cause the parent process to hang up before completion.

> So here is what you need:

> * You need to keep track of all the processes in a job (be it foreground or background). You can do that by creating a queue which stores process-type objects and encapsulating it in a job-type structure.

> * You need to keep track of background jobs and for that you need another queue which stores job-type objects. As soon as you have created all the processes in a background job, you push that job into the background job queue

(From the TA)


**Job IDs:**

> Job id should be assigned in incremental order i.e. first job has id=1 second has id=2 and so on. Also, run_script () (in the skeleton code you have been given) does not do anything with the pids or the job queues. It is up-to you how you want to manage this. Ideally pids should go into job structure and job structure should go into job queue.

> I am a little confused about your question. Can you please send me an email @ (wali@ku.edu); describing the problem you are facing in detail so that I may give some specific help with the problem you are facing?

(From the TA)

---

## "Hints" from the main readme spec:

In Quash, a job is defined as a single command or a list of commands separated
by pipes. For example the following are each one job:

cat file.txt       # A job with a single process running under it

find | grep *.qsh  # A job with two processes running under it

A job may contain more than one process and should have a unique id for the
current list of jobs in Quash, a knowledge of all of the pids for processes that
run under it, and an expanded string depicting what was typed in on the command
line to create that job. When passing the pid to the various print job functions
you just need to give one pid associated with the job. The job id should also be
assigned in a similar manner as bash assigns them. That is the job id of a new
background job is one greater than the maximum job id in the background job
list. Experiment with background jobs in Bash for more details on the id
assignment.

The structure of the functions in src/execute.c will be explained in the
following paragraphs.

The entry level function for execution in quash is @a run_script(). This
function is responsible for calling @a create_process() on an array of @a
CommandHolders. After all of the processes have been created for a job,
run_script() should either wait on all processes inside a foreground job to
complete or add it to the background job list without waiting if it is a
background job.

The @a create_process() function is intended to be the place where you fork
processes, handle pipe creation, and file redirection. You should not call
execvp(3) from this function. Instead you should call derivatives of the @a
example_run_command() function. Also you can determine whether you should use
the boolean variables at the top of this function to determine if pipes and
redirects should be setup. It may be necessary to keep a global execution state
structure so that different calls to create process can view important
information created in previous invocations of create_process() (i.e. the file
descriptors for open pipes of previous processes).

When implementing the run_<command type> functions in src/execute.c, the command
structures usually hold everything needed to pass to the corresponding function
calls. For example, the @a GenericCommand structure contains an args field fully
formatted and ready to pass into the argv argument of the @a execvp(3) library
function. The one exception to this rule is @a run_cd(). In the @a CDCommand
structure, the field dir is the path that the user typed. This needs to be
expanded to an absolute path with @a realpath(3) before you use it.

You should not have to search for environment variables ($) in your functions.
The parser uses your implementation of @a lookup_env() function to expand the
environment variables for you.

---

## Dynamic Pipeline help

Hi Everyone,

I hope you are making good progress with your project-1. Regarding the questions I have gotten from you so far, it seems that the most troublesome part for most of the students is the creation of a dynamic pipe-line which can cater to multiple processes on the fly. I gave a simple algorithm to a couple of students which can aid them in this task. To be fair to all other students, I am sharing the algorithm here. You can use it in your code but make sure that you understand it because that's what matters.

 

A couple of observations regarding a dynamic pipe-line:

 

- A dynamic pipe-line should be capable of handling multiple processes i.e.

	>> find src -name *.[ch] | xargs grep -c pipe | sort | head -n 5

 

- In order to construct such a pipe-line, all we need to keep track of, is two pipes. These are (Make sure that you understand this):

    The pipe the previous child process has written to (so that we may read from it in the new child process)
    The pipe the new child process is going to write to (so that the next child may read from it)

 

- Pipes need to be connected (via dup2) only in the child process. And they should be created only in the parent process

 

- In order to connect n-processes, n-1 pipes are required

 

- Pipes are used only in blocking mode i.e. the second stage of a pipe-line cannot proceed until the first stage has completed

 

With this information at hand, the following algorithm can take care of your dynamic pipe-lining needs in quash.



File: execute.c

	#define    READ_END     0
	#define    WRITE_END    1

	/* Create a global tracker for active pipes */
	static int environment_pipes[2][2];        	/* Global Variable */
	static int prev_pipe = -1;                 ​​​	/* Global Variable */
	static int next_pipe = 0;                  	/* Global Variable */

	...

	create_process (...)
	{

		​...

	if (p_out) {
		/* This is the only condition under which a new pipe creation is required.
		   You should be able to understand why this is the case */
		pipe (environment_pipes[next_pipe]);
	}

	...

	pid = fork ();

	if (0 == pid) {
		/* Check if this process needs to receive from previous process */
		if (p_in) {
			dup2 (environment_pipes[prev_pipe][READ_END], STDIN_FILENO);

			
			/* We are never going to write to the previous pipe so we can safely close it */
			close (environment_pipes[prev_pipe][WRITE_END]);
		}

		if (p_out) {
			dup2 (environment_pipes[next_pipe][WRITE_END], STDOUT_FILENO);

			/* We are never going to read from our own pipe so we can safely close it */
			close (environment_pipes[next_pipe][READ_END]);
		}

		/* Execute what-ever needs to be run in the child process */
		child_run_command (blah blah);

		/* Adios child process */
		exit (EXIT_SUCCESS);
	} else {
		/* Close the hanging pipes in parent */
		if (p_out) {
			close (environment_pipes[next_pipe][WRITE_END]);
		}

		/* Update the pipe trackers for next iteration */
		next_pipe = (next_pipe + 1) % 2;
		prev_pipe = (prev_pipe + 1) % 2;
	}

	/* This function can safely kick  the bucket now */
	return;

 

If you have trouble understanding this algorithm, try to work it out for this example:

	>> find src -name *.[ch] | head -n 3

 

Try answering the following question for the above pipe-line when it is constructed in quash using the described algorithm:

    What is the pipe-descriptor that find process is writing to?
    What is the pipe-descriptor that head process is reading from?
    How many pipes are constructed in the algorithm when the above command is executed by quash?

 

Please note that this algorithm takes care of all your dynamic pipe-lining needs i.e. you don't need to do anything else with pipes if you just appropriately include the stated algorithm in your code.
