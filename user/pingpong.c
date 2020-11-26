#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char *argv[]) {
    unsigned char val;
    int pid, parent_fd[2], child_fd[2], count;
    if (pipe(parent_fd) != 0 || pipe(child_fd) != 0) {
        printf("pipe() failed\n");
        return 0;
    } else {
        printf("parent_fd: %d, %d\n", parent_fd[0], parent_fd[1]);
        printf("child_fd: %d, %d\n", child_fd[0], child_fd[1]);
    }
    pid = fork();
    if (pid == 0) {
        close(parent_fd[1]);
        close(child_fd[0]);

        val = 0;
        printf("child reading\n");
        count = read(parent_fd[0], &val, 1);
        printf("%d: received ping\n", getpid());


        val = 3;
        count = write(child_fd[1], &val, 1);
        printf("child write:%d, %d\n", count, val);


    } else {
        close(parent_fd[0]);
        close(child_fd[1]);

        val = 4;
        count = write(parent_fd[1], &val, 1);
        printf("parent write %d, %d\n", count, val);

        val = 0;
        printf("parent reading\n");
        count = read(child_fd[0], &val, 1);
        printf("%d: received pong\n", getpid());
    }
    exit();
    return 0;
}
