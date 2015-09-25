//
//  MailDeleteAllCommand.cpp
//  IF
//
//  Created by 李锐奇 on 13-10-12.
//
//

#include "MailDeleteAllCommand.h"

bool MailDeleteAllCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(MAIL_DELETE_ALL_COMMAND) != 0)
        return false;
    return true;
}