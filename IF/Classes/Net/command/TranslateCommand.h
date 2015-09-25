//
//  TranslateCommand.h
//  IF
//
//  Created by wulj on 14-8-12.
//
//

#include "../../Ext/CCDevice.h"
#ifndef IF_TranslateCommand_h
#define IF_TranslateCommand_h

#include "CommandBase.h"
#define TRANSLATE_COMMAND "translate"

class TranslateCommand: public CommandBase {
public:
    TranslateCommand(std::string msg):
    CommandBase(TRANSLATE_COMMAND){
        putParam("msg", CCString::create(msg.c_str()));
        putParam("toLang",CCString::create(CCCommonUtils::getLanguage()));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};


#endif /* defined(__IF__TranslateCommand__) */
