//
//  FBHelpRecordView.h
//  IF
//
//  Created by ganxiaohua on 14-11-18.
//
//

#ifndef __IF__FBHelpRecordView__
#define __IF__FBHelpRecordView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"

class FBHelpRecordView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static FBHelpRecordView *create();
    FBHelpRecordView(){};
    ~FBHelpRecordView(){};
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
    void removeLoadingAni();
    
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_viewBg;
    CCSafeObject<CCLabelIF> m_rewardTitle;
    CCSafeObject<CCLabelIF> m_tip;
    CCSafeObject<CCSprite> m_loadingIcon;
    CCSafeObject<CCTableView> m_tabView;
    CCPoint m_touchPos;
};

class FBHelpRecordCell : public CCTableViewCell
,public CCBSelectorResolver
,public CCBMemberVariableAssigner

{
public:
    static FBHelpRecordCell *create(CCDictionary* info);
    void setData(CCDictionary* info);
private:
    FBHelpRecordCell(CCDictionary* info):m_info(info){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    CCSafeObject<CCLabelIFTTF> m_descTxt;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCDictionary> m_info;
};

#endif /* defined(__IF__FBHelpRecordView__) */
