//
//  CKFRewardInfoCommand.h
//  IF
//
//  Created by ganxiaohua on 15/6/30.
//
//

#ifndef __IF__CKFRewardInfoCommand__
#define __IF__CKFRewardInfoCommand__

#include "CommandBase.h"

#define CKF_REWARD_INFO "ckf.reward.info"

class CKFRewardInfoCommand: public CommandBase {
public:
    CKFRewardInfoCommand() : CommandBase(CKF_REWARD_INFO){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__CKFRewardInfoCommand__) */
