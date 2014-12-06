#include <linux/syscalls.h>
#include <linux/gps.h>
#include <linux/uaccess.h>
#include <linux/spinlock.h>


static struct gps_location_kern kernLocation;
static DEFINE_SPINLOCK(lock);

SYSCALL_DEFINE1(set_gps_location, struct gps_location __user, *loc)
{

	struct gps_location tempLocation;
	
	if (current_uid() != 0)
		return -EACCES;

	if (loc == NULL)
		return -EINVAL;

	/*
	*Didn't want to copy while holding a lock
	*Just a little uncomfortable doing it, that's all
	*/
	
	if (copy_from_user(&tempLocation, loc, sizeof(tempLocation))) 
			return -EFAULT;
	
	
	spin_lock(&lock);
	
	kernLocation.latitude = tempLocation.latitude;
	kernLocation.longitude = tempLocation.longitude;
	kernLocation.accuracy = tempLocation.accuracy;
	kernLocation.logtime = CURRENT_TIME;

	spin_unlock(&lock);

	printk("Successful\n");

	return 0;
}

struct gps_location_kern *getKernLocationValue (struct gps_location_kern *ptr)
{

	if (ptr == NULL)
		return NULL;

	spin_lock (&lock);
	
	ptr->latitude = kernLocation.latitude;
	ptr->longitude = kernLocation.longitude;
	ptr->accuracy = kernLocation.accuracy;
	ptr->logtime = kernLocation.logtime;

	spin_unlock(&lock);
	
	return ptr;

}
