//
//  MailSaveCommand.cpp
//  IF
//
//  Created by 李锐奇 on 14-2-27.
//
//

#include "MailSaveCommand.h"
#include "MailController.h"
#include "ChatServiceCocos2dx.h"

bool MailSaveCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(MAIL_SAVE_COMMAND) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
    }else{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//        ChatServiceCocos2dx::setMailSave(m_uid,m_type,1);
        ChatServiceCocos2dx::setMailSave(m_uid,1);
#endif
    }
    
    return true;
}


bool MailCancelSaveCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(MAIL_CANCEL_SAVE_COMMAND) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
    }else{
        const CCString *uid = params->valueForKey("uid");
        if(uid->compare("")!=0){
            //MailController::getInstance()->removeMail(uid->getCString());
        }
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ChatServiceCocos2dx::setMailSave(m_uid,0);
#endif
    }
    
    return true;
}