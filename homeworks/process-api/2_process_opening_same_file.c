#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    int file_open_status = open("file.txt", O_WRONLY);
    int rc = fork();
    if ((rc < 0) || (file_open_status < 0)){
        printf("failed to open the file or start the child process");
    } else if (rc == 0){
        int file_open_status_child = open("file.txt", O_WRONLY);
        printf("status code of child opening the file %d", file_open_status_child);
        write(file_open_status, "hello", 5);
    } else {
        write(file_open_status, "hello_", 6);
    }
}
