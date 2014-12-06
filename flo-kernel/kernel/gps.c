#include <linux/syscalls.h>
#include <linux/gps.h>
#include <linux/uaccess.h>
#include <linux/spinlock.h>


static struct gps_location_kern kernLocation;
 

SYSCALL_DEFINE1(set_gps_location, struct gps_location __user, *loc)
{
	static int first;

	if (first == 0) {
		first = 1;

		spin_lock_init(&(kernLocation.lock));
	}

	if (current_uid() != 0)
		return -EACCES;

	if (loc == NULL)
		return -EINVAL;

	spin_lock(&(kernLocation.lock));
	if (copy_from_user(&(kernLocation.location), loc, sizeof(kernLocation.location))) {
			spin_unlock(&(kernLocation.lock));
			return -EFAULT;
	}
	
	kernLocation.logtime = CURRENT_TIME;

	spin_unlock(&(kernLocation.lock));

	return 0;
}


void getKernLocationValue (struct gps_location_kern *ptr)
{

	if (ptr == NULL)
		return;

	spin_lock(&(kernLocation.lock));
	ptr->location.latitude = kernLocation.location.latitude;
	ptr->location.longitude = kernLocation.location.longitude;
	ptr->location.accuracy = kernLocation.location.accuracy;
	ptr->logtime = kernLocation.logtime;

	spin_unlock(&(kernLocation.lock));
	
	return;

}
