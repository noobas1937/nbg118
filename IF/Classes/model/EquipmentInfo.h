//
//  EquipmentInfo.h
//  IF
//
//  Created by fubin on 14-12-2.
//
//

#ifndef IF_EquipmentInfo_h
#define IF_EquipmentInfo_h

#include "CommonInclude.h"

class EquipmentInfo : public cocos2d::CCObject {
public:
//   <ItemSpec id="260100" level="1" site="0" color="0" name="104301" description="104302" icon="item701" para1="68" num1="5" show1="101401" para2="1011|1012" num2="5" show2="101402" material1="201010;1" material2="201060;1" cost="1001" time="60"/>
    EquipmentInfo(CCDictionary* dict);
    EquipmentInfo():
    itemId(0)
    ,level(0)
    ,site(0)
    ,color(0)
    ,costLv(0)
    ,name("")
    ,des("")
    ,icon("")
    ,cost("")
    ,time(0)
    ,canCrt(false)
    ,break_down(0)
    ,break_up(0)
    ,showType("")
    ,power(0)
    ,para1("")
    ,num1("")
    ,show1("")
    {};
    
    int itemId;
    int level;
    int site;
    int color;
    int costLv;
    string name;
    string des;
    string icon;
    string cost;
    int time;
    bool canCrt;
    int break_down;
    int break_up;
    string showType;
    int power;
    
    string para1;
    string num1;
    string show1;
    
    map<int, string> showDias;
    map<int, float> values;
    map<int, vector<float>> parasMap;
    vector<string> mateVec;
    map<int, int> mateMap;
    map<int, int> equipMap;
    
    void SetInfoFromServer(CCDictionary* dict);
    
    void setLongProperty(string showProperty);
};

#endif
