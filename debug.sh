#!/bin/sh

./build.sh
gdb -ex "b cpu.c:37" -ex "b cpu.c:50" -ex "b cpu.c:99" -ex "b cpu.c:57" -ex "b cpu.c:53" -ex "b cpu.c:47" -ex "b cpu.c:44" -ex "b cpu.c:41" -ex "b cpu.c:37" -ex "b cpu.c:32" -ex "b cpu.c:102" --args chippy test_opcode.ch8
