//
//  UserPlatformInfo.cpp
//  IF
//
//  Created by liruiqi on 15/6/10.
//
//

#include "UserPlatformInfo.h"

std::string UserPlatformInfo::getBindName(){
    if(bindName == ""){
        std::string key = bindPf + "Name";
        bindName = CCUserDefault::sharedUserDefault()->getStringForKey(key.c_str(), "");
    }
    return bindName;
}

void UserPlatformInfo::setBindName(std::string name){
    bindName = name;
    std::string key = bindPf + "Name";
    CCUserDefault::sharedUserDefault()->setStringForKey(key.c_str(), bindName);
}

void UserPlatformInfo::parse(CCDictionary *dict){
    if(dict){
        bindPf = dict->valueForKey("bindPf")->getCString();
        bindId = dict->valueForKey("bindId")->getCString();
    }
}