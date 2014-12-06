#ifndef GPS_LOCATION
#define GPS_LOCATION

#include <linux/time.h>

struct gps_location {
	double latitude;
	double longitude;
	float  accuracy;  /* in meters */
};


/*
*This is the structure we use to keep
*the latest GPS location in the kernel.
*No FLOP done here as we are just copying
*the variables around.
*/

struct gps_location_kern {
		
	double latitude;
	double longitude;
	float accuracy;
	struct timespec logtime;	
};


struct gps_location_kern *getKernLocationValue (struct gps_location_kern *);


#endif
