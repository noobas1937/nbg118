//
//  AllianceActivityView.h
//  IF
//
//  Created by ganxiaohua on 15/2/3.
//
//

#ifndef __IF__AllianceActivityView__
#define __IF__AllianceActivityView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "ActivityEventObj.h"

class AllianceActivityView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static AllianceActivityView *create(ActivityEventObj* info);
    
private:
    AllianceActivityView(ActivityEventObj* info):m_info(info){};
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
    void onCheckClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent);
    void onCheckReward(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent);
    void onBattleClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent);
    void onBattleDetail(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent);
    void battleCMDCallBack(cocos2d::CCObject * param);
    void getInfoCallBack(cocos2d::CCObject * param);
    void scheduleHandle(float _time);
    void goToWorld();
    void freshView(float _time=0.0);
    void delayCall(float _time=0.0);
    void confirmFun();
    
    CCSafeObject<CCNode> m_clickArea;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCNode> m_readyNode;
    CCSafeObject<CCControlButton> m_btnCheck;
    CCSafeObject<CCLabelIF> m_readyTime;
    CCSafeObject<CCLabelIF> m_readyTip;
    CCSafeObject<CCControlButton> m_btnClose;
    
    CCSafeObject<CCNode> m_startNode;
    CCSafeObject<CCControlButton> m_btnReward;
    CCSafeObject<CCControlButton> m_btnBattle;
    CCSafeObject<CCLabelIF> m_startTxt;
    CCSafeObject<CCLabelIF> m_startTip;
    
    CCSafeObject<CCNode> m_detailNode;
    CCSafeObject<CCControlButton> m_bCheckReward;
    CCSafeObject<CCControlButton> m_bDetail;
    CCSafeObject<CCLabelIF> m_bTxt1;
    CCSafeObject<CCLabelIF> m_bTxt2;
    CCSafeObject<CCLabelIF> m_bTxt3;
    CCSafeObject<CCLabelIF> m_bTxt4;
    CCSafeObject<CCLabelIF> m_bTxt5;
    CCSafeObject<CCNode> m_sNode;
    CCSafeObject<CCNode> m_coordinateNode;
    CCSafeObject<CCNode> m_loadingNode;
    CCSafeObject<CCControlButton> m_infoBtn;
    
    CCSafeObject<WaitInterface> m_waitInterface;
    int m_type;
    double m_gapTime;
    ActivityEventObj* m_info;
    std::map<std::string, int> monsterMap;
    int m_callCount;
    unsigned long currentTime;
    int m_state;
};
#endif /* defined(__IF__AllianceActivityView__) */
