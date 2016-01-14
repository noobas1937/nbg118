//
//  AllianceWarDetailView.h
//  IF
//
//  Created by ganxiaohua on 14-8-7.
//
//

#ifndef __IF__AllianceWarDetailView__
#define __IF__AllianceWarDetailView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "AllianceTeamInfo.h"
#include "YuanJunInfo.h"
#include "CCClipNode.h"
#include "AllianceWarView.h"
#include "HFHeadImgNode.h"
#include "CCTableViewTouchIFCell.h"
#define  MSG_ALLIANCE_QIANFAN "MSG_ALLIANCE_QIANFAN"

class AllianceWarDetailView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static AllianceWarDetailView *create(AllianceTeamInfo* info);
    AllianceWarDetailView(AllianceTeamInfo* info);
    ~AllianceWarDetailView();
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
    
    void jieSanAlliacne(CCObject *pSender, CCControlEvent event);
    void okJieSan();
    void onAttackClick(CCObject *pSender, CCControlEvent event);
    void onSupportClick(CCObject *pSender, CCControlEvent event);
    void onGoHomeClick(CCObject *pSender, CCControlEvent event);
    void onCellClick(CCObject* data);
    void qianFanCallBack(CCObject* data);
    void updateArriveTime(CCObject* data);
    void updateTime(float _time);
    void sendBackArmy(CCObject* obj);
    void goToWorld();
    
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_viewBg;
    CCSafeObject<CCLabelIF> m_statusTxt;
    CCSafeObject<CCNode> m_bottomNode;
//    CCSafeObject<CCNode> m_fireNode1;
//    CCSafeObject<CCNode> m_fireNode2;
    CCSafeObject<CCControlButton> m_jieSanBtn;
    CCSafeObject<CCControlButton> m_attackBtn;
    CCSafeObject<CCControlButton> m_supportBtn;
    CCSafeObject<CCControlButton> m_goHomeBtn;
    
    CCSafeObject<CCNode> m_clipperNode;
    CCSafeObject<CCClipNode> m_clipper;
    CCSafeObject<CCLabelIF> m_txtNum1;
    CCSafeObject<CCLabelIF> m_armyNum;
    
    CCSafeObject<CCNode> m_posNode1;
    CCSafeObject<CCLabelIF> m_battleTxt;
    CCSafeObject<CCLabelIF> m_targetTitle;
    CCSafeObject<CCLabelIF> m_targetPosTxt;
    CCSafeObject<CCLabelIF> m_teamTargetTxt;
    CCSafeObject<CCLabelIF> m_teamTitle;
    CCSafeObject<CCLabelIFTTF> m_leaderTxt;
    CCSafeObject<CCNode> m_cityIconNode;
    CCSafeObject<CCNode> m_defNode;
    CCSafeObject<CCNode> m_attNode;
    CCSafeObject<CCSprite> m_renSpr;
    
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<AllianceTeamInfo> m_info;
    CCSafeObject<CCArray> m_teamArmys;
    bool m_refreshMarch;
    bool m_isLeaderUser;
    CC_SYNTHESIZE_READONLY(bool, m_isAttackPer, IsAttackPer);
};

class AllianceTeamDetailCell : public CCTableViewTouchIFCell
,public CCBSelectorResolver
 
,public CCBMemberVariableAssigner

{
public:
    static AllianceTeamDetailCell *create(YuanJunInfo* info,CCNode* clickNode,bool isLeaderUser,int battleType,string teamId);
    void setData(YuanJunInfo* info,bool isLeaderUser,int battleType,string teamId);
    CCSafeObject<CCControlButton> m_btnReward;
private:
    AllianceTeamDetailCell(YuanJunInfo* info,CCNode* clickNode,bool isLeaderUser,int battleType,string teamId):m_info(info),m_clickNode(clickNode),m_isLeaderUser(isLeaderUser),m_battleType(battleType),m_teamId(teamId){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    void onSpeedClick(CCObject *pSender, CCControlEvent event);
    void onGoHomeClick(CCObject *pSender, CCControlEvent event);
    void onQianfanClick(CCObject *pSender, CCControlEvent event);
    
    void updateTime(float _time);
    void attendRally();
    
    CCSafeObject<CCLabelIFTTF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_armyNum;
    CCSafeObject<CCLabelIF> m_statusTxt;
    CCSafeObject<CCScale9Sprite> m_renderBg;
    CCSafeObject<CCNode> m_icon;
    CCSafeObject<CCControlButton> m_speedBtn;
    CCSafeObject<CCLabelIF> m_tipTxt;
    CCSafeObject<CCNode> m_unUseNode;
    CCSafeObject<CCNode> m_moveNode;
    CCSafeObject<CCNode> m_arrow;
    CCSafeObject<CCNode> m_clickArea;
    CCSafeObject<CCNode> m_funNode;
    CCSafeObject<CCControlButton> m_goHomeBtn;
    CCSafeObject<CCControlButton> m_daJieBtn;
    CCSafeObject<CCScale9Sprite> m_progress;
    CCSafeObject<CCLabelIF> m_proTxt;
    CCSafeObject<CCNode> m_soldierNode;
    CCSafeObject<CCNode> m_buttonNode;
    CCSafeObject<CCScale9Sprite> m_lineBg;
    CCSafeObject<CCNode> m_scienceNode;
    CCSafeObject<CCLabelIF> m_scienceTxt;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    
    CCNode* m_clickNode;
    CCPoint  m_touchPos;
    YuanJunInfo* m_info;
    bool m_clickFlag;
    double m_haveTime;
    double m_totalTime;
    bool m_isLeaderUser;
    int m_battleType;
    string m_teamId;
};
#endif /* defined(__IF__AllianceWarDetailView__) */
