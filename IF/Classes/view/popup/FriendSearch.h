//
//  FriendSearch.h
//  IF
//
//  Created by xxrdsg on 15-2-4.
//
//

#ifndef __IF__FriendSearch__
#define __IF__FriendSearch__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "YesNoDialog.h"
#include "CCMultiColTableView.h"
#include "SearchInfo.h"
#include "CCTableViewTouchIFCell.h"
#include "FriendsController.h"
#include "HFHeadImgNode.h"

class FriendSearchView:
public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCTableViewDelegate
, public CCTableViewDataSource
, public EditBoxDelegate
{
public:
    static FriendSearchView* create();
protected:
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual void editBoxReturn(EditBox* editBox);
    
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};

    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual cocos2d::CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual cocos2d::CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickSearchBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void onGetAllianceMembers(CCObject *data);
    
    void generateTabView();
    void generateTabViewMore();
    void getPlayerData(CCObject* param);
    
    void addLoadingAni();
    void removeLoadingAni(CCObject *obj = NULL);
    void callMore(float);
    CCSafeObject<CCSprite> m_loadingIcon;
    
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCControlButton> m_searchBtn;
    CCSafeObject<CCNode> m_editNode;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCTableView> m_tableView;
    CCSafeObject<CCEditBox> m_editBox;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_searchIconNode;
    CCSafeObject<CCLabelIF> m_searchShowTxt;
    
    vector<FriendSearchInfo> m_playerData;
    
    vector<FriendSearchInfo> m_tempData;
    vector<FriendSearchInfo> m_allianceR1;
    vector<FriendSearchInfo> m_allianceR2;
    vector<FriendSearchInfo> m_allianceR3;
    vector<FriendSearchInfo> m_allianceR4;
    vector<FriendSearchInfo> m_allianceR5;
    void reloadDataByRank(bool showR1,bool showR2,bool showR3,bool showR4,bool showR5);
    
    WaitInterface* m_waitInterface;
    int m_maxPlayerNum;//总共显示的玩家数
    int m_totalPlayerNum;//总共搜索到的玩家数
    string m_lastPlayerSearch;
    int m_playerNumPerPage;
    
    map<int,bool> cellStateMap;
};

class FriendSearchCell:
public CCTableViewTouchIFCell
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
  
{
public:
    static FriendSearchCell* create(FriendSearchInfo* searchInfo);
    void setData(FriendSearchInfo* searchInfo);
    
    int cellType;
    FriendSearchInfo* saveInfo;
protected:
private:
    virtual bool init(FriendSearchInfo* searchInfo);
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickApplyBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void onGetMsgApplySuc(CCObject* obj);
    
    CCSafeObject<CCNode> m_infoNode;
    CCSafeObject<CCNode> m_touchNode1;
    CCSafeObject<CCNode> m_iconTouchNode;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCLabelIF> m_vipLabel;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_levelTxt;
    CCSafeObject<CCLabelIF> m_onlineLabel;
    CCSafeObject<CCLabelIF> m_btnTxt;
    CCSafeObject<CCControlButton> m_applyBtn;
    
    CCSafeObject<CCNode> m_titleNode;
    CCSafeObject<CCNode> m_touchNode2;
    CCSafeObject<CCSprite> m_arrowSpr;
    CCSafeObject<CCLabelIF> m_groupLabel;
    CCSafeObject<CCLabelIF> m_numLabel;
    
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    
};

#endif /* defined(__IF__FriendSearch__) */
