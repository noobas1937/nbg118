//
//  CKFNextRoundCommand.h
//  IF
//
//  Created by ganxiaohua on 15/7/1.
//
//

#ifndef __IF__CKFNextRoundCommand__
#define __IF__CKFNextRoundCommand__

#include "CommandBase.h"

#define CKF_NEXT_ROUND "ckf.next.round"

class CKFNextRoundCommand: public CommandBase {
public:
    CKFNextRoundCommand() : CommandBase(CKF_NEXT_ROUND){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__CKFNextRoundCommand__) */
