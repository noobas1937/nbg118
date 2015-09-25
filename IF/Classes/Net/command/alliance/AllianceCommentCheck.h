//
//  AllianceCommentCheck.h
//  IF
//
//  Created by ganxiaohua on 15-1-16.
//
//

#ifndef __IF__AllianceCommentCheck__
#define __IF__AllianceCommentCheck__

#include "CommandBase.h"

#define ALLIANCE_COMMENT_CHECK "al.comment.check"

class AllianceCommentCheck: public CommandBase {
public:
    AllianceCommentCheck(std::string allianceId) : CommandBase(ALLIANCE_COMMENT_CHECK){
        putParam("allianceId", CCString::create(allianceId));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__AllianceCommentCheck__) */
