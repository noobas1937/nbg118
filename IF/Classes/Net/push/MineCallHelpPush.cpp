//
//  MineCallHelpPush.cpp
//  IF
//
//  Created by ganxiaohua on 14-11-24.
//
//

#include "MineCallHelpPush.h"
#include "ActivityController.h"

void MineCallHelpPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    string fbid = params->valueForKey("fId")->getCString();
    string fbName = params->valueForKey("fbName")->getCString();
    if(fbName!=""){
        CCCommonUtils::flyHint("", "", _lang_1("107076", fbName.c_str()));
    }
    if (fbid!="") {
        if (ActivityController::getInstance()->baozangHelpArr && ActivityController::getInstance()->baozangHelpArr->count()>0) {
            int num = ActivityController::getInstance()->baozangHelpArr->count();
            for (int i=0; i<num; i++) {
                auto oneInfo = dynamic_cast<CCDictionary*>(ActivityController::getInstance()->baozangHelpArr->objectAtIndex(i));
                if (oneInfo) {
                    string fId = oneInfo->valueForKey("fId")->getCString();
                    if (fId==fbid) {
                        oneInfo->setObject(CCString::create("1"), "help");
                    }
                }
            }
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FACEBOOK_FRIEND_HELP_CHANGE,params);
    }
}