//
//  IFShakeGuideLayer.h
//  IF
//
//  Created by wangdianzhen on 15/8/19.
//
//

#ifndef __IF__IFShakeGuideLayer__
#define __IF__IFShakeGuideLayer__

#include "CommonInclude.h"
#include "CCLabelIF.h"
#include "ParticleController.h"
#include "CCAniNode.h"
#include "CCClipNode.h"
#include "HFHeadImgNode.h"
class IFShakeGuideLayer:public  CCAniNode
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    IFShakeGuideLayer(){};
    ~IFShakeGuideLayer(){};
    bool initData();
    static IFShakeGuideLayer* create();
    bool isPicBack();
private:
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);

    CCSafeObject<CCNode> m_head;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    CCSafeObject<CCLabelIF> m_hintText;
    void removeOneself(float t);
    void onGameTick(float time);
    void monitorPic(float time);
    CCSafeObject<CCClipNode> m_clipNode;
    int m_defaultWidth;
    int m_startTime;
    
};

#endif /* defined(__IF__IFShakeGuideLayer__) */
