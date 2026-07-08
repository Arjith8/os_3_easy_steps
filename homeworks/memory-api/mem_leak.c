#include <stdlib.h>

int main(){
    // int *val = malloc(sizeof(int));
    int *val = malloc(100*sizeof(int));
    val[100] = 0;

    free(val+20);
    //free(val);

    return EXIT_SUCCESS;
}
