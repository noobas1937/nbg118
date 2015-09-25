//
//  KingChangePalaceView.h
//  IF
//
//  Created by ganxiaohua on 15-1-4.
//
//

#ifndef __IF__KingChangePalaceView__
#define __IF__KingChangePalaceView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"

class KingChangePalaceView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
//, public CCTouchDelegate
,public CCTableViewDataSource
,public DefaultTableViewDelegate

{
public:
    static KingChangePalaceView* create(string palace="301");
    KingChangePalaceView(string palace):m_palace(palace){};
    ~KingChangePalaceView(){};

protected:
    // scrollview delegate
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    virtual void scrollViewDidScroll(CCScrollView* view);
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onResetPicClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void refreshView();
    void changePic();
    void callBackSuccess(CCObject* param);
    
    CCSafeObject<CCSprite> m_leftBg;
    CCSafeObject<CCSprite> m_rightBg;
    CCSafeObject<CCNode> m_viewBg;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCScale9Sprite> m_picCover;
    CCSafeObject<CCNode> m_scrollContainer;
    CCSafeObject<CCControlButton> m_resetPicBtn;
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCArray> m_data;

    void generateData();
    void setButtonState(int offX);
    int m_resIndex;
    int m_x;
    int m_count;
    string m_palace;
};


class PalaceIconCell :
public CCTableViewCell
{
public:
    static PalaceIconCell *create(std::string palaceIndex);
    PalaceIconCell(std::string palaceIndex) : m_palaceIndex(palaceIndex) {};
    void setData(std::string palaceIndex);
private:
    void refreshView();
    bool init();
    
    virtual void onEnter();
    virtual void onExit();
    CCSafeObject<CCNode> m_picNode;
    std::string m_palaceIndex;
};

#endif /* defined(__IF__KingChangePalaceView__) */
