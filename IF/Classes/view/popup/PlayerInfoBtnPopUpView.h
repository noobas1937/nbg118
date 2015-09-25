//
//  PlayerInfoBtnPopUpView.h
//  IF
//
//  Created by lifangkai on 14-9-15.
//
//

#ifndef __IF__PlayerInfoBtnPopUpView__
#define __IF__PlayerInfoBtnPopUpView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
//#include "CCScrollView.h"
#include "CCMultiColTableView.h"
#include "InputFieldMultiLine.h"
#include "DefaultTableViewDelegate.h"

class PlayerInfoBtnPopUpView :
public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
 
{
public:
    static PlayerInfoBtnPopUpView* create(std::string checkName = "");
    PlayerInfoBtnPopUpView():m_dy(0),m_bCheckName(""){};
    virtual ~PlayerInfoBtnPopUpView(){};
    void InitBtns(AllianceDonateInfo* info);
    void InitCommonBtns(string name,string uid,bool isMail = true);
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(std::string checkName);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){};
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onKickOutBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onMailBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onViewBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBlockBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onCancelBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onInviteClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onJoinClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void callFunc(CCObject* param);
    void joinFunc(CCObject* param);
    void inviteFunc();
    void onYesBlock();
    void kickOutFun();
    
    CCSafeObject<CCControlButton> m_kickOutBtn;
    CCSafeObject<CCControlButton> m_mailBtn;
    CCSafeObject<CCControlButton> m_viewBtn;
    CCSafeObject<CCControlButton> m_blockBtn;
    CCSafeObject<CCControlButton> m_cancelBtn;
    CCSafeObject<CCControlButton> m_inviteBtn;
    CCSafeObject<CCControlButton> m_joinBtn;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCNode> m_titleBg;
    CCSafeObject<CCNode> m_totalNode;
    AllianceDonateInfo* m_info;
    
    int m_idx;
    int m_type;
    string m_uid;
    string m_name;
    string m_msg;
    float m_dy;
    string m_bCheckName;
};

#endif /* defined(__IF__PlayerInfoBtnPopUpView__) */
