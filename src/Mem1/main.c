/*************************************************************************
	> File Name: main.c
	> Author: ZHJ
	> Remarks: 共享内存测试 
	> Created Time: Mon 25 Oct 2021 09:14:45 AM CST
 ************************************************************************/

#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdio.h>
#include<unistd.h>

int main()
{
    int shmid;
    key_t key = 0x2222;//key值

    //shmget 获取/申请内存
    if((shmid = shmget(key, sizeof(int), 0777|IPC_CREAT)) < 0)
    {
        perror("shmget");
        return -1;
    }

    //shmat 令指针指向获取的内存(shmid)
    int* pid = 0;
    pid = (int*)shmat(shmid, 0, 0);
    perror("shmat");
    printf("pid address : %p\n", pid);
   
    //操作 更改内存中的内容
    printf("before:%d\n", *pid);
    *pid = getpid();
    printf("after:%d\n", *pid);

    //分离此内存 即解除映射引用
    shmdt(pid);
    
    /*
    //rm此块共享内容
    if(shmctl(shmid, IPC_RMID, 0) == -1)
    {
        perror("shmctl");
        return -2;
    }
    */

    return 0;
}
