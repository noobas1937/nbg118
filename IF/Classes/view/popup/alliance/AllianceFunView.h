//
//  AllianceFunView.h
//  IF
//
//  Created by ganxiaohua on 15-1-6.
//
//

#ifndef __IF__AllianceFunView__
#define __IF__AllianceFunView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "QuestController.h"
#include "CCMultiColTableView.h"
#include "CCTableViewTouchIFCell.h"
class AllianceFunView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
, public cocos2d::extension::CCMultiColTableViewDataSource
, public cocos2d::extension::CCMultiColTableViewDelegate
{
public:
    static AllianceFunView *create();
    AllianceFunView(){};
    ~AllianceFunView(){};
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
    void onClickConfirm(CCObject *pSender, CCControlEvent event);
    void onClickCancel(CCObject *pSender, CCControlEvent event);
    
    void refreshFlag(CCObject* obj);
    
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCNode> m_icon;
    CCSafeObject<CCNode> m_viewBg;    
    CCSafeObject<CCSprite> m_modifySpr;
    
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCNode> m_fireNode1;
    CCSafeObject<CCNode> m_fireNode2;
    
    CCSafeObject<CCNode> m_funList;
    CCSafeObject<CCMultiColTableView> m_tabView;
    vector<int> m_buttonFuns;
    int m_index;
    int m_cellNum;
};

class AllianceFunBtnCell : public CCTableViewTouchIFCell
,public CCBSelectorResolver
 
,public CCBMemberVariableAssigner

{
public:
    static AllianceFunBtnCell *create(int index,CCNode* showArea);
    void setData(int index);
    CCSafeObject<CCControlButton> m_btnReward;
private:
    AllianceFunBtnCell(int index,CCNode* showArea):m_index(index),m_showArea(showArea){};
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
    
    void sendChatFirst();
    void exitAlliance(float _time);
    void leavelSuccess(CCObject* obj);
    void jieXianAlliance();
    void secondConfirm();
    void checkIfStorage(CCObject* param);
    CCSafeObject<CCNode> m_node1;
    CCSafeObject<CCNode> m_btnNode1;
    CCSafeObject<CCLabelIF> m_nameText1;
    CCSafeObject<CCNode> m_nodeClick1;
    
    CCNode* m_showArea;
    int m_index;
    CCPoint m_startPos;
};

#endif /* defined(__IF__AllianceFunView__) */
