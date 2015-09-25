//
//  TerritoryWarehouseView.h
//  IF
//
//  Created by 〜TIAN~ on 15/7/8.
//
//

#ifndef __IF__TerritoryWarehouseView__
#define __IF__TerritoryWarehouseView__

#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCSliderBar.h"

class TerritoryWarehouseView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static TerritoryWarehouseView* create(int pointId,bool storage);
    
    TerritoryWarehouseView(int pointId,bool storage):m_warehousePtId(pointId),m_storage(storage){};
    virtual ~TerritoryWarehouseView(){};
    void updateInfo();
    void tradeSuccess(CCObject* p);
private:
    virtual void onEnter();
    virtual void onExit();
    
    void onDetailCallback(CCObject* obj);
    virtual bool init();
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){};
    
    void updateTradeData(bool isEnd=false);
    void updateCurrentLoad(CCObject* obj);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onCloseClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onBtnTradeClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void updateChangeStatus(float t);
    void playResourceAni();
    void getPlayerStorageLimit();
    void addLoadingAni();
    void removeLoadingAni(CCObject* p = NULL);
    CCSafeObject<CCLabelIF> m_wood;
    CCSafeObject<CCLabelIF> m_stone;
    CCSafeObject<CCLabelIF> m_iron;
    CCSafeObject<CCLabelIF> m_food;
    CCSafeObject<CCLabelIF> m_rateTxt;
    CCSafeObject<CCLabelIF> m_loadTxt;
    CCSafeObject<CCLabelIF> m_resTxt;
    CCSafeObject<CCLabelIF> m_infoTxt;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCControlButton> m_btnTrade;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScale9Sprite> m_viewBg;
    CCSafeObject<CCSprite> m_loadingIcon;
    CCSafeObject<CCLabelIF> m_cityNameTxt;
    CCSafeObject<CCLabelIF> m_tradeWood;
    CCSafeObject<CCLabelIF> m_tradeFood;
    CCSafeObject<CCLabelIF> m_tradeIron;
    CCSafeObject<CCLabelIF> m_tradeStone;
    CCSafeObject<CCLabelIF> m_tradePlayerTxt;
    CCSafeObject<CCNode> m_ironNode;
    CCSafeObject<CCNode> m_stoneNode;
    CCSafeObject<CCNode> m_coverNode1;
    CCSafeObject<CCNode> m_coverNode2;
    
    CCSafeObject<CCNode> m_funNode1;
    CCSafeObject<CCNode> m_funNode2;
    CCSafeObject<CCNode> m_buttomNode;
    CCSafeObject<CCNode> m_scrollNode;
    CCSafeObject<CCNode> m_ironTradeNode;
    CCSafeObject<CCNode> m_stoneTradeNode;
    
    CCSafeObject<CCNode> m_cityIcon;
    CCSafeObject<CCNode> m_cityIcon1;
    CCSafeObject<CCNode> m_warehouseIcon;
    CCSafeObject<CCNode> m_warehouseIcon1;
    CCSafeObject<CCNode> m_warehouseIcon2;
    CCSafeObject<CCNode> m_woodIcon;
    CCSafeObject<CCNode> m_foodIcon;
    CCSafeObject<CCNode> m_ironIcon;
    CCSafeObject<CCNode> m_stoneIcon;
    
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCArray> m_resList;
    int m_page;
    unsigned long m_lastTotalFood;
    unsigned long m_tradeToServerFood;
    unsigned long m_lastTrade;
    bool m_storage;
    int m_warehousePtId;
    unsigned long m_dayStorageLimit;
    unsigned long m_totalStorageLimit;
    unsigned long m_dayStorage;
    unsigned long m_totalStorage;
    unsigned long currentStorage;
    unsigned long lastStorage;
};

class TerritoryWarehouseCell :
public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static TerritoryWarehouseCell* create(unsigned long dayLimit,unsigned long totalLimit,int index,bool storage);
    TerritoryWarehouseCell(unsigned long dayLimit,unsigned long totalLimit,int index,bool storage):m_dayStorageLimit(dayLimit),m_totalStorageLimit(totalLimit),m_index(index),m_storage(storage){};
    unsigned long m_currentValue;
    unsigned long m_tradeValue;
    int m_index;
    void setData(unsigned long load);
    void resetData();
    CCSafeObject<CCSliderBar> m_slider;
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void sliderCallBack(CCObject*sender,CCControlEvent even);
    
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_valueTxt;
    CCSafeObject<CCNode> m_resNode;
    CCSafeObject<CCNode> m_sliderNode;
    CCSafeObject<CCNode> m_coverNode;
    CCSafeObject<CCLabelIF> m_coverTxt;
    
    TerritoryWarehouseView* m_warehouseView;
    bool m_invalidSliderMove;
    unsigned long m_dayStorageLimit;
    unsigned long m_totalStorageLimit;
    unsigned long m_limit;
    unsigned long m_load;
    bool m_storage;
};


#endif /* defined(__IF__TerritoryWarehouseView__) */
