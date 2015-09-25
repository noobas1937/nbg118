//
//  PortActView.h
//  IF
//
//  Created by xxrdsg on 14-12-22.
//
//

#ifndef __IF__PortActView__
#define __IF__PortActView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "TimeExchangeView.h"
#include "DailyRwdView.h"

class PortActView: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static PortActView* create(int type=0,int panelType = 0);
    PortActView(int type,int panelType):m_type(type),m_panelType(panelType){};
    virtual ~PortActView();
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickTimeRwdBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickDailyRwdBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void setShowView();
    
    void retData(CCObject* param);
    void requestData();
    void refreshData();
    void addBG();
    
    CCSafeObject<CCNode> m_bgContainer;
    
    CCSafeObject<CCNode> m_mainNode;
    
    CCSafeObject<CCControlButton> m_timeRwdBtn;
    CCSafeObject<CCControlButton> m_dailyRwdBtn;
    CCSafeObject<CCNode> m_actNode;
    
    
    CCSafeObject<TimeExchangeView> m_TimeExchangeView;
    CCSafeObject<DailyRwdView> m_dailyRwdView;
    
    WaitInterface* m_waitInterface;
    int m_type;
    int m_panelType;
};

#endif /* defined(__IF__PortActView__) */
