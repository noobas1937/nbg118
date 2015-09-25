//
//  SacrificeInfo.h
//  IF
//
//  Created by lifangkai on 14-11-19.
//
//

#ifndef __IF__SacrificeInfo__
#define __IF__SacrificeInfo__

#include "cocos2d.h"
//#include "PlayerInfo.h"
USING_NS_CC;

class SacrificeInfo : public CCObject{
public:
    CREATE_FUNC(SacrificeInfo);
    void parse(CCDictionary *dict);
    SacrificeInfo():mPrayCount(0),wood(0),food(0),iron(0),silver(0),steel(0),goldwood(0),goldfood(0),goldiron(0),goldsilver(0),goldsteel(0),isFirstFree(0),isDataReturn(true),mPrayCountFromItem(0),mPrayCountMax(0),mPrayCountFree(0),isSteelOpen(false){};
    ~SacrificeInfo(){};
    time_t freshTime;
    
    int wood;
    int food;
    int iron;
    int silver;
    int steel;
    int goldwood;
    int goldfood;
    int goldiron;
    int goldsilver;
    int goldsteel;
    int num;
    int hint;
    int resource;
    int isFirstFree;
    bool isDataReturn;
    bool isSteelOpen;
    void setPrayCountFromItem(const int count);
    bool canPray();
    bool canFreePray();
    int getLeftFreeCount();
private:
    int mPrayCountFromItem;
    int mPrayCountFree;
    int mPrayCountMax;
    int mPrayCount;
    
    virtual bool init(){return true;};
};

#endif /* defined(__IF__SacrificeInfo__) */
