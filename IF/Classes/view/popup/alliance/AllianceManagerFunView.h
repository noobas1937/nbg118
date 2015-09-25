//
//  AllianceManagerFunView.h
//  IF
//
//  Created by ganxiaohua on 14-8-12.
//
//

#ifndef __IF__AllianceManagerFunView__
#define __IF__AllianceManagerFunView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "AllianceInfoMembersView.h"
#include "AllianceManager.h"

enum AllianceOrderType {
    ORDER_COLLECT = 0
    ,ORDER_SCIENCE
    ,ORDER_HELP
    ,ORDER_DEFEND
    ,ORDER_FIGHT
    ,ORDER_BUY
};

enum AllianceButtonState {
    ButtonMail = 0
    ,ButtonTrade
    ,ButtonRallyState
    ,ButtonTranLeader
    ,ButtonUpRank
    ,ButtonDownRank
    ,ButtonKickOut
    ,ButtonAccept
    ,ButtonRefuse
    ,ButtonReplaceLeader
    ,ButtonRelinquish//relinquish 禅让王位
    ,ButtonInviteUser//邀请
    ,ButtonPopInviteTeleport//邀请迁城
    ,ButtonSendGift
    ,ButtonPlayerInfo//盟主信息
    ,ButtonApplyMessage//申请信息
    ,ButtonOrder//联盟指令
};

class AllianceManagerFunView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
{
public:
    static AllianceManagerFunView *create(AllianceInfoMember* info,CCPoint pos=ccp(0,0),bool invite=false);
    ~AllianceManagerFunView(){};
    AllianceManagerFunView(AllianceInfoMember* info,CCPoint pos,bool invite):m_info(info),m_fromPos(pos),m_invite(invite){};
protected:
    // scrollview delegate
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void onClickConfirm(CCObject *pSender, CCControlEvent event);
    void onClickCancel(CCObject *pSender, CCControlEvent event);
    void setButtonState(CCNode* node,AllianceButtonState state);
    
    void upRankFun();
    void downRankFun();
    void kickOutFun();
    void leaderTransFun();
    void refresh();
    void leaderTransSuccess(CCObject* p);
    void removeParticle(float t);
    void addParticle(bool up=true);
    void addParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    void rDownAni(float t);
    void rUpAni(float t);
    void close(CCObject* p);
    void close(float t);
    void setButtonStatus();
    void playFadeIn(CCObject* p);
    void playFadeInFinish();
    void playFadeOut();
    void playFadeOutFinish(float t);
    void addButton(RankType type,int rank);
    void replaceLeaderFun();
    void replaceSuccess(CCObject* param);
    void shanRang1();
    void shanRang2();
    void shanRangSuccess(CCObject* param);
    void inviteCallBack(CCObject* param);
    void secondConfirm();
    void checkIfStorage(CCObject* param);
    //邀请迁城
    void inviteTeleport();
    
    CCSafeObject<CCNode> m_icon;
    CCSafeObject<CCNode> m_clickNode;
    CCSafeObject<CCLabelIFTTF> m_nameTxt;
    
    CCSafeObject<CCNode> m_node1;
    CCSafeObject<CCNode> m_btnNode1;
    CCSafeObject<CCLabelIF> m_nameText1;
    
    CCSafeObject<CCNode> m_node2;
    CCSafeObject<CCNode> m_btnNode2;
    CCSafeObject<CCLabelIF> m_nameText2;
    
    CCSafeObject<CCNode> m_node3;
    CCSafeObject<CCNode> m_btnNode3;
    CCSafeObject<CCLabelIF> m_nameText3;
    
    CCSafeObject<CCNode> m_node4;
    CCSafeObject<CCNode> m_btnNode4;
    CCSafeObject<CCLabelIF> m_nameText4;
    
    CCSafeObject<CCNode> m_node5;
    CCSafeObject<CCNode> m_btnNode5;
    CCSafeObject<CCLabelIF> m_nameText5;
    
    CCSafeObject<CCNode> m_node6;
    CCSafeObject<CCNode> m_btnNode6;
    CCSafeObject<CCLabelIF> m_nameText6;
    
    CCSafeObject<CCNode> m_node7;
    CCSafeObject<CCNode> m_btnNode7;
    CCSafeObject<CCLabelIF> m_nameText7;
    
    CCSafeObject<CCNode> m_node8;
    CCSafeObject<CCNode> m_btnNode8;
    CCSafeObject<CCLabelIF> m_nameText8;
    
    CCSafeObject<CCNode> m_node9;
    CCSafeObject<CCNode> m_btnNode9;
    CCSafeObject<CCLabelIF> m_nameText9;
    
    CCSafeObject<CCNode> m_node10;
    CCSafeObject<CCNode> m_btnNode10;
    CCSafeObject<CCLabelIF> m_nameText10;
    
    CCSafeObject<CCNode> m_node11;
    CCSafeObject<CCNode> m_btnNode11;
    CCSafeObject<CCLabelIF> m_nameText11;
    
    CCSafeObject<CCNode> m_nodeClick1;
    CCSafeObject<CCNode> m_nodeClick2;
    CCSafeObject<CCNode> m_nodeClick3;
    CCSafeObject<CCNode> m_nodeClick4;
    CCSafeObject<CCNode> m_nodeClick5;
    CCSafeObject<CCNode> m_nodeClick6;
    CCSafeObject<CCNode> m_nodeClick7;
    CCSafeObject<CCNode> m_nodeClick8;
    CCSafeObject<CCNode> m_nodeClick9;
    CCSafeObject<CCNode> m_nodeClick10;
    CCSafeObject<CCNode> m_nodeClick11;
    
    CCSafeObject<CCNode> m_parNode;
    CCSafeObject<CCNode> m_parNode2;
    CCSafeObject<CCSprite> m_rankUp;
    CCSafeObject<CCSprite> m_rankDown;
    CCSafeObject<CCSprite> m_bodyIcon;
    
    AllianceInfoMember* m_info;
    std::vector<CCNode*> m_buttonNodes;
    std::vector<AllianceButtonState> m_buttonState;
    std::vector<CCParticleBatchNode*>m_parVec;
    vector<string> m_buttonIcons;
    vector<string> m_buttonNames;
    vector<AllianceButtonState> m_buttonFuns;
    
    CCPoint m_fromPos;
    bool m_playingFadeOut;
    bool m_invite;
};

class AllianceOrderView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static AllianceOrderView *create(string uid);
    ~AllianceOrderView(){};
    AllianceOrderView():m_uid(""){};
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(string uid);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onClickBtn1(CCObject *pSender, CCControlEvent event);
    void onClickBtn2(CCObject *pSender, CCControlEvent event);
    void onClickBtn3(CCObject *pSender, CCControlEvent event);
    void onClickBtn4(CCObject *pSender, CCControlEvent event);
    void onClickBtn5(CCObject *pSender, CCControlEvent event);
    void onClickBtn6(CCObject *pSender, CCControlEvent event);
    
    void onSendOrder(int cmd);
    
    CCSafeObject<CCControlButton> m_btn1;
    CCSafeObject<CCControlButton> m_btn2;
    CCSafeObject<CCControlButton> m_btn3;
    CCSafeObject<CCControlButton> m_btn4;
    CCSafeObject<CCControlButton> m_btn5;
    CCSafeObject<CCControlButton> m_btn6;
    CCSafeObject<CCLabelIF> m_btn1Label;
    CCSafeObject<CCLabelIF> m_btn2Label;
    CCSafeObject<CCLabelIF> m_btn3Label;
    CCSafeObject<CCLabelIF> m_btn4Label;
    CCSafeObject<CCLabelIF> m_btn5Label;
    CCSafeObject<CCLabelIF> m_btn6Label;
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCNode> m_touchNode;
    string m_uid;
};
#endif /* defined(__IF__AllianceManagerFunView__) */
