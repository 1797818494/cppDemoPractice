#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *Thread1(void *)
{
    while (1)
        ;
    sleep(1);
    int i, j;
    int policy;
    struct sched_param param;
    pthread_getschedparam(pthread_self(), &policy, &param);
    if (policy == SCHED_OTHER)
        printf("SCHED_OTHER\n");
    if (policy == SCHED_RR)
        ;
    printf("SCHED_RR 1 \n");
    if (policy == SCHED_FIFO)
        printf("SCHED_FIFO\n");

    for (i = 1; i < 10; i++)
    {
        for (j = 1; j < 5000000; j++)
        {
        }
        printf("thread 1\n");
    }
    printf("Pthread 1 exit\n");
}

void *Thread2(void *)
{
    while (1)
        ;
    sleep(1);
    int i, j, m;
    int policy;
    struct sched_param param;
    pthread_getschedparam(pthread_self(), &policy, &param);
    if (policy == SCHED_OTHER)
        printf("SCHED_OTHER\n");
    if (policy == SCHED_RR)
        printf("SCHED_RR\n");
    if (policy == SCHED_FIFO)
        printf("SCHED_FIFO\n");

    for (i = 1; i < 10; i++)
    {
        for (j = 1; j < 5000000; j++)
        {
        }
        printf("thread 2\n");
    }
    printf("Pthread 2 exit\n");
}

void *Thread3(void *)
{
    while (1)
        ;
    sleep(1);
    int i, j;
    int policy;
    struct sched_param param;
    pthread_getschedparam(pthread_self(), &policy, &param);
    if (policy == SCHED_OTHER)
        printf("SCHED_OTHER\n");
    if (policy == SCHED_RR)
        printf("SCHED_RR \n");
    if (policy == SCHED_FIFO)
        printf("SCHED_FIFO\n");

    for (i = 1; i < 10; i++)
    {
        for (j = 1; j < 5000000; j++)
        {
        }
        printf("thread 3\n");
    }
    printf("Pthread 3 exit\n");
}

int main()
{
    int i;
    i = getuid();
    if (i == 0)
        printf("The current user is root\n");
    else
        printf("The current user is not root\n");

    pthread_t ppid1, ppid2, ppid3;
    struct sched_param param;

    pthread_attr_t attr, attr1, attr2;
    pthread_attr_init(&attr1);
    pthread_attr_init(&attr);
    pthread_attr_init(&attr2);
    param.sched_priority = 51;
    pthread_attr_setschedpolicy(&attr2, SCHED_RR);
    pthread_attr_setschedparam(&attr2, &param);
    pthread_attr_setinheritsched(&attr2, PTHREAD_EXPLICIT_SCHED); // 要使优先级其作用必须要有这句话

    param.sched_priority = 21;
    pthread_attr_setschedpolicy(&attr1, SCHED_RR);
    pthread_attr_setschedparam(&attr1, &param);
    pthread_attr_setinheritsched(&attr1, PTHREAD_EXPLICIT_SCHED);

    pthread_create(&ppid3, &attr, Thread3, NULL);
    pthread_create(&ppid2, &attr1, Thread2, NULL);
    pthread_create(&ppid1, &attr2, Thread1, NULL);

    pthread_join(ppid3, NULL);
    pthread_join(ppid2, NULL);
    pthread_join(ppid1, NULL);
    pthread_attr_destroy(&attr2);
    pthread_attr_destroy(&attr1);
    return 0;
}