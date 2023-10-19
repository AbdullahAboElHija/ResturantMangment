#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SIZE 256


int scannerForWrittenFunctions(char* input) {
	if (!strcmp(input, "CreateMenu"))
		return 1;
	else if (!strcmp(input, "getPrice"))
		return 1;
	else if (!strcmp(input, "getMenu"))
		return 1;
	else if (!strcmp(input, "MakeOrder"))
		return 1;
	else if (!strcmp(input, "getOrderNum"))
		return 1;
	return 0; //not advanced shell function
}




int main() {
	int i, pid;
	char temp[SIZE], path[SIZE], * argv[4], * ifArgv4;
	//loop until the user enter exit
	while (1) {
		for (i = 0; i < 4; i++)
			argv[i] = NULL;
		printf("AdvShell>");
		fflush(stdin);
		rewind(stdin);
		fgets(temp, SIZE, stdin);
		temp[strlen(temp) - 1] = '\0';
		if (!strcmp(temp, "exit")) {
			//if exit say good bye :D and exit
			printf("Good Bye :D\n");
			return 0;
		}
		//starting split the input
		argv[0] = strtok(temp, " ");
		for (i = 1; i < 4; i++)
			argv[i] = strtok(NULL, " ");
		//ending split the input 
		// if we have parameter 4
		ifArgv4 = strtok(NULL, " ");

		if ((pid = fork()) == -1) {
			perror("Error! fork failed!\n");
			exit(1);
		}
		else if (pid == 0) { //if fork happened
			if (ifArgv4 != NULL) { //not enough arguments
				printf("Wrong number of arguments!\n");
				exit(1);
			}
			// if the input is for function
			else if (scannerForWrittenFunctions(argv[0]))
				strcpy(path, "/home/braude/");
			else
				strcpy(path, "/bin/");
			strcat(path, argv[0]);
			execl(path, argv[0], argv[1], argv[2], argv[3], NULL); //execute function
			printf("Not Supported\n"); //not 
			exit(1);
		}
		wait(); //wait for child to end
	}
}