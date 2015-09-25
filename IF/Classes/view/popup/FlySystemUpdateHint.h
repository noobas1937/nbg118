//
//  FlySystemUpdateHint.h
//  IF
//
//  Created by liweiyu on 14-9-15.
//
//

#ifndef __IF__FlySystemUpdateHint__
#define __IF__FlySystemUpdateHint__

#include "CommonInclude.h"
#include "FlyRollingText.h"
#include "CCAniNode.h"

class FlySystemUpdateHint : public CCAniNode
 
, public CCBMemberVariableAssigner
{
public:
    static FlySystemUpdateHint *create(double countDown, bool isLogin,string tip,FlyHintType type,string icon);
    void show();
    float getRollTime();
private:
    FlySystemUpdateHint(double countDown, bool isLogin,string tip,FlyHintType type,string icon) : m_countDown(countDown), m_isLogin(isLogin),m_tip(tip),m_type(type),m_icon(icon){};

    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    void removeSelf(CCObject* obj);
    void rollText();
    void hideUIQuestNode();
    void showUIQuestNode();
    void loop(float _time);
    
    FlyRollingText* m_rollText;
    long m_time;
    double m_countDown;
    float m_rollTime;
    bool m_isLogin;
    
    CCSafeObject<CCAction> one;
    CCSize m_textSize;
    CCSafeObject<CCLabelIFTTF> m_contentText;
    CCSafeObject<CCNode> m_container;
    CCSafeObject<CCNode> m_clickArea;
    string m_tip;
    FlyHintType m_type;
    string m_icon;
    void showInNativeUI(float time);
};

#endif /* defined(__IF__FlySystemUpdateHint__) */
