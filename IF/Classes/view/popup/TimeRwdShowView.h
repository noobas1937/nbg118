//
//  TimeRwdShowView.h
//  IF
//
//  Created by xxrdsg on 15-3-24.
//
//

#ifndef __IF__TimeRwdShowView__
#define __IF__TimeRwdShowView__

#include "CommonInclude.h"
#include "PopupBaseView.h"

class TimeRwdShowView: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
  
{
public:
    static TimeRwdShowView* create();
protected:
private:
    bool init();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    void onClickOkBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void afterGetRwd(CCObject* p);
    
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCControlButton> m_btnOk;
    CCSafeObject<CCLabelIF> m_btnTitle;
    WaitInterface* m_waitInterface;
};

#endif /* defined(__IF__TimeRwdShowView__) */
