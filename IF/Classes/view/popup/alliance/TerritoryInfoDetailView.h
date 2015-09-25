//
//  TerritoryInfoDetailView.h
//  IF
//
//  Created by 〜TIAN~ on 15/7/17.
//
//

#ifndef __IF__TerritoryInfoDetailView__
#define __IF__TerritoryInfoDetailView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "AllianceTeamInfo.h"
#include "YuanJunInfo.h"
#include "CCClipNode.h"
#include "WorldController.h"
#include "HFHeadImgNode.h"
#include "CCTableViewTouchIFCell.h"
class TerritoryInfoDetailView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static TerritoryInfoDetailView *create(WorldCityInfo& info);
    TerritoryInfoDetailView(WorldCityInfo& info);
    ~TerritoryInfoDetailView();
    void requestDetail();
    void update(float sec);
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
    void onWithdrawClick(CCObject *pSender, CCControlEvent event);
    void onTipBtnClick(CCObject *pSender, CCControlEvent event);
    void onCellClick(CCObject* data);
    void qianFanCallBack(CCObject *data);
    void withdraw();
    void secondConfirm();
    void refreshView();
    void refreshOnce();
    string getTuid();
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCNode> m_fireNode1;
    CCSafeObject<CCNode> m_fireNode2;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_bannerNode;
    CCSafeObject<CCControlButton> m_withdrawBtn;
    CCSafeObject<CCControlButton> m_tipBtn;
    
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCLabelIF> m_statLabel;
    CCSafeObject<CCLabelIF> m_defLabel1;
    CCSafeObject<CCLabelIF> m_defLabel2;
    CCSafeObject<CCLabelIF> m_troopLabel1;
    CCSafeObject<CCLabelIF> m_troopLabel2;
    CCSafeObject<CCLabelIF> m_timeLabel1;
    CCSafeObject<CCLabelIF> m_timeLabel2;
    CCSafeObject<CCLabelIF> m_tipsLabel;
    CCSafeObject<CCSprite> m_loadingIcon;
    CCSafeObject<CCTableView> m_tabView;
    bool m_owner;//true:自己false:敌人
    bool m_territoryFlag;//true:自己false:敌人
    WorldCityInfo& m_cityInfo;
    bool m_isInUpdate;
    int m_stat;
    int m_territoryStat;
    int soldierMax;
    int soldierAlready;
    bool m_ownerFlag;//true:是否为自己的建筑
    int destroyingArmy;//摧毁中的部队数
};

class TerritoryInfoDetailCell : public CCTableViewTouchIFCell
,public CCBSelectorResolver
,public CCBMemberVariableAssigner

{
public:
    static TerritoryInfoDetailCell *create(YuanJunInfo* info,CCNode* clickNode,int stat,int targetIndex,WorldCityType type,bool owner);
    void setData(YuanJunInfo* info,int stat);
    CCSafeObject<CCControlButton> m_btnReward;
private:
    TerritoryInfoDetailCell(YuanJunInfo* info,CCNode* clickNode,int stat,int targetIndex,WorldCityType type,bool owner):m_info(info),m_clickNode(clickNode),m_stat(stat),m_targetIndex(targetIndex),m_cityType(type),m_owner(owner){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void onQianfanClick(CCObject *pSender, CCControlEvent event);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    void attendRally();
    void updateTime(float _time);
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
    CCSafeObject<CCSprite> m_arrow;
    CCSafeObject<CCNode> m_clickArea;
    CCSafeObject<CCNode> m_soldierNode;
    CCSafeObject<CCScale9Sprite> m_lineBg;
    CCSafeObject<CCNode> m_scienceNode;
    CCSafeObject<CCLabelIF> m_scienceTxt;
    CCSafeObject<CCSprite> m_joinNode;
    CCSafeObject<CCNode> m_bgNodee;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    CCNode* m_clickNode;
    CCPoint  m_touchPos;
    YuanJunInfo* m_info;
    bool m_clickFlag;
    int m_stat;
    int m_targetIndex;
    WorldCityType m_cityType;
    bool m_owner;
};

#endif /* defined(__IF__TerritoryInfoDetailView__) */
