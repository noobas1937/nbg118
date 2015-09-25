//
//  GeneralInfo.h
//  IF
//
//  Created by wangzhenlei on 13-9-22.
//
//

#ifndef __IF__GeneralInfo__
#define __IF__GeneralInfo__

#include "cocos2d.h"
#include "EquipInfo.h"

USING_NS_CC;
using namespace std;

typedef enum{
    FREE, GENERAL_PVE, COUNTRY_BATTLE, RECRUITING
}general_state;

typedef enum{
    COST_WOOD, COST_STONE, COST_IRON, COST_FOOD,COST_MONEY,COST_GOLD,COST_ALLIANCEHONOR,COST_ALLIANCECONTRIBUTION
}PRICE_TYPE;


class GeneralSkillInfo : public CCObject{
public:
    static GeneralSkillInfo *create(std::string skillId, int level, std::string generalUid, std::string abilityId);
    GeneralSkillInfo(std::string skillId, int level, std::string generalUid, std::string abilityId) :
    skillId(skillId)
    , level(level)
    , generalUid(generalUid)
    , abilityId(abilityId)
    {};
    
    ~GeneralSkillInfo(){
        next.clear();
    }

    virtual bool init();
    bool isLocked();
    bool canConnect(std::string skillId);
    int getTotalSkillPoint();
    int getUpgradeSkillPointNeed();
    void parseSkill();

    std::string skillId;
    int level;
    std::string generalUid;
    std::string abilityId;
    int x;
    int y;
    int maxLevel;
    int type;
    int showType;//0百分比 1正常
    vector<float> base;
    vector<float> add;
    vector<std::string> des;
    vector<int> param1;
    vector<std::string> next;
};

class SkillCDInfo : public CCObject{
public:
    static SkillCDInfo *create();
    SkillCDInfo():skillId(string()), uuid(string()), ownerId(string()), startTime(0.0), endTime(0.0),stat(0),effectEndTime(0)
    {};
    
    ~ SkillCDInfo(){}
    virtual bool init();
    void parse(CCDictionary* dict);
    
    std::string uuid;
    std::string ownerId;
    std::string skillId;
    double startTime;
    double endTime;
    int stat;//   0:可用  1:不可用
    double effectEndTime;
};

class GeneralInfo:public cocos2d::CCObject{

public:
    GeneralInfo(CCDictionary* dict);
    //这个用于xml类型的武将，例如武将图鉴面板的武将
    void setXMLGeneralInfo(std::string generalId);
    ~GeneralInfo();
    GeneralInfo():
    uuid(string())
    ,generalId(string())
    ,level(-1)
    ,attack(-1)
    ,defense(-1)
    ,hp(0)
    ,maxForces(-1)
    ,currExp(-1)
    ,maxExp(-1)
    ,pos(0)
    ,state(0)
    ,color(0)
    ,num(0)
    ,rank(0)
    ,isLock(true)
    ,costType(0)
    ,costValue(0)
    {}
    void updateGeneralData(CCDictionary* dict,bool ref = true);
    int getAbilityIndex(std::string abilityId);
    void parseCDTime(CCDictionary* dict);
    string uuid;
    string generalId;
    int level;
    int attack;
    int defense;
    int hp;
    int maxForces;
    int currExp;
    int maxExp;
    int pos;
    int state;
    int open;
    int color;
    int num;
    int rank;
    int costType;//0-5，木石铁粮银金
    int costValue;
    string defaultSkill;
    bool isLock;
    int getPower();
    std::string name;
    std::string getName();
    std::string getDesc();
    std::string getCostName(int type);
    //天赋，能力，技能相关
    vector<std::string> ability;
    vector<std::string> abilityToChoose;
    std::map<std::string, std::map<std::string, GeneralSkillInfo*>> generalSkillMap;
    GeneralSkillInfo *getSkill(std::string abilityId, std::string skillId);
    bool checkHaveStudy(std::string skillId);
    int getUsePoint(std::string abilityId);
    string getAbilityBySkillId(std::string skillId);
};


class AttributeInfo:public cocos2d::CCObject{
    
public:
    AttributeInfo(CCDictionary* dict);
    AttributeInfo():
    aid(0)
    ,area(0)
    ,type(0)
    ,effect("")
    ,show(0)
    ,name("")
    ,icon("")
    ,description("")
    ,bMail(false)
    ,mail_order(0)
    ,bAttack(false)
    ,bDefence(false)
    {}
    
    int aid;
    int area;
    int type;
    string effect;
    int show;
    int buff;
    string name;
    string icon;
    string description;
    
    bool bMail;
    int mail_order;
    bool bAttack;
    bool bDefence;
};
#endif /* defined(__IF__GeneralInfo__) */
