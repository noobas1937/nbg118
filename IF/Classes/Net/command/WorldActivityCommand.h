//
//  WorldActivityCommand.h
//  IF
//
//  Created by 李锐奇 on 14/12/4.
//
//

#ifndef __IF__WorldActivityCommand__
#define __IF__WorldActivityCommand__

#include "CommandBase.h"

#define GET_WORLD_ACTIVITY_COMMAND "act.ls"
#define GET_CROSS_WORLD_ACTIVITY_COMMAND "kingdom.throne.info"
#define GET_CROSS_WORLD_DATA_BACK "getCrossWorldDataBack"
#define GET_WAR_BUILD_FIGHT_INFO "kingdom.fight.bulletin"
#define GET_ACT_BOSS_PT "get.act.boss.point"
#define Ann_Get_Info "1year.info"

class WorldActivityGetCommand: public CommandBase {
public:
    WorldActivityGetCommand() : CommandBase(GET_WORLD_ACTIVITY_COMMAND){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

class CrossWorldActivityGetCommand : public CommandBase{
public:
    CrossWorldActivityGetCommand(int index) : CommandBase(GET_CROSS_WORLD_ACTIVITY_COMMAND){
        putParam("serverId", CCInteger::create(GlobalData::shared()->playerInfo.currentServerId));
        putParam("pointId", CCInteger::create(index));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

class GetFightInfoCommand : public CommandBase{
public:
    GetFightInfoCommand(int start, int end, int index) : CommandBase(GET_WAR_BUILD_FIGHT_INFO){
        putParam("pointId", CCInteger::create(index));
        putParam("start", CCInteger::create(start));
        putParam("end", CCInteger::create(end));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

class GetACTBossCommand : public CommandBase{
public:
    GetACTBossCommand() : CommandBase(GET_ACT_BOSS_PT){
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

class AnniversaryGetCommand: public CommandBase {
public:
    AnniversaryGetCommand() : CommandBase(Ann_Get_Info){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};
#endif /* defined(__IF__WorldActivityCommand__) */
