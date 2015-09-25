//
//  AlliianceTerritoryView.h
//  IF
//
//  Created by 〜TIAN~ on 15/4/9.
//
//

#ifndef __IF__AlliianceTerritoryView__
#define __IF__AlliianceTerritoryView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"
#include "DefaultTableViewDelegate.h"
#include "BackpackView.h"
#include "ToolInfo.h"
#include "AllianceTerritoryInfo.h"
#include "CCTableViewTouchIFCell.h"
class AllianceTerritoryInfoCell;
class AllianceTerritoryCell;

class AllianceTerritoryView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static AllianceTerritoryView* create(unsigned int target, bool isInTheWorld);
    AllianceTerritoryView(unsigned int target, bool isInTheWorld):m_targetIndex(target),m_isInTheWorld(isInTheWorld){};
    virtual ~AllianceTerritoryView();
    void updateInfo(CCObject* data);

protected:
    // scrollview delegate
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    //virtual void scrollViewDidScroll(CCScrollView* view);
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void refresh(CCObject* param);
    void addLoadingAni();
    void removeLoadingAni(CCObject* p = NULL);
    void onTipBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCSprite> m_loadingIcon;
    CCSafeObject<CCArray> m_territoryData;//哨塔
    CCSafeObject<CCArray> m_resourceData;//超级矿
    CCSafeObject<CCArray> m_towerData;//箭塔
    CCSafeObject<CCArray> m_warehouseData;//仓库
    CCSafeObject<CCArray> m_bannerData;//国旗
    CCSafeObject<CCArray> m_showData;//显示部分
    CCSafeObject<CCControlButton> m_tipBtn;
    unsigned int m_targetIndex;
    int m_territoryTotalNum;//哨塔总数
    int m_resourceTotalNum;//超级矿总数
    int m_towerTotalNum;//箭塔总数
    int m_warehouseTotalNum;//仓库总数
    int m_bannerTotalNum;//国旗总数
    vector<int> m_dataList;
    bool m_isInTheWorld;
    bool m_territoryActive;
};

class AllianceTerritoryInfoCell :
public CCBSelectorResolver
 
,public CCBMemberVariableAssigner
,public CCTableViewTouchIFCell
{
public:
    static AllianceTerritoryInfoCell* create(int index, int type, bool tipFlag);
    AllianceTerritoryInfoCell(bool tipFlag,int index,int type,bool canTouch,int startY,int curY):m_tipFlag(tipFlag),m_index(index),m_type(type),m_canTouch(canTouch),m_startY(startY),m_curY(curY){};
    void setData(int index, int type);
    void bind(int num, int index, unsigned int target, AllianceTerritoryInfo* info1, AllianceTerritoryInfo* info2, WorldCityType type);
private:
    bool init(int index, int type);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickOpen(float rat);
    void onResetCanTouch();
    
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCNode> m_myTouchNode;
    CCSafeObject<CCSprite> m_jiantouSpr;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCLabelIF> m_tipLabel;
    CCSafeObject<CCNode> m_tipNode;
    CCSafeObject<CCScale9Sprite> m_tipBg;
    int m_index;
    int m_type;
    bool m_canTouch;
    int m_startY;
    int m_curY;
    bool m_tipFlag;
};

class AllianceTerritoryCell :
public CCBSelectorResolver
 
,public CCBMemberVariableAssigner
,public CCIFTouchNode
{
public:
    static AllianceTerritoryCell* create(int num, int index, unsigned int target, AllianceTerritoryInfo* info1, AllianceTerritoryInfo* info2, WorldCityType type);
    AllianceTerritoryCell(int num, int index, unsigned int target, AllianceTerritoryInfo* info1, AllianceTerritoryInfo* info2, WorldCityType type){
        m_totalNum = num;
        m_index = index;
        m_targetIndex = target;
        m_type = type;
        
    };
    void setData(int index, AllianceTerritoryInfo* info1, AllianceTerritoryInfo* info2, WorldCityType type);
private:
    bool init(AllianceTerritoryInfo* info1, AllianceTerritoryInfo* info2);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void refreashData(CCObject* obj);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void OnPlaceBtnClick1(CCObject * pSender, Control::EventType pCCControlEvent);
    void OnPlaceBtnClick2(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCLabelIF> m_nameLabel1;
    CCSafeObject<CCLabelIF> m_placeLabel1;
    CCSafeObject<CCLabelIF> m_statPlaceLabel1;
    CCSafeObject<CCLabelIF> m_defLabel1;
    CCSafeObject<CCLabelIF> m_coordLabel1;
    CCSafeObject<CCLabelIF> m_statUnplaceLabel1;
    CCSafeObject<CCLabelIF> m_lockTipLabel1;
    
    CCSafeObject<CCLabelIF> m_nameLabel2;
    CCSafeObject<CCLabelIF> m_placeLabel2;
    CCSafeObject<CCLabelIF> m_statPlaceLabel2;
    CCSafeObject<CCLabelIF> m_defLabel2;
    CCSafeObject<CCLabelIF> m_coordLabel2;
    CCSafeObject<CCLabelIF> m_statUnplaceLabel2;
    CCSafeObject<CCLabelIF> m_lockTipLabel2;
    
    CCSafeObject<CCNode> m_infoBgPlace1;
    CCSafeObject<CCNode> m_statBgPlace1;
    CCSafeObject<CCSprite> m_coordIcon1;
    CCSafeObject<CCScale9Sprite> m_underlineSpr1;
    CCSafeObject<CCSprite> m_lockNode1;
    
    CCSafeObject<CCNode> m_infoBgPlace2;
    CCSafeObject<CCNode> m_statBgPlace2;
    CCSafeObject<CCSprite> m_coordIcon2;
    CCSafeObject<CCScale9Sprite> m_underlineSpr2;
    CCSafeObject<CCSprite> m_lockNode2;
    
    CCSafeObject<CCNode> m_coordNode1;
    CCSafeObject<CCNode> m_coordNode2;
    CCSafeObject<CCNode> m_tNode1;
    CCSafeObject<CCNode> m_tNode2;
    CCSafeObject<CCNode> m_picNode1;
    CCSafeObject<CCNode> m_picNode2;
    CCSafeObject<CCNode> m_hintBGNode;
    CCSafeObject<CCNode> m_bgNode1;
    CCSafeObject<CCNode> m_bgNode2;
    
    CCSafeObject<CCControlButton> m_placeBtn1;
    CCSafeObject<CCControlButton> m_placeBtn2;

    unsigned int m_targetIndex;
    int m_index;
    int m_ptIndex1;
    int m_ptIndex2;
    int m_totalNum;
    bool m_btnFlag1;
    bool m_btnFlag2;
    std::string m_territoryId1;
    std::string m_territoryId2;
    WorldResourceType m_territoryResourceType1;
    WorldResourceType m_territoryResourceType2;
    std::string m_territoryName1;
    std::string m_territoryName2;
    int m_territoryCount1;
    int m_territoryCount2;
    WorldCityType m_type;
};

#endif /* defined(__IF__AlliianceTerritoryView__) */
