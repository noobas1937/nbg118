//
//  TitanFeedCommand.h
//  IF
//
//  Created by Émilie.Jiang on 15/7/24.
//
//

#ifndef __IF__TitanFeedCommand__
#define __IF__TitanFeedCommand__

#include "CommandBase.h"


#define TitanFeed "titan.feed"

class TitanFeedCommand: public CommandBase {
public:
    TitanFeedCommand() : CommandBase(TitanFeed){
//        string _version = cocos2d::extension::CCDevice::getVersionName();
//        putParam("version", CCString::create(_version.c_str()));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};

#endif /* defined(__IF__DRConfigCommand__) */
