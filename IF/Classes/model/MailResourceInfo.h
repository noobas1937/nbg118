//
//  MailResourceInfo.h
//  IF
//
//  Created by lifangkai on 14-10-9.
//
//

#ifndef __IF__MailResourceInfo__
#define __IF__MailResourceInfo__

#include "cocos2d.h"
USING_NS_CC;

class MailResourceInfo : public CCNode{
public:
    static MailResourceInfo* create();
    void parse(CCDictionary* dic);
    MailResourceInfo():uid(""),type(5),createTime(0),reward(NULL),drop(NULL){};
    ~MailResourceInfo();
    
    std::string uid;        //dialog person uid
    int type;
    time_t createTime;
    
    CCArray* reward;    //resource get
    CCArray* drop;
    int pointId;        // resource point
    int level;
    
};

#endif /* defined(__IF__MailResourceInfo__) */
