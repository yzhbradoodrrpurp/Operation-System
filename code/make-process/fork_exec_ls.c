#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    pid_t childpid;
    int status = 0;

    childpid = fork();
    if (childpid < 0) {
        perror("fork error");
        return 1;
    }

    if (childpid == 0) {
        printf("CHILD: I will execute command: ls -l\n");
        execlp("ls", "ls", "-l", (char *)NULL);
        perror("execlp error");
        exit(127);
    }

    printf("PARENT: child PID = %d\n", childpid);
    if (waitpid(childpid, &status, 0) < 0) {
        perror("waitpid error");
        return 1;
    }

    if (WIFEXITED(status)) {
        printf("PARENT: child exit code = %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("PARENT: child terminated by signal = %d\n", WTERMSIG(status));
    }

    return 0;
}
