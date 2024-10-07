# Contiguous Memory Allocation
## Description
Application that manages memory allocations within a memory pool of size MEMSIZE; supporting six different requests.
## Goals
Create a application that manages memory allocations and supports the following commands:
1. **Allocate** N bytes for a process using one of the 3 allocation algorithms
2. **Free** all allocations for a given process
3. **Show** the status of the memory pool – allocated and free blocks
4. **Read** a script – a sequence of commands from a file, and execute them
5. **Compact** the allocations, making them into one contiguous block.  (This somewhat resembles the operation of a mark-sweep garbage collector in C#)
6. **Exit**
