//
//  GetUpdateMailCommand.cpp
//  IF
//
//  Created by xupengzhan on 15-7-23.
//
//

#include "GetUpdateMailCommand.h"
#include "ChatServiceCocos2dx.h"

bool GetUpdateMailCommand::handleRecieve(cocos2d::CCDictionary *dict){
    
    CCLOGFUNC("");
    if (dict->valueForKey("cmd")->compare(CHAT_GET_UPDATE_MAILS) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        return false;
    }
    
    if (params->objectForKey("data")) {
        string data=params->valueForKey("data")->getCString();
        CCLOGFUNCF("data : %s",data.c_str());
        if (data!="") {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            ChatServiceCocos2dx::postMailUpdate(data);
#endif
        }
    }
    
//    string groupIdStr = params->valueForKey("id")->getCString();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    auto interator = GlobalData::shared()->mailList.find(groupIdStr);
//    
//    if (interator!=GlobalData::shared()->mailList.end()) {
//        MailInfo* mailInfo=dynamic_cast<MailInfo*>(interator->second);
//        if (mailInfo) {
//            ChatServiceCocos2dx::setMailInfo(groupIdStr.c_str(),mailInfo->uid.c_str(),mailInfo->fromName.c_str(),CHAT_ROOM);
//            
//            if(!ChatServiceCocos2dx::isChatShowing){
//                CCLOGFUNC("!ChatServiceCocos2dx::isChatShowing");
//            }
//            else
//
    
    return true;
}
