# 实验报告

示例代码如下：
```c
#include <errno.h> // 引入错误码相关定义，本程序主要用 perror 打印系统错误
#include <stdio.h>     
#include <stdlib.h>     
#include <sys/types.h> // 引入系统数据类型定义，如 pid_t
#include <sys/wait.h> // 引入 waitpid 及子进程状态解析宏
#include <unistd.h> // 引入 POSIX 接口，如 fork / getpid / getppid / sleep

int main(void) {                          
    pid_t childpid; // 保存 fork 返回值：父进程中是子 PID，子进程中是 0
  
    int retval = 0; // 子进程退出码，先初始化为 0
    int status = 0; // 父进程用于接收 waitpid 返回的状态字

    childpid = fork(); // 创建子进程：成功后父子都从这里继续执行
  
    if (childpid < 0) { // fork 返回负值表示创建失败
        perror("fork error"); // 打印错误原因（结合 errno）
        return 1;
    }

    if (childpid == 0) { // 返回 0 说明当前执行流处于子进程
        printf("CHILD: I am the child process!\n"); // 子进程身份提示
        printf("CHILD: Here's my PID: %d\n", getpid()); // 打印子进程自身 PID
        printf("CHILD: My parent's PID is: %d\n", getppid()); // 打印子进程看到的父进程 PID
        printf("CHILD: The value of fork return is: %d\n", childpid); // 子进程中 fork 返回值恒为 0
        printf("CHILD: Sleep for 1 second...\n"); // 提示准备休眠
        sleep(1); // 休眠 1 秒，便于观察并发输出
        printf("CHILD: Enter an exit value (0~255): "); // 提示输入退出码
        
      	if (scanf("%d", &retval) != 1) { // 读取整数；返回值不是 1 代表输入失败
            fprintf(stderr, "CHILD: Invalid input, use default 1\n"); // 错误输出到标准错误流
            retval = 1; // 输入非法时使用默认退出码 1
        }
      
        retval &= 0xFF; // 只保留低 8 位，确保退出码落在 0~255
        printf("CHILD: Goodbye!\n"); // 子进程结束前提示
        exit(retval); // 子进程按输入的退出码结束
    }

    printf("PARENT: I am the parent process!\n"); // 父进程身份提示
    printf("PARENT: Here's my PID: %d\n", getpid()); // 打印父进程自身 PID
    printf("PARENT: The value of my child's PID is: %d\n", childpid); // 打印子进程 PID（来自 fork 返回值）
    printf("PARENT: I will now wait for my child to exit.\n"); // 提示即将等待子进程

    if (waitpid(childpid, &status, 0) < 0) { // 等待指定子进程退出，失败返回 -1
        perror("waitpid error"); // 打印 waitpid 失败原因
        return 1; // 父进程异常结束
    }

    if (WIFEXITED(status)) { // 判断子进程是否“正常调用 exit/return”退出
        printf("PARENT: Child's exit code is: %d\n", WEXITSTATUS(status)); // 取出正常退出码
    } else if (WIFSIGNALED(status)) { // 否则判断是否被信号终止
        printf("PARENT: Child terminated by signal: %d\n", WTERMSIG(status)); // 打印终止信号编号
    } else { // 其余非常见状态（如停止/继续等）
        printf("PARENT: Child exited with unexpected status: %d\n", status); // 打印原始状态字
    }

    printf("PARENT: Goodbye!\n");
    return 0;
}

```

输出：

```shell
yzhbradoodrrpurp@yzhbradoodrrpurps make-process % make run_fork_wait  # 执行 Makefile
./fork_wait_demo # make 触发的实际运行命令
PARENT: I am the parent process! # fork 后父进程分支先获得 CPU，先输出父进程提示
PARENT: Here\'s my PID: 29439 # 父进程 PID=29439
PARENT: The value of my child\'s PID is: 29440 # 在父进程中 fork 返回子进程 PID=29440
PARENT: I will now wait for my child to exit. # 父进程即将调用 waitpid 阻塞等待子进程结束
CHILD: I am the child process! # 子进程开始执行并输出身份提示
CHILD: Here\'s my PID: 29440 # 子进程自身 PID=29440（与父进程上一行打印一致）
CHILD: My parent\'s PID is: 29439 # 子进程看到的父进程 PID=29439
CHILD: The value of fork return is: 0 # 在子进程中 fork 返回值固定为 0
CHILD: Sleep for 1 second... # 子进程主动休眠 1 秒，便于观察并发时序
CHILD: Enter an exit value (0~255): 155 # 用户输入退出码 155
CHILD: Goodbye! # 子进程按输入值执行 exit(155) 前的结束提示
PARENT: Child\'s exit code is: 155 # 父进程 waitpid 返回后解析到子进程退出码为 155
PARENT: Goodbye! # 父进程完成回收并正常退出
```
