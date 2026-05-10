#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 31

typedef enum {
    STATE_READY,
    STATE_RUNNING,
    STATE_FINISHED
} ProcessState;

typedef struct PCB {
    int pid;
    char name[NAME_LEN + 1];
    int priority;
    int burst_time;
    int start_time;
    int finish_time;
    int waiting_time;
    int turnaround_time;
    ProcessState state;
    struct PCB *next;
} PCB;

typedef struct {
    PCB *head;
    PCB *tail;
    int size;
} ReadyQueue;

typedef enum {
    ALG_FCFS = 1,
    ALG_PR = 2
} Algorithm;

static const char *state_to_text(ProcessState state) {
    switch (state) {
        case STATE_READY:
            return "READY";
        case STATE_RUNNING:
            return "RUN";
        case STATE_FINISHED:
            return "FIN";
        default:
            return "UNK";
    }
}

static void init_ready_queue(ReadyQueue *queue) {
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
}

static void enqueue_fcfs(ReadyQueue *queue, PCB *node) {
    node->next = NULL;
    if (queue->tail == NULL) {
        queue->head = node;
        queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }
    queue->size++;
}

static void enqueue_pr(ReadyQueue *queue, PCB *node) {
    PCB *prev = NULL;
    PCB *curr = queue->head;

    node->next = NULL;
    while (curr != NULL) {
        if (node->priority > curr->priority) {
            break;
        }
        if (node->priority == curr->priority && node->pid < curr->pid) {
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    if (prev == NULL) {
        node->next = queue->head;
        queue->head = node;
        if (queue->tail == NULL) {
            queue->tail = node;
        }
    } else {
        prev->next = node;
        node->next = curr;
        if (curr == NULL) {
            queue->tail = node;
        }
    }
    queue->size++;
}

static void enqueue(ReadyQueue *queue, PCB *node, Algorithm alg) {
    if (alg == ALG_PR) {
        enqueue_pr(queue, node);
    } else {
        enqueue_fcfs(queue, node);
    }
}

static PCB *dequeue(ReadyQueue *queue) {
    PCB *node;

    if (queue->head == NULL) {
        return NULL;
    }

    node = queue->head;
    queue->head = node->next;
    if (queue->head == NULL) {
        queue->tail = NULL;
    }
    node->next = NULL;
    queue->size--;
    return node;
}

static void print_queue(const ReadyQueue *queue) {
    const PCB *curr = queue->head;

    if (curr == NULL) {
        printf("    <empty>\n");
        return;
    }

    while (curr != NULL) {
        printf("    [PID=%d, Name=%s, Priority=%d, Burst=%d, State=%s]\n",
               curr->pid,
               curr->name,
               curr->priority,
               curr->burst_time,
               state_to_text(curr->state));
        curr = curr->next;
    }
}

static int read_int(const char *prompt, int min_value) {
    int value;
    int rc;

    for (;;) {
        printf("%s", prompt);
        rc = scanf("%d", &value);
        if (rc == 1 && value >= min_value) {
            return value;
        }
        printf("输入无效，请重新输入（最小值 %d）。\n", min_value);
        while (getchar() != '\n') {
        }
    }
}

static void read_name(char *name_buffer, size_t size, int index) {
    printf("请输入进程 %d 的名称: ", index);
    scanf("%31s", name_buffer);
    name_buffer[size - 1] = '\0';
}

static void run_schedule(PCB *pcbs, int n, Algorithm alg) {
    ReadyQueue queue;
    int clock_time = 0;
    int i;
    int step = 1;
    int total_wait = 0;
    int total_turnaround = 0;
    PCB *running;

    init_ready_queue(&queue);
    for (i = 0; i < n; ++i) {
        pcbs[i].state = STATE_READY;
        pcbs[i].start_time = -1;
        pcbs[i].finish_time = -1;
        pcbs[i].waiting_time = 0;
        pcbs[i].turnaround_time = 0;
        enqueue(&queue, &pcbs[i], alg);
    }

    printf("\n===== 调度开始（算法：%s）=====\n", alg == ALG_FCFS ? "FCFS" : "PR");
    while ((running = dequeue(&queue)) != NULL) {
        printf("\n[Step %d] CPU 调度过程\n", step++);
        printf("  运行前就绪队列:\n");
        print_queue(&queue);

        running->state = STATE_RUNNING;
        running->start_time = clock_time;
        running->waiting_time = running->start_time;
        running->finish_time = running->start_time + running->burst_time;
        running->turnaround_time = running->finish_time;
        clock_time = running->finish_time;

        printf("  -> 当前运行进程: PID=%d Name=%s\n", running->pid, running->name);
        printf("     起始时间=%d, 终止时间=%d, 运行时长=%d\n",
               running->start_time,
               running->finish_time,
               running->burst_time);

        running->state = STATE_FINISHED;
        total_wait += running->waiting_time;
        total_turnaround += running->turnaround_time;

        printf("  运行后进程状态: %s\n", state_to_text(running->state));
    }

    printf("\n===== 调度结果汇总 =====\n");
    printf("%-5s %-10s %-9s %-8s %-8s %-8s %-10s %-11s\n",
           "PID",
           "Name",
           "Priority",
           "Burst",
           "Start",
           "Finish",
           "Waiting",
           "Turnaround");
    for (i = 0; i < n; ++i) {
        printf("%-5d %-10s %-9d %-8d %-8d %-8d %-10d %-11d\n",
               pcbs[i].pid,
               pcbs[i].name,
               pcbs[i].priority,
               pcbs[i].burst_time,
               pcbs[i].start_time,
               pcbs[i].finish_time,
               pcbs[i].waiting_time,
               pcbs[i].turnaround_time);
    }
    printf("\n平均等待时间: %.2f\n", (double)total_wait / n);
    printf("平均周转时间: %.2f\n", (double)total_turnaround / n);
    printf("===== 调度结束 =====\n");
}

int main(void) {
    int n;
    int i;
    int alg_input;
    Algorithm alg;
    PCB *pcbs;

    printf("单处理机进程调度模拟程序（FCFS / PR）\n");
    n = read_int("请输入进程个数: ", 1);

    pcbs = (PCB *)calloc((size_t)n, sizeof(PCB));
    if (pcbs == NULL) {
        fprintf(stderr, "内存分配失败。\n");
        return 1;
    }

    printf("\n请输入每个进程的信息：\n");
    for (i = 0; i < n; ++i) {
        pcbs[i].pid = i + 1;
        read_name(pcbs[i].name, sizeof(pcbs[i].name), i + 1);
        pcbs[i].priority = read_int("请输入进程优先级(整数，越大越高): ", 0);
        pcbs[i].burst_time = read_int("请输入进程运行时间(>0): ", 1);
        pcbs[i].next = NULL;
        printf("\n");
    }

    printf("请选择调度算法：\n");
    printf("  1. FCFS (先来先服务)\n");
    printf("  2. PR   (优先级，数值越大优先级越高)\n");
    alg_input = read_int("输入选项(1/2): ", 1);
    if (alg_input != 1 && alg_input != 2) {
        printf("选项无效，默认使用 FCFS。\n");
        alg = ALG_FCFS;
    } else {
        alg = (Algorithm)alg_input;
    }

    run_schedule(pcbs, n, alg);

    free(pcbs);
    return 0;
}
