#include "kernel/types.h"
#include "user.h"
#include "kernel/stat.h"
#include "kernel/fs.h"


void find(char *path, char *name, char *file_name) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    
    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if (strcmp(file_name, name) == 0) {
        printf("%s\n", path);
    }
    if (st.type == T_DIR) {
        
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
            printf("ls: path too long\n");
            return;
        }

        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (!de.inum || de.name[strlen(de.name) - 1] == '.') continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            find(buf, name, de.name);
        }
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "find: invalid argument list\n");
        exit(-1);
    }

    find(argv[1], argv[2], argv[1]);
    exit(0);
}
//wget -P . https://gitee.com/ftutorials/gdb-dashboard/raw/master/.gdbinit
//grep -qxF 'set auto-load safe-path /' ./.gdbinit || echo 'set auto-load safe-path /' >> ./.gdbinit