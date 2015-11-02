//
//  StoreBagView.h
//  IF
//
//  Created by lifangkai on 14-11-11.
//
//

#ifndef __IF__StoreBagView__
#define __IF__StoreBagView__
#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCSliderBar.h"
#include "CCTableViewTouchIFCell.h"
#include "NBSlider.h"

#define  STORE_BAG_SHOW_DES "store.bag.show.des"

class StoreBagView: public Layer
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate

{
public:
    static StoreBagView* create();
    static bool sortByUseOder(int itemId1, int itemId2);
    CCNode* getGuideNode(string _key);
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
    
    void refreshView();
    void refreashData(CCObject* obj);
    void refreashData();
    void sortDataByOrderNum();
    
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCArray>m_data;
    
    CCTableView* m_tabView;
    
    void generateData();
    int m_page;
    std::vector<int> m_curList;
    std::vector<int>* m_dataList;
    int m_cityLv;
    CCPoint m_startPoint;

    void playUseAllEffect();
    void finishEffect();
};
class StoreBagEffect :
public CCNode
,public CCBMemberVariableAssigner{
public:
    static StoreBagEffect* create();
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
protected:
    virtual bool init();
private:
    CCSafeObject<CCSprite> m_sprNewIcon;
    CCSafeObject<CCSprite> m_sprNewIcon1;
};
//...cell class
class StoreBagCell :
public CCBSelectorResolver
 
,public CCBMemberVariableAssigner
,public CCTableViewTouchIFCell
{
public:
    static StoreBagCell* create(string itemId, CCNode* touchNode);
    StoreBagCell():m_items(""),m_itemId(0),m_touchNode(NULL){};
    void setData(string itemId);
    int m_itemId;
    CCSafeObject<CCSprite> m_picBG1;
    CCSafeObject<CCControlButton> m_useBtn;
    
    void playUseEffect(CCPoint &pos);
private:
    bool init(string itemId,CCNode* touchNode);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void refreashData(CCObject* obj);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickUseBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickUseAllBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onYes();
    void onUseTool(bool useAll=false);
    void onUseToolEvent();
    void sureToUseTool();
    void onBoostBuild();
    void showDesByIndex(int index);
    CCSafeObject<CCControlButton> m_useAllBtn;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCNode> m_picNode1;
    CCSafeObject<CCNode> m_picNode2;
    CCSafeObject<CCNode> m_picNode3;
    CCSafeObject<CCNode> m_picNode4;
    CCSafeObject<CCNode> m_picNode5;
    CCSafeObject<CCLabelIF> m_numLabel1;
    CCSafeObject<CCLabelIF> m_numLabel2;
    CCSafeObject<CCLabelIF> m_numLabel3;
    CCSafeObject<CCLabelIF> m_numLabel4;
    CCSafeObject<CCLabelIF> m_numLabel5;
    
    CCSafeObject<CCSprite> m_picBG2;
    CCSafeObject<CCSprite> m_picBG3;
    CCSafeObject<CCSprite> m_picBG4;
    CCSafeObject<CCSprite> m_picBG5;
    CCSafeObject<CCSprite> m_arrowSpr;
    CCSafeObject<CCSprite> m_selectSpr;
    CCSafeObject<CCNode> m_itemNode1;
    CCSafeObject<CCNode> m_itemNode2;
    CCSafeObject<CCNode> m_itemNode3;
    CCSafeObject<CCNode> m_itemNode4;
    CCSafeObject<CCNode> m_itemNode5;
    CCSafeObject<CCNode> m_newNode[5];
    
    CCSafeObject<CCNode> m_itemsNode;
    CCSafeObject<CCNode> m_desNode;
    CCSafeObject<CCScale9Sprite> m_itemBg;
    
    CCNode* m_touchNode;
    string m_items;
    int m_a[5];
    int m_boostBid;
    bool m_clickInSide;
    int m_selectIndex1;
    CCPoint m_startPoint;
    WaitInterface* m_waitInterface;
    
    CCNode* getCurPicNode();
};

class ToolNumSelectView: public PopupBaseView
, public CCBSelectorResolver
  
, public CCBMemberVariableAssigner
, public CCEditBoxDelegate
{
public:
    static ToolNumSelectView* create(int itemid, int opFrom = 0);
    ToolNumSelectView(int itemid,int opFrom): m_itemId(itemid),m_opFrom(opFrom){};
    ~ToolNumSelectView(){};
    
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);
private:
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onAddClick(CCObject *pSender, CCControlEvent event);
    void onSubClick(CCObject *pSender, CCControlEvent event);
    void onUseClick(CCObject *pSender, CCControlEvent event);
//    void moveSlider(CCObject * pSender, Control::EventType pCCControlEvent);//fusheng d
    void moveSlider(Ref *pSender, NBSlider::EventType type);
    CCSafeObject<NBSlider> m_slider;
//    void refreshNumLabel(int);
    void setNumString();
    CCSafeObject<CCLabelIF> m_infoLabel;
//    CCSafeObject<CCSliderBar> m_slider;
    CCSafeObject<CCControlButton> m_useBtn;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_infoNode;
    CCSafeObject<CCNode> m_editNode;
    CCSafeObject<CCNode> m_barNode;
    CCSafeObject<CCControlButton> m_subBtn;
    CCSafeObject<CCControlButton> m_addBtn;
    CCSafeObject<CCEditBox> m_editBox;
    CCSafeObject<CCLabelIF> m_numMaxText;
    bool m_invalidSlider;
    int m_itemId;
    int m_numAll;
    int m_numCurrent;
    
    
    int m_opFrom;  // 0- store cell  1-gift cell
};

#endif /* defined(__IF__StoreBagView__) */
