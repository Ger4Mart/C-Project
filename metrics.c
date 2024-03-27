#include <stdio.h>
#include "metrics.h"

void calculate_metrics(double total_cpu_time, double simulation_time, int total_processes_completed, double total_turnaround_time, double total_waiting_time) {
    double cpu_utilization = (total_cpu_time / simulation_time) * 100;
    double throughput = (double)total_processes_completed / simulation_time;
    double avg_turnaround_time = total_turnaround_time / total_processes_completed;
    double avg_waiting_time = total_waiting_time / total_processes_completed;

    // Print the results
    printf("CPU utilization : %.2f%%\n", cpu_utilization);
    printf("Throughput : %.3f processes / ms\n", throughput);
    printf("Avg. Turnaround time : %.1fms\n", avg_turnaround_time);
    printf("Avg. Waiting time in R queue : %.1fms\n", avg_waiting_time);
}


