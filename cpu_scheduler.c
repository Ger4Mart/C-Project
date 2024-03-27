#include <unistd.h>
#include <stdio.h>
#include "main.h"

void *CPU_scheduler_thread(void *args) {
    int quantum = *(int *)args;

    // Implement CPU scheduling algorithm
    while (1) {
        pthread_mutex_lock(&ready_queue_mutex);
        if (num_processes > 0) {
            // Pick process from ready queue based on the selected algorithm
            PCB process = ready_queue[0];
            int chosen_index = 0;
            for (int i = 1; i < num_processes; i++) {
                // Your existing algorithm selection logic goes here
            }
            // Remove selected process from ready queue
            for (int i = chosen_index; i < num_processes - 1; i++) {
                ready_queue[i] = ready_queue[i + 1];
            }
            num_processes--;
            pthread_mutex_unlock(&ready_queue_mutex);

            // Execute process for the appropriate burst time (CPU or quantum)
            int burst_time = (process.current_burst % 2 == 0) ? process.cpu_burst_times[process.current_burst] : quantum;
            usleep(burst_time * 1000); // Sleep in milliseconds

            // Update total CPU time
            total_cpu_time += burst_time;

            // Handle transitions (to I/O queue or termination)
            if (process.current_burst % 2 == 0) {
                // CPU burst completed, move to I/O queue or terminate
                process.current_burst++;
                if (process.current_burst < process.num_bursts) {
                    // Add to I/O queue (if needed)
                } else {
                    // Terminate
                    printf("Process terminated: Priority %d\n", process.priority);
                    pthread_mutex_destroy(&process.mutex);
                    total_turnaround_time += total_cpu_time; // Update total turnaround time
                    total_processes_completed++; // Update total completed processes count
                    continue; // Skip moving to I/O queue
                }
            } else {
                // I/O burst completed, move back to ready queue (if needed)
                pthread_mutex_lock(&ready_queue_mutex);
                ready_queue[num_processes++] = process;
                pthread_mutex_unlock(&ready_queue_mutex);
            }
        } else {
            pthread_mutex_unlock(&ready_queue_mutex);
        }
    }

    return NULL;
}

