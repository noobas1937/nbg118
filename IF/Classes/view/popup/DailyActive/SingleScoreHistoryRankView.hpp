//
//  SingleScoreHistoryRankView.hpp
//  IF
//
//  Created by 付彬 on 15/11/25.
//
//

#ifndef SingleScoreHistoryRankView_hpp
#define SingleScoreHistoryRankView_hpp

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "TimeExchangeView.h"
#include "ActivityEventObj.h"
#include "ActivityInfo.h"
#include "DefaultTableViewDelegate.h"
#include "CCTableViewTouchIFCell.h"
#include "HFHeadImgNode.h"

class SingleScoreHistoryRankView: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCTableViewDataSource
, public DefaultTableViewDelegate
{
public:
    static SingleScoreHistoryRankView* create(int type);
    SingleScoreHistoryRankView(){};
    virtual ~SingleScoreHistoryRankView(){};
    
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
    
    void retData(CCObject* param);
    void refreshData();
    
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCLabelIF> m_info1Label;
    CCSafeObject<CCLabelIF> m_info2Label;
    CCSafeObject<CCLabelIF> m_info3Label;
    CCSafeObject<CCControlButton> m_tipBtn;
    
    CCTableView* m_tableView;
    CCSafeObject<CCArray> m_data;
    WaitInterface* m_waitInterface;
    
    int m_type;
    int m_panelType;
    int m_startY;
    
    int m_score;
    int m_price;
    bool m_bHasGet;
};

class SingleScoreHistoryRankCell: public CCTableViewTouchIFCell
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static SingleScoreHistoryRankCell* create();
    SingleScoreHistoryRankCell(){};
    virtual ~SingleScoreHistoryRankCell(){};
    
    void setTitleData(string title);
    void setSubTitleData(string subTitle);
    void setValueData(string value);
    void setTopData(string title);
    void setItemData(string rank, string server, string player, string rewardId, int idx, string pic="", int picVer=0, string uid="");
    void setCountryData(string rank, string server, string player, int idx, string pic="");
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCNode> m_rankNode;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCLabelIF> m_rankLabel;
    CCSafeObject<CCLabelIF> m_playerLabel;
    CCSafeObject<CCLabelIF> m_serverLabel;
    CCSafeObject<CCScale9Sprite> m_rect;
    
    CCSafeObject<CCNode> m_serverNode;
    CCSafeObject<CCLabelIF> m_valueLabel;
    
    CCSafeObject<CCNode> m_headNode;
    CCSafeObject<CCSprite> m_headbg;
    CCSafeObject<CCSprite> m_albg;
    
    CCSafeObject<CCSprite> m_bgGreen;
    CCSafeObject<CCSprite> m_bgRed;
    
    CCSafeObject<CCNode> m_titleNode;
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelIF> m_subTitleLabel;
    
    CCSafeObject<CCNode> m_topNode;
    CCSafeObject<CCLabelIF> m_topLabel;
    
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    string m_rewardId;
    int m_cellIdx;
    
    string m_pic;
    int m_picVer;
    
    void retData(CCObject* param);
    void refresh(CCObject* param);
    void showRwd();
};

#endif /* SingleScoreHistoryRankView_hpp */
