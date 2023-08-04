#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>

static int gpio_sysfs_probe(struct platform_device *pdev);
static int gpio_sysfs_remove(struct platform_device *pdev);

static struct of_device_id gpio_dev_match[] = {
  { .compatible = "bradford,bcm2711-gpio-driver" },
  {},
};

struct driver_private_data {
  int total_devices;
  struct class *class_gpio;
  struct device **dev;
};

static struct driver_private_data gpio_drv_priv_data;

struct platform_driver gpio_sysfs_platform_driver = {
  .probe = gpio_sysfs_probe,
  .remove = gpio_sysfs_remove,
  .driver = {
    .name = "bcm2711-gpio-driver",
    .of_match_table = of_match_ptr(gpio_dev_match),
  },
};

static int gpio_sysfs_probe(struct platform_device *pdev)
{
  struct device *dev = &pdev->dev;
  dev_info(dev, "sysfs probe called\n");

  return 0;
}

static int gpio_sysfs_remove(struct platform_device *pdev)
{
  struct device *dev = &pdev->dev;
  dev_info(dev, "sysfs remove called\n");

  return 0;
}

static int __init gpio_sysfs_init(void)
{
  gpio_drv_priv_data.class_gpio = class_create(THIS_MODULE, "bcm2711-gpios");
  if (IS_ERR(gpio_drv_priv_data.class_gpio)) {
    pr_err("Error creating class\n");
    return PTR_ERR(gpio_drv_priv_data.class_gpio);
  }

  platform_driver_register(&gpio_sysfs_platform_driver);

  pr_info("Module successfully loaded\n");
  return 0;
}

static void __exit gpio_sysfs_exit(void)
{
  platform_driver_unregister(&gpio_sysfs_platform_driver);
  class_destroy(gpio_drv_priv_data.class_gpio);
  pr_info("Module successfully unloaded\n");
}

module_init(gpio_sysfs_init);
module_exit(gpio_sysfs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("bradford-hamilton");
MODULE_DESCRIPTION("practice gpio driver for bcm2711");
MODULE_VERSION("0.1");
