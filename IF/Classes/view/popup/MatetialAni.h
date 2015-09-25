//
//  MatetialAni.h
//  IF
//
//  Created by 李朝辉 on 15-7-31.
//
//

#ifndef __IF__MatetialAni__
#define __IF__MatetialAni__

#include "CommonInclude.h"
#include "CCAniNode.h"

class MatetialAni:public CCAniNode
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    MatetialAni(){};
    ~MatetialAni(){};
    bool init();
    static MatetialAni* create();
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCSprite> m_alliance;
    
private:
    
};


#endif /* defined(__IF__MatetialAni__) */
