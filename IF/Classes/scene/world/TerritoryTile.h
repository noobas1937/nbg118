//
//  TerritoryTile.h
//  IF
//
//  Created by 〜TIAN~ on 15/4/10.
//
//

#ifndef __IF__TerritoryTile__
#define __IF__TerritoryTile__

#include "BaseTileInfo.h"
#include "NewBaseTileInfo.h"

enum TerritoryTileState{
    TerritoryBuilding = 0
    ,TerritoryUnfinished
    ,TerritoryStation
    ,TerritoryUnstation
    ,TerritoryRepair
    ,TerritoryBroken
    ,TerritoryMineOccupy
    ,TerritoryOtherUnOccupy
    ,TerritoryOtherOccupy
    ,TerritoryOtherOccupyByMe
};
class TerritoryTile : public NewBaseTileInfo {
public:
    
    TerritoryTile(WorldCityInfo& info):
    NewBaseTileInfo(info)
    {}
    
    static TerritoryTile* create(WorldCityInfo& info) {
        auto ret = new TerritoryTile(info);
        if (ret->init()) {
            ret->autorelease();
        } else {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
    
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    void requestDetail();
    void onDetailCallback(CCObject* obj);
    bool init();
    void onEnter();
    void onExit();
    void onClickButton(CCObject * pSender, Control::EventType pCCControlEvent);
    void updateView();
    void update(float sec);
    virtual BaseTileInfo* getFavoriteView();
    TerritoryTileState getState();
    
private:
    TerritoryTileState m_currentState;
    bool m_isInUpdate;
    int soldierMax;
    int soldierAlready;
    bool dataBack;
};

#endif /* defined(__IF__TerritoryTile__) */
