//
//  DailyActiveItemInfo.cpp
//  IF
//
//  Created by chenfubi on 15/4/10.
//
//

#include "DailyActiveItemInfo.h"

void DailyActiveItemInfo::initData(string itemId)
{
    CCDictionary* dict = LocalController::shared()->DBXMLManager()->getObjectByKey("");
    initData(dict);
}

void DailyActiveItemInfo::initData(cocos2d::CCDictionary *dict)
{
    if (dict->objectForKey("id"))
    {
       itemId = dict->valueForKey("id")->getCString();
    }
    if (dict->objectForKey("accept1"))
    {
        unlockLevel = dict->valueForKey("accept1")->intValue();
    }
    if (dict->objectForKey("name"))
    {
        name = dict->valueForKey("name")->getCString();
        name = _lang(name);
    }
    if (dict->objectForKey("desc"))
    {
        desc = dict->valueForKey("desc")->getCString();
    }
    if (dict->objectForKey("desc2"))
    {
        desc2 = dict->valueForKey("desc2")->intValue();
    }
    if (dict->objectForKey("target"))
    {
        target = dict->valueForKey("target")->getCString();
    }
    if (dict->objectForKey("point"))
    {
        point = dict->valueForKey("point")->intValue();
    }
    if (dict->objectForKey("gotype"))
    {
        gotype = dict->valueForKey("gotype")->intValue();
    }
    if (dict->objectForKey("go"))
    {
        go = dict->valueForKey("go")->intValue();
    }
    if (dict->objectForKey("pic"))
    {
        pic = dict->valueForKey("pic")->getCString();
        pic.append(".png");
    }
    if (dict->objectForKey("icon"))
    {
        icon = dict->valueForKey("icon")->getCString();
        icon.append(".png");
    }
    if (dict->objectForKey("show"))
    {
        isShow = dict->valueForKey("show")->boolValue();
    }
}

void DailyActiveItemInfo::updateData(cocos2d::CCDictionary *dict, bool setIsNew)
{
    if (dict->objectForKey("num"))
    {
        nowNum = dict->valueForKey("num")->intValue();
    }
    
    if (dict->objectForKey("totalNum"))
    {
        totalNum = dict->valueForKey("totalNum")->intValue();
    }
    
    if (dict->objectForKey("times"))
    {
        times = dict->valueForKey("times")->intValue();
    }
    
    if (dict->objectForKey("totalTimes"))
    {
        totalTimes = dict->valueForKey("totalTimes")->intValue();
    }
    
    if (dict->objectForKey("hasCom"))
    {
        hasComplete = dict->valueForKey("hasCom")->boolValue();
    }
    else if (times >= totalTimes)
    {
        hasComplete = true;
        if (setIsNew)
        {
            isNew = true;
        }
    }
    else
    {
        hasComplete = false;
    }
}