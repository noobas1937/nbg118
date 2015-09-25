//
//  InviteForGiftView.h
//  IF
//
//  Created by xxrdsg on 14-11-20.
//
//

#ifndef __IF__InviteForGiftView__
#define __IF__InviteForGiftView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"

class InviteForGiftView: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
  
, public cocos2d::extension::CCMultiColTableViewDataSource
, public cocos2d::extension::CCMultiColTableViewDelegate
{
public:
    static InviteForGiftView* create();
    InviteForGiftView();
    ~InviteForGiftView();
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickGiftBtn(CCObject *pSender, CCControlEvent event);
    void onClickInviteBtn(CCObject *pSender, CCControlEvent event);
    void onCopyClick(CCObject *pSender, CCControlEvent event);
    void onEnterClick(CCObject *pSender, CCControlEvent event);
    void onMailBtnClick(CCObject *pSender, CCControlEvent event);
    void onJumpBtnClick(CCObject *pSender, CCControlEvent event);
    void onBranchBtnClick(CCObject *pSender, CCControlEvent event);
    
    void afterGetFriendsInfo(CCObject* param);
    void getInviteFriends(CCObject *data);
    void checkFb(float _time);
    
    void onRwdInfoClick(CCObject *pSender, CCControlEvent event);
    void getBranchUrl(CCObject *param);
    void onFacebookClick(CCObject *pSender, CCControlEvent event);
    
    void updateInviterInfo();
    void onCodeCallBack(CCObject* param);
    void onInviteInfoCallBack(CCObject* param);
    void onInviteRewardCallBack(CCObject* param);
    void enterCode();
    void clearCode();
    void refreshData(CCObject* param);
    void loginSuccess(CCObject* param);
    void bindSuccess(CCObject* param);
    void delayCall(float t_time);
    void getRequestFriends(CCObject *data);
    void delayShowData(float t);
    void delayShowAppRequestView(float t);
    void getServerCallBack(CCObject *data);
    void refreshTableView(CCObject *obj=NULL);
    void refreshReward(int receiveCount);//刷新奖品
    
    void removeWaitInterface(float t);
    
    CCSafeObject<CCScale9Sprite> m_buildBG;
//    CCSafeObject<CCNode> m_spriteNode;
    CCSafeObject<CCNode> m_scrollNode1;
    CCSafeObject<CCNode> m_scrollNode2;
    CCSafeObject<CCNode> m_titleNode3;
//    CCSafeObject<CCScrollView> m_scrollView1;
    CCSafeObject<CCScrollView> m_scrollView2;
    CCSafeObject<CCNode> m_infoGiftNode1;
    CCSafeObject<CCNode> m_infoGiftNode2;
    CCSafeObject<CCNode> m_infoInviteNode;
//    CCSafeObject<CCNode> m_introGiftNode;
    CCSafeObject<CCNode> m_introInviteNode;
    
    CCSafeObject<CCControlButton> m_giftBtn;
    CCSafeObject<CCControlButton> m_inviteBtn;
    CCSafeObject<CCControlButton> m_copyBtn;
    CCSafeObject<CCLabelIF> m_copyBtnText;
    CCSafeObject<CCControlButton> m_enterBtn;
    CCSafeObject<CCControlButton> m_mailBtn;
    CCSafeObject<CCControlButton> m_jumpBtn;
    CCSafeObject<CCLabelIF> m_jumpBtnText;
    CCSafeObject<CCControlButton> m_rwdInfoBtn;
    
//    CCSafeObject<CCLabelIF> m_codeLabel;
    CCSafeObject<CCLabelIF> m_infoLabel1;
    CCSafeObject<CCLabelIF> m_infoLabel2;
    CCSafeObject<CCLabelIF> m_infoLabel3;
    CCSafeObject<CCLabelIF> m_infoLabel4;
    CCSafeObject<CCLabelIF> m_infoLabel5;
    CCSafeObject<CCLabelIF> m_infoLabel6;
    CCSafeObject<CCLabelIF> m_infoLabel7;
//    CCSafeObject<CCLabelIF> m_infoLabel8;
//    CCSafeObject<CCLabelIF> m_infoLabel9;
//    CCSafeObject<CCLabelIF> m_infoLabel10;
//    CCSafeObject<CCLabelIF> m_infoLabel11;
    CCSafeObject<CCLabelIF> m_infoLabel12;
//    CCSafeObject<CCLabelIF> m_infoLabel13;
//    CCSafeObject<CCLabelIF> m_infoLabel14;
//    CCSafeObject<CCLabelIF> m_infoLabel15;
//    CCSafeObject<CCLabelIF> m_infoLabel16;
//    CCSafeObject<CCLabelIF> m_fbLabel;
    
    CCSafeObject<CCEditBox> m_editBox;
    WaitInterface* m_waitInterface1;
    CCSafeObject<CCNode> m_fbNode;
    CCSafeObject<CCControlButton> m_facebookBtn;
    CCSafeObject<CCLabelIF> m_fbText;
    CCSafeObject<CCNode> m_ccbNode;
    //领奖node
    CCSafeObject<CCNode> m_rewardNode;
    CCSafeObject<CCNode> m_rwdNode[4];
    CCSafeObject<CCNode> m_rwdPicNode[4];
    CCSafeObject<CCLabelIF> m_rwdNum[4];
    CCSafeObject<CCLayerColor> m_rwdCover[4];
    CCSafeObject<CCControlButton> m_rewardBtn;
    CCSafeObject<CCLabelIF> m_rewardBtnText;
    void onRewardBtnClick(CCObject *pSender, CCControlEvent event);
    CCSafeObject<CCLabelIF> m_upBarText;
    CCSafeObject<CCLabelIF> m_downBarTxt1;
    CCSafeObject<CCLabelIF> m_downBarTxt3;
    CCSafeObject<CCLabelIF> m_downBarTxt5;
    CCSafeObject<CCLabelIF> m_downBarTxt10;
    CCSafeObject<CCLabelIF> m_downBarTxt20;
    CCSafeObject<CCScale9Sprite> m_progressBar;
    CCSafeObject<CCLabelIF> m_titleTxt1;
    CCSafeObject<CCLabelIF> m_titleTxt2;
    CCSafeObject<CCLabelIF> m_titleTxt3;
    CCSafeObject<CCNode> m_upBgSpr1;
    
    CCSafeObject<CCNode> m_jumpBtnNode;
    CCSafeObject<CCNode> m_copyBtnNode;
    CCSafeObject<CCNode> m_branchBtnNode;
    CCSafeObject<CCControlButton> m_branchBtn;
    CCSafeObject<CCLabelIF> m_branchBtnText;
    CCSafeObject<CCNode> m_fbIcon;
    
    int m_page;//标示在gift界面（1） 还是在 invite界面（2）
    string m_inviterUid;//邀请我的人的uid
    string m_inviterName;//
    string m_inviterServer;//
    bool m_isInvited;//是否被邀请过
    int m_inviteCount;//我已经邀请了的朋友的个数
    int m_receiveCount;//已领取邀请朋友的个数的奖品
    int m_countNumber;//当前领取的奖品号
    string m_inviCode;
//    bool m_first;
    string m_fbUid;
    string m_requestUids;
    CCSafeObject<CCArray> m_requestArr;
    CCSafeObject<CCArray> m_installData;
    CCSafeObject<CCArray> m_inviteData;
    int m_time;
    bool m_fbClick;
    bool m_isIOS;
//    CCSafeObject<CCArray> m_rewardArr1;//奖品数组
//    CCSafeObject<CCArray> m_rewardArr2;//奖品数组
    CCSafeObject<CCDictionary> m_rewardSaveDict;//保存的奖品字段
    CCSafeObject<CCNode> m_tableViewNode;
//    float m_tableViewNodeX;
    CCSafeObject<CCLabelIF> m_tvTipTxt;
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCMultiColTableView> m_tabView;
    //安卓ios开关
    bool m_android;
    bool m_ios;
protected:
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
    virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual unsigned int numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
    
};


class InvitationMethodView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
  
{
public:
    static InvitationMethodView* create(string inviteStr);
    InvitationMethodView();
    ~InvitationMethodView();
    
private:
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(string inviteStr);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
//    void onGoFacebookClick(CCObject *pSender, CCControlEvent event);
    void onGoTwitterClick(CCObject *pSender, CCControlEvent event);
    void afterGetFriendsInfo(CCObject* param);
    void getInviteFriends(CCObject *data);
    void checkFb(float _time);
    void loginSuccess(CCObject* param);
    void bindSuccess(CCObject* param);
    
    CCSafeObject<CCLabelIF> m_infoLabel1;
    CCSafeObject<CCLabelIF> m_infoLabel2;
//    CCSafeObject<CCControlButton> m_goFacebookBtn;
    CCSafeObject<CCControlButton> m_goTwitterBtn;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_twitterIcon;
    WaitInterface* m_waitInterface1;
    CCSafeObject<CCArray> m_installData;
    CCSafeObject<CCArray> m_inviteData;
    int m_time;
    string m_fbUid;
    string m_inviteStr;
    
};


class InviteRewardInfoView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
  
{
public:
    static InviteRewardInfoView* create();
    InviteRewardInfoView(){};
    ~InviteRewardInfoView(){};
    
private:
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    CCSafeObject<CCLabelIF> m_infoLabel1;
    CCSafeObject<CCLabelIF> m_infoLabel2;
    CCSafeObject<CCLabelIF> m_infoLabel3;
    CCSafeObject<CCLabelIF> m_infoLabel4;
    CCSafeObject<CCLabelIF> m_infoLabel5;
    CCSafeObject<CCLabelIF> m_infoLabel6;
    CCSafeObject<CCLabelIF> m_infoLabel7;
    CCSafeObject<CCLabelIF> m_infoLabel8;
    CCSafeObject<CCLabelIF> m_infoLabel9;
    CCSafeObject<CCLabelIF> m_infoLabel10;
    CCSafeObject<CCLabelIF> m_infoLabel11;
    CCSafeObject<CCLabelIF> m_infoLabel12;
    CCSafeObject<CCLabelIF> m_infoLabel13;
    CCSafeObject<CCLabelIF> m_infoLabel14;
    CCSafeObject<CCLabelIF> m_infoLabel15;
    CCSafeObject<CCLabelIF> m_infoLabel16;
    CCSafeObject<CCLabelIF> m_infoLabel17;
    CCSafeObject<CCLabelIF> m_infoLabel18;
    CCSafeObject<CCLabelIF> m_infoLabel19;
    CCSafeObject<CCLabelIF> m_infoLabel20;

    CCSafeObject<CCNode> m_touchNode;
};

class InviteWarningView: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
  
{
public:
    static InviteWarningView* create(CCCallFunc* yesfunc, CCCallFunc* nofunc);
    InviteWarningView(){};
    ~InviteWarningView(){};
    
private:
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(CCCallFunc* yesfunc, CCCallFunc* nofunc);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onOKClick(CCObject *pSender, CCControlEvent event);
    void onCancelClick(CCObject *pSender, CCControlEvent event);
    
//    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelIF> m_infoLabel;
    CCSafeObject<CCControlButton> m_okBtn;
    CCSafeObject<CCControlButton> m_cancelBtn;
    CCSafeObject<CCNode> m_touchNode;
    CC_SYNTHESIZE_SAFEOBJECT(CCCallFunc, m_onYes, YesCallback);
    CC_SYNTHESIZE_SAFEOBJECT(CCCallFunc, m_onNo, NoCallback);
};


#endif /* defined(__IF__InviteForGiftView__) */
