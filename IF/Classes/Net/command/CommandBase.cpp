//
//  CommandBase.cpp
//  IF
//
//  Created by zhaohf on 13-9-04.
//
//

#include "CommandBase.h"
#include "NetController.h"
#include "CCThreadManager.h"

void CommandBase::send()
{
    NetController::shared()->send(this);
}

void CommandBase::cancel()
{
    setCallback(NULL);
}

void CommandBase::putParam(const std::string &key, cocos2d::CCObject *value)
{
    m_data->setObject(value, key);
}

void CommandBase::callFail(CCObject *result)
{
    if (!getFailCallback())
        return;
    
    CCCallFuncO * func=getFailCallback();
    func->retain();
    setFailCallback(NULL);
    func->setObject(result);
    func->execute();
    CC_SAFE_RELEASE(func);
}

void CommandBase::callSuccess(NetResult *result)
{
    if (!getSuccessCallback())
        return;
    
    CCCallFuncO * func=getSuccessCallback();
    func->retain();
    setSuccessCallback(NULL);
    func->setObject(result);
    func->execute();
    CC_SAFE_RELEASE(func);
}

