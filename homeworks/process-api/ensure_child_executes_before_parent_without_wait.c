#include <stdio.h>
#include <unistd.h>

int main()
{
    printf("start\n");
    int rc = fork();
    if (rc < 0){
        printf("unsuccessful creation of sub process");
    } else if (rc==0){
        printf("from child\n");
    } else {
        sleep(1);
        printf("from parent\n");
    }
}
