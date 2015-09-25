//
//  AllianceApplyMessageView.h
//  IF
//
//  Created by 〜TIAN~ on 15/8/6.
//
//

#ifndef __IF__AllianceApplyMessageView__
#define __IF__AllianceApplyMessageView__

#include "CommonInclude.h"
#include "PopupBaseView.h"

class AllianceApplyMessageView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    ~AllianceApplyMessageView(){};
    AllianceApplyMessageView(std::string content):m_content(content){};
    static AllianceApplyMessageView *create(std::string content);
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
    
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCLabelIF> m_messageLabel;
    std::string m_content;
};


#endif /* defined(__IF__AllianceApplyMessageView__) */
