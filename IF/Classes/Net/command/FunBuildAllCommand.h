//
//  FunBuildAllCommand.h
//  IF
//
//  Created by fubin on 13-11-5.
//
//

#ifndef IF_FunBuildAllCommand_h
#define IF_FunBuildAllCommand_h

#include "CommandBase.h"
#define PALACE_SHOW_COMMAND "build.info"

class FunBuildAllCommand: public CommandBase {
public:
    FunBuildAllCommand(string cmd):CommandBase(cmd){}
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

#endif /* defined(__IF__FunBuildCommand__) */
