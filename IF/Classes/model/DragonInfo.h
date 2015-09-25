//
//  DragonInfo.h
//  IF
//
//  Created by 张军 on 15/6/5.
//
//

#ifndef __IF__DragonInfo__
#define __IF__DragonInfo__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class DragonInfo : public cocos2d::CCObject {
public:
    DragonInfo(cocos2d::CCDictionary *dict);
    DragonInfo():
    dragonId(0),
    uuid(""),
    des(""),
    name(""),
    level(1),
    exp(1),
    starLevel(1),
    statas(0),
    time_need(0),
    attribute(""),
    production(0),
    foodCost(0),
    buyCost(0),
    place(""),
    icon(""),
    pic(""),
    saveDict(NULL)
    {};

    int dragonId;
    string uuid;
    string des;         //描述
    string name;        //名字
    int level;          //等级
    int exp;            //经验
    int starLevel;      //星级.稀有度
    int statas;         //孵化状态，合成中、蛋、孵化中、龙
    int time_need;      //孵化时间
    string attribute;      //属性
    int production;     //产量
    int foodCost;       //消耗饲料
    int buyCost;        //买蛋价格
    int sellPrice;      //出售价格
    string place;       //存放地
    
    string icon;
    string pic;
    
//    int habitat;//栖息地
//    int skill;//技能
    CCDictionary *saveDict;//保存信息
    
    void SetInfoFromXml(CCDictionary *dict);
    void SetInfoFromServer(CCDictionary *dict);
};
#endif /* defined(__IF__DragonInfo__) */
