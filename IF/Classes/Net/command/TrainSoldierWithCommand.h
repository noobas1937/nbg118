//
//  TrainSoldierWithCommand.h
//  IF
//
//  Created by 李锐奇 on 14-3-4.
//
//

#ifndef __IF__TrainSoldierWithCommand__
#define __IF__TrainSoldierWithCommand__

#include "CommandBase.h"
#define TRAIN_WITH_GOLD_COMMAND "army.cd"

class TrainSoldierWithCommand: public CommandBase {
public:
    TrainSoldierWithCommand(std::string itemId, int num, int max = 0):
    CommandBase(TRAIN_WITH_GOLD_COMMAND)
    {
        m_id = itemId;
        putParam("id", CCString::create(itemId));
        putParam("num", CCInteger::create(num));
        if(max == 1){
            putParam("max", CCInteger::create(max));
        }
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    std::string m_id;
    void parseResult(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__TrainSoldierWithCommand__) */
