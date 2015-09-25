//
//  AllianceMarketView.h
//  IF
//
//  Created by ganxiaohua on 14-4-4.
//
//

#ifndef __IF__AllianceMarketView__
#define __IF__AllianceMarketView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"
#include "DefaultTableViewDelegate.h"

class AllianceMarketView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static AllianceMarketView* create();
    AllianceMarketView(){};
    virtual ~AllianceMarketView(){};
    void updateInfo(CCObject* p);
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    
    void onCloseClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void openJoinAlliance(CCObject * pSender, Control::EventType pCCControlEvent);
    void addLoadingAni();
    
    CCSafeObject<CCNode> m_baseContainer;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScale9Sprite> m_viewBg;
    CCSafeObject<CCLabelIF> m_infoTxt;
    CCSafeObject<CCNode> m_noAllianeNode;
    CCSafeObject<CCLabelIF> m_noAllianceTip;
    CCSafeObject<CCControlButton> m_btnJoin;
    CCSafeObject<CCSprite> m_loadingIcon;
    
    CCSafeObject<CCTableView> m_tableView;
    CCSafeObject<CCArray> m_data;
    int m_page;
};

class AllianceMarketCell :
public CCTableViewCell
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static AllianceMarketCell* create(PlayerInfo* player);
    AllianceMarketCell(PlayerInfo* player):m_info(player){};
    void setData(PlayerInfo* info);
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onBtnHelpClick(CCObject * pSender, Control::EventType pCCControlEvent);

    CCSafeObject<CCLabelIF> m_powerTxt;
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCNode> m_head;
    CCSafeObject<CCControlButton> m_btnHelp;
    PlayerInfo* m_info;
};
#endif /* defined(__IF__AllianceMarketView__) */
