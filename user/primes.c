#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READ 0
#define WRITE 1

int main(int argc, char *argv[]) {
    int pid;
    unsigned int i;
    int *read_fd, *write_fd;
    int first_fds[2], second_fds[2];
    if (pipe(first_fds) != 0) {
        printf("pipe failed, 1\n");
        exit();
    }



    pid = fork();
    if (pid > 0) {
        close(first_fds[READ]);
        for (i = 2; i < 35; i ++) {
            write(first_fds[WRITE], &i, sizeof(i));
        }
        close(first_fds[WRITE]);
        //printf("%d: waiting.\n", getpid());
        wait();
    } else {
        read_fd = first_fds;
        write_fd = second_fds;

        while (1) {
            unsigned int val = 0, first = 0;


            close(read_fd[WRITE]);

            if (read(read_fd[READ], &first, sizeof(first)) == 4) {
                printf("prime %d\n", first);
            } else {
                //printf("%d: done\n", getpid());
                break;
            }


            if (pipe(write_fd) != 0) {
                printf("pipe failed 2\n");
            }


            pid = fork();
            if (pid > 0) {
                close(write_fd[READ]);
                while (read(read_fd[READ], &val, sizeof(val)) == 4) {
                    if (val % first == 0) {
                        continue;
                    }
                    write(write_fd[WRITE], &val, sizeof(val));
                }
                close(read_fd[READ]);
                close(write_fd[WRITE]);
                //printf("%d: waiting\n", getpid());
                wait();
                break;
            } else {
                close(read_fd[READ]);
                int* temp = read_fd;
                read_fd = write_fd;
                write_fd = temp;
            }
        }
    }
    exit();
}
