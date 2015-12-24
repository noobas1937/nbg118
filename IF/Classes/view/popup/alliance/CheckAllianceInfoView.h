//
//  CheckAllianceInfoView.h
//  IF
//
//  Created by ganxiaohua on 14-8-22.
//
//

#ifndef __IF__CheckAllianceInfoView__
#define __IF__CheckAllianceInfoView__
#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"
#include "AllianceInfo.h"
#include "WaitInterface.h"

class CheckAllianceInfoView: public PopupBaseView
,public CCBSelectorResolver
 
,public CCBMemberVariableAssigner
{
public:
    static CheckAllianceInfoView* create(AllianceInfo* info);
    CheckAllianceInfoView(AllianceInfo* info):m_info(info){};
    virtual ~CheckAllianceInfoView(){};
    void updateInfo();
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){};
    
    void saveSuccess();
    void initFun();
    void updateAnnounce(CCObject* param);
    
    void onMailClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onMemberClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onCommentClick(CCObject * pSender, Control::EventType pCCControlEvent);

    void revokeAlliacne(CCObject * pSender, Control::EventType pCCControlEvent);
    void applyAlliacne(CCObject * pSender, Control::EventType pCCControlEvent);
    void joinAlliacne(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void success(CCObject* obj);
    void joinSuccess(CCObject* obj);
    void cancelApply(CCObject* obj);
    void addBtnPar(CCControlButton* btn);
    void removeBtnPar(CCControlButton*);
    void checkComment(CCObject* obj);
    
    
    CCSafeObject<CCLabelIFTTF> m_leaderTxt;
    CCSafeObject<CCLabelIF> m_powerTxt;
    CCSafeObject<CCLabelIF> m_membersTxt;
    CCSafeObject<CCLabelIFTTF> m_nameTxt;
    CCSafeObject<CCLabelIFTTF> m_announceTxt;
    CCSafeObject<CCLabelIF> m_language;
    CCSafeObject<CCLabelIFTTF> m_enterStrict;
    CCSafeObject<CCLabelIFTTF> m_ConNumShow;
    
    CCSafeObject<CCNode> m_announceNode;
    CCSafeObject<CCEditBox> m_announceEditBox;
    CCSafeObject<CCScale9Sprite> m_viewBg;
    CCSafeObject<CCNode> m_allianceIcon;
    CCSafeObject<CCNode> m_funList;
    CCSafeObject<CCNode> m_clickNode;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCControlButton> m_rovkeBtn;
    CCSafeObject<CCControlButton> m_applyBtn;
    CCSafeObject<CCControlButton> m_joinAlliance;
    CCSafeObject<CCControlButton> m_RestrictApply;

    CCSafeObject<CCControlButton> m_btnMail;
    CCSafeObject<CCControlButton> m_btnMember;
    CCSafeObject<CCControlButton> m_btnComment;
    CCSafeObject<CCNode> m_textNode;
    CCSafeObject<WaitInterface> m_waitInterface;
    CCSafeObject<CCNode> m_middleNode;
    CCSafeObject<CCNode> m_zhandouNode;
    CCSafeObject<CCNode> m_renshuNode;
    
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<AllianceInfo> m_info;
};
#endif /* defined(__IF__CheckAllianceInfoView__) */
