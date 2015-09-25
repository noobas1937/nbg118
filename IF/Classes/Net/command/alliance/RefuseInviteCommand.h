//
//  RefuseInviteCommand.h
//  IF
//
//  Created by ganxiaohua on 14-4-3.
//
//

#ifndef __IF__RefuseInviteCommand__
#define __IF__RefuseInviteCommand__

#include "CommandBase.h"

#define REFUSE_INVITE "al.refuseinvite"

class RefuseInviteCommand: public CommandBase {
public:
    RefuseInviteCommand(std::string Id);
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    std::string m_Id;
};
#endif /* defined(__IF__RefuseInviteCommand__) */
