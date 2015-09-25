//
//  PushSettingView.h
//  IF
//
//  Created by zhaohf on 14-4-4.
//
//

#ifndef __IF__PushSettingView__
#define __IF__PushSettingView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCTableViewTouchIFCell.h"
class PushSettingView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
//, public CCTouchDelegate
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static PushSettingView* create();
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual ~PushSettingView();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);

    void setDataList();

    CCSafeObject<CCScale9Sprite> m_popupBG;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCTableView> m_tabView;
    
    CCSafeObject<CCArray>m_data;
};

class PushCell :
public CCTableViewTouchIFCell,
public CCBSelectorResolver,
public CCBMemberVariableAssigner
//,public CCTargetedTouchDelegate
{
public:
    static PushCell* create(int index);
    void setData(int index);
    void showActivityDetail();
private:
    PushCell(int index){};
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(int index);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void refreshView();
    int m_index;
    CCPoint m_touchPoint;
    CCSafeObject<CCLabelIF> m_TitleText;
    CCSafeObject<CCLabelIF> m_desText;
    CCSafeObject<PushSettingInfo>m_info;

    CCSafeObject<CCSprite> m_touchBtnBg;
    CCSafeObject<CCSprite> m_touchBtn1;
    CCSafeObject<CCSprite> m_touchBtn;
    CCSafeObject<CCSprite> m_soundOnSpr;
    CCSafeObject<CCSprite> m_soundOffSpr;
};


#endif /* defined(__IF__PushSettingView__) */
