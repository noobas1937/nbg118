//
//  MedalInfo.cpp
//  IF
//
//  Created by chenfubi on 15/4/3.
//
//

#include "MedalInfo.h"

void MedalInfo::initData(cocos2d::CCDictionary *dict)
{
    setXmlData(dict);
}

void MedalInfo::initData(string medalId)
{
    auto itemDict = LocalController::shared()->DBXMLManager()->getObjectByKey(medalId);
    initData(itemDict);
}

void MedalInfo::setXmlData(string medalId)
{
    auto itemDict = LocalController::shared()->DBXMLManager()->getObjectByKey(medalId);
    setXmlData(itemDict);
}

void MedalInfo::setXmlData(cocos2d::CCDictionary *dict)
{
    medalId = dict->valueForKey("id")->intValue();
    name = _lang(dict->valueForKey("name")->getCString());
    icon_main = dict->valueForKey("icon_main")->getCString();
    icon_main.append(".png");
    icon_base = dict->valueForKey("icon_base")->getCString();
    icon_base.append(".png");
    achievement = dict->valueForKey("achievement")->getCString();
    icon_color = dict->valueForKey("icon_sub")->intValue();
}

