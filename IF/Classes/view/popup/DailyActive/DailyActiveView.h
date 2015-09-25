//
//  DailyActiveView.h
//  IF
//
//  Created by chenfubi on 15/4/10.
//
//

#ifndef __IF__DailyActiveView__
#define __IF__DailyActiveView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
//#include "DefaultTableViewDelegate.h"
#include "CCMultiColTableView.h"
#include "CCTableViewTouchIFCell.h"
class DailyActiveView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCMultiColTableViewDataSource
, public CCMultiColTableViewDelegate
{
public:
	static DailyActiveView* create();
	DailyActiveView(){};
	virtual ~DailyActiveView(){};
    CCSafeObject<CCNode> m_listNode;

public:
    void cellCallBack();
    virtual void update(float delta);
private:
	virtual bool init();
	virtual void onEnter();
    virtual void onExit();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
//    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
//    virtual CCSize cellSizeForTable(CCTableView *table);
//    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
//    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view){};
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){};
    virtual CCTableViewCell* gridAtIndex(CCMultiColTableView *multiTable, unsigned int idx);
    virtual ssize_t numberOfCellsInTableView(CCMultiColTableView *multiTable);
    virtual unsigned int numberOfGridsInCell(CCMultiColTableView *multiTable);
    virtual CCSize gridSizeForTable(CCMultiColTableView *table);
    
    void reloadData(CCObject* pObj);
    void reInitBoxInfo(CCObject* pObj);
    void playParticle(CCPoint p);
    void particleBack(CCObject* pObj);
    void addScrollChild();
    void onGameTick(float dt);
   
    float getGoWidth(int nowScore);
    
    void onShowHand(float posx);
    void hideFlyArrow(float dt=0);
    
private:
    CCSafeObject<CCNode> m_scrollNode;
    CCSafeObject<CCNode> m_headNode;
    CCSafeObject<CCNode> m_particleNode;
    CCSafeObject<CCNode> m_subNode;
    CCSafeObject<CCLabelIF> m_titleTTF0;
    CCSafeObject<CCLabelIF> m_titleTTF1;
    CCSafeObject<CCLabelIF> m_numTTF0;
    CCSafeObject<CCLabelIF> m_numTTF1;
    CCSafeObject<CCScale9Sprite> m_progress;
    CCSafeObject<WaitInterface> m_waitInterface;
    CCSafeObject<CCNode> m_headNode2;
    
    CCSafeObject<CCNode> m_handNode;
    CCSafeObject<CCSprite> m_flyArrow;

    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCPointArray> m_pointArray;
    
    CCMultiColTableView* m_tableView;
    CCScrollView* m_scrollView;
    map<int, float> m_posMap;
    
    float m_hasGoWidth;
    float m_needGoWidth;
    float m_rewardWidth;
    float defaultWidth;
    bool m_isBack;
};


//CLASS DailyActiveCell
class DailyActiveCell : public CCTableViewTouchIFCell
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
//, public CCTouchDelegate
{
public:
    static DailyActiveCell* create(string itemId);
	DailyActiveCell(){};
    void setData(string itemId);
    
private:
	virtual bool init(string itemId);
	virtual void onEnter();
    virtual void onExit();
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    void spineCallBack();
    void doAniAction();
    void aniComplete();
    CCNode *getParentPanel();
private:
    CCSafeObject<CCSprite> m_bgIcon;
    CCSafeObject<CCSprite> m_icon;
    CCSafeObject<CCSprite> m_ttfBg;
    CCSafeObject<CCLabelIF> m_titleTTF;
    CCSafeObject<CCLabelIF> m_numTTF;
    CCSafeObject<CCLabelIF> m_comTTF;
    CCSafeObject<CCLabelIF> m_levelTTF;
    CCSafeObject<CCNode> m_lockNode;
    CCSafeObject<CCNode> m_completeNode;
    CCSafeObject<CCNode> m_spineNode;
    CCSafeObject<CCNode> m_ownNode;
    
//    CCSafeObject<CCNode> m_parentNode;
    
    string m_itemId;
    
};

//CLASS DailyActiveBoxInfoCell
class DailyActiveBoxInfoCell : public CCAniNode
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
//, public CCTouchDelegate
{
public:
    static DailyActiveBoxInfoCell* create(string itemId);
    DailyActiveBoxInfoCell(){};
    virtual ~DailyActiveBoxInfoCell(){};
//    void setData(string itemId);
    
private:
    virtual bool init(string itemId);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
//    void onRewardBtnClick(CCObject* pNode, CCControlEvent pEvent);
    void updateInfo(CCObject* pObj);
    
private:
    CCSafeObject<CCNode> m_particleNode;
    CCSafeObject<CCNode> m_particleNode2;
    CCSafeObject<CCSprite> m_rewardSp;
    CCSafeObject<CCLabelIF> m_pointTTF;
    CCSafeObject<CCLabelIF> m_btnTTF;
    CCSafeObject<CCSprite> m_spr;
    
    string m_itemId;
};

#endif /* defined(__IF__DailyActiveView__) */
