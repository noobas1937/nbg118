//
//  PushInit.cpp
//  IF
//
//  Created by 邹 程 on 13-9-13.
//
//

#include "InitCommand.h"
#include "CommonInclude.h"
#include "SceneController.h"
#include "MailController.h"
#include "NetController.h"
#include "PopupViewController.h"
#include "FunBuildController.h"
#include "FunBuildInfo.h"
#include "QuestController.h"
#include "GeneralManager.h"
#include "PayController.h"
#include "GCMRewardController.h"
#include "ChatController.h"
#include "WorldController.h"
#include "ChatShieldInfo.h"
#include "QueueController.h"

#include "EnemyInfoController.h"
#include "ToolController.h"
#include "TroopsController.h"

#include "CityCrashView.h"
#include "TrainInfo.h"
#include "TalentController.h"
#include "ActivityController.h"
#include "ScienceController.h"
#include "ActivityController.h"
#include "SocialController.h"

bool InitCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    CCLOG("InitCommand::handleRecieve");
    
    if (!dict)
        return false;
    
    if (dict->valueForKey("cmd")->compare(INIT_COMMAND) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params)
        return false;
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {;
        return false;
    }

    if (params) {
        CCCommonUtils::parseData(params);
    }
    return true;
}

