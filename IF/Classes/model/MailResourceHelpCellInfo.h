//
//  MailResourceHelpCellInfo.h
//  IF
//
//  Created by lifangkai on 14-11-26.
//
//

#ifndef __IF__MailResourceHelpCellInfo__
#define __IF__MailResourceHelpCellInfo__

#include "MailInfo.h"
//继承 mailInfo

class MailResourceHelpCellInfo : public MailInfo{
public:
    static MailResourceHelpCellInfo* create();
    void parse(CCDictionary* dic);
    ~MailResourceHelpCellInfo();
    
    CCArray *collect;
    int totalNum;
    int unread;
    int resourceType;
    int resourceValue;
};

#endif /* defined(__IF__MailResourceHelpCellInfo__) */
