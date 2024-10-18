#include "kernel/types.h"
#include "user.h"

int main(int argc, char* argv[]) {
    int ctf[2];
    int ftc[2];
    pipe(ctf);
    pipe(ftc);
    char ibuf[512];
    char obuf[512];
    memset(ibuf, 0, 512);
    memset(obuf, 0, 512);
    if (fork() == 0) {
        int cpid = getpid();
        close(ftc[1]);
        int n = read(ftc[0], ibuf, 512), fpid;
        fpid = atoi(ibuf);
        if (n > 0) {
            printf("%d: received ping from pid %d\n", cpid, fpid);
        }
        itoa(cpid, obuf);
        close(ctf[0]);
        write(ctf[1], obuf, 512);
    }
    else {
        // parent
        int fpid = getpid();
        itoa(fpid, obuf);
        close(ftc[0]);
        write(ftc[1], obuf, 512);
        
        close(ctf[1]);
        int n = read(ctf[0], ibuf, 512), cpid;
        cpid = atoi(ibuf);
        if (n > 0) {
            printf("%d: received pong from pid %d\n", fpid, cpid);
        }
    }
    exit(0);
}