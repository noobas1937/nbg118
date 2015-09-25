//
//  TerritoryWarehouseTile.h
//  IF
//
//  Created by 〜TIAN~ on 15/7/7.
//
//

#ifndef __IF__TerritoryWarehouseTile__
#define __IF__TerritoryWarehouseTile__


#include "BaseTileInfo.h"
#include "NewBaseTileInfo.h"

enum TerritoryWarehouseTileState{
    TerritoryWarehouseUnfinished= 0
    ,TerritoryWarehouseBuilding
    ,TerritoryWarehouseStoring
    ,TerritoryWarehouseOther
    ,TerritoryWarehouseNone
};
class TerritoryWarehouseTile : public NewBaseTileInfo {
public:
    
    TerritoryWarehouseTile(WorldCityInfo& info):
    NewBaseTileInfo(info)
    {}
    
    static TerritoryWarehouseTile* create(WorldCityInfo& info) {
        auto ret = new TerritoryWarehouseTile(info);
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
    TerritoryWarehouseTileState getState();
    bool m_resOnTheWay;
private:
    TerritoryWarehouseTileState m_currentState;
    bool m_isInUpdate;
    int soldierMax;
    int soldierAlready;
    bool dataBack;
};


#endif /* defined(__IF__TerritoryWarehouseTile__) */
