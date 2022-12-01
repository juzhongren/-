#ifndef _THREADPOOL_H
#define _THREADPOOL_H

typedef strut ThreadPool ThreadPool;
//创建线程池并初始化
ThreadPool *threadPoolCreate(int min,int max,int queueSize);
//销毁线程池
//给线程池添加任务
//获取线程池中的工作线程的个数
//获取线程池中活着的线程个数
#endif