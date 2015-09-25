//
//  MailResourceCellInfo.h
//  IF
//
//  Created by lifangkai on 14-10-9.
//
//

#ifndef __IF__MailResourceCellInfo__
#define __IF__MailResourceCellInfo__
#include "MailInfo.h"
//继承 mailInfo

class MailResourceCellInfo : public MailInfo{
public:
    static MailResourceCellInfo* create();
    void parse(CCDictionary* dic);
    ~MailResourceCellInfo();

    bool init();
    CCArray *collect;
    int totalNum;
    int unread;
    int resourceType;
    int resourceValue;
    bool resourceResult;
};

#endif /* defined(__IF__MailResourceCellInfo__) */
