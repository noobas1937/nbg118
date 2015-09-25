//
//  AllianceInfoView.h
//  IF
//
//  Created by ganxiaohua on 14-3-25.
//
//

#ifndef __IF__AllianceInfoView__
#define __IF__AllianceInfoView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "AllianceInfo.h"
#include "DefaultTableViewDelegate.h"
#include "AllianceEventInfo.h"

class AllianceInfoView: public PopupBaseView
,public CCBSelectorResolver
 
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static AllianceInfoView* create(AllianceInfo* info);
    AllianceInfoView(AllianceInfo* info):m_info(info){};
    virtual ~AllianceInfoView(){};
    
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    
    void updateInfo();
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    void saveSuccess();
    void initFun();
    void updateAnnounce(CCObject* param);
    void showAnnounce(std::string announce);
    void sortData();
    void getEvents();
    void updateAllianceEvent(CCObject* param);
    void callMore();
    void getCallData(CCObject* param);
    void playApplyTipAnim();
    void playApplyTipAnimFinish();
    
    void onSaveClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onEditClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickFlag(CCObject * pSender, Control::EventType pCCControlEvent);
    void changeAllicaneIcon(CCObject* param);
    
    void onArrowBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onTipBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    void delayShowAnnounce(float t);
    void onClickEvent(CCNode* p,void* p1);
    void clickEventCell(CCObject* sender,void* pData);
    void addLoadingAni();
    void removeLoadingAni(CCObject* p = NULL);
    void showFunOrEvent(bool showFun);
    void updateApplyFlag(CCObject* param);
    CCSafeObject<CCLabelIFTTF> m_leaderTxt;
    CCSafeObject<CCLabelIF> m_powerTxt;
    CCSafeObject<CCLabelIF> m_membersTxt;
    CCSafeObject<CCLabelIFTTF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_language;

    CCSafeObject<CCNode> m_announceNode;
    CCSafeObject<CCEditBox> m_announceEditBox;
    CCSafeObject<CCScale9Sprite> m_viewBg;
    CCSafeObject<CCScale9Sprite> m_eventListBg;
    CCSafeObject<CCNode> m_allianceIcon;
    CCSafeObject<CCNode> m_funList;
    CCSafeObject<CCNode> m_clickNode;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCControlButton> m_btnArrow;
    CCSafeObject<CCNode> m_EditNode;
    CCSafeObject<CCNode> m_allianceIconClickNode;
    CCSafeObject<CCNode> m_eventNode;
    CCSafeObject<CCLabelIFTTF> m_testTxt;
    
    //new
    CCSafeObject<CCNode> m_btnNode;
    CCSafeObject<CCSprite> m_mailSpr;
    CCSafeObject<CCSprite> m_inviteSpr;
    CCSafeObject<CCSprite> m_memberSpr;
    CCSafeObject<CCSprite> m_managerSpr;
    CCSafeObject<CCLabelIF> m_mailTxt;
    CCSafeObject<CCLabelIF> m_inviteTxt;
    CCSafeObject<CCLabelIF> m_memberTxt;
    CCSafeObject<CCLabelIF> m_managerTxt;
    
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCSprite> m_loadingIcon;
    CCSafeObject<CCSprite> m_applyFlag;
    CCSafeObject<CCNode> m_applyNode;
    CCSafeObject<CCLabelIF> m_applyTipNum;
    CCSafeObject<AllianceInfo> m_info;
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCArray> m_srcData;
    CCSafeObject<CCArray> m_dataCells;
    
    //
    CCSafeObject<CCLabelIF> m_titleTxt;
    
    WaitInterface* m_waitInterface;
    CCPoint m_touchPos;
    int m_eventNum;
    bool m_isMove;
    bool m_init;
    int m_add;
    CCPoint m_applyNodePt;
};

class AllianceFunCell : public Layer
 
, public CCBSelectorResolver
, public CCBMemberVariableAssigner

{
public:
    static AllianceFunCell *create(std::string title,AllianceInfo* info,std::string icon,CCNode* clickArea,int index=0);
    void clickHandle(CCObject *pSender, CCControlEvent event);
private:
    AllianceFunCell(std::string title,AllianceInfo* info,std::string icon,CCNode* clickArea,int index) : m_titleStr(title),m_info(info),m_icon(icon),m_index(index),m_clickArea(clickArea){};
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    void updateNum(CCObject* param);
    std::string m_titleStr;
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCNode> m_renderBG;
    CCSafeObject<CCNode> m_helpNumNode;
    CCSafeObject<CCLabelIF> m_helpNumTxt;
    
    CCNode* m_clickArea;
    AllianceInfo* m_info;
    int m_index;
    std::string m_icon;
    CCPoint m_movePoint;
};

class AllianceEventCell :  public CCTableViewCell
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static AllianceEventCell *create(AllianceEventInfo* info,AllianceEventInfo* lastInfo=NULL);
    void setData(AllianceEventInfo* info,AllianceEventInfo* lastInfo=NULL);
    int getMaxHeight();
    void showCell(AllianceEventInfo* info);
    bool checkIsEventInSide(cocos2d::CCTouch *pTouch);
    CCSafeObject<CCScale9Sprite> m_clickBg;
    CCSafeObject<AllianceEventInfo> m_info;
private:
    AllianceEventCell(AllianceEventInfo* info,AllianceEventInfo* lastInfo) :m_info(info),m_lastInfo(lastInfo){};
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    CCSafeObject<CCNode> m_moveNode;
    CCSafeObject<CCNode> m_icon;
    CCSafeObject<CCScale9Sprite> m_cellBg;
    CCSafeObject<CCSprite> m_arrow;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCLabelIFTTF> m_contentTxt;
    CCSafeObject<CCNode> m_node1;
    CCSafeObject<CCNode> m_node2;
    
    AllianceEventInfo* m_lastInfo;
};

#endif /* defined(__IF__AllianceInfoView__) */
