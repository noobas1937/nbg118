//
//  PaymentCommand.cpp
//  IF
//
//  Created by 邹 程 on 13-12-3.
//
//

#include "PaymentCommand.h"
#include "UIComponent.h"
#include "GlobalData.h"
#include "PayController.h"
#include "PopupViewController.h"
#include "FreeListView.h"
#include "ToolController.h"
#include "YesNoDialog.h"
#include "RewardController.h"
#include "RechargeACTVCell.h"

bool PaymentCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(PAYMENT_COMMAND) != 0)
        return false;
    PayController::getInstance()->setGoldExchangeSaleBoughtFlag(m_itemId, false);
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (m_orderId.compare(params->valueForKey("orderId")->getCString()) != 0) {
        return false;
    }
    
    if (params->objectForKey("errorCode")) {
        callFail(NetResult::createWithFail(params));
        return true;
    }
    
    int status = params->valueForKey("status")->intValue();
    if(CCCommonUtils::payParseData(params))
    {
        auto ret = CCDictionary::create();
        ret->setObject(CCInteger::create(status), "status");
        ret->setObject(CCString::create(m_orderId), "orderId");
        callSuccess(NetResult::createWithSuccess(ret));
    }
    return true;
}

bool PaymentIOSCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(PAYMENT_IOS_COMMAND) != 0)
        return false;
    PayController::getInstance()->setGoldExchangeSaleBoughtFlag(m_itemId, false);
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (m_orderId.compare(params->valueForKey("orderId")->getCString()) != 0) {
        return false;
    }
    
    if (params->objectForKey("errorCode")) {
        callFail(NetResult::createWithFail(params));
        return true;
    }
    
    int status = params->valueForKey("status")->intValue();
    if(CCCommonUtils::payParseData(params))
    {
        auto ret = CCDictionary::create();
        ret->setObject(CCInteger::create(status), "status");
        ret->setObject(CCString::create(m_orderId), "orderId");
        callSuccess(NetResult::createWithSuccess(ret));
    }
    return true;
}

bool PaymentTstoreCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(PAYMENT_TSTORE_COMMAND) != 0)
        return false;
    PayController::getInstance()->setGoldExchangeSaleBoughtFlag(m_itemId, false);
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (m_orderId.compare(params->valueForKey("orderId")->getCString()) != 0) {
        return false;
    }
    
    if (params->objectForKey("errorCode")) {
        callFail(NetResult::createWithFail(params));
        return true;
    }
    
    int status = params->valueForKey("status")->intValue();
    if(CCCommonUtils::payParseData(params))
    {
        auto ret = CCDictionary::create();
        ret->setObject(CCInteger::create(status), "status");
        ret->setObject(CCString::create(m_orderId), "orderId");
        callSuccess(NetResult::createWithSuccess(ret));
    }
    return true;
}

bool PaymentAmazonCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(PAYMENT_AMAZON_COMMAND) != 0)
        return false;
    PayController::getInstance()->setGoldExchangeSaleBoughtFlag(m_itemId, false);
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (m_orderId.compare(params->valueForKey("orderId")->getCString()) != 0) {
        return false;
    }
    
    if (params->objectForKey("errorCode")) {
        callFail(NetResult::createWithFail(params));
        return true;
    }
    
    int status = params->valueForKey("status")->intValue();
    if(CCCommonUtils::payParseData(params))
    {
        auto ret = CCDictionary::create();
        ret->setObject(CCInteger::create(status), "status");
        ret->setObject(CCString::create(m_orderId), "orderId");
        callSuccess(NetResult::createWithSuccess(ret));
    }
    return true;
}


bool PaymentAndroidCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(PAYMENT_ANDROID_COMMAND) != 0)
        return false;
    PayController::getInstance()->setGoldExchangeSaleBoughtFlag(m_itemId, false);
    CCDictionary *params = _dict(dict->objectForKey("params"));
    
    if (m_orderId.compare(params->valueForKey("orderId")->getCString()) != 0) {
        return false;
    }
    
    if (params->objectForKey("errorCode")) {
        callFail(NetResult::createWithFail(params));
        return true;
    }
    
    int status = params->valueForKey("status")->intValue();
    if(CCCommonUtils::payParseData(params))
    {
        auto ret = CCDictionary::create();
        ret->setObject(CCInteger::create(status), "status");
        ret->setObject(CCString::create(m_orderId), "orderId");
        callSuccess(NetResult::createWithSuccess(ret));
    }
    return true;
}