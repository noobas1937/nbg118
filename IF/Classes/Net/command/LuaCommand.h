//
//  LuaCommand.h
//  IF
//
//  Created by xxrdsg on 15-5-12.
//
//

#ifndef __IF__LuaCommand__
#define __IF__LuaCommand__

#include "CommandBase.h"

class LuaCommand: public CommandBase
{
public:
    LuaCommand(std::string name, CCDictionary* dic): CommandBase(name), m_name(name), m_valueDic(dic)
    {
        CCDictElement* ele = NULL;
        CCDICT_FOREACH(dic, ele)
        {
            if (ele && ele->getObject())
            {
                std::string key = ele->getStrKey();
                CCObject* value = ele->getObject();
                if(!key.empty() && value)
                {
                    putParam(key, value);
                }
            }
        }
        if (m_valueDic == nullptr) {
            m_valueDic = CCDictionary::create();
        }
    }
protected:
    virtual bool handleRecieve(CCDictionary*);
private:
    std::string m_name;
    CCSafeObject<CCDictionary> m_valueDic;
};

#endif /* defined(__IF__LuaCommand__) */
