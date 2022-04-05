# OperatingSystemsAsgnOne
First Assignment for Operating Systems (Spring 2022) - Bounded-Buffer with shared memory
The task for this assignment was to implement a solution for the Producer-Consumer Problem. There is a table of shared data between the two processes, the producer produces data until the table is full then waits. The consumer then takes all the data until the table is empty and waits. The table should be implemented to only hold two items. 

The header file includes a basic struct containing the necessary elements like the semaphores and table array.
Rarely the program would halt after the first ./producer & ./consumer & line and would need to be entered again but this solved the problem when if it occurs.
