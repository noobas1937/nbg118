//
//  MailReadPopUpView.h
//  IF
//
//  Created by 李锐奇 on 13-10-11.
//
//

#ifndef IF_MailReadPopUpView_h
#define IF_MailReadPopUpView_h

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCBExtension.h"
#include "CCLabelIF.h"
#include "MailInfo.h"
#include "HFHeadImgNode.h"

class MailReadPopUpView : public PopupBaseView
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
{
public:
    static MailReadPopUpView* create(MailInfo& info);
protected:
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
private:
    MailReadPopUpView(MailInfo& info) : m_info(&info){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);

    void onDeleteClick(CCObject *pSender, CCControlEvent event);
    void onBlockClick(CCObject *pSender, CCControlEvent event);
    void onReplyClick(CCObject *pSender, CCControlEvent event);
    void onDonateClick(CCObject *pSender, CCControlEvent event);
    void onRewardClick(CCObject *pSender, CCControlEvent event);
    void onAddSaveClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onReturnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onWriteClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onJoinAllianceBtnClick(CCObject *pSender, CCControlEvent event);
    void onRefuseAllianceBtnClick(CCObject *pSender, CCControlEvent event);
    void onShareBtnClick(CCObject *pSender, CCControlEvent event);
    void onFbLikeClick(CCObject *pSender, CCControlEvent event);
    void refreshContent(CCObject* p);
    void refrehsReward();
    void onOkDeleteMail();
    void showRewardAnim();
    void showJoinAllianceBtn();
    void blockAction();
    void setAllBtnPosition();
    void showInviteTeleportBtn();
    void showKickOutInterface();
    void showRefuseApplyInterface();
    void showShareBtn();
    void onLinkClicked(CCObject *ccObj = NULL);
    void ckfRewardHandle();
    
    void showAllianceOrderBtn();
    void onGoToAllianceOrder(cocos2d::CCObject *pSender, CCControlEvent event);
    bool checkRewardBtnVisible();
    
    CCSafeObject<MailInfo> m_info;
    
    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCLabelIF> m_timeText;
    CCSafeObject<CCLabelIF> m_rewardTitleText;
    CCSafeObject<CCLabelIF> m_mailTitle;
    CCSafeObject<CCNode> m_contentContainer;
    CCSafeObject<CCNode> m_userHeadContainer;
    CCSafeObject<CCSprite> m_userHeadBack;
    CCSafeObject<CCNode> m_downNode;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_totalNode;
    CCSafeObject<CCNode> m_rewardNode;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCScale9Sprite> m_listBG;

//    CCSafeObject<CCNode> m_blockBtnNode;
    CCSafeObject<CCControlButton> m_deleteBtn;
    
    CCSafeObject<CCControlButton> m_blockBtn;
    CCSafeObject<CCControlButton> m_rewardBtn;
    CCSafeObject<CCControlButton> m_replyBtn;
    CCSafeObject<CCControlButton> m_likeBtn;
    CCSafeObject<CCControlButton> m_donateBtn;
//    CCSafeObject<CCSprite> m_blockSprite;
//    CCSafeObject<CCSprite> m_deleteSprite;
   // CCSafeObject<CCSprite> m_savespr;
  //  CCSafeObject<CCSprite> m_unsavespr;
//    CCSafeObject<CCLabelIF> m_deleteBtnTitle;

  //  CCSafeObject<CCLabelIF> m_saveBtnTitle;

    

//    CCSafeObject<CCScale9Sprite> m_line;
    CCSafeObject<CCNode> m_guideNode;
    CCSafeObject<CCNode> m_rewardContainer;
    CCSafeObject<CCScrollView> m_scroll;

    CCSafeObject<CCControlButton> m_addSaveBtn;
    CCSafeObject<CCControlButton> m_writeBtn;
    CCSafeObject<CCControlButton> m_returnBtn;
    CCSafeObject<CCSprite> m_returnSpr;
//    CCSafeObject<CCControlButton> m_unSaveBtn;
    CCSafeObject<CCModelLayerColor>m_modelLayer;
    CCSafeObject<CCArray> m_listArr;
    CCSafeObject<CCArray> m_listAnimArr;
    CCSafeObject<CCNode> m_ccbNode;
    CCSafeObject<CCNode> m_ListNode;
    CCSafeObject<CCControlButton> m_joinAllianceBtn;
    CCSafeObject<CCControlButton> m_refuseAllianceBtn;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    CCSafeObject<CCRenderTexture> m_selfModelLayer;
    CCSafeObject<Node> m_underLineNode;
    int m_totalH;
    std::vector<std::string> m_linkUrls;
    int _allianceOrderType;
    bool m_blendFlag;
};
//联盟邀请\申请邮件
class MailAllianceInviteCell:public CCIFTouchNode
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
 
{
public:
    static MailAllianceInviteCell* create(MailInfo* info);
    MailAllianceInviteCell(MailInfo* info):m_mailInfo(info){};
    ~MailAllianceInviteCell(){};
protected:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
private:
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void setData();
    void onJoinAllianceBtnClick(CCObject *pSender, CCControlEvent event);
    void joinSuccess(CCObject* p);
    void onRefuseAllianceBtnClick(CCObject *pSender, CCControlEvent event);
    void onSendMailBtnClick(CCObject *pSender, CCControlEvent event);
    void showAllianceInfo(CCObject* obj);
    CCSafeObject<CCLabelIF> m_alliancenameText;
    CCSafeObject<CCLabelIF> m_allianceLeaderText;
    CCSafeObject<CCLabelIF> m_powerText;
    CCSafeObject<CCLabelIF> m_allianceNumText;
    CCSafeObject<CCLabelIF> m_allianceDesText;
    CCSafeObject<CCLabelIF> m_langText;
    CCSafeObject<CCNode> m_flagNode;
    CCSafeObject<CCControlButton> m_joinAllianceBtn;
    CCSafeObject<CCControlButton> m_refuseAllianceBtn;
    CCSafeObject<CCControlButton> m_sendMailBtn;
    CCSafeObject<CCSprite> m_flagSpr;
//    CCSafeObject<CCScale9Sprite> m_lineFlag;
    CCSafeObject<CCSprite> m_bg;
    
    CCSafeObject<CCNode> m_inviteNode;
    CCSafeObject<CCNode> m_applyNode;
    CCSafeObject<CCNode> m_headIcon;
    CCSafeObject<CCLabelIF> m_powerTxt;
    CCSafeObject<CCLabelIFTTF> m_playerNameTxt;
    CCSafeObject<CCLabelIF> m_powerValue;
    CCSafeObject<MailInfo> m_mailInfo;

};

//联盟踢人邮件
class MailAllianceKickCell: public CCIFTouchNode
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
  
{
public:
    static MailAllianceKickCell* create(MailInfo* info);
    MailAllianceKickCell(MailInfo* info): m_mailInfo(info){};
    ~MailAllianceKickCell(){};
    void setData();
protected:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onViewOtherAllBtnClick(CCObject *pSender, CCControlEvent event);
private:
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void joinSuccess(CCObject* p);
    CCSafeObject<CCLabelIF> m_alliancenameText;
    CCSafeObject<CCLabelIF> m_allianceLeaderText;
    CCSafeObject<CCLabelIF> m_powerText;
    CCSafeObject<CCLabelIF> m_allianceNumText;
    CCSafeObject<CCLabelIF> m_allianceDesText;
    CCSafeObject<CCLabelIF> m_langText;
    CCSafeObject<CCNode> m_flagNode;
    CCSafeObject<CCSprite> m_flagSpr;
//    CCSafeObject<CCScale9Sprite> m_lineFlag;
    CCSafeObject<CCNode> m_kickNode;
    
    CCSafeObject<MailInfo> m_mailInfo;
};

//邀请迁城邮件
class MailInviteTeleCell: public CCNode
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
{
public:
    static MailInviteTeleCell* create(MailInfo* info);
    MailInviteTeleCell(MailInfo* info): m_mailInfo(info){};
    ~MailInviteTeleCell(){};
    void setData();
protected:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
private:
    void removeWaitInterface();
    void afterGetNearestPoint(CCObject*);
    void onRefuseBtnClick(CCObject *pSender, CCControlEvent event);
    void onAcceptBtnClick(CCObject *pSender, CCControlEvent event);
    CCSafeObject<CCNode> m_btnNode;
    CCSafeObject<CCControlButton> m_refuseBtn;
    CCSafeObject<CCControlButton> m_acceptBtn;
    CCSafeObject<CCLabelIF> m_desText;
    CCSafeObject<MailInfo> m_mailInfo;
    WaitInterface* m_waitInterFace1;
    WaitInterface* m_waitInterFace2;
};

class ItemAndGeneralCell : public CCNode{
public:
    static ItemAndGeneralCell *create(int type, int value, int num);
private:
    virtual bool init(int type, int value, int num);
};

class DetailRewardCell : public CCTableViewCell
,public CCBSelectorResolver
,public CCBMemberVariableAssigner

{
public:
    static DetailRewardCell *create(int type,int value,int num,int color=0,bool gray = false);
    void setData(int type,int value,int num,int color,bool gray);
    CCNode* getPicNode();
private:
    DetailRewardCell(int type,int value,int num,int color=0,bool gray = false):m_type(type),m_value(value),m_num(num),m_color(color),m_gay(gray){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCLabelIF> m_numText;
    CCSafeObject<CCControlButton> m_rewardBtn;
    int m_type;
    int m_value;
    int m_num;
    int m_color;
    bool m_gay;
};
#endif
