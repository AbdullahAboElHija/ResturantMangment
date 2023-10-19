#include<stdio.h>
#include<dirent.h>
#include <string.h>

int main(int argc, char* argv[])
{
    //function that count the num of the order by count num the files in folder the resturant
    int cnt = -2;
    DIR* d;
    char path[100], order[] = "_Order";
    struct dirent* dir;
    //creating the path of the resturant
    strcpy(path, argv[1]);
    strcat(path, order);
    //ending creating the path of the resturant
    d = opendir(path);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            cnt++; // count files in the dictionary
        }
        closedir(d);
    }
    printf("num orders in resturant %s is : %d\n", argv[1], cnt);
    return(0);
}