//
//  TrialCommand.h
//  IF
//
//  Created by xxrdsg on 15-7-22.
//
//

#ifndef __IF__TrialCommand__
#define __IF__TrialCommand__

#include "CommandBase.h"

#define TRIAL_START_CMD "trial.start"
#define TRIAL_ATK_CMD "trial.atk"
#define TRIAL_FINISH_CMD "trial.finish"
#define TRIAL_BUY_CMD "trial.buy"

class TrialStartCmd: public CommandBase
{
public:
    TrialStartCmd(int type): m_type(type), CommandBase(TRIAL_START_CMD)
    {
        putParam("type", CCInteger::create(type));//0 免费 1 试炼券 2 金币
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    int m_type;
};

class TrialAtkCmd: public CommandBase
{
public:
    TrialAtkCmd(CCDictionary* dic): CommandBase(TRIAL_ATK_CMD)
    {
        putParam("gold", dic->objectForKey("gold"));
        putParam("ids", dic->objectForKey("ids"));
        auto gold = dynamic_cast<CCBoolean*>(dic->objectForKey("gold"));
        m_gold = gold->getValue();
        m_ids = dic->valueForKey("ids")->getCString();
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    bool m_gold;
    string m_ids;
};

class TrialFinishCmd: public CommandBase
{
public:
    TrialFinishCmd():CommandBase(TRIAL_FINISH_CMD)
    {
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

class TrialBuyCmd: public CommandBase
{
public:
    TrialBuyCmd():CommandBase(TRIAL_BUY_CMD){}
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

#endif /* defined(__IF__TrialCommand__) */
