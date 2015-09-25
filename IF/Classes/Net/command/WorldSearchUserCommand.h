//
//  WorldSearchUserCommand.h
//  IF
//
//  Created by ganxiaohua on 15-1-22.
//
//

#ifndef __IF__WorldSearchUserCommand__
#define __IF__WorldSearchUserCommand__

#include "CommandBase.h"
#define WORLD_SEARCH_USER "world.search.user"

class WorldSearchUserCommand: public CommandBase {
public:
    WorldSearchUserCommand(string name,int page) : CommandBase(WORLD_SEARCH_USER){
        putParam("name", CCString::create(name.c_str()));
        putParam("page", CCInteger::create(page));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    void parseSearchedResult(cocos2d::CCDictionary* param);
};
#endif /* defined(__IF__WorldSearchUserCommand__) */
