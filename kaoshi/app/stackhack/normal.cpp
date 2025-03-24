#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;

void thread_func_a(int** addr) {
    int stack_var = 14;
    *addr = &stack_var;
    std::cout << "Thread A: Stack variable address: " << addr << std::endl;

    while (1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Thread A: Stack variable value: " << stack_var << std::endl;
        cv.notify_all();
    }
}

void thread_func_b(int** addr) {
    std::unique_lock <std::mutex> lck(mtx);
    cv.wait(lck);
    std::cout << "Thread B: Stack variable address: " << addr << std::endl;
    **addr = 255;
}

int main(){
    int* addr;
    
    std::thread t1(thread_func_a, &addr);
    t1.detach();
    std::thread t2(thread_func_b, &addr);
    t2.detach();
    
    while(1)
    {
        sleep(1);
    }
    return 0;
}