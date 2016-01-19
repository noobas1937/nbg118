//
//  SingleScoreWaiteView.hpp
//  IF
//
//  Created by 付彬 on 15/11/19.
//
//

#ifndef SingleScoreWaiteView_hpp
#define SingleScoreWaiteView_hpp

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "ActivityEventObj.h"

class SingleScoreWaiteView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static SingleScoreWaiteView *create(ActivityEventObj* info);
    SingleScoreWaiteView(ActivityEventObj* info):m_info(info){};
    ~SingleScoreWaiteView(){};
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onCloseClick(CCObject *pSender, CCControlEvent event);
    void onInfoClick(CCObject *pSender, CCControlEvent event);
    void updateTime(float _time);
    
    CCSafeObject<CCControlButton> m_btnClose;
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCLabelIF> m_readyTime;
    CCSafeObject<CCLabelIF> m_readyTip;
    CCSafeObject<CCNode> m_clickArea;
    ActivityEventObj* m_info;
    int m_current;
    int m_maxSoldier;
    int m_gold;
    int m_qid;
};

#endif /* SingleScoreWaiteView_hpp */
