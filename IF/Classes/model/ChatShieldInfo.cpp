//
//  ChatShieldInfo.cpp
//  IF
//
//  Created by 李锐奇 on 14-2-8.
//
//

#include "ChatShieldInfo.h"

bool ChatShieldInfo::isShield(std::string uid){
    cocos2d::CCObject* obj;
    CCARRAY_FOREACH(shieldList, obj){
        ShieldInfo* info = dynamic_cast<ShieldInfo*>(obj);
        if(info->uid == uid){
            return true;
        }
    }
    return false;
}

std::string ChatShieldInfo::getUuid(std::string uid){
    cocos2d::CCObject* obj;
    CCARRAY_FOREACH(shieldList, obj){
        ShieldInfo* info = dynamic_cast<ShieldInfo*>(obj);
        if(info->uid == uid){
            return info->uuid;
        }
    }
    return "";
}

ShieldInfo* ChatShieldInfo::getShieldInfo(std::string uuid)
{
    cocos2d::CCObject* obj;
    CCARRAY_FOREACH(shieldList, obj){
        ShieldInfo* info = dynamic_cast<ShieldInfo*>(obj);
        if(info->uuid == uuid){
            return info;
        }
    }
    return NULL;
}

bool ChatShieldInfo::isReachLimit(){
    if(shieldList && shieldList->count() >= maxNum){
        return true;
    }
    return false;
}

void ChatShieldInfo::parse(cocos2d::CCArray* arr){
    shieldList->removeAllObjects();
    cocos2d::CCObject* obj;
    CCARRAY_FOREACH(arr, obj){
        cocos2d::CCDictionary *dict = dynamic_cast<cocos2d::CCDictionary*>(obj);
        if(dict){
            addToList(dict);
        }
    }
}

void ChatShieldInfo::removeFromList(std::string uid){
    cocos2d::CCObject* obj;
    CCARRAY_FOREACH(shieldList, obj){
        ShieldInfo* info = dynamic_cast<ShieldInfo*>(obj);
        if(info->uid == uid){
            shieldList->removeObject(obj);
            break;
        }
    }
}

void ChatShieldInfo::addToList(cocos2d::CCDictionary *dict){
    ShieldInfo *info = new ShieldInfo();
    std::string uid = dict->valueForKey("other")->getCString();
    if(isShield(uid)){
        return;
    }
    info->uid = uid;
    
    std::string uuid = dict->valueForKey("uuid")->getCString();
    info->uuid = uuid;
    
    std::string name = dict->valueForKey("name")->getCString();
    if(name != ""){
        info->name = name;

    }
    
    std::string rank = dict->valueForKey("repLevel")->getCString();
    if(rank != ""){
        info->rank = atoi(rank.c_str());
    }
    std::string pic = dict->valueForKey("pic")->getCString();
    if (pic != "") {
        info->pic = pic;
    }
    info->picVer = dict->valueForKey("picVer")->intValue();
    info->power = dict->valueForKey("power")->intValue();
    std::string abbr = dict->valueForKey("abbr")->getCString();
    if (abbr != "") {
        info->abbr = abbr;
    }

    shieldList->addObject(info);
    info->release();
}