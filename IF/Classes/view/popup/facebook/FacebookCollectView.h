//
//  FacebookCollectView.h
//  IF
//
//  Created by ganxiaohua on 14-10-15.
//
//

#ifndef __IF__FacebookCollectView__
#define __IF__FacebookCollectView__

#include "CommonInclude.h"
#include "PopupBaseView.h"

class FacebookCollectView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    CREATE_FUNC(FacebookCollectView);
    
    FacebookCollectView();
    ~FacebookCollectView();
    
private:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    void onFacebookClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onCollectClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void FacebookCallback(CCObject* params);
    void collectCallback(CCObject* params);
    void delayUseData(float _time);
    
    CCSafeObject<CCScale9Sprite> m_viewBg;
    CCSafeObject<CCLabelIF> m_buttonTxt;
    CCSafeObject<CCLabelIF> m_descTxt;
    CCSafeObject<CCLabelIF> m_rewardTxt;
    CCSafeObject<CCControlButton> m_loginBtn;
    CCSafeObject<CCControlButton> m_collectBtn;
    CCSafeObject<CCNode> m_facebookNode;
    CCSafeObject<CCNode> m_goldNode;
};

#endif /* defined(__IF__FacebookCollectView__) */
