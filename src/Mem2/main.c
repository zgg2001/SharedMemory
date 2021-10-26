/*************************************************************************
	> File Name: main.c
	> Author: ZHJ
	> Remarks: 信号量测试
	> Created Time: Tue 26 Oct 2021 11:33:27 AM CST
 ************************************************************************/

#include<sys/ipc.h>
#include<sys/sem.h>
#include<stdio.h>
#include<unistd.h>
#include<errno.h>

//信号灯共同体
typedef union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *arry;
} semun;
//初始化信号灯
int init(key_t key, int* semid);
//等待信号灯挂出
int wait(int semid);
//挂出信号灯
int hangout(int semid);

int main()
{
    int semid;
    key_t key = 0x2333;//key值

    //初始化
    if(init(key, &semid) == 0)
    {
        return -1;
    }
    //进入队列，等待信号灯挂出
    if(wait(semid) == 0)
    {
        return -2;
    }
    //各种操作
    printf("pid: %d 获取到信号灯 开始操作\n", getpid());
    printf("操作1\n");
    printf("操作2 - sleep 10秒\n");
    sleep(10);
    //挂出信号灯
    if(hangout(semid) == 0)
    {
        return -3;
    }
    /*/销毁 
    if(semctl(sem_id, 0, IPC_RMID) == -1)
    {
        perror("destroy semctl()");
        return -4;
    }
    */
    return 0;
}

int init(key_t key, int* semid)
{
    if((*semid = semget(key, 1, 0777)) < 0)
    {
        //errno = 2 说明没创建
        if(errno == 2)
        {
            if((*semid = semget(key, 1, 0777|IPC_CREAT)) < 0) 
            { 
                perror("2 - semget()"); 
                return 0; 
            }
            //信号灯初始化 SETVAL为初始化 IPC_RMID为销毁
            semun sem_union;
            sem_union.val = 1;
            if(semctl(*semid, 0, SETVAL, sem_union) < 0) 
            { 
                perror("semctl()"); 
                return 0; 
            }
        }
        else
        { 
            perror("1 - semget()"); 
            return 0; 
        }
    } 
    return 1;
}

int wait(int semid)
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = -1;
    sem_b.sem_flg = SEM_UNDO;
    if(semop(semid, &sem_b, 1) < 0) 
    { 
        perror("wait semop()"); 
        return 0; 
    } 
    return 1;
}

int hangout(int semid)
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = 1;  
    sem_b.sem_flg = SEM_UNDO;
    if(semop(semid, &sem_b, 1) < 0)
    {
        perror("hang out semop()");
        return 0; 
    }
    return 1;
}
