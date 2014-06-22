#ifndef PROCESS_H
#define PROCESS_H

#include "hardware/mmu_c.h"
#include "hardware/paging.h"

#define PROCESS_START_PAGE_COUNT 8

// int main() of processes
typedef int(*process_entry_func)(void);

typedef enum {
	Running, // Currently executing
	Waiting, // Waiting for resource/IO etc
	Ready    // Ready to be executed
} processState;

typedef enum {
	processPriorityLow,
	processPriorityMedium,
	processPriorityHigh,
	processPriorityVeryHigh
} processPriority;

// Stores process state when another process is being scheduled
typedef struct {
	unsigned int r0;
	unsigned int r1;
	unsigned int r2;
	unsigned int r3;
	unsigned int r4;

	unsigned int r5;
	unsigned int r6;
	unsigned int r7;
	unsigned int r8;
	unsigned int r9;

	unsigned int r10;
	unsigned int r11;
	unsigned int r12;
	unsigned int r13; // SP
	unsigned int r14; // LR
	unsigned int r15; // PC
} registers;

typedef struct {
	// Register state needs to be restored
	registers* registers;

	unsigned int    id;           // 
	processPriority priority;     // The priority of the task
	unsigned int    timeElapsed;  // How long this task has been running since it was switched in
	unsigned int    active;       // Whether this task is currently executing
	processState    state;        // Current state of task
    process_entry_func entry;     // Entry point of task in memory
    unsigned int    started;      // Whether the task has started
    int             result;       // The return value of the entry function
    char*           name;         // Print friendly name
    char*           path;         // Path to the executable (if any)
    unsigned int*   ttb0;         // Address to page table for this task
    unsigned int*   mem_pages;    // Array of all allocated pages for the user memory
    unsigned int    num_mem_pages;// Numer of elements in mem_pages
    unsigned int*   ttb0_physical;// Physical address to the ttb0 
    unsigned int    num_ttb0_pages;// Number of pages the ttb0 spans
    ttbc_ttbr0_size ttb0_size;     // The size of the ttb0 to set TTBC split to
} Process;

Process* Process_Create(char* filename, char* name);

// Frees all memory associated with the given task
void Process_Delete(Process* p);

#endif