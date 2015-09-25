//
//  FBMineHelpCommand.h
//  IF
//
//  Created by ganxiaohua on 14-11-3.
//
//

#ifndef __IF__FBMineHelpCommand__
#define __IF__FBMineHelpCommand__

#include "CommandBase.h"

#define FB_MINE_HELP "mine.help"

class FBMineHelpCommand: public CommandBase {
public:
    FBMineHelpCommand(int server,std::string gUid,string fbName) : CommandBase(FB_MINE_HELP){
        putParam("server", CCInteger::create(server));
        putParam("gUid", CCString::create(gUid.c_str()));
        putParam("fbName", CCString::create(fbName.c_str()));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_fbId;
};
#endif /* defined(__IF__FBMineHelpCommand__) */
