//
//  AllianceCallHelpCommand.h
//  IF
//
//  Created by ganxiaohua on 14-7-17.
//
//

#ifndef __IF__AllianceCallHelpCommand__
#define __IF__AllianceCallHelpCommand__

#include "CommandBase.h"

#define ALLIANCE_AL_CALL_HELP "al.callhelp"
#define ALLIANCE_MARK_LINE_CMD "leader.rally.point"

class AllianceCallHelpCommand: public CommandBase {
public:
    AllianceCallHelpCommand(std::string queueId,int type,std::string itemId) : CommandBase(ALLIANCE_AL_CALL_HELP),m_queueId(queueId),m_type(type),m_itemId(itemId){
        putParam("queueId", CCString::create(m_queueId.c_str()));
        putParam("qType", CCInteger::create(m_type));
        putParam("itemId", CCString::create(m_itemId.c_str()));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_queueId;
    int m_type;
    std::string m_itemId;
};

class AllianceMarkLineCommand: public CommandBase {
public:
    AllianceMarkLineCommand(int rallyPoint) : CommandBase(ALLIANCE_MARK_LINE_CMD){
        putParam("rallyPoint", CCInteger::create(rallyPoint));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};
#endif /* defined(__IF__AllianceCallHelpCommand__) */
