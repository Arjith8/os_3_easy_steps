#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    size_t size = strlen("sbbbbb");
    printf("%zu %zu", size, sizeof(char));
    return EXIT_SUCCESS;
}
