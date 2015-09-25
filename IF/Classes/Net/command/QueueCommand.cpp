//
//  QueueCommand.cpp
//  IF
//
//  Created by fubin on 14-2-20.
//
//

#include "QueueCommand.h"
#include "QueueController.h"

bool QueueOpenCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(QUEUE_OPEN_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        QueueController::getInstance()->endOpenQueue(params1);
        return true;
    }
    return false;
}

bool QueueCCDCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(QUEUE_CCD_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        QueueController::getInstance()->endCCDQueue(params1);
        return true;
    }
    return false;
}

bool QueueFinishCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(QUEUE_FINISH_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        QueueController::getInstance()->endFinishQueue(params1);
        return true;
    }
    return false;
}

bool QueueCancelCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(QUEUE_CANCEL_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        QueueController::getInstance()->endCancelQueue(params1);
        return true;
    }
    return false;
}

bool QueueRentCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(QUEUE_RENT_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        if (params1) {
            int gold = params1->valueForKey("gold")->intValue();
            UIComponent::getInstance()->updateGold(gold);
            CCDictionary *queue = _dict(params1->objectForKey("queue"));
            QueueController::getInstance()->endOpenQueue(queue);
        }
        return true;
    }
    return false;
}

bool QueueRefreshCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(QUEUE_MATE_REFRESH_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        QueueController::getInstance()->endRefreshMateQueue(params1);
        return true;
    }
    return false;
}
