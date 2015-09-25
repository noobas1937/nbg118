//
//  ActivityEventObj.cpp
//  IF
//
//  Created by ganxiaohua on 15-1-5.
//
//

#include "ActivityEventObj.h"
#include "ActivityController.h"

void ActivityEventObj::parse(CCDictionary *dict){
    if(dict==NULL) return ;
    if(dict->objectForKey("startTime")){
        startTime = dict->valueForKey("startTime")->doubleValue() / 1000;
        startTime = GlobalData::shared()->changeTime(startTime);
    }
    if(dict->objectForKey("endTime")){
        endTime = dict->valueForKey("endTime")->doubleValue() / 1000;
        endTime = GlobalData::shared()->changeTime(endTime);
    }
    if(dict->objectForKey("id")){
        id = dict->valueForKey("id")->getCString();
    }
    if(dict->objectForKey("reward")){
        m_reward = dynamic_cast<CCArray*>(dict->objectForKey("reward"));
    }
    if (dict->objectForKey("exchange")){
        exchange = dict->valueForKey("exchange")->getCString();
    }
    if (dict->objectForKey("popup")) {
        popup = dict->valueForKey("popup")->intValue();
    } else {
        popup = -1;
    }
    
    name = CCCommonUtils::getNameById(id);
    desc = _lang(CCCommonUtils::getPropById(id, "desc").c_str());
    descDia = CCCommonUtils::getPropById(id, "desc").c_str();
    desc_info = CCCommonUtils::getPropById(id, "desc_info");
    type = atoi(CCCommonUtils::getPropById(id, "type").c_str());
    sort = atoi(CCCommonUtils::getPropById(id, "sort").c_str());
    activityid = CCCommonUtils::getPropById(id, "activityid");
    Advertise_pic = CCCommonUtils::getPropById(id, "Advertise_pic");
    story = CCCommonUtils::getPropById(id, "story");
    num = CCCommonUtils::getPropById(id, "num");
    if(m_reward == nullptr)
    {
        m_reward = CCArray::create();
    }
    if(type==3){
        if(dict->objectForKey("siegeST")){
            double siegeST = dict->valueForKey("siegeST")->doubleValue()/1000;
            siegeST = GlobalData::shared()->changeTime(siegeST);
            ActivityController::getInstance()->aActivityInfo.siegeST = siegeST;
        }
        if(dict->objectForKey("siegeET")){
            double siegeET = dict->valueForKey("siegeET")->doubleValue()/1000;
            siegeET = GlobalData::shared()->changeTime(siegeET);
            ActivityController::getInstance()->aActivityInfo.siegeET = siegeET;
        }
    }
}

CCDictionary* ActivityEventObj::toDic()
{
    CCDictionary* dic = CCDictionary::create();
    dic->setObject(CCString::create(name), "name");
    dic->setObject(CCString::create(CC_ITOA(type)), "type");
    dic->setObject(CCString::create(CC_ITOA(startTime)), "startTime");
    dic->setObject(CCString::create(CC_ITOA(endTime)), "endTime");
    dic->setObject(CCString::create(id), "id");
    dic->setObject(CCString::create(desc), "desc");
    dic->setObject(CCString::create(descDia), "descDia");
    dic->setObject(CCString::create(desc_info), "desc_info");
    dic->setObject(CCString::create(CC_ITOA(sort)), "sort");
    dic->setObject(CCString::create(activityid), "activityid");
    dic->setObject(CCString::create(Advertise_pic), "Advertise_pic");
    dic->setObject(CCString::create(exchange), "exchange");
    dic->setObject(CCString::create(story), "story");
    dic->setObject(CCString::create(num), "num");
    return dic;
}