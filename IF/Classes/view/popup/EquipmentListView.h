//
//  EquipmentListView.h
//  IF
//
//  Created by fubin on 14-12-18.
//
//

#ifndef __IF__EquipmentListView__
#define __IF__EquipmentListView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCClipNode.h"

#define EQUIP_SEL_DATA_REFRESH "equip.sel.data.refresh"

class EquipmentListView: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCTableViewDataSource
, public DefaultTableViewDelegate
{
public:
    static EquipmentListView* create(int site);
    static bool antiCompareEqui(int item1, int item2);
    CCNode* getGuideNode(string _key);
protected:
    // scrollview delegate
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(int site);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    vector<int> SelEquipmentsByPlayerLv();
    bool isInForging(int idx);
    bool isLevelFirst(int idx);
    void refreshData(CCObject* p);
    void generateData();
    bool isForgeWork();//判断当前是否有装备在锻造
    float calListCellHeight(CCTableView *table,int idx);
    
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCTableView> m_tabView;

    vector<int> m_data;
    int m_weapType;
    int m_mateType;
    bool m_isForgeWork;
    CCPoint offsetPt;
    float miny;
};

//...cell class
class EquipmentListCell :
public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewCell
{
public:
    static EquipmentListCell* create(int itemId, bool isLvFirst, bool isInForging, bool isForgeWork);
    EquipmentListCell():m_itemId(0), m_isLvFirst(false), m_isInForging(false), m_isForgeWork(false){};
    void setData(int itemId, bool isLvFirst, bool isInForging, bool isForgeWork);
    void cellTouchEnded(CCTouch* pTouch);
    int m_itemId;
    bool m_isLvFirst;//1表示当前等级第一个 0表示其他
    bool m_isInForging;//1自己正在锻造 0自己没在锻造
    bool m_isForgeWork;//1有装备正在锻造  0没有装备正在锻造
    
    CCSafeObject<CCControlButton> m_forgeBtn;
    virtual void update(float time);
private:
    bool init(int itemId, bool isLvFirst, bool isInForging, bool isForgeWork);
    virtual void onEnter();
    virtual void onExit();
    
    void refreashData(CCObject* obj);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    void onForgeBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onViewBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void setLabelTag();
    
    
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_levelLabel;
    CCSafeObject<CCLabelIF> m_infoLabel1;
    CCSafeObject<CCLabelIF> m_infoLabel2;
    CCSafeObject<CCLabelIF> m_infoLabel3;
    CCSafeObject<CCLabelIF> m_infoLabel4;
//    CCSafeObject<CCLabelIF> m_headLvLabel;
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCSprite> m_topSpr;
    CCSafeObject<CCSprite> m_bottomSpr;
    CCSafeObject<CCScale9Sprite> m_bgSpr;
    CCSafeObject<CCScale9Sprite> m_bgSpr2;
//    CCSafeObject<CCSprite> m_headLvBg;
    CCSafeObject<CCNode> m_mainNode2;
    CCSafeObject<CCControlButton> m_viewBtn;
    
    CCSafeObject<CCNode> m_proNode;
    CCSafeObject<CCScale9Sprite> m_progressBG;
    CCSafeObject<CCScale9Sprite> m_progrossBar;
    CCSafeObject<CCLabelIF> m_timeText;
    CCSafeObject<CCClipNode>  m_barClipNode;
    
    CCSafeObject<CCNode> m_infoNode;
    CCSafeObject<CCLabelIF> m_noEnoughMaterialLb;
};

class SelTipView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
 
{
public:
    static SelTipView* create(int weapType, int mateType);
    SelTipView(){};
    ~SelTipView(){};
    
private:
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(int weapType, int mateType);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onClickOKBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickCancelBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCControlButton> m_okBtn;
    CCSafeObject<CCControlButton> m_cancelBtn;
    CCSafeObject<CCSprite> m_weapOk;
    CCSafeObject<CCSprite> m_mateOK;
    
    CCSafeObject<CCLabelIF> m_titleLabel;
    map<int, CCNode*> m_weapNodes;
    map<int, CCNode*> m_mateNodes;
    
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScrollView> m_scrollView;
    
    int m_weapType;
    int m_mateType;
};

#endif /* defined(__IF__EquipmentListView__) */
