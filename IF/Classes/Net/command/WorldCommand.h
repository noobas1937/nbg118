//
//  WorldCommand.h
//  IF
//
//  Created by 邹 程 on 14-2-18.
//
//

#ifndef __IF__WorldCommand__
#define __IF__WorldCommand__

#include "CommandBase.h"
#include "WorldController.h"
#include "GuideController.h"

#define WORLD_RESOURCE_UPDATE_COMMAND "common.res.syn"
class WorldResourceUpdateCommand: public CommandBase {
public:
    WorldResourceUpdateCommand(const string& marchUuid):
    CommandBase(WORLD_RESOURCE_UPDATE_COMMAND)
    ,m_uuid(marchUuid)
    {
        putParam("uuid", CCString::create(marchUuid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    string m_uuid;
};



#define MARCH_DETAIL_COMMAND "world.user.army"
class WorldMarchDetailCommand: public CommandBase {
public:
    WorldMarchDetailCommand(unsigned int index):
    CommandBase(MARCH_DETAIL_COMMAND)
    ,m_index(index)
    {
        putParam("point", CCInteger::create(index));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    unsigned int m_index;
};


#define MARCH_DETAIL_UPDATE_COMMAND "world.get.march"
class WorldMarchDetailUpdateCommand: public CommandBase {
public:
    WorldMarchDetailUpdateCommand(const string& uuid):
    CommandBase(MARCH_DETAIL_UPDATE_COMMAND)
    ,m_uuid(uuid)
    {
        putParam("uuid", CCString::create(uuid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    const string& m_uuid;
};




#define WORLD_COMMAND "world.get"
class WorldCommand: public CommandBase {
public:
    WorldCommand(const CCPoint& centerPoint,int time,unsigned int type = 0,int forceServerId = -1):
    CommandBase(WORLD_COMMAND)
    ,m_point(centerPoint)
    ,m_timeStamp(time)
    ,m_type(type)
    {
        putParam("x", CCInteger::create((int)m_point.x));
        putParam("y", CCInteger::create((int)m_point.y));
        putParam("timeStamp", CCInteger::create(m_timeStamp));
        putParam("type", CCInteger::create(type)); // 0 : normal 1 : single point update
        if(GuideController::share()->getCurrentId() == "3030200" && GuideController::share()->arr->count() == 0){
            putParam("guide", CCInteger::create(1));
        }
        if (WorldController::getInstance()->openMarchId != "") {
            putParam("extendMarchUuid", CCStringMake(WorldController::getInstance()->openMarchId));
        }
        int sid = GlobalData::shared()->playerInfo.currentServerId;
        if(forceServerId != -1){
            sid = forceServerId;
        }
        m_serverId = sid;
        CCLOG("server=%d x=%f y=%f",sid,m_point.x,m_point.y);
        putParam("serverId", CCInteger::create(sid));
        WorldController::getInstance()->setisAsyEnd(false);
    }
public:
    int getServerId(){
        return m_serverId;
    };
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    int m_timeStamp;
    CCPoint m_point;
    int m_type;
    int m_serverId;
};


#define WORLD_LEAVE_COMMAND "world.leave"
class WorldLeaveCommand: public CommandBase {
public:
    WorldLeaveCommand():
    CommandBase(WORLD_LEAVE_COMMAND)
    {}
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};



#define WORLD_DETAIL_COMMAND "world.get.detail"
#define MSG_WORLD_DEFAIL_VIEW "msg.world.defail.view"
class WorldDetailCommand: public CommandBase {
public:
    WorldDetailCommand(unsigned int index,int serverId):
    CommandBase(WORLD_DETAIL_COMMAND)
    ,m_point(index)
    {
        putParam("point", CCInteger::create(m_point));
        putParam("serverId", CCInteger::create(serverId));
        CCLOG("logwdz+++++worldDetailCommand point+++++%d",m_point);
        CCLOG("logwdz+++++worldDetailCommand serverId+++++%d",serverId);
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    unsigned int m_point;
};


#define WORLD_BUY_PVE_COMMAND "world.pve.count.buy"
class WorldBuyPVECommand: public CommandBase {
public:
    WorldBuyPVECommand(unsigned int index):
    CommandBase(WORLD_BUY_PVE_COMMAND)
    ,m_point(index)
    {
        putParam("point", CCInteger::create(m_point));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    unsigned int m_point;
};


#define WORLD_MARCH_COMMAND "world.march"
class WorldMarchCommand: public CommandBase {
public:
    WorldMarchCommand(unsigned int index,MarchMethodType marchType,unsigned int ownerFlag = 1,CCDictionary* army = NULL,CCArray* resArr = NULL,int monsterIndex = -1, CCArray *generals = NULL,int wtIndex=-1,std::string teamId="",int warehouse = -1/*是否从联盟仓库取*/):
    CommandBase(WORLD_MARCH_COMMAND)
    {
        putParam("point", CCInteger::create(index));
        putParam("type", CCInteger::create((int)marchType));
        putParam("ownerFlag", CCInteger::create(ownerFlag)); // 0 : no owner 1 : have owner
        if (army) {
            putParam("soldiers", army);
        }
        if (resArr) {
            if (warehouse == -1)
                putParam("res", resArr);
            else if (warehouse == 0)
                putParam("retrieveRes", resArr);
            
            putParam("soldiers", CCDictionary::create());
        }
        if (monsterIndex != -1) {
            putParam("ept", CCInteger::create(monsterIndex));
        }
        
        if(generals){
            putParam("generals", generals);
        }
        if(wtIndex!=-1){
            putParam("wtIndex", CCInteger::create(wtIndex));
        }
        if(teamId!=""){
            putParam("team", CCString::create(teamId));
        }
        m_type = marchType;
        usePower= 0;
        if (m_type == MethodFieldMonster) {
            usePower = WorldController::getInstance()->getMonsterUsePower(index);
        }
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    MarchMethodType m_type;
    int usePower;
};

#define WORLD_MARCH_SPEED_UP_COMMAND "world.march.spd"
class WorldMarchSpeedUpCommand: public CommandBase {
public:
    WorldMarchSpeedUpCommand(const string& uuid,const string& itemUuid):
    CommandBase(WORLD_MARCH_SPEED_UP_COMMAND)
    {
        putParam("uuid", CCString::create(uuid));
        putParam("itemUUid", CCString::create(itemUuid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#define WORLD_MARCH_CANCEL_COMMAND "world.march.retreat"
class WorldMarchCancelCommand: public CommandBase {
public:
    WorldMarchCancelCommand(const string& uuid,const string& itemUuid):
    CommandBase(WORLD_MARCH_CANCEL_COMMAND)
    {
        putParam("uuid", CCString::create(uuid));
        putParam("itemUUid", CCString::create(itemUuid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};


#define WORLD_ADD_FAVORITE_COMMAND "world.favo.add"

class WorldAddFavoriteCommand: public CommandBase {
public:
    WorldAddFavoriteCommand(const std::string &name,unsigned int index,unsigned int type,int serverId) :
    CommandBase(WORLD_ADD_FAVORITE_COMMAND)
    ,m_point(index)
    ,m_name(name)
    ,m_type(type)
    {
        putParam("name", CCString::create(name));
        putParam("point",CCInteger::create(index));
        putParam("type", CCInteger::create(type));
        putParam("server", CCInteger::create(serverId));
        m_serverId = serverId;
    }
    
protected:
    unsigned int m_point;
    std::string m_name;
    unsigned int m_type;
    int m_serverId;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#define WORLD_DEL_FAVORITE_COMMAND "world.favo.del"

class WorldDelFavoriteCommand: public CommandBase {
public:
    WorldDelFavoriteCommand(unsigned int index, int serverId) :
    CommandBase(WORLD_DEL_FAVORITE_COMMAND)
    ,m_point(index)
    {
        putParam("point",CCInteger::create(index));
        putParam("server", CCInteger::create(serverId));
        m_server = serverId;
    }
    
protected:
    unsigned int m_point;
    int m_server;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#define WORLD_GET_FAVORITE_COMMAND "world.favo.get"

class WorldGetFavoriteCommand: public CommandBase {
public:
    WorldGetFavoriteCommand() :
    CommandBase(WORLD_GET_FAVORITE_COMMAND)
    {
        putParam("server", CCInteger::create(GlobalData::shared()->playerInfo.currentServerId));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};



#define WORLD_SCOUT_DETAIL_COMMAND "world.scout.detail"

class WorldScoutDetailCommand: public CommandBase {
public:
    WorldScoutDetailCommand(unsigned int index) :
    CommandBase(WORLD_SCOUT_DETAIL_COMMAND)
    ,m_point(index)
    {
        putParam("point", CCInteger::create(index));
    }
    
protected:
    unsigned int m_point;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};



#define WORLD_CITY_MOVE_COMMAND "world.mv"

class WorldCityMoveCommand: public CommandBase {
public:
    WorldCityMoveCommand(unsigned int index,const string& itemUuid) :
    CommandBase(WORLD_CITY_MOVE_COMMAND)
    ,m_point(index)
    ,m_mailUid("")
    ,m_alMoveType(0)
    {
        putParam("point", CCInteger::create(index));
        putParam("itemUUid", CCString::create(itemUuid));
    }
    
    void addMailUid(string mailUid);
    void setAlMovType(int type);
protected:
    string m_mailUid;
    int m_alMoveType;
    unsigned int m_point;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#define WORLD_ADD_ITEM_COMMAND "world.use"

class WorldAddItemCommand: public CommandBase {
public:
    WorldAddItemCommand(unsigned int index,const string& itemUuid) :
    CommandBase(WORLD_ADD_ITEM_COMMAND)
    ,m_point(index)
    {
        putParam("point", CCInteger::create(index));
        putParam("itemUUid", CCString::create(itemUuid));
    }
    
protected:
    unsigned int m_point;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};


#define  WORLD_AAREA_CREATE "territory.create"

class WorldAllianceAreaCreateCommand: public CommandBase{
public:
    WorldAllianceAreaCreateCommand(unsigned int index, string& uid):
    CommandBase(WORLD_AAREA_CREATE)
    ,m_point(index)
    ,m_uid(uid)
    {
        putParam("point", CCInteger::create(index));
        putParam("uid", CCString::create(uid));
    }
protected:
    int m_point;
    string m_uid;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#define  WORLD_AAREA_CANCEL "territory.cancel"

class WorldAllianceAreaCancelCommand: public CommandBase{
public:
    WorldAllianceAreaCancelCommand(unsigned int index, string& uid):
    CommandBase(WORLD_AAREA_CANCEL)
    ,m_point(index)
    ,m_uid(uid)
    {
        putParam("point", CCInteger::create(index));
        putParam("uid", CCString::create(uid));
    }
protected:
    int m_point;
    string m_uid;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
};


#define WORLD_TERRITORY_RES_CREATE "territory.resource.create"

class WorldTerritoryResCreateCommand :public CommandBase{
public:
    WorldTerritoryResCreateCommand(unsigned int index, WorldResourceType resType):
    CommandBase(WORLD_TERRITORY_RES_CREATE)
    ,m_point(index)
    ,m_resType(resType)
    {
        putParam("point", CCInteger::create(index));
        putParam("resType", CCInteger::create(resType));
    }
protected:
    int m_point;
    WorldResourceType m_resType;
    virtual bool handleRecieve(cocos2d::CCDictionary* pDic);
};
#define WORLD_TERRITORY_RES_CANCEL "territory.resource.cancel"

class WorldTerritoryResCancelCommand :public CommandBase{
public:
    WorldTerritoryResCancelCommand(std::string uid, unsigned int index, WorldResourceType resType):
    CommandBase(WORLD_TERRITORY_RES_CANCEL)
    ,m_uid(uid)
    ,m_point(index)
    ,m_resType(resType)
    {
        putParam("uid", CCString::create(uid));
        putParam("point", CCInteger::create(index));
        putParam("resType", CCInteger::create(resType));
    }
protected:
    std::string m_uid;
    int m_point;
    WorldResourceType m_resType;
    virtual bool handleRecieve(cocos2d::CCDictionary* pDic);
};

#define WORLD_TOWER_CREATE "territory.turret.create"

class WorldTowerCreateCommand :public CommandBase{
public:
    WorldTowerCreateCommand(unsigned int index,string uid):
    CommandBase(WORLD_TOWER_CREATE)
    ,m_point(index)
    ,m_uid(uid)
    {
        putParam("point", CCInteger::create(index));
        putParam("uid", CCStringMake(uid));
    }
protected:
    int m_point;
    string m_uid;
    virtual bool handleRecieve(cocos2d::CCDictionary* pDic);
};
#define WORLD_TOWER_CANCEL "territory.turret.cancel"

class WorldTowerCancelCommand :public CommandBase{
public:
    WorldTowerCancelCommand(std::string uid, unsigned int index):
    CommandBase(WORLD_TOWER_CANCEL)
    ,m_uid(uid)
    ,m_point(index)
    {
        putParam("uid", CCString::create(uid));
        putParam("point", CCInteger::create(index));
    }
protected:
    std::string m_uid;
    int m_point;
    virtual bool handleRecieve(cocos2d::CCDictionary* pDic);
};

#define WORLD_TERRITORY_WAREHOUSE_CREATE "territory.warehouse.create"

class WorldTerritoryWarehouseCreateCommand :public CommandBase{
public:
    WorldTerritoryWarehouseCreateCommand(unsigned int index):
    CommandBase(WORLD_TERRITORY_WAREHOUSE_CREATE)
    ,m_point(index)
    {
        putParam("point", CCInteger::create(index));
    }
protected:
    int m_point;
    virtual bool handleRecieve(cocos2d::CCDictionary* pDic);
};
#define WORLD_TERRITORY_WAREHOUSE_CANCEL "territory.warehouse.cancel"

class WorldTerritoryWarehouseCancelCommand :public CommandBase{
public:
    WorldTerritoryWarehouseCancelCommand(std::string uid, unsigned int index):
    CommandBase(WORLD_TERRITORY_WAREHOUSE_CANCEL)
    ,m_uid(uid)
    ,m_point(index)
    {
        putParam("uid", CCString::create(uid));
        putParam("point", CCInteger::create(index));
    }
protected:
    std::string m_uid;
    int m_point;
    virtual bool handleRecieve(cocos2d::CCDictionary* pDic);
};

#define WORLD_TERRITORY_BANNER_CREATE "territory.flag.create"

class WorldTerritoryBannerCreateCommand :public CommandBase{
public:
    WorldTerritoryBannerCreateCommand(unsigned int index, std::string banner):
    CommandBase(WORLD_TERRITORY_BANNER_CREATE)
    ,m_point(index)
    ,m_banner(banner)
    {
        putParam("point", CCInteger::create(index));
        putParam("flag", CCString::create(banner));
    }
protected:
    int m_point;
    std::string m_banner;
    virtual bool handleRecieve(cocos2d::CCDictionary* pDic);
};
#define WORLD_TERRITORY_BANNER_CANCEL "territory.flag.cancel"

class WorldTerritoryBannerCancelCommand :public CommandBase{
public:
    WorldTerritoryBannerCancelCommand(std::string uid, unsigned int index):
    CommandBase(WORLD_TERRITORY_BANNER_CANCEL)
    ,m_uid(uid)
    ,m_point(index)
    {
        putParam("point", CCInteger::create(index));
        putParam("uid", CCString::create(uid));
    }
protected:
    std::string m_uid;
    int m_point;
    virtual bool handleRecieve(cocos2d::CCDictionary* pDic);
};

#define WORLD_CROSS_MOVE_COMMAND "cross.world.mv"

class WorldCrossCityMoveCommand: public CommandBase {
public:
    WorldCrossCityMoveCommand(unsigned int index,const string& itemUuid, int serverId) :
    CommandBase(WORLD_CROSS_MOVE_COMMAND)
    ,m_point(index)
    {
        putParam("point", CCInteger::create(index));
        putParam("itemUUid", CCString::create(itemUuid));
        putParam("serverId", CCInteger::create(serverId));
        m_serverId = serverId;
    }
    
protected:
    unsigned int m_point;
    int m_serverId;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#define MARCH_CHECK_COMMAND "world.user.march"

class WorldMarchTimeUpCheckCommand: public CommandBase {
public:
    WorldMarchTimeUpCheckCommand() : CommandBase(MARCH_CHECK_COMMAND){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};


#define WORLD_CITY_MOVE_GOLD_COMMAND "world.mv.gold"

class WorldCityMoveGoldCommand: public CommandBase {
public:
    WorldCityMoveGoldCommand(unsigned int index) :
    CommandBase(WORLD_CITY_MOVE_GOLD_COMMAND)
    ,m_point(index)
    ,m_mailUid("")
    {
        putParam("point", CCInteger::create(index));
    }
    
    void addMailUid(string mailUid);
    
protected:
    string m_mailUid;
    unsigned int m_point;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};


#define WORLD_TREASUREMAP_GET_COMMAND "treasuremap.get"
class WorldTreasureMapGetCommand: public CommandBase {
public:
    WorldTreasureMapGetCommand(string itemid) :
    CommandBase(WORLD_TREASUREMAP_GET_COMMAND)
    ,mitemid(itemid)
    {
        putParam("itemid", CCString::create(itemid));
    }
    
protected:
    string mitemid;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};
#define WORLD_TREASUREMAP_DESTROY_COMMAND "treasuremap.destroy"
class WorldTreasureMapDestroyCommand: public CommandBase {
public:
    WorldTreasureMapDestroyCommand(string treasureUUID) :
    CommandBase(WORLD_TREASUREMAP_DESTROY_COMMAND)
    ,mtreasureUUID(treasureUUID){
        putParam("treasureuuid", CCString::create(treasureUUID));
    }
protected:
    string mtreasureUUID;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};
#define WORLD_TREASUREMAP_SPD_COMMAND "treasuremap.spd"
class WorldTreasureMapSpeedupCommand: public CommandBase {
public:
    WorldTreasureMapSpeedupCommand(string marchid) :
    CommandBase(WORLD_TREASUREMAP_SPD_COMMAND),mmarchid(marchid){
        putParam("marchid", CCString::create(marchid));
    }
protected:
    string mmarchid;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};
#endif /* defined(__IF__WorldCommand__) */
