#define _GNU_SOURCE
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <sched.h>

int main(){
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(1, &mask);
    sched_setaffinity(getpid(), sizeof(cpu_set_t), &mask);
    int p2c_pipe[2], c2p_pipe[2];
    if ((pipe(p2c_pipe) < 0) || (pipe(c2p_pipe) < 0)){
        printf("Pipe creation failed");
    }
    int rc = fork();
    if (rc == 0){
        char buf[2];
        read(p2c_pipe[0], buf, 2);
        write(c2p_pipe[1], "h\n", 2);
    } else {
        char buf[2];

        struct timeval start_time;
        gettimeofday(&start_time, NULL);
        long start_time_us = start_time.tv_sec * 1000 * 1000 + start_time.tv_usec;

        write(p2c_pipe[1], "h\n", 2);
        read(c2p_pipe[0], buf, 2);

        struct timeval end_time;
        gettimeofday(&end_time, NULL);
        long end_time_us = end_time.tv_sec * 1000 * 1000 + end_time.tv_usec;
        printf("Time taken for context switch roundtrip %ldus \n", end_time_us - start_time_us);
    }
    return 0;
}
