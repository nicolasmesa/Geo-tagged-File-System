#include "gpsd.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
	struct gps_location loc;
	int ret;

	if (argc < 2) {
		printf("Usage: %s <path name>\n", argv[0]);
		exit(1);
	}

	ret = get_gps_location(argv[1], &loc);

	printf("Lat: %lf\n:Lng: %lf\nAcc: %f\n\n", loc.latitude, loc.longitude, loc.accuracy);		

	printf("Ret: %d\n", ret);

	return 0;
}

