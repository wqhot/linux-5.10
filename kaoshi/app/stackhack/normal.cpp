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
        printf("pid: %d\n", getpid());
        fflush(stdout);
        while(1) {
            sleep(1); // 保持运行
            printf("stack addr:  %p\n", (void*)&dummy);
            printf("stack value: %d\n", dummy);
        }
    }
};

class target2 {
    public:
        float dummy;
        target2() {
            dummy = 14;
        }
        void leak_stack() {
            printf("栈地址：%p\n", (void*)&dummy); // 泄漏栈地址
            printf("pid: %d\n", getpid());
            fflush(stdout);
            while(1) {
                sleep(1); // 保持运行
                printf("stack addr:  %p\n", (void*)&dummy);
                printf("stack value: %f\n", dummy);
            }
        }
    };

void thread_func(int v) {
    target local_tgt; 
    local_tgt.dummy = v;
    local_tgt.leak_stack();
}

void thread_func2(int v) {
    target2 local_tgt; 
    local_tgt.dummy = v;
    local_tgt.leak_stack();
}

int main(){
    int dummy;
    printf("main thread address: %p\n", &dummy);
    std::thread t1(thread_func, 10);
    t1.detach();
    std::thread t2(thread_func2, 100);
    t2.detach();
    while(1)
    {
        sleep(1);
    }
    return 0;
}