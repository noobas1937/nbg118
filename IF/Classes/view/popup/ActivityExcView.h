//
//  ActivityExcView.h
//  IF
//
//  Created by xxrdsg on 15-6-3.
//
//

#ifndef __IF__ActivityExcView__
#define __IF__ActivityExcView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "ActivityEventObj.h"
#include "ActivityController.h"
#include "DefaultTableViewDelegate.h"
#include "CCClipNode.h"
#include "CCTableViewTouchIFCell.h"
#define MSG_TOUCH_CELL_HEAD "msg.touch.cell.head"
#define MSG_TOUCH_CELL_BODY "msg.touch.cell.body"
#define MSG_START_EXC_PROCESS "msg.start.exc.process"
#define MSG_END_EXC_PROCESS "msg.end.exc.process"
#define MSG_START_REFRESH_PROCESS "msg.start.refresh.process"
#define MSG_END_REFRESH_PROCESS "msg.end.refresh.process"

class ActivityExcNewView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
, public CCTableViewDataSource
, public DefaultTableViewDelegate
{
public:
    ActivityExcNewView(ActivityEventObj* obj): m_obj(obj), m_openIdx(0), m_isCellRefreshing(false), m_isRefreshing(false), m_tableView(nullptr), m_waitInterface1(nullptr), m_waitInterface2(nullptr){};
    static ActivityExcNewView* create(ActivityEventObj* obj);
    void onEnter();
    void onExit();
    bool init();
    
    void onEnterFrame(float dt);
    
    // scrollview delegate
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    virtual void scrollViewDidScroll(CCScrollView* view);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onClickCloseBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onClickBuyBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onClickRefreshBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onClickTipBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    void confirmFun();
    
    void onGetActDataSuccessMsg(CCObject*);
    void onGetActDataFailMsg(CCObject*);
    void onGetStartExcProcMsg(CCObject*);
    void onGetEndExcProcMsg(CCObject*);
    void onGetExcSuccessMsg(CCObject*);
    void onGetExcFailMsg(CCObject*);
    void onGetRefreshFailMsg(CCObject*);
    void onGetEndRefreshMsg(CCObject*);
    
    void onGetTouchHeadMsg(CCObject*);
    void onGetTouchBodyMsg(CCObject*);
    
    void refreshTopIcons();
    void refreshTabView();
    void refreshRefBtn();
    
    void addWaitInter();
    void removeWaitInter();
private:
    CCSafeObject<CCNode> m_iconMainNode;
    CCSafeObject<CCNode> m_labelMainNode;
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelIF> m_rewardLabel;
    CCSafeObject<CCLabelIF> m_collectLabel;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCControlButton> m_buyBtn;
    CCSafeObject<CCControlButton> m_refreshBtn;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCLabelIF> m_refreshLabel;
    CCSafeObject<CCLabelIF> m_goldLabel;
    
    CCSafeObject<CCNode> m_buyBtnNode;
    CCSafeObject<CCNode> m_refreshBtnNode;
    
    WaitInterface* m_waitInterface1;
    WaitInterface* m_waitInterface2;
    
    CCSafeObject<CCTableView> m_tableView;
    ActivityEventObj* m_obj;
    string m_itemId;
    int m_openIdx;
    bool m_isRefreshing;
    bool m_isCellRefreshing;
};

class ActivityExcNewCell: public CCTableViewTouchIFCell
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    ActivityExcNewCell(ActivityEventObj* obj, int idx, bool open, CCNode* clickArea): m_obj(obj), m_idx(idx), m_isOpen(open), m_clickArea(clickArea){};
    static ActivityExcNewCell* create(ActivityEventObj* obj, int idx, bool open, CCNode* clickArea);
    void setData(int idx, bool open);
    bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void refreshRewardInfo(CCObject*);
    void refreshRewardInfoWithoutMulIcon(CCObject*);
    void refreshNumLabel(float t);
    void showRandomIcons(CCObject*);
    void changePic(CCObject*);
    
    void onGetExcSucMsg(CCObject*);
    void onGetExcFailMsg(CCObject*);
    
    void onGetStartExcProc(CCObject*);
    void onGetEndExcProc(CCObject*);
    
    void onGetStartRefreshPro(CCObject*);
    void onGetEndRefreshPro(CCObject*);
    
    void onGetRefreshSucMsg(CCObject*);
    void onGetRefreshFailMsg(CCObject*);
    
    void onClickExchangeBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    void refreshRuneIcons();
    void refreshGlowNode();
private:
    CCSafeObject<CCNode> m_mainNode;
    
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<CCScale9Sprite> m_middleBg;
    CCSafeObject<CCSprite> m_rightBg;
    CCSafeObject<CCNode> m_itemNode;
    CCSafeObject<CCSprite> m_iconBg;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_noIconBgNode;
    
    CCSafeObject<CCLayer> m_desNode;
    CCSafeObject<CCScale9Sprite> m_desBg;
    CCSafeObject<CCLabelIF> m_desNameLabel;
    CCSafeObject<CCLabelIF> m_desInfoLabel;
    CCSafeObject<CCControlButton> m_exchangeBtn;
    
    CCSafeObject<CCNode> m_mulNode;
    CCSafeObject<CCLabelIFBMFont> m_mulLabel;
    
    CCSafeObject<CCAniNode> m_glowNode;
    
    ActivityEventObj* m_obj;
    int m_idx;
    bool m_isOpen;
    
    CCPoint p1;
    CCPoint p2;
//    WaitInterface* m_waitInterface;
    CCNode* m_clickArea;
};

class ActivityExcNewCellCell: public CCNode
,public CCBMemberVariableAssigner
{
public:
    ActivityExcNewCellCell(string actId, int idx, string itemId, int pos): m_actId(actId), m_idx(idx), m_itemId(itemId), m_pos(pos), m_startPos(0){};
    static ActivityExcNewCellCell* create(string actId, int idx, string itemId, int pos);
    bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual void onEnter();
    virtual void onExit();
    
    void onGetExcSucMsg(CCObject*);
    void onGetRefreshSucMsg(CCObject*);
    
    void playEffectsByExc(float t);
    void playEffectsByRefresh(float t);
    void showRandomIconByExc(CCObject*);
    void showRandomIconByRefresh(CCObject*);
    void changePic(CCObject*);
    void refreshSprColor();
    
    CCSafeObject<CCNode> m_iconNode;
    string m_actId;
    int m_idx;
    string m_itemId;
    int m_pos;
    
    int m_startPos;
};

class ActExcConfirmView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    ActExcConfirmView(string actId, int idx): m_actId(actId), m_idx(idx){};
    static ActExcConfirmView* create(string actId, int idx);
    bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onClickConfBtn(CCObject * pSender, CCControlEvent pCCControlEvent);

private:
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCLabelIF> m_confLabel;
    CCSafeObject<CCControlButton> m_confBtn;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCLabelIF> m_numLabel;
    
    string m_actId;
    int m_idx;
    
};

#endif /* defined(__IF__ActivityExcView__) */
