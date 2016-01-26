//
//  GetNewSystemMailMsg_iOS_Command.h
//  IF
//
//  Created by 张彦涛 on 15/9/27.
//
//

#ifndef __IF__GetNewSystemMailMsg_iOS_Command__
#define __IF__GetNewSystemMailMsg_iOS_Command__


#include "CommandBase.h"
#define Chat_Get_NewSystemMailMsg_iOS "chat.get.system.mails"

class GetNewSystemMailMsg_iOS_Command:public CommandBase
{
public:
    //clientseq:邮件id
    //count:一次获取的数量
    GetNewSystemMailMsg_iOS_Command(std::string vClientSeq,std::string vCreatTime ,int vCount):CommandBase(Chat_Get_NewSystemMailMsg_iOS){
        putParam("clientseq", CCString::create(vClientSeq));
        putParam("time",CCString::create(vCreatTime));
        putParam("count",CCInteger::create(vCount));
    }
    ~GetNewSystemMailMsg_iOS_Command();
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
 
};



#endif /* defined(__IF__GetNewSystemMailMsg_iOS_Command__) */
