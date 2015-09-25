//
//  GetMsgBySeqIdCommand.h
//  IF
//
//  Created by 周雨 on 15-5-28.
//
//

#ifndef __IF__GetMsgBySeqIdCommand__
#define __IF__GetMsgBySeqIdCommand__


#include "CommandBase.h"

#define GET_MSG_BY_SEQ_ID "chat.get.by.seq"

//根据seqid获取历史消息
class GetMsgBySeqIdCommand:public CommandBase
{
public:
    GetMsgBySeqIdCommand(int minSeqId, int maxSeqId, int channelType, std::string channelId):
    m_minSeqId(minSeqId)
    ,m_maxSeqId(maxSeqId)
    ,m_channelType(channelType)
    ,m_channelId(channelId)
    ,CommandBase(GET_MSG_BY_SEQ_ID)
    {
        putParam("start", CCString::create(CC_ITOA(minSeqId)));
        putParam("end", CCString::create(CC_ITOA(maxSeqId)));
        putParam("type", CCInteger::create(channelType));
        putParam("id", CCString::create(channelId));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    int m_minSeqId;
    int m_maxSeqId;
    int m_channelType;
    std::string m_channelId;
};


#endif /* defined(__IF__GetMsgBySeqIdCommand__) */
