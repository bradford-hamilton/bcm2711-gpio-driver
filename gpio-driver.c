#include <linux/module.h>
#include <linux/device.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>

static int gpio_sysfs_probe(struct platform_device *pdev);
static int gpio_sysfs_remove(struct platform_device *pdev);

struct driver_private_data {
	int total_devices;
	struct class *class_gpio;
	struct device **dev;
};

struct device_private_data {
	char label[20];
	struct gpio_desc *desc;
	struct mutex dev_lock;
};

ssize_t direction_show(struct device *dev, struct device_attribute *attr,
		       char *buf)
{
	struct device_private_data *dev_data = dev_get_drvdata(dev);
	int dir;
	char *direction;
	ssize_t written;

	mutex_lock(&dev_data->dev_lock);

	dir = gpiod_get_direction(dev_data->desc);
	if (dir < 0) {
		mutex_unlock(&dev_data->dev_lock);
		return dir;
	}
	direction = (dir == 0) ? "out" : "in";
	written = sprintf(buf, "%s\n", direction);

	mutex_unlock(&dev_data->dev_lock);

	return written;
}

ssize_t direction_store(struct device *dev, struct device_attribute *attr,
			const char *buf, size_t count)
{
	struct device_private_data *dev_data = dev_get_drvdata(dev);
	int ret;

	mutex_lock(&dev_data->dev_lock);

	if (sysfs_streq(buf, "in")) {
		ret = gpiod_direction_input(dev_data->desc);
	} else if (sysfs_streq(buf, "out")) {
		ret = gpiod_direction_output(dev_data->desc, 0);
	} else {
		ret = -EINVAL;
	}

	mutex_unlock(&dev_data->dev_lock);

	return ret ? ret : count;
}

ssize_t value_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct device_private_data *dev_data = dev_get_drvdata(dev);
	int value;
	ssize_t written;

	mutex_lock(&dev_data->dev_lock);

	value = gpiod_get_value(dev_data->desc);
	written = sprintf(buf, "%d\n", value);

	mutex_unlock(&dev_data->dev_lock);

	return written;
}

ssize_t value_store(struct device *dev, struct device_attribute *attr,
		    const char *buf, size_t count)
{
	struct device_private_data *dev_data = dev_get_drvdata(dev);
	int ret;
	long value;

	mutex_lock(&dev_data->dev_lock);

	ret = kstrtol(buf, 0, &value);
	if (ret) {
		mutex_unlock(&dev_data->dev_lock);
		return ret;
	}
	gpiod_set_value(dev_data->desc, value);

	mutex_unlock(&dev_data->dev_lock);

	return count;
}

ssize_t label_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct device_private_data *dev_data = dev_get_drvdata(dev);
	ssize_t written;

	mutex_lock(&dev_data->dev_lock);
	written = sprintf(buf, "%s\n", dev_data->label);
	mutex_unlock(&dev_data->dev_lock);

	return written;
}

static DEVICE_ATTR_RW(direction);
static DEVICE_ATTR_RW(value);
static DEVICE_ATTR_RO(label);

static struct attribute *gpio_attrs[] = {
	&dev_attr_direction.attr,
	&dev_attr_value.attr,
	&dev_attr_label.attr,
	NULL,
};

static struct attribute_group gpio_attr_group = {
	.attrs = gpio_attrs,
};

static const struct attribute_group *gpio_attr_groups[] = {
	&gpio_attr_group,
	NULL,
};

static struct of_device_id gpio_dev_match[] = {
	{.compatible = "bradford,bcm2711-gpio"},
	{},
};

static struct driver_private_data gpio_drv_priv_data;

struct platform_driver gpio_platform_driver = {
	.probe = gpio_sysfs_probe,
	.remove = gpio_sysfs_remove,
	.driver = {
		   .name = "bcm2711-gpio",
		   .of_match_table = of_match_ptr(gpio_dev_match),
		   },
};

static int gpio_sysfs_probe(struct platform_device *pdev)
{
	int ret;
	struct device *dev = &pdev->dev;
	struct device_node *parent = pdev->dev.of_node;
	struct device_node *child = NULL;
	struct device_private_data *dev_data;
	const char *name;
	int i = 0;

	dev_info(dev, "sysfs probe called\n");

	gpio_drv_priv_data.total_devices = of_get_child_count(parent);
	if (!gpio_drv_priv_data.total_devices) {
		dev_err(dev, "No devices found\n");
		return -EINVAL;
	}

	dev_info(dev, "Total devices == %d\n",
		 gpio_drv_priv_data.total_devices);

	gpio_drv_priv_data.dev =
	    devm_kzalloc(dev,
			 sizeof(struct device *) *
			 gpio_drv_priv_data.total_devices, GFP_KERNEL);

	for_each_available_child_of_node(parent, child) {
		dev_data = devm_kzalloc(dev, sizeof(*dev_data), GFP_KERNEL);
		if (!dev_data) {
			dev_err(dev, "Cannot allocate memory\n");
			return -ENOMEM;
		}

		mutex_init(&dev_data->dev_lock);

		if (of_property_read_string(child, "label", &name)) {
			dev_warn(dev, "Missing label information\n");
			snprintf(dev_data->label, sizeof(dev_data->label),
				 "unkngpio%d", i);
		} else {
			strcpy(dev_data->label, name);
			dev_info(dev, "GPIO label = %s\n", dev_data->label);
		}

		dev_data->desc =
		    devm_fwnode_get_gpiod_from_child(dev, "bcm2711",
						     &child->fwnode, GPIOD_ASIS,
						     dev_data->label);
		if (IS_ERR(dev_data->desc)) {
			ret = PTR_ERR(dev_data->desc);
			if (ret == -ENOENT) {
				dev_err(dev,
					"No gpio has been assigned to the requested function and/or index\n");
			}
			return ret;
		}

		gpio_drv_priv_data.dev[i] =
		    device_create_with_groups(gpio_drv_priv_data.class_gpio,
					      dev, 0, dev_data,
					      gpio_attr_groups,
					      dev_data->label);
		if (IS_ERR(gpio_drv_priv_data.dev[i])) {
			dev_err(dev, "Error during device_create\n");
			return PTR_ERR(gpio_drv_priv_data.dev[i]);
		}

		i++;
	}

	return 0;
}

static int gpio_sysfs_remove(struct platform_device *pdev)
{
	int i;
	for (i = 0; i < gpio_drv_priv_data.total_devices; i++) {
		device_unregister(gpio_drv_priv_data.dev[i]);
	}
	dev_info(&pdev->dev, "sysfs remove called\n");
	return 0;
}

static int __init gpio_sysfs_init(void)
{
	gpio_drv_priv_data.class_gpio =
	    class_create(THIS_MODULE, "bcm2711-gpios");
	if (IS_ERR(gpio_drv_priv_data.class_gpio)) {
		pr_err("Error creating class\n");
		return PTR_ERR(gpio_drv_priv_data.class_gpio);
	}

	platform_driver_register(&gpio_platform_driver);

	pr_info("Module successfully loaded\n");

	return 0;
}

static void __exit gpio_sysfs_exit(void)
{
	platform_driver_unregister(&gpio_platform_driver);
	class_destroy(gpio_drv_priv_data.class_gpio);
	pr_info("Module successfully unloaded\n");
}

module_init(gpio_sysfs_init);
module_exit(gpio_sysfs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("bradford-hamilton");
MODULE_DESCRIPTION("gpio driver for bcm2711");
MODULE_VERSION("0.1");
