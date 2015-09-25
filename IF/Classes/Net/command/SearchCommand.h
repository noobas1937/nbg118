//
//  SearchCommand.h
//  IF
//
//  Created by xxrdsg on 15-2-5.
//
//

#ifndef __IF__SearchCommand__
#define __IF__SearchCommand__

#include "CommandBase.h"

#define SEARCH_PLAYER_COMMAND "search.player"
#define SEARCH_ALLIANCE_COMMAND "search.alliance"

class SearchPlayerCommand: public CommandBase
{
public:
    SearchPlayerCommand(string key, int page):
    CommandBase(SEARCH_PLAYER_COMMAND)
    {
        putParam("key", CCString::create(key));
        putParam("page", CCInteger::create(page));
    }
protected:
    virtual bool handleRecieve(CCDictionary* dict);
private:
};

class SearchAllianceCommand: public CommandBase
{
public:
    SearchAllianceCommand(string key, int page):
    CommandBase(SEARCH_ALLIANCE_COMMAND)
    {
        putParam("key", CCString::create(key));
        putParam("page", CCInteger::create(page));
    }
protected:
    virtual bool handleRecieve(CCDictionary* dict);
private:
};

#endif /* defined(__IF__SearchCommand__) */
