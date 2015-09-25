//
//  MailBlockCommand.cpp
//  IF
//
//  Created by 李锐奇 on 14-2-28.
//
//

#include "MailBlockCommand.h"

bool MailBlockCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(MAIL_BLOCK_COMMAND) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
    }else{
        
    }
    
    return true;
}