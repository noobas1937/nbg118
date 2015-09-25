//
//  HelpView.h
//  IF
//
//  Created by fubin on 14-4-8.
//
//

#ifndef IF_HelpView_h
#define IF_HelpView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCWebView.h"

class HelpView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static HelpView* create();
    HelpView(){};
    virtual ~HelpView(){};
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCNode> m_infoList;
    
    CCSafeObject<CCWebView> m_webView;
};

#endif
