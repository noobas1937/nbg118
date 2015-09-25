//
//  HowToPlayView.h
//  COK
//
//  Created by zhaohf on 14-7-24.
//
//

#ifndef IF_HowToPlayView_h
#define IF_HowToPlayView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"

class HowToPlayView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
 
{
public:
    static HowToPlayView* create();
    HowToPlayView():m_htpGroup(NULL),m_htpCount(0){};
    virtual ~HowToPlayView(){};
    
    virtual bool onTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void onTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void onTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    
protected:
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view);
//    virtual void scrollViewDidZoom(CCScrollView* view){};

    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
//    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell){};
//    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onCloseClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void HTPAnim(float dt);
    //CCSafeObject<CCControlButton> m_closeBtn;
    CCSafeObject<CCNode> m_list;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCSprite> m_bottom;
    CCSafeObject<CCLabelIFTTF> m_label;
    CCSafeObject<CCArray> m_cellSize;
    CCDictionary* m_htpGroup;
    int m_htpCount;
    CCSize winsize;
};

#endif /* defined(IF_HowToPlayView_h) */
