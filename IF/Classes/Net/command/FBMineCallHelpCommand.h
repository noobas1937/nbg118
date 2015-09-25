//
//  FBMineCallHelpCommand.h
//  IF
//
//  Created by ganxiaohua on 14-11-19.
//
//

#ifndef __IF__FBMineCallHelpCommand__
#define __IF__FBMineCallHelpCommand__

#include "CommandBase.h"

#define FB_MINE_CALL_HELP "mine.callHelp"

class FBMineCallHelpCommand: public CommandBase {
public:
    FBMineCallHelpCommand(string uid,string server,string fbName) : CommandBase(FB_MINE_CALL_HELP){
        putParam("uids", CCString::create(uid));
        putParam("servers", CCString::create(server));
        putParam("fbName", CCString::create(fbName));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_fbId;
};
#endif /* defined(__IF__FBMineCallHelpCommand__) */
