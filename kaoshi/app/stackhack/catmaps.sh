#!/bin/bash
PID=$1
echo "Thread IDs for process $PID:"
ps -L -o tid= -p $PID | while read tid; do
    echo "Thread $tid:"
    grep -E "\[stack" /proc/$tid/maps || echo "  [stack not found]"
done