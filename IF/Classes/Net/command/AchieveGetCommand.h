//
//  AchieveGetCommand.h
//  IF
//
//  Created by ganxiaohua on 13-12-24.
//
//

#ifndef __IF__AchieveGetCommand__
#define __IF__AchieveGetCommand__

#include "CommandBase.h"
#define ACHIEVE_GET "achieve.ls"
#define ACHIEVE_GET_REWARD "achieve.reward"
#define MEDAL_SAVE "medal.save"
#define MEDAL_INIT "medal.list"
#define ACHIEVE_FIND "achieve.find"

class AchieveGetCommand: public CommandBase {
public:
    AchieveGetCommand() : CommandBase(ACHIEVE_GET){};
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

class AchieveGetRewardCommand: public CommandBase {
public:
    AchieveGetRewardCommand(std::string itemId) : CommandBase(ACHIEVE_GET_REWARD){
        putParam("id", CCString::create(itemId));
        m_itemId = itemId;
    };
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    std::string m_itemId;
};

class MedalSaveCommand: public CommandBase
{
public:
    MedalSaveCommand(std::string medalId) : CommandBase(MEDAL_SAVE)
    {
        putParam("medal", ccs(medalId));
    };
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

class MedalInitCommand: public CommandBase
{
public:
    MedalInitCommand() : CommandBase(MEDAL_INIT)
    {
        
    };
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

class AchieveFindCmd : public CommandBase
{
public:
    AchieveFindCmd(): CommandBase(ACHIEVE_FIND)
    {
        
    };
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
};

#endif /* defined(__IF__AchieveGetCommand__) */
