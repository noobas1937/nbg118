//
//  FacebookLikeView.h
//  IF
//
//  Created by xingxiaorui on 14-11-10.
//
//

#ifndef __IF__FacebookLikeView__
#define __IF__FacebookLikeView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"

class FacebookLikeView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
  
{
public:
    static FacebookLikeView* create();
    FacebookLikeView(){};
    ~FacebookLikeView(){};
    
private:
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onGoClick(CCObject *pSender, CCControlEvent event);
    
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelIF> m_infoLabel;
    CCSafeObject<CCControlButton> m_goBtn;
    CCSafeObject<CCNode> m_touchNode;
};


#endif /* defined(__IF__FacebookLikeView__) */
