//
//  CityCrashView.h
//  IF
//
//  Created by 邹 程 on 14-3-12.
//
//

#ifndef __IF__CityCrashView__
#define __IF__CityCrashView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"

class CityCrashView :
public Layer
//public PopupBaseView
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
  
{
public:
    
    CREATE_FUNC(CityCrashView)
    
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    
    bool init();
    
    void onRebuildCallback(CCObject* obj);
    void onClickButtonRebuild(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void onEnter();
    void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
private:
    CCSafeObject<CCControlButton> m_buttonRebuild;
    CCSafeObject<CCLabelIF> m_info;
    
};


#endif /* defined(__IF__CityCrashView__) */
