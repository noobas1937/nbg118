//
//  CCThreadManager.cpp
//  IF
//
//  Created by 邹 程 on 13-9-13.
//
//

#include "CCThreadManager.h"
USING_NS_CC;
using namespace std;

#define TOTAL_THREAD 4
#define NET_THREAD 4
static int threadUseCount = 0;

CCThreadManager::CCThreadManager() {
    for (int i=0; i<TOTAL_THREAD+1;i++) {
        char name[16];
        sprintf(name, "ChildThread_%d",i+1);
        m_threadPool.push_back(CCPThread::create(name));
        m_threadPool[i]->registerHandler(this, message_handler_selector(CCThreadManager::handleThreadMessage));
    }
    CCDirector::sharedMainThread()->registerHandler(this, message_handler_selector(CCThreadManager::handleThreadMessage));
//    pthread_mutex_init(&m_mutexCounter, NULL);
}

CCThreadManager::~CCThreadManager() {
    for (auto it : m_threadPool) {
        it->unregisterHandler(this, message_handler_selector(CCThreadManager::handleThreadMessage));
        delete it;
    }
    m_threadPool.clear();
    CCDirector::sharedMainThread()->unregisterHandler(this, message_handler_selector(CCThreadManager::handleThreadMessage));
//    pthread_mutex_destroy(&m_mutexCounter);
}

static CCThreadManager *_instance = NULL;
CCThreadManager* CCThreadManager::getInstance() {
    if (!_instance) {
        _instance = new CCThreadManager();
    }
    return _instance;
}

bool CCThreadManager::handleThreadMessage(const cocos2d::CCMessage &msg) {
    ThreadInvoker *invoker = (ThreadInvoker*)(msg.m_data);
    if (invoker && invoker->target && invoker->callFunc) {
        ((invoker->target)->*(invoker->callFunc))(invoker->params);
    }
    delete invoker;
    return true;
}

void CCThreadManager::runInNewThread(cocos2d::CCObject *target, cocos2d::SEL_CallFuncO callFunc,cocos2d::CCObject *params) {
    CCMessage msg;
    msg.m_id = 200;
    msg.m_data = new ThreadInvoker(target,callFunc,params);
//    pthread_mutex_lock(&m_mutexCounter);
    int thread = (threadUseCount++)%TOTAL_THREAD;
//    pthread_mutex_unlock(&m_mutexCounter);
    m_threadPool[thread]->postMessage(msg);
}

void CCThreadManager::runInNetThread(cocos2d::CCObject *target, cocos2d::SEL_CallFuncO callFunc,cocos2d::CCObject *params) {
    CCMessage msg;
    msg.m_id = 400;
    msg.m_data = new ThreadInvoker(target,callFunc,params);
    m_threadPool[NET_THREAD]->postMessage(msg);
}

void CCThreadManager::runInMainThread(cocos2d::CCObject *target, cocos2d::SEL_CallFuncO callFunc,cocos2d::CCObject *params) {
    CCMessage msg;
    msg.m_id = 800;
    msg.m_data = new ThreadInvoker(target,callFunc,params);
    CCDirector::sharedMainThread()->postMessage(msg);
}
