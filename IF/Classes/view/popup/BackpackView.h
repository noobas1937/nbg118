//
//  BackpackView.h
//  IF
//
//  Created by ganxiaohua on 14-7-24.
//
//

#ifndef __IF__BackpackView__
#define __IF__BackpackView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"
#include "DefaultTableViewDelegate.h"
#include "CCTableViewTouchIFCell.h"
class BackpackView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
, public cocos2d::extension::CCMultiColTableViewDataSource
, public cocos2d::extension::CCMultiColTableViewDelegate
{
public:
    static BackpackView* create(StoreType storeType = STORE_TYPE_STORE,
                             StoreResourceType storeResourceType = STORE_RESOURCE_TYPE_WAR);
    BackpackView(StoreType storeType = STORE_TYPE_STORE,
              StoreResourceType storeResourceType = STORE_RESOURCE_TYPE_WAR):m_type(storeType),m_page(storeResourceType),m_cityLv(0){};
    virtual ~BackpackView();
    void updateInfo();
    void showList(bool flag=true);
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
    
    void refreashData(CCObject* obj);
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickWarBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickSpdBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickResBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickOthBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCControlButton> m_warBtn;
    CCSafeObject<CCControlButton> m_spdBtn;
    CCSafeObject<CCControlButton> m_resBtn;
    CCSafeObject<CCControlButton> m_othBtn;
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelIF> m_msgLabel;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    
    CCSafeObject<CCMultiColTableView > m_tabView;
    
    int m_type;
    int m_page;
    std::vector<int> m_curList;
    std::vector<int>* m_dataList;
    int m_cityLv;
    int m_minNum;
};

class BackpackCell : public CCTableViewTouchIFCell
 
{
public:
    static BackpackCell *create(int itemId);
    void setData(int itemId);
    int m_itemId;
private:
    BackpackCell(int itemId) : m_itemId(itemId){};
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

    CCSafeObject<CCNode> m_flagNode;
    CCSafeObject<CCNode> m_numNode;
    CCSafeObject<CCLabelIF> m_numTxt;
    CCPoint m_touchPos;
};

#endif /* defined(__IF__BackpackView__) */
