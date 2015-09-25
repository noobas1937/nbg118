//
//  LuaCommand.cpp
//  IF
//
//  Created by xxrdsg on 15-5-12.
//
//

#include "LuaCommand.h"
#include "LuaController.h"

bool LuaCommand::handleRecieve(CCDictionary* dict)
{
    if (dict->valueForKey("cmd")->compare(m_name.c_str()) != 0) {
        return false;
    }
    
    if (!dict->objectForKey("params")) {
        return false;
    }
    
    CCDictionary* params = _dict(dict->objectForKey("params"));
    params->setObject(m_valueDic, "infoDic");
    params->setObject(CCString::create(CC_ITOA(GlobalData::shared()->getTimeStamp())), "addingTime");
    LuaController::getInstance()->backCMD(m_name, params);
    
    return true;
}
