#include <bits/types/struct_timeval.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
int main()
{
    struct timeval start_time;
    gettimeofday(&start_time, NULL);
    long start_time_us = start_time.tv_sec * 1000 * 1000 + start_time.tv_usec;

    int rc = fork();

    if (rc > 0) {
        struct timeval end_time;
        gettimeofday(&end_time, NULL);
        long end_time_us = end_time.tv_sec * 1000 * 1000 + end_time.tv_usec;
        printf("Time taken for fork is %ld \n", end_time_us - start_time_us);
    }
}
