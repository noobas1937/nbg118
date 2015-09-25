//
//  ItemStatusView.h
//  IF
//
//  Created by ganxiaohua on 14-4-18.
//
//

#ifndef __IF__ItemStatusView__
#define __IF__ItemStatusView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"
#include "VipUtil.h"
#include "DefaultTableViewDelegate.h"

class ItemStatusView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
, public CCTableViewDataSource
, public DefaultTableViewDelegate
{
public:
    static ItemStatusView* create();
    ItemStatusView(){};
    ~ItemStatusView(){};
    void updateInfo(CCObject* p);
    
    void onEnterUpdate();
protected:
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    
private:
    virtual void onEnter();
    virtual void onExit();
    CCArray* getTypeArray(int type);
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCScale9Sprite> m_viewBg;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCTableView> m_tabView;
    
    vector<int> m_goodTypes;
    
};

class ItemStatusTypeCell :
//public CCTableViewCell
//public CCTargetedTouchDelegate
public cocosbuilder::CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCAniNode
{
public:
    static ItemStatusTypeCell* create(int type,CCNode* pNode);
    ItemStatusTypeCell(int type,CCNode* pNode):m_type(type),m_pNode(pNode){};
    void setType(int type);
    int m_type;
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void updateTime(float t);
    void resetTime(CCObject* param);
    void freshData();
    
    CCSafeObject<CCNode> m_receiveGlow;
    
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_descTxt;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCNode> m_barNode;
    CCSafeObject<CCNode> m_clickNode;
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCScale9Sprite> m_progress;
    CCSafeObject<CCScale9Sprite> m_cellBG;
    CCSafeObject<CCSprite> m_sprArrow;
    CCNode* m_pNode;
    CCSafeObject<CCSprite> m_barRight;
    
    bool m_moveFlag;
    int m_time;
    CCNode *m_headParticleNode;
};

class NewItemStatusTypeCell : public CCTableViewCell
{
public:
    static NewItemStatusTypeCell* create(int type,CCNode* pNode);
    NewItemStatusTypeCell():mCell(NULL){};
    ~NewItemStatusTypeCell(){};
    void setData(int type,CCNode* pNode);
    
    ItemStatusTypeCell* mCell;
private:
    bool init(int type,CCNode* pNode);
    
};
#endif /* defined(__IF__ItemStatusView__) */
