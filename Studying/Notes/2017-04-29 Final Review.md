# Final Review

FML.

* [Go to my midterm analysis](#midterm-analysis)
* [Go to my final review stuff](#final-exam-review)
* [Go to my final exam postmortem](#final-exam-postmortem)

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

*Other things not necessarily on the slides:*

* *Multiprogramming*
* *Time sharing*
* *Dual mode hardware mechanism*
* *"who" manages the CPU cache?*
* *What hardware support is required to implement a preemptive CPU scheduler?*
* *Where CPU register values are stored*
* *Where global variables are stored*
* *Anonymous piping*
* *User-level threading*
* *Protection of multi-threaded architecture vs. multi-process architecture*

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

Types of application problems:

* Given code that forks a process and runs an exec command, what is the output?
* Spinlock code with fill-in-the-blank parts to complete it.
* Multithreaded program without locks, possible outcomes.
* Implementing a monitor and a sempahore for bounded-buffer problem.
	* Subjective question of "Which version is easier to understand?" that I apparently got 1 point off on???
* Given tables of processes and resources, figure out if there is a deadlock.
* Given a table of processes and arrival/burst times, draw Gantt diagrams and calculate averages.

---

# Final Exam review

---

# Final Exam postmortem
