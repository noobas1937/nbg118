//
//  MoveBuildView.h
//  IF
//
//  Created by fubin on 15/2/5.
//
//

#ifndef __IF__MoveBuildView__
#define __IF__MoveBuildView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"

class MoveBuildView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
 
{
public:
    static MoveBuildView* create();
    MoveBuildView(){};
    ~MoveBuildView(){};
    
private:
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onOKClick(CCObject *pSender, CCControlEvent event);
    
    CCSafeObject<CCLabelIF> m_infoLabel;
    CCSafeObject<CCControlButton> m_okBtn;
    CCSafeObject<CCNode> m_touchNode;
    
    CCSafeObject<CCNode> m_goldNode;
    CCSafeObject<CCLabelIF> m_btnMsgLabel;
    CCSafeObject<CCLabelIF> m_inBtnGoldNum;
    
    CCSafeObject<CCNode> m_toolNode;
    CCSafeObject<CCLabelIF> m_toolMsgLabel;
    CCSafeObject<CCNode> m_iconNode;
    
    int m_gold;
};

#endif /* defined(__IF__MoveBuildView__) */
