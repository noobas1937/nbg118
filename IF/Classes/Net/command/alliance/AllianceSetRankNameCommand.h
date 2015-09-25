//
//  AllianceSetRankNameCommand.h
//  IF
//
//  Created by ganxiaohua on 14-8-6.
//
//

#ifndef __IF__AllianceSetRankNameCommand__
#define __IF__AllianceSetRankNameCommand__

#include "CommandBase.h"

#define ALLIANCE_AL_RANK_NAME_SET "al.rankname.set"
#define ALLIANCE_AL_TERRITORY_NAME_SET "al.territoryname.set"

class AllianceSetRankNameCommand: public CommandBase {
public:
    AllianceSetRankNameCommand() : CommandBase(ALLIANCE_AL_RANK_NAME_SET){

    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};

class AllianceSetTerritoryNameCommand: public CommandBase {
public:
    AllianceSetTerritoryNameCommand() : CommandBase(ALLIANCE_AL_TERRITORY_NAME_SET){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};

#endif /* defined(__IF__AllianceSetRankNameCommand__) */
