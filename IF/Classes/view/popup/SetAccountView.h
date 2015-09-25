//
//  SetAccountView.h
//  IF
//
//  Created by wangzhenlei on 13-11-4.
//
//

#ifndef __IF__SetAccountView__
#define __IF__SetAccountView__
#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"

class SetAccountView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static SetAccountView* create();
    SetAccountView():m_bReturnParent(true){};
    ~SetAccountView(){};
    CC_SYNTHESIZE(bool, m_bReturnParent, ReturnParent);
    CCNode* getGuideNode(string _key);
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onBindClick(CCObject *pSender, CCControlEvent event);
    void onChangeClick(CCObject *pSender, CCControlEvent event);
    void onNewAccountClick(CCObject *pSender, CCControlEvent event);
    
    void onOKNewAccount();
    void onLastOKNewAccount();
    void onSendNewAccount();
    void onRetWarOKNewAccount(CCObject* p);
    void onBidWarOKNewAccount();
    
    void onUpdateState();
    
    void onStartNewAccount();
    
    CCSafeObject<CCControlButton> m_bindBtn;
    CCSafeObject<CCControlButton> m_changeBtn;
    
    CCSafeObject<CCNode> m_Listnode;
    CCSafeObject<CCSprite> m_googleFlagspr;
    CCSafeObject<CCScale9Sprite> m_viewBg;
    CCSafeObject<CCNode> m_gNode;
    CCSafeObject<CCNode> m_topNode;
    CCSafeObject<CCNode> m_top2Node;
    
    CCSafeObject<CCControlButton> m_newBtn;
    
    CCSafeObject<Label> m_nameLabel;
    CCSafeObject<CCLabelIF> m_castleLabel;
    CCSafeObject<CCLabelIFTTF> m_fbActLabel;
    CCSafeObject<CCLabelIFTTF> m_gpActLabel;
    CCSafeObject<CCLabelIFTTF> m_fbActMsgLabel;
    CCSafeObject<CCLabelIFTTF> m_gpActMsgLabel;
    CCSafeObject<CCLabelIF> m_noAct1Label;
    CCSafeObject<CCLabelIF> m_noAct2Label;
    CCSafeObject<CCLabelIF> m_newActLabel;
    CCSafeObject<CCLabelIF> m_rdMsgLabel;
    CCSafeObject<CCNode> m_iconNode;
    
    WaitInterface* m_waitInterface;
    int m_optType;
    string m_facebookUid;
    string m_facebookName;
    string m_googleUid;
    string m_googleName;
};

class SetAccountNextView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static SetAccountNextView* create(int type);
    SetAccountNextView():m_bReturnParent(true){};
    ~SetAccountNextView(){};
    CC_SYNTHESIZE(bool, m_bReturnParent, ReturnParent);
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(int type);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onFacebookClick(CCObject *pSender, CCControlEvent event);
    void onGooglePlayClick(CCObject *pSender, CCControlEvent event);
    
    void OpenLoginFB();
    void OpenLoginGP();
    
    void FacebookCallback(CCObject* params);
    void GooglePlayCallback(CCObject* params);
    void UserBindCancel(CCObject* params);
    void updateButtonState(CCObject* p);
    void loginFacebook();
    
    void DisconnectFacebook();
    void DisconnectGooglePlay();
    void DisconnectIOS();
    
    void loginFacebookAction();
    void loginFacebookAction1();
    void loginCancel();
    void logingoogle();
    void logingoogleAction();
    void logingoogleAction1();
    void loginOtherAction(float delta);
    
    void onResponsed_3rdPlatform(CCObject *obj);
    
    CCSafeObject<CCControlButton> m_facebookBtn;
    CCSafeObject<CCControlButton> m_googlePlayBtn;
    CCSafeObject<CCControlButton> m_facebookLoginBtn;
    CCSafeObject<CCControlButton> m_googlePlayLoginBtn;
    
    CCSafeObject<CCLabelIFTTF> m_facebookLoginTitle1;
    CCSafeObject<CCLabelIFTTF> m_googleLoginTitle1;
    CCSafeObject<CCLabelIF> m_description;
    CCSafeObject<CCNode> m_gpIconNode;
    
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCNode> m_fireNode1;
    CCSafeObject<CCNode> m_fireNode2;
    CCSafeObject<CCNode> m_Listnode;
    CCSafeObject<CCSprite> m_googleFlagspr;
    CCSafeObject<CCScale9Sprite> m_viewBg;
    CCSafeObject<CCNode> m_gNode;
    CCSafeObject<CCNode> m_otherNode;
    CCSafeObject<CCDictionary> m_dict;
    WaitInterface* m_waitInterface;
    int m_optType;
    string m_facebookUid;
    string m_facebookName;
    string m_googleUid;
    string m_googleName;
    int m_type;
    std::string m_bindId;
    std::string m_bindName;
};

class SettingAccountButton : public CCNode{
public:
    static SettingAccountButton *create(int bindType, int panelType);
    void refreshState(CCObject *obj);
private:
    SettingAccountButton(int bindType, int panelType) : m_bindType(bindType), m_panelType(panelType){};
    int m_bindType;
    int m_panelType;
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    std::string getIconStr();
    CCSafeObject<CCControlButton> m_btn;
    
    CCSafeObject<CCSprite> m_icon;
    CCSafeObject<CCLabelIF> m_stateText;
    void onBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onBindBtnClick();
    void onUnBindBtnClick();
    void onChangeBtnClick();
    void login();
    void logout(CCObject *obj);
    void onConfirmUnBind();
};

class WarningView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
 
{
public:
    static WarningView* create(string title, string info, CCCallFunc* yesfunc);
    WarningView(){};
    ~WarningView(){};
    
private:
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(string title, string info, CCCallFunc* yesfunc);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onOKClick(CCObject *pSender, CCControlEvent event);
    void onCancelClick(CCObject *pSender, CCControlEvent event);
    
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelIF> m_infoLabel;
    CCSafeObject<CCControlButton> m_okBtn;
    CCSafeObject<CCControlButton> m_cancelBtn;
    CCSafeObject<CCNode> m_touchNode;
    CC_SYNTHESIZE_SAFEOBJECT(CCCallFunc, m_onYes, YesCallback);
    int m_yesCnt;
};

class SelServerView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
 
{
public:
    static SelServerView* create(CCCallFunc* yesfunc);
    SelServerView(){};
    ~SelServerView(){};
    
private:
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(CCCallFunc* yesfunc);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onOldClick(CCObject *pSender, CCControlEvent event);
    void onNewClick(CCObject *pSender, CCControlEvent event);
    
    CCSafeObject<CCLabelIF> m_infoLabel;
    CCSafeObject<CCControlButton> m_oldBtn;
    CCSafeObject<CCControlButton> m_newBtn;
    CCSafeObject<CCNode> m_touchNode;
    CC_SYNTHESIZE_SAFEOBJECT(CCCallFunc, m_onYes, YesCallback);
};

#endif /* defined(__IF__SetAccountView__) */
