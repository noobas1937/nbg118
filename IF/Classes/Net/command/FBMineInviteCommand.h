//
//  FBMineInviteCommand.h
//  IF
//
//  Created by ganxiaohua on 14-11-3.
//
//

#ifndef __IF__FBMineInviteCommand__
#define __IF__FBMineInviteCommand__

#include "CommandBase.h"

#define FB_MINE_INVITE "mine.invite"

class FBMineInviteCommand: public CommandBase {
public:
    FBMineInviteCommand(std::string fbId,std::string friendIds) : CommandBase(FB_MINE_INVITE),m_fbId(fbId),m_friendIds(friendIds){
        putParam("fbId", CCString::create(fbId.c_str()));
        putParam("friend", CCString::create(friendIds.c_str()));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_fbId;
    std::string m_friendIds;
};
#endif /* defined(__IF__FBMineInviteCommand__) */
