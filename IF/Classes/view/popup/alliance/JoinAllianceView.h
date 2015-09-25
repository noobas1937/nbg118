//
//  JoinAllianceView.h
//  IF
//
//  Created by ganxiaohua on 14-3-24.
//
//

#ifndef __IF__JoinAllianceView__
#define __IF__JoinAllianceView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
//#include "CCMultiColTableView.h"
#include "AllianceInfo.h"
#include "JoinAllianceCommand.h"
#include "YesNoDialog.h"
#include "AllianceInfoView.h"
#include "CCClipNode.h"
//#include "CCTableView.h"
#include "CCTableViewTouchIFCell.h"
class JoinAllianceView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
, public CCTableViewDataSource
, public CCTableViewDelegate
 
{
public:
    static JoinAllianceView* create(float operateAreaHeight);
    JoinAllianceView(float operateAreaHeight):m_operateAreaHeight(operateAreaHeight),isRecommendView(true),m_total_receive(0),cellTouchIdx(-1){};
    ~JoinAllianceView();
    void updateAlliances(CCObject* data);
    void getMoreInfo(CCObject* data);
    void onAllianceShake(CCObject* p);
    bool canShakeAlliance();
protected:
    virtual void tableCellTouched(cocos2d::extension::TableView * table, CCTableViewCell* cell);
    virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx);
	virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::TableView  *table);
    virtual CCTableViewCell* tableCellAtIndex(cocos2d::extension::TableView  *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView  *table);
    virtual void tableCellHighlight(cocos2d::extension::TableView  *table, CCTableViewCell* cell);
    virtual void tableCellUnhighlight(cocos2d::extension::TableView  *table, CCTableViewCell* cell);
    
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view){};
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onSelectClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onSearchClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onNextPageClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onLastPageClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void clickBtn1(CCObject * pSender, Control::EventType pCCControlEvent);
    void clickBtn2(CCObject * pSender, Control::EventType pCCControlEvent);
    void clickBtn3(CCObject * pSender, Control::EventType pCCControlEvent);
    void createAlliacne(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void pageChange();
    void showAlliance(CCObject* p);
    void getDataToServer(int page);
    void onGameTick(float time);
    void onShakeAni();
    void addLoadingAni();
    void removeLoadingAni();
    void callMore(float t);
    void shakeDataBack(CCObject* pObj);
    void delayFun(float time);
    CCSafeObject<CCSprite> m_loadingIcon;
    CCSafeObject<CCNode> m_fireNode1;
    CCSafeObject<CCNode> m_fireNode2;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCScale9Sprite> m_viewBg;
    CCSafeObject<CCControlButton> m_btnSearch;
    CCSafeObject<CCControlButton> m_btnAlliance;
    CCSafeObject<CCLabelIF> m_allianceBtnTitle;
    CCSafeObject<CCLabelIF> m_createBtnCost;
    CCSafeObject<CCNode> m_allianceList;
    CCSafeObject<CCLabelIF> m_pageTxt;
    CCSafeObject<CCNode> m_pageNode;
    CCSafeObject<CCNode> m_searchNode;
    CCSafeObject<CCControlButton> m_btnFirst;
    CCSafeObject<CCControlButton> m_btnPrev;
    CCSafeObject<CCControlButton> m_nextBtn;
    CCSafeObject<CCControlButton> m_lastPageBtn;
    CCSafeObject<CCNode> m_buttonNode;
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCEditBox> m_editBox;
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCArray> allianceArray;
    CCSafeObject<CCSprite> m_iconGold;
    CCSafeObject<CCNode> m_shakeGuideNode;
    
    float m_operateAreaHeight;
    int currentPage;
    bool m_first;

    bool isRecommendView;
    unsigned long m_total_receive;
    int cellTouchIdx;
};

class JoinAlliaceCell :
public CCTableViewTouchIFCell
 
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static JoinAlliaceCell* create(AllianceInfo* info,CCNode* touchNode);
    JoinAlliaceCell(AllianceInfo* info,CCNode* touchNode):m_info(info),m_touchNode(touchNode){};
    void updateAllianceInfo(AllianceInfo* info);
    void setHighlight();
    void unHighlight();
    
private:
    bool init();
    
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void onViewClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onJoinClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onApplyClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onRevokeClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void success(CCObject* p);
    void joinSuccess(CCObject* obj);
    void scrollText();
    void cancelApply(CCObject* p);
    void joinLater();

    CCSafeObject<CCLabelIFTTF> m_nameTxt;
    CCSafeObject<CCLabelIFTTF> m_leaderTxt;
    CCSafeObject<CCLabelIFTTF> m_allianceNumTxt;
    CCSafeObject<CCLabelIFTTF> m_forceTxt;
    CCSafeObject<CCLabelIFTTF> m_appTxt;
    CCSafeObject<CCNode> m_btnView;
    CCSafeObject<CCNode> m_RestrictIcon;
    CCSafeObject<CCControlButton> m_btnJoin;
    CCSafeObject<CCControlButton> m_applyBtn;
    CCSafeObject<CCControlButton> bntRevoke;
    CCSafeObject<CCNode> m_appNode;
    CCSafeObject<CCNode> m_allianceIcon;
    CCSafeObject<CCNode> m_clickNode;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCLabelIFTTF> m_language;
    CCSafeObject<CCLabelIFTTF> m_allianceTxt;
   
    
    
    CCNode* m_touchNode;
    AllianceInfo* m_info;
    
    CCPoint m_touchPos;
};
#endif /* defined(__IF__JoinAllianceView__) */

