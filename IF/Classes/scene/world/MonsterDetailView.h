//
//  MonsterDetailView.h
//  IF
//
//  Created by 邹 程 on 14-4-14.
//
//

#ifndef __IF__MonsterDetailView__
#define __IF__MonsterDetailView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "WorldController.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"

class MonsterDetailView : public PopupBaseView
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
//, public CCTouchDelegate

{
    
public:
    
    static MonsterDetailView* create(unsigned int index) {
        auto ret = new MonsterDetailView();
        if (ret->init(index)) {
            ret->autorelease();
        } else {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
    
    CCNode* getGuideNode(string _key);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    
    void requestDetail();
    void onDetailCallback(CCObject* obj);
    
    void onClickButton1(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickButton2(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickButton3(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickButton4(CCObject * pSender, Control::EventType pCCControlEvent);
//    void onCloseClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void openMarchPanel(int index);
    
    void onEnter();
    void onExit();
    void updateView(CCObject* obj);
    void updateViewTime(float dt);
    bool init(unsigned int index);
private:
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

    CCSafeObject<CCControlButton> m_button1;
    CCSafeObject<CCControlButton> m_button2;
    CCSafeObject<CCControlButton> m_button3;
    CCSafeObject<CCControlButton> m_button4;
    CCSafeObject<CCLabelIF> m_progressLabel;
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCLabelIF> m_hintText;
    CCSafeObject<CCScale9Sprite> m_bg;

    void addButton();
    void setButtonState();
    unsigned int m_tileIndex;
    bool m_isInUpdate;
};

class MonsterAttackDetailView : public PopupBaseView
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
,public CCTableViewDataSource
,public DefaultTableViewDelegate

{
public:
    static MonsterAttackDetailView *create(unsigned int idx);
    int getIndex();
    
protected:
    // scrollview delegate
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);

private:
    bool init(unsigned int idx);
    void onEnter();
    void onExit();
    void refreshMaze();
    void refreshFlag();
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);

    void onReturnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onReportDetailClick(CCObject * pSender, Control::EventType pCCControlEvent);

    void confirmReturn();
    void requestDetail();
    void updateViewTime(float dt);
    void onDataBack(CCObject *obj);
    void initView(CCObject* p = NULL);
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCScale9Sprite> m_cdProgress;
    CCSafeObject<CCNode> m_scrollContainer;
    CCSafeObject<CCNode> m_downNode;
    CCSafeObject<CCNode> m_upNode;
    CCSafeObject<CCNode> m_bgContainer;
    CCSafeObject<CCLabelIF> m_progressLabel;
    CCSafeObject<CCControlButton> m_returnBtn;
    CCSafeObject<CCLabelIF> m_timeLabel;
    CCSafeObject<CCTableView> m_tabView;

    CCLabelIF *m_timeToNext;
    unsigned int m_idx;
};

class TextCell :
public CCTableViewCell
{
public:
    static TextCell *create(std::string str, bool isLast = false);
    TextCell(std::string str, bool isLast):m_str(str), m_isLast(isLast){};
    void setData(std::string str, bool isLast = false);
    void update(float time);
private:
    void refreshView();
//    void update(float time);
    bool init();
    virtual void onEnter();
    virtual void onExit();
    
    CCSafeObject<CCLabelIF> m_text;
    std::string m_str;
    bool m_isLast;
    int dotCount;
};

#endif /* defined(__IF__MonsterDetailView__) */
