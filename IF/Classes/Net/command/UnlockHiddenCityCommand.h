//
//  UnlockHiddenCityCommand.h
//  IF
//
//  Created by 邹 程 on 13-10-14.
//
//

#ifndef __IF__UnlockHiddenCityCommand__
#define __IF__UnlockHiddenCityCommand__

#include "CommandBase.h"
#define UNLOCK_HIDDEN_CITY_COMMAND "scenes.unlock.city"

class UnlockHiddenCityCommand : public CommandBase {
public:
    UnlockHiddenCityCommand(int cityId):
    CommandBase(UNLOCK_HIDDEN_CITY_COMMAND)
    ,m_cityId(cityId)
    {
        putParam("cityId", CCInteger::create(m_cityId));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    int m_cityId;
};

#endif /* defined(__IF__UnlockHiddenCityCommand__) */
