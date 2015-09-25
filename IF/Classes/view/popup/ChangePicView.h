//
//  ChangePicView.h
//  IF
//
//  Created by ganxiaohua on 14-7-23.
//
//

#ifndef __IF__ChangePicView__
#define __IF__ChangePicView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "QuestController.h"
#include "CCMultiColTableView.h"
#include "QuestInfo.h"
#include "CCTableViewTouchIFCell.h"
class ChangePicView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
, public cocos2d::extension::CCMultiColTableViewDataSource
, public cocos2d::extension::CCMultiColTableViewDelegate
{
public:
    static ChangePicView *create();
    CCNode* getGuideNode(string _key);
    int getScrollPos();
    
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
    
    void refreshView();
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCNode> m_viewBg;
    CCSafeObject<CCControlButton> m_changeBtn;
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCMultiColTableView> m_tabView;
    CCSafeObject<CCArray> m_cells;
    std::string m_currentPic;
};

class ChangePicCell : public CCTableViewTouchIFCell
,public CCBSelectorResolver
 
,public CCBMemberVariableAssigner

{
public:
    static ChangePicCell *create(std::string pic);
    void setData(std::string pic);
    CCSafeObject<CCControlButton> m_btnReward;
private:
    ChangePicCell(std::string pic):m_pic(pic){};
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
    
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCNode> m_selectNode;
    CCSafeObject<CCNode> m_head;
    CCSafeObject<CCNode> m_headNode;
    CCSafeObject<CCNode> m_clickNode;
    bool m_isDraging;
    int m_type;
    std::string m_pic;
};

#endif /* defined(__IF__ChangePicView__) */
