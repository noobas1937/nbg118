//
//  GeneralCommand.h
//  IF
//
//  Created by 李锐奇 on 14-3-10.
//
//

#ifndef __IF__GeneralCommand__
#define __IF__GeneralCommand__

#include "CommandBase.h"
#define GENERAL_FIRE_COMMAND "general.fire"
#define GENERAL_ABILITY_CHOOSE_COMMAND "ability.confirm"
#define GENERAL_ABILITY_RESET_COMMAND "ability.random"
#define GENERAL_SKILL_SAVE_COMMAND "skill.save"
#define GENERAL_LEVEL_UP_COMMAND "g.lv.up"
#define GENERAL_SKILL_RESET "skill.clear"

class GeneralLevelUpCommand: public CommandBase {
public:
    GeneralLevelUpCommand(string uid):
    CommandBase(GENERAL_LEVEL_UP_COMMAND){
        putParam("uuid", CCString::create(uid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

class GeneralFireCommand: public CommandBase {
public:
    GeneralFireCommand(string uid):
    CommandBase(GENERAL_FIRE_COMMAND){
        putParam("uuid", CCString::create(uid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

class GeneralAbilityChooseCommand : public CommandBase{
public:
    GeneralAbilityChooseCommand(std::string uid, int index, std::string abilityId):
    CommandBase(GENERAL_ABILITY_CHOOSE_COMMAND){
        putParam("gUid", CCString::create(uid));
        putParam("aIndex", CCInteger::create(index));//0, 1
    }

protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

class GeneralAbilityResetCommand : public CommandBase{
public:
    GeneralAbilityResetCommand(std::string uid, std::string abilityId,int index = -1, std::string reset = ""):
    CommandBase(GENERAL_ABILITY_RESET_COMMAND){
        
        putParam("gUid", CCString::create(uid));
        if(index != -1){
            putParam("index", CCInteger::create(index));
        }
        if(reset != ""){
            putParam("reset", CCString::create(reset));
        }
        m_uid = uid;
        m_abilityId = abilityId;
        m_index = index;
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_uid;
    std::string m_abilityId;
    int m_index;
};

class GeneralSkillSaveCommand : public CommandBase{
public:
    GeneralSkillSaveCommand(std::string uid, std::string abilityId, CCDictionary *dic,bool flag=false,string skillId=""):
    CommandBase(GENERAL_SKILL_SAVE_COMMAND),m_abilityId(abilityId),m_flag(flag),m_skillId(skillId){
        putParam("gUid", CCString::create(uid));
        putParam("abilityId", CCString::create(abilityId));
        putParam("skill", dic);
    }

protected:
    bool m_flag;
    string m_abilityId;
    string m_skillId;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

class GeneralSkillResetCommand : public CommandBase{
public:
    GeneralSkillResetCommand(std::string uid, std::string abilityId, std::string itemId=""):
    CommandBase(GENERAL_SKILL_RESET),m_itemId(itemId){
        putParam("gUid", CCString::create(uid));
        //putParam("abilityId", CCString::create(abilityId));
        if(itemId!=""){
            putParam("itemUUid", CCString::create(m_itemId));
        }
    }
    
protected:
    string m_itemId;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};
#endif /* defined(__IF__GeneralCommand__) */
