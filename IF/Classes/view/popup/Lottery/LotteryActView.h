//
//  LotteryActView.h
//  IF
//
//  Created by chenfubi on 15/1/13.
//
//

#ifndef __IF__LotteryActView__
#define __IF__LotteryActView__

#include "CommonInclude.h"
#include "PopupBaseView.h"


class LotteryActView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
////, public CCTouchDelegate
{
public:
    static LotteryActView* create(bool playAni = false);
	virtual bool init(bool playAni);
    LotteryActView():m_canTouch(true),m_aniComplete(true),m_touchTime(0),m_dataIndex(0),m_cmdComplete(true),m_rewardId(0),m_itemId(""),m_itemCnt(0),mIsGoods(false),m_waitInterface(NULL){};
    virtual ~LotteryActView(){};
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onSuperModeClick();
    
    void commandCallBack(CCObject* pObj);
    void initRewardMap();
    void playAnimation(CCObject* pObj=NULL);
    void updateAccount(CCObject* pObj);
    void doRewardAct();
    
    void addPopParticleToBatch(CCParticleSystemQuad* particle);
    void showPopParticle(CCObject* pObj);
    void removePopParticle(float dt);
    string getIcon(string itemId);
    void openComplete();
    void closeComplete();
    void initInfoTTF2();
    void rotateListener(CCObject* pObj);
    void onBuyBtnClick(CCObject* pSender, CCControlEvent event);
    bool isChipEnough();
    void buyChipBack(CCObject* pObj);
    void rewardAniBack(float dt);
    void onGameTick(float dt);
    
private:
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCNode> m_superNode;
    CCSafeObject<CCNode> m_popNode;
    CCSafeObject<CCNode> m_aniGuide;
    CCSafeObject<CCLabelIF> m_chipTTF2;
    CCSafeObject<CCLabelIF> m_infoTTF1;
    CCSafeObject<CCLabelIF> m_infoTTF2;
    CCSafeObject<CCLabelIF> m_infoTTF3;
    CCSafeObject<CCLabelIF> m_superTTF;
    CCSafeObject<CCSprite> m_superBg;
    CCSafeObject<CCSprite> m_superIcon;
    CCSafeObject<CCSprite> m_bg0;
    CCSafeObject<CCSprite> m_bg1;
    CCSafeObject<CCSprite> m_arrowSp;
    CCSafeObject<CCDictionary> m_rewardDic;
    CCSafeObject<CCLayer> m_popLayer;
    CCSafeObject<CCNode> m_aniNode;
    CCSafeObject<CCNode> m_particleNode;
    CCSafeObject<CCNode> m_lightNode;
    CCSafeObject<CCNode> m_buyNode;
    CCSafeObject<CCNode> m_fireNode;
    CCSafeObject<CCControlButton> m_buyBtn;
    CCSafeObject<CCLabelIF> m_goldTTF;
    CCSafeObject<CCLabelIF> m_goldNum;
    CCSafeObject<CCLayerColor> m_colorLayer;
    CCSafeObject<CCLabelIF> m_timeTTF;
    
    WaitInterface* m_waitInterface;
    
    bool m_canTouch;
    unsigned long m_touchTime;
    bool m_aniComplete;
    int m_dataIndex;
    bool m_cmdComplete;
    int m_rewardId;
    string m_itemId;
    int m_itemCnt;
    
    map<int, vector<string>> m_rewardMap;
    vector<CCParticleBatchNode*> m_parPopVec;
    
    bool mIsGoods;

};

#endif /* defined(__IF__LotteryActView__) */
