//
//  ResourceTile.h
//  IF
//
//  Created by 邹 程 on 14-2-26.
//
//

#ifndef __IF__ResourceTile__
#define __IF__ResourceTile__

#include "BaseTileInfo.h"
#include "NewBaseTileInfo.h"
enum ResourceTileState {
    ResourceBlank
    ,ResourceSelf
    ,ResourceAlliance
    ,ResourceOther
};

class ResourceTile : public NewBaseTileInfo {
public:
    
    ResourceTile(WorldCityInfo& info):
    NewBaseTileInfo(info)
    {}
    
    static ResourceTile* create(WorldCityInfo& info) {
        auto ret = new ResourceTile(info);
        if (ret->init()) {
            ret->autorelease();
        } else {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
    
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    
    bool init();
    void onEnter();
    void onExit();
    void onClickButton(CCObject * pSender, Control::EventType pCCControlEvent);
    CCNode *getNodeByType(std::string type);
    void requestDetail();
    void onDetailCallback(CCObject* obj);
    void updateView();
    void update(float sec);
    virtual BaseTileInfo* getFavoriteView();
    ResourceTileState getState();
    string m_playerUid;
    
private:
    ResourceTileState m_currentState;
    bool m_isInUpdate;
};

#endif /* defined(__IF__ResourceTile__) */
