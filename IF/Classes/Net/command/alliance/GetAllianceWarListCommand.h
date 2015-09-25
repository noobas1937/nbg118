//
//  GetAllianceWarListCommand.h
//  IF
//
//  Created by ganxiaohua on 14-4-16.
//
//

#ifndef __IF__GetAllianceWarListCommand__
#define __IF__GetAllianceWarListCommand__

#include "CommandBase.h"
#define ALLIANCE_WAR_TEAM_LIST "alliance.team.ls"

class GetAllianceWarListCommand: public CommandBase {
public:
    GetAllianceWarListCommand();
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};
#endif /* defined(__IF__GetAllianceWarListCommand__) */
