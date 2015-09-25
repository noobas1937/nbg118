//
//  SystemUpdatePush.cpp
//  IF
//
//  Created by liweiyu on 14-9-16.
//
//

#include "SystemUpdatePush.h"
#include "CommonInclude.h"

void SystemUpdatePush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary* params = _dict(dict->objectForKey("params"));
    
    if (!params)
        return;

    auto time = params->valueForKey("time")->doubleValue();
    long curTime = GlobalData::shared()->getWorldTime();
    auto m_countDown = GlobalData::shared()->changeTime(time/1000);
    auto m_time = (long)floor( m_countDown - curTime );
    string delayTime = CC_ITOA(m_time/60);
    string tip = CCString::createWithFormat("%s",_lang_1("105324", delayTime.c_str()))->getCString();
    CCCommonUtils::flySystemUpdateHint(time, false,tip,FLY_HINT_LOGIN);
    return;
}