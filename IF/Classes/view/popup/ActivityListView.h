//
//  ActivityListView.h
//  IF
//
//  Created by chenfubi on 15/1/6.
//
//

#ifndef IF_ActivityListView_h
#define IF_ActivityListView_h

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "ActivityEventObj.h"
#include "CCTableViewTouchIFCell.h"
class ActivityListView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
////, public CCTouchDelegate
, public CCTableViewDataSource
, public DefaultTableViewDelegate
{
public:
    static ActivityListView* create();
	virtual bool init();
    ActivityListView(){};
    virtual ~ActivityListView(){};
    
    virtual void onEnter();
    virtual void onExit();
    
protected:
    // scrollview delegate
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    
private:
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    void getServerData(CCObject* obj);
    
private:
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_adNode;
    CCSafeObject<CCNode> m_viewBg;
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCArray> m_data;
    
};

class ActivityListNewCell:
public CCTableViewCell
{
public:
    static ActivityListNewCell* create(ActivityEventObj* actObj, CCNode* clickArea);
    void setData(ActivityEventObj* actObj, CCNode* clickArea);
    bool init(ActivityEventObj* actObj, CCNode* clickArea);
};

class ActivityListLuaCell:
public Layer
  
, public CCBMemberVariableAssigner
{
public:
    ActivityListLuaCell(ActivityEventObj* actObj, CCNode* clickArea): m_obj(actObj), m_clickArea(clickArea){_ignoreAnchorPointForPosition = false;setAnchorPoint(Vec2(0,0));};
    static ActivityListLuaCell* create(ActivityEventObj* actObj, CCNode* clickArea);
    virtual bool init();
    ActivityEventObj* m_obj;
    CCNode* m_clickArea;
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    void onGameTick(float time);
    
    CCSafeObject<CCNode> m_cardNode;
    CCSafeObject<CCNode> m_cellNode;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_infoTTF;
    CCSafeObject<CCLabelIF> m_timeLabel1;
    CCSafeObject<CCNode> m_aIcon;
};

//ActivityListCell class
class ActivityListCell :
public CCBSelectorResolver
 
,public CCBMemberVariableAssigner
,public CCTableViewTouchIFCell
{
public:
    static ActivityListCell* create(ActivityEventObj* actObj,CCNode* clickArea);
    ActivityListCell():m_isGetData(false),m_infoStr(""),m_sprItemBG(NULL),m_btnReward(NULL),m_waitInterface(NULL),m_clickArea(NULL){};
    virtual ~ActivityListCell(){};
    void setData(ActivityEventObj* actObj);
    void rewardSpe(CCObject *ccObj);
private:
    bool init(ActivityEventObj* actObj,CCNode* clickArea);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onTipClick(CCObject *pSender, CCControlEvent event);
    void onRewardClick(CCObject *pSender, CCControlEvent event);
    void onClickQuickBtn(CCObject *pSender, CCControlEvent event);
    void buySuccess(CCObject *ccObj);
    void errorPayment(cocos2d::CCObject *obj);
    void onShowError(float dt);
    
    void onGameTick(float time);
    void initGeneralAct();
    void onGeneralClick();
    void initSpeAct();
    void onSpeClick();
    void refreshData(CCObject* obj);
    
private:
    CCSafeObject<CCNode> m_infoNode;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_infoTTF;
    CCSafeObject<CCLabelIF> m_timeLabel1;
    CCSafeObject<ActivityEventObj> m_actObj;
    CCSafeObject<CCNode> m_aIcon;
    
    //monthcard reward
    CCSafeObject<CCScale9Sprite> m_sprItemBG;
    CCSafeObject<CCControlButton> m_btnReward;
    CCSafeObject<CCControlButton> m_btnInfo;
    
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCLabelIF> m_desText1;
    CCSafeObject<CCLabelIF> m_lblItem1;
    CCSafeObject<CCLabelIF> m_lblItem2;
    CCSafeObject<CCLabelIF> m_moreLabel;
    CCSafeObject<CCNode> m_nodeItem1;
    CCSafeObject<CCNode> m_nodeItem2;
    
    CCSafeObject<CCNode> m_cardNode;
    CCSafeObject<CCNode> m_cellNode;
    CCSafeObject<CCNode> cardRewardNode;
    CCSafeObject<CCNode> m_rewardItemNode;
    CCSafeObject<CCLabelIF> m_rewardName;
    CCSafeObject<CCLabelIF> m_timeLabel2;
    
    WaitInterface* m_waitInterface;
    CCNode* m_clickArea;
    void clearSpeWait();
    bool m_bTick;
    string m_infoStr;
    bool m_isGetData;
    
};


#endif
