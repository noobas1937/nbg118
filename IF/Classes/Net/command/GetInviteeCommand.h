//
//  GetInviteeCommand.h
//  IF
//
//  Created by xupengzhan on 15-5-20.
//
//

#ifndef __IF__GetInviteeCommand__
#define __IF__GetInviteeCommand__

#include "CommandBase.h"

class GetInviteeCommand: public CommandBase {
public:
    GetInviteeCommand();
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    void onGetAllianceInfoSuccess(CCObject* pObj);
};

#endif /* defined(__IF__GetInviteeCommand__) */
