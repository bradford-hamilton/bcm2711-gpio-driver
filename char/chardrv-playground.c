#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/gpio/consumer.h>
#include <linux/delay.h>

#undef pr_fmt
#define pr_fmt(fmt) "%s : " fmt,__func__

#define DEVICE_COUNT 1

static void write_4_bits(uint8_t data);
void lcd_enable(void);
void lcd_init(void);
void lcd_send_command(uint8_t cmd);
void lcd_print_char(uint8_t data);
void lcd_print_string(char *message);
void lcd_display_clear(void);
void lcd_display_return_home(void);

static int lcd_open(struct inode *inod, struct file *filp);
static int lcd_release(struct inode *inod, struct file *filp);
static ssize_t lcd_read(struct file *filp, char __user *user_buf, size_t count, loff_t *fpos);
static ssize_t lcd_write(struct file *filp, const char __user *user_buf, size_t count, loff_t *fpos);

static const struct file_operations lcd_fops = {
  .open = lcd_open,
  .release = lcd_release,
  .read = lcd_read,
  .write = lcd_write,
  .owner = THIS_MODULE,
};

dev_t lcd_dev_num;
struct cdev lcd_cdev;
struct class *lcd_class;
struct device *lcd_device;

struct gpio_desc *g6;
struct gpio_desc *g5;
struct gpio_desc *g25;
struct gpio_desc *g24;
struct gpio_desc *g23;
struct gpio_desc *g22;
struct gpio_desc *g27;

static int __init lcd_chardev_init(void)
{
  int ret;

  // Dynamically allocate a device number
  ret = alloc_chrdev_region(&lcd_dev_num, 0, DEVICE_COUNT, "lcd_device");
  if (ret < 0) {
    pr_err("alloc chrdev failed\n");
    goto out;
  }

  pr_info("device number <major>:<minor> == %d:%d\n", MAJOR(lcd_dev_num), MINOR(lcd_dev_num));

  // Initialize cdev structure with lcd file operations
  cdev_init(&lcd_cdev, &lcd_fops);

  // Register a device (cdev structure) with VFS
  lcd_cdev.owner = THIS_MODULE;
  ret = cdev_add(&lcd_cdev, lcd_dev_num, DEVICE_COUNT);
  if (ret < 0) {
    pr_err("cdev_add failed\n");
    goto unreg_chrdev;
  }

  // Create lcd device class in /sys/class
  lcd_class = class_create(THIS_MODULE, "lcd_class");
  if (IS_ERR(lcd_class)) {
    pr_err("class creation failed\n");
    ret = PTR_ERR(lcd_class);
    goto cdev_delete;
  }

  lcd_device = device_create(lcd_class, NULL, lcd_dev_num, NULL, "lcd");
  if (IS_ERR(lcd_device)) {
    pr_err("failed to create device\n");
    ret = PTR_ERR(lcd_device);
    goto class_destroy;
  }

  pr_info("module initialized successfully \n");

  return 0;

class_destroy:
  class_destroy(lcd_class);
cdev_delete:
  cdev_del(&lcd_cdev);
unreg_chrdev:
  unregister_chrdev_region(lcd_dev_num, DEVICE_COUNT);
out:
  pr_err("module insertion failed\n");
  return ret;
}

static void __exit lcd_chardev_exit(void)
{
  device_destroy(lcd_class, lcd_dev_num);
  class_destroy(lcd_class);
  cdev_del(&lcd_cdev);
  unregister_chrdev_region(lcd_dev_num, DEVICE_COUNT);

  pr_info("module removed\n");
}

static int lcd_open(struct inode *inod, struct file *filp)
{
  pr_info("lcd opened\n");
  return 0;
}

static int lcd_release(struct inode *inod, struct file *filp)
{
  pr_info("lcd released\n");
  return 0;
}

static ssize_t lcd_read(struct file *filp, char __user *user_buf, size_t count, loff_t *fpos)
{
  pr_info("lcd read\n");
  return count;
}

static ssize_t lcd_write(struct file *filp, const char __user *user_buf, size_t count, loff_t *fpos)
{
  pr_info("lcd write\n");

  g6 = devm_gpiod_get(lcd_device, "GPIO6", GPIOD_OUT_LOW);
  if (IS_ERR(g6)) {
    dev_err(lcd_device, "failed to get GPIO descriptor\n");
    return PTR_ERR(g6);
  }
  g5 = devm_gpiod_get(lcd_device, "GPIO5", GPIOD_OUT_LOW);
  if (IS_ERR(g5)) {
    dev_err(lcd_device, "failed to get GPIO descriptor\n");
    return PTR_ERR(g5);
  }
  g25 = devm_gpiod_get(lcd_device, "GPIO25", GPIOD_OUT_LOW);
  if (IS_ERR(g25)) {
    dev_err(lcd_device, "failed to get GPIO descriptor\n");
    return PTR_ERR(g25);
  }
  g24 = devm_gpiod_get(lcd_device, "GPIO24", GPIOD_OUT_LOW);
  if (IS_ERR(g24)) {
    dev_err(lcd_device, "failed to get GPIO descriptor\n");
    return PTR_ERR(g24);
  }
  g23 = devm_gpiod_get(lcd_device, "GPIO23", GPIOD_OUT_LOW);
  if (IS_ERR(g23)) {
    dev_err(lcd_device, "failed to get GPIO descriptor\n");
    return PTR_ERR(g23);
  }
  g22 = devm_gpiod_get(lcd_device, "GPIO22", GPIOD_OUT_LOW);
  if (IS_ERR(g22)) {
    dev_err(lcd_device, "failed to get GPIO descriptor\n");
    return PTR_ERR(g22);
  }
  g27 = devm_gpiod_get(lcd_device, "GPIO27", GPIOD_OUT_LOW);
  if (IS_ERR(g27)) {
    dev_err(lcd_device, "failed to get GPIO descriptor\n");
    return PTR_ERR(g27);
  }

  lcd_init();
  lcd_display_clear();
  lcd_print_string("bradford is neat");

  return count;
}

module_init(lcd_chardev_init);
module_exit(lcd_chardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("bradford-hamilton");
MODULE_DESCRIPTION("gpio char driver");
MODULE_VERSION("1.0");

// Writes 4 bits of data to D4, D5, D6, D7 lines for 4-bit mode.
static void write_4_bits(uint8_t data)
{
  gpiod_set_value(g24, (data >> 0) & 0x1);
  gpiod_set_value(g23, (data >> 1) & 0x1);
  gpiod_set_value(g22, (data >> 2) & 0x1);
  gpiod_set_value(g27, (data >> 3) & 0x1);

  lcd_enable();
}

void lcd_enable(void)
{
  gpiod_set_value(g25, 0);
  fsleep(1);
  gpiod_set_value(g25, 1);
  fsleep(1);
  gpiod_set_value(g25, 0);
  fsleep(100);
}

void lcd_init(void)
{
  fsleep(40 * 1000);                        // Wait for more than 40 ms after LCD VCC rises to 2.7 V
  gpiod_set_value(g6, 0);                   // Set RS == 0 for LCD command
  gpiod_set_value(g5, 0);                   // Set RW == 0 for write
  write_4_bits(0x3);                        // Set D7, D6, D5, D4 to 0011
  fsleep(5 * 1000);                         // Wait for more than 4.1 ms
  write_4_bits(0x3);                        // Set D7, D6, D5, D4 to 0011
  fsleep(105);                              // Wait for more than 100 µs
  write_4_bits(0x3);                        // Set D7, D6, D5, D4 to 0011
  write_4_bits(0x02);                       // Set D7, D6, D5, D4 to 0010
  lcd_send_command(0x28);                   // 2 lines, font size 5x8
  lcd_send_command(0xE);                    // Turn display and cursor on
  lcd_display_clear();                      // Clear the display
  lcd_send_command(0x6);                    // Set entry mode increment addr by one
}

void lcd_send_command(uint8_t cmd)
{
  gpiod_set_value(g6, 0); // Set RS == 0 for LCD command
  gpiod_set_value(g5, 0); // Set RW == 0 for write

  write_4_bits(cmd >> 4);                   // High nibble
  write_4_bits(cmd);                        // Low nibble
}

void lcd_print_char(uint8_t data)
{
  gpiod_set_value(g6, 1);   // Set RS == 1 for data
  gpiod_set_value(g5, 0);  // Set RW == 0 for write

  write_4_bits(data >> 4);                   // High nibble
  write_4_bits(data);                        // Low nibble
}

void lcd_print_string(char *message)
{
  int i = 0;
  do {
    if (i == 16) {
      lcd_send_command(0xC0); // Set DDRAM address so that the cursor is positioned at the head of the 2nd line.
    }
    lcd_print_char((uint8_t)*message++);
    i++;
  } while (*message != '\0');
}

void lcd_display_clear(void)
{
  lcd_send_command(0x1);
  fsleep(1700); // Execution Time = 82µs - 1.64ms
}

void lcd_display_return_home(void)
{
  lcd_send_command(0x2);
  fsleep(1700); // Execution Time = 40µs - 1.6ms
}