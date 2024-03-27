#include "file_reader.h"
#include "cpu_scheduler.h"
#include "io.h"
#include "metrics.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define MAX_BURSTS 100 // Maximum number of CPU bursts for a process
#define MAX_PROCESSES 100 // Maximum number of processes
#define QUANTUM_DEFAULT 10 // Default quantum for Round Robin

// Define PCB structure
typedef struct {
    int priority;
    int cpu_burst_times[MAX_BURSTS]; // Array of CPU burst times
    int io_burst_times[MAX_BURSTS];  // Array of I/O burst times
    int num_bursts;       // Number of CPU bursts
    int current_burst;    // Index of current CPU burst
    pthread_mutex_t mutex; // Mutex for synchronization
} PCB;

// Define other global variables and data structures
PCB ready_queue[MAX_PROCESSES]; // Ready queue
int num_processes = 0; // Number of processes in the ready queue
pthread_mutex_t ready_queue_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex for ready queue

// Function prototypes
void *FileRead_thread(void *args);
void *CPU_scheduler_thread(void *args);
void *IO_thread(void *args);
void calculate_metrics();

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s -alg [FIFO|SJF|PR|RR] [-quantum <integer>] -input [file name]\n", argv[0]);
        return 1;
    }

    // Parse command line arguments
    char *algorithm;
    int quantum = QUANTUM_DEFAULT;
    char *input_file;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-alg") == 0) {
            algorithm = argv[++i];
        } else if (strcmp(argv[i], "-quantum") == 0) {
            quantum = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-input") == 0) {
            input_file = argv[++i];
        }
    }

    // Initialize data structures

    // Create threads for FileRead_thread, CPU_scheduler_thread, and IO_thread
    pthread_t file_read_thread, cpu_scheduler_thread, io_thread;
    pthread_create(&file_read_thread, NULL, FileRead_thread, input_file);
    pthread_create(&cpu_scheduler_thread, NULL, CPU_scheduler_thread, &quantum);
    pthread_create(&io_thread, NULL, IO_thread, NULL);

    // Wait for all threads to finish
    pthread_join(file_read_thread, NULL);
    pthread_join(cpu_scheduler_thread, NULL);
    pthread_join(io_thread, NULL);

    // Calculate and print performance metrics
    calculate_metrics();

    return 0;
}

