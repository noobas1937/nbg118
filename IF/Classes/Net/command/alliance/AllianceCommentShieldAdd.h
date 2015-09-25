//
//  AllianceCommentShieldAdd.h
//  IF
//
//  Created by ganxiaohua on 15-1-15.
//
//

#ifndef __IF__AllianceCommentShieldAdd__
#define __IF__AllianceCommentShieldAdd__

#include "CommandBase.h"

#define ALLIANCE_COMMENT_SHIELD_ADD "al.comment.shield.add"

class AllianceCommentShieldAdd: public CommandBase {
public:
    AllianceCommentShieldAdd(std::string playerUid ,std::string allianceId) : CommandBase(ALLIANCE_COMMENT_SHIELD_ADD){
        if(playerUid!=""){
            putParam("playerUid", CCString::create(playerUid));
        }
        if(allianceId!=""){
            putParam("allianceId", CCString::create(allianceId));
        }
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__AllianceCommentShieldAdd__) */
