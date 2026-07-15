#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <x86intrin.h>

typedef struct counter{
    pthread_mutex_t lock;
    int count;
} counter_t;

typedef struct{
    counter_t *local_counter;
    counter_t *global_counter;
    int *target;
} worker_arg_t;

void counter_init(counter_t *c){
    pthread_mutex_init(&c->lock, NULL);
    c->count = 0;
}

int THRESHOLD = 5;

void *worker(void* arg){
    worker_arg_t *data = arg;
    while (1){
        data->local_counter->count++;
        if (data->local_counter->count >= THRESHOLD){
            pthread_mutex_lock(&data->global_counter->lock);
            if (data->global_counter->count >= *data->target){
                pthread_mutex_unlock(&data->global_counter->lock);
                return NULL;
            }
            data->global_counter->count+=data->local_counter->count;
            data->local_counter->count = 0;
            pthread_mutex_unlock(&data->global_counter->lock);
        }
    }
}

int main(int argc, char *argv[]){
    long cores = sysconf(_SC_NPROCESSORS_ONLN);

    counter_t global_counter;
    counter_init(&global_counter);

    int target = 1000;

    int base = 10;
    char *end_ptr;
    errno = 0;
    if (argc >= 3 && strcmp(argv[1], "-c") == 0){
        target = strtol(argv[2], &end_ptr, base);
        if ((errno == ERANGE) || (end_ptr == argv[2])){
            fprintf(stderr, "No digits found: %s\n", argv[2]);
            exit(EXIT_FAILURE);
        }
    }

    int threads = cores;
    errno = 0;
    if (argc >= 5 && strcmp(argv[3], "-t") == 0){
        threads = strtol(argv[4], &end_ptr, base);
        if ((errno == ERANGE) || (end_ptr == argv[4])){
            fprintf(stderr, "No digits found: %s\n", argv[4]);
            exit(EXIT_FAILURE);
        }
    }

    errno = 0;
    if (argc >= 7 && strcmp(argv[5], "-s") == 0) {
        THRESHOLD = strtol(argv[6], &end_ptr, base);
        if ((errno == ERANGE) || (end_ptr == argv[6])) {
            fprintf(stderr, "No digits found: %s\n", argv[6]);
            exit(EXIT_FAILURE);
        }
    }

    pthread_t thread_array[threads];
    counter_t sub_counters[threads];
    worker_arg_t args[threads];
    struct timespec start, end;

    unsigned long long start_cycles = __rdtsc();
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < threads; i++){
        counter_init(&sub_counters[i]);
        args[i].global_counter = &global_counter;
        args[i].local_counter = &sub_counters[i];
        args[i].target = &target;
        pthread_create(&thread_array[i], NULL, worker, &args[i]);
    }

    for (int i = 0; i < threads; i++){
        pthread_join(thread_array[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    unsigned long long end_cycles = __rdtsc();

    long long elapsed =
        (end.tv_sec - start.tv_sec) * 1000000000LL +
        (end.tv_nsec - start.tv_nsec);

    printf("Target      : %d\n", target);
    printf("Threads     : %d\n", threads);
    printf("Threshold   : %d\n", THRESHOLD);
    printf("Global Count: %d\n", global_counter.count);
    printf("Time        : %lld ns\n", elapsed);
    printf("Cycles      : %llu\n", end_cycles - start_cycles);

    return EXIT_SUCCESS;
}
