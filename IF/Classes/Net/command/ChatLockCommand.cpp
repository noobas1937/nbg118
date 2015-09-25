//
//  ChatLockCommand.cpp
//  IF
//
//  Created by 李锐奇 on 14-2-8.
//
//

#include "ChatLockCommand.h"
#include "ChatController.h"


bool ChatLockCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(CHAT_LOCK_COMMAND) != 0){
        return false;
    }
    CCDictionary* params = _dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
       // CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
         CCCommonUtils::flyHint("quest_icon_1.png", "", (_lang(pStr->getCString()).c_str()));
    }else{
        CCArray *arr = dynamic_cast<CCArray*>(params->objectForKey("chatShield"));
        CCObject *obj;
        CCARRAY_FOREACH(arr, obj){
            GlobalData::shared()->chatShieldInfo.addToList(_dict(obj));
           CCCommonUtils::flyHint("quest_icon_1.png", _lang("105312"), _lang_1("105319", _dict(obj)->valueForKey("name")->getCString()));

        }
    }
    return true;
}

bool ChatBanCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(CHAT_BAN_COMMAND) != 0){
        return false;
    }
    CCDictionary* params = _dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyHint("quest_icon_1.png", "", (_lang(pStr->getCString()).c_str()));
    }else{
        CCCommonUtils::flyHint("", "", _lang("105209"));
//        string uid = params->valueForKey("uid")->getCString();
//        time_t time  = dict->valueForKey("banTime")->doubleValue()/1000;
//        string banName = params->valueForKey("banGmName")->getCString();
//        
//        for(vector<ChatInfo>::iterator it = ChatController::getInstance()->m_chatCountryPool.begin(); it != ChatController::getInstance()->m_chatCountryPool.end(); ++it){
//            if((*it).uid==uid){
//                (*it).banGmName=banName;
//                (*it).banTime=time;
//            }
//        }
//
//        for(vector<ChatInfo>::iterator it = ChatController::getInstance()->m_chatAlliancePool.begin(); it != ChatController::getInstance()->m_chatAlliancePool.end(); ++it){
//            if((*it).uid==uid){
//                (*it).banGmName=banName;
//                (*it).banTime=time;
//            }
//        }
    }
    return true;
}
bool ChatUnBanCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(CHAT_UNBAN_COMMAND) != 0){
        return false;
    }
    CCDictionary* params = _dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyHint("quest_icon_1.png", "", (_lang(pStr->getCString()).c_str()));
    }else{
        string uid = params->valueForKey("uid")->getCString();
        time_t time  = dict->valueForKey("banTime")->doubleValue()/1000;
        string banName = params->valueForKey("banGmName")->getCString();
        
        for(vector<ChatInfo>::iterator it = ChatController::getInstance()->m_chatCountryPool.begin(); it < ChatController::getInstance()->m_chatCountryPool.end(); ++it){
            if((*it).uid==m_uid){
                (*it).banGmName="";
                (*it).banTime=0;
            }
        }
        
        for(vector<ChatInfo>::iterator it = ChatController::getInstance()->m_chatAlliancePool.begin(); it < ChatController::getInstance()->m_chatAlliancePool.end(); ++it){
            if((*it).uid==uid){
                (*it).banGmName=banName;
                (*it).banTime=time;
            }
        }
        CCCommonUtils::flyHint("", "", _lang("110068"));
    }
    return true;
}