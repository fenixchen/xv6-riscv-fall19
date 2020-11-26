#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char *argv[]) {
    int tick;

    if(argc != 2) {
        printf("usage: sleep ms\n");
        exit();
    }
    tick = atoi(argv[1]);
    sleep(tick);
    exit();
}
