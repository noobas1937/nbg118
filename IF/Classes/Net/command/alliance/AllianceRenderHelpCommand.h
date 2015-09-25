//
//  AllianceRenderHelpCommand.h
//  IF
//
//  Created by ganxiaohua on 14-7-16.
//
//

#ifndef __IF__AllianceRenderHelpCommand__
#define __IF__AllianceRenderHelpCommand__

#include "CommandBase.h"

#define ALLIANCE_AL_RENDER_HELP "al.renderhelp"

class AllianceRenderHelpCommand: public CommandBase {
public:
    AllianceRenderHelpCommand(std::string helpId) : CommandBase(ALLIANCE_AL_RENDER_HELP),m_helpId(helpId){
        putParam("helpId", CCString::create(m_helpId.c_str()));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_helpId;
};

#endif /* defined(__IF__AllianceRenderHelpCommand__) */
