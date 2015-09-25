//
//  GetNewMsgCommand.h
//  IF
//
//  Created by 周雨 on 15-5-28.
//
//

#ifndef __IF__GetNewMsgCommand__
#define __IF__GetNewMsgCommand__

#include "CommandBase.h"

#define CHAT_GET_NEW_MSG "chat.get.new"

class GetNewMsgCommand:public CommandBase
{
public:
    // "id|seqid|channelType,id|seqid|channelType,id|seqid|channelType"
    GetNewMsgCommand(std::string allChannelStr) : CommandBase(CHAT_GET_NEW_MSG)
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
    }
    ~GetNewMsgCommand();
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    int m_start;
    int m_count;
    std::string m_groupId;
    bool m_isDialogShowing;
    CCDictionary* m_paramDic;
};

#endif /* defined(__IF__GetNewMsgCommand__) */
