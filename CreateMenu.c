#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>



#define MAX_NAME_LENGTH 256

typedef struct dish {
	char dishName[MAX_NAME_LENGTH];
	char price[10];
}dish;

typedef struct typeDish {
	char  dishTypeName[MAX_NAME_LENGTH];
	dish dishes[100];
	int len;
} typeDish;

int isNumber(char* s);
void CreateMenu(char* resName, int numKinds);

int main(int argc, char* argv[]) {
	int n;
	if (argc != 3) {
		printf("The number of arguments are not vaild!\n");
		exit(1);
	}
	n = atoi(argv[2]);
	CreateMenu(argv[1], n);
	return 0;

}
void CreateMenu(char* resName, int numKinds) {
	char charKind = 'a';
	int fd, wbytes, fd_dir;
	int i, j, k, n, n1 = 0, len, longestName = 0, stop = 1, flagfirsttimetocopy = 1, flagfirsttimetocopys = 1;
	char txt[] = ".txt", temp[100], dishtype[100] = " ";
	char menu[] = " Menu\n", * ptr;
	char charNum[2];
	char snum[5];
	char bon[] = "Bon Appetit";
	typeDish* types = (typeDish*)malloc(numKinds * sizeof(typeDish));
	strcpy(temp, resName);
	len = strlen(temp);
	strcat(temp, txt);
	if ((fd = open(temp, O_WRONLY | O_CREAT, 0664)) == -1) { perror("open"); exit(EXIT_FAILURE); } // creating new file for the menu

	// creating new file txt for the menu.
	// writing the name of the resturant.
	k = 1;
	j = 0;
	for (i = 0; i < numKinds; i++) {
		//inset dish type
		charNum[0] = charKind;
		printf("Insert type dish %c:\n", charKind);
		charKind = charKind + 1;
		fgets(temp, 100, stdin);
		temp[strlen(temp) - 1] = '\0';
		strcpy(types[i].dishTypeName, temp);


		// end of insert dish type
		printf("type name : %s\n", types[i].dishTypeName);
		//start insert dishes

		j = 0;
		k = 1;
		while (1) {
			printf("inset dish name %d :\n", j + 1);
			fgets(temp, 100, stdin);
			if ((n = strcmp(temp, "stop\n")) == 0) { break; }
			temp[strlen(temp) - 1] = '\0';
			ptr = strtok(temp, " ");
			while (ptr != NULL) {
				if (isNumber(ptr)) { strcpy(types[i].dishes[j].price, ptr); break; }
				else if (flagfirsttimetocopys) { strcpy(types[i].dishes[j].dishName, temp); flagfirsttimetocopys = 0; }
				else { strcat(types[i].dishes[j].dishName, " "); strcat(types[i].dishes[j].dishName, ptr); }
				ptr = strtok(NULL, " ");
			}
			if (strlen(types[i].dishes[j].dishName) > longestName) { longestName = strlen(types[i].dishes[j].dishName); }
			j++;
			flagfirsttimetocopys = 1;
		}
		types[i].len = j;
	}
	//start printing resturant name
	if ((wbytes = write(fd, resName, len)) == -1) { close(fd); perror("open"); exit(EXIT_FAILURE); }// writing the name of the res
	if ((wbytes = write(fd, menu, strlen(menu))) == -1) { close(fd); perror("open"); exit(EXIT_FAILURE); }
	if ((wbytes = write(fd, "\n", 1)) == -1) { close(fd); perror("WRITE"); exit(EXIT_FAILURE); } //\n after res menu.
//end printing resturant name
	charKind = 'a';
	charNum[0] = charKind;
	for (i = 0; i < numKinds; i++) {
		charNum[0] = charKind;
		charNum[1] = '\0';
		//printing the type name
		if ((wbytes = write(fd, charNum, 1)) == -1) { close(fd); perror("WRITE"); exit(EXIT_FAILURE); }
		if ((wbytes = write(fd, ". ", 2)) == -1) { close(fd); perror("WRITE"); exit(EXIT_FAILURE); }
		if ((wbytes = write(fd, types[i].dishTypeName, strlen(types[i].dishTypeName))) == -1) { close(fd); perror("WRITE"); exit(EXIT_FAILURE); } //writing char.type ending here
		if ((wbytes = write(fd, "\n", 1)) == -1) { close(fd); perror("WRITE"); exit(EXIT_FAILURE); }
		//ending print the type name
		n1 = types[i].len;
		for (j = 0; j < n1; j++) {
			if ((wbytes = write(fd, " ", 1)) == -1) { close(fd); perror("WRITE"); exit(EXIT_FAILURE); }
			if ((wbytes = write(fd, types[i].dishes[j].dishName, strlen(types[i].dishes[j].dishName))) == -1) { close(fd); perror("WRITE"); exit(EXIT_FAILURE); } //writing char.type ending here
			n = strlen(types[i].dishes[j].dishName);
			while (n < longestName + 5) {
				if ((wbytes = write(fd, ".", 1)) == -1) { close(fd); perror("WRITE"); exit(EXIT_FAILURE); }
				n++;
			}
			if ((wbytes = write(fd, types[i].dishes[j].price, strlen(types[i].dishes[j].price))) == -1) { close(fd); perror("WRITE"); exit(EXIT_FAILURE); } //writing char.type ending here
			if ((wbytes = write(fd, "NIS", 3)) == -1) { close(fd); perror("WRITE"); exit(EXIT_FAILURE); }
			if ((wbytes = write(fd, "\n", 1)) == -1) { close(fd); perror("WRITE"); exit(EXIT_FAILURE); }

		}
		charKind++;
	}
	//printing bon appite
	if ((wbytes = write(fd, "\n", 1)) == -1) { close(fd); perror("WRITE"); exit(EXIT_FAILURE); }
	if ((wbytes = write(fd, "     ", 5)) == -1) { close(fd); perror("WRITE"); exit(EXIT_FAILURE); }
	if ((wbytes = write(fd, bon, strlen(bon))) == -1) { close(fd); perror("WRITE"); exit(EXIT_FAILURE); }

	// ending printing bon appite
	strcpy(temp, resName);
	strcat(temp, "_Order");
	if ((fd_dir = mkdir(temp, 0777) == -1)) {
		//creating dictionary 
		perror("open dir");
		exit(1);
	}
	close(fd);
	free(types);
	printf("Successfully created\n");

}


int isNumber(char* s) {
	//function that checks if the index input is number.
	while (*s != '\0')
	{
		if (*s > '9' || *s < '0')
			return 0;
		s++;
	}
	return 1;
}