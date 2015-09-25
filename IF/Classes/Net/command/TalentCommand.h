//
//  TalentCommand.h
//  IF
//
//  Created by 李锐奇 on 14-3-20.
//
//

#ifndef __IF__TalentCommand__
#define __IF__TalentCommand__

#include "CommandBase.h"
#define TALENT_SAVE "talent.save"
#define TALENT_CLEAR "talent.clear"

class TalentSaveCommand: public CommandBase {
public:
    TalentSaveCommand(std::string strData) : CommandBase(TALENT_SAVE){
        putParam("talentData", CCString::create(strData));
    }

protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

class TalentClearCommand: public CommandBase {
public:
    TalentClearCommand(std::string gUUid) : CommandBase(TALENT_CLEAR){
        putParam("goodsUid", CCString::create(gUUid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__TalentCommand__) */
