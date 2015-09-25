//
//  GetAllianceMemberPointCommand.h
//  IF
//
//  Created by 李锐奇 on 14-10-14.
//
//

#ifndef __IF__GetAllianceMemberPointCommand__
#define __IF__GetAllianceMemberPointCommand__

#include "CommandBase.h"

#define GET_MEMBER_POINT "world.al.leader.pt"
#define GET_MEMBER_POINT_BACK "getMemberPointBack"

class GetAllianceMemberPointCommand: public CommandBase {
public:
    GetAllianceMemberPointCommand():
    CommandBase(GET_MEMBER_POINT){
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__GetAllianceMemberPointCommand__) */
