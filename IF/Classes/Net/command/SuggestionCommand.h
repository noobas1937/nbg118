//
//  SuggestionCommand.h
//  IF
//
//  Created by fubin on 13-11-14.
//
//

#ifndef IF_SuggestionCommand_h
#define IF_SuggestionCommand_h

#include "CommandBase.h"
#include "cocos2d.h"

#define SUGGESTION_COMMADN "suggestion.make"

class SuggestionCommand : public CommandBase{
public:
    SuggestionCommand(string msg) : CommandBase(SUGGESTION_COMMADN){
        putParam("contents", CCString::create(msg));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
};

#endif /* defined(__IF__EquipOffCommand__) */
