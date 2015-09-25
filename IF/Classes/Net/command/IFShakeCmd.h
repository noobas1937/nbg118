//
//  IFShakeCmd.h
//  IF
//
//  Created by wangdianzhen on 15/8/19.
//
//

#ifndef __IF__IFShakeCmd__
#define __IF__IFShakeCmd__

#include "CommandBase.h"
#include "ShakeController.h"
#define SHAKECOMMAND "monster.find"
class IFShakeCmd : public CommandBase{
public:
    IFShakeCmd(int round): m_round(round), CommandBase(SHAKECOMMAND)
    {
        putParam("round", CCInteger::create(round));//shake index
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    int m_round;
};

#endif /* defined(__IF__IFShakeCmd__) */
