//
//  GeAlliancetRankInfoCommand.h
//  IF
//
//  Created by ganxiaohua on 15/4/10.
//
//

#ifndef __IF__GeAlliancetRankInfoCommand__
#define __IF__GeAlliancetRankInfoCommand__

#include "CommandBase.h"

#define MS_GET_TOP_INFO "ms.get.top.info"

class GeAlliancetRankInfoCommand: public CommandBase {
public:
    GeAlliancetRankInfoCommand() : CommandBase(MS_GET_TOP_INFO){

    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__GeAlliancetRankInfoCommand__) */
