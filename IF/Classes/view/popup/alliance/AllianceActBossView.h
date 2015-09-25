//
//  AllianceActBossView.h
//  IF
//
//  Created by 付彬 on 15/4/27.
//
//

#ifndef __IF__AllianceActBossView__
#define __IF__AllianceActBossView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "ActivityEventObj.h"

class AllianceActBossView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
{
public:
    static AllianceActBossView *create(ActivityEventObj* info);
    
private:
    AllianceActBossView(ActivityEventObj* info):m_info(info){};
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onCloseClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onAddAlliance(CCObject * pSender, Control::EventType pCCControlEvent);
    void onJijie(CCObject * pSender, Control::EventType pCCControlEvent);
    void onLookTroops(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void scheduleHandle(float _time);
    void goToWorld();
    void freshView(float _time=0.0);
    
    CCSafeObject<CCControlButton> m_btnClose;
    CCSafeObject<CCNode> m_clickArea;
    CCSafeObject<CCLabelIF> m_titleTxt;
    
    CCSafeObject<CCNode> m_readyNode;
    CCSafeObject<CCLabelIF> m_readyTime;
    CCSafeObject<CCLabelIF> m_readyTip;
    
    CCSafeObject<CCNode> m_startNode;
    CCSafeObject<CCControlButton> m_btnLook;
    CCSafeObject<CCControlButton> m_btnBattle;
    CCSafeObject<CCControlButton> m_btnAddAlliance;
    CCSafeObject<CCLabelIF> m_startTxt;
    CCSafeObject<CCLabelIF> m_startTip;
    CCSafeObject<CCSprite> m_monsterSp;
    CCSafeObject<CCNode> m_loadingNode;
    
    CCSafeObject<WaitInterface> m_waitInterface;
    
    ActivityEventObj* m_info;
};

#endif /* defined(__IF__AllianceActBossView__) */
