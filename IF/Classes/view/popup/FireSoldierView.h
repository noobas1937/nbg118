//
//  FireSoldierView.h
//  LastEmpire
//
//  Created by zhangjun on 2015-03-19 10:33:42.
//
//
		
#ifndef __LastEmpire__FireSoldierView__
#define __LastEmpire__FireSoldierView__
		
#include "CommonInclude.h"
#include "CCBExtension.h"
#include "CCLabelIF.h"
#include "PopupBaseView.h"
//#include "ArcPopupBaseView.h"
#include "CCSliderBar.h"
		
class FireSoldierView :
public PopupBaseView,
//public ArcPopupBaseView,
public CCBSelectorResolver,
public CCBMemberVariableAssigner,
//public cocos2d::CCTargetedTouchDelegate,
public CCEditBoxDelegate
{
public:
    FireSoldierView(TreatInfo *info):m_info(info){};
    virtual ~FireSoldierView();
    virtual bool init();
    virtual void onExit();
    virtual void onEnter();
    static FireSoldierView* create(TreatInfo *info);
    
public:
    bool onAssignCCBMemberVariable(CCObject * pTarget, const char* pMemberVariableName, CCNode * pNode);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    virtual SEL_CallFuncN onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName);
		
protected:
	void onAddClick(CCObject * pSender, CCControlEvent pCCControlEvent);
	void onSubClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onFireBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){};
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    void editBoxReturn(CCEditBox *editBox);
    
    void sliderCallBack(CCObject*sender,CCControlEvent even);
    
    void refreshNum(CCObject* p = NULL);
    void goToDismissAction();
protected:
    CCSafeObject<CCNode> m_editBoxNode;
    CCSafeObject<CCNode> m_sliderContainer;
    CCSafeObject<CCNode> m_sprBG;
    CCSafeObject<CCNode> m_subNode;
    CCSafeObject<CCControlButton> m_addBtn;
    CCSafeObject<CCControlButton> m_subBtn;
    CCSafeObject<CCLabelIF> m_numMaxText;
    CCSafeObject<CCControlButton> m_fireBtn;
    CCSafeObject<CCLabelIF> m_fireLabel;
    
    CCSafeObject<CCSliderBar> m_fireSlider;
    CCSafeObject<CCEditBox> m_editBox;
    CCSafeObject<TreatInfo> m_info;
    
    int m_fireNum;
    int m_totalNum;
};

//end 3
		
#endif /* defined(__LastEmpire__FireSoldierView__) */
		
