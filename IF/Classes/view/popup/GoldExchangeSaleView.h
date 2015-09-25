//
//  GoldExchangeSaleView.h
//  IF
//
//  Created by lifangkai on 14-9-10.
//
//

#ifndef __IF__GoldExchangeSaleView__
#define __IF__GoldExchangeSaleView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "CCLabelIF.h"

class GoldExchangeSaleView : public Layer
, public CCBSelectorResolver
  
, public CCBMemberVariableAssigner
{
public:
    GoldExchangeSaleView(){
        
    }
    GoldExchangeSaleView(GoldExchangeItem *goldExchangeItem, int parentType){
        m_dataItem = goldExchangeItem;
        m_parentType = parentType;
        m_parentScrollView = NULL;
        
    }
    static GoldExchangeSaleView* create(GoldExchangeItem *goldExchangeItem, int parentType);
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
    
    void onCloseBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickCostBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onMoreBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickGivePackageBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void goToExchangeCommamand();
    void onEnterFrame(float dt);
    
    void errorPayment(CCObject* obj = NULL);
    void onShowError(float dt);
    
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelBMFont> m_percentLabel;
    CCSafeObject<CCLabelBMFont> m_getGoldNumText;
    CCSafeObject<CCLabelIF> m_timeLabel;
    
    CCSafeObject<CCLabelIF> m_oldPriceLabel;
    CCSafeObject<CCLabelIF> m_newPriceLabel;
    CCSafeObject<CCLabelIF> m_getLabel;
    CCSafeObject<CCLabelIF> m_getLabel1;
    CCSafeObject<CCLabelIF> m_moreLabel;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_touchMoreNode;
    CCSafeObject<CCNode> m_moreNode;
    CCSafeObject<CCControlButton> m_costBtn;
    CCSafeObject<CCControlButton> m_moreBtn;
    CCSafeObject<CCSprite> m_soleOutSpr;
    CCSafeObject<CCSprite> m_moreSpr;
    
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCNode> m_animNode;

    CCSafeObject<CCNode> m_nodeBuy;
    CCSafeObject<CCControlButton> m_btnGivePackage;
    CCPoint m_startPoint;
    GoldExchangeItem *m_dataItem;
    CCScrollView* m_parentScrollView;
    int m_parentType;
};

class GoldExchangeSaleCell :
public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static GoldExchangeSaleCell* create(int itemId, int num,string popUpImage,int type);
    GoldExchangeSaleCell(){};
private:
    bool init(int itemId, int num,string popUpImage,int type);
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<CCNode> m_iconNode;
};

class GoldExchangeFirstPayCell : public Layer
, public CCBSelectorResolver
  
, public CCBMemberVariableAssigner
{
public:
    GoldExchangeFirstPayCell(){
    }
    GoldExchangeFirstPayCell(GoldExchangeItem *goldExchangeItem, int parentType){
        m_dataItem = goldExchangeItem;
        m_parentType = parentType;
        m_parentScrollView = NULL;
        
    }
    static GoldExchangeFirstPayCell* create(GoldExchangeItem *goldExchangeItem, int parentType);
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
    
    void errorPayment(CCObject* obj = NULL);
    void onShowError(float dt);
    
    CCSafeObject<CCLabelBMFont> m_titleLabel;
    CCSafeObject<CCLabelBMFont> m_getGoldNumText;
    CCSafeObject<CCLabelIF> m_timeLabel;
    
    CCSafeObject<CCLabelIF> m_titleLabel1;
    CCSafeObject<CCLabelIF> m_getLabel;
    CCSafeObject<CCLabelIF> m_percentLabel;
    CCSafeObject<CCLabelIF> m_doubleLabel;
    CCSafeObject<CCLabelIF> m_priceLabel3;
    CCSafeObject<CCLabelIF> m_buttonLabel;
    CCSafeObject<CCControlButton> m_costBtn;
    CCSafeObject<CCNode> m_touchNode;
    
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCNode> m_animNode;
    
    CCPoint m_startPoint;
    GoldExchangeItem *m_dataItem;
    CCScrollView* m_parentScrollView;
    int m_parentType;
};

//...............//
class MonthCardPayCell : public Layer
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    ~MonthCardPayCell(){
    }
    MonthCardPayCell(GoldExchangeItem *goldExchangeItem):m_touchSprite(NULL),m_touchNode(NULL),m_lblTitle(NULL){
        ignoreAnchorPointForPosition(false);
        setAnchorPoint(Vec2(0,0));
        m_dataItem = goldExchangeItem;
    }
    static MonthCardPayCell* create(GoldExchangeItem *goldExchangeItem);
    CCRect getCostBtnRect();
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
    void refreshData();
    void onEnterFrame(float dt);
    void onClickCostBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onTipClick(CCObject *pSender, CCControlEvent event);
    void onCloseBtnClick(CCObject *pSender, CCControlEvent event);
    CCSafeObject<CCLabelBMFont> m_percentLabel;
    CCSafeObject<CCLabelBMFont> m_desLabel;
    CCSafeObject<CCLabelIF> m_lblItem[2];
    CCSafeObject<CCNode> m_nodeItem[2];
    CCSafeObject<CCLabelIF> m_moreLabel;
    CCSafeObject<CCControlButton> m_costBtn;
    CCSafeObject<CCControlButton> m_btnTip;
    CCSafeObject<CCSprite> m_touchSprite;
    CCSafeObject<CCLabelIF> m_lblTitle;
    GoldExchangeItem *m_dataItem;
    CCSafeObject<CCNode> m_touchNode;
    CCPoint m_touchLocation;
};

#endif /* defined(__IF__GoldExchangeSaleView__) */
