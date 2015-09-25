
#ifndef __IF__RepayView__
#define __IF__RepayView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"
#include "DefaultTableViewDelegate.h"



class RepayView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
//, public CCTouchDelegate
,public CCTableViewDataSource
,public DefaultTableViewDelegate

{
public:
    static RepayView* create();
    RepayView();
    virtual ~RepayView();
    
    void refreshData(CCObject *ccObj);
    void showDes(CCObject *ccObj);
protected:
    // scrollview delegate
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    virtual void scrollViewDidScroll(CCScrollView* view);
    
    void onTimer(float dt);
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    void initPointNode(bool bTween);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onGoToPayClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onTipClick(CCObject * pSender, Control::EventType pCCControlEvent);
    CCTableView* m_tabRewards;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCLabelIF> m_lblTime;
    CCSafeObject<CCLabelIF> m_lblTitle;
    CCSafeObject<CCLabelIF> m_lblPointTitle;
    CCSafeObject<CCLabelIF> m_lblPointNum;
    CCSafeObject<CCNode> m_nodePoints;
    CCSafeObject<CCLabelIF> m_lblDes;
    CCSafeObject<CCControlButton> m_btnGoBuy;
    CCSafeObject<CCLabelIF> m_lblRewardTitle;
    CCSafeObject<CCNode> m_nodeRewards;
    CCSafeObject<CCNode> m_desNode;
    bool m_bInit;
    int m_dataCount;
    float m_lblPointOrgX;
    float m_lblPointOrgW;
    bool m_isTouchMove;
};


class RepayCell :
public CCBMemberVariableAssigner
,public CCTableViewCell
{
public:
    static RepayCell* create(int index, CCNode* touchNode);
    RepayCell();
    void setData(int index);
private:
    bool init(int index, CCNode* touchNode);
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    CCSafeObject<CCScale9Sprite> m_sprBG;
    CCSafeObject<CCLabelIF> m_lblTitle;
    CCSafeObject<CCNode> m_nodeReward;
    CCSize mOrgSize;
    CCNode* m_touchNode;
    int m_index;
};

class RepayItemCell :
public CCBMemberVariableAssigner
//,public CCTouchDelegate
,public Layer
{
public:
    static RepayItemCell* create(std::string itemid,int type,int num);
    RepayItemCell();
protected:
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
private:
    bool init(std::string itemid,int type,int num);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    CCSafeObject<CCLabelIF> m_lblNum;
    CCSafeObject<CCNode> m_nodeIcon;
    CCSafeObject<CCSprite> m_sprBG;
    std::string toolName;
    std::string toolDes;
};

class RepayIcon: public CCBMemberVariableAssigner
,public CCNode{
public:
    static RepayIcon* create();
    RepayIcon();
    virtual ~RepayIcon();
    void showPartical(bool bshow);
    void onTimer(float dt);
private:
    bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    CCSafeObject<CCLabelIF> m_lblTime;
    CCSafeObject<CCNode> m_aniNode;
};


#endif /* defined(__IF__RepayView__) */
