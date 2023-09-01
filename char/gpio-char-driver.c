#include <linux/module.h>

#undef pr_fmt
#define pr_fmt(fmt) "%s : " fmt,__func__

static int __init gpio_char_drv_init(void)
{
  pr_info("char driver initialized\n");
  return 0;
}

static void __exit gpio_char_drv_exit(void)
{
  pr_info("char driver removed\n");
  return;
}

module_init(gpio_char_drv_init);
module_exit(gpio_char_drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("bradford-hamilton");
MODULE_DESCRIPTION("gpio char driver");
MODULE_VERSION("1.0");
