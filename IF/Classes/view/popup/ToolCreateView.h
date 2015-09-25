//
//  ToolCreateView.h
//  IF
//
//  Created by 付彬 on 15/5/6.
//
//

#ifndef __IF__ToolCreateView__
#define __IF__ToolCreateView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCMultiColTableView.h"
#include "ArcPopupBaseView.h"
#include "CCClipNode.h"

class ToolCreateView: public ArcPopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
//, public CCTouchDelegate
{
public:
    static ToolCreateView* create();
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
    
    void openCell();
    void showOpenCell(string queue_unlock);
    void onCostResBack();
    void refreshFlyNode(int idx);
    void onEnterFrame(float dt);
    void updateCrtRet(CCObject* p);
    void setViewByMate(CCObject* p);
    void updateWaitCells(string allId);
    void updateLockCell(CCObject* p);
    
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
    
    CCSafeObject<CCNode> m_buyTouchNode;
    CCSafeObject<CCSprite> m_buyBg;
    CCSafeObject<CCNode> m_buySprNode;
    CCSafeObject<CCLabelIF> m_price;
    CCSafeObject<CCControlButton> m_buyBtn;
    void onBuyBtnClick(CCObject *pSender, CCControlEvent event);
    int m_buyItemId;
    int m_buyPrice;
    
    void onBuy(CCObject *ccObj);
    void onRetBuy();
    
    //等待空位
    CCSafeObject<CCNode> m_queueNode;
    CCSafeObject<CCNode> m_waitNode[5];
    CCSafeObject<CCNode> m_waitInNode[5];
    CCSafeObject<CCControlButton> m_cancelBtn[5];
    void onCancelBtn1Click(CCObject *pSender, CCControlEvent event);
    void onCancelBtn2Click(CCObject *pSender, CCControlEvent event);
    void onCancelBtn3Click(CCObject *pSender, CCControlEvent event);
    void onCancelBtn4Click(CCObject *pSender, CCControlEvent event);
    void onCancelBtn5Click(CCObject *pSender, CCControlEvent event);
    CCSafeObject<CCControlButton> m_boxBtn;
    void onBoxBtnClick(CCObject *pSender, CCControlEvent event);
    bool m_canUse[5];
    
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
    int m_resType;
    int m_resCost;
    
    CCPoint startPt;
    int m_curCreatingItemId;
    string m_willIconPath;
    int m_buildId;
    WaitInterface* m_waitInterface;
};

#endif /* defined(__IF__ToolCreateView__) */
