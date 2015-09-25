//
//  TavernCommand.h
//  IF
//
//  Created by 李锐奇 on 14-3-20.
//
//

#ifndef __IF__TavernCommand__
#define __IF__TavernCommand__

#include "CommandBase.h"
#define GENERAL_GAMBLE "general.rnd"
#define GENERAL_RECRUIT "general.recruit"
#define GENERAL_PICK_COMMAND "general.pick"

#define RECRUIT_CHANGE "recruitChange"

enum GeneralRecruitType{
    RECRUIT_FREE,
    RECRUIT_GOLD,
};

class GeneralRecruitCommand : public CommandBase{
public:
    GeneralRecruitCommand(std::string generalId) : CommandBase(GENERAL_RECRUIT){
        m_generalId = generalId;
        putParam("genId", CCString::create(generalId.c_str()));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    std::string m_generalId;
};

class GamblePickCommand : public CommandBase{
public:
    GamblePickCommand(int index) : CommandBase(GENERAL_PICK_COMMAND){
        putParam("pos", CCInteger::create(index));
        m_pos = index;
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    int m_pos;
};

#endif /* defined(__IF__TavernCommand__) */
