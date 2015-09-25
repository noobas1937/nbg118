//
//  SocialPopUpView.h
//  IF
//
//  Created by ganxiaohua on 14-10-11.
//
//

#ifndef __IF__SocialPopUpView__
#define __IF__SocialPopUpView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"

class SocialPopUpView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCTableViewDataSource
, public DefaultTableViewDelegate
{
public:
    CREATE_FUNC(SocialPopUpView);
    
    SocialPopUpView();
    ~SocialPopUpView();
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell){};
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell){};
    
private:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    void test(CCObject* param);
    
    CCSafeObject<CCScale9Sprite> m_background;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCTableView> m_tableView;
    CCSafeObject<CCArray> m_datas;
};



class SocialBtnCell :
public CCTableViewCell
 
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    CREATE_FUNC(SocialBtnCell);
    
    SocialBtnCell(){};
    
    ~SocialBtnCell()
    {
    }
    void setData(int index);
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    
    
    void onGoBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCNode> m_icon;
    CCSafeObject<CCLabelIF> m_descTxt;
    CCSafeObject<CCControlButton> m_goBtn;
    int m_index;
};

#endif /* defined(__IF__SocialPopUpView__) */
