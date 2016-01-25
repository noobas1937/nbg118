//
//  NBAllianceHelpReportView.hpp
//  DragonClans
//
//  Created by gj on 1/15/16.
//
//

#ifndef NBAllianceHelpReportView_hpp
#define NBAllianceHelpReportView_hpp


#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCTableViewTouchIFCell.h"

class NBAllianceHelpReportView : public PopupBaseView
, public CCBSelectorResolver

, public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static NBAllianceHelpReportView *create();
    NBAllianceHelpReportView(){};
    ~NBAllianceHelpReportView(){};
protected:
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_viewBg;
    CCSafeObject<CCLabelIF> m_typeTxt;
    CCSafeObject<CCLabelIF> m_attTxt;
    CCSafeObject<CCLabelIF> m_desTxt;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCSprite> m_loadingIcon;
    CCSafeObject<CCLabelIF> m_tip;
    
    CCSafeObject<CCTableView> m_tabView;
    
    void refreashData(CCObject* obj);
    void getRankData();
    void updateInfo();
};

class NBAllianceHelpReportCell : public CCTableViewTouchIFCell
,public CCBSelectorResolver
,public CCBMemberVariableAssigner

{
public:
    static NBAllianceHelpReportCell *create(int index);
    void setData(int index);
private:
    NBAllianceHelpReportCell(int index):m_index(index){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(int index);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    CCSafeObject<CCLabelIF> m_resultTxt1;
    CCSafeObject<CCLabelIF> m_resultTxt2;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCNode> m_iconNode;

    int m_index;
    CCPoint m_startPoint;
};

#endif /* NBAllianceHelpReportView_hpp */
