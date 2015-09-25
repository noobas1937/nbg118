//
//  AllianceScienceCommand.cpp
//  IF
//
//  Created by fubin on 14-4-9.
//
//

#include "AllianceScienceCommand.h"
#include "UIComponent.h"
#include "YesNoDialog.h"
#include "FunBuildController.h"
#include "ScienceController.h"

bool AllScienceResearchCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALL_SCIENCE_RESEARCH) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    ScienceController::getInstance()->endResearchAccSc(params);
    return true;
}

bool AllScienceDonateCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALL_SCIENCE_DONATE) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    ScienceController::getInstance()->endDonate(params);
    return true;
}

bool AllScienceDonateRefreshCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALL_SCIENCE_DONATE_REFRESH) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    ScienceController::getInstance()->endDonateRefresh(params);
    return true;
}

bool AllScienceDonateCDCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALL_SCIENCE_DONATE_CD) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    ScienceController::getInstance()->endCDDonate(params);
    return true;
}