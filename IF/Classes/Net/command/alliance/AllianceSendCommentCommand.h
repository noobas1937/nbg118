//
//  AllianceSendCommentCommand.h
//  IF
//
//  Created by ganxiaohua on 15-1-15.
//
//

#ifndef __IF__AllianceSendCommentCommand__
#define __IF__AllianceSendCommentCommand__

#include "CommandBase.h"

#define ALLIANCE_AL_COMMENT "al.comment"

class AllianceSendCommentCommand: public CommandBase {
public:
    AllianceSendCommentCommand(std::string allianceId,std::string comment,std::string lang) : CommandBase(ALLIANCE_AL_COMMENT){
        putParam("allianceId", CCString::create(allianceId));
        putParam("comment", CCString::create(comment));
        putParam("lang", CCString::create(lang));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__AllianceSendCommentCommand__) */
