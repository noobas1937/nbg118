//
//  FunBuildBtnsView.h
//  IF
//
//  Created by fubin on 14-7-4.
//
//

#ifndef IF_FunBuildBtnsView_h
#define IF_FunBuildBtnsView_h

#include "CommonInclude.h"
#include "CCLabelIF.h"
#include "FunBuildInfo.h"
#include "ParticleController.h"
#include "Person.h"
#include "CCLabelBatch.h"
#include "CCAniNode.h"
#include "ToolInfo.h"

class FunBuildBtnsView: public CCAniNode
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
 
{
public:
    FunBuildBtnsView():m_info(NULL),m_toolInfoId(0),m_buildingKey(0),m_gold(0),buildType(0),m_qid(QID_MAX),lastTime(0),guidKey(""),m_initEnd(false),m_btnNum(2){};
    ~FunBuildBtnsView(){};
    bool init();
    static FunBuildBtnsView* create();
    
    CCNode* getGuideNode(string _key);
    
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual void onEnter();
    virtual void onExit();
    void onHide();
    void onShow(int buildId, int questType = -1);
    CCSafeObject<CCNode> m_toolNode;
protected:
    FunBuildInfo* m_info;
    int m_toolInfoId;
    int m_buildingKey;
    int m_gold;
    int buildType;
    int m_qid;
    int lastTime;
    string guidKey;
    int m_questType;
    void onBtn1Click(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBtn2Click(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBtn3Click(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBtn4Click(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBtn5Click(CCObject * pSender, Control::EventType pCCControlEvent);
    
    bool onShowInfo();
    void onHideBtn();
    void onHideShadow(bool st=true);
    void showColloctSoldier(CCObject* params);
    void onEnterFrame(float dt);
    void onCostGoldBack();
    void onGuidClick();
    void animationFunc();
    void yesUseIncreated();
    
    bool onUpdateToolBtn(int type);
    int getSPDTypeByBuildingState();
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
    CCSafeObject<CCSprite> m_nameBgSpr;
    
    //increated
    CCSafeObject<CCNode> m_IncreasedNode;
    CCSafeObject<CCSprite> m_IncreasedGold;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCLabelIF> m_IncreasedGlodTxt;
    CCSafeObject<CCLabelIF> m_IncreasedTime;
    
    bool m_initEnd;
    int m_btnNum;
private:
    
};
class FunBuildBtnsEffect : public CCAniNode
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static FunBuildBtnsEffect *create();
private:
    FunBuildBtnsEffect(){};
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
};
#endif
