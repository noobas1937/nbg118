//
//  FBMineFriendsCommand.h
//  IF
//
//  Created by ganxiaohua on 14-11-3.
//
//

#ifndef __IF__FBMineFriendsCommand__
#define __IF__FBMineFriendsCommand__

#include "CommandBase.h"

#define FB_MINE_FRIEND "mine.friend"

class FBMineFriendsCommand: public CommandBase {
public:
    FBMineFriendsCommand(string ids) : CommandBase(FB_MINE_FRIEND){
        putParam("ids", CCString::create(ids.c_str()));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:

};
#endif /* defined(__IF__FBMineFriendsCommand__) */
