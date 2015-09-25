//
//  AllianceWarResultView.h
//  IF
//
//  Created by ganxiaohua on 14-8-26.
//
//

#ifndef __IF__AllianceWarResultView__
#define __IF__AllianceWarResultView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "AllianceTeamInfo.h"
#include "YuanJunInfo.h"
#include "AllianceFightInfo.h"

class AllianceWarResultView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static AllianceWarResultView *create();
    AllianceWarResultView(){};
    ~AllianceWarResultView(){};
protected:
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    void getServerData(CCObject* data);
    void updateTime();
    void callMore();
    void addLoadingAni();
    void removeLoadingAni(CCObject* obj);
    
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_viewBg;
    CCSafeObject<CCLabelIF> m_typeTxt;
    CCSafeObject<CCLabelIF> m_attTxt;
    CCSafeObject<CCLabelIF> m_desTxt;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCSprite> m_loadingIcon;
    CCSafeObject<CCLabelIF> m_tip;
    
    CCSafeObject<CCTableView> m_tabView;
    CCPoint m_touchPos;
    bool m_isMove;
    bool m_first;
};

class AllianceWarResultCell : public CCTableViewCell
,public CCBSelectorResolver
,public CCBMemberVariableAssigner

{
public:
    static AllianceWarResultCell *create(AllianceFightInfo* info,int index);
    void setData(AllianceFightInfo* info,int index);
private:
    AllianceWarResultCell(AllianceFightInfo* info,int index):m_info(info),m_index(index){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);

    CCSafeObject<CCLabelIF> m_resultTxt1;
    CCSafeObject<CCLabelIF> m_resultTxt2;
    CCSafeObject<CCLabelIF> m_attTxt;
    CCSafeObject<CCLabelIF> m_defTxt;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCNode> m_lostNode;
    CCSafeObject<CCNode> m_winNode;
    CCSafeObject<AllianceFightInfo> m_info;
    int m_index;
    
};

#endif /* defined(__IF__AllianceWarResultView__) */
