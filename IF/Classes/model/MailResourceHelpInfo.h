//
//  MailResourceHelpInfo.h
//  IF
//
//  Created by lifangkai on 14-11-26.
//
//

#ifndef __IF__MailResourceHelpInfo__
#define __IF__MailResourceHelpInfo__

#include "cocos2d.h"
USING_NS_CC;

class MailResourceHelpInfo : public CCNode{
public:
    static MailResourceHelpInfo* create();
    void parse(CCDictionary* dic);
    MailResourceHelpInfo():uid(""),type(21),createTime(0),reward(NULL){};
    ~MailResourceHelpInfo();
    
    std::string uid;        //dialog person uid
    int type;
    time_t createTime;
    
    CCArray* reward;    //resource get
    int pointId;        // resource point
    int level;
    std::string alliance;
    std::string fromName;
    std::string pic;
    std::string traderAlliance;
    
};

#endif /* defined(__IF__MailResourceHelpInfo__) */
