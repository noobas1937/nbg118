//
//  CCAsyncTask.h
//  ccgoe
//
//  Created by 谢文杰 on 13-1-15.
//
//

#ifndef __ccgoe__CCAsyncTask__
#define __ccgoe__CCAsyncTask__

#include "cocos2d.h"
#include "ExtensionMacros.h"

NS_CC_EXT_BEGIN

class CCAsyncTask: public CCObject {
public:
    CCAsyncTask() {
        pthread_mutex_init(&taskQueueMutex, NULL);
        CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, true);
    }
    ~CCAsyncTask() {
        CCDirector::sharedDirector()->getScheduler()->unscheduleUpdateForTarget(this);
        pthread_mutex_destroy(&taskQueueMutex);
    }
    static CCAsyncTask* sharedTaskManager() {
        static CCAsyncTask* s_sharedDispatcher=NULL;
        if (!s_sharedDispatcher) {
            s_sharedDispatcher=new CCAsyncTask();
        }
        return s_sharedDispatcher;
    }
    static void purgeSharedTaskManager() {
        sharedTaskManager()->release();
    }
    void push(CCCallFunc* task) {
        pthread_mutex_lock(&taskQueueMutex);
        if(0==taskQueueArr.size())
        {
           CCDirector::sharedDirector()->getScheduler()->resumeTarget(this);
        }
        taskQueueArr.pushBack(task);
        pthread_mutex_unlock(&taskQueueMutex);
        
//        if (0 == taskQueue.count())
//            CCDirector::sharedDirector()->getScheduler()->resumeTarget(this);
//        taskQueue.addObject(task);
//        pthread_mutex_unlock(&taskQueueMutex);
    }
    void update(float dt)
    {
        CCCallFunc* task = NULL;
        
        pthread_mutex_lock(&taskQueueMutex);
        
        if (taskQueueArr.size()>0)
        {
            task = taskQueueArr.at(0);
            CC_SAFE_RETAIN(task);
            taskQueueArr.erase(0);
            if (0 == taskQueueArr.size())
                CCDirector::sharedDirector()->getScheduler()->pauseTarget(this);
        }
//        if (taskQueue.count())
//        {
//            task = dynamic_cast<CCCallFunc*>(taskQueue.objectAtIndex(0));
//            CC_SAFE_RETAIN(task);
//            taskQueue.removeObjectAtIndex(0);
//            if (0 == taskQueue.count())
//                CCDirector::sharedDirector()->getScheduler()->pauseTarget(this);
//        }
        pthread_mutex_unlock(&taskQueueMutex);
        
        if (!task)
            return;
        
        task->execute();
        task->release();
    }
    
private:
    Vector<CCCallFunc*> taskQueueArr;
  //  CCArray taskQueue;
    pthread_mutex_t taskQueueMutex;
};

NS_CC_EXT_END

#endif /* defined(__ccgoe__CCAsyncTask__) */
