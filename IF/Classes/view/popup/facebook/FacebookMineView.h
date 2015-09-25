//
//  FacebookMineView.h
//  IF
//
//  Created by ganxiaohua on 14-10-16.
//
//

#ifndef __IF__FacebookMineView__
#define __IF__FacebookMineView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "FBFriendInfo.h"
#include "FBFriendCell.h"

class FacebookMineView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
 
, public CCTableViewDataSource
, public DefaultTableViewDelegate
{
public:
    CREATE_FUNC(FacebookMineView);
    
    FacebookMineView();
    ~FacebookMineView();
    
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell){};
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell){};
private:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    void onBtnSendClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onfriendInfoClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onExpHelpClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onLVInfoBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onFBInfoBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onCallHelpClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onRecordClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void afterGetFriendsInfo(CCObject* param);
    void afterGetRequestResult(CCObject* param);
    void updateWorkers(CCObject* param);
    void getWorkersFromServer(CCObject* param);
    void parseWorker(string workers);
    void checkCall(float _time);
    void showTip();
    void updateLine();
    void freshWorker();
    void digMineSuccess(CCObject* param);
    void checkFriendsData(float _time);
    
    CCSafeObject<CCScale9Sprite> m_viewBg;
    CCSafeObject<CCLabelIF> m_lvTxt;
    CCSafeObject<CCLabelIF> m_workerNum;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCLabelIF> m_btnSendTxt;
    CCSafeObject<CCLabelIF> m_btnTime;
    
    CCSafeObject<CCControlButton> m_requestBtn;
    CCSafeObject<CCControlButton> m_btnSend;
    CCSafeObject<CCControlButton> m_LVInfoBtn;
    CCSafeObject<CCControlButton> m_FBInfoBtn;
    CCSafeObject<CCControlButton> m_btnCallHelp;
    CCSafeObject<CCControlButton> m_btnRecord;
    
    CCSafeObject<CCNode> m_friendNode;
    CCSafeObject<CCTableView> m_tableView;
    CCSafeObject<CCSprite> body;
    CCSafeObject<CCNode> m_workerNode;
    CCSafeObject<CCNode> m_moveNode;
    CCSafeObject<CCNode> m_buttomNode;
    CCSafeObject<CCScale9Sprite> m_progress;
    CCSafeObject<CCScrollView> m_scrollView;
    
    CCSafeObject<CCLabelIF> m_dialogTxt;
    CCSafeObject<CCSprite> m_gouGlod;
    CCSafeObject<CCScale9Sprite> m_goldGouLine;
    CCSafeObject<CCNode> m_mineNode;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCLabelIF> m_goldTxt;
    
    CCSafeObject<CCNode> m_btnNode;
    CCSafeObject<CCNode> m_dialogNode;
    CCSafeObject<CCNode> m_dialogArea;
    CCSafeObject<CCNode> m_frieNode;
    CCSafeObject<CCNode> m_par1;
    CCSafeObject<CCNode> m_par2;
    CCSafeObject<CCNode> m_smokingNode;
    
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCArray> m_dataWorkers;
    CCSafeObject<CCArray> m_friendDatas;
    double m_leftTime;
    string m_currentAniName;
    bool m_isSend;
    int m_addH;
    int m_timeCount;
    int m_parCount;
    int m_parSmokeCount;
};

class FBWorkderCell :
public Layer
 
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    CREATE_FUNC(FBWorkderCell);
    
    FBWorkderCell(){_ignoreAnchorPointForPosition = false;};
    ~FBWorkderCell(){}
    void setData(FBFriendInfo* info,CCNode* clickArea);
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();

    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    CCSafeObject<CCNode> m_head;
    CCSafeObject<CCNode> m_clickNode;
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<FBFriendInfo> m_info;
    CCNode* m_clickArea;
};

#endif /* defined(__IF__FacebookMineView__) */
