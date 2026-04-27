#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    pid_t childpid;
    int retval = 0;
    int status = 0;

    childpid = fork();
    if (childpid < 0) {
        perror("fork error");
        return 1;
    }

    if (childpid == 0) {
        printf("CHILD: I am the child process!\n");
        printf("CHILD: Here's my PID: %d\n", getpid());
        printf("CHILD: My parent's PID is: %d\n", getppid());
        printf("CHILD: The value of fork return is: %d\n", childpid);
        printf("CHILD: Sleep for 1 second...\n");
        sleep(1);
        printf("CHILD: Enter an exit value (0~255): ");
        if (scanf("%d", &retval) != 1) {
            fprintf(stderr, "CHILD: Invalid input, use default 1\n");
            retval = 1;
        }
        retval &= 0xFF;
        printf("CHILD: Goodbye!\n");
        exit(retval);
    }

    printf("PARENT: I am the parent process!\n");
    printf("PARENT: Here's my PID: %d\n", getpid());
    printf("PARENT: The value of my child's PID is: %d\n", childpid);
    printf("PARENT: I will now wait for my child to exit.\n");

    if (waitpid(childpid, &status, 0) < 0) {
        perror("waitpid error");
        return 1;
    }

    if (WIFEXITED(status)) {
        printf("PARENT: Child's exit code is: %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("PARENT: Child terminated by signal: %d\n", WTERMSIG(status));
    } else {
        printf("PARENT: Child exited with unexpected status: %d\n", status);
    }

    printf("PARENT: Goodbye!\n");
    return 0;
}
