//
//  MailHeiqishiInfo.hpp
//  IF
//
//  Created by 〜TIAN~ on 15/10/28.
//
//

#ifndef MailHeiqishiInfo_hpp
#define MailHeiqishiInfo_hpp

#include "MailInfo.h"
//继承 mailInfo
USING_NS_CC;

class MailHeiqishiInfo : public MailInfo{
public:
    static MailHeiqishiInfo* create();
    void parse(CCDictionary* dic);
    MailHeiqishiInfo():uid(""),createTime(0){};
    ~MailHeiqishiInfo();
    
    std::string uid;
    time_t createTime;
};

#endif /* MailHeiqishiInfo_hpp */
