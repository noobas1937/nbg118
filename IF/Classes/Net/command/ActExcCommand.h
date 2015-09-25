//
//  ActExcCommand.h
//  IF
//
//  Created by xxrdsg on 15-6-3.
//
//

#ifndef __IF__ActExcCommand__
#define __IF__ActExcCommand__

#include "CommandBase.h"

class GetAct6ExcDataCommand: public CommandBase
{
public:
    GetAct6ExcDataCommand(string ids):CommandBase("act.exchange")
    {
        putParam("ids", CCString::create(ids));
    };
protected:
    virtual bool handleRecieve(CCDictionary*);
};

class Act6ExcCommand: public CommandBase
{
public:
    Act6ExcCommand(string actId, int type): CommandBase("act.ex.reward"), m_tempActId(actId), m_type(type)
    {
        putParam("id", CCString::create(actId));
        putParam("type", CCInteger::create(type));
    }
protected:
    virtual bool handleRecieve(CCDictionary*);
    string m_tempActId;
    int m_type;
};

class Act6RefreshCommand: public CommandBase
{
public:
    Act6RefreshCommand(string actId): CommandBase("act.ex.flush"), m_actId(actId)
    {
        putParam("actId", CCString::create(actId));
    }
protected:
    virtual bool handleRecieve(CCDictionary*);
    string m_actId;
};


#endif /* defined(__IF__ActExcCommand__) */
