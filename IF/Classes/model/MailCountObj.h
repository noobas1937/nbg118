//
//  MailCountObj.h
//  IF
//
//  Created by lifangkai on 14-7-8.
//
//

#ifndef __IF__MailCountObj__
#define __IF__MailCountObj__
#include "cocos2d.h"
class MailCountObj : public cocos2d::CCObject {
public:
    MailCountObj():
    saveR(0)
    ,sysR(0)
    ,perR(0)
    ,saveT(0)
    ,sysT(0)
    ,perT(0)
    ,total(0)
    {};
    
    int saveR;//
    int sysR;//
    int upR;
    int perR;
    int saveT;
    int sysT;
    int perT;
    int studioT;
    int studioR;
    int fightT;
    int fightR;
    int modT;
    int modR;
    int total;
};

#endif /* defined(__IF__MailCountObj__) */
