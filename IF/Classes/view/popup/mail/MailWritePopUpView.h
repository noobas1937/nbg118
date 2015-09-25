//
//  MailWritePopUpView.h
//  IF
//
//  Created by 李锐奇 on 13-10-11.
//
//

#ifndef IF_MailWritePopUpView_h
#define IF_MailWritePopUpView_h

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCBExtension.h"
#include "CCLabelIF.h"
//#include "CCEditText.h"
#include "InputFieldMultiLine.h"

class MailWritePopUpView : public PopupBaseView
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
//, public CCTouchDelegate
{
public:
    static MailWritePopUpView* create(std::string toName, std::string title,std::string allianceId="",std::string thxMail = "");
    static MailWritePopUpView* createWithGift(std::string toName, int giftid = -1,int propID = 200010);
protected:
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
private:
    MailWritePopUpView(std::string toName, std::string title, std::string allianceId,std::string thxMail) : m_toName(toName), m_title(title),m_allianceId(allianceId),mType(0),m_thanksMail(thxMail),/*m_waitInterface(NULL)*/m_isSendingGift(false),mInited(false),mChooseUids(""),mUserCount(0){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool initWithGift(int giftID,int propID);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);

    void onCloseEvent(CCObject* p);
    void onCloseClick(CCObject *pSender, CCControlEvent event);
    void onSendClick(CCObject *pSender, CCControlEvent event);
    void onChooseName(CCObject *pSender, CCControlEvent event);
    void onInvitePlayer(CCObject *pSender, CCControlEvent event);
    void onUpdateStatusEvent(CCObject* p);
    void onChoosePlayer(CCObject* p);
    void updateBtnStatus();
    void onRefreshName(CCObject *p);
    
    void showChooseView();
    
    
    std::string m_toName;
    std::string m_title;
    std::string m_allianceId;
    std::string m_thanksMail;
    CCSafeObject<CCControlButton> m_sendBtn;
    CCSafeObject<CCControlButton> m_inviteBtn;
    CCSafeObject<CCControlButton> m_closeBtn;
    CCSafeObject<CCNode> m_titleContainer;
    CCSafeObject<CCNode> m_contentContainer;
    CCSafeObject<CCNode> m_nameContainer;
    
    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCLabelIF> m_contentText;
    CCSafeObject<CCNode> m_downNode;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    
    CCSafeObject<InputFieldMultiLine> m_nameEditBox;
    CCSafeObject<InputFieldMultiLine> m_titleEditBox;
    CCSafeObject<InputFieldMultiLine> m_contentEditBox;
    
    CCSafeObject<CCSprite> m_sprMailIcon;
    
    //送礼增加
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCNode> m_nodeContent;
    bool m_isSendingGift;
//    WaitInterface* m_waitInterface;
    CCSafeObject<CCNode> m_waitingNode;
    void onSendGift();
    void onSendGiftBack(CCObject* p);
    int mType;
    int mPropID; // 火漆信等道具ID
    std::string mChooseUids;
    int mUserCount;
    
    bool mInited;
};

#endif
