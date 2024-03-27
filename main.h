#ifndef MAIN_H
#define MAIN_H

#include <pthread.h>

#define MAX_BURSTS 100 // Maximum number of CPU bursts for a process
#define MAX_PROCESSES 100 // Maximum number of processes

// Define PCB structure
typedef struct {
    int priority;
    int cpu_burst_times[MAX_BURSTS]; // Array of CPU burst times
    int io_burst_times[MAX_BURSTS];  // Array of I/O burst times
    int num_bursts;       // Number of CPU bursts
    int current_burst;    // Index of current CPU burst
    pthread_mutex_t mutex; // Mutex for synchronization
} PCB;

extern PCB ready_queue[MAX_PROCESSES]; // Ready queue
extern int num_processes; // Number of processes in the ready queue
extern pthread_mutex_t ready_queue_mutex; // Mutex for ready queue

#endif /* MAIN_H */
