//
//  TrainSoldierCommand.h
//  IF
//
//  Created by jiaohongye on 14-2-20.
//
//

#ifndef __IF__TrainSoldierCommand__
#define __IF__TrainSoldierCommand__

#include "CommandBase.h"
#define SOLDIER "army.add"
class TrainSoldierCommand: public CommandBase {
public:
    TrainSoldierCommand(std::string itemId, int num,bool useGold):
    CommandBase(SOLDIER)
    {
        m_id = itemId;
        m_num = num;
        m_useGold = useGold;
        putParam("id", CCString::create(itemId));
        putParam("num", CCInteger::create(num));
        putParam("gold", CCBoolean::create(useGold));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    std::string m_id;
    int m_num;
    bool m_useGold;
    void parseResult(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__WorldCommand__) */

