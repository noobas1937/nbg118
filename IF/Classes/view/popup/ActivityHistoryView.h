//
//  ActivityHistoryView.h
//  IF
//
//  Created by ganxiaohua on 14-8-26.
//
//

#ifndef __IF__ActivityHistoryView__
#define __IF__ActivityHistoryView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "AllianceFightInfo.h"

class ActivityHistoryView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static ActivityHistoryView *create(int type=0);
    ActivityHistoryView(int type):m_type(type),m_needclose(false){};
    ~ActivityHistoryView(){
    };
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
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void onCloseClick(CCObject *pSender);
    
    void getServerData(CCObject* data);
    void getTopHistoryData(CCObject* data);
    void getTopAllianceRankData(CCObject* data);
    void getTopUserRankData(CCObject* data);
    void getCKFData(CCObject* data);
    void addLoadingAni();
    void removeLoadingAni(CCObject* obj);
    void reFresh();
    bool m_needclose;
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCLabelIF> m_tip;
    CCSafeObject<CCNode> m_viewBg;
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCSprite> m_loadingIcon;
    int m_type;//0, 活动事件排名，1最强领主，2联盟活动联盟排名，3联盟活动个人排名
};

class ActivityHistoryCell : public CCTableViewCell
,public CCBSelectorResolver
,public CCBMemberVariableAssigner

{
public:
    static ActivityHistoryCell *create(CCDictionary* info,int type=0);
    void setData(CCDictionary* info);
private:
    ActivityHistoryCell(CCDictionary* info,int type):m_info(info),m_type(type){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onClickHistory(CCObject *pSender, CCControlEvent event);
    
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCLabelIF> m_allianceTxt;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCNode> m_line;
    CCSafeObject<CCNode> m_renderBg1;
    CCSafeObject<CCNode> m_renderBg2;
    CCSafeObject<CCNode> m_rankNode;
    CCSafeObject<CCNode> m_allianceNode;
    CCSafeObject<CCNode> m_titleNode;
    CCSafeObject<CCNode> m_rNode;
    CCSafeObject<CCControlButton> m_btnHistory;
    CCDictionary* m_info;
    int m_index;
    int m_type;
    
};
#endif /* defined(__IF__ActivityHistoryView__) */

