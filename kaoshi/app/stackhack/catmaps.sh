#!/bin/bash
PID=$1
echo "Thread IDs for process $PID:"
ps -L -o tid= -p $PID | while read tid; do
    echo "Thread $tid:"
    # 查找明确的栈标记或匿名映射区域
    if grep -q "\[stack" /proc/$tid/maps; then
        grep "\[stack" /proc/$tid/maps
    else
        echo "  [子线程栈位于匿名映射区域]"
        # 打印内存映射的末尾部分（栈通常在高地址）
        tail -n 5 /proc/$tid/maps | grep -B1 -A3 'rw-p'
    fi
done