#include "gpsd.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>


int get_line(int fd, char *line)
{
	char c;

	while (read(fd, &c, 1)) {
		if (c == '\n')
			break;
		*(line++) = c;
	}
	*line = '\0';

	return 0;
}

void populate_location(int fd, struct gps_location *loc)
{
	char line[100];

	get_line(fd, line);
	loc->latitude = strtod(line, NULL);

	get_line(fd, line);
	loc->longitude = strtod(line, NULL);

	get_line(fd, line);
	loc->accuracy = (float) strtod(line, NULL);
}

/*
* Followed the process to daemonize
* a function from the solutions
* of one of the earlier homeworks.
* Homework 3 - accelerationd.c to be
* specific
*/

void daemonize(void)
{

	pid_t pid;

	pid = fork();

	if (pid < 0) {
		perror("Fail fork");
		exit(EXIT_FAILURE);
	}

	if (pid > 0)
		exit(EXIT_SUCCESS);
	if (setsid() < 0) {
		perror("Fail to setsid");
		exit(EXIT_FAILURE);
	}

	pid = fork();

	if (pid < 0) {
		perror("Fail fork");
		exit(EXIT_FAILURE);
	}

	if (pid > 0)
		exit(EXIT_SUCCESS);

	close(0);
	close(1);
	close(2);
	chdir("/data/misc/");
	umask(0);
}


int main(int argc, char *argv[])
{
	struct gps_location loc;
	int ret;
	int fd;

	daemonize();

	fd = open(GPS_LOCATION_FILE, O_RDONLY);

	if (fd < 0) {
		printf("Error: %s\n", strerror(errno));
		exit(1);
	}

	while (1) {
		populate_location(fd, &loc);
		ret = set_gps_location(&loc);

		if (ret < 0)
			exit(1);

		sleep(1);
		lseek(fd, 0, SEEK_SET);
	}

	/*printf("Lat: %lf\n Lng: %lf\nAcc: %f\n\n", loc.latitude,
		loc.longitude, loc.accuracy);*/

	return 0;
}

