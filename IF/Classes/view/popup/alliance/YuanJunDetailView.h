//
//  YuanJunDetailView.h
//  IF
//
//  Created by ganxiaohua on 14-8-12.
//
//

#ifndef __IF__YuanJunDetailView__
#define __IF__YuanJunDetailView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "DefaultTableViewDelegate.h"
#include "AllianceInfo.h"
#include "YuanJunInfo.h"
#include "CCTableViewTouchIFCell.h"
class YuanJunDetailView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static YuanJunDetailView* create();
    YuanJunDetailView(){};
    virtual ~YuanJunDetailView(){};
    void updateInfo();
    
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    void onClickResult(CCObject * pSender, CCControlEvent pCCControlEvent);
    void saveSuccess();
    void cellClick(CCObject* param);
    void getServerData(CCObject* data);
    void removeOneCell(CCObject* param);
    
    CCSafeObject<CCControlButton> m_btnResult;
    CCSafeObject<CCNode> m_bottomNode;

    CCSafeObject<CCLabelIF> m_numTxt;
    CCSafeObject<CCLabelIF> m_tip;
    CCSafeObject<CCScale9Sprite> m_viewBg;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_clickNode;
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCArray> m_data;
    int m_maxSoldier;
    int m_currentSoldier;
};

class YuanJunDetailCell : public CCTableViewTouchIFCell
,public CCBSelectorResolver
 
,public CCBMemberVariableAssigner
{
public:
    static YuanJunDetailCell *create(YuanJunInfo* info);
    void setData(YuanJunInfo* info);
    CCSafeObject<CCControlButton> m_btnReward;
private:
    YuanJunDetailCell(YuanJunInfo* info):m_info(info){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    void onSendHomeClick(CCObject *pSender, CCControlEvent event);
    
    void changePic();
    void changeSuccess();
    void clickHead(CCObject* data);
    void callBack(CCObject* data);
    
    CCSafeObject<CCNode> m_moveNode;
    CCSafeObject<CCLabelIF> m_playerName;
    CCSafeObject<CCLabelIF> m_powerTxt;
    CCSafeObject<CCLabelIF> m_numTxt;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCNode> m_head;
    CCSafeObject<CCNode> m_icon;
    CCSafeObject<CCNode> m_arrow;
    CCSafeObject<CCNode> m_clickNode;
    CCSafeObject<CCControlButton> m_sendHomeBtn;
    CCSafeObject<CCNode> m_soldierNode;
    YuanJunInfo* m_info;
    CCPoint m_touchPos;
};

class YuanJunSoldierCell : public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static YuanJunSoldierCell *create(CCDictionary* info);
    
    CCSafeObject<CCSprite> m_bg;
    
private:
    YuanJunSoldierCell(CCDictionary* info):m_info(info){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_LvTxt;
    CCSafeObject<CCLabelIF> m_numTxt;
    CCSafeObject<CCNode> m_icon;
    CCDictionary* m_info;
};
#endif /* defined(__IF__YuanJunDetailView__) */
