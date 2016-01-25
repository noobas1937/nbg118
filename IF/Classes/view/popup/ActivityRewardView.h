//
//  ActivityRewardView.h
//  IF
//
//  Created by ganxiaohua on 14-9-2.
//
//

#ifndef __IF__ActivityRewardView__
#define __IF__ActivityRewardView__
#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "AllianceFightInfo.h"
#include "ActivityRankReward.h"

class ActivityRewardView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static ActivityRewardView *create(CCArray* data,int type=0);
    ActivityRewardView(CCArray* data,int type):m_data(data),m_type(type),m_needclose(false){};
    ~ActivityRewardView(){};
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
    bool m_needclose;
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_viewBg;
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCLabelIF> m_titleTxt;
    int m_type;
    
};

class ActivityRewardCell : public CCTableViewCell
,public CCBSelectorResolver
,public CCBMemberVariableAssigner

{
public:
    static ActivityRewardCell *create(AllianceFightInfo* info,int index);
    void setData(AllianceFightInfo* info,int index);
private:
    ActivityRewardCell(AllianceFightInfo* info,int index):m_info(info),m_index(index){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    CCSafeObject<CCLabelIF> m_txt1;
    CCSafeObject<CCLabelIF> m_txt2;
    CCSafeObject<CCScale9Sprite> m_bg1;
    CCSafeObject<CCScale9Sprite> m_bg2;
    CCSafeObject<AllianceFightInfo> m_info;
    int m_index;
    
};
#endif /* defined(__IF__ActivityRewardView__) */
