#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <thread>

class target {
public:
    int dummy;
    target() {
        dummy = 14;
    }
    void leak_stack() {
        printf("栈地址：%p\n", (void*)&dummy); // 泄漏栈地址
        fflush(stdout);
        while(1) {
            sleep(1); // 保持运行
            printf("stack addr:  %p\n", (void*)&dummy);
            printf("stack value: %d\n", dummy);
        }
    }
};

struct stack_write_request {
    int pid;
    unsigned long addr;
    unsigned long data;
};

class attacker {
    void attack(int pid, unsigned long addr, unsigned long data) {
        // 伪造请求
        struct stack_write_request req;
        req.pid = pid;
        req.addr = addr;
        req.data = data;
        // 打开设备
        int fd = open("/dev/stack_hack", O_RDWR);
        if (fd < 0) {
            perror("打开设备失败");
            return;
        }
        // 发送攻击请求
        if (ioctl(fd, WRITE_STACK, &req) < 0) {
            perror("ioctl失败");
            close(fd);
            return;
        }
        close(fd);
    }

    void normal(target &t) {
        t.dummy = 144;
    }
}

int main(){
    target t;
    attacker a;
    a.normal(t);
    std::thread t1(&target::leak_stack, &t);
    t1.detach();
    int pid = getpid();
    unsigned long addr = (unsigned long)&t.dummy;
    unsigned long data = 256;
    sleep(5);
    a.attack(pid, addr, data);
    return 0;
}