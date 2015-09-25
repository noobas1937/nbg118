//
//  CrossServiceTargetView.h
//  IF
//
//  Created by ganxiaohua on 15/6/8.
//
//

#ifndef __IF__CrossServiceTargetView__
#define __IF__CrossServiceTargetView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCBExtension.h"
#include "ActivityEventObj.h"
#include "DefaultTableViewDelegate.h"
#include "CCAniNode.h"
#include "QuestInfo.h"
#include "HFHeadImgNode.h"

#include "CCTableViewTouchIFCell.h"
class CrossServiceTargetView: public PopupBaseView
,public CCBSelectorResolver
 
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static CrossServiceTargetView *create();
    CrossServiceTargetView(){};
    ~CrossServiceTargetView(){};
    
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
    
    void onCloseClick(CCObject *pSender, CCControlEvent event);
    void onClickTab1(CCObject *pSender, CCControlEvent event);
    void onClickTab2(CCObject *pSender, CCControlEvent event);
    void onClickTab3(CCObject *pSender, CCControlEvent event);
    void onClickTab4(CCObject *pSender, CCControlEvent event);
    void onClickHelpBtn(CCObject *pSender, CCControlEvent event);
    void onInfoClick(CCObject *pSender, CCControlEvent event);
    void onClickRwdBtn(CCObject *pSender, CCControlEvent event);
    void onLianShengClick(CCObject *pSender, CCControlEvent event);
    
    void getData(CCObject *pSender);
    void getData2(CCObject *param);
    void updateTime(float _time);
    void freshScrollView();
    void refreshTarget(CCObject *pSender);
    void updateScore(float _time);
    bool checkHaveCompleteTask();
    void delayFresh(float _time);
    
    CCSafeObject<CCControlButton> m_btnClose;
    CCSafeObject<CCControlButton> m_tab1;
    CCSafeObject<CCControlButton> m_tab2;
    CCSafeObject<CCControlButton> m_tab3;
    CCSafeObject<CCControlButton> m_tab4;
    CCSafeObject<CCLabelIF> m_tabTxt1;
    CCSafeObject<CCLabelIF> m_tabTxt2;
    CCSafeObject<CCLabelIF> m_tabTxt3;
    CCSafeObject<CCLabelIF> m_tabTxt4;
    CCSafeObject<CCLabelIF> m_titleTxt;
    
    CCSafeObject<CCNode> m_tableNode;
    CCSafeObject<CCNode> m_clickArea;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCLabelIF> m_txt1;
    CCSafeObject<CCLabelIF> m_txt2;
    CCSafeObject<CCLabelIF> m_txt3;
    CCSafeObject<CCLabelIF> m_txt1Value;
    CCSafeObject<CCLabelIF> m_txt2Value;
    CCSafeObject<CCLabelIF> m_txt3Value;
    
    CCSafeObject<CCNode> m_scNode;
    CCSafeObject<CCNode> m_tabRewardNode1;
    CCSafeObject<CCNode> m_tabNode1;
    CCSafeObject<CCNode> m_rankNode;
    CCSafeObject<CCNode> m_viewBg;
    
    CCSafeObject<CCNode> m_introNode;
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_descTxt;
    
    CCSafeObject<CCControlButton> m_helpBtn;
    CCSafeObject<CCNode> m_parNode;
    CCSafeObject<CCNode> m_scoreNode;
    CCSafeObject<CCLabelIF> m_targetTip;
    
    CCSafeObject<CCNode> m_targetNode;
    CCSafeObject<CCLabelIF> m_perTxt;
    CCSafeObject<CCControlButton> m_rwdBtn;
    CCSafeObject<CCLabelIF> m_taskNameTxt;
    CCSafeObject<CCLabelIF> m_rewardTxt;
    CCSafeObject<CCLabelIF> m_helpTip;
    CCSafeObject<CCLabelIF> m_tastTxt;
    CCSafeObject<CCLabelIF> m_helpTxt;
    CCSafeObject<CCNode> m_rewardNode;
    CCSafeObject<CCScale9Sprite> m_perSpr;
    CCSafeObject<CCNode> m_taskIcon;
    CCSafeObject<CCNode> m_fireNode1;
    CCSafeObject<CCNode> m_fireNode2;
    CCSafeObject<CCNode> m_bottomNode;
    
    CCSafeObject<WaitInterface> m_waitInterface;
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCScrollView> m_scrollView2;
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCArray> m_taskData;
    
    CCSafeObject<CCDictionary> m_scoreInfo;
    
    ActivityEventObj* m_actObj;
    int m_tabIndex;
    double m_perScore;
    double m_kingdomScore;
    double m_userScore;
    double m_score;
    string itemId;
    string m_desc;
};

class ActivityTargetCell :  public Layer
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static ActivityTargetCell *create(QuestInfo* info,CCNode* clickArea);
    void setData();
private:
    ActivityTargetCell(QuestInfo* info,CCNode* clickArea):m_info(info),m_clickArea(clickArea){ignoreAnchorPointForPosition(false);setAnchorPoint(Vec2(0,0));};
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onClickRwdBtn(CCObject *pSender, CCControlEvent event);
    void setSpriteGray(CCNode* node);
    
    CCSafeObject<CCNode> m_gray;
    CCSafeObject<CCNode> m_rewardNode;
    CCSafeObject<CCScale9Sprite> m_perSpr;
    CCSafeObject<CCControlButton> m_rwdBtn;
    CCSafeObject<CCLabelIF> m_perTxt;
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCNode> m_okSpr;
    
    CCNode* m_clickArea;
    QuestInfo* m_info;
};

class TargetCellAni:public CCAniNode
{
public:
    TargetCellAni(){};
    ~TargetCellAni(){};
    bool init();
    static TargetCellAni* create();
    
    virtual void onEnter();
    virtual void onExit();
    
private:
    
};

class ActivityRankCell : public CCTableViewTouchIFCell
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner

{
public:
    static ActivityRankCell *create(CCDictionary* info,CCNode* clickArea,int type);
    void setData(CCDictionary* info,int type);
private:
    ActivityRankCell(CCDictionary* info,CCNode* clickArea,int type):m_info(info),m_clickArea(clickArea),m_type(type){};
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    CCSafeObject<CCNode> m_top3Node;
    CCSafeObject<CCSprite> m_numspr;
    CCSafeObject<CCNode> m_icon;
    CCSafeObject<CCLabelIF> m_nameTxt2;
    CCSafeObject<CCLabelIF> m_sTxt;
    CCSafeObject<CCLabelIF> m_scoreTxt2;
    CCSafeObject<CCNode> m_normalNode;
    CCSafeObject<CCLabelIF> m_numTxt;
    CCSafeObject<CCLabelIF> m_scoreTxt1;
    CCSafeObject<CCLabelIF> m_nameTxt1;
    CCSafeObject<CCControlButton> m_btn1;
    CCSafeObject<CCControlButton> m_btn2;
    CCSafeObject<CCNode> m_btnNode;
    CCSafeObject<CCNode> m_aniNode;
    CCSafeObject<CCNode> m_nodeBg1;
    CCSafeObject<CCNode> m_nodeBg2;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    CCSafeObject<CCNode> m_clickNode;
    
    CCNode* m_clickArea;
    CCDictionary* m_info;
    int m_type;
    
};

class RankCellAni:public CCAniNode
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    RankCellAni(int rank):m_rank(rank){};
    ~RankCellAni(){};
    bool init();
    static RankCellAni* create(int rank);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
private:
    CCSafeObject<CCNode> m_ani1;
    CCSafeObject<CCNode> m_ani2;
    CCSafeObject<CCNode> m_ani3;
    int m_rank;
};

class TargetRewardCell :  public CCLayer
{
public:
    static TargetRewardCell *create(CCDictionary* info,CCNode* clickArea);
private:
    TargetRewardCell(CCDictionary* info,CCNode* clickArea):m_info(info),m_clickArea(clickArea),m_itemId(0){ignoreAnchorPointForPosition(false);setAnchorPoint(Vec2(0,0));};
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void setLeft(float _time);
    void setRight(float _time);
    
    CCSafeObject<CCSprite> m_bg;
    CCSafeObject<CCLabelIF> m_numTxt;
    CCSafeObject<CCLabelIF> m_nameTxt;
    
    CCNode* m_clickArea;
    CCDictionary* m_info;
    CCPoint m_startPoint;
    int m_itemId;
};
#endif /* defined(__IF__CrossServiceTargetView__) */
