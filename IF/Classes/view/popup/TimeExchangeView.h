//
//  TimeExchangeView.h
//  IF
//
//  Created by xxrdsg on 14-12-22.
//
//

#ifndef __IF__TimeExchangeView__
#define __IF__TimeExchangeView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCMultiColTableView.h"

#define MSG_HIDE_RWDDES_NODE "msg.hide.rwddes.node"

class TimeExchangeView: public CCNode
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public cocos2d::extension::CCMultiColTableViewDataSource
, public cocos2d::extension::CCMultiColTableViewDelegate
{
public:
    static TimeExchangeView* create();
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
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onEnterFrame(float dt);
    void onClickTipBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void requestData(CCObject* obj);
    void resetTabviewPos();
    
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCControlButton> m_tipBtn;
    CCSafeObject<CCLabelIF> m_infoLabel;
    CCSafeObject<CCMultiColTableView> m_tabView;
    
    CCSafeObject<CCArray> m_data;
    
    void generateData(CCObject* p);
    bool m_st;
    
    //最多可获得多少等级的奖励
    int m_maxRwdLv;
};

//...cell class
class TimeExchangeCell :
public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewCell
{
public:
    static TimeExchangeCell* create(int itemIds);
    TimeExchangeCell():m_itemId(0){};
    void setData(int itemId);
    void cellTouchEnded(CCTouch* pTouch);
    int m_itemId;
private:
    bool init(int itemId);
    virtual void onEnter();
    virtual void onExit();
    
    void refreashData(CCObject* obj);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickRwdBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void hideDesNode(CCObject* param);
    void setDesNode(int pos);
    void setSprite();
    void onTimer(float dt);
    
    CCSafeObject<CCSprite> m_picBG1;
    CCSafeObject<CCSprite> m_picBG2;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_timeLabel;
    CCSafeObject<CCNode> m_picNode1;
    CCSafeObject<CCNode> m_picNode2;
    CCSafeObject<CCSprite> m_numBG1;
    CCSafeObject<CCSprite> m_numBG2;
    CCSafeObject<CCLabelIF> m_numLabel1;
    CCSafeObject<CCLabelIF> m_numLabel2;
    CCSafeObject<CCControlButton> m_rwdBtn;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCScale9Sprite> m_nameLabelBg;
    
    CCSafeObject<CCNode> m_desNode;
    CCSafeObject<CCLabelIF> m_desName;
    CCSafeObject<CCLabelIF> m_desLabel;
    
    CCSafeObject<CCNode> m_touchNode1;
    CCSafeObject<CCNode> m_touchNode2;
    map<int, string> m_rdMap;
    
    WaitInterface* m_waitInterface;
};

#endif /* defined(__IF__TimeExchangeView__) */
