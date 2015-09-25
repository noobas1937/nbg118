//
//  ActExcCommand.cpp
//  IF
//
//  Created by xxrdsg on 15-6-3.
//
//

#include "ActExcCommand.h"
#include "ActivityController.h"

bool GetAct6ExcDataCommand::handleRecieve(CCDictionary* dict)
{
    if (dict->valueForKey("cmd")->compare("act.exchange") != 0) {
        return false;
    }
    if (!dict->objectForKey("params")) {
        return false;
    }
    auto params = _dict(dict->objectForKey("params"));
    ActivityController::getInstance()->endGetAct6Data(params);
    return true;
}

bool Act6ExcCommand::handleRecieve(CCDictionary* dict)
{
    if (dict->valueForKey("cmd")->compare("act.ex.reward") != 0) {
        return false;
    }
    if (!dict->objectForKey("params")) {
        return false;
    }
    auto params = _dict(dict->objectForKey("params"));
    params->setObject(CCString::create(m_tempActId), "tempActId");
    params->setObject(CCInteger::create(m_type), "type");
    ActivityController::getInstance()->endAct6Exchange(params);
    return true;
}

bool Act6RefreshCommand::handleRecieve(CCDictionary* dict)
{
    if (dict->valueForKey("cmd")->compare("act.ex.flush") != 0) {
        return false;
    }
    if (!dict->objectForKey("params")) {
        return false;
    }
    auto params = _dict(dict->objectForKey("params"));
    params->setObject(CCString::create(m_actId), "m_actId");
    ActivityController::getInstance()->endAct6Refresh(params);
    return true;
}