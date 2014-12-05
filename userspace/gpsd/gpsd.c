#include "gpsd.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

int get_line(int fd, char *line)
{
	char c;
	while(read(fd, &c, 1)) {
		if (c == '\n') {
			break;
		}

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

int main(int argc, char *argv[])
{
	struct gps_location loc;
	int ret;
	int fd;

	fd = open(GPS_LOCATION_FILE, O_RDONLY);

	if (fd < 0) {
		printf("Error: %s\n", strerror(errno));
		exit(1);
	}


	while (1) {
		populate_location(fd, &loc);
		ret = set_gps_location(&loc);

		if (ret < 0) {
			exit(1);
		}

		sleep(1);
		lseek(fd, 0, SEEK_SET);
	}

	//printf("Lat: %lf\n Lng: %lf\nAcc: %f\n\n", loc.latitude, loc.longitude, loc.accuracy);

	return 0;
}

