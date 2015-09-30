//
//  ArmyInfo.h
//  IF
//
//  Created by 李锐奇 on 14-2-28.
//
//

#ifndef __IF__ArmyInfo__
#define __IF__ArmyInfo__

#include "cocos2d.h"

class ArmyInfo : public cocos2d::CCObject{
public:
    ArmyInfo(cocos2d::CCDictionary *dict);
    ArmyInfo():power(1.0),train(0),march(0){};
    int range;
    std::string armyId;
    int healSilver;
    int healRes;
    int healTime;
    int time;
    int free;
    float upkeep;
    int defen;
    int move;
    int attack;
    int defence;
    int pve;
    int wood;
    int food;
    int silver;
    int iron;
    int stone;
    int level;
    int armyLevel;
    int load;
    float power;
    int speed;
    time_t finishTime;
    int train;
    bool isArmy;
    int march;
    int health;
    int unlockLevel;//解锁等级
    void setFinishiTime(cocos2d::CCDictionary *dict);
    void setEndTime(double time, bool isCancel = false);
    void setArmyInfo(cocos2d::CCDictionary *dict);
    void update();
    std::string getRealItemId();
    std::string getName();
    std::string getIcon();
    std::string getBodyIcon();
    std::string getModelName();
    std::string getModelTexName();
    std::string getModelAniName();
    std::string getHeadIcon();
    std::string getDesc();
    std::string getAttrIcon(int index);
    void getAttrDesc(int index, std::vector<std::string> &descVec);
    
};

#endif /* defined(__IF__ArmyInfo__) */
