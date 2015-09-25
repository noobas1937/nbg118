//
//  CDkeyPopUpView.h
//  IF
//
//  Created by ganxiaohua on 14-10-27.
//
//

#ifndef __IF__CDkeyPopUpView__
#define __IF__CDkeyPopUpView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"

class CDkeyPopUpView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
 
{
public:
    CREATE_FUNC(CDkeyPopUpView);
    
    CDkeyPopUpView();
    ~CDkeyPopUpView();
    
private:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);

    void onBtnOkClick(CCObject *pSender, CCControlEvent event);
    void getRewardCallBack(CCObject* param);
    void checkCanUse(float _time);
    
    CCSafeObject<CCScale9Sprite> m_background;
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCLabelIF> m_descTxt;
    CCSafeObject<CCControlButton> m_btnOk;
    CCSafeObject<CCEditBox> m_editBox;
    CCSafeObject<CCNode> m_editNode;
};

#endif /* defined(__IF__CDkeyPopUpView__) */
