//
//  AllianceActivityRewardView.h
//  IF
//
//  Created by ganxiaohua on 15/3/2.
//
//

#ifndef __IF__AllianceActivityRewardView__
#define __IF__AllianceActivityRewardView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "ActivityEventObj.h"

class AllianceActivityRewardView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
{
public:
    static AllianceActivityRewardView *create();
    
private:
    AllianceActivityRewardView(){};
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    void onCloseClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent);
    void onInfoClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent);
    void showRewardByTab(int tab);
    void scheduleHandle(float _time);
    void callBack(CCObject* param);
    
    CCSafeObject<CCNode> m_clickArea;
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCControlButton> m_btnClose;
    CCSafeObject<CCNode> m_tab1;
    CCSafeObject<CCNode> m_tab2;
    CCSafeObject<CCNode> m_tab3;
    CCSafeObject<CCLabelIF> m_tab1Txt1;
    CCSafeObject<CCLabelIF> m_tab2Txt1;
    CCSafeObject<CCLabelIF> m_tab3Txt1;
    CCSafeObject<CCLabelIF> m_tipTxt;
    
    CCSafeObject<CCNode> m_tabNode1;
    CCSafeObject<CCNode> m_tabRewardNode1;
    CCSafeObject<CCLabelIF> m_tipTxt1;
    CCSafeObject<CCLabelIF> m_tipTxt2;
    CCSafeObject<CCLabelIF> m_tipTxt3;
    CCSafeObject<CCLabelIF> m_tipTxt4;
    CCSafeObject<CCNode> m_scNode;
    CCSafeObject<CCControlButton> m_infoBtn;
    CCSafeObject<CCNode> m_selectBg;
    
    CCSafeObject<CCNode> m_btnNode;
    CCSafeObject<CCControlButton> m_rewardBtn;
    CCSafeObject<CCControlButton> m_playerBtn;
    CCSafeObject<CCControlButton> m_allianceBtn;
    
    CCSafeObject<CCScrollView> m_scrollView;
    double m_haveTime;
    int m_tap;
};

class AllianceActivityRewardCell : public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static AllianceActivityRewardCell *create(CCDictionary* info);
private:
    AllianceActivityRewardCell(CCDictionary* info):m_info(info){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_numTxt;
    CCSafeObject<CCNode> m_iconNode;
    CCDictionary* m_info;
};

#endif /* defined(__IF__AllianceActivityRewardView__) */
