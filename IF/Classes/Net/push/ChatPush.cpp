//
//  ChatPush.cpp
//  IF
//
//  Created by fubin on 13-9-26.
//
//

#include "ChatPush.h"
#include "CommonInclude.h"
#include "ChatController.h"

void ChatPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (params == NULL) {
        return;
    }
    auto type = params->valueForKey("type");
//    if(type && type->intValue() == 2){
//        std::string nameStr = params->valueForKey("name")->getCString();
//        auto generals = dynamic_cast<CCArray*>(params->objectForKey("generals"));
//        int i = 0;
//        while(i < generals->count()){
//            std::string generalId = dynamic_cast<CCString*>(generals->objectAtIndex(i))->getCString();
//            CCDictionary* dictInfo = _dict(LocalController::shared()->DBXMLManager()->getObjectByKey(generalId));
//            
//            std::string content = _lang("102319").append(" |").append(nameStr).append("#1| ")
//            .append(_lang("102320")).append(" |")
//            .append(CCCommonUtils::getNameById(generalId))
//            .append("#").append(dictInfo->valueForKey("color")->getCString());
//            
//            CCDictionary* dict1 = CCDictionary::create();
//            dict1->setObject(CCString::create(""), "senderUid");
//            dict1->setObject(CCString::create("2"), "type");
//            dict1->setObject(CCString::create(_lang("105305")), "senderName");
//            dict1->setObject(CCString::create(content), "msg");
//            dict1->setObject(CCDouble::create(GlobalData::shared()->getWorldTime()), "time");
//            ChatController::getInstance()->receiveChat(dict1);
//            i++;
//        }
//    }else{
        std::string uid = params->valueForKey("senderUid")->getCString();
        int post = params->valueForKey("post")->intValue();
        if (post==CHAT_TYPE_INVITE) {
            std::string msg = params->valueForKey("msg")->getCString();
            if(params->objectForKey("inviteAlliance")){
                std::string inviteAlliance = params->valueForKey("inviteAlliance")->getCString();
                msg = _lang_1("115261",inviteAlliance.c_str());
                //把原来的替换了
                params->setObject(CCString::create(msg), "msg");
                params->setObject(CCString::create(""), "translationMsg");
                
            }
            double worldTime = GlobalData::shared()->getWorldTime();
            worldTime *= 1000;
            worldTime = GlobalData::shared()->renewTime(worldTime);
            auto time = worldTime+1000*59;
            CCCommonUtils::flySystemUpdateHint(time, true,msg,FLY_HINT_ALLIANCE_INVITE);
        }
        if(params && params->objectForKey("type")){
            int type = params->valueForKey("type")->intValue();
            if(post!=CHAT_TYPE_NOTICE){
                if(!GlobalData::shared()->chatShieldInfo.isShield(uid)){
                    ChatController::getInstance()->receiveChat( params );
                }
            }else{
                ChatController::getInstance()->receiveChat( params );
            }
        }
//    }
    return;
}
void ChatBanPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (params == NULL) {
        return;
    }
    string uid = params->valueForKey("uid")->getCString();
    time_t time  = params->valueForKey("banTime")->doubleValue()/1000;
    if(time>0){
        time = GlobalData::shared()->changeTime(time);
    }
    string banName = params->valueForKey("banGmName")->getCString();
    
    for(vector<ChatInfo>::iterator it = ChatController::getInstance()->m_chatCountryPool.begin(); it != ChatController::getInstance()->m_chatCountryPool.end(); ++it){
        if((*it).uid==uid){
            (*it).banGmName=banName;
            (*it).banTime=time;
        }
    }
    
    for(vector<ChatInfo>::iterator it = ChatController::getInstance()->m_chatAlliancePool.begin(); it != ChatController::getInstance()->m_chatAlliancePool.end(); ++it){
        if((*it).uid==uid){
            (*it).banGmName=banName;
            (*it).banTime=time;
        }
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHAT_BAN);
    return;
}