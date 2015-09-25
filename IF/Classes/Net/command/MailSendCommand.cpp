//
//  MailSendCommand.cpp
//  IF
//
//  Created by 李锐奇 on 13-10-28.
//
//

#include "MailSendCommand.h"
#include "MailController.h"
#include "ToolController.h"
#include "ChatServiceCocos2dx.h"

bool MailSendCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(MAIL_SEND) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        //CCCommonUtils::flyText();
        if(m_showTip==true){
            CCCommonUtils::flyHint("", "", (_lang(pStr->getCString()).c_str()));
        }
    }else{
        if(m_type == MAIL_MOD_SEND || m_type == MAIL_MOD_PERSONAL){
            params->setObject(CCString::create(CC_ITOA(MAIL_MOD_PERSONAL)), "type");
        }else{
            params->setObject(CCString::create(CC_ITOA(MAIL_SELF_SEND)), "type");
        }
        double t = GlobalData::shared()->getWorldTime();
        t *= 1000;
        
        t = GlobalData::shared()->renewTime(t);
        params->setObject(CCString::create(CC_ITOA(t)), "createTime");
        params->setObject(CCString::create(""), "rewardId");
        params->setObject(CCString::create(m_name), "fromName");
        params->setObject(CCString::create(CC_ITOA(0)), "save");
        params->setObject(CCString::create(CC_ITOA(0)), "rewardStatus");
        params->setObject(CCString::create(m_contents), "contents");
        params->setObject(CCString::create(CC_ITOA(0)), "itemIdFlag");
        params->setObject(CCString::create(m_title), "title");
        params->setObject(CCString::create(CC_ITOA(1)), "status");
        
        string toUid = params->valueForKey("toUid")->getCString();
        params->setObject(CCString::create(toUid), "fromUid");
        if(m_showTip==true){
            if(m_thanksMail.empty()){
                //新版邮件里，送信成功不提示
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                if(!(ChatServiceCocos2dx::enableNativeMail && GlobalData::shared()->android_native_chat == 1))
                {
                    CCCommonUtils::flyHint("", _lang("105552"), _lang_1("105553", m_name.c_str()));
                }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                if(!(ChatServiceCocos2dx::enableNativeChat))
                {
                    CCCommonUtils::flyHint("", _lang("105552"), _lang_1("105553", m_name.c_str()));
                }
#endif
            }else{
                CCCommonUtils::flyHint("", "", _lang("101389"));
            }
        }
        if(m_mailUid==""){
            m_mailUid = toUid;
        }
        
        std::string uid = params->valueForKey("uid")->getCString();
        auto search = GlobalData::shared()->mailList.find(toUid);
        if(search != GlobalData::shared()->mailList.end() && uid != ""){
            GlobalData::shared()->mailList[uid] = search->second;
            search->second->uid = uid;
            GlobalData::shared()->mailList.erase(search);
            m_mailUid = uid;
        }
        
        bool isChat = false;
        map<string, MailInfo*>::iterator it;
        for(it = GlobalData::shared()->mailList.begin(); it != GlobalData::shared()->mailList.end(); it++){
            if(!it->second)
                continue;
            
            if(m_type == MAIL_MOD_PERSONAL || m_type == MAIL_MOD_SEND){
                if((it->second->type == MAIL_MOD_PERSONAL || it->second->type == MAIL_MOD_SEND) && it->second->fromUid==toUid){
                    MailController::getInstance()->addOneDialogToMailEnd(params,it->second->uid);
                    isChat = true;
                    break;
                }
            }else{
                if(it->second->tabType==0 && it->second->fromUid==toUid){
                    if(it->second->type != MAIL_MOD_PERSONAL && it->second->type != MAIL_MOD_SEND){
                        MailController::getInstance()->addOneDialogToMailEnd(params,it->second->uid);
                        isChat = true;
                        break;
                    }
                }
            }
//
//            if((it->second->tabType==0 )&&it->second->fromUid==toUid){
//                MailController::getInstance()->addOneDialogToMailEnd(params,it->second->uid);
//                isChat = true;
//                break;
//            }
        }
        if(!isChat)
            MailController::getInstance()->addOneDialogToMailEnd(params,m_mailUid);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_SEND_SUCCESS);
    }
    return true;
}

bool MailGiftSendCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(MAIL_GIFT_SEND) != 0)
        return false;
    
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        //CCCommonUtils::flyText();
        CCCommonUtils::flyHint("", "", (_lang(pStr->getCString()).c_str()));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_SEND_GIFT_SUCCESS,CCInteger::create(0));
    }else{
        auto &tool = ToolController::getInstance()->getToolInfoById(m_consum_id);
        if(tool.getCNT()>0){
            tool.setCNT(tool.getCNT()-1);
        }
        auto &tool1 = ToolController::getInstance()->getToolInfoById(m_giftid);
        if(params->objectForKey("remainCount")){
            tool1.setCNT(params->valueForKey("remainCount")->intValue());
        }else{
            if(tool1.getCNT()>0){
                if(m_giftcount>0){
                    tool1.setCNT(tool1.getCNT()-m_giftcount);
                }else{
                    tool1.setCNT(tool1.getCNT()-1);
                }
            }
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_SEND_GIFT_SUCCESS,CCInteger::create(1));
        CCCommonUtils::flyHint("", "", _lang("101384"));
    }
    if (m_consum_id == ITEM_SEND_GIFTALLIANCE) {
        return true;
    }
    
    params->setObject(CCString::create(CC_ITOA(MAIL_SELF_SEND)), "type");
    double t = GlobalData::shared()->getWorldTime();
    t *= 1000;
    
    t = GlobalData::shared()->renewTime(t);
    params->setObject(CCString::create(CC_ITOA(t)), "createTime");
    params->setObject(CCString::create(""), "rewardId");
    if(m_uids.empty()){
        params->setObject(CCString::create(m_name), "fromName");
    }else{
        params->setObject(CCString::create(GlobalData::shared()->playerInfo.name), "fromName");
    }
    params->setObject(CCString::create(CC_ITOA(0)), "save");
    params->setObject(CCString::create(CC_ITOA(0)), "rewardStatus");
    params->setObject(CCString::create(m_contents), "contents");
    params->setObject(CCString::create(CC_ITOA(0)), "itemIdFlag");
    params->setObject(CCString::create(""), "title");
    params->setObject(CCString::create(CC_ITOA(1)), "status");
    string toUid = params->valueForKey("toUid")->getCString();
    params->setObject(CCString::create(toUid), "fromUid");
    
    std::string uid = params->valueForKey("uid")->getCString();
    
    bool isChat = false;
    map<string, MailInfo*>::iterator it;
    for(it = GlobalData::shared()->mailList.begin(); it != GlobalData::shared()->mailList.end(); it++){
        if(!it->second)
            continue;
        if(it->second->tabType==0 && it->second->fromUid==toUid){
            if(it->second->type != MAIL_MOD_PERSONAL && it->second->type != MAIL_MOD_SEND){
                uid = it->second->uid;
                MailController::getInstance()->addOneDialogToMailEnd(params,it->second->uid);
                isChat = true;
                break;
            }
        }
    }
    if(!isChat){
        MailController::getInstance()->addOneDialogToMailEnd(params,uid);
    }
    
    map<string, MailInfo*>::iterator itr = GlobalData::shared()->mailList.find(uid);
    if(itr != GlobalData::shared()->mailList.end()){
        itr->second->unreadDialogNum+=1;
        GlobalData::shared()->mailCountObj.perR+=1;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
    }
    return true;
}
