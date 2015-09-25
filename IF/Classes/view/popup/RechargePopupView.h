//
//  RechargePopupView.h
//  IF
//
//  Created by 李锐奇 on 15/1/20.
//
//

#ifndef __IF__RechargePopupView__
#define __IF__RechargePopupView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCBExtension.h"
#include "CheckBox.h"
#include "CCMultiColTableView.h"

static const map<int, string> areas = {
    pair<int, string>(1, "paypal,PayPal"),
    pair<int, string>(2, "skrill,Skrill"),
//    pair<int, string>(3, "paysafecard,Paysafecard"),
    pair<int, string>(3, "paymaster,Paymaster")

//    pair<int, string>(4, "bocompra,Bocompra"),
//    pair<int, string>(5, "Mol,Mol"),
//    pair<int, string>(7, "onecard,Onecard"),
//    pair<int, string>(8, "cashu,Cashu"),
//    pair<int, string>(9, "paysbuy,Paysbuy"),
//    pair<int, string>(10, "Ycoin,Ycoin"),
//    pair<int, string>(11, "alipay,alipay"),
//    pair<int, string>(12, "tenpay,tenpay"),
//    pair<int, string>(13, "mycard,Mycard"),
//    pair<int, string>(14, "gash,Gash"),
//    pair<int, string>(15, "dotpay,Dotpay"),
//    pair<int, string>(16, "nganluong,Nganluong"),
//    pair<int, string>(17, "paydibs,Paydibs")
};

class Recharge : public CCTableViewCell{
public:
    CREATE_FUNC(Recharge);
    void setData(std::string channel, std::string name);
    std::string getChannel();
private:
    void refreshView();
    virtual bool init();
    std::string m_channel;
    std::string m_name;
};

class RechargePopupView : public PopupBaseView
,public CCBMemberVariableAssigner
, public CCTableViewDataSource
, public CCTableViewDelegate
{
public:
    static RechargePopupView *create(GoldExchangeItem *item, std::string uid);
    
protected:
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void tableCellTouched(  cocos2d::extension::TableView* table, CCTableViewCell* cell);
    virtual cocos2d::CCSize tableCellSizeForIndex(  cocos2d::extension::TableView *table, ssize_t idx);
    virtual cocos2d::CCSize cellSizeForTable(  cocos2d::extension::TableView *table);
    virtual CCTableViewCell* tableCellAtIndex(  cocos2d::extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(  cocos2d::extension::TableView *table);
    
private:
    RechargePopupView(GoldExchangeItem *item, std::string uid) : m_item(item), m_uid(uid){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCScale9Sprite> m_bg1;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCTableView> m_tabView;
    GoldExchangeItem *m_item;
    std::string m_uid;
};

#endif /* defined(__IF__RechargePopupView__) */
