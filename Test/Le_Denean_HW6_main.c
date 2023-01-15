/**************************************************************
* Class:  CSC-415-01 Fall 2022
* Name: Denean Le
* Student ID: 921330745
* GitHub ID: ledenean
* Project: Assignment 6 – Device Driver
*
* File: Le_Denean_HW6_main.c
*
* Description: This program is created to be tested by the driver.
**************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main (int argc, char * argv[]){
    int fd, option;
    char buffer[512];

    fd = open("/dev/Le_Denean_HW6_main", O_RDWR);
    printf("returned from open file, %d\n", fd);
    if(fd < 0){
        printf("Device Open Error\n");
        perror("Device File Open Error");
        return -1;
    }
    else{
        printf("Device Open Success\n");
    }

    printf("    1.Encrypt\n    2.Decrypt\nEnter number associated to option: ");
    scanf("%d", &option);

    printf("Enter string: ");
    scanf("%s", &buffer);
   

    write(fd, &buffer, strlen(buffer) + 1);
    ioctl(fd, &option);
    read(fd, &buffer, strlen(buffer));
    printf("Now the string is : %s\n", buffer);
    close(fd);
    return 0;
}