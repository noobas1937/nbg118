//
//  TrainSoldiertimeCommand.h
//  IF
//
//  Created by jiaohongye on 14-2-26.
//
//

#ifndef __IF__TrainSoldiertimeCommand__
#define __IF__TrainSoldiertimeCommand__

#include "CommandBase.h"
#define COMPLETE "army.complete"

class TrainSoldiertimeCommand: public CommandBase {
public:
    TrainSoldiertimeCommand(std::string  itemId, std::string uuid,int buildingKey):
    CommandBase(COMPLETE)
    {
        m_id = itemId;
        m_buildingKey = buildingKey;
        putParam("id", CCString::create(itemId));
        putParam("uuid", CCString::create(uuid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    std::string m_id;
    int m_buildingKey;
};
#endif /* defined(__IF__WorldCommand__) */