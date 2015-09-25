//
//  NpcTalkView.h
//  IF
//
//  Created by fubin on 14-11-11.
//
//

#ifndef __IF__NpcTalkView__
#define __IF__NpcTalkView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"

class NpcTalkView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
   
{
public:
    static NpcTalkView* create(std::string dialogs);
private:
    NpcTalkView(std::string dialogs) : m_dialogs(dialogs){};
    
    bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onCloseClick(CCObject *pSender, CCControlEvent event);
    void refreshWord();
    void onClose();
    
    void onEnter();
    void onExit();
    
    bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    CCSafeObject<CCLabelIF> m_contentText;
    CCSafeObject<CCLabelIF> m_nameText;
    
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCNode> m_txtNode;
    CCSafeObject<CCNode> m_nextNode;
    
    std::string m_dialogs;
};

#endif /* defined(__IF__NpcTalkView__) */
