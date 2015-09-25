//
//  KingsGiftView.h
//  IF
//
//  Created by 李锐奇 on 15/1/23.
//
//

#ifndef __IF__KingsGiftView__
#define __IF__KingsGiftView__

#include <stdio.h>
#include "CommonInclude.h"
#include "PopupViewController.h"
#include "CCTableViewTouchIFCell.h"
class KingsGiftInfo : public CCObject{
public:
    CREATE_FUNC(KingsGiftInfo);
    virtual bool init(){return true;};
    
    std::string presentId;
    std::string dialogId;
    std::string rewardId;
    CCSafeObject<CCArray> m_reward;
    int usedCount;
    int totalCount;
};

class KingsGiftCell : public CCTableViewTouchIFCell
,public CCBMemberVariableAssigner
//,public CCTouchDelegate
{
public:
    CREATE_FUNC(KingsGiftCell);
    void setData(KingsGiftInfo *info);
private:
    void refreshView();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void addBG(int totalH);
    KingsGiftInfo *m_info;
    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCLabelIF> m_numText;
    CCSafeObject<CCNode> m_iconContainer;
    CCSafeObject<CCControlButton> m_giftBtn;
    CCSafeObject<CCNode> m_titleNode;
    CCSafeObject<CCNode> m_cellBGNode;
    CCSafeObject<CCScale9Sprite> m_sprItemBG;
};

class KingsGiftView : public PopupBaseView
,public CCBMemberVariableAssigner
, public CCBSelectorResolver
, public CCTableViewDataSource
, public CCTableViewDelegate
{
public:
    CREATE_FUNC(KingsGiftView);
protected:
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void tableCellTouched(  cocos2d::extension::TableView* table, CCTableViewCell* cell);
    virtual cocos2d::CCSize tableCellSizeForIndex(  cocos2d::extension::TableView *table, ssize_t idx);
    virtual cocos2d::CCSize cellSizeForTable(  cocos2d::extension::TableView *table);
    virtual CCTableViewCell* tableCellAtIndex(  cocos2d::extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(  cocos2d::extension::TableView *table);
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    void onIntroClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onHistoryClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void getData();
    void getDataBack(CCObject *dict);
    void refreshView();
    void addBG();
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCNode> m_downNode;
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCScale9Sprite>  m_buildBG;
    CCSafeObject<CCControlButton> m_introBtn;
    CCSafeObject<CCControlButton> m_historyBtn;
    CCSafeObject<CCNode> m_bgContainer;
    CCSafeObject<CCNode> m_fireNode1;
    CCSafeObject<CCNode> m_fireNode2;
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCArray> m_info;
};

class KingsGiftHistoryCell :  public CCTableViewCell
, public CCBMemberVariableAssigner
{
public:
    CREATE_FUNC(KingsGiftHistoryCell);
    void setData(std::string contents, std::string timeStr);
private:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    void refresh();
    
    CCSafeObject<CCScale9Sprite> m_cellBg;
    CCSafeObject<CCNode> m_node2;
    CCSafeObject<CCLabelIFTTF> m_contentTxt;
    CCSafeObject<CCLabelIF> m_timeTxt;
    std::string m_contents;
    std::string m_timeStr;
};

class GiftGiveHistoryView : public PopupBaseView
, public CCBMemberVariableAssigner
, public CCTableViewDataSource
, public CCTableViewDelegate

{
public:
    CREATE_FUNC(GiftGiveHistoryView);
protected:
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void tableCellTouched(  cocos2d::extension::TableView* table, CCTableViewCell* cell);
    virtual cocos2d::CCSize tableCellSizeForIndex(  cocos2d::extension::TableView *table, ssize_t idx);
    virtual cocos2d::CCSize cellSizeForTable(  cocos2d::extension::TableView *table);
    virtual CCTableViewCell* tableCellAtIndex(  cocos2d::extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(  cocos2d::extension::TableView *table);
private:
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}

    void getData();
    void getDataBack(CCObject *dict);
    void refreshView();
    CCSafeObject<CCLabelIFTTF> m_testTxt;
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCNode> m_infoNode;
    CCSafeObject<CCNode> m_historyNode;
    CCSafeObject<CCControlButton> m_historyBtn;
    CCSafeObject<CCControlButton> m_infoBtn;

    std::vector<std::string> m_contents;
    std::vector<std::string> m_times;
};

#endif /* defined(__IF__KingsGiftView__) */
