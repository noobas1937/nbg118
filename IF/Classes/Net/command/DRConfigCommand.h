//
//  DRConfigCommand.h
//  IF
//
//  Created by Émilie.Jiang on 15/7/24.
//
//

#ifndef __IF__DRConfigCommand__
#define __IF__DRConfigCommand__

#include "CommandBase.h"
#include "../../Ext/CCDevice.h"

#define DRConfigGet "drconfig.get"

class DRConfigCommand: public CommandBase {
public:
    DRConfigCommand() : CommandBase(DRConfigGet){
        string _version = cocos2d::extension::CCDevice::getVersionName();
        putParam("version", CCString::create(_version.c_str()));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};

#endif /* defined(__IF__DRConfigCommand__) */
