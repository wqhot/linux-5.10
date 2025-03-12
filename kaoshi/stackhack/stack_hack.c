#include <linux/module.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <linux/cdev.h>

#define DEVICE_NAME "stack_hack"

static struct class *stack_class;
static dev_t devno;
static struct cdev stack_cdev;

struct stack_write_request {
	int pid;
	unsigned long addr;
	unsigned long data;
};

static long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct stack_write_request req;
	struct pid *pid_struct;
	struct task_struct *tsk;
	unsigned long data;
	int ret;

	if (copy_from_user(&req, (void __user *)arg, sizeof(req))) {
		printk(KERN_ERR "Failed to copy_from_user\n");
		return -EFAULT;
	}

	pid_struct = find_get_pid(req.pid);
	if (!pid_struct) {
		printk(KERN_ERR "Failed to find_get_pid\n");
		return -ESRCH;
	}

	tsk = pid_task(pid_struct, PIDTYPE_PID);
	if (!tsk) {
        printk(KERN_ERR "Failed to find_task_by_pid\n");
		put_pid(pid_struct);
		return -ESRCH;
	}

	data = req.data;
	ret = access_process_vm(tsk, req.addr, &data, sizeof(data), FOLL_WRITE);
    printk(KERN_INFO "access_process_vm ret: %d\n", ret);

	put_pid(pid_struct);
    printk(KERN_INFO "data: %lx\n", data);
	return (ret == sizeof(data)) ? 0 : -EIO;
}

static struct file_operations fops = {
	.unlocked_ioctl = device_ioctl,
};

static int __init stack_hack_init(void)
{
	// 分配设备号
    int major, minor;
    alloc_chrdev_region(&devno, 0, 1, DEVICE_NAME);
    major = MAJOR(devno);
    minor = MINOR(devno);

    // 注册 file_operations
    cdev_init(&stack_cdev, &fops);
    cdev_add(&stack_cdev, devno, 1);

    // 创建设备类和设备节点
    stack_class = class_create(THIS_MODULE, DEVICE_NAME);
    device_create(stack_class, NULL, devno, NULL, DEVICE_NAME);

    printk(KERN_INFO "Registered stack_hack with major %d\n", major);
	return 0;
}

static void __exit stack_hack_exit(void)
{
	device_destroy(stack_class, devno);
    class_unregister(stack_class);
    class_destroy(stack_class);
    unregister_chrdev_region(devno, 1);
    cdev_del(&stack_cdev);
	printk(KERN_INFO "Unregistered stack_hack device\n");
}

module_init(stack_hack_init);
module_exit(stack_hack_exit);
MODULE_LICENSE("GPL");