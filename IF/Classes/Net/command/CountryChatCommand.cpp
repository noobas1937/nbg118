//
//  CountryChatCommand.cpp
//  IF
//
//  Created by fubin on 13-9-26.
//
//

#include "CountryChatCommand.h"
#include "ChatController.h"
#include "ChatServiceCocos2dx.h"
#include "ToolController.h"
#include "UIComponent.h"
#include "YesNoDialog.h"

bool CountryChatCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(CHAT_STATE_COUNTRY_COMMAND) == 0||dict->valueForKey("cmd")->compare(CHAT_STATE_ALLIANCE_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        if (params1->objectForKey("errorCode")) {
            
            string errorCode = params1->valueForKey("errorCode")->getCString();
            string banGmName = params1->valueForKey("banGmName")->getCString();
            if (errorCode == "105201") {
                auto tmpT = params1->valueForKey("banTime")->doubleValue()/1000;
                int lastTime = GlobalData::shared()->changeTime(tmpT) - GlobalData::shared()->getWorldTime();
                CCCommonUtils::flyText(_lang_2("105201", banGmName.c_str(),CC_SECTOA(lastTime)));

            }
        }else{
            if(m_post==CHAT_TYPE_INVITE){
                CCCommonUtils::flyHint("", "", _lang("115264"));
                if(params1->objectForKey("gold")){
                    int  gold = params1->valueForKey("gold")->intValue();
                    GlobalData::shared()->playerInfo.gold = gold;
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
                }
            }
            if(m_post==CHAT_TYPE_BATTLE_SHARE || m_post == CHAT_TYPE_DETECT_SHARE){
                CCCommonUtils::flyHint("", "", _lang("115275"));
            }
        }
        return true;
    }
    return false;
}

bool NoticeChatCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(CHAT_STATE_NOTICE_COMMAND) == 0){
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        if (params1->objectForKey("errorCode")) {
            string errorCode = params1->valueForKey("errorCode")->getCString();
            if(!errorCode.empty()){
                if (errorCode == "105334") {
                    
                    auto tmpT = params1->valueForKey("noticeBanTime")->doubleValue()/1000;
                    ChatController::getInstance()->setNoticeBanTime(tmpT);
                    int lastTime = GlobalData::shared()->changeTime(tmpT) - GlobalData::shared()->getWorldTime();
                    CCLOG("errorCode == 105334 lastTime:%d",lastTime);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                    string dialogText=_lang(errorCode).c_str();
                    dialogText.append("\n").append( _lang("105335").c_str()).append(CC_SECTOA(lastTime));
                   YesNoDialog::showYesDialog(dialogText.c_str());
#else
                    YesNoDialog::showTimeWithDes(_lang(errorCode).c_str(), _lang("105335").c_str(), lastTime);
#endif
                }else{
                    CCCommonUtils::flyText(_lang(errorCode));
                }
            }
//            ChatController::getInstance()->backNotice(false);
        }else{
            if(params1->objectForKey("gold")) {
                string pStr = params1->valueForKey("gold")->getCString();
                if(!pStr.empty()){
                    int tmpInt = params1->valueForKey("gold")->intValue();
                    UIComponent::getInstance()->updateGold(tmpInt);
                }
            }
            if(mUsePoint==false){
                if(ToolController::getInstance()->m_toolInfos.find(mItemId)!=ToolController::getInstance()->m_toolInfos.end()){
                    auto &info = ToolController::getInstance()->getToolInfoById(mItemId);
                    info.setCNT(info.getCNT()-1);
                }
            }
//            ChatController::getInstance()->backNotice(true);
        }
        return true;
    }
    return false;
}


bool GetChatCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(CHAT_STATE_GET_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        if (params1->objectForKey("errorCode")) {
             CCLOG("GetChatCommand::handleRecieve errorCode");
            ChatController::getInstance()->receiveNoMessage();
        }
        else {
            ChatController::getInstance()->retRequestChat(params1);
        }
        return true;
    }
    return false;
}