/*************************************************************************
	> File Name: memcatch.cpp
	> Author: ZHJ
	> Remarks: 此文件执行任务
	> Created Time: Sat 30 Oct 2021 07:56:07 PM CST
 ************************************************************************/

#include<iostream>
#include<unistd.h>
#include<sys/sysinfo.h>
#include"semaphore.h"
#include"shared_memory.h"

using namespace std;

int main()
{
    //获取程序运行时的内存消耗
    struct sysinfo s_info;
    //相关对象
    semaphore *sem = new semaphore((key_t)0x1111);
    shared_memory *mem = new shared_memory((key_t)0x2222);
    long long* list;
    //初始化信号灯
    sem->init();
    //获取内存
    mem->getmem(sizeof(long long) * 2); //max 和 min
    //此内存是否被初始化
    bool init = false;

    while(1)
    {
        //等待信号灯挂出
        sem->wait();
        //操作
        list = (long long*)mem->getptr();
        
        //获取当前消耗内存
        int error = sysinfo(&s_info);
        if(init == false && list[0] == 0 && list[1] == 0)
        {
            list[0] = s_info.freeram;
            list[1] = s_info.freeram;
            init = true;
        }
        else
        {
            if(list[0] < s_info.freeram)
            {
                list[0] = s_info.freeram;
            }
            else if(list[1] > s_info.freeram)
            {
                list[1] = s_info.freeram;
            }
        }
        
        //分离内存
        mem->memdt((void*)list);
        //挂出信号灯
        sem->hangout();
        sleep(60);
    }

    return 0;
}
