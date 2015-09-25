//
//  CityDefenseView.h
//  IF
//
//  Created by fubin on 14-8-13.
//
//

#ifndef IF_CityDefenseView_h
#define IF_CityDefenseView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"

class CityDefenseView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static CityDefenseView* create(int buildId);
    CityDefenseView():m_buildId(0),m_isInit(false),m_st(0){};
    virtual ~CityDefenseView(){};
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(int buildId);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickAddDefense(CCObject * pSender, Control::EventType pCCControlEvent);
    void onHelpClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onFuFeiClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void onEnterFrame(float dt);
    void addParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    
    void updateInfo();
    void onUpdateDefenseCallback(cocos2d::CCObject *obj);
    void changeST(int type);
    void fuFeiHandle();
    void sendSuccess(CCObject* data);
    
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCNode> m_bNode;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCNode> m_fireNode1;
    CCSafeObject<CCNode> m_fireNode2;
    
    CCSafeObject<CCControlButton> m_defBtn;
    CCSafeObject<CCLabelIF> m_defLabel;
    CCSafeObject<CCLabelIF> m_defMsgLabel;
    CCSafeObject<CCLabelIF> m_fireLabel;
    CCSafeObject<CCScale9Sprite> m_defbar;
    CCSafeObject<CCScale9Sprite> m_fullbar;
    CCSafeObject<CCLabelIF> m_defbarLabel;
    CCSafeObject<CCNode> m_middleNode;
    CCSafeObject<CCControlButton> m_fufeiBtn;
    CCSafeObject<CCLabelIF> m_statusTxt;
    CCSafeObject<CCLabelIF> m_statusDesc;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCLabelIF> m_timeDesc;
    CCSafeObject<CCLabelIF> m_fuFeiBtnTxt;
    CCSafeObject<CCLabelIF> m_fuFeiValue;
    CCSafeObject<CCLabelIF> m_fullTip;
    
    CCSafeObject<CCNode> m_smokeNode;
    vector<CCParticleBatchNode*> m_parVec;
    
    int m_buildId;
    bool m_isInit;
    int m_st;
    int m_cityDefenseVal;
};

#endif
