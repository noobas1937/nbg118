//
//  CommandBase.cpp
//  IF
//
//  Created by zhaohf on 13-9-04.
//
//

#ifndef __IF__CommandBase__
#define __IF__CommandBase__

#include "CommonInclude.h"
#include <algorithm>

enum CheckType{
    CHECK_NULL,
    CHECK_CMD,
    CHECK_CMD_PARAM,
};

/// wrapper for IntArray in SFS.
class CCIntArray: public CCArray {
public:
    static CCIntArray* create() {
        auto ret = new CCIntArray();
        if (ret) {
            ret->autorelease();
        } else {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
protected:
    CCIntArray():CCArray(){}
};

/// 命令返回的数据包装
class NetResult: public cocos2d::CCObject
{
public:
    NetResult(Error code, cocos2d::CCObject *data)
    : errorCode(code), m_data(data) {}
    
    static NetResult* createWithSuccess(CCObject *data=NULL) {
        return create(Error_OK, data);
    }
    
    static NetResult* createWithFail(CCDictionary* failParams=NULL) {
        auto ret = create();
        ret->setErrorCode(Error_OP_FAILURE);
        if (failParams) {
            ret->setData(CCString::create(failParams->valueForKey("errorCode")->getCString()));
        }
        return ret;
    }
    
    static NetResult* create(int errorCode=Error_OK, CCObject *data=NULL) {
        NetResult* result=new NetResult((Error)errorCode, data);
        if (result)
            result->autorelease();
        return result;
    }
    
    /// 错误码，0为成功
    CC_SYNTHESIZE(Error, errorCode, ErrorCode);
    /// 附加数据
    CC_SYNTHESIZE_SAFEOBJECT(CCObject, m_data, Data);
};

class CommandBase: public cocos2d::CCObject
{
public:
    CommandBase(const std::string &cmd, CheckType checkType = CHECK_NULL, CCDictionary *checkDict = NULL)
    : m_cmd(cmd), m_data(NULL)
    , m_timeSended(0)
    , m_checkType(checkType)
    , m_checkDict(checkDict)
    {
        CCLOG("CommandBase:%s", cmd.c_str());
        m_data = cocos2d::CCDictionary::create();
        m_data->setObject(CCString::create(CC_ITOA(GlobalData::shared()->getWorldTime())), "cmdBaseTime");
    }
    virtual ~CommandBase(void) {CCLOG("~CommandBase:%s", m_cmd.c_str());};
    /// @retval true 表示此返回属于此命令
    /// @retval false 表示此返回不属于此命令
    virtual bool handleRecieve(cocos2d::CCDictionary *dict)=0;
    
    virtual void send();
    void sendAndRelease() { send(); release(); }
    void cancel();
    
    // 调用失败回调
    void callFail(CCObject* result);
    
    // 同时设置成功和失败回调.
    void setCallback(CCCallFuncO* callback)
    { setSuccessCallback(callback); setFailCallback(callback); }
    
    void putParam(const std::string &key, CCObject *value);
    
protected:
    void onFail(CCObject * result);
    
    void callSuccess(NetResult* result);
    
private:
//    int m_sendTimes;
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_cmd, Command);
    CC_SYNTHESIZE_SAFEOBJECT(cocos2d::CCDictionary, m_data, Data);
    CC_SYNTHESIZE(CheckType, m_checkType, CheckType);
    CC_SYNTHESIZE_SAFEOBJECT(cocos2d::CCDictionary, m_checkDict, CheckDict);

    CC_SYNTHESIZE(time_t, m_timeSended, TimeSended);
    
    /// 成功请求时回调，参数为AOENetResult指针
    CC_SYNTHESIZE_SAFEOBJECT(cocos2d::CCCallFuncO, m_onSuccess, SuccessCallback);
    /// 请求失败时回调，参数为AOENetResult指针
    CC_SYNTHESIZE_SAFEOBJECT(cocos2d::CCCallFuncO, m_onFail, FailCallback);
};

/// 用于存用到的网络命令，析构时会调用所有的cancel
class NetCommandHolder {
public:
    typedef map<string, cocos2d::extension::CCSafeObject<CommandBase> > CommandMap;
    ~NetCommandHolder()
    {
        clear();
    }
    
    void clear()
    {
        for (CommandMap::iterator iter=m_cmdMap.begin(); iter!=m_cmdMap.end(); ++iter)
            iter->second->cancel();
        m_cmdMap.clear();
    }
    
    void set(const string& key, CommandBase*cmd)
    { erase(key); m_cmdMap[key]=cmd; }
    void erase(const string& key)
    {
        CommandMap::iterator iter=m_cmdMap.find(key);
        if (iter!=m_cmdMap.end()) {
            iter->second->cancel();
            m_cmdMap.erase(iter);
        }
    }
private:
    CommandMap m_cmdMap;
};

#endif /* defined(__IF__CommandBase__) */