//
//  ChangeNickNameView.h
//  IF
//
//  Created by wangzhenlei on 13-11-4.
//
//

#ifndef __IF__ChangeNickNameView__
#define __IF__ChangeNickNameView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "InputFieldMultiLine.h"

class ChangeNickNameView : public PopupBaseView
 
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static ChangeNickNameView* create(string goodsUUID);
    ChangeNickNameView():m_goodsUUID(""){};
    ~ChangeNickNameView(){};
    CCSafeObject<CCControlButton> m_okBtn;
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(string goodsUUID);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onOkBtnClick(CCObject *pSender, CCControlEvent event);
    void onCancelClick(CCObject *pSender, CCControlEvent event);
    void onOKCostGold();

//    virtual void keyBackClicked(){}
    virtual bool isClickSwallowed(){return true;}
    virtual int getPriority(){return 0;}
    void resetBtnState(CCObject* obj);
    void checkSuccess(CCObject* data);
    void chechNameFun(float _time);
    
    //
    void onEditTextEvent(CCObject * params);
    void onInputFieldCloseEvent(cocos2d::CCObject *params);
    
    CCSafeObject<CCLabelIF> m_btnText;
    CCSafeObject<CCLabelIF> m_goldNum;
    CCSafeObject<CCSprite> m_goldIcon;

    CCSafeObject<CCLabelIF> m_nickNameTitle;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCLabelIF> m_msgLabel;
    CCSafeObject<CCNode> m_nameNode;
    CCSafeObject<CCNode> m_nameOkNode;
    CCSafeObject<CCNode> m_bg;
//    CCSafeObject<CCControlButton> m_btnCancel;
    
    CCSafeObject<InputFieldMultiLine> m_editBox;
    string m_goodsUUID;
    string m_lastName;
    bool m_isExist;

    int nameMaxLen;
};

#endif /* defined(__IF__ChangeNickNameView__) */
