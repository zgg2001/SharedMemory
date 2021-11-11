# SharedMemory
此项目中记录了我在共享内存相关的学习代码

src目录中存放的为相关源码
* Mem1内为`共享内存`相关的测试代码
* Mem2内为`信号量`相关的测试代码
* Mem3内为上两个内容的结合体。其中memcatch为守护进程，记录了运行期间可用内存的最大值和最小值，并存入共享内存。memlook为查看共享内存中的数据，从而获取守护进程统计的数据。
* Mem4内为`有名管道`相关的测试代码。其中使用libevent来监听可读事件。
