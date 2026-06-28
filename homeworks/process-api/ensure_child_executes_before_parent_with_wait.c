#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main()
{
    printf("start\n");
    int rc = fork();
    int status;
    int hh = wait(&status);
    if (rc < 0){
        printf("unsuccessful creation of sub process");
    } else if (rc==0){
        printf("from child\n");
    } else {
        printf("from parent\n");
    }
    printf("%d %d %d \n", rc, status, hh);
}
