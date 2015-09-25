//
//  TerritoryResourceTile.h
//  IF
//
//  Created by 〜TIAN~ on 15/5/15.
//
//

#ifndef __IF__TerritoryResourceTile__
#define __IF__TerritoryResourceTile__

#include "BaseTileInfo.h"
#include "NewBaseTileInfo.h"

enum TerritoryResourceTileState{
    TerritoryResourceUnfinished= 0
    ,TerritoryResourceBuilding
    ,TerritoryResourceUngathering
    ,TerritoryResourceGathering
    ,TerritoryResourceOther
    ,TerritoryResourceNone
};
class TerritoryResourceTile : public NewBaseTileInfo {
public:
    
    TerritoryResourceTile(WorldCityInfo& info):
    NewBaseTileInfo(info)
    {}
    
    static TerritoryResourceTile* create(WorldCityInfo& info) {
        auto ret = new TerritoryResourceTile(info);
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
    TerritoryResourceTileState getState();
    
private:
    TerritoryResourceTileState m_currentState;
    bool m_isInUpdate;
    bool dataBack;
};


#endif /* defined(__IF__TerritoryResourceTile__) */
