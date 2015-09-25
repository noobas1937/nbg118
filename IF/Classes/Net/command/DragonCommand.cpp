//
//  DragonCommand.cpp
//  IF
//
//  Created by 张军 on 15/6/8.
//
//

#include "DragonCommand.h"
#include "DragonController.h"

//喂龙
bool DragonFeedCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(DRAGON_CMD_FEED) == 0)
    {
        CCDictionary *params=_dict(dict->objectForKey("params"));
        DragonController::getInstance()->endFeedDragon(params);
        return true;
    }
    else
    {
        return false;
    }
}

//卖龙
bool DragonSellCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(DRAGON_CMD_SELL) == 0)
    {
        CCDictionary *params=_dict(dict->objectForKey("params"));
        DragonController::getInstance()->endSellDragon(params);
        return true;
    }
    else
    {
        return false;
    }
}
