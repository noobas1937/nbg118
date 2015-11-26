//
//  GoldExchangeAdvertisingView.h
//  IF
//
//  Created by lifangkai on 14-10-29.
//
//

#ifndef __IF__GoldExchangeAdvertisingView__
#define __IF__GoldExchangeAdvertisingView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"

#define GOLDEXCHANGE_ADVERTISING_SCROLL "goldexchange.advertising.scroll"
#define GOLDEXCHANGE_ADVERTISING_STOP_SCROLL "goldexchange.advertising.stop.scroll"

class GoldExchangeAdvertisingView: public Layer
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
//, public CCTouchDelegate
,public CCTableViewDataSource
,public DefaultTableViewDelegate

{
public:
    static GoldExchangeAdvertisingView* create();
protected:
    // scrollview delegate
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    virtual void scrollViewDidScroll(CCScrollView* view);
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onClickCostBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void refreshView(CCObject* obj);
    void onPlayEnterFrame(float dt);
    void recoverScroll(CCObject* obj);
    void stopScroll(CCObject* obj);

    void errorPayment(CCObject* obj = NULL);
    void onShowError(float dt);
    CCSafeObject<CCLabelIF> m_getLabel;
    CCSafeObject<CCLabelIF> m_newPriceLabel;
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCNode> m_titleNode;
    CCSafeObject<CCNode> m_scrollContainer;
    CCSafeObject<CCControlButton> m_costBtn;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_pageNode;
    CCSafeObject<CCSprite> m_pageflag;
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCNode> m_animNode;
    CCTableView* m_tabView;

    void generateData();
    void generalDataAgain(float _time);
    void generalEndDataAgain(float _time);
    void setButtonState(int offX,bool isreset = true);
    void addButtonLight(bool bAdd);
    void showButtonLight(float _time);
    int m_x;
    int m_startX;
    int m_endX;

    GoldExchangeItem* m_dataItem;
};

//.............//
class GoldExchangeAdvertisingCell:
public CCTableViewCell
{
public:
    static GoldExchangeAdvertisingCell *create(GoldExchangeItem* item);
    GoldExchangeAdvertisingCell(GoldExchangeItem* item) : m_item(item) {};
    void setData(GoldExchangeItem* item);
private:
    void refreshView();
    bool init();

    
    virtual void onEnter();
    virtual void onExit();
    GoldExchangeItem* m_item;
    
    CC_SYNTHESIZE(CCRect, m_costBtnRect, CostBtnRect);
};
//...............//
class GoldExchangeHallweenCell : public Layer
, public CCBSelectorResolver
  
, public CCBMemberVariableAssigner
{
public:
    ~GoldExchangeHallweenCell(){
    }
    GoldExchangeHallweenCell(GoldExchangeItem *goldExchangeItem){
        m_dataItem = goldExchangeItem;
    }
    static GoldExchangeHallweenCell* create(GoldExchangeItem *goldExchangeItem);
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
    void onClickCostBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickGivePackageBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCLabelBMFont> m_getGoldNumText;
    CCSafeObject<CCLabelIF> m_percentLabel;
    CCSafeObject<CCLabelIF> m_timeLabel;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCLabelIF> m_moreLabel;
    CCSafeObject<CCLabelIF> m_getLabel;
    CCSafeObject<CCLabelIF> m_newPriceLabel;
    CCSafeObject<CCNode> m_touchMoreNode1;
    CCSafeObject<CCNode> m_touchMoreNode2;
    CCSafeObject<CCControlButton> m_costBtn;
    CCSafeObject<CCNode> m_animNode;
    CCSafeObject<CCControlButton> m_btnGivePackage;
    GoldExchangeItem *m_dataItem;

    CCPoint m_touchLocation;
};
//...............//
class GoldExchangeAdvertisingCommCell : public Layer
, public CCBSelectorResolver
  
, public CCBMemberVariableAssigner
{
public:
    ~GoldExchangeAdvertisingCommCell(){
    }
    GoldExchangeAdvertisingCommCell(GoldExchangeItem *goldExchangeItem){
        m_dataItem = goldExchangeItem;
        
    }
    static GoldExchangeAdvertisingCommCell* create(GoldExchangeItem *goldExchangeItem);
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
    void onClickCostBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickGivePackageBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCLabelBMFont> m_getGoldNumText;
    CCSafeObject<CCLabelIF> m_percentLabel;
     CCSafeObject<CCLabelIF> m_percentLabel2;
    CCSafeObject<CCLabelIF> m_timeLabel;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCLabelIF> m_moreLabel;
    CCSafeObject<CCLabelIF> m_getLabel;
    CCSafeObject<CCLabelIF> m_newPriceLabel;
    CCSafeObject<CCNode> m_touchMoreNode1;
    CCSafeObject<CCNode> m_touchMoreNode2;
    CCSafeObject<CCNode> m_showMoreNode;
    CCSafeObject<CCControlButton> m_costBtn;
    CCSafeObject<CCNode> m_animNode;
    CCSafeObject<CCNode> m_showMoneyNode;
    CCSafeObject<CCControlButton> m_btnGivePackage;
    GoldExchangeItem *m_dataItem;
    
    CCPoint m_touchLocation;
};
#endif /* defined(__IF__GoldExchangeAdvertisingView__) */
