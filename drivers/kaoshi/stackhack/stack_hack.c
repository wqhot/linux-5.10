#include <linux/module.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/uaccess.h>
#include <linux/mm.h>

#define DEVICE_NAME "stack_hack"

struct stack_write_request {
    int pid;
    unsigned long addr;
    unsigned long data;
};

static long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    struct stack_write_request req;

    if (copy_from_user(&req, (void __user *)arg, sizeof(req)))
        return -EFAULT;

    struct pid *pid_struct = find_get_pid(req.pid);
    if (!pid_struct)
        return -ESRCH;

    struct task_struct *tsk = pid_task(pid_struct, PIDTYPE_PID);
    if (!tsk) {
        put_pid(pid_struct);
        return -ESRCH;
    }

    unsigned long data = req.data;
    int ret = access_process_vm(tsk, req.addr, &data, sizeof(data), FOLL_WRITE);
    
    put_pid(pid_struct);
    return (ret == sizeof(data)) ? 0 : -EIO;
}

static struct file_operations fops = {
    .unlocked_ioctl = device_ioctl,
};

static int __init module_init(void) {
    register_chrdev(0, DEVICE_NAME, &fops);
    return 0;
}

static void __exit module_exit(void) {
    unregister_chrdev(0, DEVICE_NAME);
}

module_init(module_init);
module_exit(module_exit);
MODULE_LICENSE("GPL");