//
//  UseSkillCommand.h
//  IF
//
//  Created by ganxiaohua on 14-11-3.
//
//

#ifndef __IF__UseSkillCommand__
#define __IF__UseSkillCommand__

#include "CommandBase.h"
#define USE_SKILL_COMMAND "skill.use"

class UseSkillCommand: public CommandBase {
public:
    UseSkillCommand(std::string skillId,std::string uuid) : CommandBase(USE_SKILL_COMMAND),m_skillId(skillId),m_uuid(uuid){
        putParam("skillId", CCString::create(skillId.c_str()));
        putParam("gUid", CCString::create(uuid.c_str()));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_skillId;
    std::string m_uuid;
};
#endif /* defined(__IF__UseSkillCommand__) */
