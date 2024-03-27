#include "main.h" // Include the header file where global variables and types are declared
#include <unistd.h> // Include the header file for usleep function
#include <stdio.h>
#include <stdlib.h> // Include the header file for exit function
#include <string.h>

void *FileRead_thread(void *args) {
    char *input_file = (char *)args;
    FILE *file = fopen(input_file, "r");
    if (!file) {
        perror("Error opening file");
        exit(1); // Terminate the program if file opening fails
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "proc", 4) == 0) {
            // Parse process information and populate PCB
            pthread_mutex_lock(&ready_queue_mutex); // Lock the ready queue mutex
            PCB process;
            char *token = strtok(line, " ");
            token = strtok(NULL, " "); // Skip "proc"
            process.priority = atoi(token);
            token = strtok(NULL, " "); // Get the first burst time
            int i = 0;
            while (token != NULL) {
                process.cpu_burst_times[i] = atoi(token);
                token = strtok(NULL, " ");
                process.io_burst_times[i] = atoi(token);
                token = strtok(NULL, " ");
                i++;
            }
            process.num_bursts = i;
            process.current_burst = 0;
            pthread_mutex_init(&process.mutex, NULL); // Initialize the process mutex
            ready_queue[num_processes++] = process; // Add the process to the ready queue
            pthread_mutex_unlock(&ready_queue_mutex); // Unlock the ready queue mutex
        } else if (strncmp(line, "sleep", 5) == 0) {
            int sleep_time = atoi(line + 6);
            usleep(sleep_time * 1000); // Sleep in milliseconds
        } else if (strncmp(line, "stop", 4) == 0) {
            break; // End of file, exit the loop
        }
    }

    fclose(file); // Close the file
    return NULL;
}

