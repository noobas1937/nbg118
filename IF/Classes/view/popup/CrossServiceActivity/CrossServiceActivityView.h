//
//  CrossServiceActivityView.h
//  IF
//
//  Created by ganxiaohua on 15/6/4.
//
//

#ifndef __IF__CrossServiceActivityView__
#define __IF__CrossServiceActivityView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "ActivityEventObj.h"

class CrossServiceActivityView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
{
public:
    static CrossServiceActivityView *create(ActivityEventObj* info);
    CrossServiceActivityView(ActivityEventObj* info):m_info(info){};
    ~CrossServiceActivityView(){};
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onClickExitBtn(CCObject *pSender, CCControlEvent event);
    void onCloseClick(CCObject *pSender, CCControlEvent event);
    void onClickBtn1(CCObject *pSender, CCControlEvent event);
    void onInfoClick(CCObject *pSender, CCControlEvent event);
    void getData(CCObject *param);
    void updateTime(float _time);
    void leaveCKF();
    void enterCKF();
    void healSoldier();
    void onCostGoldBack();
    
    CCSafeObject<CCControlButton> m_btnClose;
    CCSafeObject<CCControlButton> m_btn1;
    CCSafeObject<CCControlButton> m_exitBtn;
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCLabelIF> m_readyTime;
    CCSafeObject<CCLabelIF> m_readyTip;
    CCSafeObject<CCLabelIF> m_tipTxt;
    CCSafeObject<CCLabelIF> m_numTxt;
    CCSafeObject<CCLabelIF> m_txt1;
    CCSafeObject<CCLabelIF> m_txt2;
    CCSafeObject<CCLabelIF> m_txt3;
    CCSafeObject<CCNode> m_icon;
    CCSafeObject<CCNode> m_requireNode;
    CCSafeObject<CCNode> m_clickArea;
    CCSafeObject<CCNode> m_infoNode;
    CCSafeObject<WaitInterface> m_waitInterface;
    ActivityEventObj* m_info;
    int m_current;
    int m_maxSoldier;
    int m_gold;
    int m_qid;
};

#endif /* defined(__IF__CrossServiceActivityView__) */
