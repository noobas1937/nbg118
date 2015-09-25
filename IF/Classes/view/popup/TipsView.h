//
//  TipsView.h
//  IF
//
//  Created by 李锐奇 on 13-11-4.
//
//

#ifndef __IF__TipsView__
#define __IF__TipsView__

#include "CCBExtension.h"
#include "PopupBaseView.h"

class TipsView : public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static TipsView* create(string msg,CCTextAlignment align=kCCTextAlignmentCenter);
    TipsView(){};
    virtual ~TipsView(){};
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(string msg,CCTextAlignment align);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickCloseBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
//    CCSafeObject<CCControlButton> m_btnOk;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCLabelIF> m_msgLabel;
    CCTextAlignment m_align;
};
class TipsVipView : public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static TipsVipView* create(string title,string msg);
    TipsVipView(){};
    virtual ~TipsVipView(){};
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(string title,string msg);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickCloseBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCControlButton> m_btnOk;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCLabelIF> m_msgLabel;
    CCSafeObject<CCLabelIF> m_msgLabelTitle;
    CCSafeObject<CCNode> m_upNode;
};
//
class TipsGuideView : public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static TipsGuideView* create(string msg,float dy=0,CCTextAlignment align=kCCTextAlignmentCenter);
    TipsGuideView(){};
    virtual ~TipsGuideView(){};
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(string msg,float dy,CCTextAlignment align);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickCloseBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCNode> m_totalNode;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCLabelIF> m_msgLabel;
    CCTextAlignment m_align;
};

//
class TipsNewView : public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static TipsNewView* create(string title, string msg,CCTextAlignment align=kCCTextAlignmentCenter, float titleFontSize = 0.0f, float infoFontSize = 0.0f);
    TipsNewView(){};
    virtual ~TipsNewView(){};
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(string title, string msg,CCTextAlignment align, float titleFontSize, float infoFontSize);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickCloseBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCScale9Sprite> m_buildBG2;
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelIF> m_msgLabel;
//    CCTextAlignment m_tAlign;
//    CCTextAlignment m_mAlign;
};
#endif /* defined(__IF__TipsView__) */
