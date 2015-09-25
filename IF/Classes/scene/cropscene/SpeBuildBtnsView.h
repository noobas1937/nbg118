//
//  SpeBuildBtnsView.h
//  IF
//
//  Created by fubin on 15/3/11.
//
//

#ifndef __IF__SpeBuildBtnsView__
#define __IF__SpeBuildBtnsView__

#include "CommonInclude.h"
#include "CCLabelIF.h"
#include "FunBuildInfo.h"
#include "ParticleController.h"
#include "Person.h"
#include "CCLabelBatch.h"
#include "CCAniNode.h"
#include "ToolInfo.h"

class SpeBuildBtnsView: public CCAniNode
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
{
public:
    SpeBuildBtnsView():m_buildingKey(0),m_initEnd(false),m_btnNum(2){};
    ~SpeBuildBtnsView(){};
    bool init();
    static SpeBuildBtnsView* create();
    
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual void onEnter();
    virtual void onExit();
    void onHide();
    void onShow(int buildId);
    CCSafeObject<CCNode> m_toolNode;
    int m_buildingKey;
protected:
    
    void onBtn1Click(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBtn2Click(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBtn3Click(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBtn4Click(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBtn5Click(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickFunc(int idx);
    
    bool onShowInfo();
    void onHideBtn();
    void onHideShadow(bool st=true);
    void onEnterFrame(float dt);
    void animationFunc();
    
    CCSafeObject<CCLabelIF> m_goldLabel;
    
    CCSafeObject<CCNode> m_spdNode;
    
    CCSafeObject<CCNode> m_btn1Node;
    CCSafeObject<CCNode> m_btn2Node;
    CCSafeObject<CCNode> m_btn3Node;
    CCSafeObject<CCNode> m_btn4Node;
    CCSafeObject<CCNode> m_btn5Node;
    
    CCSafeObject<CCNode> m_icon1Node;
    CCSafeObject<CCNode> m_icon2Node;
    CCSafeObject<CCNode> m_icon3Node;
    CCSafeObject<CCNode> m_icon4Node;
    CCSafeObject<CCNode> m_icon5Node;
    
    CCSafeObject<CCControlButton> m_btn1;
    CCSafeObject<CCControlButton> m_btn2;
    CCSafeObject<CCControlButton> m_btn3;
    CCSafeObject<CCControlButton> m_btn4;
    CCSafeObject<CCControlButton> m_btn5;
    
    CCSafeObject<CCLabelIF> m_name1;
    CCSafeObject<CCLabelIF> m_name2;
    CCSafeObject<CCLabelIF> m_name3;
    CCSafeObject<CCLabelIF> m_name4;
    CCSafeObject<CCLabelIF> m_name5;
    
    CCSafeObject<CCLabelIF> m_msg5;
    CCSafeObject<CCLabelIF> m_msgV5;
    
    CCSafeObject<CCNode> m_nameNode;
    CCSafeObject<CCLabelIF> m_nameLabel;
    
    //increated
    CCSafeObject<CCNode> m_touchNode;
    
    bool m_initEnd;
    int m_btnNum;
    vector<string> m_funKeyVec;
private:
    
};

#endif /* defined(__IF__SpeBuildBtnsView__) */
