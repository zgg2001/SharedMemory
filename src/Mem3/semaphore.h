/*************************************************************************
	> File Name: semaphore.h
	> Author: ZHJ
	> Remarks: 信号量头文件
	> Created Time: Sat 30 Oct 2021 09:57:36 PM CST
 ************************************************************************/

#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

#include<sys/ipc.h>
#include<sys/sem.h>
#include<unistd.h>
#include<errno.h>

//信号灯共同体
typedef union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *arry;
} semun;

//信号灯类
class semaphore
{
public:
    semaphore(key_t key)
        : _key(key) {}
    ~semaphore() {}
    //信号灯初始化
    int init()
    {
        if((_semid = semget(_key, 1, 0777)) < 0)
        {
            //errno = 2 说明没创建
            if(errno == 2)
            {
                if((_semid = semget(_key, 1, 0777|IPC_CREAT)) < 0)
                {
                    perror("2 - semget()");
                    return 0;
                }
                //信号灯初始化 SETVAL为初始化 IPC_RMID为销毁
                semun sem_union;
                sem_union.val = 1;
                if(semctl(_semid, 0, SETVAL, sem_union) < 0)
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
    //等待信号灯挂出    
    int wait()
    {
        struct sembuf sem_b;
        sem_b.sem_num = 0;
        sem_b.sem_op = -1;
        sem_b.sem_flg = SEM_UNDO;
        if(semop(_semid, &sem_b, 1) < 0)
        {
            perror("wait semop()");
            return 0;
        }
        return 1;
    }
    //挂出信号灯
    int hangout()
    {
        struct sembuf sem_b;
        sem_b.sem_num = 0;
        sem_b.sem_op = 1;
        sem_b.sem_flg = SEM_UNDO;
        if(semop(_semid, &sem_b, 1) < 0)
        {
            perror("hang out semop()");
            return 0;
        }
        return 1;
    }
    //销毁信号灯
    int destroy()
    {        
        if(semctl(_semid, 0, IPC_RMID) == -1)
        {
            perror("destroy semctl()");
            return 0;
        }
        return 1;
    }

private:
    //键
    key_t _key = 0x0000;
    //信号id
    int _semid = 0;
};

#endif
