//
//  KingdomAppointView.h
//  IF
//
//  Created by ganxiaohua on 14-12-10.
//
//

#ifndef __IF__KingdomAppointView__
#define __IF__KingdomAppointView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCMultiColTableView.h"
#include "DefaultTableViewDelegate.h"
#include "KingdomExecCommand.h"
#include "KingdomOfficersCommand.h"
#include "OfficeInfo.h"
#include "HFHeadImgNode.h"
#include "CCTableViewTouchIFCell.h"
class KingdomAppointView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static KingdomAppointView *create(string playerId,string playerName);
    KingdomAppointView(string playerId,string playerName):m_playerId(playerId),m_playerName(playerName){};
    
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
protected:
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onClickTab1(CCObject *pSender, CCControlEvent event);
    void onClickTab2(CCObject *pSender, CCControlEvent event);
    void onClickAppointBtn(CCObject *pSender, CCControlEvent event);
    void onRemoveAppointedBtn(CCObject *pSender, CCControlEvent event);
    
    void clickCellHandle(CCObject* param);
    void getOfficersData(CCObject* data);
    void appointedSuccess(CCObject* param);
    void appointedFun();
    void removeAppointedSuccess(CCObject* param);
    void removeAppointedFun();
    void refreshData();
    void btnHandle();
    
    CCSafeObject<CCNode> m_viewBg;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCControlButton> m_tab1;
    CCSafeObject<CCControlButton> m_tab2;
    CCSafeObject<CCControlButton> m_appointBtn;
    CCSafeObject<CCControlButton> m_removeBtn;
    CCSafeObject<CCTableView> m_tabView;
    
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCArray> m_srcData;
    string m_playerId;
    string m_playerName;
    int m_currentIndex;
};

class KingdomAppointCell :  public CCTableViewTouchIFCell
 
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static KingdomAppointCell *create(OfficeInfo* info,CCNode* showArea);
    void setData(OfficeInfo* info);
private:
    KingdomAppointCell(OfficeInfo* info,CCNode* showArea):m_info(info),m_showArea(showArea){};
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void clickCellHandle(CCObject* param);
    void refreshCell(CCObject* param);
    
    CCSafeObject<CCNode> m_clickArea;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCSprite> m_selectNode;
    CCSafeObject<CCLabelIFTTF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_txt1;
    CCSafeObject<CCLabelIF> m_txtValue1;
    CCSafeObject<CCLabelIF> m_txt2;
    CCSafeObject<CCLabelIF> m_txtValue2;
    CCSafeObject<CCLabelIF> m_txt3;
    CCSafeObject<CCLabelIF> m_txtValue3;
    CCSafeObject<CCNode> m_tipNode1;
    CCSafeObject<CCNode> m_tipNode2;
    CCSafeObject<CCNode> m_tipNode3;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    CCNode* m_showArea;
    OfficeInfo* m_info;
    CCPoint m_startPos;
};
#endif /* defined(__IF__KingdomAppointView__) */
