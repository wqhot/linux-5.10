#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

struct stack_write_request {
    int pid;
    unsigned long addr;
    unsigned long data;
};

#define WRITE_STACK _IOW('S', 1, struct stack_write_request)

int main() {
    int fd = open("/dev/stack_hack", O_RDWR);
    if (fd < 0) {
        perror("打开设备失败");
        return 1;
    }

    struct stack_write_request req;
    printf("输入目标PID: ");
    scanf("%d", &req.pid);
    printf("输入要写入的地址（16进制）: ");
    scanf("%lx", &req.addr);
    printf("输入数据（16进制）: ");
    scanf("%lx", &req.data);

    if (ioctl(fd, WRITE_STACK, &req) < 0) {
        perror("ioctl失败");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}