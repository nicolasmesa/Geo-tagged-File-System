#ifndef GPS_LOCATION
#define GPS_LOCATION

struct gps_location {
	double latitude;
	double longitude;
	float  accuracy;  /* in meters */
};
#endif
