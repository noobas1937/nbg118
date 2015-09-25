//
//  AllianceShopView.h
//  IF
//
//  Created by lifangkai on 14-9-1.
//
//

#ifndef __IF__AllianceShopView__
#define __IF__AllianceShopView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"
#include "DefaultTableViewDelegate.h"
#include "BackpackView.h"
#include "ToolInfo.h"
#include "CCSliderBar.h"

#include "CCTableViewTouchIFCell.h"
class AllianceShopCell;

class AllianceShopView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static AllianceShopView* create(StoreType storeType = STORE_TYPE_STORE,
                             StoreResourceType storeResourceType = STORE_RESOURCE_TYPE_WAR);
    AllianceShopView(StoreType storeType = STORE_TYPE_STORE,
              StoreResourceType storeResourceType = STORE_RESOURCE_TYPE_WAR):m_type(storeType),m_page(storeResourceType),m_cityLv(0){};
    virtual ~AllianceShopView();
    void updateInfo(CCObject* p = NULL);
    static bool sortByUse(int itemId1, int itemId2);
    CCRect getStoreBtnRec();
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
    void onClickStoreBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickBagBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onTipBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onHelpBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    void sortData();
    void getData();
    void refreshPoint(CCObject* p);
    
    
    //
        CCSafeObject<CCLabelIF> m_noMSG;
    CCSafeObject<CCControlButton> m_closeBtn;
    CCSafeObject<CCControlButton> m_storeBtn;
    CCSafeObject<CCControlButton> m_bagBtn;
    CCSafeObject<CCControlButton> m_tipBtn;

    CCSafeObject<CCLabelIF> m_pointLabel;
    CCSafeObject<CCLabelIF> m_pointNumLabel;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCSprite> m_allianceContributionSpr;
    CCSafeObject<CCSprite> m_alliancePointSpr;
    
    CCSafeObject<CCTableView> m_tabView;
    
    CCSafeObject<CCLabelIF> m_titleTxt;
    
    int m_type;
    int m_page;
    bool m_isTab;
    std::vector<int> m_curList;
   // std::vector< vector<int> > m_data;
    std::vector<int>* m_dataList;
    int m_cityLv;
};

class AllianceShopCell :
public CCBSelectorResolver
 
,public CCBMemberVariableAssigner
,public CCTableViewTouchIFCell
{
public:
    static AllianceShopCell* create(int index, int type, vector<int> &cellData);
    AllianceShopCell(int index, int type, vector<int> &cellData){
        m_index = index;
        m_type = type;
        m_cellData = cellData;
    };
    void setData(int index, int type, vector<int> &cellData);
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void refreashData(CCObject* obj);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickUseBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickBuyBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickStoreUseBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onUseAction();
    void onBuyAction();
    void onUseTool(CCObject* obj);
    void onRetBuyTool(CCObject* obj);

    void onBuyAction(CCObject*);
    void onUseAction(CCObject*);
    
    //CCSafeObject<CCControlButton> m_useBtn;
   // CCSafeObject<CCControlButton> m_buyBtn;
    
    CCSafeObject<CCLabelIF> m_nameText1;
    CCSafeObject<CCLabelIF> m_numText1;
    CCSafeObject<CCLabelIF> m_priceText1;
    CCSafeObject<CCNode> m_picNode1;
    CCSafeObject<CCNode> m_hintNode1;
    CCSafeObject<CCNode> m_downnode1;
    CCSafeObject<CCNode> m_upnode1;
    
    CCSafeObject<CCLabelIF> m_nameText2;
    CCSafeObject<CCLabelIF> m_numText2;
    CCSafeObject<CCLabelIF> m_priceText2;
    CCSafeObject<CCNode> m_picNode2;
    CCSafeObject<CCNode> m_hintNode2;
    CCSafeObject<CCNode> m_downnode2;
    CCSafeObject<CCNode> m_upnode2;
    
    CCSafeObject<CCLabelIF> m_nameText3;
    CCSafeObject<CCLabelIF> m_numText3;
    CCSafeObject<CCLabelIF> m_priceText3;

    CCSafeObject<CCNode> m_picNode3;
    CCSafeObject<CCNode> m_hintNode3;
    CCSafeObject<CCNode> m_downnode3;
    CCSafeObject<CCNode> m_upnode3;
    
    CCSafeObject<CCSprite> m_nameBGSpr1;
    CCSafeObject<CCSprite> m_nameBGSpr2;
    CCSafeObject<CCSprite> m_nameBGSpr3;
    CCSafeObject<CCSprite> m_nunBGSpr1;
    CCSafeObject<CCSprite> m_nunBGSpr2;
    CCSafeObject<CCSprite> m_nunBGSpr3;
    
    CCSafeObject<CCSprite> m_allianceContributionSpr1;
    CCSafeObject<CCSprite> m_alliancePointSpr1;
    
    CCSafeObject<CCSprite> m_allianceContributionSpr2;
    CCSafeObject<CCSprite> m_alliancePointSpr2;
    
    CCSafeObject<CCSprite> m_allianceContributionSpr3;
    CCSafeObject<CCSprite> m_alliancePointSpr3;
    
    std::vector<int> m_cellData;
    
//    CCSafeObject<CCLabelIF> m_priceLabel;
//    CCSafeObject<CCNode> m_priceNode;
    
//    CCSafeObject<CCScale9Sprite> m_itemBg;
//    CCSafeObject<CCControlButton> m_storeUseBtn;
//    CCSafeObject<CCControlButton> m_unUseBtn;
//    CCSafeObject<CCNode> m_mainNode;
//    CCSafeObject<CCNode> m_rewardNode;
    
    CCNode* m_touchNode;
    bool m_clickInSide;
    int m_itemId;
    int m_type;
    int m_price;
    int m_index;
    CCPoint m_curPt;
    int m_pos;
};

class AllToolNumSelectView: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCEditBoxDelegate
{
public:
    static AllToolNumSelectView* create(int itemid);
    AllToolNumSelectView(int itemid): m_itemId(itemid){};
    ~AllToolNumSelectView(){};
    
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);
private:
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onAddClick(CCObject *pSender, CCControlEvent event);
    void onSubClick(CCObject *pSender, CCControlEvent event);
    void onUseClick(CCObject *pSender, CCControlEvent event);
    void moveSlider(CCObject * pSender, CCControlEvent pCCControlEvent);
    void setNumString();
    void sureBuy();

    CCSafeObject<CCSliderBar> m_slider;
    
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCNode> m_subNode;
    CCSafeObject<CCLabelIF> m_tipLabel;
    CCSafeObject<CCControlButton> m_useBtn;
    CCSafeObject<CCLabelIF> m_btnLabel;
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<CCNode> m_sprNode;
    
    CCSafeObject<CCNode> m_editNode;
    CCSafeObject<CCNode> m_barNode;
    CCSafeObject<CCControlButton> m_subBtn;
    CCSafeObject<CCControlButton> m_addBtn;
    CCSafeObject<CCSprite> m_iconBg;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCNode> m_touchNode;
    
    CCSafeObject<CCEditBox> m_editBox;
    
    CC_SYNTHESIZE_SAFEOBJECT(CCCallFuncO, m_onYes, YesCallback);
    
    bool m_invalidSlider;
    int m_itemId;
    int m_numAll;
    int m_numCurrent;
    
};

#endif /* defined(__IF__AllianceShopView__) */
