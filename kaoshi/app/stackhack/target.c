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

void print_stack_base() {
    FILE *fp = fopen("/proc/self/stat", "r");
    if (fp) {
        char buf[1024];
        if (fgets(buf, sizeof(buf), fp)) {
            char *p = strtok(buf, " ");
            for (int i=0; i<33; i++) p = strtok(NULL, " "); // 第34个字段
            printf("进程栈底地址: %s\n", p);
        }
        fclose(fp);
    }
}


int main() {
    print_stack_base();
    leak_stack();
    return 0;
}