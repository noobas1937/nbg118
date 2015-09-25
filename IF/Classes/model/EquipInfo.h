//
//  EquipInfo.h
//  IF
//
//  Created by 李锐奇 on 13-10-8.
//
//

#ifndef IF_EquipInfo_h
#define IF_EquipInfo_h

#include "cocos2d.h"

class EquipInfo : public cocos2d::CCObject{
public:
    void parse(cocos2d::CCDictionary* dic);
    std::string uid;
    std::string itemId;
    int count;
    std::string general;
    int type;
    int star;
    int priceBuy;
    int priceSell;
    std::string effect;
    int value;
    std::string attr1;
    std::string attr2;
    std::string attr3;
    std::string attr4;
    int attrLevel1;
    int attrLevel2;
    int attrLevel3;
    int attrLevel4;
};

#endif
