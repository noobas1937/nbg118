//
//  SearchUserAllianceCommand.h
//  IF
//
//  Created by ganxiaohua on 14-4-3.
//
//

#ifndef __IF__SearchUserAllianceCommand__
#define __IF__SearchUserAllianceCommand__

#include "CommandBase.h"
#define SEARCH_USER "al.searchuser"

class SearchUserAllianceCommand: public CommandBase {
public:
    SearchUserAllianceCommand(std::string key,string lang,int page);
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    std::string m_key;
    std::string m_lang;
    int m_page;
};
#endif /* defined(__IF__SearchUserAllianceCommand__) */
