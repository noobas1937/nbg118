//
//  PushSettingInfo.h
//  IF
//
//  Created by lifangkai on 14-11-27.
//
//

#ifndef __IF__PushSettingInfo__
#define __IF__PushSettingInfo__

#include "cocos2d.h"
//#include "PlayerInfo.h"
USING_NS_CC;


class PushSettingInfo : public CCObject{
public:
    CREATE_FUNC(PushSettingInfo);
    void parse(CCDictionary *dict);
    PushSettingInfo():status(0),type(0),sound(0){};
    ~PushSettingInfo(){

    };
    
    int status;
    int type;
    int sound;
    
private:
    virtual bool init(){return true;};
};

#endif /* defined(__IF__PushSettingInfo__) */
