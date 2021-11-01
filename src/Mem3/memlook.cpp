/*************************************************************************
	> File Name: memlook.cpp
	> Author: ZHJ
	> Remarks: 
	> Created Time: Mon 01 Nov 2021 05:23:50 PM CST
 ************************************************************************/

#include<iostream>
#include<unistd.h>
#include"semaphore.h"
#include"shared_memory.h"

using namespace std;

int main()
{
    //相关对象
    semaphore *sem = new semaphore((key_t)0x1111);
    shared_memory *mem = new shared_memory((key_t)0x2222);
    int* list;
    //初始化信号灯
    sem->init();
    //获取内存
    mem->getmem(sizeof(int) * 2); //max 和 min

    //等待信号灯挂出
    sem->wait();
    //操作
    list = (int*)mem->getptr();

    printf("最大可用内存：%d bytes / %lf MB\n", list[0], (double)list[0]/1024/1024);
    printf("最小可用内存：%d bytes / %lf MB\n", list[1], (double)list[1]/1024/1024);

    //分离内存
    mem->memdt((void*)list);
    //挂出信号灯
    sem->hangout();

    return 0;
}


