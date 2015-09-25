//
//  UseBackpackItemView.h
//  IF
//
//  Created by ganxiaohua on 14-7-24.
//
//

#ifndef __IF__UseBackpackItemView__
#define __IF__UseBackpackItemView__

#include "CommonInclude.h"
#include "PopupBaseView.h"

class UseBackpackItemView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
{
public:
    static UseBackpackItemView *create(int itemId);
    UseBackpackItemView(int itemId):m_itemId(itemId){};
    ~UseBackpackItemView(){};
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onUseClick(CCObject *pSender, CCControlEvent event);
    void onUseAndBuyClick(CCObject *pSender, CCControlEvent event);
    void onYes();
    void onRetBuyTool();
    void onUseTool();
    
    CCSafeObject<CCControlButton> m_useBtn;
    CCSafeObject<CCControlButton> m_useAnBuyBtn;
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCLabelIF> m_desc;
    CCSafeObject<CCLabelIF> m_numTxt;
    CCSafeObject<CCLabelIF> m_btnMsgLabel;
    CCSafeObject<CCLabelIF> m_inBtnGoldNum;
    CCSafeObject<CCNode> m_goldNode;
    CCSafeObject<CCNode> m_itemNode;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCNode> m_bg;
    int m_itemId;
    int m_type;
};
#endif /* defined(__IF__UseBackpackItemView__) */
