/*************************************************************************
	> File Name: shared_memory.h
	> Author: ZHJ
	> Remarks: 共享内存类
	> Created Time: Mon 01 Nov 2021 08:45:49 AM CST
 ************************************************************************/

#ifndef _SHARED_MEMORY_H
#define _SHARED_MEMORY_H

#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>

class shared_memory
{
public:  
    shared_memory(key_t key)
        : _key(key) {}
    ~shared_memory() {}
    //获取内存
    int getmem(int size)
    {
        _shmid = shmget(_key, size, 0777|IPC_CREAT);
        if(_shmid < 0)
        {
            perror("shmget");
            return 0;
        }
        return 1;
    }
    //获取内存的地址
    void* getptr()
    {
        void* temp = shmat(_shmid, 0, 0);
        if(temp < 0)
        {
            perror("shmat");
            return NULL;
        }
        return temp;
    }
    //分离内存
    int memdt(void* ptr)
    {
        if(shmdt(ptr) < 0)
        {
            perror("shmdt");
            return 0;
        }
        return 1;
    }
    //删除内存
    int memctl()
    {
        if(shmctl(_shmid, IPC_RMID, 0) < 0)
        {
            perror("shmctl");
            return 0;
        }
        return 1;
    }


private:
    key_t  _key = 0x0000;
    int _shmid = 0;
};

#endif
