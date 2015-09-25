//
//  GuideRecordCommand.h
//  IF
//
//  Created by 李锐奇 on 13-11-13.
//
//

#ifndef __IF__GuideRecordCommand__
#define __IF__GuideRecordCommand__

#include "CommandBase.h"

#define GUIDE_RECORD_COMMAND "stat.tt"

class GuideRecordCommand : public CommandBase{
public:
    GuideRecordCommand(std::string stepId) : CommandBase(GUIDE_RECORD_COMMAND){
        putParam("id", CCString::create(stepId));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

#endif /* defined(__IF__GuideRecordCommand__) */
