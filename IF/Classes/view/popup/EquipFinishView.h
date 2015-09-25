//
//  EquipFinishView.h
//  IF
//
//  Created by fubin on 15-1-26.
//
//

#ifndef __IF__EquipFinishView__
#define __IF__EquipFinishView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCMultiColTableView.h"

class EquipFinishView: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
//, public CCTouchDelegate
{
public:
    static EquipFinishView* create(int itemId);
    void setCurPopType(int type);
protected:
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(int itemId);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onClickGoToBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void onRefreshEquipInfo(int color);
    void addPopParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    void showPopParticle(float dt);
    void removePopParticle(float dt);
    void playBombEff();
    void playParticle();
    void animationFunc();
    
    CCSafeObject<CCControlButton> m_gotoBtn;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelIF> m_msgLabel;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_msg1Label;
    CCSafeObject<CCLabelIF> m_msg2Label;
    CCSafeObject<CCLabelIF> m_msg3Label;
    CCSafeObject<CCLabelIF> m_msg4Label;
    
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_popNode;
    
    CCSafeObject<CCLayer> m_popLayer;
    CCSafeObject<CCNode> m_playNode;
    vector<CCParticleBatchNode*> m_parPopVec;
    
    CCSafeObject<CCSprite> m_checkBox;
    CCSafeObject<CCLabelIF> m_shareLab;
    CCSafeObject<CCNode> m_finishShareNode;
    
    int m_itemId;
    int m_weapType;
    int m_curPopType;
    bool m_initEnd;
    bool m_isShare;
};

#endif /* defined(__IF__EquipFinishView__) */
