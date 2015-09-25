//
//  TerritoryBannerTile.h
//  IF
//
//  Created by 〜TIAN~ on 15/8/19.
//
//

#ifndef __IF__TerritoryBannerTile__
#define __IF__TerritoryBannerTile__

#include "BaseTileInfo.h"
#include "NewBaseTileInfo.h"

enum TerritoryBannerTileState{
    TerritoryBannerBuilding= 0
    ,TerritoryBannerUnfinished
    ,TerritoryBannerStation
    ,TerritoryBannerUnstation
    ,TerritoryBannerRepair
    ,TerritoryBannerBroken
    ,TerritoryBannerMineOccupy
    ,TerritoryBannerOtherUnOccupy
    ,TerritoryBannerOtherOccupy
    ,TerritoryBannerOtherOccupyByMe
    ,TerritoryBannerNone
};
class TerritoryBannerTile : public NewBaseTileInfo {
public:
    
    TerritoryBannerTile(WorldCityInfo& info):
    NewBaseTileInfo(info)
    {}
    
    static TerritoryBannerTile* create(WorldCityInfo& info) {
        auto ret = new TerritoryBannerTile(info);
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
    void onClickButton(CCObject * pSender, CCControlEvent pCCControlEvent);
    void updateView();
    void update(float sec);
    virtual BaseTileInfo* getFavoriteView();
    TerritoryBannerTileState getState();
    
private:
    TerritoryBannerTileState m_currentState;
    bool m_isInUpdate;
    int soldierMax;
    int soldierAlready;
    bool dataBack;
};
#endif /* defined(__IF__TerritoryBannerTile__) */
