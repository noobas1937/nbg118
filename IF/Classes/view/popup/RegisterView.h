//
//  RegisterView.h
//  IF
//
//  Created by zhaohf on 14-4-10.
//
//

#ifndef IF_RegisterView_h
#define IF_RegisterView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"

class RegisterView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static RegisterView* create();
    RegisterView(){};
    virtual ~RegisterView(){};
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onOk(CCObject * pSender, Control::EventType pCCControlEvent);
    void onCloseView(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCControlButton> m_closeBtn;
    CCSafeObject<CCControlButton> m_okBtn;
    CCSafeObject<CCNode> m_subNode;
    
    CCSafeObject<CCEditBox> m_editBoxEmail;
    CCSafeObject<CCEditBox> m_editBoxPassword;
    CCSafeObject<CCEditBox> m_editBoxPassword1;
};

#endif /* defined(__IF__ChatNameView__) */
