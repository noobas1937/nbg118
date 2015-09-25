//
//  CKFGetTopInfoCommand.h
//  IF
//
//  Created by ganxiaohua on 15/7/3.
//
//

#ifndef __IF__CKFGetTopInfoCommand__
#define __IF__CKFGetTopInfoCommand__

#include "CommandBase.h"

#define CKF_GET_TOP_INFO "ckf.get.top.info"

class CKFGetTopInfoCommand: public CommandBase {
public:
    CKFGetTopInfoCommand(string type) : CommandBase(CKF_GET_TOP_INFO){
        putParam("type", CCString::create(type));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__CKFGetTopInfoCommand__) */
