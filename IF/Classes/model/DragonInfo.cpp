//
//  DragonInfo.cpp
//  IF
//
//  Created by 张军 on 15/6/5.
//
//

#include "DragonInfo.h"

DragonInfo::DragonInfo(cocos2d::CCDictionary *dict){
    SetInfoFromXml(dict);
}

void DragonInfo::SetInfoFromXml(CCDictionary *dict){
    this->dragonId = dict->valueForKey("id")->intValue();
    this->name = dict->valueForKey("name")->getCString();
    this->level = dict->valueForKey("level")->intValue();
    this->attribute = dict->valueForKey("system")->getCString();//属性
    this->starLevel = dict->valueForKey("star")->intValue();
    this->production = dict->valueForKey("gain")->intValue();//产量
//    dict->valueForKey("limit_level")->getCString();
//    dict->valueForKey("exp")->getCString();
//    dict->valueForKey("pic_order")->getCString();
//    dict->valueForKey("pic_head")->getCString();
    this->saveDict = dict;
//    dict->writeToFile("/Users/zhangjun/Desktop/dictf/DragonInfoXML");
}
void DragonInfo::SetInfoFromServer(CCDictionary *dict){
    this->place = dict->valueForKey("place")->getCString();
    this->exp = dict->valueForKey("exp")->intValue();
    this->dragonId = dict->valueForKey("id")->intValue();
    this->level = dict->valueForKey("level")->intValue();
    this->uuid = dict->valueForKey("uuid")->getCString();
    this->sellPrice = dict->valueForKey("sell")->intValue();
}