//
//  UserApplyListCommand.h
//  IF
//
//  Created by ganxiaohua on 14-4-3.
//
//

#ifndef __IF__UserApplyListCommand__
#define __IF__UserApplyListCommand__
#include "CommandBase.h"
#define USER_APPLY_LIST "al.userapplylist"

class UserApplyListCommand: public CommandBase {
public:
    UserApplyListCommand(int page);
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    int m_page;
};
#endif /* defined(__IF__UserApplyListCommand__) */
