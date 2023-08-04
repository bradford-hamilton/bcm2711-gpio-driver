#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>

static int gpio_sysfs_probe(struct platform_device* pdev)
{
  return 0;
}

static int gpio_sysfs_remove(struct platform_device* pdev)
{
  return 0;
}

static struct of_device_id gpio_dev_match[] = {
  { .compatible = "bradford,bcm2711-gpio-driver" },
  {},
};

const static struct platform_driver gpio_sysfs_platform_driver {
  .probe = gpio_sysfs_probe,
  .remove = gpio_sysfs_remove,
  .driver = {
    .name = "bcm2711-gpio-driver",
    .of_match_table = of_match_ptr(gpio_dev_match),
  },
};

static int __init gpio_sysfs_init(void)
{
  return 0;
}

static void __exit gpio_sysfs_exit(void)
{

}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("bradford-hamilton");
MODULE_DESCRIPTION("practice gpio driver for bcm2711");
MODULE_VERSION("0.1");
