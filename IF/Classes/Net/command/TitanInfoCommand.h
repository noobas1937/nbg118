//
//  TitanInfoCommand.h
//  IF
//
//  Created by Émilie.Jiang on 15/7/24.
//
//

#ifndef __IF__TitanInfoCommand__
#define __IF__TitanInfoCommand__

#include "CommandBase.h"


#define TitanInfoCMD "titan.info"

class TitanInfoCommand: public CommandBase {
public:
    TitanInfoCommand() : CommandBase(TitanInfoCMD){

    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};

#endif /* defined(__IF__DRConfigCommand__) */
