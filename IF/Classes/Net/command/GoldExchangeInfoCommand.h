//
//  GoldExchangeInfoCommand.h
//  IF
//
//  Created by lifangkai on 14-9-10.
//
//

#ifndef __IF__GoldExchangeInfoCommand__
#define __IF__GoldExchangeInfoCommand__

#include "CommandBase.h"
#define GOLDEXCHANGE_SHOW_COMMAND "exchange.info"
#define MONTH_CARD_REWARD_COMMAND "mCard.reward"
/*
 *用于矫正用户等级和经验，请求后台刷新
 */
class GoldExchangeInfoCommand: public CommandBase {
public:
    GoldExchangeInfoCommand():
    CommandBase(GOLDEXCHANGE_SHOW_COMMAND){
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

/*
 * 月卡领取
 */
class MonthCardRewardCommand: public CommandBase {
public:
    MonthCardRewardCommand(const string itemId):
    CommandBase(MONTH_CARD_REWARD_COMMAND){
        putParam("itemId", CCString::create(itemId));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};
#endif /* defined(__IF__GoldExchangeInfoCommand__) */
