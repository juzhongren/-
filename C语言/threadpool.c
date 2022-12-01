#include "threadpool.h"
#include <pthread.h>

//任务结构体
typedef struct Task{
    void (*function)(void* arg);
    void *arg;
} Task;

// 线程结构体
struct ThreadPool
{
    /* data */
    //任务队列
    Task* taskQ;
    int queueCapacity;   //容量
    int queueSize;        //当前任务个数  
    int queueFront;     //队头->取数据
    int queueRear;      //队尾->放数据

    pthread_t managerID;    //管理者线程ID
    pthread_t *threadIDs;   //工作的线程ID

    int minNum;             //最小线程数
    int maxNum;
    int busyNum;            //忙的线程数
    int liveNum;            //存活的线程数
    int exitNum;            //要销毁的线程个数

    pthread_mutex_t mutexPool;     //锁整个线程
    pthread_mutex_t mutexBusy;   //   锁busyNum变量
    pthread_cond_t notFull;     //      任务队列是不是满了
    pthread_cond_t notEmpty;    //任务队列是不是空了

    int shutdown;           //是不是要销毁，销毁为1，不销毁为0

};




ThreadPool* threadPoolCreate(int min,int max,int queueSize)
{
    ThreadPool* pool = (ThreadPool*)malloc(sizeof(ThreadPool));
    if(pool == NULL)
    {
        printf("malloc threadpool fail...\n");
        return NULL;
    }

    pool->threadIDs = (pthread_t *)malloc(sizeof(pthread_t) * max);
    if(pool->threadIDs == NULL){
        printf("malloc threadIDs fail...\n");
        return NULL;
    }
    memset(pool->threadIDs,0,sizeof(pthread_t) * max);
    pool->minNum = min;
    pool->maxNum = max;
    pool->liveNum = min; //和最小个数相等
    pool->exitNum = 0;

    //初始化信号量和条件量
    if(pthread_mutex_init(&pool->mutexPool,NULL) != 0||
        pthread_mutex_init(&pool->mutexBusy,NULL) != 0||
        pthread_cond_init(&pool->notFull,NULL) != 0  ||
        pthread_cond_init(&pool->notEmpty,NULL) != 0 
        )
        {
            printf("mutex or conda fail\n");
        }
    return NULL;
}
