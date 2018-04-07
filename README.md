# OSDesign
## In this implementation of xv6 we added functions to the Operating System.
* The extended the functions exit() and wait(), to exit(status) and wait(pid). (part 1)
* Implemented a Priority Scheduler algorithm that schedule jobs with the highest priority first. If the process schedul Forks the child will have the same Priority as the parent or higher.(part 2)
## Memory management
* We move the stack to the top of the address space and it grows down, the heap will grow up. before the stack was before the heap unable to grow up.(part 5)
* implemented a function that allows multiple process to share the same physical page of memory.(part 4)

