//
//  VipMainView.h
//  IF
//
//  Created by chenliang on 14-3-17.
//
//

#ifndef __IF__VipMainView__
#define __IF__VipMainView__

#include "CommonInclude.h"
#include "PopupBaseView.h"


#include "CommonInclude.h"
#include "PopupBaseView.h"


class VipMainView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
  
//, public CCTableViewDataSource
{
public:
    static VipMainView* create();
    
    VipMainView();
    void refresh();
protected:
//    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
//    virtual CCSize cellSizeForTable(CCTableView *table);
//    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
//    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onAddPointBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onActivityPointBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCControlButton> m_addPointsBtn;
    CCSafeObject<CCControlButton> m_activateVipBtn;
    
    CCSafeObject<CCLabelIF> m_vipLvText;
    CCSafeObject<CCLabelIF> m_vipLeftTimeText;
    CCSafeObject<CCLabelIF> m_loginDayText;
    CCSafeObject<CCLabelIF> m_vipLoginPointText;
    CCSafeObject<CCLabelIF> m_vip1text;
    CCSafeObject<CCLabelIF> m_vip2text;
    CCSafeObject<CCLabelIF> m_vipPoint1Text;
    CCSafeObject<CCLabelIF> m_vipPoint2Text;
    
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCScale9Sprite> m_progressbar;
    
    CCSafeObject<CCNode> m_picHeadNode;
    CCSafeObject<CCNode> m_listContainer;
    
    CCTableView* m_tableView;
    CCPoint m_touchPoint;
    CCSafeObject<CCArray> m_data;
    CCDictionary* m_vipGroup;
    int m_vipLevel;
    
    int m_intialIndex;
    
    CCArray m_freeVipPointsCells;
    CCArray m_usedVipPointsCells;
    
};


#endif /* defined(__IF__VipMainView__) */
