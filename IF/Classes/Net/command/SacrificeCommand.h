//
//  SacrificeCommand.h
//  IF
//
//  Created by lifangkai on 14-11-19.
//
//

#ifndef __IF__SacrificeCommand__
#define __IF__SacrificeCommand__

#include "CommandBase.h"

#define SACRIFICE_GET "user.pray.info"
#define SACRIFICE_BUY "user.pray"

class SacrificeGetCommand: public CommandBase {
public:
    SacrificeGetCommand() : CommandBase(SACRIFICE_GET){
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
};

class SacrificeBuyCommand: public CommandBase {
public:
    SacrificeBuyCommand(int type,int gold,int num) : CommandBase(SACRIFICE_BUY){
        putParam("gold", CCInteger::create(gold));
        putParam("resource", CCInteger::create(type));
        putParam("num", CCInteger::create(num));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
};

#endif /* defined(__IF__SacrificeCommand__) */
