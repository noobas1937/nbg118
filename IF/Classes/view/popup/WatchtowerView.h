//
//  WatchtowerView.h
//  IF
//
//  Created by fubin on 14-2-24.
//
//

#ifndef IF_WatchtowerView_h
#define IF_WatchtowerView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"
#include "HFHeadImgNode.h"

class EnemyInfoCell;

class WatchtowerView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
, public CCMultiColTableViewDataSource
, public CCMultiColTableViewDelegate
{
public:
    static WatchtowerView* create(int buildId);
    WatchtowerView():m_buildId(0),m_dataCnt(0),m_cellNum(0){};
    virtual ~WatchtowerView(){};
    void updateInfo(CCObject* p);
    
protected:
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
	virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual unsigned int numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
    
private:
    virtual void onEnter();
    virtual void onExit();
    void refreshIgnoreList(CCObject* obj);
    void onClickIgnoreBtn(CCObject *pSender, CCControlEvent event);
    void secondConfirm();
    virtual bool init(int buildId);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    void getInfoCallBack(CCObject* param);
    void updateTime(float _time);
    void onClickHelpBtn(CCObject *pSender, CCControlEvent event);
    void showMonsterInfo();
    
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_baseContainer;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCLabelIF> m_msgLabel;
    CCSafeObject<CCControlButton> m_ignoreBtn;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCNode> m_monster;
    CCSafeObject<CCControlButton> m_helpBtn;
    CCSafeObject<CCLabelIF> m_txt1;
    CCSafeObject<CCLabelIF> m_txt2;
    CCSafeObject<CCLabelIF> m_txt3;
    CCSafeObject<CCLabelIF> m_txt4;
    
    CCSafeObject<CCMultiColTableView > m_tabView;
    CCSafeObject<CCScrollView> m_scrollView;
    
    int m_buildId;
    int m_dataCnt;
    int m_cellNum;
    int m_callCount;
};

class EnemyInfoCell :
public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewCell
{
public:
    static EnemyInfoCell* create(int itemId, int buildId);
    EnemyInfoCell():m_itemId(0),m_buildId(0),m_sumTime(0),m_arrTime(0){};
    void setData(int itemId, int buildId);
    void cellTouchEnded(cocos2d::CCTouch *pTouch);
private:
    bool init(int itemId, int buildId);
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickWatchBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onMoreClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onEnterFrame(float dt);
    
    CCSafeObject<CCControlButton> m_watchBtn;
    CCSafeObject<Label> m_nameLabel;
    CCSafeObject<CCLabelIF> m_timeLabel;
    CCSafeObject<CCLabelIF> m_barLabel;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCScale9Sprite> m_bar;
    
    CCSafeObject<CCNode> m_node1;
    CCSafeObject<CCNode> m_node2;
    CCSafeObject<CCLabelIF> m_titleLabel0;
    CCSafeObject<CCLabelIF> m_titleLabel1;
    CCSafeObject<CCScale9Sprite> m_bar0;
    CCSafeObject<CCScale9Sprite> m_bar1;
    CCSafeObject<CCLabelIF> m_barLabel0;
    CCSafeObject<CCLabelIF> m_barLabel1;
    
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    
    int m_itemId;
    int m_buildId;
    int m_sumTime;
    double m_arrTime;
    double m_waitTime;
    int m_type;
};

#endif
