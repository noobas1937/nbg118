//
//  SingleScoreRankView.hpp
//  IF
//
//  Created by 付彬 on 15/11/25.
//
//

#ifndef SingleScoreRankView_hpp
#define SingleScoreRankView_hpp

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "TimeExchangeView.h"
#include "ActivityEventObj.h"
#include "ActivityInfo.h"
#include "DefaultTableViewDelegate.h"

class SingleScoreRankView: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCTableViewDataSource
, public DefaultTableViewDelegate
{
public:
    static SingleScoreRankView* create(int type);
    SingleScoreRankView(){};
    virtual ~SingleScoreRankView(){};
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(int type);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    
    void onTipBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onHistoryBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onConRankBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    void refreshData();
    
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScale9Sprite> m_bg;
    
    CCSafeObject<CCLabelIF> m_info1Label;
    CCSafeObject<CCLabelIF> m_info2Label;
    CCSafeObject<CCLabelIF> m_info3Label;
    CCSafeObject<CCControlButton> m_tipBtn;
    CCSafeObject<CCControlButton> m_historyBtn;
    CCSafeObject<CCControlButton> m_conRankBtn;
    
    CCTableView* m_tableView;
    CCSafeObject<CCArray> m_data;
    
    int m_type;
    
    int m_panelType;
    int m_startY;
    
    int m_score;
    int m_price;
    bool m_bHasGet;
};

class SingleScoreRankCell: public CCTableViewCell
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static SingleScoreRankCell* create();
    SingleScoreRankCell(){};
    virtual ~SingleScoreRankCell(){};
    
    void setTitleData(string title, bool isMine);
    void setItemData(int itemId,string nameStr,int countStr,int itemType);
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCNode> m_itemNode;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIFBMFont> m_numLabel;
    
    CCSafeObject<CCNode> m_titleNode;
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelIF> m_stateLabel;
    CCSafeObject<CCNode> m_stateNode;
};

#endif /* SingleScoreRankView_hpp */
