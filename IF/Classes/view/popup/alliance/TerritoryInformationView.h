//
//  TerritoryInformationView.h
//  IF
//
//  Created by 〜TIAN~ on 15/4/14.
//
//

#ifndef __IF__TerritoryInformationView__
#define __IF__TerritoryInformationView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "AllianceTeamInfo.h"
#include "YuanJunInfo.h"
#include "CCClipNode.h"
#include "WorldController.h"
#include "CCTableViewTouchIFCell.h"
#include "HFHeadImgNode.h"

class TerritoryInformationView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static TerritoryInformationView *create(WorldCityInfo& info);
    TerritoryInformationView(WorldCityInfo& info);
    ~TerritoryInformationView();
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
    void onWithdrawClick(CCObject *pSender, CCControlEvent event);
    void onTipBtnClick(CCObject *pSender, CCControlEvent event);
    void onCellClick(CCObject* data);
    void withdraw();
    void secondConfirm();
    void refreshView();
    string getTuid();
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCArray> m_arr;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCNode> m_fireNode1;
    CCSafeObject<CCNode> m_fireNode2;
    CCSafeObject<CCNode> m_bgNode;
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
    CCSafeObject<CCLabelIF> m_noDeadLabel;
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
    int m_totalKill;
    bool m_noDeadFlag;
};

class TerritoryInformationCell : public CCTableViewTouchIFCell
,public CCBSelectorResolver
 
,public CCBMemberVariableAssigner

{
public:
    static TerritoryInformationCell *create(YuanJunInfo* info,CCNode* clickNode,int stat,int targetIndex,WorldCityType type);
    void setData(YuanJunInfo* info,int stat);
    CCSafeObject<CCControlButton> m_btnReward;
private:
    TerritoryInformationCell(YuanJunInfo* info,CCNode* clickNode,int stat,int targetIndex,WorldCityType type):m_info(info),m_clickNode(clickNode),m_stat(stat),m_targetIndex(targetIndex),m_cityType(type){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_armyNum;
    CCSafeObject<CCLabelIF> m_statusTxt;
    CCSafeObject<CCLabelIF> m_woodTxt;
    CCSafeObject<CCLabelIF> m_foodTxt;
    CCSafeObject<CCLabelIF> m_ironTxt;
    CCSafeObject<CCLabelIF> m_stoneTxt;
    CCSafeObject<CCScale9Sprite> m_renderBg;
    CCSafeObject<CCScale9Sprite> m_jiaruBg;
    CCSafeObject<CCNode> m_icon;
    CCSafeObject<CCNode> m_resNode;
    CCSafeObject<CCLabelIF> m_tipTxt;
    CCSafeObject<CCNode> m_unUseNode;
    CCSafeObject<CCNode> m_moveNode;
    CCSafeObject<CCNode> m_arrow;
    CCSafeObject<CCNode> m_clickArea;
    CCSafeObject<CCNode> m_soldierNode;
    CCSafeObject<CCScale9Sprite> m_lineBg;
    CCSafeObject<CCNode> m_scienceNode;
    CCSafeObject<CCLabelIF> m_scienceTxt;
    CCSafeObject<CCSprite> m_joinNode;
    CCSafeObject<CCSprite> m_bgNodee;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    CCNode* m_clickNode;
    CCPoint  m_touchPos;
    YuanJunInfo* m_info;
    bool m_clickFlag;
    int m_stat;
    int m_targetIndex;
    WorldCityType m_cityType;
};

#endif /* defined(__IF__TerritoryInformationView__) */
