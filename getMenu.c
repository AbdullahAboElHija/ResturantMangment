#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    //function that printing the menu of resturnat and if it not exist it print no resturant like this
    char resName[50], buff[256] = { 0 };
    int fd_from, rbytes;
    strcpy(resName, argv[1]);
    strcat(resName, ".txt");
    if ((fd_from = open(resName, O_RDONLY)) == -1) { perror("no resturant like this."); return(-1); }
    if ((rbytes = read(fd_from, buff, 256)) == -1) {
        perror("read 1");    close(fd_from);
        ; return(-1);
    }
    while (rbytes > 0) {
        printf(buff);
        if ((rbytes = read(fd_from, buff, 256)) == -1) {
            perror("read 1");    close(fd_from);
            ; return(-1);
        }
    }
    close(fd_from);
    return 0;


}