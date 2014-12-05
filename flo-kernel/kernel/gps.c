#include <linux/syscalls.h>
#include <linux/gps.h>
#include <linux/uaccess.h>
#include <linux/spinlock.h>


static struct gps_location location;
static DEFINE_SPINLOCK(lock);

SYSCALL_DEFINE1(set_gps_location, struct gps_location __user, *loc)
{

	if (current_uid() != 0)
		return -EACCES;

	if (loc == NULL)
		return -EINVAL;

	spin_lock(&lock);
	if (copy_from_user(&location, loc, sizeof(location)))
		return -EFAULT;
	spin_unlock(&lock);

	printk("Successful\n");

	return 0;
}
