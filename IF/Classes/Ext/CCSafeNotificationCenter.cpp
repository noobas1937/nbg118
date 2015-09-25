//
//  CCSafeNotificationCenter.cpp
//  ccgoe
//
//  Created by 谢文杰 on 12-11-14.
//
//

#include "CCSafeNotificationCenter.h"
#include "CCTypesExt.h"
#include <string>

using namespace std;

NS_CC_EXT_BEGIN

static CCSafeNotificationCenter * s_sharedNotifCenter=NULL;

CCSafeNotificationCenter::CCSafeNotificationCenter()
{}

CCSafeNotificationCenter::~CCSafeNotificationCenter()
{
    for (ObserversMap::const_iterator it = m_observers.begin(); it != m_observers.end(); ++it) {
        it->second->release();
    }
    m_observers.clear();
}

CCSafeNotificationCenter *CCSafeNotificationCenter::sharedNotificationCenter(void)
{
    if (!s_sharedNotifCenter)
    {
        s_sharedNotifCenter = new CCSafeNotificationCenter;
    }
    return s_sharedNotifCenter;
}

void CCSafeNotificationCenter::purgeNotificationCenter(void)
{
    CC_SAFE_DELETE(s_sharedNotifCenter);
    s_sharedNotifCenter=NULL;
}

//
// internal functions
//
bool CCSafeNotificationCenter::observerExisted(CCObject *target,const char *name)
{
    const pair<ObserversMap::const_iterator,ObserversMap::const_iterator> &range=m_observers.equal_range(name);
    for (ObserversMap::const_iterator it2=range.first; it2!=range.second; ++it2) {
        if (it2->second->getTarget()==target)
            return true;
    }
    return false;
}

//
// observer functions
//
void CCSafeNotificationCenter::addObserver(CCObject *target,
                                       SEL_CallFuncO selector,
                                       const char *name,
                                       CCObject *obj)
{
    if (this->observerExisted(target, name))
        return;
    

    CCNotificationObserver *observer = new CCNotificationObserver(target, selector, name, obj);
    if (!observer)
        return;
    
    m_observers.insert(pair<string,CCNotificationObserver*>(name,observer));
    //CCLOG("CCSafeNotificationCenter::addObserver %s added with target 0x%X, observers:%d", name, target, m_observers.size());
}

void CCSafeNotificationCenter::removeObserver(CCObject *target,const char *name)
{
    const pair<ObserversMap::iterator,ObserversMap::iterator> &range=m_observers.equal_range(name);
    for (ObserversMap::iterator it2=range.first; it2!=range.second; ++it2) {
        if (it2->second->getTarget()==target) {
            it2->second->release();
            m_observers.erase(it2);
           // CCLOG("CCSafeNotificationCenter::removeObserver %s removed with target 0x%X", name, target);
            return;
        }
    }
}

void CCSafeNotificationCenter::postNotification(const char *name, CCObject *object)
{
   
    const pair<ObserversMap::const_iterator,ObserversMap::const_iterator> &range=m_observers.equal_range(name);
    Vector<CCNotificationObserver*> array;
    for (ObserversMap::const_iterator it2=range.first; it2!=range.second;) {
        if (it2->second == NULL) {
            continue;
        }
        array.pushBack(it2->second);
        ++it2;
    }
    for(auto child : array)
    {
        CCNotificationObserver* observer =child;
        if (observer && (observer->getSender() == object || observer->getSender() == NULL || object == NULL)) {
            //CCLOG("CCSafeNotificationCenter::postNotification perform %s with target 0x%X", name, observer);
            observer->performSelector(object);
        }
    }
    
//    for (ObserversMap::const_iterator it2=range.first; it2!=range.second;) {
//        if (it2->second == NULL) {
//            continue;
//        }
//        CCNotificationObserver* observer =it2->second;
//        ++it2;
//        if(observer->getName()!=name){
//            CCLOG("CCNotificationObserver::name:%s , postNotification::%s",observer->getName().c_str(),name);
//        }
//        if (observer && (observer->getSender() == object || observer->getSender() == NULL || object == NULL)) {
//            //CCLOG("CCSafeNotificationCenter::postNotification perform %s with target 0x%X", name, observer);
//            observer->performSelector(object);
//        }
//    }
}

NS_CC_EXT_END

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#ifdef __cplusplus
extern "C" {
#endif
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
	void Java_org_cocos2dx_ext_Native_nativePostNotification(JNIEnv* env, jobject thiz, jstring msg)
	{
        string ret=cocos2d::JniHelper::jstring2string(msg);
        CCLOGFUNCF(ret.c_str());
        cocos2d::extension::CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(ret.c_str());
	}
    
    void Java_org_cocos2dx_ext_Native_nativePostNotificationArgs(JNIEnv* env, jobject thiz, jstring msgName,jstring jsonMsg)
	{
        string msgId=cocos2d::JniHelper::jstring2string(msgName);
        string jsonStr=cocos2d::JniHelper::jstring2string(jsonMsg);
        CCLOG("CCSafeNotificationCenter::msgId:%s Json:%s", msgId.c_str(), jsonStr.c_str());
        
        CCLOGFUNCF(msgId.c_str());
        cocos2d::extension::CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(msgId.c_str(),cocos2d::CCString::create(jsonStr));
	}
#ifdef __cplusplus
}
#endif
#endif
