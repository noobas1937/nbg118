//
//  CCThreadManager.h
//  IF
//
//  Created by 邹 程 on 13-9-13.
//
//

#ifndef __IF__CCThreadManager__
#define __IF__CCThreadManager__

#include "cocos2d.h"

class CCThreadManager :
public cocos2d::CCObject
{
public:
    static CCThreadManager* getInstance();
    
    void runInNewThread(cocos2d::CCObject *target,cocos2d::SEL_CallFuncO callFunc,cocos2d::CCObject *params = NULL);
    void runInNetThread(cocos2d::CCObject *target,cocos2d::SEL_CallFuncO callFunc,cocos2d::CCObject *params = NULL);
    void runInMainThread(cocos2d::CCObject *target,cocos2d::SEL_CallFuncO callFunc,cocos2d::CCObject *params = NULL);
    
private:
    CCThreadManager();
    ~CCThreadManager();

    CCThreadManager(const CCThreadManager&) = default;
    CCThreadManager& operator=(const CCThreadManager&) = default;
    
    std::vector<cocos2d::CCPThread*> m_threadPool;
    bool handleThreadMessage(const cocos2d::CCMessage &msg);
    
    class ThreadInvoker {
    public:
        ThreadInvoker(cocos2d::CCObject *target
                      ,cocos2d::SEL_CallFuncO callFunc
                      ,cocos2d::CCObject *params
                      ):
        target(target)
        ,callFunc(callFunc)
        ,params(params)
        {}
        cocos2d::CCObject *target;
        cocos2d::SEL_CallFuncO callFunc;
        cocos2d::CCObject *params;
    };
    
//    pthread_mutex_t m_mutexCounter;

};

#endif /* defined(__IF__CCThreadManager__) */
