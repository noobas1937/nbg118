//
//  StoreMallView.h
//  IF
//
//  Created by lifangkai on 14-11-11.
//
//

#ifndef __IF__StoreMallView__
#define __IF__StoreMallView__
#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCTableViewTouchIFCell.h"
#include "CCGallery.h"

#define  STORE_MALL_HIDE_DES "store.mall.hide.des"

class StoreMallView: public Layer
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
,public CCGalleryDelegate
{
public:
    static StoreMallView* create();
    static bool sortByUseOder(int itemId1, int itemId2);

public:
    virtual void slideBegan(CCGallery *gallery);
    virtual void slideEnded(CCGallery *gallery, CCGalleryItem *pGItem);
    virtual void selectionChanged(CCGallery *gallery, CCGalleryItem *pGItem);
    virtual void selectionDecided(CCGallery *gallery, CCGalleryItem *pGItem);
protected:
    
    // tao.yu change event
    void onChangeItem(int index);
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
    void getHotItems();
    void refreshView(CCObject* obj);
//    void onClickWarBtn(CCObject * pSender, Control::EventType pCCControlEvent);
//    void onClickBuffBtn(CCObject * pSender, Control::EventType pCCControlEvent);
//    void onClickResBtn(CCObject * pSender, Control::EventType pCCControlEvent);
//    void onClickOthBtn(CCObject * pSender, Control::EventType pCCControlEvent);
//    void onClickHotBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickWarBtn();
    void onClickBuffBtn();
    void onClickResBtn();
    void onClickOthBtn();
    void onClickHotBtn();
    void sortDataByOrderNum();
    void refreshTime(CCObject* param);
    void onTimer(float dt);
    string getTimeStr(int secs);
    
    CCSafeObject<CCControlButton> m_warBtn;
    CCSafeObject<CCControlButton> m_buffBtn;
    CCSafeObject<CCControlButton> m_resBtn;
    CCSafeObject<CCControlButton> m_othBtn;
    CCSafeObject<CCControlButton> m_hotBtn;
    CCSafeObject<CCSprite> m_sprHot;
    
    CCSafeObject<CCNode> m_leftNode;
    CCSafeObject<CCNode> m_rightNode;
    CCSafeObject<CCNode> m_btnPartNode;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCArray>m_data;
    CCSafeObject<CCNode> m_bgNode;
    
    CCSafeObject<CCNode> m_timeNode;
    CCSafeObject<CCLabelIF> m_timeTitleText;
    CCSafeObject<CCLabelIF> m_timeLeftText;
    
    CCSafeObject<CCLayer> m_galleryLayer;
    CCSafeObject<CCGallery> m_mallGallery;
    
    double m_refreshTime;
    double m_leftTime;
    
    CCTableView* m_tabView;
    
    void generateData();
    int m_page;
    bool m_isInit;
    std::vector<int> m_curList;
    std::vector<int>* m_dataList;
    int m_cityLv;
    CCPoint m_startPoint1;
    CCPoint m_startPoint2;
    CCPoint m_startPoint3;
    CCPoint m_startPoint4;
    CCPoint m_startPoint5;
    bool m_onTimer;
};

//...cell class
class StoreMallCell :
public CCBSelectorResolver
 
,public CCBMemberVariableAssigner
,public CCTableViewTouchIFCell
{
public:
    static StoreMallCell* create(int itemId, int itemId1, CCNode* touchNode);
    StoreMallCell();
    void setData(int itemId, int itemId1);
private:
    bool init(int itemId, int itemId1, CCNode* touchNode);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void refreashData(CCObject* obj);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickBuyBtn1(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickBuyBtn2(CCObject * pSender, Control::EventType pCCControlEvent);
    void onYes();
    void onCloseConfirm();
    void onBuyTool1(CCObject *ccObj = NULL);
    void onBuyTool2(CCObject *ccObj = NULL);
    void onRetBuyTool();
    void setDesNode(int itemId,int index);
    void setLeft(float dt);
    void setRight(float dt);
    void setLeftHot(bool ishot,int num);
    void setRightHot(bool ishot,int num);
    
    CCSafeObject<CCLabelIF> m_nameLabel1;
    CCSafeObject<CCLabelIF> m_nameLabel2;
    CCSafeObject<CCLabelIF> m_priceLabel1;
    CCSafeObject<CCLabelIF> m_priceLabel2;
    CCSafeObject<CCNode> m_picNode1;
    CCSafeObject<CCNode> m_picNode2;
    CCSafeObject<CCNode> m_itemNode1;
    CCSafeObject<CCNode> m_itemNode2;
    CCSafeObject<CCSprite> m_picBG1;
    CCSafeObject<CCSprite> m_picBG2;
    CCSafeObject<CCNode> m_desNode;
    CCSafeObject<CCLabelIF> m_desName;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCScale9Sprite> m_buyBtnSpr1;
    CCSafeObject<CCScale9Sprite> m_buyBtnSpr2;
    CCSafeObject<CCScale9Sprite> m_buyBtngraySpr1;
    CCSafeObject<CCScale9Sprite> m_buyBtngraySpr2;
    CCSafeObject<CCScale9Sprite> m_saleoutSpr1;
    CCSafeObject<CCScale9Sprite> m_saleoutSpr2;
    CCSafeObject<CCSprite> m_goldSpr1;
    CCSafeObject<CCSprite> m_goldSpr2;

    CCSafeObject<CCNode> m_itemHotNode1;
    CCSafeObject<CCSprite> m_hotSpr1;
    CCSafeObject<CCSprite> m_hotgraySpr1;
    CCSafeObject<CCSprite> m_hotNumSpr1;
    CCSafeObject<CCSprite> m_hotNumgraySpr1;
    CCSafeObject<CCLabelIF> m_hotdesText1;
    CCSafeObject<CCLabelIF> m_hotNum1Text;
    CCSafeObject<CCScale9Sprite> m_hotLineSpr1;
    CCSafeObject<CCLabelIF> m_hotpriceLabel1;

    CCSafeObject<CCNode> m_itemHotNode2;
    CCSafeObject<CCSprite> m_hotSpr2;
    CCSafeObject<CCSprite> m_hotgraySpr2;
    CCSafeObject<CCSprite> m_hotNumSpr2;
    CCSafeObject<CCSprite> m_hotNumgraySpr2;
    CCSafeObject<CCLabelIF> m_hotdesText2;
    CCSafeObject<CCLabelIF> m_hotNum2Text;
    CCSafeObject<CCScale9Sprite> m_hotLineSpr2;
    CCSafeObject<CCLabelIF> m_hotpriceLabel2;
    
    CCNode* m_touchNode;
    int m_itemId;
    int m_itemId1;
    int m_price;
    int m_boostBid;
    bool m_clickInSide;
    bool m_isClick;
    CCPoint m_startPoint;
    int m_num;
};




class StoreMallCellNew :
public CCBSelectorResolver

,public CCBMemberVariableAssigner
,public CCTableViewTouchIFCell
{
public:
    static StoreMallCellNew* create(int itemId,int index, CCNode* touchNode);
    StoreMallCellNew();
    void setData(int itemId);
private:
    bool init(int itemId, int index,CCNode* touchNode);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void refreashData(CCObject* obj);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickBuyBtn1(CCObject * pSender, Control::EventType pCCControlEvent);

    void onYes();
    void onCloseConfirm();
    void onBuyTool1(CCObject *ccObj = NULL);

    void onRetBuyTool();
    void setDesNode(int itemId,int index);
    void setLeft(float dt);

    void setLeftHot(bool ishot,int num);

    
    CCSafeObject<CCLabelIF> m_nameLabel1;

    CCSafeObject<CCLabelIF> m_priceLabel1;

    CCSafeObject<CCNode> m_picNode1;

    CCSafeObject<CCNode> m_itemNode1;

    CCSafeObject<CCSprite> m_picBG1;

    CCSafeObject<CCNode> m_desNode;
    CCSafeObject<CCLabelIF> m_desName;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCScale9Sprite> m_buyBtnSpr1;

    CCSafeObject<CCScale9Sprite> m_buyBtngraySpr1;

    CCSafeObject<CCScale9Sprite> m_saleoutSpr1;

    CCSafeObject<CCSprite> m_goldSpr1;

    
    CCSafeObject<CCNode> m_itemHotNode1;
    CCSafeObject<CCSprite> m_hotSpr1;
    CCSafeObject<CCSprite> m_hotgraySpr1;
    CCSafeObject<CCSprite> m_hotNumSpr1;
    CCSafeObject<CCSprite> m_hotNumgraySpr1;
    CCSafeObject<CCLabelIF> m_hotdesText1;
    CCSafeObject<CCLabelIF> m_hotNum1Text;
    CCSafeObject<CCScale9Sprite> m_hotLineSpr1;
    CCSafeObject<CCLabelIF> m_hotpriceLabel1;
    

    
    CCNode* m_touchNode;
    int m_itemId;

    int m_price;
    int m_boostBid;
    bool m_clickInSide;
    bool m_isClick;
    CCPoint m_startPoint;
    int m_num;
    int index;
};


class StoreMallCells: //fusheng 由一行改成一个个单独的 创建一个类管理一行3个个体

public CCTableViewTouchIFCell
{
public:
    static StoreMallCells* create(int itemId1,int itemId2,int itemId3, CCNode* touchNode);
   
    StoreMallCells();
    
    void setData(int itemId1,int itemId2,int itemId3);
    
    bool init(int itemId1,int itemId2,int itemId3, CCNode* touchNode);
private:
    int m_itemId1;
    int m_itemId2;
    int m_itemId3;
    
    StoreMallCellNew* cellLeft;
    StoreMallCellNew* cellCenter;
    StoreMallCellNew* cellRight;
    
};



class StoreMallCellTipNB :
public CCBSelectorResolver

,public CCBMemberVariableAssigner
,public Node
{
public:
    static StoreMallCellTipNB* create(int itemId);
    StoreMallCellTipNB();
private:
    bool init(int itemId);

    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    

    
    CCSafeObject<CCNode> m_desNode;
    CCSafeObject<CCLabelIF> m_desName;
    CCSafeObject<CCLabelIF> m_desLabel;
};
#endif /* defined(__IF__StoreMallView__) */
