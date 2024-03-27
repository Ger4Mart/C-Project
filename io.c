#include "main.h" // Include the header file where global variables and types are declared
#include <pthread.h> // Include the pthread header file for thread-related functions
#include <unistd.h> // Include the header file for usleep function

void *IO_thread(void *args) {
    while (1) {
        pthread_mutex_lock(&ready_queue_mutex); // Lock the ready queue mutex
        if (num_processes > 0) {
            // Initialize variables for selecting the best process
            PCB best_process = ready_queue[0];
            int best_index = 0;
            int best_priority = ready_queue[0].priority; // Assuming priority is the criterion

            // Iterate through the processes in the I/O queue
            for (int i = 1; i < num_processes; i++) {
                // Evaluate the current process based on your selection criteria
                if (ready_queue[i].priority < best_priority) {
                    // Update the best process if the current process is better
                    best_process = ready_queue[i];
                    best_index = i;
                    best_priority = ready_queue[i].priority;
                }
            }

            // Remove the best process from the I/O queue
            for (int i = best_index; i < num_processes - 1; i++) {
                ready_queue[i] = ready_queue[i + 1];
            }
            num_processes--;
            pthread_mutex_unlock(&ready_queue_mutex); // Unlock the ready queue mutex

            // Execute I/O burst time
            int io_burst_time = best_process.io_burst_times[best_process.current_burst];
            usleep(io_burst_time * 1000); // Sleep in milliseconds

            // Move the best process back to the ready queue
            pthread_mutex_lock(&ready_queue_mutex); // Lock the ready queue mutex
            ready_queue[num_processes++] = best_process;
            pthread_mutex_unlock(&ready_queue_mutex); // Unlock the ready queue mutex
        } else {
            pthread_mutex_unlock(&ready_queue_mutex); // Unlock the ready queue mutex
        }
        usleep(1000); // Sleep for 1 millisecond to avoid busy-waiting
    }
    return NULL;
}
