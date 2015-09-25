//
//  KingdomServantCell.h
//  IF
//
//  Created by ganxiaohua on 14-12-10.
//
//

#ifndef __IF__KingdomServantCell__
#define __IF__KingdomServantCell__
#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "KingdomRemoveAppointView.h"
#include "PopupViewController.h"
#include "OfficeInfo.h"

class KingdomServantCell : public Layer
,public CCBSelectorResolver
 
,public CCBMemberVariableAssigner

{
public:
    static KingdomServantCell *create(OfficeInfo* info,CCNode* showArea,bool manager=false);
    void setData(OfficeInfo* info);
    void refreshView();
    CCSafeObject<CCControlButton> m_btnReward;
private:
    KingdomServantCell(OfficeInfo* info,CCNode* showArea,bool manager):m_info(info),m_manager(manager),m_showArea(showArea){ _ignoreAnchorPointForPosition = false;setAnchorPoint(Vec2(0,0));};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    void refresh(CCObject* param);
    
    CCSafeObject<CCLabelIF> m_officalsTxt;
    CCSafeObject<CCSprite> m_cellBg;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCLabelIFTTF> m_nameTxt;
    CCSafeObject<CCNode> m_clickArea;
    CCNode* m_showArea;
    OfficeInfo* m_info;
    bool m_manager;
    CCPoint m_startPos;
};

#endif /* defined(__IF__KingdomServantCell__) */
