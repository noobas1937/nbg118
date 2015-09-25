//
//  TrialCommand.cpp
//  IF
//
//  Created by xxrdsg on 15-7-22.
//
//

#include "TrialCommand.h"
#include "ActivityController.h"

bool TrialStartCmd::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TRIAL_START_CMD) != 0)
        return false;
    GameController::getInstance()->removeWaitInterface();
    auto params = _dict(dict->objectForKey("params"));
    params->setObject(CCString::create(CC_ITOA(m_type)), "type");
    ActivityController::getInstance()->endStartTrialCmd(params);
    return true;
}

bool TrialAtkCmd::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TRIAL_ATK_CMD) != 0)
        return false;
    auto params = _dict(dict->objectForKey("params"));
    ActivityController::getInstance()->endAtkCmd(params);
    return true;
}

bool TrialFinishCmd::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TRIAL_FINISH_CMD) != 0)
        return false;
    GameController::getInstance()->removeWaitInterface();
    auto params = _dict(dict->objectForKey("params"));
    ActivityController::getInstance()->endTrialFinishCmd(params);
    return true;
}

bool TrialBuyCmd::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TRIAL_BUY_CMD) != 0)
        return false;
    GameController::getInstance()->removeWaitInterface();
    auto params = _dict(dict->objectForKey("params"));
    ActivityController::getInstance()->endTrialBuyCmd(params);
    return true;
}