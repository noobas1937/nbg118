//
//  MailDeleteCommand.cpp
//  IF
//
//  Created by 李锐奇 on 13-10-12.
//
//

#include "MailDeleteCommand.h"
#include "MailController.h"
#include "ChatServiceCocos2dx.h"

bool MailDeleteCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(MAIL_DELETE_COMMAND) != 0)
        return false;
 //   CCDictionary* params = _dict(dict->objectForKey("params"));
 //   CCArray* array = dynamic_cast<CCArray*>(params->objectForKey("result"));
//    if(array!=NULL){
 //       int num = array->count();
//        for (int i=0; i<num; i++) {
//            auto dic = _dict(array->objectAtIndex(i));
//            MailController::getInstance()->addMail(dic);
//        }
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_SAVE_LIST_CHANGE);
 //   }
    
    return true;
}
bool MailDialogReadCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(MAIL_DIALOG_READ_COMMAND) != 0)
        return false;

    
    return true;
}

bool MailDialogSaveCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(MAIL_DIALOG_SAVE_COMMAND) != 0)
        return false;
    
    
    return true;
}
bool MailDialogDeleteCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(MAIL_DIALOG_DELETE_COMMAND) != 0)
        return false;
    if (MailController::getInstance()->getIsNewMailListEnable()) {
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
        ChatServiceCocos2dx::deleteMail(m_uid ,CHANNEL_TYPE_OFFICIAL,m_type);
#endif
    }

    
    return true;
}