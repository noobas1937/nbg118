//
//  FunBuildCommand.cpp
//  IF
//
//  Created by fubin on 13-11-4.
//
//

#include "FunBuildCommand.h"
#include "FunBuildController.h"
#include "QueueController.h"

#include "DragonBuildingController.h"
#include "ToolController.h"
#include "GCMRewardController.h"
#include "RewardController.h"

bool FunBuildCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(PALACE_UP_COMMAND) == 0)
    {
        CCDictionary *params2=_dict(dict->objectForKey("params"));
        FunBuildController::getInstance()->endUpFunBuild(params2, _gold);
        return true;
    }
    else if (dict->valueForKey("cmd")->compare(DRAGON_UP_COMMAND) == 0){
        CCDictionary *params2=_dict(dict->objectForKey("params"));
        DragonBuildingController::getInstance()->endUpFunBuild(params2, _gold);
        return true;
    }
    else if (dict->valueForKey("cmd")->compare(PALACE_CD_COMMAND) == 0)
    {
        CCDictionary *params3=_dict(dict->objectForKey("params"));
        FunBuildController::getInstance()->retCostCD(params3);
        return true;
    }
    else if (dict->valueForKey("cmd")->compare(PALACE_GET_COMMAND) == 0)
    {
        CCDictionary *params4=_dict(dict->objectForKey("params"));
        FunBuildController::getInstance()->endGetFunBuildProduct(params4);
        if (params4->objectForKey("reward")) {
            auto rewards = dynamic_cast<CCArray*>(params4->objectForKey("reward"));
            if (rewards) {
                GCMRewardController::getInstance()->flyToolReward(GCMRewardController::getInstance()->getFlyArr(rewards));
                std::string rewardStr = RewardController::getInstance()->retReward(rewards);
                //CCCommonUtils::flyHint("", "", rewardStr);
            }
        }
        return true;
    }
    else if (dict->valueForKey("cmd")->compare(DRAGON_DFOOD_COMMAND) == 0||dict->valueForKey("cmd")->compare(DRAGON_DGOLD_COMMAND) == 0)
    {
        CCDictionary *params4=_dict(dict->objectForKey("params"));
        DragonBuildingController::getInstance()->endGetFunBuildProduct(params4);
        return true;
    }
    else if (dict->valueForKey("cmd")->compare(PALACE_DES_COMMAND) == 0)
    {
        CCDictionary *params5=_dict(dict->objectForKey("params"));
        FunBuildController::getInstance()->endDestroyBuild(params5);
        return true;
    }
    else
    {
        return false;
    }
}

bool FunBuildCreate::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(PALACE_CT_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        FunBuildController::getInstance()->endOpenFunBuild(params1, _gold);
        QueueController::getInstance()->initQueueData(params1);
        return true;
    }else if (dict->valueForKey("cmd")->compare(DRAGON_CT_COMMAND) == 0){
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        DragonBuildingController::getInstance()->endOpenFunBuild(params1, _gold);
        QueueController::getInstance()->initQueueData(params1);
        return true;
    }
    else
    {
        return false;
    }
}

bool FunBuildMove::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(PALACE_MOVE_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        FunBuildController::getInstance()->EndMoveBuild(params1);
        return true;
    }
    else
    {
        return false;
    }
}

bool TileOpenCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(PALACE_OPEN_TILE) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        FunBuildController::getInstance()->endOpenTile(params1);
        return true;
    }
    else
    {
        return false;
    }
}

bool PlantDragonFood::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(DRAGON_PLANT_COMMAND) == 0)
    {
        CCDictionary *params=_dict(dict->objectForKey("params"));
        DragonBuildingController::getInstance()->endPlantFood(params);
        return true;
    }else{
        return false;
    }
}
