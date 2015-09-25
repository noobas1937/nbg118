//
//  GetAllianceMembersCommand.cpp
//  IF
//
//  Created by chenliang on 14-4-4.
//
//

#include "GetAllianceMembersCommand.h"
#include "NetController.h"
#include "MailController.h"
#include "HFUrlImageCache.h"
#include "ChatServiceCocos2dx.h"

static string COMMAND_AL_RANK = "al.rank";

GetAllianceMembersCommand::GetAllianceMembersCommand()
: CommandBase(COMMAND_AL_RANK)
{
    
}

bool GetAllianceMembersCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(COMMAND_AL_RANK.c_str()) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(_lang(pStr->getCString()));
        callFail(NetResult::create());
    }else{
//       callSuccess(NetResult::create(Error_OK, (CCObject*)(params->objectForKey("list"))));
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}