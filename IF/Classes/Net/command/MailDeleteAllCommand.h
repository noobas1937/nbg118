//
//  MailDeleteAllCommand.h
//  IF
//
//  Created by 李锐奇 on 13-10-12.
//
//

#ifndef IF_MailDeleteAllCommand_h
#define IF_MailDeleteAllCommand_h

#include "CommandBase.h"
#include "cocos2d.h"

#define MAIL_DELETE_ALL_COMMAND "mail.delete.all"

class MailDeleteAllCommand : public CommandBase{
public:
    MailDeleteAllCommand() : CommandBase(MAIL_DELETE_ALL_COMMAND){

    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif
