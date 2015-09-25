//
//  EquipInfo.cpp
//  IF
//
//  Created by 李锐奇 on 13-10-8.
//
//

#include "EquipInfo.h"

void EquipInfo::parse(cocos2d::CCDictionary* dic){
    uid = dic->valueForKey("uid")->getCString();
    effect = dic->valueForKey("effect1")->getCString();
    itemId = dic->valueForKey("itemId")->getCString();
    count = dic->valueForKey("count")->intValue();
    value = dic->valueForKey("value1")->intValue();
    type = dic->valueForKey("type")->intValue();
    star = dic->valueForKey("star")->intValue();
    priceBuy = dic->valueForKey("price_buy")->intValue();
    priceSell = dic->valueForKey("price_sell")->intValue();
    if(dic->valueForKey("general") != NULL){
        general = dic->valueForKey("general")->getCString();
    }else{
        general = "";
    }
    attr1 = dic->valueForKey("attr1")->getCString();
    attr2 = dic->valueForKey("attr2")->getCString();
    attr3 = dic->valueForKey("attr3")->getCString();
    attr4 = dic->valueForKey("attr4")->getCString();
    
    attrLevel1 = dic->valueForKey("attrLevel1")->intValue();
    attrLevel2 = dic->valueForKey("attrLevel2")->intValue();
    attrLevel3 = dic->valueForKey("attrLevel3")->intValue();
    attrLevel4 = dic->valueForKey("attrLevel4")->intValue();
}