//
//  MonsterTile.h
//  IF
//
//  Created by 李锐奇 on 14-8-5.
//
//

#ifndef __IF__MonsterTile__
#define __IF__MonsterTile__

#include "NewBaseTileInfo.h"

class MonsterTile : public NewBaseTileInfo {
public:
    
    MonsterTile(WorldCityInfo& info):
    NewBaseTileInfo(info)
    {}
    
    static MonsterTile* create(WorldCityInfo& info) {
        auto ret = new MonsterTile(info);
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
    
    void onInfoClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickOccupy(CCObject * pSender, Control::EventType pCCControlEvent);
    void onAllianceActClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void yesFun();
//    void setBtnState();
    virtual BaseTileInfo* getFavoriteView();
};

#endif /* defined(__IF__MonsterTile__) */
