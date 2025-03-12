#include <stdio.h>
#include <unistd.h>

void leak_stack() {
    int dummy = 14;
    printf("栈地址：%p\n", (void*)&dummy); // 泄漏栈地址
    fflush(stdout);
    while(1) {
            sleep(1); // 保持运行
            printf("stack addr:  %p\n", (void*)&dummy);
            printf("stack value: %d\n", dummy);
    }
}

int main() {
    leak_stack();
    return 0;
}