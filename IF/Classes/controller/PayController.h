//
//  PayController.h
//  IF
//
//  Created by 邹 程 on 13-11-22.
//
//

#ifndef __IF__PayController__
#define __IF__PayController__

#include "CommonInclude.h"

#define PAY_GOLD_PRODUCT_ID "todo"
#define CATCH_ITEM_ID "catch_item_id"
#define TSTORE_TXID_CACHE "tstore_txid_cache"
#define TSTORE_RECEIPT_CACHE "tstore_receipt_cache"
#define MSG_PAY_FAILED "pay_failed"

class PayController :public CCObject{
public:
    PayController();
    static PayController* getInstance();
    void setGoldExchangeSaleBoughtFlag(string itemId,bool isCallBack);//防止二次购买，目前只针对只能购买一次的礼包
    // type:
    // 1: pay not init
    // 2: pay failed
    // 3: pay canceled
    // 4: pay success
    void onPayCallback(int callbackType,string msg);
    void onPayCancel();
    
    void addOrderCache(string pf,string orderId,string sSignedData,string productId,string itemId,string productType="",string purchaseTime="");
    void checkHistoryPurchase();
    void checkPurchaseInfoList();
    bool checkPurchaseSuccessed(string orderId);
    void removeOrderCache(string orderId);
    void addToPurchaseIdList(string orderId);
    //buy for friends
    void callPaymentToFriend(GoldExchangeItem *m_dataItem,std::string touid);
    //pay
    void callPayment(GoldExchangeItem *goldExchangeItem,std::string fromView="GoldExchangeView");
    // ios pay
    void pay(string productId,string amount,string gross,string currency,string description,CCCallFunc* callback,bool buyMore=false);
    // server send goods
    void callPaymentIOSSendGoods(string orderId,string sSignedData,string productId,string itemId);
    void onCallPaymentServerCallback(CCObject* obj);
    void payForGold(int amount,string gross,string currency,CCCallFunc* callback = NULL);
    string getDollarText(string dollar,string productId);
    void setPayNumber(int productid,int number);
    void resetPayState(); //for ios choose num
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    void payGoogle(string goldType,CCCallFunc* callback = NULL);
    void payAndroid(const string &pay_id,const string &gold_num,const string &get_gold_count,const string &userId);
    void payAndroidToUid(const string &pay_id,const string &gold_num,const string &get_gold_count,const string &userId,const string &toUID);
    void pay337(const string &pay_id,const string &gold_num,const string &get_gold_count,const string &userId, const string &channel);

    void removeWebView();
    void initGoogle();
    void callPaymentCheck(string orderId,string purchaseTime,string productId,string signData,string signature);
    void onCallPaymentCheckCallback(CCObject* obj);
    void callPaymentTstoreSendGoods(string txid,string receipt,string itemId);
    void callPaymentAmazonSendGoods(string productType,string purchaseTime,string sku,string amazonUserId,string orderId,string itemId);
    void callPaymentNstoreSendGoods(string paymentSeq,string nonce,string itemId);
    void callPaymentCafebazaarSendGoods(string orderId,string purchaseToken,string sku,string itemId);
    void callPaymentTencentSendGoods(string orderId,string openid,string itemId);
    void onCallPaymentAndroidCallback(CCObject* obj);
#endif
    void showErrorDialog();
private:
    // state:
    // 0: idle
    // 1: waiting platform call back
    // 2: platform call back: waiting for server call back(success); do failed(failed)
    // 3: server call back, do finish
    CC_SYNTHESIZE(int, m_payState, PayState);
    CC_SYNTHESIZE_READONLY(string, m_orderId, OrderId);
    CC_SYNTHESIZE_READONLY(string, m_productId, ProductId);
    CC_SYNTHESIZE_READONLY(string, m_gross, Gross);
    CC_SYNTHESIZE_READONLY(string, m_amount, Amount);
    CC_SYNTHESIZE_READONLY(string, m_currency, Currency);
    CC_SYNTHESIZE_READONLY(string, m_description, Description);
    CC_SYNTHESIZE_READONLY(string, m_productType, ProductType);
    CC_SYNTHESIZE_READONLY(string, m_donateUID, DonateUID);
    CC_SYNTHESIZE_READONLY(int, m_payNumber, PayNumber);
    string m_itemId;
    void setItemId(string _itemId);
    
    int m_payCallbackType;
    
    CCSafeObject<CCCallFunc> m_callback;
    
    // platform independent
    void doPay();

    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    void doPayGoogle();
#endif
    
    // pay finish callback
    void payForGoldFinished();
    
    std::map<string,int> mCost;
};

#endif /* defined(__IF__PayController__) */
