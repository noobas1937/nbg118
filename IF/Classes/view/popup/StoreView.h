//
//  StoreView.h
//  IF
//
//  Created by fubin on 14-2-27.
//
//

#ifndef IF_StoreView_h
#define IF_StoreView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"
#include "DefaultTableViewDelegate.h"
#include "BackpackView.h"
#include "ToolInfo.h"
#include "StoreBagView.h"
#include "StoreMallView.h"
#include "CCTableViewTouchIFCell.h"
class ToolCell;
class ToolRewardCell;

class StoreView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static StoreView* create(StoreType storeType = STORE_TYPE_STORE,
                             StoreResourceType storeResourceType = STORE_RESOURCE_TYPE_WAR);
    StoreView(StoreType storeType = STORE_TYPE_STORE,
              StoreResourceType storeResourceType = STORE_RESOURCE_TYPE_WAR):m_type(storeType),m_page(storeResourceType),m_cityLv(0){};
    virtual ~StoreView();
    void updateInfo();
    CCNode* getGuideNode(string _key);
    static bool sortByUse(int itemId1, int itemId2);
    static bool sortByUseOder(int itemId1, int itemId2);
    CCRect getBagTabRect();
    void onClickStoreBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickBagBtn(CCObject * pSender, Control::EventType pCCControlEvent);
protected:
    // scrollview delegate
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    void refreashData();
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onCloseClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void onClickWarBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickSpdBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickResBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickOthBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void sortData();
    void sortDataByOrderNum();
   //
    
    CCSafeObject<CCControlButton> m_closeBtn;
    CCSafeObject<CCControlButton> m_storeBtn;
    CCSafeObject<CCControlButton> m_bagBtn;
    CCSafeObject<CCControlButton> m_warBtn;
    CCSafeObject<CCControlButton> m_spdBtn;
    CCSafeObject<CCControlButton> m_resBtn;
    CCSafeObject<CCControlButton> m_othBtn;
    CCSafeObject<CCScale9Sprite> m_BGNode;
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelIF> m_msgLabel;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCNode> m_bagNode;
    CCSafeObject<CCNode> m_mallNode;
    CCSafeObject<StoreBagView>m_bagView;
    CCSafeObject<StoreMallView>m_mallView;
    
//    CCSafeObject<CCMultiColTableView > m_tabView;
    CCSafeObject<CCTableView> m_tabView;
    
    
    int m_type;
    int m_page;
    std::vector<int> m_curList;
    std::vector<int>* m_dataList;
    int m_cityLv;
};

class ToolCell :
public CCBSelectorResolver
 
,public CCBMemberVariableAssigner
,public CCTableViewTouchIFCell
{
public:
    static ToolCell* create(int itemId, int type, CCNode* touchNode);
    ToolCell():m_itemId(0),m_type(0),m_price(0),m_boostBid(0),m_touchNode(NULL){};
    void setData(int itemId, int type);
private:
    bool init(int itemId, int type, CCNode* touchNode);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void refreashData(CCObject* obj);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickUseBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickBuyBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickStoreUseBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onYes();
    void onUseTool();
    void onBuyTool();
    void onRetBuyTool();
    void onBoostBuild();
    
    CCSafeObject<CCControlButton> m_useBtn;
    CCSafeObject<CCControlButton> m_buyBtn;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCLabelIF> m_inBtnLabel;
    
    CCSafeObject<CCLabelIF> m_priceLabel;
    CCSafeObject<CCNode> m_priceNode;
    
    CCSafeObject<CCScale9Sprite> m_itemBg;
    CCSafeObject<CCControlButton> m_storeUseBtn;
    CCSafeObject<CCControlButton> m_unUseBtn;
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCNode> m_rewardNode;
    
    CCNode* m_touchNode;
    bool m_clickInSide;
    int m_itemId;
    int m_type;
    int m_price;
    int m_boostBid;
    WaitInterface* m_waitInterface;
};

class ToolRewardCell :
public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCNode
{
public:
    static ToolRewardCell* create(int r_type, int itemId, int num);
    ToolRewardCell():m_itemId(0),m_type(0),m_num(0){};
private:
    bool init(int r_type, int itemId, int num);
    virtual void onEnter();
    virtual void onExit();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<CCNode> m_picNode;
    
    int m_itemId;
    int m_type;
    int m_num;
};

#endif
