//
//  CKFStreakListCommand.h
//  IF
//
//  Created by ganxiaohua on 15/8/6.
//
//

#ifndef __IF__CKFStreakListCommand__
#define __IF__CKFStreakListCommand__

#include "CommandBase.h"

#define CKF_STREAK_LIST "ckf.streak.list"

class CKFStreakListCommand: public CommandBase {
public:
    CKFStreakListCommand() : CommandBase(CKF_STREAK_LIST){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__CKFStreakListCommand__) */
