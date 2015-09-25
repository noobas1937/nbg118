

#ifndef __IF__MonthCardView__
#define __IF__MonthCardView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"

class MonthCardView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
//, public CCTouchDelegate
{
public:
    static MonthCardView* create();
    MonthCardView();
    ~MonthCardView();
    
    void refreshData(CCObject *ccObj);
protected:
    void generateData();
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
//    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    CCScrollView *m_scrollView;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCNode> m_BGNode;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCLabelIF> m_lblDes;
    
    vector<string> mCardID;
};

class MonthCardRewardNode : public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
//,public CCTouchDelegate
{
public:
    static MonthCardRewardNode* create(string cardid);
    MonthCardRewardNode();
    ~MonthCardRewardNode();
    
    void rewardCard(CCObject *ccObj);
protected:
    void onGameTick(float time);
    void refreshData();
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(string cardid);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickReward(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    void showDes(int idx);
    
    CCSafeObject<CCLabelIF> m_lblTitle1;
    CCSafeObject<CCLabelIF> m_lblTitle2;
    CCSafeObject<CCControlButton> m_btnReward;
    CCSafeObject<CCNode> m_nodeTime;
    CCSafeObject<CCLabelIF> m_lblDes;
    CCSafeObject<CCLabelIF> m_lblTime;
    CCSafeObject<CCNode> m_picNode[2];
    CCSafeObject<CCLabelIF> m_numLabel[2];
    CCSafeObject<CCSprite> m_itemSpr[2];
    CCSafeObject<CCNode> m_desNode;
    WaitInterface* m_waitInterface;
    string mCardID;
    bool m_bTick;
};


class MonthCardBuyNode : public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
//,public CCTouchDelegate
{
public:
    static MonthCardBuyNode* create(string cardid);
    MonthCardBuyNode();
    ~MonthCardBuyNode();
    
    void buySuccess(CCObject *ccObj);
    void errorPayment(cocos2d::CCObject *obj);
    void onShowError(float dt);
protected:
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(string cardid);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickBuy(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onClickTip(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    void showDes(int idx);
    CCSafeObject<CCLabelIF> m_lblTitle1;
    CCSafeObject<CCLabelIF> m_lblTitle2;
//    CCSafeObject<CCLabelIF> m_lblDes;
//    CCSafeObject<CCLabelIF> m_lblCost;
    CCSafeObject<CCSprite> m_itemSpr[2];
    CCSafeObject<CCNode> m_picNode[2];
    CCSafeObject<CCLabelIF> m_numLabel[2];
    CCSafeObject<CCControlButton> m_btnBuy;
    CCSafeObject<CCNode> m_desNode;
    
    string mCardID;
//    bool m_isBuy;
};
#endif /* defined(__IF__MonthCardView__) */
