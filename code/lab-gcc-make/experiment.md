# 实验报告

## 实验一

```shell
Last login: Tue Apr 21 14:46:42 on ttys013
yzhbradoodrrpurp@yzhbradoodrrpurps-MacBook code % cd .. # 去到某个路径
yzhbradoodrrpurp@yzhbradoodrrpurps-MacBook OS % pwd # 显示当前路径
/Users/yzhbradoodrrpurp/Desktop/OS
yzhbradoodrrpurp@yzhbradoodrrpurps-MacBook OS % ls # -a 显示所有文件; -l 显示具体信息
code	notes	slides
yzhbradoodrrpurp@yzhbradoodrrpurps-MacBook OS % touch test.txt
yzhbradoodrrpurp@yzhbradoodrrpurps-MacBook OS % cp test.txt backup.txt # 复制文件; -r 递归复制
yzhbradoodrrpurp@yzhbradoodrrpurps-MacBook OS % mv test.txt ./code/text.txt # -r 递归删除
yzhbradoodrrpurp@yzhbradoodrrpurps-MacBook OS % rm backup.txt # -r 递归删除
yzhbradoodrrpurp@yzhbradoodrrpurps-MacBook OS % mkdir test
yzhbradoodrrpurp@yzhbradoodrrpurps-MacBook OS % rmdir test
yzhbradoodrrpurp@yzhbradoodrrpurps-MacBook OS % cat code/text.txt  
yzhbradoodrrpurp@yzhbradoodrrpurps-MacBook OS % more code/lab-gcc-make/myapp.c # 分页查看
#include "greeting.h"

int main(void) {
    greeting();
    return 0;
}
 ESCOC










~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
 ESCOD
#include "greeting.h"

int main(void) {
    greeting();
    return 0;
}
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
 ESCOD
#include "greeting.h"

int main(void) {
    greeting();
    return 0;
}
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
 ESCOD
#include "greeting.h"

int main(void) {
    greeting();
    return 0;
}
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
~
yzhbradoodrrpurp@yzhbradoodrrpurps-MacBook OS % less code/lab-gcc-make/myapp.c # 分页查看
yzhbradoodrrpurp@yzhbradoodrrpurps-MacBook OS % head code/lab-gcc-make/myapp.c # tail file 查看前10行; -n 20 查看前20行
#include "greeting.h"

int main(void) {
    greeting();
    return 0;
}
yzhbradoodrrpurp@yzhbradoodrrpurps-MacBook OS % tail code/lab-gcc-make/myapp.c # 查看后10行; -n 20 查看后20行
#include "greeting.h"

int main(void) {
    greeting();
    return 0;
}
yzhbradoodrrpurp@yzhbradoodrrpurps-MacBook OS % find . -name "*.txt" # 根据名字找文件
./code/text.txt
yzhbradoodrrpurp@yzhbradoodrrpurps-MacBook OS % whereis gcc # 查找命令
gcc: /usr/bin/gcc
yzhbradoodrrpurp@yzhbradoodrrpurps-MacBook OS % grep "main" ./code/lab-gcc-make/myapp.c # 在文件中搜索字符串
int main(void) {
yzhbradoodrrpurp@yzhbradoodrrpurps-MacBook OS % chmod u+w ./code/lab-gcc-make/myapp.c # 对于该文件给用户加上写权限

yzhbradoodrrpurp@yzhbradoodrrpurps-MacBook OS % tar -czvf ./code/lab-gcc-make.tar.gz ./code/lab-gcc-make # c=创建 z=gzip v=显示过程 f=文件名
a ./code/lab-gcc-make
a ./code/lab-gcc-make/hello.c
a ./code/lab-gcc-make/experiment.md
a ./code/lab-gcc-make/.DS_Store
a ./code/lab-gcc-make/Makefile
a ./code/lab-gcc-make/resources
a ./code/lab-gcc-make/myapp.c
a ./code/lab-gcc-make/greeting.c
a ./code/lab-gcc-make/myapp
a ./code/lab-gcc-make/hello.i
a ./code/lab-gcc-make/hello.s
a ./code/lab-gcc-make/hello.o
a ./code/lab-gcc-make/hello
a ./code/lab-gcc-make/experiment.pdf
a ./code/lab-gcc-make/greeting.o
a ./code/lab-gcc-make/greeting.h
a ./code/lab-gcc-make/myapp.o
a ./code/lab-gcc-make/resources/make.png
a ./code/lab-gcc-make/resources/hello.png
yzhbradoodrrpurp@yzhbradoodrrpurps-MacBook OS % gzip ./code/text.txt  
yzhbradoodrrpurp@yzhbradoodrrpurps-MacBook OS % touch test.txt             
yzhbradoodrrpurp@yzhbradoodrrpurps-MacBook OS % bzip2 test.txt 
yzhbradoodrrpurp@yzhbradoodrrpurps-MacBook OS % 

```



## Hello gcc

`hello.c` 文件内容如下：

```c
#include <stdio.h>

int main(void) {
    printf("Hello gcc!\n");
    return 0;
}
```

在 Terminal 中通过如下指令得到 `*.c`, `*.i`, `*.s`, `*.o` 文件：

```shell
# 预处理：.c -> .i
gcc -E hello.c -o hello.i
# 编译成汇编：.i/.c -> .s
gcc -S hello.c -o hello.s
# 汇编成目标文件：.s/.c -> .o
gcc -c hello.c -o hello.o
# 链接成可执行文件
gcc hello.c -o hello
```

Terminal 截图：

![hello](/Users/yzhbradoodrrpurp/Desktop/OS/code/lab-gcc-make/resources/hello.png)

## Makefile

`myapp.c` 文件内容：

```c
#include "greeting.h"

int main(void) {
    greeting();
    return 0;
}
```

`greeting.c` 文件内容：

```c
#include <stdio.h>
#include "greeting.h"

void greeting(void) {
    printf("Hello from greeting.c!\n");
}
```

`greeting.h` 文件内容：

```c
#ifndef GREETING_H
#define GREETING_H

void greeting(void);

#endif
```

`Makefile` 文件内容：

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -g

.PHONY: all run hello_stages clean

all: myapp

myapp: myapp.o greeting.o
	$(CC) -o $@ $^

myapp.o: myapp.c greeting.h
	$(CC) $(CFLAGS) -c myapp.c -o myapp.o

greeting.o: greeting.c greeting.h
	$(CC) $(CFLAGS) -c greeting.c -o greeting.o

run: myapp
	./myapp

hello_stages: hello.c
	$(CC) -E hello.c -o hello.i
	$(CC) -S hello.c -o hello.s
	$(CC) -c hello.c -o hello.o
	$(CC) hello.c -o hello
	$(CC) -g hello.c -o hello_g

clean:
	rm -f *.o myapp hello hello_g hello.i hello.s

```

在 Terminal 当前目录下输入 `make` 即可得到可执行文件：

![make](/Users/yzhbradoodrrpurp/Desktop/OS/code/lab-gcc-make/resources/make.png)
