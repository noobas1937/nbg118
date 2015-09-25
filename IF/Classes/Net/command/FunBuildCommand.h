//
//  FunBuildCommand.h
//  IF
//
//  Created by fubin on 13-11-4.
//
//

#ifndef IF_FunBuildCommand_h
#define IF_FunBuildCommand_h

#include "CommandBase.h"
#define PALACE_CT_COMMAND "build.create"
#define PALACE_UP_COMMAND "build.upgrade"
#define PALACE_DES_COMMAND "build.destroy"
#define PALACE_CD_COMMAND "build.ccd"
#define PALACE_GET_COMMAND "build.collect"
#define PALACE_FINISH_COMMAND "build.finish"
#define PALACE_MOVE_COMMAND "build.move"
#define PALACE_OPEN_TILE "tile.open"

//龙相关
#define DRAGON_CT_COMMAND "d.build.create"
#define DRAGON_UP_COMMAND "d.build.upgrade"
#define DRAGON_DFOOD_COMMAND "dFood.collect"
#define DRAGON_DGOLD_COMMAND "dGold.collect"
#define DRAGON_PLANT_COMMAND "dragon.plant"

class FunBuildCreate;
class TileOpenCommand;

class FunBuildCommand: public CommandBase {
public:
    FunBuildCommand(string uuid, string cmd, int gold=0, int isUpStar=0):
    CommandBase(cmd){
        putParam("uuid", CCString::create(uuid));
        putParam("gold", CCInteger::create(gold));
        putParam("upStar", CCInteger::create(isUpStar));
        _gold = gold;
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    int _gold;
};

class FunBuildCreate: public CommandBase {
public:
    FunBuildCreate(int itemId, int pos, string cmd, int gold=0):
    CommandBase(cmd){
        putParam("itemId", CCString::create(CC_ITOA(itemId)));
        putParam("pos", CCInteger::create(pos));
        putParam("gold", CCInteger::create(gold));
        _gold = gold;
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    int _gold;
};

class FunBuildMove: public CommandBase {
public:
    FunBuildMove(int pos1, int pos2, int gold=0):
    CommandBase(PALACE_MOVE_COMMAND){
        putParam("pos1", CCInteger::create(pos1));
        putParam("pos2", CCInteger::create(pos2));
        putParam("gold", CCInteger::create(gold));
        _gold = gold;
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    int _gold;
};

class TileOpenCommand: public CommandBase {
public:
    TileOpenCommand(int pos, string cmd, int gold=0):
    CommandBase(cmd){
        putParam("pos", CCInteger::create(pos));
        putParam("gold", CCInteger::create(gold));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

//种植龙食物
class PlantDragonFood: public CommandBase {
public:
    PlantDragonFood(string uuid, int itemId, string cmd):
    CommandBase(cmd){
        putParam("uuid", CCString::create(uuid));
        putParam("food", CCString::create(CC_ITOA(itemId)));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__FunBuildCommand__) */
