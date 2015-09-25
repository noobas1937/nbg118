//
//  PlayerTile.h
//  IF
//
//  Created by 邹 程 on 14-2-26.
//
//

#ifndef __IF__PlayerTile__
#define __IF__PlayerTile__

#define WORLD_MAP_REFRESH "worldMapRefresh"

#include "BaseTileInfo.h"
#include "NewBaseTileInfo.h"

class PlayerTile : public NewBaseTileInfo {
public:
    
    PlayerTile(WorldCityInfo& info):
    NewBaseTileInfo(info), m_viewReinforceFlag(false),m_marchId(""),targetTooStrong(false),slowDown(false)
    {}
    
    static PlayerTile* create(WorldCityInfo& info) {
        auto ret = new PlayerTile(info);
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
    
    bool isInProtect();
    bool isInResourceProtect();
    void requestDetail();
    void onDetailCallback(CCObject* obj);
    
    void onClickButton(CCObject * pSender, Control::EventType pCCControlEvent);
    
    virtual BaseTileInfo* getFavoriteView();
    
    bool canBeInvited();
    bool canBeRallyAttacked();
    //跳转到出征选兵界面
    void isPlayerUseSlowDownSkill();
    void gotoOpenMarchDeploy();
    void openMarchDeployWithTips();
    void openAllianceMassView();
    void onCostGoldBack();
    
    void onCampBachHome();
protected:
    void showCampHomeBackTip();
private:
    virtual void onEnter();
    virtual void onExit();
    void refreshView(CCObject * obj);
//    CCSafeObject<CCLabelIF> m_label[6];
//    CCSafeObject<CCSprite> m_playerSprite;
//    CCSafeObject<CCNode> m_viewInfoNode;
//    CCSafeObject<CCNode> m_pic;
    
    PlayerTileInfo * m_playerInfo;
    
    string m_playerUid;
    bool m_viewReinforceFlag;
    void viewTroopInfo();
    
    bool targetTooStrong;
    bool slowDown;
    double registerTime;
    int level;
    
    string m_marchId; //treasuremap
};

#endif /* defined(__IF__PlayerTile__) */
