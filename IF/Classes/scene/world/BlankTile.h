//
//  BlankTile.h
//  IF
//
//  Created by 邹 程 on 14-2-26.
//
//

#ifndef __IF__BlankTile__
#define __IF__BlankTile__

#include "NewBaseTileInfo.h"

class BlankTile : public NewBaseTileInfo {
public:
    
    BlankTile(WorldCityInfo& info):
    NewBaseTileInfo(info)
    {}
    
    static BlankTile* create(WorldCityInfo& info) {
        auto ret = new BlankTile(info);
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
    void onClickTeleport(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickOccupy(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickInviteTeleport(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickPlace(CCObject * pSender, Control::EventType pCCControlEvent);
    void setBtnState();
    virtual BaseTileInfo* getFavoriteView();
    bool canOccupy();
};


#endif /* defined(__IF__BlankTile__) */
