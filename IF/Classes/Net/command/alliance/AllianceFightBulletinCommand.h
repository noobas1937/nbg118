//
//  AllianceFightBulletinCommand.h
//  IF
//
//  Created by ganxiaohua on 14-8-26.
//
//

#ifndef __IF__AllianceFightBulletinCommand__
#define __IF__AllianceFightBulletinCommand__

#include "CommandBase.h"

#define ALLIANCE_AL_FIGHT_BULLETIN "al.fight.bulletin"

class AllianceFightBulletinCommand: public CommandBase {
public:
    AllianceFightBulletinCommand(int start,int end) : CommandBase(ALLIANCE_AL_FIGHT_BULLETIN){
        putParam("start", CCInteger::create(start));
        putParam("end", CCInteger::create(end));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);

private:
};
#endif /* defined(__IF__AllianceFightBulletinCommand__) */
