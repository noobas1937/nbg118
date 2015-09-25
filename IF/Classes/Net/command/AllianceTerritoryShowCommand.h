//
//  AllianceTerritoryShowCommand.h
//  IF
//
//  Created by wangchaoyi on 15/4/9.
//
//

#ifndef __IF__AllianceTerritoryShowCommand__
#define __IF__AllianceTerritoryShowCommand__

#include "CommandBase.h"

#define ALLIANCE_TERRITORY_SHOW "territory.check"
#define ALLIANCE_TERRITORY_PLACE "al.territory.place"
#define ALLIANCE_TERRITORY_WITHDRAW "al.territory.withdraw"
#define ALLIANCE_TERRITORY_DETAIL "territory.detail"
#define ALLIANCE_TERRITORY_DATA_BACK "al.territory.data.back"
#define ALLIANCE_TERRITORY_DATA_BACKFAIL "al.territory.data.backfail"
#define ALLIANCE_TERRITORY_CHECKIFSTORAGE "check.if.storage"
#define CHECKIFSTORAGE_DATA_BACK "check.if.storage.databack"
#define CHECKIFSTORAGE_DATA_BACKFAIL "check.if.storage.databackfail"
#define ALLIANCE_TERRITORY_QIANFAN "world.march.member.retreat"

class AllianceTerritoryShowCommand: public CommandBase {
public:
    AllianceTerritoryShowCommand() : CommandBase(ALLIANCE_TERRITORY_SHOW){
    };
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

class AllianceTerritoryDetailCommand: public CommandBase {
public:
    AllianceTerritoryDetailCommand(int territoryPointId): CommandBase(ALLIANCE_TERRITORY_DETAIL){
        m_territoryPointId = territoryPointId;
        putParam("point", CCInteger::create(territoryPointId));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    int m_territoryPointId;
};

class AllianceTerritoryCheckIfStorageCommand: public CommandBase {
public:
    AllianceTerritoryCheckIfStorageCommand(std::string uid): CommandBase(ALLIANCE_TERRITORY_CHECKIFSTORAGE){
        putParam("uid", CCString::create(uid));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

class AllianceTerritoryQianfanCommand: public CommandBase {
public:
    AllianceTerritoryQianfanCommand(std::string uuid): CommandBase(ALLIANCE_TERRITORY_QIANFAN){
        putParam("uuid", CCString::create(uuid));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};
#endif /* defined(__IF__AllianceTerritoryShowCommand__) */
