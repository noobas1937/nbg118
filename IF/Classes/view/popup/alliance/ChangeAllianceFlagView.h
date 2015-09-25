//
//  ChangeAllianceFlagView.h
//  IF
//
//  Created by ganxiaohua on 14-8-4.
//
//

#ifndef __IF__ChangeAllianceFlagView__
#define __IF__ChangeAllianceFlagView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "QuestController.h"
#include "CCMultiColTableView.h"
#include "QuestInfo.h"
#include "CCTableViewTouchIFCell.h"
class ChangeAllianceFlagView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
, public cocos2d::extension::CCMultiColTableViewDataSource
, public cocos2d::extension::CCMultiColTableViewDelegate
{
public:
    static ChangeAllianceFlagView *create();

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
    void saveSuccess(CCObject * p);
    void refreshView();
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCNode> m_viewBg;
    CCSafeObject<CCControlButton> m_changeBtn;
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCNode> m_goldNode;
    CCSafeObject<CCLabelIF> m_btnTxt;
    CCSafeObject<CCLabelIF> m_goldTxt;
    
    CCSafeObject<CCMultiColTableView> m_tabView;
    CCSafeObject<CCArray> m_cells;
    std::string m_currentPic;
};

class ChangeAllianceFlagCell : public CCTableViewTouchIFCell
,public CCBSelectorResolver
 
,public CCBMemberVariableAssigner

{
public:
    static ChangeAllianceFlagCell *create(std::string pic,CCNode* clickNode);
    void setData(std::string pic);
    CCSafeObject<CCControlButton> m_btnReward;
private:
    ChangeAllianceFlagCell(std::string pic,CCNode* clickNode):m_pic(pic),m_clickNode(clickNode){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    void clickHead(CCObject* data);
    
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCScale9Sprite> m_select;
    CCSafeObject<CCNode> m_head;
    CCSafeObject<CCNode> m_headNode;
    CCNode* m_clickNode;
    CCPoint  m_touchPos;
    int m_type;
    std::string m_pic;
};

#endif /* defined(__IF__ChangeAllianceFlagView__) */
