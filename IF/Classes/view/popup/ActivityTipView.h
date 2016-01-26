//
//  ActivityTipView.h
//  IF
//
//  Created by ganxiaohua on 14-9-2.
//
//

#ifndef __IF__ActivityTipView__
#define __IF__ActivityTipView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "PopupViewController.h"
#include "ActivityEventObj.h"

class ActivityTipView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
{
public:
    static ActivityTipView *create(ActivityEventObj* info);
    ActivityTipView(ActivityEventObj* info):m_info(info){};
    ~ActivityTipView(){};
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
    void updateTime(float _time);
    void initReward();
    
    CCSafeObject<CCLabelIF> m_txt1;
    CCSafeObject<CCLabelIF> m_txt2;
    CCSafeObject<CCLabelIF> m_txt3;
    CCSafeObject<CCLabelIF> m_txt4;
    CCSafeObject<CCLabelIF> m_txt5;
    CCSafeObject<CCNode> m_clickArea;
//    CCSafeObject<CCNode> m_fireNode1;
//    CCSafeObject<CCNode> m_fireNode2;
    
    CCSafeObject<CCNode> m_rewardIcon1;
    CCSafeObject<CCLabelIF> m_rewardTxt1;
    CCSafeObject<CCNode> m_rewardIcon2;
    CCSafeObject<CCLabelIF> m_rewardTxt2;
    CCSafeObject<CCNode> m_rewardIcon3;
    CCSafeObject<CCLabelIF> m_rewardTxt3;
    CCSafeObject<CCNode> m_rewardIcon4;
    CCSafeObject<CCLabelIF> m_rewardTxt4;
    
    CCSafeObject<CCLabelIF> m_numTxt1;
    CCSafeObject<CCLabelIF> m_numTxt2;
    CCSafeObject<CCLabelIF> m_numTxt3;
    
    ActivityEventObj* m_info;
    double m_haveTime;
};
#endif /* defined(__IF__ActivityTipView__) */
