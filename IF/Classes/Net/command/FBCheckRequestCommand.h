//
//  FBCheckRequestCommand.h
//  IF
//
//  Created by ganxiaohua on 15/4/1.
//
//

#ifndef __IF__FBCheckRequestCommand__
#define __IF__FBCheckRequestCommand__

#include "CommandBase.h"

#define FB_FBID_CONFIRM "fbId.confirm"

class FBCheckRequestCommand: public CommandBase {
public:
    FBCheckRequestCommand(string ids) : CommandBase(FB_FBID_CONFIRM){
        putParam("ids", CCString::create(ids));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__FBCheckRequestCommand__) */
