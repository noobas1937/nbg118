//
//  TerritoryWarehouseDetailView.h
//  IF
//
//  Created by 〜TIAN~ on 15/7/9.
//
//

#ifndef __IF__TerritoryWarehouseDetailView__
#define __IF__TerritoryWarehouseDetailView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"
#include "DefaultTableViewDelegate.h"
#include "WorldController.h"
#include "HFHeadImgNode.h"
#include "CCTableViewTouchIFCell.h"
class PlayerResInfo:public cocos2d::CCObject
{
public:
    std::string uid;
    std::string name;
    std::string pic;
    int picVer;
    unsigned long total;
    unsigned long wood;
    unsigned long food;
    unsigned long iron;
    unsigned long stone;
    int index;
    bool open;
};

class TerritoryWarehouseDetailCell;

class TerritoryWarehouseDetailView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static TerritoryWarehouseDetailView *create(WorldCityInfo& info);
    TerritoryWarehouseDetailView(WorldCityInfo& info);
    ~TerritoryWarehouseDetailView();
    void requestDetail();
protected:
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onDetailCallback(CCObject* obj);
    void addLoadingAni();
    void removeLoadingAni(CCObject* p = NULL);
    void onTipBtnClick(CCObject *pSender, CCControlEvent event);
    void onWithdrawClick(CCObject *pSender, CCControlEvent event);
    void onCellClick(CCObject* data);
    void withdraw();
    void secondConfirm();
    void refreshView();
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCNode> m_fireNode1;
    CCSafeObject<CCNode> m_fireNode2;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCControlButton> m_tipBtn;
    CCSafeObject<CCControlButton> m_withdrawBtn;
    CCSafeObject<CCSprite> m_loadingIcon;
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelIF> m_statLabel;
    CCSafeObject<CCLabelIF> m_defLabel1;
    CCSafeObject<CCLabelIF> m_defLabel2;
    CCSafeObject<CCLabelIF> m_troopLabel1;
    CCSafeObject<CCLabelIF> m_troopLabel2;
    CCSafeObject<CCLabelIF> m_tipsLabel;
    CCSafeObject<CCLabelIF> m_timeLabel1;
    CCSafeObject<CCLabelIF> m_timeLabel2;
    CCSafeObject<CCLabelIF> m_titleTxt;
    
    CCSafeObject<CCTableView> m_tabView;
    WorldCityInfo& m_cityInfo;
    bool m_isInUpdate;
    int m_stat;
    int m_territoryStat;
    unsigned long m_totalResource;
    unsigned long m_playerStorage;
    int m_totalNum;
};

class TerritoryWarehouseDetailCell : public CCTableViewTouchIFCell
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static TerritoryWarehouseDetailCell *create(PlayerResInfo* info,CCNode* clickNode,int stat,int index);
    void setData(PlayerResInfo* info,int stat);
    CCSafeObject<CCControlButton> m_btnReward;
private:
    TerritoryWarehouseDetailCell(PlayerResInfo* info,CCNode* clickNode,int stat,int index):m_info(info),m_clickNode(clickNode),m_stat(stat),m_targetIndex(index){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    void attendRally();
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_armyNum;
    CCSafeObject<CCLabelIF> m_statusTxt;
    CCSafeObject<CCLabelIF> m_woodTxt;
    CCSafeObject<CCLabelIF> m_foodTxt;
    CCSafeObject<CCLabelIF> m_ironTxt;
    CCSafeObject<CCLabelIF> m_stoneTxt;
    CCSafeObject<CCScale9Sprite> m_renderBg;
    CCSafeObject<CCNode> m_icon;
    CCSafeObject<CCNode> m_resNode;
    CCSafeObject<CCLabelIF> m_tipTxt;
    CCSafeObject<CCNode> m_unUseNode;
    CCSafeObject<CCNode> m_moveNode;
    CCSafeObject<CCNode> m_bgNodee;
    CCSafeObject<CCNode> m_arrow;
    CCSafeObject<CCNode> m_clickArea;
    CCSafeObject<CCNode> m_soldierNode;
    CCSafeObject<CCNode> m_scienceNode;
    CCSafeObject<CCLabelIF> m_scienceTxt;
    CCSafeObject<CCSprite> m_joinNode;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    CCNode* m_clickNode;
    CCPoint  m_touchPos;
    PlayerResInfo* m_info;
    int m_targetIndex;
    bool m_clickFlag;
    int m_stat;
};


#endif /* defined(__IF__TerritoryWarehouseDetailView__) */
