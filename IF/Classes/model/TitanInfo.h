//
//  TitanInfo.h
//  IF
//
//  Created by xxrdsg on 15-2-5.
//
//

#ifndef __IF__TitanInfo__
#define __IF__TitanInfo__

#include "cocos2d.h"
#include "cocos-ext.h"




#include "LocalController.h"




USING_NS_CC;
USING_NS_CC_EXT;

const char * const MSG_TITAN_INFORMATION_RESET = "MSG_TITAN_INFORMATION_RESET"; //泰坦刷新数据
const char * const MSG_TITAN_STATUS_CHANGE = "MSG_TITAN_STATUS_CHANGE";//泰坦状态改变
const char * const MSG_TITAN_FEED_SPEED_UP_COMPLETE = "MSG_TITAN_FEED_SPEED_UP_COMPLETE"; //加速完成

class TitanInfo:
public cocos2d::CCObject
{
public:
    CREATE_FUNC(TitanInfo);
    TitanInfo(CCDictionary* dict);
    TitanInfo():
    feedNum(0),
    level(0),
    currentManual(0),
    exp(0),
    titanId(""),
    status(0),
    feedcd(0),
    nextExp(0),
    feedMaxNum(0),
    feedFoodNum(0),
    uuid("")
    {};
    
    int resetTitanInfo(CCDictionary* dict);// 0没有改变 1数值改变(页面需要刷新的数值) 2状态改变
    
    int feedNum;
    int level;
    int currentManual;
    int exp;
    std::string titanId;//对应士兵ID
    int status;//泰坦状态 0在家
    long feedcd;
    int nextExp;//升级所需经验值
    int feedMaxNum;//每天最多喂食几次
    int feedFoodNum; //每次喂食的粮食个数
    int maxManual;//对应级别的体力上限
    std::string uuid ;
private:
    virtual bool init(){return true;};
    
};


#endif /* defined(__IF__PlayerSearchInfo__) */
