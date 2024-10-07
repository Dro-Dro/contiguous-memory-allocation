#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"

#define MEMSIZE    80

bool progRun = true;
char memory[MEMSIZE] = "................................................................................";

// Allocate N bytes for a process using one of the 3 allocation algorithms
void allocate(char *name, int bytes, char *algo) {
    printf("Allocating %d bytes for process %s using %s\n", bytes, name, algo);
    char nAlgo = *algo;
    char nName = *name;
    bool isBigEnough = false;
    bool isBlock = false;
    int blockSize = 0;
    if (nAlgo == 'F') {
        int blockStart;
        int blockEnd;

        bool gate = true;
        int z = 0;

        while (gate && z < MEMSIZE) {
            if (memory[z] == '.') {
                if (!isBlock) {
                    isBlock = true;
                    blockStart = z;
                }
                blockSize++;
            } else {
                if (isBlock) {
                    isBlock = false;
                    blockEnd = z - 1;
                    if (blockSize < bytes) {
                        blockSize = 0;
                    }
                }
            }

            if (blockSize >= bytes) {
                blockEnd = z;
                isBigEnough = true;
                for (int i = 0; i < bytes; i++) {
                    if (i + blockStart <= blockEnd) {
                        memory[i + blockStart] = nName;
                    }
                }
                gate = false;
            }
            z++;
        }
    } else if (nAlgo == 'W') {
        int blockStart;
        int blockEnd;
        int worstStart;
        int worstEnd;
        int worstSize = 0;
        
        for (int i = 0; i < MEMSIZE; i++) {
            if (memory[i] == '.') {
                if (!isBlock) {
                    isBlock = true;
                    blockStart = i;
                }
                blockSize++;
            } else {
                if (isBlock) {
                    isBlock = false;
                    blockEnd = i - 1;
                    if (blockSize < bytes) {
                        blockSize = 0;
                    } else {
                        if (blockSize > worstSize) {
                            worstSize = blockSize;
                            worstStart = blockStart;
                            worstEnd = blockEnd;
                        }
                    }
                }
            }
        }

        if (worstSize >= bytes) {
            isBigEnough = true;
            for (int i = 0; i < bytes; i++) {
                if (i + worstStart <= worstEnd) {
                    memory[i + worstStart] = nName;
                }
            }
        }

    } else if (nAlgo == 'B') {
        int blockStart;
        int blockEnd;
        int bestStart;
        int bestEnd;
        int bestSize = 100;
        
        for (int i = 0; i < MEMSIZE; i++) {
            if (memory[i] == '.') {
                if (!isBlock) {
                    isBlock = true;
                    blockStart = i;
                }
                blockSize++;
            } else {
                if (isBlock) {
                    isBlock = false;
                    blockEnd = i - 1;
                    if (blockSize < bytes) {
                        blockSize = 0;
                    } else {
                        if (blockSize < bestSize) {
                            bestSize = blockSize;
                            bestStart = blockStart;
                            bestEnd = blockEnd;
                        }
                        blockSize = 0;
                    }
                }
            }
        }


        if (bestSize >= bytes) {
            isBigEnough = true;
            for (int i = 0; i < bytes; i++) {
                if (i + bestStart <= bestEnd) {
                    memory[i + bestStart] = nName;
                }
            }
        }

    } else {
        printf("INCORRECT ALGORITHM: %s\n", algo);
    }

    if (!isBigEnough) {
            printf("Insufficient contiguous memory to allocate process %s of size %d\n\n", name, bytes);
    } else {
        //printf("New Memory Allocation: \n%s \n\n", memory);
    }
}

// Free all allocations for a given process
void freeAllo(char *name) {
    printf("Freeing all allocations owned by process %s\n", name);
    char nName = *name;
    for (int i = 0; i < MEMSIZE; i++) {
        if (memory[i] == nName) {
            memory[i] = '.';
        }
    }
}

// Compact the allocations, making them into one contiguous block.
// (This somewhat resembles the operation of a mark-sweep garbage collector in C#)
void compact() {
    printf("Compacting the memory pool \n\n");
    char compMemory[MEMSIZE] = "................................................................................";
    int j = 0;
    for (int i = 0; i < MEMSIZE; i++) {
        if (memory[i] != '.') {
            compMemory[j] = memory[i];
            j++;
        }
    }
    for (int i = 0; i < MEMSIZE; i++) {
        memory[i] = compMemory[i];
    }
    printf("New Compacted Memory Allocation: \n%s \n\n", memory);
}

// Read a script – a sequence of commands from a file, and execute them
void readFile(char *fileName) {
    printf("\n\nReading file: %s\n\n", fileName);
    FILE *in;
    char *temp;
    char task[MEMSIZE];

    char *command;
    char *name;
    int bytes;
    char *algo;

    in = fopen(fileName,"r");

    while ((fgets(task,MEMSIZE,in) != NULL) && progRun == true) {
        temp = strdup(task);
        command = strsep(&temp," ");
        char cmd = command[0];

        // need to turn command into one char
        if (strcmp(command, a) == 0) {
            name = strsep(&temp," ");
            bytes = atoi(strsep(&temp," "));
            algo = strsep(&temp," ");
            allocate(name, bytes, algo);
        } else if (strcmp(command, f) == 0) {
            name = strsep(&temp," ");
            freeAllo(name);
        } else if (cmd == 'S') {
            printf("Showing memory: \n%s \n\n", memory);
        } else if (strcmp(command, r) == 0) {
            name = strsep(&temp," ");
            readFile(name);
        } else if (cmd == 'C') {
            compact();
        } else if (strcmp(task, e) == 0) {
            printf("Exiting program.\n");
            progRun = false;
        } else {
            printf("INCORRECT COMMAND: %s\n", command);
        }

        free(temp);
    }

    fclose(in);
    printf("\n\nClosing file: %s\n\n", fileName);
}

int main(int argc, char *argv[]) {
    printf("Below is the starting memory.\n");
    printf("Printing memory: \n%s \n\n", memory);
    readFile(argv[1]);
    return 0;    
}