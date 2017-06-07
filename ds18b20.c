// sudo nano /boot/config.txt to add
// dtoverlay=w1-gpio

#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main (void) {
	DIR *dir;
	FILE *fp;
	struct dirent *dirent;
	char email[]="wpballa.wine@gmail.com";
//	char phone[]="9257846015";
	char dev[16];      // Dev ID
	char devPath[128]; // Path to device
	char buf[256];     // Data from device
	char tmpData[6];   // Temp C * 1000 reported by device
	char path[] = "/sys/bus/w1/devices";
	ssize_t numRead;
	float alarm=60.5;
	float fail=63.0;
	float tempC=0.0;
	float tempF=0.0;
	int ix;
	char out_string[200];
	char *sys_string;
	time_t current_time;
	current_time=time(NULL);
	out_string[0]='\0';

	dir = opendir (path);
	if (dir != NULL)
	{
		while ((dirent = readdir (dir)))

// 1-wire devices are links beginning with 28-

		if (dirent->d_type == DT_LNK &&
		strstr(dirent->d_name, "28-") != NULL) {
			strcpy(dev, dirent->d_name);
//			printf("\nDevice: %s\n", dev);
		}
		(void) closedir (dir);
	}
	else
	{
		perror ("Couldn't open the w1 devices directory");
		return 1;
	}

// Assemble path to OneWire device

	sprintf(devPath, "%s/%s/w1_slave", path, dev);

// Read temp
// Opening the device's file triggers new reading

	int fd = open(devPath, O_RDONLY);
	if(fd == -1)
	{
		perror ("Couldn't open the w1 device.");
		return 1;
	}
	while((numRead = read(fd, buf, 256)) > 0)
	{
		strncpy(tmpData, strstr(buf, "t=") + 2, 5);
		tempC = strtof(tmpData, NULL);
		tempC = tempC/1000;
		tempF = tempC*9./5.+32.;

// debug lines
//		printf("Device: %s  - ", dev);
//		printf("Temp: %.3f C  ", tempC);
//		printf("%.3f F\n\n\n", tempF);

	}
	close(fd);

// Now, write to log file

	fp = fopen("/home/pi/wineroom/wine.log", "a");
	fprintf(fp,"%5.1f\t%s",tempF,ctime(&current_time));
	fclose(fp);

//  send e-mail if over temperature, and text as well. Notice silly game in
//  text to get double slash not to be a comment. Be sure to use your e-mail
//  and phone number!

 	if(tempF > fail) {
		sprintf(out_string, "echo \'wine room fail %5.1f F\' | "
		"mail -s \'wine room fail %5.1f F\' %s",
		tempF, tempF, email);
		sys_string = out_string;
   		ix = system(sys_string);
	} else if (tempF > alarm) {
		sprintf(out_string, "echo \'wine room alarm %5.1f F\' | "
		"mail -s \'wine room alarm %5.1f F\' %s",
		tempF, tempF, email);
		sys_string = out_string;
		ix = system(sys_string);
	}
        return 1;
}

// Compiling & Running

// Assuming the code is saved in a file called ds18b20.c,
// run the following to compile -

//gcc -Wall -std=gnu99 ds18b20.c -o ds18b20

//Run the code by typing ./ds18b20.
