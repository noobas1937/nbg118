//
//  KoreaPermitPopUPView.h
//  IF
//
//  Created by 李锐奇 on 15/3/9.
//
//

#ifndef __IF__KoreaPermitPopUPView__
#define __IF__KoreaPermitPopUPView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"

class KoreaTextTab : public CCNode
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static KoreaTextTab *create(const std::vector<std::string> v, CCNode *node);
    KoreaTextTab(const std::vector<std::string> v) : m_v(v){};
    ~KoreaTextTab();
    virtual void update(float time);
protected:
    // scrollview delegate
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    
private:
    bool init(CCNode *node);
    
    const std::vector<std::string> m_v;
    CCSafeObject<CCTableView> m_tabView;
    std::map<int, int> m_map;
    CCSafeObject<CCNode> m_scrollContainer;
};

class KoreaPermitPopUPView : public PopupBaseView
,public CCBMemberVariableAssigner
,public CCBSelectorResolver

{
public:
    static KoreaPermitPopUPView *create(std::string title, std::string title1, const std::vector<std::string> v1, std::string title2, const std::vector<std::string> v2);
    KoreaPermitPopUPView(const std::vector<std::string> v1, const std::vector<std::string> v2) : m_v1(v1), m_v2(v2){};
private:
    virtual bool init(std::string title, std::string title1, std::string title2);
    virtual void onExit();
    virtual void onEnter();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    void onBtn1Click(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBtn2Click(CCObject * pSender, Control::EventType pCCControlEvent);
    bool checkPermit();
    void doPermit();
    bool m_permit1;
    bool m_permit2;
    
    CCSafeObject<CCControlButton> m_btn1;
    CCSafeObject<CCControlButton> m_btn2;
    CCSafeObject<CCNode> m_viewContainer1;
    CCSafeObject<CCNode> m_viewContainer2;
    CCSafeObject<CCLabelIF> m_title;
    CCSafeObject<CCLabelIF> m_title1;
    CCSafeObject<CCLabelIF> m_title2;
    CCSafeObject<CCArray> m_arr;
    const std::vector<std::string> m_v1;
    const std::vector<std::string> m_v2;
};

class KoreaTextCell :
public CCTableViewCell
{
public:
    static KoreaTextCell *create(std::string str);
    KoreaTextCell(std::string str):m_str(str){};
    void setData(std::string str);
private:
    void refreshView();
    bool init();
    virtual void onEnter();
    virtual void onExit();
    
    CCSafeObject<CCLabelIF> m_text;
    std::string m_str;
};

#endif /* defined(__IF__KoreaPermitPopUPView__) */
