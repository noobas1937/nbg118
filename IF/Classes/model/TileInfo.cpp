//
//  TileInfo.cpp
//  IF
//
//  Created by fubin on 14-5-12.
//
//

#include "TileInfo.h"
#include "CCCommonUtils.h"
#include "FunBuildController.h"

TileInfo::TileInfo(CCDictionary* dict)
{
    SetInfoFromXml(dict);
}

void TileInfo::SetInfoFromXml(CCDictionary* dict)
{
    this->tileId = dict->valueForKey("position")->intValue();
    this->unlock = dict->valueForKey("unlock")->intValue();
    this->xmlOpen = dict->valueForKey("hide")->intValue();
    
    if (this->unlock == 0) {
        this->state = FUN_BUILD_LOCK;
    }
    else if (this->unlock == 1) {
        this->state = FUN_BUILD_ULOCK;
    }
}

BigTileInfo::BigTileInfo(CCDictionary* dict)
{
    SetInfoFromXml(dict);
}

void BigTileInfo::SetInfoFromXml(CCDictionary* dict)
{
    this->tileId = dict->valueForKey("id")->intValue();
    this->unlock = dict->valueForKey("unlock")->intValue();
    
    this->silver_need = dict->valueForKey("silver")->intValue();
    this->wood_need = dict->valueForKey("wood")->intValue();
    this->stone_need = dict->valueForKey("stone")->intValue();
    this->food_need = dict->valueForKey("food")->intValue();
    this->iron_need = dict->valueForKey("iron")->intValue();
    this->positions = dict->valueForKey("position")->getCString();
    this->level = dict->valueForKey("lv")->intValue();
    this->pos = dict->valueForKey("pos")->intValue();
    
    if (this->unlock == 0) {
        this->state = FUN_BUILD_LOCK;
    }
    else if (this->unlock == 1) {
        this->state = FUN_BUILD_ULOCK;
    }
    else if (this->unlock == 2) {
        this->state = FUN_BUILD_CANOPEN;
    }
}