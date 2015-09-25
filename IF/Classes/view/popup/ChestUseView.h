//
//  ChestUseView.h
//  LastEmpire
//
//  Created by zhangjun on 2015-03-24 17:28:19.
//
//
		
#ifndef __LastEmpire__ChestUseView__
#define __LastEmpire__ChestUseView__
		
#include "CommonInclude.h"
#include "CCBExtension.h"
#include "CCLabelIF.h"
#include "PopupBaseView.h"
		
class ChestUseView :
public PopupBaseView,
//public cocos2d::CCTargetedTouchDelegate,
public CCTableViewDataSource,
public CCTableViewDelegate,
public CCBSelectorResolver,
public CCBMemberVariableAssigner
{
public:
    ChestUseView(CCDictionary *dict, bool isOnlyShow=false):m_dataDict(dict),m_dataArr(NULL){CCLOGFUNC();};
    virtual ~ChestUseView();
    virtual bool init();
    virtual void onExit();
    virtual void onEnter();
    static ChestUseView* create(CCDictionary *dict);
		
    bool onAssignCCBMemberVariable(CCObject * pTarget, const char* pMemberVariableName, CCNode * pNode);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    virtual SEL_CallFuncN onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName);
    
protected:
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void tableCellTouched(  cocos2d::extension::TableView* table, CCTableViewCell* cell);
    virtual CCSize cellSizeForTable(  cocos2d::extension::TableView *table);
    virtual CCTableViewCell* tableCellAtIndex(  cocos2d::extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(  cocos2d::extension::TableView *table);
    virtual cocos2d::CCSize tableCellSizeForIndex(  cocos2d::extension::TableView *table, ssize_t idx);
    
protected:
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){};
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    CCSafeObject<CCNode> m_sprBG;
    CCSafeObject<CCNode> m_subNode;
    CCSafeObject<CCNode> m_tableNode;
    CCSafeObject<CCLabelIF> m_titleLabel;
    
    CCSafeObject<CCNode> m_bgNode;
    
private:
    CCTableView* m_tabView;
    CCArray *m_dataArr;
    CCArray *m_saveDataArr;
    CCDictionary *m_eqNumDict;
    CCDictionary *m_dataDict;
};

///ChestRDView
class ChestRDView :
public PopupBaseView,
public CCTableViewDataSource,
public CCTableViewDelegate,
public CCBSelectorResolver,
public CCBMemberVariableAssigner
{
public:
    ChestRDView(CCDictionary *dict):m_dataDict(dict),m_dataArr(NULL){CCLOGFUNC();};
    virtual ~ChestRDView();
    virtual bool init();
    virtual void onExit();
    virtual void onEnter();
    static ChestRDView* create(CCDictionary *dict);
    
    bool onAssignCCBMemberVariable(CCObject * pTarget, const char* pMemberVariableName, CCNode * pNode);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    virtual SEL_CallFuncN onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName);
    
protected:
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t  idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual cocos2d::CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    
protected:
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){};
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    CCSafeObject<CCNode> m_sprBG;
    CCSafeObject<CCNode> m_subNode;
    CCSafeObject<CCNode> m_tableNode;
    CCSafeObject<CCLabelIF> m_titleLabel;
    
    CCSafeObject<CCNode> m_bgNode;
    
private:
    CCTableView* m_tabView;
    CCArray *m_dataArr;
    CCArray *m_saveDataArr;
    CCDictionary *m_eqNumDict;
    CCDictionary *m_dataDict;
};

//end 3
		
#endif /* defined(__LastEmpire__ChestUseView__) */
		
