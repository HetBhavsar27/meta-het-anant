// SPDX-License-Identifier: GPL-2.0-only

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/uaccess.h>

#define DRIVER_NAME "het_char"
#define BUFFER_SIZE 256

struct het_char_device {
	char buffer[BUFFER_SIZE];
	size_t data_size;
	struct mutex lock;
	atomic_t open_count;
	atomic64_t read_count;
	atomic64_t write_count;
};

static struct het_char_device het_device;

static int het_char_open(struct inode *inode, struct file *file)
{
	int count;
	int ret;

	ret = nonseekable_open(inode, file);
	if (ret)
		return ret;

	count = atomic_inc_return(&het_device.open_count);

	pr_info("%s: device opened, active opens=%d\n",
		DRIVER_NAME, count);

	return 0;
}

static int het_char_release(struct inode *inode, struct file *file)
{
	int count;

	count = atomic_dec_return(&het_device.open_count);

	pr_info("%s: device closed, active opens=%d\n",
		DRIVER_NAME, count);

	return 0;
}

static ssize_t het_char_read(struct file *file,
			     char __user *user_buffer,
			     size_t count,
			     loff_t *offset)
{
	ssize_t bytes_to_copy;
	ssize_t result;

	if (mutex_lock_interruptible(&het_device.lock))
		return -ERESTARTSYS;

	if (*offset >= het_device.data_size) {
		result = 0;
		goto out_unlock;
	}

	bytes_to_copy = min_t(size_t,
			      count,
			      het_device.data_size - *offset);

	if (copy_to_user(user_buffer,
			 het_device.buffer + *offset,
			 bytes_to_copy)) {
		result = -EFAULT;
		goto out_unlock;
	}

	*offset += bytes_to_copy;
	atomic64_add(bytes_to_copy, &het_device.read_count);
	result = bytes_to_copy;

out_unlock:
	mutex_unlock(&het_device.lock);
	return result;
}

static ssize_t het_char_write(struct file *file,
			      const char __user *user_buffer,
			      size_t count,
			      loff_t *offset)
{
	ssize_t result;

	/*
	 * One write() call represents one complete message.
	 * Reserve one byte for the terminating NUL character.
	 */
	if (count == 0)
		return 0;

	if (count > BUFFER_SIZE - 1) {
		pr_warn("%s: rejected oversized message: requested=%zu maximum=%d\n",
			DRIVER_NAME, count, BUFFER_SIZE - 1);
		return -EMSGSIZE;
	}

	if (mutex_lock_interruptible(&het_device.lock))
		return -ERESTARTSYS;

	/*
	 * Replace the previous message only after validating the new message.
	 * A rejected oversized message therefore leaves the old message intact.
	 */
	memset(het_device.buffer, 0, sizeof(het_device.buffer));

	if (copy_from_user(het_device.buffer, user_buffer, count)) {
		result = -EFAULT;
		goto out_unlock;
	}

	het_device.buffer[count] = '\0';
	het_device.data_size = count;

	atomic64_add(count, &het_device.write_count);
	result = count;

	pr_info("%s: stored complete message: %zu bytes\n",
		DRIVER_NAME, count);

out_unlock:
	mutex_unlock(&het_device.lock);
	return result;
}

static const struct file_operations het_char_fops = {
	.owner = THIS_MODULE,
	.open = het_char_open,
	.release = het_char_release,
	.read = het_char_read,
	.write = het_char_write,
};

static struct miscdevice het_char_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DRIVER_NAME,
	.fops = &het_char_fops,
	.mode = 0660,
};

static int __init het_char_init(void)
{
	int ret;

	memset(&het_device, 0, sizeof(het_device));
	mutex_init(&het_device.lock);
	atomic_set(&het_device.open_count, 0);
	atomic64_set(&het_device.read_count, 0);
	atomic64_set(&het_device.write_count, 0);

	ret = misc_register(&het_char_misc_device);
	if (ret) {
		pr_err("%s: misc_register failed: %d\n",
		       DRIVER_NAME, ret);
		return ret;
	}

	pr_info("%s: registered /dev/%s with minor=%d\n",
		DRIVER_NAME,
		het_char_misc_device.name,
		het_char_misc_device.minor);

	return 0;
}

static void __exit het_char_exit(void)
{
	misc_deregister(&het_char_misc_device);

	pr_info("%s: unloaded, reads=%lld bytes, writes=%lld bytes\n",
		DRIVER_NAME,
		(long long)atomic64_read(&het_device.read_count),
		(long long)atomic64_read(&het_device.write_count));
}

module_init(het_char_init);
module_exit(het_char_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Het Bhavsar");
MODULE_DESCRIPTION("Educational miscellaneous character driver");
MODULE_VERSION("1.0");
