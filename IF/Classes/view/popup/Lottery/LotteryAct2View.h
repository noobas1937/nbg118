//
//  LotteryAct2View.h
//  IF
//
//  Created by chenfubi on 15/1/23.
//
//

#ifndef __IF__LotteryAct2View__
#define __IF__LotteryAct2View__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "WaitInterface.h"

class LotteryAct2View : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
//, public CCTouchDelegate
{
public:
    static LotteryAct2View* create();
	virtual bool init();
    LotteryAct2View():m_firstOpen(false),m_aniIndex(-1){};
    virtual ~LotteryAct2View(){};
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onQuitSuperModeClick(CCObject* pSender, CCControlEvent event);
    void onAskClick(CCObject* pSender, CCControlEvent event);
    void onPlayClick(CCObject* pSender, CCControlEvent event);
    void onViewClick(CCObject* pSender, CCControlEvent event);
    void playAnimation(float time);
    void openAniCom();
    void guideAniCom();
    void animationComplete();
    void closeAniCom();
    void notifyCallBack(CCObject* pObj);
    void initListNode();
    void initAniNode(int type = 0);
    string getIcon(string itemId);
    string getColor(string itemId);
    void initDiamondTTF();
    void removeParticle(float time);
    void yesDialogBack();
    void displayDesNode(CCObject* pObj);
    void setDesNode(CCObject* pObj, bool desAniInfo = false);
    int hasTouchNode(cocos2d::CCTouch *pTouch);
    void guideBack(CCObject* pObj);
    void doListAniAct(CCObject* pObj);
    void playComplete(CCNode* pNode, void* pObj);
    void playParticle(CCObject* pObj);
    void removeParticle2(float dt);
    
private:
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCNode> m_aniListNode;
    CCSafeObject<CCNode> m_superNode;
    CCSafeObject<CCNode> m_particleNode;
    CCSafeObject<CCNode> m_midNode;
    CCSafeObject<CCNode> m_desNode;
    CCSafeObject<CCLabelIF> m_desName;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCLabelIF> m_infoTTF;
    CCSafeObject<CCLabelIF> m_infoTTF1;
    CCSafeObject<CCControlButton> m_askBtn;
    CCSafeObject<CCControlButton> m_quitBtn;
    CCSafeObject<CCControlButton> m_playBtn;
    CCSafeObject<CCControlButton> m_viewBtn;
    CCSafeObject<CCSprite> m_bg0;
    CCSafeObject<CCSprite> m_bg1;
    CCSafeObject<CCScale9Sprite> m_desBg;
    CCSafeObject<CCNode> m_guideNode0;
    CCSafeObject<CCLabelIF> m_guideTTF0;
    CCSafeObject<CCNode> m_guideNode2;
    CCSafeObject<CCNode> m_insideGuide;
    CCSafeObject<CCLabelIF> m_guideTTF;
    CCSafeObject<CCSprite> m_guideArrow;
    CCSafeObject<CCSprite> m_guideSp1;
    CCSafeObject<CCSprite> m_guideSp0;
    CCSafeObject<CCScale9Sprite> m_guideBg;
    CCSafeObject<CCNode> m_listAniNode;
    CCSafeObject<CCNode> m_particleNode2;
    
    bool m_firstOpen;
    int m_aniIndex;

};

//LotteryAct2Cell class
class LotteryAct2Cell :
public CCBSelectorResolver
,public CCAniNode
,public CCBMemberVariableAssigner
//,public CCTouchDelegate
{
public:
    static LotteryAct2Cell* create(string rewardId, int dataId, string rewardCnt);
    LotteryAct2Cell():m_waitInterface(nullptr){};
    virtual ~LotteryAct2Cell(){};
    
    void setData(string rewardId, string rewardCnt);
    void setLightEffect();
    
private:
    bool init(string rewardId, int dataId, string rewardCnt);
    
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onTouchClick();
    void cmdCallBack(CCObject* pObj);
    void sendCmd(int useGold);
    void yesDialogBack(CCObject* pObj);
    void yesDialogBack2();
    string getIcon();
    string getColor();
    void aniComplete();
    void doRewardAni();
    void setDesNode();
    void onGameTick(float dt);
    void setMultiEffect(CCObject* pObj);
    
private:
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_itemNode;
    CCSafeObject<CCNode> m_particleNode;
    CCSafeObject<CCNode> m_particleNode2;
    CCSafeObject<CCSprite> m_bg;
    CCSafeObject<CCSprite> m_icon;
    CCSafeObject<CCSprite> m_iconBg;
    CCSafeObject<CCSprite> m_lotteryRatingSp;
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<CCSprite> m_numBg;
    CCSafeObject<CCSprite> m_lightBg;

    CCSafeObject<CCDictionary> m_rewardDic;
    
    WaitInterface* m_waitInterface;
    int m_dataId;
    string m_rewardId;
    string m_rewardCnt;
};

#endif /* defined(__IF__LotteryAct2View__) */
