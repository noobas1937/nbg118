//
//  MailBlockCommand.h
//  IF
//
//  Created by 李锐奇 on 14-2-28.
//
//

#ifndef __IF__MailBlockCommand__
#define __IF__MailBlockCommand__

#include "CommandBase.h"
#include "CommonInclude.h"

#define MAIL_BLOCK_COMMAND "mail.save"

class MailBlockCommand: public CommandBase {
public:
    MailBlockCommand(std::string name):
    CommandBase(MAIL_BLOCK_COMMAND)
    {
        putParam("name", CCString::create(name));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__MailBlockCommand__) */
