//
//  DRConfigCommand.cpp
//  IF
//
//  Created by Émilie.Jiang on 15/7/24.
//
//

#include "DRConfigCommand.h"
#include "DynamicResourceController.h"
bool DRConfigCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(DRConfigGet) != 0)
        return false;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    //test code
    if(params->objectForKey("drconfig")){
        DynamicResourceController::getInstance()->initCommonInfo(params->objectForKey("drconfig"));
    }
    return true;
}