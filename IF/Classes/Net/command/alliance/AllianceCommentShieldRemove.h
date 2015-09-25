//
//  AllianceCommentShieldRemove.h
//  IF
//
//  Created by ganxiaohua on 15-1-15.
//
//

#ifndef __IF__AllianceCommentShieldRemove__
#define __IF__AllianceCommentShieldRemove__

#include "CommandBase.h"

#define ALLIANCE_COMMENT_SHIELD_REMOVE "al.comment.shield.remove"

class AllianceCommentShieldRemove: public CommandBase {
public:
    AllianceCommentShieldRemove(std::string uuid) : CommandBase(ALLIANCE_COMMENT_SHIELD_REMOVE){
        putParam("uuid", CCString::create(uuid));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__AllianceCommentShieldRemove__) */
