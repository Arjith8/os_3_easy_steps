#define _GNU_SOURCE
#include <sched.h>
#include <stdlib.h>

#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

int main(int argc, char** argv)
{
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);
    sched_setaffinity(0, sizeof(set), &set);
    long PAGE_SIZE = sysconf(_SC_PAGESIZE);

    int runs = 1;
    int tries = 1;

    if (argc >= 2){
        if (strcmp(argv[1], "-r") == 0) {
            runs = atoi(argv[2]);
            if (argc >= 4){
                if (strcmp(argv[3], "-t") == 0) {
                    tries = atoi(argv[4]);
                }
            }
        }
    }

    size_t size = runs * PAGE_SIZE;
    char* arr = malloc(size);

    struct timeval start_tv;
    gettimeofday(&start_tv, NULL);
    
    volatile int temp = 0;
    for (int j = 0; j < tries; j++){
        for (int i = 0; i < size; i += PAGE_SIZE){
            temp = arr[i];
        }
    }

    struct timeval end_tv;
    gettimeofday(&end_tv, NULL);
    free(arr);
    double elapsed_us = (double)((end_tv.tv_sec - start_tv.tv_sec) * 1000000L +
                            (end_tv.tv_usec - start_tv.tv_usec));

    double accesses = (double)runs * tries;
    printf("Total time elapsed to access %d pages for %d runs is %f at %f us/access\n",
        runs, tries, elapsed_us, elapsed_us / accesses);

}
