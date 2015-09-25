//
//  ConfirmDialogView.h
//  IF
//
//  Created by ganxiaohua on 14-6-4.
//
//

#ifndef __IF__ConfirmDialogView__
#define __IF__ConfirmDialogView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "CCLabelIF.h"
#include "PopupBaseView.h"

class ConfirmDialogView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner

{
public:
    static ConfirmDialogView* create(int food,int wood,int iron,int stone,int money,int gold,CCObject* callTarget,SEL_CallFunc callunc, string tips="", string btnName="");
    ConfirmDialogView(int food,int wood,int iron,int stone,int money,int gold,CCObject* callTarget,SEL_CallFunc callunc, string tips="", string btnName=""):m_food(food),
    m_wood(wood),
    m_iron(iron),
    m_stone(stone),
    m_money(money),
    m_gold(gold),
    m_callbackTarget(callTarget),
    m_callbackFunc(callunc),
    m_tips(tips),
    m_btnName(btnName)
    {};
    ~ConfirmDialogView(){};
protected:
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){};
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void keyPressedBtnOk(CCObject * pSender, Control::EventType pCCControlEvent);
    void keyPressedBtnCancel(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCLabelIF> m_tipTxt;
    CCSafeObject<CCLabelIF> m_needTip;
    CCSafeObject<CCLabelIF> m_btnTxt;
    CCSafeObject<CCLabelIF> m_useGoldTxt;
    CCSafeObject<CCNode> m_resNode;
    CCSafeObject<CCNode> m_bg;
    CCSafeObject<CCControlButton> m_btnOk;
    CCObject *m_callbackTarget;
    SEL_CallFunc m_callbackFunc;
    long m_food;
    long m_wood;
    long m_iron;
    long m_stone;
    long m_money;
    long m_gold;
    string m_tips;
    string m_btnName;
};
#endif /* defined(__IF__ConfirmDialogView__) */
