# Rachel's Notes - Lab 4 - EECS 678, Spring 2017

[POSIX Signals](http://www.ittc.ku.edu/~heechul/courses/eecs678/S17/labs/signal.html)

## Reference material

* [Signal Handling, gnu.org](https://www.gnu.org/software/libc/manual/html_node/Signal-Handling.html)
* [C signal handling (Wikipedia)](https://en.wikipedia.org/wiki/C_signal_handling)
* [IPC:Interrupts and Signals](https://users.cs.cf.ac.uk/Dave.Marshall/C/node24.html)

---

# Lab Notes

You can mask signals so that the default action is not executed.
It cannot mask SIGKILL and SIGSTOP.
