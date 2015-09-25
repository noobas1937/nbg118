//
//  AllianceHelpView.h
//  IF
//
//  Created by chenliang on 14-4-15.
//
//

#ifndef __IF__AllianceHelpView__
#define __IF__AllianceHelpView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"

class AllianceHelp
{
public:
    CC_SYNTHESIZE(int, m_rank, Rank);
    CC_SYNTHESIZE(string, m_icon, Icon);
    CC_SYNTHESIZE(string, m_name, Name);
    CC_SYNTHESIZE(bool, m_isValid, IsValid);
    CC_SYNTHESIZE(int, m_currentHelp, CurrentHelp);
    CC_SYNTHESIZE(int, m_level, Level);
    CC_SYNTHESIZE(int, m_stats, Stats);//0 为自己的，1为别人的
    CC_SYNTHESIZE(string, m_itemId, ItemId);
    CC_SYNTHESIZE(string, m_pic, Pic);
    CC_SYNTHESIZE(int, m_picVer, PicVer);
    CC_SYNTHESIZE(string, m_helpId, HelpId);
    CC_SYNTHESIZE(string, m_senderId, SenderId);
    CC_SYNTHESIZE(string, m_allianceId, AllianceId);
    CC_SYNTHESIZE(string, m_buildingName, BuildingName);
    CC_SYNTHESIZE(int, m_amount, Amount);
    CC_SYNTHESIZE(int, m_allianceHelpType, AllianceHelpType);
};

typedef vector<AllianceHelp> AllianceHelps;

class AllianceHelpView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCTableViewDataSource
, public DefaultTableViewDelegate
{
public:
    CREATE_FUNC(AllianceHelpView);
    
    AllianceHelpView()
    {
    };
    ~AllianceHelpView();
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual void scrollViewDidScroll(CCScrollView* view);
private:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    Vector<Node*> m_cells;
    
    void onHelpBtnClick(CCObject *pSender, CCControlEvent event);
    void openJoinAlliance(CCObject *pSender, CCControlEvent event);
    void onClickHellpAll(CCObject *pSender, CCControlEvent event);
    void updateData(CCObject* param);
    void removeOneHelp(CCObject* param);
    void addOneHelp(CCObject* param);
    void updateHelpData(CCObject* param);
    void addLoadingAni();
    
    CCSafeObject<CCScale9Sprite> m_background;
    CCSafeObject<CCScale9Sprite> m_background2;
    CCSafeObject<CCControlButton> m_helpAll;
    CCSafeObject<CCControlButton> m_btnJoin;
    CCSafeObject<CCLabelIF> m_noAllianceTip;
    CCSafeObject<CCNode> m_buttonNode;
    CCSafeObject<CCNode> m_noAllianeNode;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCLabelIF> m_tipTxt;
    CCSafeObject<CCTableView> m_tableView;
    CCSafeObject<CCSprite> m_loadingIcon;
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCNode> m_bgNode;
    
    CCSafeObject<CCNode> m_fireNode1;
    CCSafeObject<CCNode> m_fireNode2;
    
    AllianceHelps m_allianceHelps;
};

#include "HFHeadImgNode.h"

class AllianceHelpViewCell :
public CCTableViewCell
 
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    CREATE_FUNC(AllianceHelpViewCell);
    
    AllianceHelpViewCell(){};
    
    ~AllianceHelpViewCell()
    {
    }
    
    void bind(AllianceHelp* allianceHelp);
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_iconPlaceHolder;
    CCSafeObject<CCLabelIF> m_descTxt;
    CCSafeObject<CCLabelIFTTF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_progressTxt;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCScale9Sprite> m_progress;
    
    CCSafeObject<CCControlButton> m_helpBtn;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    
    void onHelpClick(CCObject *pSender, CCControlEvent event);
    void helpSuccess();

    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CC_SYNTHESIZE(AllianceHelp*, m_allianceHelp, AllianceHelp);
};

#endif /* defined(__IF__AllianceHelpView__) */
