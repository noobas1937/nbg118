//
//  GoldExchangeItem.h
//  IF
//
//  Created by wangzhenlei on 13-12-17.
//
//

#ifndef __IF__GoldExchangeItem__
#define __IF__GoldExchangeItem__

#include "cocos2d.h"
USING_NS_CC;
using namespace std;

class GoldExchangeItem{
public:
    GoldExchangeItem(CCDictionary* dict);
    GoldExchangeItem() :
    id(""),
    type(""),
    gold_doller(""),
    dollar(""),
    name_doller(""),
    gift(""),
    item(""),
    percent(""),
    oldprice(""),
    popup_image(""),
    name(""),
    product_id(""),
    start(0),
    end(0),
    time(0),
    bought(false),
    popup(0),
    price(0),
    isPayCallBack(false),
    md5(""),
    equipment(""),
    send_mail(false),
    buyMore(false)
    {};
    // item id:
    // 9000 ~ 9004
    std::string id;
    std::string type;//type 1：普通 2：促销
    std::string gold_doller;
    std::string dollar;
    std::string name_doller;
    std::string gift;
    std::string item;
    std::string percent;
    std::string oldprice;
    std::string popup_image;
    std::string name;   //
    std::string product_id;
    bool buyMore;
    int start;
    int end;
    int time;
    bool bought;
    int popup;
    int price;
    bool isPayCallBack;
    bool send_mail;
    std::string md5;
    std::string equipment;
    std::string toString();
};

class MonthCardInfo{
public:
    MonthCardInfo(CCDictionary* dict);
    MonthCardInfo():mItemId(""),mTimes(0),mAccepted(false),mCount(0),
    rewards(NULL),mBuy(false){
        
    }
    CC_SYNTHESIZE_READONLY(std::string, mItemId, ItemId);
    CC_SYNTHESIZE_READONLY(int, mTimes, Times);
    CC_SYNTHESIZE_READONLY(bool, mAccepted, Accepted);
    CC_SYNTHESIZE_READONLY(int, mCount, Count);
    CC_SYNTHESIZE_READONLY(bool, mBuy, Buy);
    CCArray* rewards;
    void buyCard();
    void resetCard();
    void addTime();
    bool canReward();
    bool isAlreadyReward();
    void setServerInfo(CCDictionary* dict);
};

#endif /* defined(__IF__GoldExchangeItem__) */
