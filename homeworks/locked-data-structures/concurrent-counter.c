#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <x86intrin.h>

typedef struct {
    pthread_mutex_t lock;
    int count;
} counter_t;

void counter_init(counter_t *c){
    c->count = 0;
    pthread_mutex_init(&c->lock, NULL);
}

typedef struct {
    int target;
    counter_t *c;
} counter_worker_t;

void *worker(void *arg){

    counter_worker_t *data = arg;
    counter_t *c = data->c;
    while (1) {
        pthread_mutex_lock(&c->lock);
        if (c->count >= data->target) {
            pthread_mutex_unlock(&c->lock);
            break;
        }
        c->count++;
        pthread_mutex_unlock(&c->lock);
    }
    return NULL;
}

int main(int argc, char** argv){
    long cores = sysconf(_SC_NPROCESSORS_ONLN);
    counter_t c;
    counter_init(&c);

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

    pthread_t thread_array[threads];

    counter_worker_t args;
    args.target = target;
    args.c = &c;

    unsigned long long start = __rdtsc();

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    for (int i = 0; i < threads; i++){
        pthread_create(&thread_array[i], NULL, worker, &args);
    }
    
    for (int i = 0; i < threads; i++){
        pthread_join(thread_array[i], NULL);
    }

    unsigned long long end = __rdtsc();
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    long long elapsed_ns =
        (end_time.tv_sec - start_time.tv_sec) * 1000000000LL +
        (end_time.tv_nsec - start_time.tv_nsec);

    printf("count = %d\n", c.count);
    printf("Time: %lld ns\n", elapsed_ns);
    printf("cycles %llu\n", end - start);
    printf("\n");
    return EXIT_SUCCESS;
}
