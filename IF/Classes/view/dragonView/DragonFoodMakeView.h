//
//  DragonFoodMakeView.h
//  IF
//
//  Created by 付彬 on 15/6/5.
//
//

#ifndef __IF__DragonFoodMakeView__
#define __IF__DragonFoodMakeView__

//#include "CommonInclude.h"
//#include "PopupBaseView.h"
//#include "DefaultTableViewDelegate.h"
//#include "CCClipNode.h"

//class DragonFoodMakeView: public PopupBaseView
//, public CCBSelectorResolver
//, public CCBMemberVariableAssigner
//{
//public:
//    static DragonFoodMakeView* create();
//    DragonFoodMakeView(){};
//    virtual ~DragonFoodMakeView(){};
//    
//private:
//    virtual void onEnter();
//    virtual void onExit();
//    virtual bool init();
//    
//    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
//    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
//    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
//    
//    void onOKClick(cocos2d::CCObject * pSender, CCControlEvent pCCControlEvent);
//    
//    CCSafeObject<CCNode> m_infoList;
//    CCSafeObject<CCScale9Sprite> m_buildBG;
//    CCSafeObject<CCControlButton> m_OKBtn;
//    CCSafeObject<CCTableView> m_tabView;
//};

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCMultiColTableView.h"
#include "ArcPopupBaseView.h"
#include "CCClipNode.h"

class DragonFoodMakeView: public ArcPopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static DragonFoodMakeView* create();
    void playClose();
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onShowWillMate(bool show);
    
    void onCostGoldBack();
    void AnimationFadeIn();
    void AnimationClose();
    void closeView(CCObject* params);
    void addParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    void flyEnd();
    
    void onOKCreateMate();
    
    void refreshFlyNode(int idx);
    void onEnterFrame(float dt);
    void updateCrtRet(CCObject* p);
    
    CCSafeObject<CCNode> m_particleNode;
    
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_makeNode;
    CCSafeObject<CCNode> m_make2Node;
    CCSafeObject<CCNode> m_curIconNode;
    CCSafeObject<CCLabelIF> m_lastTime;
    CCSafeObject<CCSprite> m_spdIcon;
    
    CCSafeObject<CCNode> m_tipNode;
    CCSafeObject<CCSprite> m_tipBg;
    CCSafeObject<CCLabelIF> m_tipName;
    CCSafeObject<CCLabelIF> m_tipTime;
    
    CCSafeObject<CCNode> m_flyNode;
    CCSafeObject<CCNode> m_tool1Node;
    CCSafeObject<CCNode> m_tool2Node;
    CCSafeObject<CCNode> m_tool3Node;
    CCSafeObject<CCNode> m_tool4Node;
    CCSafeObject<CCNode> m_tool5Node;
    CCSafeObject<CCNode> m_tool6Node;
    CCSafeObject<CCNode> m_tool7Node;
    CCSafeObject<CCNode> m_tool8Node;
    CCSafeObject<CCNode> m_tool9Node;
    CCSafeObject<CCNode> m_tool10Node;
    CCSafeObject<CCNode> m_tool11Node;
    CCSafeObject<CCNode> m_tool12Node;
    
    vector<CCNode*> m_toolNodeVec;
    vector<int> m_makeTools;
    
    int m_curOptIdx;
    
    int m_qid;
    int m_sumTime;
    int m_curTime;
    int _tmpGold;
    int m_sumGold;
    int m_oldSumTime;
    int _openNum;
    
    CCPoint startPt;
    int m_curCreatingItemId;
    string m_willIconPath;
    int m_buildId;
    WaitInterface* m_waitInterface;
};

#endif /* defined(__IF__DragonFoodMakeView__) */
