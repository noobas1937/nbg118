//
//  LotteryActCommand.h
//  IF
//
//  Created by chenfubi on 15/1/23.
//
//

#ifndef __IF__LotteryActCommand__
#define __IF__LotteryActCommand__

#include "CommandBase.h"
#include "../../controller/LotteryController.h"

#define LOTTERY_START_COMMAND "lottery.start"
#define LOTTERY_CANCEL_COMMAND "lottery.cancel"
#define LOTTERY_BUY_COMMAND "lottery.buy"

class LotteryActCommand: public CommandBase
{
public:
    LotteryActCommand(int position, int useGold = 0, int type = 0):CommandBase(LOTTERY_START_COMMAND)
    {
        m_position = position;
        m_super = LotteryController::shared()->lotteryInfo.superMode;
        m_super = 0;//todocfb超级模式暂时屏蔽
        m_type = type;
        int times = LotteryController::shared()->lotteryInfo.lotteryCnt;
        if (m_type == 2)
        {
            times = LotteryController::shared()->lotteryInfo.lotteryCnt2;
        }
        putParam("type", CCString::createWithFormat("%d", m_type));
        putParam("super", CCString::createWithFormat("%d", m_super));
        putParam("position", CCString::createWithFormat("%d", m_position));
        putParam("times", CCString::createWithFormat("%d", times));
        putParam("useGold", CCString::createWithFormat("%d", useGold));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    bool handleRecieve1(cocos2d::CCDictionary *dict);
    bool handleRecieve2(cocos2d::CCDictionary *dict);
    
private:
    int m_type;
    int m_super;
    int m_position;
};

class LotteryCanCelCmd : public CommandBase
{
public:
    LotteryCanCelCmd():CommandBase(LOTTERY_CANCEL_COMMAND)
    {
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

class LotteryBuyCmd : public CommandBase
{
public:
    LotteryBuyCmd():CommandBase(LOTTERY_BUY_COMMAND)
    {
        putParam("super", CCString::createWithFormat("%d", LotteryController::shared()->lotteryInfo.superMode));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__LotteryActCommand__) */
