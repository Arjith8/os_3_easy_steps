#include <stdio.h>
#include <unistd.h>
int main()
{
    int x = 100;
    int rc = fork();
    if (rc < 0){
        fprintf(stderr, "Failed to fork the process");
    } else if (rc==0){
        printf("Value of x in child: %d \n", x);
    } else {
        printf("Value of x in parent: %d \n", x);
    }
}
