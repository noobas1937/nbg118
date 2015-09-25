//
//  ToolInfo.h
//  IF
//
//  Created by fubin on 14-2-27.
//
//

#ifndef IF_ToolInfo_h
#define IF_ToolInfo_h

#include "CommonInclude.h"

class ToolInfo : public cocos2d::CCObject {
public:
    string getActId();
    CCDictionary* toDic();
    ToolInfo(CCDictionary* dict);
    ToolInfo():
    itemId(0)
    ,uuid("")
    ,name("")
    ,icon("")
    ,des("")
    ,page(0)
    ,type(0)
    ,type2(0)
    ,use(0)
    ,orderNum(0)
    ,color(0)
    ,buyType(0)
    ,price(0)
    ,price_all(0)
    ,cnt(0)
    ,limitLv(0)
    ,sales("")
    ,limitUseLv(0)
    ,allianceCnt(0)
    ,onsale(0)
    ,useall(0)
    ,hotprice(0)
    ,price_hot(0)
    ,hotnum(0)
    ,ishot(false)
    ,isShowHot(false)
    ,para("")
    ,is_show(0)
    ,crt_order(0)
    ,crt_cd(0)
    ,crt_gold(0)
    ,pages(0)
    ,store_order(0)
    ,alliance_order(0)
    ,version("")
    ,para1("")
    ,para2("")
    ,para3("")
    ,pagehot(0)
    ,is_gift(true)
    ,LongJing_price(0)
    ,equipId("")
    {};
    
    int itemId;
    string uuid;
//    string name;
    string icon;
    string des;
    int page;
    int type;
    int type2;
    int use;
    int orderNum;
    int color;
    int buyType;
    float price;
    float price_all;   //联盟物品积分价格，>0时就是联盟物品
    int limitLv;
    int limitUseLv;
    int allianceCnt;  //联盟物品数量
    int onsale;
    string sales;
    int useall;
    int hotprice;
    int price_hot;
    int hotnum;
    bool ishot;
    bool isShowHot;
    int is_show;
    bool is_gift; //是否能使用火漆信赠送
    int crt_order;
    int crt_cd;
    int crt_gold;
    map<int,string> paras;
    string para1;
    string para2;
    string para3;
    
    //新增字段,用于商店切页和排序
    int pages;
    int store_order;
    int alliance_order;
    int pagehot;    //商店热卖字段
    void SetInfoFromServer(CCDictionary* dict,bool isinit = false);
    float LongJing_price;//龙晶加个
    string equipId;//对应的装备id
private:
    CC_PROPERTY(int, cnt, CNT);
    CC_SYNTHESIZE_READONLY(string, para, Para);
    CC_SYNTHESIZE_READONLY(string, version, Version);
    CC_PROPERTY_READONLY(string, name, Name);
};

class ToolRewardInfo : public cocos2d::CCObject {
public:
    ToolRewardInfo(CCDictionary* dict);
    ToolRewardInfo():
    itemId(0)
    ,resId(0)
    ,num(0)
    {};
    
    int resId;
    int itemId;
    int num;
};

enum MerchantToolType{
    MerchantTool_RESOURCE,
    MerchantTool_GOODS,
    MerchantTool_EQUIP
};

class MerchantToolInfo : public cocos2d::CCObject{
public:
    MerchantToolInfo(CCDictionary* dict);
    MerchantToolInfo():itemId(0),num(0),itemNum(0),price(0),priceType(0),color(0),type(MerchantTool_GOODS),priceHot(0){
    };
    int itemId;
    int num;
    int itemNum;
    int price;
    int priceType;
    int priceHot;
    int orderNum;
    int color;
    MerchantToolType type; // 1- goods, 2 - equip
    virtual void SetInfoFromServer(CCDictionary* dict);
};
#endif
