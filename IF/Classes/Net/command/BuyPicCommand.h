//
//  BuyPicCommand.h
//  IF
//
//  Created by ganxiaohua on 14-7-23.
//
//

#ifndef __IF__BuyPicCommand__
#define __IF__BuyPicCommand__

#include "CommandBase.h"

#define PIC_BUY "pic.buy"

class BuyPicCommand: public CommandBase {
public:
    BuyPicCommand(std::string pic) : CommandBase(PIC_BUY),m_pic(pic){
        putParam("pic", CCString::create(m_pic.c_str()));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_pic;
};
#endif /* defined(__IF__BuyPicCommand__) */
