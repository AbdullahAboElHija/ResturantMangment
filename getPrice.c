#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#define MaxBuffer 256



int main(int argc, char* argv[]) {
	//function that print the price of the item
	char resName[50], temp[50], temp1[50], buff[2] = { 0 };
	int n1, n2, i, k, fd_from, rbytes = 1, n = 0, flag = 1;
	strcpy(temp1, " ");
	strcat(temp1, argv[2]);
	if (argv[3] != NULL) {
		strcat(temp1, " ");
		strcat(temp1, argv[3]);
	}

	strcpy(temp, "");

	strcpy(resName, argv[1]);
	strcat(resName, ".txt");
	if ((fd_from = open(resName, O_RDONLY)) == -1) { perror("no resturant like this."); return(-1); }
	k = strlen(temp1); // save the length of the item for search it
	//start searching the item
	while (rbytes > 0) {
		for (i = 0; i < k; i++) {
			if ((rbytes = read(fd_from, buff, 1)) == -1) { perror("read 1");    close(fd_from);; return(-1); }
			//if we don't find the item
			n = strcmp(buff, "\n");
			if (!n) {
				flag = 0;
				break;
			}
			strcat(temp, buff);
			n2 = strcmp(temp, "  ");
			if (!n2) { printf("no item like this.\n"); return 0; }
		}
		//if the line not empty
		if (flag) {
			//compare between the input and the output
			n = strcmp(temp, temp1);
			//if after the input we have '.'
			if ((rbytes = read(fd_from, buff, 1)) == -1) { perror("read 1");    close(fd_from);; return(-1); }
			n1 = strcmp(".", buff);
			if ((!n) && (!n1)) {
				if ((rbytes = read(fd_from, buff, 1)) == -1) { perror("read 1");    close(fd_from);; return(-1); }
				n = strcmp(".", buff);
				while (!n) {
					if ((rbytes = read(fd_from, buff, 1)) == -1) { perror("read 1");    close(fd_from);; return(-1); }
					n = strcmp(".", buff);
				}
				//printing the price
				while (!(buff[0] < '0' || buff[0]>'9')) {
					printf("%s", buff);
					if ((rbytes = read(fd_from, buff, 1)) == -1) { perror("read 1");    close(fd_from);; return(-1); }

				}
				printf(" NIS\n");
				//end printing the price
				return 0;


			}
		}
		//if not empty and not equal to the item we continue to the end of the line
		if (flag) {
			while (1) {
				if ((rbytes = read(fd_from, buff, 1)) == -1) { perror("read 1");    close(fd_from);; return(-1); }
				n = strcmp(buff, "\n");
				if (!n) {
					break;
				}
			}
		}
		flag = 1;
		strcpy(temp, "");

	}

	return 0;

}
