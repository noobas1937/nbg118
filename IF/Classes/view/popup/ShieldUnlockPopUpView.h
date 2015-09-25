//
//  ShieldUnlockPopUpView.h
//  IF
//
//  Created by 李锐奇 on 14-2-8.
//
//

#ifndef __IF__ShieldUnlockPopUpView__
#define __IF__ShieldUnlockPopUpView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCMultiColTableView.h"
#include "HFHeadImgNode.h"

#define NUM_PER_COL 1.0
#define REFRESH_SHIELD_PANEL "refreshShieldPanel"

class ShieldUnlockPopUpView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
,public cocos2d::extension::CCMultiColTableViewDataSource
,public cocos2d::extension::CCMultiColTableViewDelegate

{
public:
    static ShieldUnlockPopUpView* create();
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){};
	virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual unsigned int numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);

    void onCloseBtnClick(CCObject *pSender, CCControlEvent event);
    void refreshView(CCObject* p);
    void loadDataBack();
    void getData();
    
    CCSafeObject<CCControlButton> m_closeBtn;
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCNode> m_container;
    CCSafeObject<CCScale9Sprite> m_BG;
    CCSafeObject<CCMultiColTableView> m_tabView;
};

class ShieldUnlockCellView :
public CCTableViewCell,
public CCBSelectorResolver,
public CCBMemberVariableAssigner
{
public:
    static ShieldUnlockCellView* create(ShieldInfo *info);
    void setCellData(ShieldInfo *info);

private:
    ShieldUnlockCellView(ShieldInfo *info) : m_info(info){};
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onUnBlockClick(CCObject *pSender, CCControlEvent event);
    ShieldInfo *m_info;
    
    CCSafeObject<CCLabelIF> m_power;
    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCNode> m_picContainer;
    CCSafeObject<CCControlButton> m_unBlockBtn;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
};

#endif /* defined(__IF__ShieldUnlockPopUpView__) */
