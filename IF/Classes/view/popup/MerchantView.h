
#ifndef __IF__MerchantView__
#define __IF__MerchantView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"
#include "DefaultTableViewDelegate.h"
#include "ToolInfo.h"
#include "ButtonLightEffect.h"

class MerchantView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
//, public CCTouchDelegate
,public CCTableViewDataSource
,public DefaultTableViewDelegate

{
public:
    static MerchantView* create();
    MerchantView();
    ~MerchantView();
    void refreshView(CCObject *ccObj = NULL);
    void closeView(CCObject *ccObj = NULL);
    void finishAnimation(CCObject *ccObj = NULL);
    void onOkRefresh();
    void resetRefreshTip(CCObject *ccObj = NULL);
    void onNewDay(float dt);
protected:
    // scrollview delegate
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    virtual void scrollViewDidScroll(CCScrollView* view);
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    void addEffect();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void onRefreshClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onTipClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void getHotItems(bool useGold = false);
    void sortDataByOrderNum();
    void refreshTime(CCObject* param);
    void onTimer(float dt);
    void refreshTip();
    void setTip(int index);
    CCTableView* m_tabView;
//    CCSafeObject<CCScale9Sprite> m_buildBG;
//    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_timeNode;
    CCSafeObject<CCNode> m_itemNode1;
    CCSafeObject<CCLabelIF> m_lblInfo1;
//    CCSafeObject<CCLabelIF> m_timeTitleText;
    CCSafeObject<CCLabelIF> m_timeLeftText;
    CCSafeObject<CCControlButton> m_btnRefresh;
    CCSafeObject<CCLabelIF> m_lblRefresh;
    CCSafeObject<CCLabelIF> m_lblTitleRefresh;
    CCSafeObject<CCSprite> m_sprRefresh;
    CCSafeObject<CCLabelIF> m_lblDialog;
    CCSafeObject<CCNode> m_desNode;
    CCSafeObject<CCLabelIF> m_desName;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCNode> m_receiveGlow;
    double m_refreshTime;
    double m_leftTime;
    int m_refreshCost;
    int m_dataCount;
    void generateData();
    bool m_onTimer;
    bool m_isInit;
    bool m_isRefresh;
    int m_lastIndex;
    //0-不显示 1-每日首次刷新 2-价格变化刷新 3-列表中拥有超值物品
    int m_refreshTipType;
    bool m_showRefreshEff;
    int m_curFinAnimation;
};
class MerchantShining:
public CCNode{
public:
    static MerchantShining* create(const ccColor3B &color,const CCSize size);
    MerchantShining():mTopSprite(NULL),mBottomSprite(NULL),mEffSprite(NULL),mEffSprite1(NULL),m_startPlay(false){
        
    }
    ~MerchantShining();
    void setShineColor(const ccColor3B &color);
    void setShineAlpha(const int alpha);
    void showEff2(bool show, float scale = 1);
    void startPlay(bool play);
private:
    virtual void onEnter();
    virtual void onExit();
    bool init(const ccColor3B &color,const CCSize size);
    CCSprite *mTopSprite;
    CCSprite *mBottomSprite;
    ButtonLightEffect *mEffSprite;
    CCParticleSystemQuad *mEffSprite1;
//    void playFrame(float fUpdate);
//    int m_curFrame;
//    int m_curDelay;
    int m_fAlpha;
//    int m_curEffFrame;
//    int m_curEffDelay;
//    int m_maxEffDelay;
//    bool m_playEff;
    bool m_startPlay;
};

class MerchantCell : public CCAniNode
,public CCBSelectorResolver
 
,public CCBMemberVariableAssigner
{
public:
    static MerchantCell* create(int index, CCNode* touchNode);
    MerchantCell():m_info1(NULL),m_price(0),m_boostBid(0),m_touchNode(NULL),m_bBuy(false),m_shiningNode(NULL),m_bRefreshClick(false),m_bIsBuying(false){};
    void setData();
    void playAnimation(bool bRefresh=false);
    void animationCallback();
    void changeData();
    ~MerchantCell();
private:
    bool init(int index, CCNode* touchNode);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    void onClickBuyBtn1(CCObject * pSender, Control::EventType pCCControlEvent);
    void onCloseConfirm();
    void onBuyTool1(CCObject *ccObj = NULL);
    void onRetBuyTool();
    void setDesNode(MerchantToolInfo* info,int index);
    void setLeft(float dt);
    void setLeftHot(bool ishot,int num);
    
    CCSafeObject<CCLabelIF> m_nameLabel1;
    CCSafeObject<CCLabelIF> m_priceLabel1;
    CCSafeObject<CCNode> m_picNode1;
    CCSafeObject<CCNode> m_itemNode1;
    CCSafeObject<CCSprite> m_picBG1;
    CCSafeObject<CCNode> m_desNode;
    CCSafeObject<CCLabelIF> m_desName;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCScale9Sprite> m_buyBtnSpr1;
    CCSafeObject<CCScale9Sprite> m_buyBtngraySpr1;
    CCSafeObject<CCScale9Sprite> m_saleoutSpr1;
    CCSafeObject<CCNode> m_costNode1;
    CCSafeObject<CCNode> m_itemHotNode1;
    CCSafeObject<CCNode> m_itemInfoNode;
    CCSafeObject<CCSprite> m_hotSpr1;
    CCSafeObject<CCSprite> m_hotgraySpr1;
    CCSafeObject<CCSprite> m_hotNumSpr1;
    CCSafeObject<CCSprite> m_hotNumgraySpr1;
    CCSafeObject<CCLabelIF> m_hotdesText1;
    CCSafeObject<CCLabelIF> m_hotNum1Text;
    CCSafeObject<CCScale9Sprite> m_hotLineSpr1;
    CCSafeObject<CCLabelIF> m_hotpriceLabel1;
    CCSafeObject<CCNode> m_waitingNode;
    CCSafeObject<CCNode> m_superNode;
    CCSafeObject<CCLabelIF> m_lblSuper;
    MerchantShining* m_shiningNode;
    CCNode* m_touchNode;
    MerchantToolInfo* m_info1;
    int m_price;
    int m_boostBid;
    bool m_clickInSide;
    CCPoint m_startPoint;
    bool m_isClick;
    int m_num;
    bool m_bBuy;
    CC_SYNTHESIZE(int, m_index, DataIndex);
    bool m_bRefreshClick;
    bool m_bIsBuying;
};

class MerchantTabelCell
:public CCTableViewCell{
public:
    static MerchantTabelCell* create(int index, CCNode* touchNode);
    void setData(int index,bool bAnimate = false);
    void playAnimation();
private:
    bool init(int index, CCNode* touchNode);
    MerchantCell *mCell;
};


#endif
