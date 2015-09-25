//
//  AgainstInfoView.h
//  IF
//
//  Created by ganxiaohua on 15/8/27.
//
//

#ifndef __IF__AgainstInfoView__
#define __IF__AgainstInfoView__

#include "CommonInclude.h"
#include "PopupBaseView.h"

class AgainstInfoView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static AgainstInfoView *create();
    AgainstInfoView(){};
    ~AgainstInfoView(){};
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    CCSafeObject<CCLabelIF> m_infoTxt;
    CCSafeObject<CCLabelIF> m_timeTitle;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCLabelIF> m_allianceNameTxt;
    CCSafeObject<CCLabelIF> m_leaderTxt;
    CCSafeObject<CCLabelIF> m_powerTxt;
    CCSafeObject<CCLabelIF> m_rankTxt;
    CCSafeObject<CCLabelIF> m_kingdomTxt;
    CCSafeObject<CCLabelIF> m_memberTxt;
    CCSafeObject<CCLabelIF> m_winTxt;
    
    CCSafeObject<CCNode> m_clickArea;
    
};
#endif /* defined(__IF__AgainstInfoView__) */
