//
//  ToolCommand.h
//  IF
//
//  Created by fubin on 14-3-4.
//
//

#ifndef IF_ToolCommand_h
#define IF_ToolCommand_h

#include "CommandBase.h"
#define TOOL_BUY_COMMAND "item.buy"
#define TOOL_USE_COMMAND "item.use"
#define TOOL_HOT_GET_COMMAND "hot.item.get"
#define TOOL_HOT_BUY_COMMAND "hot.item.buy"

#define TOOL_MERCHANT_GET_COMMAND "hot.item.v2.get"
#define TOOL_MERCHANT_REFRESH_COMMAND "hot.item.refresh"
#define TOOL_MERCHANT_BUY_COMMAND "hot.item.v2.buy"

#define TOOL_CREATE_COMMAND "create.tool"
#define TOOL_HAVEST_COMMAND "havest.tool"
#define TOOL_QUEUE_COMMAND "tool.queue.buy"
#define TOOL_QUEUE_CANCEL_COMMAND "rm.tool"

#define TOOL_ITEM_COMPOSE "item.compose"

#define LONG_BUY_COMMAND "crystal.buy"

class ToolUseCommand;

class ToolBuyCommand: public CommandBase {
public:
    ToolBuyCommand(string cmd, string itemId, int num,bool buyAndUse,int batch = 0):
    CommandBase(cmd, CHECK_CMD_PARAM),m_buyAndUse(buyAndUse),m_num(num){
        putParam("itemId", CCString::create(itemId));
        putParam("num", CCInteger::create(num));
        if(batch>0){
            putParam("batch", CCInteger::create(batch));
        }
        auto dict = CCDictionary::create();
        dict->setObject(CCString::create(itemId), "itemId");
        this->setCheckDict(dict);
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    bool m_buyAndUse;
    int m_num;
private:
};

class ToolUseCommand: public CommandBase {
public:
    ToolUseCommand(string cmd, string uuid, int num):
    CommandBase(cmd){
        putParam("uuid", CCString::create(uuid));
        putParam("num", CCInteger::create(num));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};
//
class ToolHotGetCommand: public CommandBase {
public:
    ToolHotGetCommand():
    CommandBase(TOOL_HOT_GET_COMMAND){
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

class ToolHotBuyCommand: public CommandBase {
public:
    ToolHotBuyCommand(string cmd, string itemId, int num,int price):
    CommandBase(cmd){
        putParam("itemId", CCString::create(itemId));
        putParam("num", CCInteger::create(num));
        putParam("price", CCInteger::create(price));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};
//行走商人CMD
//
class ToolMerchantGetCommand: public CommandBase {
public:
    ToolMerchantGetCommand():
    CommandBase(TOOL_MERCHANT_GET_COMMAND){
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};
class ToolMerchantRefreshCommand: public CommandBase {
public:
    ToolMerchantRefreshCommand():
    CommandBase(TOOL_MERCHANT_REFRESH_COMMAND){
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};
class ToolMerchantBuyCommand: public CommandBase {
public:
    ToolMerchantBuyCommand(const string itemId,const int itemNum,const int num,const int price,const int priceType):
    CommandBase(TOOL_MERCHANT_BUY_COMMAND){
        putParam("itemId", CCString::create(itemId));
        putParam("itemNum", CCInteger::create(itemNum));
        putParam("price", CCInteger::create(price));
        putParam("priceType", CCInteger::create(priceType));
        putParam("num", CCInteger::create(num));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

class ToolCreateCommand: public CommandBase {
public:
    ToolCreateCommand(int itemId):
    CommandBase(TOOL_CREATE_COMMAND){
        putParam("itemId", CCString::create(CC_ITOA(itemId)));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};
class ToolHavestCommand: public CommandBase {
public:
    ToolHavestCommand(string quuid,int _index = 0):
    CommandBase(TOOL_HAVEST_COMMAND){
        putParam("quuid", CCString::create(quuid));
        putParam("index", CCInteger::create(_index));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};
//添加建造材料空位
class ToolQueueAddCommand: public CommandBase {
public:
    ToolQueueAddCommand(int num):
    CommandBase(TOOL_QUEUE_COMMAND){
        putParam("num", CCInteger::create(num));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

//取消建造材料
class ToolQueueCancelCommand: public CommandBase {
public:
    ToolQueueCancelCommand(int _index):
    CommandBase(TOOL_QUEUE_CANCEL_COMMAND){
        putParam("index", CCInteger::create(_index));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

class ItemComposeCommand: public CommandBase {
public:
    ItemComposeCommand(int itemId):
    CommandBase(TOOL_ITEM_COMPOSE){
        m_itemId = itemId;
        putParam("itemId", CCString::create(CC_ITOA(itemId)));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    int m_itemId;
    void updatedatashow(int m_itemId);
};
#endif
