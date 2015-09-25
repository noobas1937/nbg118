//
//  PaymentCommand.h
//  IF
//
//  Created by 邹 程 on 13-12-3.
//
//

#ifndef __IF__PaymentCommand__
#define __IF__PaymentCommand__

#include "CommandBase.h"
#define PAYMENT_COMMAND "pay"
#define PAYMENT_IOS_COMMAND "pay.ios"
#define PAYMENT_TSTORE_COMMAND "pay.tstore"
#define PAYMENT_AMAZON_COMMAND "pay.amazon"
#define PAYMENT_ANDROID_COMMAND "pay.android"

class PaymentCommand : public CommandBase {
public:
    PaymentCommand(string orderId,string purchaseTime,string productId,string signData,string signature,string itemId):
    m_orderId(orderId)
    ,CommandBase(PAYMENT_COMMAND)
    {
        m_itemId =itemId;
        putParam("orderId", CCString::create(orderId));
        putParam("productId", CCString::create(productId));
        putParam("purchaseTime", CCString::create(purchaseTime));
        putParam("signData", CCString::create(signData));
        putParam("signature", CCString::create(signature));
        putParam("itemId", CCString::create(itemId));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    string m_orderId;
    string m_itemId;
};

class PaymentIOSCommand : public CommandBase {
public:
    PaymentIOSCommand(string orderId,string sSignedData,string productId,string itemId,string toUID=""):
    m_orderId(orderId)
    ,CommandBase(PAYMENT_IOS_COMMAND)
    {
        m_itemId =itemId;
        putParam("orderId", CCString::create(orderId));//平台生成
        putParam("sSignedData", CCString::create(sSignedData));//平台验证
        putParam("productId", CCString::create(productId));//gold_1 gold_2 ...
        putParam("itemId", CCString::create(itemId));//9001 9902 ...
        if(toUID.empty()==false){
            putParam("toUID", CCString::create(toUID));//送礼包给某人
        }
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    string m_orderId;
    string m_itemId;
};

class PaymentTstoreCommand : public CommandBase {
public:
    PaymentTstoreCommand(string txid,string signdata,string itemId):
    m_orderId(txid)
    ,CommandBase(PAYMENT_TSTORE_COMMAND)
    {
        m_itemId = itemId;
        putParam("txid", CCString::create(txid));
        putParam("signdata", CCString::create(signdata));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    string m_orderId;
    string m_itemId;
};

class PaymentAmazonCommand : public CommandBase {
public:
    PaymentAmazonCommand(string productType,string purchaseTime,string sku,string amazonUserId,string orderId,string itemId,string toUser = ""):
    m_orderId(orderId)
    ,CommandBase(PAYMENT_AMAZON_COMMAND)
    {
        m_itemId = itemId;
        putParam("productType", CCString::create(productType));
        putParam("purchaseTime", CCString::create(purchaseTime));
        putParam("sku", CCString::create(sku));
        putParam("amazonUserId", CCString::create(amazonUserId));
        putParam("orderId", CCString::create(orderId));
        putParam("itemId", CCString::create(itemId));
        if(toUser.empty()==false){
            putParam("toUID", CCString::create(toUser));//送礼包给某人
        }
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    string m_orderId;
    string m_itemId;
};

class PaymentAndroidCommand : public CommandBase {
public:
    PaymentAndroidCommand(string orderId,string itemId):
    m_orderId(orderId)
    ,CommandBase(PAYMENT_ANDROID_COMMAND)
    {
        m_itemId = itemId;
        putParam("itemId", CCString::create(itemId));
        putParam("orderId", CCString::create(orderId));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    string m_orderId;
    string m_itemId;
};

#endif /* defined(__IF__PaymentCommand__) */
