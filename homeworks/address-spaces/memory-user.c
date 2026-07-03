#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc <= 2){
        fprintf(stderr, "to call this utility, atleast -m argument with value should be used\n");
        return 1;
    }

    if (strcmp(argv[1], "-m") != 0){
        fprintf(stderr, "First argument should be -m\n");
        return 1;
    }

    printf("%d\n", (int)getpid());

    size_t size_to_occupy_mb = atoi(argv[2]);
    size_t size_to_occupy_byte = size_to_occupy_mb * 1024 * 1024;

    size_t num_of_elems_required = size_to_occupy_byte / sizeof(int);
    int *mem_hoarder = malloc(size_to_occupy_byte);

    while (1){
        for (size_t i=0; i < num_of_elems_required; i++){
            mem_hoarder[i] = 0;
        }
    }

    return EXIT_SUCCESS;
}
