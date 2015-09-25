//
//  CSAWorldBtn.h
//  IF
//
//  Created by ganxiaohua on 15/6/11.
//
//

#ifndef __IF__CSAWorldBtn__
#define __IF__CSAWorldBtn__

#include "CommonInclude.h"
#include "ActivityEventObj.h"
#include "CCAniNode.h"

class CSAWorldBtn :  public CCAniNode
 
, public CCBMemberVariableAssigner
{
public:
    static CSAWorldBtn *create();
private:
    CSAWorldBtn(){};
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    void updateTime(float _time);
    void updateState(int state);
    
    CCSafeObject<CCSprite> m_icon;
    CCSafeObject<CCNode> m_parNode;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCNode> m_pNode;
    CCSafeObject<CCLabelIF> m_pTipTxt;
    ActivityEventObj* m_actObj;
    int m_state;
    double m_startTime;
    double m_endTime;
};

#endif /* defined(__IF__CSAWorldBtn__) */
