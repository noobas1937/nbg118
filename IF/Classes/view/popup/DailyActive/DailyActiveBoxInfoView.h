//
//  DailyActiveBoxInfoView.h
//  IF
//
//  Created by chenfubi on 15/4/13.
//
//

#ifndef __IF__DailyActiveBoxInfoView__
#define __IF__DailyActiveBoxInfoView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"

class DailyActiveBoxInfoView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCTableViewDataSource
, public DefaultTableViewDelegate
//, public CCTouchDelegate
{
public:
	static DailyActiveBoxInfoView* create(string itemId, int index);
    DailyActiveBoxInfoView():m_itemId(""){};
	virtual ~DailyActiveBoxInfoView(){};
private:
	virtual bool init(string itemId, int index);
	virtual void onEnter();
    virtual void onExit();
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    
    void onOkClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void doCloseAction();
    void addParticle();
    void initBoxInfo();
    void reloadData(CCObject* pObj);
    void playGiftAnim(float dt);

private:
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_congratureNode;
    CCSafeObject<CCNode> m_particleNode1;
    CCSafeObject<CCNode> m_particleNode2;
    CCSafeObject<CCLabelIF> m_titleTTF;
    CCSafeObject<CCLabelIF> m_congratuTTF;
    CCSafeObject<CCControlButton> m_okBtn;
    
    CCSafeObject<CCArray> m_data;
    
    CCTableView* m_tableView;
    string m_itemId;
    int m_index;
    
};


//CLASS BoxInfoCell
class BoxInfoCell : public CCTableViewCell
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
	static BoxInfoCell* create(CCDictionary* dict);
	BoxInfoCell(){};
	virtual ~BoxInfoCell(){};
    
    void setData(CCDictionary* dict);
private:
	virtual bool init(CCDictionary* dict);
	virtual void onEnter();
    virtual void onExit();
//    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
//    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
private:
    CCSafeObject<CCLabelIF> m_infoTTF;
    CCSafeObject<CCLabelIF> m_numTTF;
    CCSafeObject<CCSprite> m_icon;
    CCSafeObject<CCSprite> m_colorBg;
    
};



//CLASS DailyActiveItemInfoView
class DailyActiveItemInfoView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
//, public CCTouchDelegate
{
public:
	static DailyActiveItemInfoView* create(string itemId);
	DailyActiveItemInfoView(){};
	virtual ~DailyActiveItemInfoView(){};
private:
	virtual bool init(string itemId);
	virtual void onEnter();
    virtual void onExit();
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onGoClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
private:
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCLabelIF> m_titleTTF;
    CCSafeObject<CCLabelIF> m_descripTTF;
    CCSafeObject<CCLabelIF> m_infoTTF1;
    CCSafeObject<CCLabelIF> m_infoTTF11;
    CCSafeObject<CCLabelIF> m_jinduTTF;
    CCSafeObject<CCControlButton> m_goBtn;
    CCSafeObject<CCSprite> m_infoImg;
    
    string m_itemId;
    
};


#endif /* defined(__IF__DailyActiveBoxInfoView__) */
