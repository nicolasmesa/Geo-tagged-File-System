#include <linux/syscalls.h>
#include <linux/gps.h>
#include <linux/uaccess.h>
#include <linux/spinlock.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/namei.h>

static struct gps_location_kern kernLocation;

SYSCALL_DEFINE1(set_gps_location, struct gps_location __user, *loc)
{
	static int first;
	struct timespec time = CURRENT_TIME;
	time_t seconds = time.tv_sec;

	if (first == 0) {
		first = 1;

		spin_lock_init(&(kernLocation.lock));
	}

	if (current_uid() != 0)
		return -EACCES;

	if (loc == NULL)
		return -EINVAL;

	spin_lock(&(kernLocation.lock));
	if (copy_from_user(&(kernLocation.location), loc,
		sizeof(kernLocation.location))) {
			spin_unlock(&(kernLocation.lock));
			return -EFAULT;
	}
	kernLocation.logtime = (unsigned int) seconds;

	printk("Seconds = %u\n", kernLocation.logtime);
	spin_unlock(&(kernLocation.lock));
	return 0;
}


SYSCALL_DEFINE2(get_gps_location, const char __user, *pathname,
	struct gps_location __user, *loc)
{
	struct path path;
	struct inode *inode;
	int res;
	struct gps_location k_loc;

	if (pathname == NULL)
		return -EINVAL;

	if (loc == NULL)
		return -EINVAL;

	res = user_path(pathname, &path);

	if (res)
		return -EINVAL;

	inode = path.dentry->d_inode;

	if (inode->i_op->get_gps_location != NULL)
		res= inode->i_op->get_gps_location(inode, &k_loc);
	else
		return -EINVAL;

	if (copy_to_user(loc, &k_loc, sizeof(*loc)))
		return -EFAULT;

	return res;
}

void getKernLocationValue(struct gps_location_kern *ptr)
{
	if (ptr == NULL)
		return;

	spin_lock(&(kernLocation.lock));
	ptr->location.latitude = kernLocation.location.latitude;
	ptr->location.longitude = kernLocation.location.longitude;
	ptr->location.accuracy = kernLocation.location.accuracy;
	ptr->logtime = kernLocation.logtime;
	spin_unlock(&(kernLocation.lock));
}
