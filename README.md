# Completely Fair Scheduler
Implementation of the completely fair scheduler

## What is Completely Fair Scheduler?
CFS relies on maintaining balance in allocating processor time to tasks, based on policies and dynamic priorities assigned per task. Process scheduling under CFS is implemented under the premise that it has an "ideal, precise multi-tasking CPU," that equally powers all processes at its peak capacity.


## Working of CFS
* Each runnable process have a virtual time associated with it in PCB (process control block).
* Whenever a context switch happens(or at every scheduling point) then current running process virtual time is increased by virtualruntime_currprocess+=T.
* where T is time for which it is executed recently.
* Runtime for the process therefore monotonically increases.

## CFS vs Round Robin - Which is better?
With Round Robin, each of the processes on the ready queue gets an equal share of CPU time, but the processes that are blocked/waiting for I/O sit on the I/O queue for a long time, and they don't get any built-up credit for that once they get back into the ready queue.

With CFS, processes DO get credit for that waiting time, and will get more CPU time once they are no longer blocked. That helps reward more interactive processes (which tend to use more I/O) and promotes system responsiveness.

## Our Implementation
* We use RED-BLACK TREE data structure to implement the CFS algorithm
* We will be using complex mathematical equations such as Mandelbrot set and Julia set as processes
