//
//  CrossServerScoreCommand.h
//  IF
//
//  Created by ganxiaohua on 15/6/18.
//
//

#ifndef __IF__CrossServerScoreCommand__
#define __IF__CrossServerScoreCommand__

#include "CommandBase.h"

#define CROSS_SERVER_GET_SCORE "ckf.get.score"

class CrossServerScoreCommand: public CommandBase {
public:
    CrossServerScoreCommand(string type="") : CommandBase(CROSS_SERVER_GET_SCORE){
        putParam("type", CCString::create(type));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};

#endif /* defined(__IF__CrossServerScoreCommand__) */
