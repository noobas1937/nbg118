//
//  ActivityPopupView.h
//  IF
//
//  Created by xxrdsg on 15-7-9.
//
//

#ifndef __IF__ActivityPopupView__
#define __IF__ActivityPopupView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "ActivityEventObj.h"

class ActivityPopupView: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static ActivityPopupView* create(string itemId);
    ActivityPopupView(string itemId): m_itemId(itemId), m_obj(nullptr){}
    
    bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void onClickJumpBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onCloseBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
private:
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_storyLabel;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCControlButton> m_jumpBtn;
    CCSafeObject<CCNode> m_labelNode;
    
    string m_itemId;
    ActivityEventObj* m_obj;
};

#endif /* defined(__IF__ActivityPopupView__) */
