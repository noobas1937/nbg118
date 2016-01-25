//
//  TranslateOptimizeCommand.h
//  IF
//
//  Created by xupengzhan on 15-10-12.
//
//

#ifndef __IF__TranslateOptimizeCommand__
#define __IF__TranslateOptimizeCommand__

#include "CommandBase.h"

#define TRANSLATE_IMPROVE "chat.translate.improve"

#define METHOD_NOT_UNDERSTAND "notunderstand"
#define METHOD_ADD_TRANSLATE "addtranslate"
#define METHOD_VOTE_TRANSLATE "votetranslate"

class TranslateOptimizeCommand : public CommandBase{
public:
    TranslateOptimizeCommand(std::string method,std::string originalLang = "",std::string userLang = "",std::string msg = "",std::string translationMsg = "") :m_method(method),
    CommandBase(TRANSLATE_IMPROVE){
        putParam("method", CCString::create(method));
        if(originalLang!="")
            putParam("originalLang", CCString::create(originalLang));
        if(userLang!="")
        putParam("userLang", CCString::create(userLang));
        if(msg!="")
            putParam("msg", CCString::create(msg));
        if(translationMsg!="")
            putParam("translationMsg", CCString::create(translationMsg));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    std::string m_method;
};

#endif /* defined(__IF__TranslateOptimizeCommand__) */
