//
//  GetAllianceCommentsCommand.cpp
//  IF
//
//  Created by chenliang on 14-4-11.
//
//

#include "GetAllianceCommentsCommand.h"
#include "NetController.h"

static string COMMAND_NAME = "al.commentview";

GetAllianceCommentsCommand::GetAllianceCommentsCommand(std::string allianceId,int page,std::string lang)
: CommandBase(COMMAND_NAME)
{
    putParam("allianceId", CCString::create(allianceId));
    putParam("page", CCInteger::create(page));
    putParam("lang", CCString::create(lang));
}

bool GetAllianceCommentsCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(COMMAND_NAME.c_str()) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    callSuccess(NetResult::create(Error_OK, (CCObject*)(params->objectForKey("list"))));
    return true;
}