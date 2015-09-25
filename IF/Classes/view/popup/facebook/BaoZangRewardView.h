//
//  BaoZangRewardView.h
//  IF
//
//  Created by ganxiaohua on 14-11-19.
//
//

#ifndef __IF__BaoZangRewardView__
#define __IF__BaoZangRewardView__

#include "CommonInclude.h"
#include "PopupBaseView.h"

class BaoZangRewardView : public PopupBaseView
 
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:

    static BaoZangRewardView* create(CCDictionary* info);
    BaoZangRewardView(CCDictionary* info):m_rewardInfo(info){};
    virtual ~BaoZangRewardView(){};
    
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
    
    void onThankClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void collectCallback(CCObject* params);

    CCSafeObject<CCLabelIF> m_rewardTitle;
    CCSafeObject<CCLabelIF> m_reward1;
    CCSafeObject<CCLabelIF> m_reward2;
    CCSafeObject<CCControlButton> m_thankBtn;
    CCSafeObject<CCNode> m_goldNode;
    CCSafeObject<CCNode> m_icon2;
    CCSafeObject<CCNode> m_clickArea;
    CCSafeObject<CCDictionary> m_rewardInfo;
};

#endif /* defined(__IF__BaoZangRewardView__) */
