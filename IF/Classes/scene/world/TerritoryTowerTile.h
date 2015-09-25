//
//  TerritoryTowerTile.h
//  IF
//
//  Created by 〜TIAN~ on 15/6/8.
//
//

#ifndef __IF__TerritoryTowerTile__
#define __IF__TerritoryTowerTile__

#include "BaseTileInfo.h"
#include "NewBaseTileInfo.h"

enum TerritoryTowerTileState{
    TerritoryTowerUnfinished= 0
    ,TerritoryTowerBuilding
    ,TerritoryTowerFinished
    ,TerritoryTowerOther
    ,TerritoryTowerNone
};
class TerritoryTowerTile : public NewBaseTileInfo {
public:
    
    TerritoryTowerTile(WorldCityInfo& info):
    NewBaseTileInfo(info)
    {}
    
    static TerritoryTowerTile* create(WorldCityInfo& info) {
        auto ret = new TerritoryTowerTile(info);
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
    TerritoryTowerTileState getState();
    
private:
    TerritoryTowerTileState m_currentState;
    bool m_isInUpdate;
    int soldierMax;
    int soldierAlready;
    bool dataBack;
};

#endif /* defined(__IF__TerritoryTowerTile__) */
