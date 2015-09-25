//
//  AllianceCommentShieldList.h
//  IF
//
//  Created by ganxiaohua on 15-1-15.
//
//

#ifndef __IF__AllianceCommentShieldList__
#define __IF__AllianceCommentShieldList__

#include "CommandBase.h"

#define ALLIANCE_COMMENT_SHIELD_LIST "al.comment.shield.list"

class AllianceCommentShieldList: public CommandBase {
public:
    AllianceCommentShieldList() : CommandBase(ALLIANCE_COMMENT_SHIELD_LIST){
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__AllianceCommentShieldList__) */
