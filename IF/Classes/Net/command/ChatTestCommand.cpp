//
//  ChatTestCommand.cpp
//  IF
//
//  Created by xupengzhan on 15-7-23.
//
//

#include "ChatTestCommand.h"

bool ChatTestCommand::handleRecieve(cocos2d::CCDictionary *dict){
    
    CCLOGFUNC("");
    if (dict->valueForKey("cmd")->compare(CHAT_ROOM_CREATE) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        return false;
    }
   
    return true;
}

