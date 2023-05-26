/**
 * This C program writes the command line arguments passed into it into /dev/p78_gpio
 * Input: 0 or 1, for GPIO on or off, respectively
 * 
 * Author: p7810456
*/

#include <stdio.h>

FILE* led_dev_file;

int main(int argc, char **argv) {
	if(argc < 2 || argc > 2) {
		printf("Invalid number of arguments\n");
		return -1;
	}

	if((led_dev_file = fopen("/dev/p78_gpio", "wb")) == NULL) {
		printf("Couldn't open file\n");
		return -1;
	}

	int data = (argv[1])[0];
	data -= 48; // Convert ASCII to dec

	fputc(data, led_dev_file);
	fputc('\0', led_dev_file); // Add NULL terminator
	
	return 0;
}
