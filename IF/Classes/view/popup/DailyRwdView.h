//
//  DailyRwdView.h
//  IF
//
//  Created by xxrdsg on 14-12-31.
//
//

#ifndef __IF__DailyRwdView__
#define __IF__DailyRwdView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCMultiColTableView.h"
#include "FBFriendInfo.h"
#include "CCTableViewTouchIFCell.h"
#define MSG_DAILY_CELL_CLICK "daily.cell.click"

class DailyRwdView: public Layer
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public cocos2d::extension::CCMultiColTableViewDataSource
, public cocos2d::extension::CCMultiColTableViewDelegate
{
public:
    DailyRwdView();
    ~DailyRwdView();
    static DailyRwdView* create(int type = 0);
    void resetTabViewPos();
    void setListNodeVisible(bool);
protected:
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(int type);
    
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
    virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual unsigned int numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    void onClickRwdBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickVipRwdBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickTipBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onSelectClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onFriendClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onCloseBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onRmvWaitInter1(CCObject* params);
    void onRmvWaitInter2(CCObject* params);
    void refreshRDData();
    void refreshBtnState();
    void refreshTitle();
    void generateData(CCObject* p);
    void initFriend();
    
    void afterGetFriendsInfo(CCObject* p);
    void loginSuccess(CCObject* p);
    void bindSuccess(CCObject* p);
    void getInviteFriends(CCObject* p);
    void getRequestFriends(CCObject* p);
    void yesFuns();
    void getDailyReward();
    void checkFriend(float t_time);
    void addFriends();
    
    int m_itemId;
    WaitInterface* m_waitInterface1;
    WaitInterface* m_waitInterface2;
//    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCMultiColTableView> m_tabView;
    
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_shipPicNode;
    CCSafeObject<CCNode> m_topNode;
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCScale9Sprite> m_listBg;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCNode> m_dayNode;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCLabelIF> m_vipLabel;
    CCSafeObject<CCControlButton> m_rwdBtn;
    CCSafeObject<CCControlButton> m_tipBtn;
    CCSafeObject<CCControlButton> m_friendBtn;
    CCSafeObject<CCControlButton> m_selectBtn;
    CCSafeObject<CCLabelIF> m_friendTxt;
    CCSafeObject<CCNode> m_friendNode;
    CCSafeObject<CCNode> m_rwdList;
    CCSafeObject<CCSprite> m_leftSpr;
    CCSafeObject<CCSprite> m_rightSpr;
    CCSafeObject<CCSprite> m_selectSpr;
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCNode> m_scNode;
    CCSafeObject<CCNode> m_moveNode;
    CCSafeObject<CCLabelIF> m_tipTxt1;
    CCSafeObject<CCLabelIF> m_tipTxt2;
    CCSafeObject<CCLabelIF> m_tipTxt3;
    CCSafeObject<CCNode> m_infoNode;
    CCSafeObject<CCNode> m_touchNode;
    
    CCSafeObject<CCArray> m_friendDatas;
    CCSafeObject<CCArray> m_inviteDatas;
    CCSafeObject<CCArray> m_showDatas;
    string m_requestId;
    string m_sendId;
    int m_count;
    int m_inviteNum;
    int m_notInstallNum;
    int m_installNum;
    
    int m_viewType; //0---码头  1---首登弹出框
};

class DailyCell:
public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
, public CCTableViewTouchIFCell
{
public:
    static DailyCell* create(int itemId,CCNode* clickArea);
    DailyCell(CCNode* clickArea): m_itemId(0),m_clickArea(clickArea){};
    void cellTouchEnded(CCTouch* pTouch);
    int m_itemId;
    void setData(int itemId);
private:
    bool init(int itemId);
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    void refreshRd(CCObject* params);
    
    void onRefreshBaoXiang(bool st);
    
    CCSafeObject<CCSprite> m_bgSpr;
    CCSafeObject<CCSprite> m_picSpr;
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<CCLabelIF> m_dayLabel;
    CCSafeObject<CCSprite> m_flashSpr;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCNode> m_particleNode;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCSprite> m_itemBG;
    CCSafeObject<CCSprite> m_numBG;
    CCNode* m_clickArea;
    CCPoint m_startPos;
    
    CCSafeObject<CCNode> m_rdNode;
    CCSafeObject<CCSprite> m_spr1;
    CCSafeObject<CCSprite> m_spr2;
    CCSafeObject<CCSprite> m_spr3;
    CCSafeObject<CCSprite> m_wenhao;
};

class DailyRwdPop: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
  
{
public:
    static DailyRwdPop* create(int itemId);
    DailyRwdPop(): m_itemId(0){};
    ~DailyRwdPop(){};
private:
    bool init(int itemId);
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    int m_itemId;
    
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_desLabel;
};

class DailyFriendCell:
public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCNode
{
public:
    static DailyFriendCell* create(FBFriendInfo* info,CCNode* clickNode);
    DailyFriendCell(FBFriendInfo* info,CCNode* clickNode): m_info(info),m_clickNode(clickNode){};
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onSelectClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCNode* m_clickNode;
    CCSafeObject<CCSprite> m_selectSpr;
    CCSafeObject<CCLabelIFTTF> m_nameTxt;
    CCSafeObject<CCControlButton> m_selectBtn;
    FBFriendInfo* m_info;
};


#endif /* defined(__IF__DailyRwdView__) */
