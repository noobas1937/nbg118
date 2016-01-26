//
//  MailHeiqishiCellInfo.hpp
//  IF
//
//  Created by 〜TIAN~ on 15/10/28.
//
//

#ifndef MailHeiqishiCellInfo_hpp
#define MailHeiqishiCellInfo_hpp

#include "MailInfo.h"
//继承 mailInfo

class MailHeiqishiCellInfo : public MailInfo{
public:
    static MailHeiqishiCellInfo* create();
    void parse(CCDictionary* dic);
    MailHeiqishiCellInfo():knight(NULL),totalNum(0),unread(0){};
    ~MailHeiqishiCellInfo();
    
    CCArray *knight;
    int totalNum;
    int unread;
};

#endif /* MailHeiqishiCellInfo_hpp */
