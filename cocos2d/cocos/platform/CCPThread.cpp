//
//  CCPThread.cpp
//  IF
//
//  Created by 邹 程 on 13-9-6.
//
//

#include "CCPThread.h"

NS_CC_BEGIN

CCPThread::CCPThread(const char *name) {
    int ret = 0;
    ret = pthread_mutex_init(&m_mutexInvoker, NULL);
    CCAssert(ret == 0, "CCPThread mutex invoker init failed!");
    ret = pthread_mutex_init(&m_mutexMessage, NULL);
    CCAssert(ret == 0, "CCPThread mutex message init failed!");
    ret = pthread_cond_init(&m_condMessage, NULL);
    CCAssert(ret == 0, "CCPThread cond message init failed!");
    
    if (strlen(name) > PTHREAD_NAME_MAX || !name) {
        name = "NoneNameThread";
    }
    memset(m_name, 0, PTHREAD_NAME_MAX + 1);
    strcpy(m_name, name);
}

CCPThread::~CCPThread() {
    pthread_mutex_destroy(&m_mutexInvoker);
    pthread_mutex_destroy(&m_mutexMessage);
    pthread_cond_destroy(&m_condMessage);
}

CCPThread* CCPThread::create(const char *name) {
    pthread_attr_t threadAttr;
    pthread_attr_init(&threadAttr);
    pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);
    
    auto ccpThread = new CCPThread(name);
    int result = pthread_create(&ccpThread->m_pThreadId, &threadAttr, CCPThread::loop,ccpThread);
    if (result != 0) {
        CC_SAFE_DELETE(ccpThread);
        CCLOG("CCPThread::create returns: %d", result);
    }
    return ccpThread;
}

void* CCPThread::loop(void *pArgs) {
    while (true) {
        if (hasMessage((CCPThread*)pArgs)) {
            processMessage((CCPThread*)pArgs);
        }
    }
    return NULL;
}

bool CCPThread::hasMessage(CCPThread *pThread) {
    CCAssert(pThread != NULL, "Null Thread , call hasMessage error!");
    pthread_mutex_lock(&pThread->m_mutexMessage);
    while (pThread->m_messageStack.empty()) {
        pthread_cond_wait(&pThread->m_condMessage, &pThread->m_mutexMessage);
    }
    pthread_mutex_unlock(&pThread->m_mutexMessage);
    return true;
}

void CCPThread::processMessage(CCPThread *pThread) {
    CCAssert(pThread != NULL, "Null Thread , call processMessage error!");
    while (true) {
        int ret = -1;
        pthread_mutex_lock(&pThread->m_mutexMessage);
        if (pThread->m_messageStack.empty()) {
            pthread_mutex_unlock(&pThread->m_mutexMessage);
            return;
        }
        auto message = pThread->m_messageStack.pop();
        pthread_mutex_unlock(&pThread->m_mutexMessage);
        
        pthread_mutex_lock(&pThread->m_mutexInvoker);
        ret = pThread->m_invokerStack.traverseBreakAtFirstProcess(processInternal, &message);
        pthread_mutex_unlock(&pThread->m_mutexInvoker);
        
        if (ret == -1) {
            CCLOG("Message [%d] is NOT processed by thread [%s]!!!",message.m_id,pThread->m_name);
        }
    }
}

void CCPThread::registerHandler(const Ref *pTarget, const CCMessageHandler handler) {
    if (!pTarget || !handler) {
        return;
    }
    Invoker invoker(pTarget,handler);
    pthread_mutex_lock(&m_mutexInvoker);
    if (!m_invokerStack.haveElement(invoker)) {
        m_invokerStack.push(invoker);
    }
    pthread_mutex_unlock(&m_mutexInvoker);
}

void CCPThread::unregisterHandler(const Ref *pTarget, const CCMessageHandler handler) {
    if (!pTarget || !handler) {
        return;
    }
    Invoker invoker(pTarget,handler);
    pthread_mutex_lock(&m_mutexInvoker);
    if (m_invokerStack.haveElement(invoker)) {
        m_invokerStack.removeElement(invoker);
    }
    pthread_mutex_unlock(&m_mutexInvoker);
}

void CCPThread::postMessage(const CCMessage &msg) {
    pthread_mutex_lock(&m_mutexMessage);
    m_messageStack.push(msg);
    pthread_cond_signal(&m_condMessage);
    pthread_mutex_unlock(&m_mutexMessage);
}

NS_CC_END
