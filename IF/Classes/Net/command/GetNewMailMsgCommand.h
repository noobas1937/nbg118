//
//  GetNewMailMsgCommand.h
//  IF
//
//  Created by xupengzhan on 15-7-15.
//
//

#ifndef __IF__GetNewMailMsgCommand__
#define __IF__GetNewMailMsgCommand__

#include "CommandBase.h"

#define CHAT_GET_NEW_MAIL_MSG "chat.get.new.mails"

class GetNewMailMsgCommand:public CommandBase
{
public:
    // "id|seqid|channelType,id|seqid|channelType,id|seqid|channelType"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // "id|seqid|channelType,id|seqid|channelType,id|seqid|channelType"
    GetNewMailMsgCommand(std::string allChannelStr,std::string updateMailStr="") : CommandBase(CHAT_GET_NEW_MAIL_MSG)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    // "id|seqid|channelType,id|seqid|channelType,id|seqid|channelType"
    GetNewMailMsgCommand(int commandType,std::string allChannelStr,std::string updateMailStr="") : CommandBase(CHAT_GET_NEW_MAIL_MSG),ios_commandType(commandType)
#endif
    {
        m_paramDic=CCDictionary::create();
        m_paramDic->retain();
        vector<std::string> *allChannel = new vector<std::string>();
        CCCommonUtils::splitString(allChannelStr, ",", *allChannel);
        //        CCArray* arr=CCArray::create();
        for(vector<std::string>::iterator it = allChannel->begin(); it != allChannel->end(); ++it){
            std::string channelStr = *it;
            
            vector<std::string> *channel = new vector<std::string>();
            CCCommonUtils::splitString(channelStr, "|", *channel);
            
            //            CCDictionary *dic = CCDictionary::create();
            //            dic->setObject(CCString::create(channel->at(0)), "id");
            //            dic->setObject(CCString::create(channel->at(1)), "seqid");
            //            dic->setObject(CCInteger::create(atoi(channel->at(2).c_str())), "type");
            //
            //            arr->addObject(dic);
            m_paramDic->setObject(CCString::create(channel->at(1)), channel->at(0));
            delete channel;
        }
        delete allChannel;
        //        putParam("rooms", arr);
        putParam("params", CCString::create(allChannelStr));
        if(updateMailStr!="")
            putParam("updatemailtime", CCString::create(updateMailStr));
        
    }
    ~GetNewMailMsgCommand();
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    int m_start;
    int m_count;
    std::string m_groupId;
    bool m_isDialogShowing;
    CCDictionary* m_paramDic;
    int ios_commandType; //simon
};


#endif /* defined(__IF__GetNewMailMsgCommand__) */
