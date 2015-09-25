//
//  IFShakeCmd.cpp
//  IF
//
//  Created by wangdianzhen on 15/8/19.
//
//

#include "IFShakeCmd.h"
bool IFShakeCmd::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(SHAKECOMMAND) != 0)
        return false;
    auto params = _dict(dict->objectForKey("params"));
    ShakeController::getInstance()->resetShakeMonsterData(params);
    ShakeController::getInstance()->setcanshake(true);
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
    }
    return true;
}