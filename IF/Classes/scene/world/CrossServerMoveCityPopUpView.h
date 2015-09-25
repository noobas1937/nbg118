//
//  CrossServerMoveCityPopUpView.h
//  IF
//
//  Created by 李锐奇 on 14-10-17.
//
//

#ifndef __IF__CrossServerMoveCityPopUpView__
#define __IF__CrossServerMoveCityPopUpView__

#include "MoveCityPopUpView.h"
#include "CommonInclude.h"
#include "CCBExtension.h"

#define CROSS_SERVER_CONFIRM "crossServerConfirm"

class CrossServerMoveCityPopUpView : public MoveCityPopUpView{
public:
    CREATE_FUNC(CrossServerMoveCityPopUpView);
protected:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual int getFreeMoveItemId();
    virtual int getMoveItemId();
    virtual void doWhenHasItem(CCDictionary *dict);
    virtual void onMoveCityClick(CCObject *pSender, CCControlEvent event);
    virtual void doWhenUseMoney(CCDictionary *dict);
    void alertShow(CCObject *dict);
};

class CrossServerAlertPopUpView : public PopupBaseView
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
{
public:
    CREATE_FUNC(CrossServerAlertPopUpView);
private:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickOKBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickCancelBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCLabelIF> m_title;
    CCSafeObject<CCLabelIF> m_hintText;
    CCSafeObject<CCControlButton> m_confirmBtn;
    CCSafeObject<CCControlButton> m_cancelBtn;
};

#endif /* defined(__IF__CrossServerMoveCityPopUpView__) */
