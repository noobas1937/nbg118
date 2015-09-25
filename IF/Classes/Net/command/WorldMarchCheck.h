//
//  WorldMarchCheck.h
//  IF
//
//  Created by ganxiaohua on 15-1-15.
//
//

#ifndef __IF__WorldMarchCheck__
#define __IF__WorldMarchCheck__

#include "CommandBase.h"

#define WORLD_MARCH_CHECK "world.march.check"

class WorldMarchCheck: public CommandBase {
public:
    WorldMarchCheck(int point, int cityType, int wantToFight=0) : CommandBase(WORLD_MARCH_CHECK){
        putParam("point", CCInteger::create(point));
        putParam("cityType", CCInteger::create(cityType));
        putParam("wantToFight", CCInteger::create(wantToFight));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_nickName;
};
#endif /* defined(__IF__WorldMarchCheck__) */
