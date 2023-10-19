#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <fcntl.h>
#include<time.h>
#include <unistd.h>


int FindThePrice(char* file, char* temp, int quantity) {
	//function that sum the price and return it
	char buff[1] = { 0 }, price[6] = { 0 }, pathFile[30] = { 0 };
	int fd_file, rbytes, count = 0, p;
	temp[strlen(temp) - 1] = '\0';
	strcpy(pathFile, file);
	strcat(pathFile, ".txt");
	if ((fd_file = open(pathFile, O_RDONLY)) == -1) {
		printf("Not Found!");
		exit(1);
	}
	else {
		while (rbytes = (read(fd_file, buff, 1))) {
			if (rbytes == -1) { perror("error while reading"); exit(1); }
			if (buff[0] == '\n')
				count = 0;
			if (buff[0] == temp[count])
				count++;
			if (count == strlen(temp)) {
				if (read(fd_file, buff, 1) == -1) { perror("error while reading"); exit(1); }
				while (buff[0] == '.' || buff[0] == ' ')
					if (read(fd_file, buff, 1) == -1) { perror("error while reading"); exit(1); }
				p = 0;
				while ((buff[0] >= '0' && buff[0] <= '9')) {
					price[p] = buff[0];
					if (read(fd_file, buff, 1) == -1) { perror("error while reading"); exit(1); }
					p++;
				}
				return atoi(price) * quantity;
			}
		}
		if (price[0] == '\0') {
			printf("No Dish like This in the resturant!\n");
			return 0;
		}
	}
	return 0;
}


int main(int argc, char* argv[]) {
	char buff[256] = { 0 }, order[20] = { 0 }, pathFile[256] = { 0 }, ordername[30] = { 0 }, * pointer;
	int fd_file, fo_file, bytes, wbytes, flag = 0, receipt = 0, LengthWeHave, priceSum = 0;
	time_t rawtime;
	struct tm* TimeMaking;
	if (argc != 3) {
		printf("Failed num of arguments\n");
		exit(1);
	}
	//trying to open file by name the resturant to check if we have resturant like this
	strcpy(pathFile, argv[1]);
	strcat(pathFile, ".txt");
	if ((fo_file = open(pathFile, O_RDONLY)) == -1) {
		printf("Restaurant Not Found!\n");
		close(fd_file);
		exit(1);
	}
	else {
		strcpy(pathFile, "/home/braude/"); // /home/braude/
		strcat(pathFile, argv[1]);  //  /home/braude/BBB
		strcat(pathFile, "_Order/"); //  /home/braude/BBB_Order
		strcat(pathFile, argv[2]); //  /home/braude/BBB_Order/Name
		strcat(pathFile, ".txt"); //home/braude/BBB_Order/Name.txt
		if ((fd_file = open(pathFile, O_WRONLY | O_CREAT, 0664)) == -1) {// open it for write (and putting it in the dir).
			perror("Error in open");
			exit(1);
		}
		close(fo_file);
		strcpy(ordername, argv[1]);
		bytes = strlen(ordername);
		wbytes = write(fd_file, ordername, bytes);// BBB Order...
		if (wbytes == -1) { perror("Writeing to File Error!"); exit(1); }
		wbytes = write(fd_file, " Order\n", 7);
		if (wbytes == -1) { perror("Writeing to File Error!"); exit(1); }
		printf("Insert your order(Finish to finish) :\n");
		while (flag != 1) {
			fgets(buff, 20, stdin);
			buff[strlen(buff) - 1] = '\0';
			//If we write Finish we will make the order
			if (!(strcmp(buff, "Finish"))) { flag = 1; }
			else {
				if ((wbytes = write(fd_file, "\n", 1)) == -1) { perror("Writeing to File Error!"); exit(1); }
				if ((wbytes = write(fd_file, buff, strlen(buff))) == -1) { perror("Writeing to File Error!"); exit(1); }
				//start spliting
				pointer = strtok(buff, " ");
				while (pointer != NULL) {
					if (atoi(pointer) != 0) { priceSum = priceSum + (FindThePrice(argv[1], order, atoi(pointer))); }
					strcat(order, pointer);
					strcat(order, " ");
					pointer = strtok(NULL, " ");
				}
				order[0] = '\0';// after each while we return the order null for the next check.
			}
			//ending spliting

		}
		time(&rawtime);
		TimeMaking = localtime(&rawtime);
		//printing the price
		if ((wbytes = write(fd_file, "\nTotal Price:", 13)) == -1) { perror("Writeing to File Error!"); exit(1); }
		LengthWeHave = sprintf(buff, "%d", priceSum);
		if ((wbytes = write(fd_file, buff, strlen(buff))) == -1) { perror("Writeing to File Error!"); exit(1); }
		strftime(buff, 256, "\n\n%x", TimeMaking);
		if ((wbytes = write(fd_file, buff, strlen(buff))) == -1) { perror("Writeing to File Error!"); exit(1); }
		printf("Total Price: %d NIS (Confirm to approve/else cancel)\n", priceSum);
		fgets(buff, 20, stdin);
		buff[strlen(buff) - 1] = '\0';
		while (flag == 1) {
			// If cancel remove the File and stop
			if (!(strcmp(buff, "cancel"))) { close(fd_file); remove(pathFile); flag = 0; }
			//if confirm create the file and exit
			else if (!(strcmp(buff, "Confirm"))) { close(fd_file); printf("Order created!\n"); flag = 0; }
			else {
				printf("Please Confirm or cancel.\n");
				fgets(buff, 20, stdin);
				buff[strlen(buff) - 1] = '\0';
			}
		}
	}

	return 0;
}
