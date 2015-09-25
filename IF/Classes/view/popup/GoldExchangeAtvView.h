//
//  GoldExchangeAtvView.h
//  IF
//
//  Created by zhaohf on 14-4-16.
//
//

#ifndef __IF__GoldExchangeAtvView__
#define __IF__GoldExchangeAtvView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "CCLabelIF.h"

class GoldExchangeAtvView : public Layer
, public CCBSelectorResolver
  
, public CCBMemberVariableAssigner
{
public:
    GoldExchangeAtvView(){
         _ignoreAnchorPointForPosition = false;
        setAnchorPoint(Vec2(0,0));
    }
    GoldExchangeAtvView(GoldExchangeItem *goldExchangeItem, int parentType){
        m_dataItem = goldExchangeItem;
        m_parentType = parentType;
        m_parentScrollView = NULL;
    }
    static GoldExchangeAtvView* create(GoldExchangeItem *goldExchangeItem, int parentType);
    void setParentScrollView(CCScrollView* parentScrollView);
private:
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void onClickCostBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onMoreBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void goToExchangeCommamand();
    void onEnterFrame(float dt);
    
    CCSafeObject<CCLabelBMFont> m_titleLabel;
    CCSafeObject<CCLabelBMFont> m_percentLabel;
    CCSafeObject<CCLabelIF> m_getGoldNumText;
    CCSafeObject<CCLabelIF> m_timeLabel;
    
    CCSafeObject<CCLabelIF> m_oldPriceLabel;
    CCSafeObject<CCLabelIF> m_newPriceLabel;
    CCSafeObject<CCLabelIF> m_getLabel;
    CCSafeObject<CCLabelIF> m_moreLabel;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_touchMoreNode;
    CCSafeObject<CCControlButton> m_costBtn;
    CCSafeObject<CCNode> m_animNode;
   // CCSafeObject<CCControlButton> m_moreBtn;
    CCSafeObject<CCSprite> m_soleOutSpr;
    
    CCSafeObject<CCScrollView> m_scrollView;
    
    GoldExchangeItem *m_dataItem;
    CCScrollView* m_parentScrollView;
    CCPoint m_startPoint;
    int m_parentType;
};

class RechargeCell :
public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static RechargeCell* create(int itemId, int num,int type);
    RechargeCell(){};
private:
    bool init(int itemId, int num,int type);
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<CCNode> m_iconNode;
};

#endif /* defined(__IF__GoldExchangeAtvView__) */
