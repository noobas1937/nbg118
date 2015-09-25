//
//  TestFeedBackCommand.cpp
//  IF
//
//  Created by chenfubi on 15/2/10.
//
//

#include "TestFeedBackCommand.h"
#include "../../controller/TestFeedBackController.h"

bool TestFeedBackCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TESTFEEDBACK_GET_CMD) != 0)
    {
        return false;
    }
    CCDictionary* params = _dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0)
    {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
        return true;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(TESTFEEDBACK_CMDBACK, params);
    return true;
}

bool TestFeedBackSaveCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TESTFEEDBACK_SAVE_CMD) != 0)
    {
        return false;
    }
    return true;
}
