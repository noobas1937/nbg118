//
//  CrossServiceActivityCommand.h
//  IF
//
//  Created by ganxiaohua on 15/6/4.
//
//

#ifndef __IF__CrossServiceActivityCommand__
#define __IF__CrossServiceActivityCommand__

#include "CommandBase.h"

#define CROSS_SERVER_ACCESS "ckf.access"

class CrossServiceActivityCommand: public CommandBase {
public:
    CrossServiceActivityCommand(std::string itemUUid) : CommandBase(CROSS_SERVER_ACCESS){
         putParam("itemUUid", CCString::create(itemUUid));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    void onNext(float d);
};
#endif /* defined(__IF__CrossServiceActivityCommand__) */
