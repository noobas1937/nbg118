//
//  CityDefenceCommand.h
//  IF
//
//  Created by 李锐奇 on 14-5-26.
//
//

#ifndef __IF__CityDefenceCommand__
#define __IF__CityDefenceCommand__

#include "CommandBase.h"
#define CITY_DEFENCE_CMD "general.def.ch"

class CityDefenceCommand: public CommandBase {
public:
    CityDefenceCommand(std::string src, std::string dst) : CommandBase(CITY_DEFENCE_CMD){
        putParam("src", CCString::create(src));
        if(dst != ""){
            putParam("dst", CCString::create(dst));
        }
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__CityDefenceCommand__) */
