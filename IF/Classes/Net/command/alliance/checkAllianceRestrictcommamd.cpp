//
//  checkAllianceRestrictcommamd.cpp
//  IF
//
//  Created by 李朝辉 on 15-8-11.
//
//

#include "checkAllianceRestrictcommamd.h"
#include "NetController.h"

static string COMMAND_RESTRICTION = "al.attr";

checkAllianceRestrictcommamd::checkAllianceRestrictcommamd()
: CommandBase(COMMAND_RESTRICTION)
{
    
}

bool checkAllianceRestrictcommamd::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(COMMAND_RESTRICTION.c_str()) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
    }else{
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}
