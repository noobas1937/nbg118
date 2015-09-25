

#ifndef __IF__VipActiveView__
#define __IF__VipActiveView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"


struct VipBenefitStruct{
    string key;
    int value;
    int idx;
};

class VipActiveView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCTableViewDataSource
, public DefaultTableViewDelegate
{
public:
    static VipActiveView* create();
    VipActiveView();
    virtual ~VipActiveView();
protected:
    
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
private:
    
    void onCloseBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onActiveBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    

    
    CCSafeObject<CCLabelIF> m_titleLabel1;
    CCSafeObject<CCLabelIF> m_titleLabel2;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCControlButton> m_activeBtn;
    CCSafeObject<CCControlButton> m_cancelBtn;
    CCSafeObject<CCNode> m_nodeList;
    CCSafeObject<CCNode> m_nodeTouch;
    
    CCTableView* m_tableView;
    vector<VipBenefitStruct> m_data;
};


class VipActiveCell :
public CCTableViewCell
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static VipActiveCell* create(VipBenefitStruct &benefit);
    VipActiveCell();
    
    virtual ~VipActiveCell();
    void setData(VipBenefitStruct &benefit);
protected:
    bool init(VipBenefitStruct &benefit);
    virtual void onEnter();
    virtual void onExit();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
private:
    CCSafeObject<CCNode> m_sprPic;
    CCSafeObject<CCLabelIF> m_lblName;
    CCSafeObject<CCLabelIF> m_lblNum;
    CCSafeObject<CCSprite> m_sprAdd;
};
#endif 
