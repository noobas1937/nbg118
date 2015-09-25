//
//  GeneralManager.h
//  IF
//
//  Created by 李锐奇 on 13-11-27.
//
//

#ifndef __IF__GeneralManager__
#define __IF__GeneralManager__

#include "CommonInclude.h"

#define GENERAL_NUM_CHANGE "generalNumChange"
#define TAVERN_CHANGE "tavernChange"
#define TAVERN_FREE_NUM_CHANGE "tavernFreeNumChange"
#define GENERAL_POS_CHANGE "generalPosChange"

//#define GENERAL_STATE_FREE 0
//#define GENERAL_STATE_DEFENCE 1
//#define GENERAL_STATE_MARCH 2

#define GENERAL_SKILL_UPDATE "generalSkillUpdate"
#define GENERAL_ABILITY_RESET "generalAbilityReset"
#define GENERAL_ABILITY_CHOOSE "generalAbilityChoose"
#define GENERAL_UNLOCK_SKILL_UPDATE "generalUnlockSkillUpdate"
#define SAVE_SKILL_SUCCESS "saveSkillSuccess"
#define SAVE_SKILL_FAIL "saveSkillFAIL"

class GeneralManager : public CCObject{
public:    
    static GeneralManager* getInstance();
    GeneralManager();
    ~GeneralManager();
    void addGeneral(CCDictionary* dic);
    void addGeneral(GeneralInfo& info);
    void removeGeneralByUid(std::string uid);
    
    int getGeneralNumById(std::string generalId);

    //是否曾经获得过武将
    bool isAlreadyGetGeneral(std::string generalId);
    
    std::string getGeneralArmyIcon(std::string generalId);
            
    std::string getGeneralLevelStr(int level);
    
    //获得相对应的半身像plist下标
    int getBustPlistIndex(std::string generalId);
    
    //武将位相关
    int getTotalOpenGeneralPosition();
    //监测武将是否达到上限
    bool isGeneralFull();
    
    void addGeneralAbility(GeneralInfo &generalInfo, std::string itemId);
    void removeGeneralAbility(GeneralInfo &generalInfo, std::string itemId);
    void resetGeneralAbility(GeneralInfo &generalInfo, std::string str);
    int getTotalSkillPoint(GeneralInfo &generalInfo);
    int getTotalUsedSkillPoint(GeneralInfo &generalInfo);
    int getTotalFreeSkillPoint(GeneralInfo &generalInfo);

    int getTotalAbilitySkillPoint(GeneralInfo &generalInfo, std::string abilityId);
    void updateGeneralSkill(GeneralInfo &generalInfo, std::string activityId, std::string skillId, int level);
    
    int getTotalOpenedAbility(GeneralInfo &generalInfo);
    int getNextAbilityOpenLevel(GeneralInfo &generalInfo);
    int getGeneralAbilityNum();
    int getProtectTime();
    void saveSkill(std::string abilityId, std::string skillId);
    void saveSkillFail(std::string abilityId, std::string skillId);
    
    float getValueByEffect(int effect);
    void resetGeneralSkillEffectValue();
    map<int, std::string> defenceGeneral;
    map<int, float> generalSkillEffect;
    bool isSendingResetCommand;
    map<std::string, SkillCDInfo*> SkillCDMap;//主动技能的CD数据
    string getBgBySkill(std::string skillId);
    
    bool isSkillClick(std::string generalUid, std::string skillId);
    void saveSkillClick(std::string generalUid, std::string skillId);
    
    bool isAbilityLevelClick(std::string generalUid, int level);
    void saveAbilityLevelClick(std::string generalUid, int level);
    
    void initSortAttribute();
    
    map<int, AttributeInfo> GeneralAttMap;
    map<int, vector<int>> GeneralAttAreaMap;
};

#endif /* defined(__IF__GeneralManager__) */
