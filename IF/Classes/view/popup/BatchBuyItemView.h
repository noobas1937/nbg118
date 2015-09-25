//
//  BatchBuyItemView.h
//  IF
//
//  Created by fubin on 14-4-21.
//
//

#ifndef IF_BatchBuyItemView_h
#define IF_BatchBuyItemView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"

class BatchItemCell;

class BatchBuyItemView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
, public CCMultiColTableViewDataSource
, public CCMultiColTableViewDelegate
{
public:
    static BatchBuyItemView* create(int toolId, int needNum);
    BatchBuyItemView():m_toolId(0){};
    virtual ~BatchBuyItemView(){};
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
    
    virtual bool init(int toolId, int needNum);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void refreashData(CCObject* p);
    
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCMultiColTableView > m_tabView;
    
    int m_toolId;
    int m_needNum;
    std::vector<int> m_numList;
    std::vector<float> m_priceList;
};

class BatchItemCell : public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewCell
{
public:
    static BatchItemCell* create(int itemId, int num, float price);
    BatchItemCell():m_itemId(0),m_num(0),m_price(0),m_waitInterface(NULL){};
    void setData(int itemId, int num, float price);
private:
    bool init(int itemId, int num, float price);
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickBuyBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onUseTool();
    void onRetBuyTool();
    
    CCSafeObject<CCControlButton> m_useBtn;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<CCNode> m_picNode;
    
    CCSafeObject<CCNode> m_buyNode;
    CCSafeObject<CCControlButton> m_buyBtn;
    CCSafeObject<CCLabelIF> m_btnMsgLabel;
    CCSafeObject<CCLabelIF> m_inBtnGoldNum;
    
    CCSafeObject<CCDictionary> m_dict;
    WaitInterface* m_waitInterface;
    int m_itemId;
    int m_num;
    float m_price;
};


#endif
