//
//  BaoZangView.h
//  IF
//
//  Created by ganxiaohua on 14-11-18.
//
//

#ifndef __IF__BaoZangView__
#define __IF__BaoZangView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "FBFriendInfo.h"

class BaoZangView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
 
, public CCTableViewDataSource
, public DefaultTableViewDelegate
{
public:
    static BaoZangView* create(int panelType = 0);
    
    BaoZangView(int panelType);
    virtual ~BaoZangView();
    
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell){};
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell){};
    
    bool static getTipFlag();
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
    
    void onExpHelpClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onLVInfoBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onFBInfoBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onCallHelpClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onRecordClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onfriendInfoClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onLoginClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onLvClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onHelpClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void afterGetFriendsInfo(CCObject* param);
    void afterGetRequestResult(CCObject* param);
    void checkBaoZang(float _time);
    void digMineSuccess(CCObject* param);
    void checkFriendsData(float _time);
    void loginSuccess(CCObject* param);
    void delayUseData(float _time);
    void bindSuccess(CCObject* param);
    void updateServerData(CCObject* param);
    void checkTime();
    void delayShow(float _time);
    void playRewardAni(float _time);
    void openRewardView(float _time);
    void playParAni();
    void showCD(float _time);
    void showFriendNode();
    void callHelpSuccess(CCObject* param);
    void faceHelpChange(CCObject* param);
    void requestShareHandle(CCObject* param);
    void showTipHandle(float _time);
    void digFail(CCObject* param);
    void addParticleToBatch(CCNode* node,cocos2d::CCParticleSystemQuad *particle);
    void addParticle(CCNode* node,CCPoint pos,CCSize size,bool addBatch=true);
    void FbRequestUserId(CCObject* param);
    void delayCheck(float t_time);
    void getInviteFriends(CCObject *data);
    void openInviteFriends(CCObject *data);
    void askForHelpHandle(CCObject *data);
    void delayParseInvite(float _time);
    void refreshHelpData();
    
    CCSafeObject<CCScale9Sprite> m_viewBg;
    CCSafeObject<CCLabelIF> m_lvTxt;
    CCSafeObject<CCControlButton> m_FBInfoBtn;
    CCSafeObject<CCControlButton> m_btnCallHelp;
    CCSafeObject<CCControlButton> m_btnRecord;
    CCSafeObject<CCControlButton> m_helpBtn;
    
    CCSafeObject<CCNode> m_friendNode;
    CCSafeObject<CCTableView> m_tableView;
    CCSafeObject<CCSprite> body;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_moveNode;
    CCSafeObject<CCNode> m_buttomNode;
    CCSafeObject<CCLabelIF> m_helpDesc;
    CCSafeObject<CCNode> m_boxOpen;
    CCSafeObject<CCNode> m_boxClose;
    CCSafeObject<CCNode> m_helpNode;
    CCSafeObject<CCNode> m_fbLoginNode;
    CCSafeObject<CCLabelIF> m_loginDesc;
    CCSafeObject<CCLabelIF> m_loginBtnTxt;
    CCSafeObject<CCNode> m_boxNode;
    CCSafeObject<CCNode> m_lockNode;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCLabelIF> m_cdTxt;
    CCSafeObject<CCScale9Sprite> m_progress;
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCNode> m_clickRewardArea;
    CCSafeObject<CCNode> m_fireNode1;
    CCSafeObject<CCNode> m_fireNode2;
    CCSafeObject<CCNode> m_boxEffectNode;
    CCSafeObject<CCNode> m_loginTipNode;
    CCSafeObject<CCNode> m_helpTipNode;
    CCSafeObject<CCControlButton> m_btnLv;
    CCSafeObject<CCNode> m_introNode;
    
    CCSafeObject<CCDictionary> m_rewardDic;
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCArray> m_friendDatas;
    CCSafeObject<CCArray> m_inviteDatas;
    CCSafeObject<CCArray> m_installDatas;
    vector<CCParticleBatchNode*> m_parVec;
    double m_leftTime;
    string m_currentAniName;
    int m_timeCount;
    int m_mutiGlod;
    bool m_haveClick;
    bool m_reLoginFb;//绑定过，但数据被清，要重新登录的
    string m_fbUid;
    bool m_requestFb;
    int m_panelType;
};
#endif /* defined(__IF__BaoZangView__) */
