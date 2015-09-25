//
//  TranslateCommand.cpp
//  IF
//
//  Created by wulj on 14-8-12.
//
//

#include "TranslateCommand.h"

bool TranslateCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TRANSLATE_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        if (params1->objectForKey("errorCode")) {
            const CCString *pStr = params1->valueForKey("errorCode");
            if (pStr->compare("")!=0) {
                CCCommonUtils::flyText(_lang(pStr->getCString()));
            }
        }
        else {
            //CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TRANSLATE,CCString::create(params1->valueForKey("translationMsg")->getCString()));
            callSuccess(NetResult::create(Error_OK, params1));
        }
        return true;
    }
    return false;
}
