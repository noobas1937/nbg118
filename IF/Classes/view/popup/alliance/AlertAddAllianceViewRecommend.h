//
//  AlertAddAllianceView.h
//  IF
//
//  Created by ganxiaohua on 14-7-7.
//
//

#ifndef __IF__AlertAddAllianceViewRecommend__
#define __IF__AlertAddAllianceViewRecommend__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "QuestController.h"
#include "CCMultiColTableView.h"
#include "QuestInfo.h"
#include "AllianceInfo.h"

class AlertAddAllianceViewRecommend : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
{
public:
    ~AlertAddAllianceViewRecommend(){};
    AlertAddAllianceViewRecommend(AllianceInfo* info):m_info(info){};
    static AlertAddAllianceViewRecommend *create(AllianceInfo* info);
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
    
    void onClickClose(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCControlButton> m_btnJoin;
    CCSafeObject<CCNode> m_clickNode;
    CCSafeObject<CCLabelIF> m_txt1;
    CCSafeObject<CCLabelIF> m_txt2;
    CCSafeObject<CCLabelIF> m_txt3;
    
    //begin a by ljf
    CCSafeObject<CCLabelIF> m_title;
    CCSafeObject<CCLabelIF> m_gemNum;
    CCSafeObject<CCLabelIF> m_allianceName;
    CCSafeObject<CCLabelIF> m_language;
    CCSafeObject<CCLabelIF> m_allianceLeader;
    CCSafeObject<CCLabelIF> m_memberNum;
    CCSafeObject<CCLabelIF> m_battleNum;
    CCSafeObject<CCSprite> m_allianceIcon;
    CCSafeObject<AllianceInfo> m_info;
    //end a by ljf
};
#endif /* defined(__IF__AlertAddAllianceViewRecommend__) */
