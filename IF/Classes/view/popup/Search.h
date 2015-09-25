//
//  Search.h
//  IF
//
//  Created by xxrdsg on 15-2-4.
//
//

#ifndef __IF__Search__
#define __IF__Search__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "YesNoDialog.h"
#include "CCMultiColTableView.h"
#include "SearchInfo.h"
#include "CCTableViewTouchIFCell.h"
class SearchView:
public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCMultiColTableViewDataSource
, public CCMultiColTableViewDelegate
  
{
public:
    static SearchView* create();
protected:
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
    virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual unsigned int numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickPlayerBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickAllianceBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickSearchBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void generateTabView();
    void generateTabViewMore();
    void getPlayerData(CCObject* param);
    void getAllianceData(CCObject* param);
    
    void addLoadingAni();
    void removeLoadingAni();
    void callMore(float);
    CCSafeObject<CCSprite> m_loadingIcon;
    
    CCSafeObject<CCControlButton> m_playerBtn;
    CCSafeObject<CCControlButton> m_allianceBtn;
    CCSafeObject<CCControlButton> m_searchBtn;
    CCSafeObject<CCNode> m_editNode;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCMultiColTableView> m_tableView;
    CCSafeObject<CCEditBox> m_editBox;
    CCSafeObject<CCNode> m_bgNode;
    
    vector<PlayerSearchInfo> m_playerData;
    //    CCSafeObject<CCArray> m_playerData;
    CCSafeObject<CCArray> m_allianceData;
    
    
    WaitInterface* m_waitInterface;
    int m_page;
    int m_maxPlayerNum;//总共显示的玩家数
    int m_maxAllianceNum;
    int m_totalPlayerNum;//总共搜索到的玩家数
    int m_totalAllianceNum;
    string m_lastPlayerSearch;
    string m_lastAllianceSearch;
    int m_playerNumPerPage;
    int m_allianceNumPerPage;
};

class SearchCell:
public CCTableViewTouchIFCell
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
  
{
public:
    static SearchCell* create(PlayerSearchInfo*, AllianceInfo*, CCNode* containerNode, int tipType, int showType);//showtype == 1 player  ;   2   alliance
    void setData(PlayerSearchInfo*, AllianceInfo*, CCNode* containerNode, int tipType, int showType);
protected:
private:
    virtual bool init(PlayerSearchInfo*, AllianceInfo*, CCNode* containerNode, int tipType, int showType);
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCNode> m_playerNode;
    CCSafeObject<CCNode> m_clickNode1;
    CCSafeObject<CCNode> m_clickNode2;
    CCSafeObject<CCNode> m_playerIconNode;
    CCSafeObject<CCSprite> m_flagBg;
    CCSafeObject<CCLabelIF> m_myAlliLabel;
    CCSafeObject<CCLabelIF> m_myAlliNameLabel;
    CCSafeObject<CCLabelIF> m_kingdomLabel;
    CCSafeObject<CCLabelIF> m_playerNameLabel;
    
    CCSafeObject<CCNode> m_allianceNode;
    CCSafeObject<CCLabelIF> m_alliNameLabel;
    CCSafeObject<CCLabelIF> m_leaderLabel;
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<CCLabelIF> m_forceLabel;
    CCSafeObject<CCLabelIF> m_alliLanLabel;
    CCSafeObject<CCNode> m_allianceIconNode;
    CCSafeObject<CCNode> m_clickNode;
    CCSafeObject<CCScale9Sprite> m_bg;
    
    CCSafeObject<CCLabelIF> m_tipLabel;
    
    CCNode* m_containerNode;
    PlayerSearchInfo* m_playerInfo;
    AllianceInfo* m_allianceInfo;
    int m_tipType;
    int m_showType;//1 player; 2 alliance;
};

class SearchPlayerInfoBtnView:
public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
  
{
public:
    static SearchPlayerInfoBtnView* create(PlayerSearchInfo* info);
protected:
private:
    virtual bool init(PlayerSearchInfo* info);
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickViewBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickMailBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCControlButton> m_viewBtn;
    CCSafeObject<CCControlButton> m_mailBtn;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCScale9Sprite> m_bg;
    
    PlayerSearchInfo* m_info;
};

class SearchAllianceInfoView: public PopupBaseView
,public CCBSelectorResolver
 
,public CCBMemberVariableAssigner
{
public:
    static SearchAllianceInfoView* create(AllianceInfo* info);
//    SearchAllianceInfoView(AllianceInfo* info):m_info(info){};
    SearchAllianceInfoView(){};
    virtual ~SearchAllianceInfoView(){};
    void updateInfo();
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(AllianceInfo* info);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){};
    
    void saveSuccess();
    void initFun();
    void updateAnnounce(CCObject* param);
    
    void onMailClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onMemberClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onCommentClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void revokeAlliacne(CCObject * pSender, Control::EventType pCCControlEvent);
    void applyAlliacne(CCObject * pSender, Control::EventType pCCControlEvent);
    void joinAlliacne(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void success(CCObject* obj);
    void joinSuccess(CCObject* obj);
    void cancelApply(CCObject* obj);
    void addBtnPar(CCControlButton* btn);
    void checkComment(CCObject* obj);
    
    
    CCSafeObject<CCLabelIFTTF> m_leaderTxt;
    CCSafeObject<CCLabelIF> m_powerTxt;
    CCSafeObject<CCLabelIF> m_membersTxt;
    CCSafeObject<CCLabelIFTTF> m_nameTxt;
    CCSafeObject<CCLabelIFTTF> m_announceTxt;
    CCSafeObject<CCLabelIF> m_language;
    
    CCSafeObject<CCNode> m_announceNode;
    CCSafeObject<CCEditBox> m_announceEditBox;
    CCSafeObject<CCScale9Sprite> m_viewBg;
    CCSafeObject<CCNode> m_allianceIcon;
    CCSafeObject<CCNode> m_funList;
    CCSafeObject<CCNode> m_clickNode;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCControlButton> m_rovkeBtn;
    CCSafeObject<CCControlButton> m_applyBtn;
    CCSafeObject<CCControlButton> m_joinAlliance;
    CCSafeObject<CCNode> m_fireNode1;
    CCSafeObject<CCNode> m_fireNode2;
    CCSafeObject<CCControlButton> m_btnMail;
    CCSafeObject<CCControlButton> m_btnMember;
    CCSafeObject<CCControlButton> m_btnComment;
    CCSafeObject<CCNode> m_textNode;
    CCSafeObject<WaitInterface> m_waitInterface;
    
    CCSafeObject<CCScrollView> m_scrollView;
    AllianceInfo* m_info;
    int m_serverId;
};

#endif /* defined(__IF__Search__) */
