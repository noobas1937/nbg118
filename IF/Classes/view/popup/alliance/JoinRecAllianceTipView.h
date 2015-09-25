//
//  JoinRecAllianceTipView.h
//  IF
//
//  Created by 付彬 on 15/8/13.
//
//

#ifndef __IF__JoinRecAllianceTipView__
#define __IF__JoinRecAllianceTipView__


#include "CommonInclude.h"
#include "CCLabelIF.h"
#include "ParticleController.h"
#include "CCAniNode.h"
#include "HFHeadImgNode.h"

class JoinRecAllianceTipView: public CCAniNode
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    JoinRecAllianceTipView(){};
    ~JoinRecAllianceTipView(){};
    bool initJoinRecAllianceTipView();
    static JoinRecAllianceTipView* create();
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void onJoinBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onCloseBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    void removeSelf(float dt=0);
    void updateHead(CCObject* obj);
    
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCControlButton> m_joinBtn;
    CCSafeObject<CCControlButton> m_closeBtn;
    
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_infoLabel;
    CCSafeObject<CCLabelIF> m_allianceLabel;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    AllianceInfo* m_info;
private:
};

#endif /* defined(__IF__JoinRecAllianceTipView__) */
