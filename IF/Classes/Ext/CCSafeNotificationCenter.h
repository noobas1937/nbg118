//
//  CCSafeNotificationCenter.h
//  ccgoe
//
//  Created by 谢文杰 on 12-11-14.
//
//

#ifndef __ccgoe__CCSafeNotificationCenter__
#define __ccgoe__CCSafeNotificationCenter__

#include "cocos2d.h"
#include "ExtensionMacros.h"

NS_CC_EXT_BEGIN

class  CCSafeNotificationCenter {
public:
    typedef std::multimap<std::string, NotificationObserver*> ObserversMap;
    
    CCSafeNotificationCenter();
    ~CCSafeNotificationCenter();
    
    static CCSafeNotificationCenter *sharedNotificationCenter(void);
    static void purgeNotificationCenter(void);
    
    void addObserver(CCObject *target,
                     SEL_CallFuncO selector,
                     const char *name,
                     CCObject *obj);
    
    void removeObserver(CCObject *target,const char *name);
    
    void postNotification(const char *name, CCObject *object=NULL);
    
private:
    bool observerExisted(CCObject *target,const char *name);
    
    ObserversMap m_observers;
};

NS_CC_EXT_END

#endif /* defined(__ccgoe__CCSafeNotificationCenter__) */
