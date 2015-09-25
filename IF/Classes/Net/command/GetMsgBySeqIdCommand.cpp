//
//  GetMsgBySeqIdCommand.cpp
//  IF
//
//  Created by 周雨 on 15-5-28.
//
//

#include "GetMsgBySeqIdCommand.h"
#include "MailController.h"
#include "ChatServiceCocos2dx.h"
#include "ChatMailInfo.h"
#include "MailDialogView.h"
#include "PopupViewController.h"
#include "UIComponent.h"
#include "ChatController.h"

bool GetMsgBySeqIdCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    CCLOGFUNC("");
    if (dict->valueForKey("cmd")->compare(GET_MSG_BY_SEQ_ID) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
        ChatServiceCocos2dx::postChannelNoMoreData (m_channelType,true);
        ChatServiceCocos2dx::postNoMoreMessage(m_channelType);
#endif
        return false;
    }
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(pStr->getCString());
        return false;
    }
    
    string customName=params->valueForKey("name")->getCString();
    CCArray* array = dynamic_cast<CCArray*>(params->objectForKey("msg"));
    
    if(array==NULL)
    {
        CCLOGFUNC("array==NULL");
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
        ChatServiceCocos2dx::postChannelNoMoreData (m_channelType,true);
        ChatServiceCocos2dx::postNoMoreMessage(m_channelType);
#endif
        return false;
    }
    int requestCount=m_maxSeqId-m_minSeqId;
    int num = array->count();
    
    if(m_channelType==CHANNEL_TYPE_COUNTRY)
    {
        ChatController::getInstance()->isFirstRequestCountry=false;
    }
    
    CCLOGFUNCF("num %d  requestCount:%d",num,requestCount);
    if(num<requestCount)
    {
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
//        ChatServiceCocos2dx::postChannelNoMoreData (m_channelType,true);
        if(num==0)
        {
            ChatServiceCocos2dx::postNoMoreMessage(m_channelType);
            return false;
        }
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
        if(m_channelType==CHANNEL_TYPE_COUNTRY)
            ChatController::getInstance()->canCountryRequest = false;
        else if (m_channelType==CHANNEL_TYPE_ALLIANCE)
            ChatController::getInstance()->canAllianceRequest = false;
#endif
    }
    
    CCDictionary *item = NULL;
    CCArray* chatInfoArr=CCArray::create();
    if(m_channelType==CHANNEL_TYPE_COUNTRY || m_channelType==CHANNEL_TYPE_ALLIANCE)
    {          for (int i=0; i<array->count(); i++) {
            item = _dict(array->objectAtIndex(i));
            if (m_channelType==CHANNEL_TYPE_COUNTRY)
                ChatController::getInstance()->m_chatCountryPool.insert(ChatController::getInstance()->m_chatCountryPool.begin(), ChatInfo(item));
            else if (m_channelType==CHANNEL_TYPE_ALLIANCE)
                 ChatController::getInstance()->m_chatAlliancePool.insert(ChatController::getInstance()->m_chatAlliancePool.begin(), ChatInfo(item));
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
            ChatMailInfo* info=ChatMailInfo::create(ChatInfo(item),false);
            chatInfoArr->addObject(info);
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
            ChatMailInfo* info=ChatMailInfo::create(ChatInfo(item),false);
            chatInfoArr->addObject(info);
#endif
        }
    }
    else if(m_channelType==CHANNEL_TYPE_CHATROOM && MailController::getInstance()->getChatRoomEnable())
    {
        auto search = GlobalData::shared()->mailList.find(m_channelId);
        bool isExistMail=(search != GlobalData::shared()->mailList.end() && m_channelId != "");
        if (!isExistMail) {
            return false;
        }
        
        for (int i=array->count()-1; i>=0; i--) {
            item = _dict(array->objectAtIndex(i));
            MailDialogInfo* dialogInfo=MailController::getInstance()->parseChatRoomDic(item,false,true);
            if (dialogInfo) {
                ChatMailInfo* info=ChatMailInfo::create(dynamic_cast<MailInfo*>(search->second),dialogInfo,false,CHANNEL_TYPE_CHATROOM);
                chatInfoArr->addObject(info);
            }
        }
    }
#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    ChatController::getInstance()->notifyMsgToIOS(chatInfoArr, m_channelType, m_channelId);
#endif
    ChatController::getInstance()->notifyMsgToAndroid(chatInfoArr, m_channelType, m_channelId,customName);
    return true;
}