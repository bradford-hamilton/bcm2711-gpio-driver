#include <linux/module.h>
#include <linux/fs.h>

#undef pr_fmt
#define pr_fmt(fmt) "%s : " fmt,__func__

static int gcd_open(struct inode *inod, struct file *filp);
static int gcd_release(struct inode *inod, struct file *filp);
static ssize_t gcd_read(struct file *filp, char __user *user_buf, size_t count, loff_t *fpos);
static ssize_t gcd_write(struct file *filp, const char __user *user_buf, size_t count, loff_t *fpos);

static const struct file_operations gcd_fops = {
  .open = gcd_open,
  .release = gcd_release,
  .read = gcd_read,
  .write = gcd_write,
  .owner = THIS_MODULE,
};

static int __init gcd_init(void)
{
  pr_info("char driver initialized\n");
  return 0;
}

static void __exit gcd_exit(void)
{
  pr_info("char driver removed\n");
  return;
}

static int gcd_open(struct inode *inod, struct file *filp)
{
  pr_info("gcd opened\n");
  return 0;
}

static int gcd_release(struct inode *inod, struct file *filp)
{
  pr_info("gcd released\n");
  return 0;
}

static ssize_t gcd_read(struct file *filp, char __user *user_buf, size_t count, loff_t *fpos)
{
  pr_info("gcd read\n");
  return 0;
}

static ssize_t gcd_write(struct file *filp, const char __user *user_buf, size_t count, loff_t *fpos)
{
  pr_info("gcd write\n");
  return 0;
}

module_init(gcd_init);
module_exit(gcd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("bradford-hamilton");
MODULE_DESCRIPTION("gpio char driver");
MODULE_VERSION("1.0");
