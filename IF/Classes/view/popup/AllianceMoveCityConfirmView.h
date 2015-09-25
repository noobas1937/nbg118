//
//  AllianceMoveCityConfirmView.h
//  IF
//
//  Created by xxrdsg on 15-7-2.
//
//

#ifndef __IF__AllianceMoveCityConfirmView__
#define __IF__AllianceMoveCityConfirmView__

#include "CommonInclude.h"
#include "PopupBaseView.h"

class AllianceMoveCityConfirmView:
public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static AllianceMoveCityConfirmView* create();
    bool init();
    void onEnter();
    void onExit();
    
    AllianceMoveCityConfirmView(){};
    virtual ~AllianceMoveCityConfirmView(){};
    
    void onClickTerMoveBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onClickHeadMoveBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char* pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCControlButton> m_terMoveBtn;
    CCSafeObject<CCControlButton> m_headMoveBtn;
    CCSafeObject<CCNode> m_touchNode;
};

#endif /* defined(__IF__AllianceMoveCityConfirmView__) */
