//
//  GetAllianceListCommand.h
//  IF
//
//  Created by ganxiaohua on 14-3-25.
//
//

#ifndef __IF__GetAllianceListCommand__
#define __IF__GetAllianceListCommand__

#include "CommandBase.h"
#define GET_ALLIANCE_LIST "al.search"
#define GET_ALLIANCE_URL "al.get.url"

class GetAllianceListCommand: public CommandBase {
public:
    GetAllianceListCommand(std::string key,int page,int type=0);
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    std::string m_key;
    int m_page;
    int m_type;
};

class GetAllianceUrlCommand: public CommandBase {
public:
    GetAllianceUrlCommand(string uuid):
    CommandBase(GET_ALLIANCE_URL){
        putParam("uuid", CCString::create(uuid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

#endif /* defined(__IF__GetAllianceListCommand__) */
