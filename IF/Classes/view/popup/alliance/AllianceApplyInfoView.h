//
//  AllianceApplyInfoView.h
//  IF
//
//  Created by 〜TIAN~ on 15/8/5.
//
//

#ifndef __IF__AllianceApplyInfoView__
#define __IF__AllianceApplyInfoView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "InputFieldMultiLine.h"

class AllianceApplyInfoView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    ~AllianceApplyInfoView(){};
    AllianceApplyInfoView(std::string allianceId):m_allianceId(allianceId){};
    static AllianceApplyInfoView *create(std::string allianceId);
protected:
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void onSendClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onNoSendClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onShowTips(CCObject* data);
    void checkInput(float t);
    CCSafeObject<CCControlButton> m_sendBtn;
    CCSafeObject<CCControlButton> m_nosendBtn;
    CCSafeObject<CCNode> m_messageNode;
    CCSafeObject<CCNode> m_fireNode1;
    CCSafeObject<CCNode> m_fireNode2;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelIF> m_tipsLabel;
    CCSafeObject<InputFieldMultiLine> m_contentEditBox;
    std::string m_allianceId;
};
#endif /* defined(__IF__AllianceApplyInfoView__) */
