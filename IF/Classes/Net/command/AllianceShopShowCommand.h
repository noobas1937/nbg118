//
//  AllianceShopShowCommand.h
//  IF
//
//  Created by lifangkai on 14-9-1.
//
//

#ifndef __IF__AllianceShopShowCommand__
#define __IF__AllianceShopShowCommand__

#include "CommandBase.h"

#define ALLIANCE_SHOP_SHOW "al.shop.show"
#define ALLIANCE_SHOP_BUY_LEADER "al.shop.buy.al"
#define ALLIANCE_SHOP_BUY_USER "al.shop.buy.usr"

class AllianceShopShowCommand: public CommandBase {
public:
    AllianceShopShowCommand() : CommandBase(ALLIANCE_SHOP_SHOW){
       // putParam("uuid", CCString::create(m_uid.c_str()));
    };
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_uid;
};

class AllianceShopLeaderBuyCommand: public CommandBase {
public:
    AllianceShopLeaderBuyCommand(std::string goodsId,int count) : CommandBase(ALLIANCE_SHOP_BUY_LEADER){
        m_goodId =goodsId;
        m_num = count;
        putParam("goodsId", CCString::create(goodsId.c_str()));
        putParam("num", CCInteger::create(count));
    };
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_goodId;
    int m_num;
};

class AllianceShopUserBuyCommand: public CommandBase {
public:
    AllianceShopUserBuyCommand(std::string goodsId,int count) : CommandBase(ALLIANCE_SHOP_BUY_USER){
        m_goodId =goodsId;
        m_num = count;
        putParam("goodsId", CCString::create(goodsId.c_str()));
        putParam("num", CCInteger::create(count));
    };
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    std::string m_goodId;
    int m_num;
};
#endif /* defined(__IF__AllianceShopShowCommand__) */
