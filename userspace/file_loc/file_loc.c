#include "file_loc.h"

int main(int argc, char **argv)
{

	struct gps_location loc;
	int ret;

	if (argc < 2) {

		printf("Usage: %s <path to file>\n", argv[0]);
		exit(1);
	}

	ret = get_gps_location(argv[1], &loc);

	if (ret < 0) {

		printf("Error: %s\n", strerror(errno));
		printf("You probably entered a path in a different FS\n");
		exit(1);
	}

	printf("Latitude: %lf\tLongitude: %lf\tAccuracy: %f\n", loc.latitude,
		loc.longitude, loc.accuracy);
	printf("Age: %d\n", ret);

	printf("URL: https://www.google.com/maps?&z=10&q=%f+-%f&ll=%f+%f\n",
		loc.latitude, loc.longitude, loc.latitude, loc.longitude);

	return 0;
}
