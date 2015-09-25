//
//  UseToolView.h
//  IF
//
//  Created by fubin on 14-3-14.
//
//

#ifndef IF_UseToolView_h
#define IF_UseToolView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"

class OneToolCell;

class UseToolView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
, public CCMultiColTableViewDataSource
, public CCMultiColTableViewDelegate
{
public:
    static UseToolView* create(string type, CCDictionary* dict,string title="");
    UseToolView():m_type(""){};
    virtual ~UseToolView();
    void updateInfo();
    
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
    
    virtual bool init(string type, CCDictionary* dict,string title="");
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void refreashData(CCObject* obj);
    
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCMultiColTableView > m_tabView;
    CCSafeObject<CCLabelIF> m_numTTF; 
    
    CCSafeObject<CCDictionary> m_dict;
    std::string m_type;
    std::vector<int> m_curList;
    
    void onTimer(float dt);
};

class OneToolCell : public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewCell
{
public:
    static OneToolCell* create(int itemId, string type, CCDictionary* dict, CCNode* listNode);
    OneToolCell():m_itemId(0),m_type(""),m_price(0),m_listNode(NULL),m_itemBuyCount(1){};
    void setData(int itemId, string type, CCDictionary* dict);
private:
    bool init(int itemId, string type, CCDictionary* dict, CCNode* listNode);
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickUseBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickBuyBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onUseTool1();
    void onUseTool();
    void onYes();
    void onYes1(CCObject* obj);
    void refreashData(CCObject* obj);
    
    CCSafeObject<CCControlButton> m_useBtn;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<CCNode> m_picNode;
    
    CCSafeObject<CCNode> m_buyNode;
    CCSafeObject<CCControlButton> m_buyBtn;
    CCSafeObject<CCLabelIF> m_btnMsgLabel;
    CCSafeObject<CCLabelIF> m_inBtnGoldNum;
    
    WaitInterface* m_waitInterface;
    
    CCSafeObject<CCDictionary> m_dict;
    int m_itemId;
    string m_type;
    float m_price;
    CCNode* m_listNode;
    
    int m_itemBuyCount;
};

#endif
