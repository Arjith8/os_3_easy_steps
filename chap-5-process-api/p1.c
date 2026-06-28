#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    printf("hello (pid:%d) \n", (int) getpid());
    int rc = fork();
    int status;
    wait(&status);
    if (rc < 0){
        fprintf(stderr, "fork failed \n");
    } else if (rc == 0){
        printf("child (pid:%d) \n", (int) getpid());
    } else {
        printf("parent of %d (pid:%d)\n", rc, (int)getpid());
    }
    return 0;
}
