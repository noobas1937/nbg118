//
//  CountryChooseFlagView.h
//  IF
//
//  Created by ganxiaohua on 14-12-24.
//
//

#ifndef __IF__CountryChooseFlagView__
#define __IF__CountryChooseFlagView__
#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "QuestController.h"
#include "CCMultiColTableView.h"
#include "CCTableViewTouchIFCell.h"
#define SELECT_BANNER_CELL "select_banner_cell"

class CountryChooseFlagView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
, public cocos2d::extension::CCMultiColTableViewDataSource
, public cocos2d::extension::CCMultiColTableViewDelegate
{
public:
    static CountryChooseFlagView *create();
protected:
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
	virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual unsigned int numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onChangeClick(CCObject *pSender, CCControlEvent event);
    void onPicClick(CCObject *data);
    void confirmFun();
    void changeSuccess(CCObject *data);
    void getFlagSuccess(CCObject *data);
    
    void refreshView();
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCNode> m_viewBg;
    CCSafeObject<CCControlButton> m_changeBtn;
    CCSafeObject<CCMultiColTableView> m_tabView;
    CCSafeObject<CCArray> m_cells;
    std::string m_currentFlag;
    std::string m_userFlag;
};

class CountryFlagCell : public CCTableViewTouchIFCell
,public CCBSelectorResolver
 
,public CCBMemberVariableAssigner

{
public:
    static CountryFlagCell *create(std::string pic,CCNode* showArea);
    void setData(std::string pic);
    CCSafeObject<CCControlButton> m_btnReward;
private:
    CountryFlagCell(std::string pic,CCNode* showArea):m_pic(pic),m_showArea(showArea){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    void changePic();
    void changeSuccess();
    void clickHead(CCObject* data);
    
    CCSafeObject<CCLabelIFTTF> m_nameTxt;
    CCSafeObject<CCNode> m_selectNode;
    CCSafeObject<CCNode> m_head;
    CCSafeObject<CCSprite> m_headNode;
    CCSafeObject<CCNode> m_clickNode;
    CCNode* m_showArea;
    bool m_isDraging;
    int m_type;
    std::string m_pic;
};

#endif /* defined(__IF__CountryChooseFlagView__) */
