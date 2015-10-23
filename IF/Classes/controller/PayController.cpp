//
//  PayController.cpp
//  IF
//
//  Created by 邹 程 on 13-11-22.
//
//

#include "PayController.h"
#include "PaymentCommand.h"
#include "YesNoDialog.h"
#include "DataRecordCommand.h"
#include "RechargePopupView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "NetController.h"
#include "FBUtilies.h"
#include "FunBuildController.h"
#include "AppLibHelper.h"

#include "StoreBagView.h"

/** 漏单信息的分割符 */
static const char* COK_PURCHASE_DELIMITER_DATA = "|#|";//不同字段中间的分隔符
static const char* COK_PURCHASE_DELIMITER_ORDERS = "|*|";//不同订单中间的分隔符

//订单信息存放的key
#define COK_PURCHASE_KEY "COKPurchaseInfoList"
//已成功订单号存放的key
#define COK_PURCHASE_SUCCESSED_KEY  "COK_PURCHASE_SUCCESSED_KEY"

static PayController* _instance = NULL;

PayController* PayController::getInstance() {
    if (!_instance) {
        _instance = new PayController();
    }
    return _instance;
}
PayController::PayController():
m_payState(0)
,m_orderId(string())
,m_productId(string())
,m_gross(string())
,m_amount(string())
,m_currency(string())
,m_description(string())
,m_payCallbackType(0)
{
    string analytic = GlobalData::shared()->analyticID;
    if(GlobalData::shared()->isXiaoMiPlatForm()==true || analytic == "cn1"){
        mCost.insert(pair<string, int>("gold_1",30));
        mCost.insert(pair<string, int>("gold_2",68));
        mCost.insert(pair<string, int>("gold_3",128));
        mCost.insert(pair<string, int>("gold_4",328));
        mCost.insert(pair<string, int>("gold_5",648));
        mCost.insert(pair<string, int>("gold_6",163));
        mCost.insert(pair<string, int>("gold_7",6498));
        mCost.insert(pair<string, int>("gold_8",6));
    }else if(analytic == "mycard" || analytic == "gash"){
        mCost.insert(pair<string, int>("gold_1",150));
        mCost.insert(pair<string, int>("gold_2",300));
        mCost.insert(pair<string, int>("gold_3",600));
        mCost.insert(pair<string, int>("gold_4",1500));
        mCost.insert(pair<string, int>("gold_5",3000));
        mCost.insert(pair<string, int>("gold_6",750));
//        mCost.insert(pair<string, int>("gold_7",6498));
        mCost.insert(pair<string, int>("gold_8",30));
    }
}

void PayController::callPaymentToFriend(GoldExchangeItem *m_dataItem,std::string touid){
    m_donateUID = touid;
    if(m_dataItem){
        if(PayRecordCommand::isSendRecord==false){
            PayRecordCommand::isSendRecord = true;
            PayRecordCommand *cmd1 = new PayRecordCommand(m_dataItem->id,GlobalData::shared()->analyticID);
            cmd1->sendAndRelease();
        }
    }
//    if(GlobalData::shared()->playerInfo.gmFlag==1){
//        PayController::getInstance()->setItemId(m_dataItem->id);
//        std::string aaa = "{\"developerPayload\":\"test|" +m_dataItem->id + "|" +m_donateUID +"\"}";
//        double time =GlobalData::shared()->getTimeStamp()*1000.0;
//        string timestr = CC_ITOA(time);
//        auto pcmd = new PaymentCommand(CC_ITOA(atoi(m_dataItem->id.c_str())-8889+10000) ,timestr,m_dataItem->product_id,aaa,"1",m_dataItem->id);
//        pcmd->sendAndRelease();
//        return;
//    }

    
    int itemId = atoi(m_dataItem->id.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (!GlobalData::shared()->isGoogle()) {
        string _userId = GlobalData::shared()->playerInfo.uid;
        PayController::getInstance()->setItemId(m_dataItem->id);
        
        if(GlobalData::shared()->analyticID == "elex337"){
            PopupViewController::getInstance()->removeAllPopupView();
            PopupViewController::getInstance()->addPopupInView(RechargePopupView::create(m_dataItem, GlobalData::shared()->playerInfo.uid));
        }else{
            PayController::getInstance()->payAndroidToUid(m_dataItem->id,m_dataItem->dollar,m_dataItem->gold_doller,_userId,m_donateUID);
        }
    }
    else
    {
        PayController::getInstance()->setItemId(m_dataItem->id);
        PayController::getInstance()->payGoogle(m_dataItem->product_id);
    }
#else
    PayController::getInstance()->setItemId(m_dataItem->id);
    PayController::getInstance()->pay(m_dataItem->id,m_dataItem->gold_doller,m_dataItem->dollar,"USD",m_dataItem->product_id,NULL);
#endif
    FBUtilies::appEventGiftPackage("GoldExchangePopupView", m_dataItem->id, atoi(m_dataItem->id.c_str()), FunBuildController::getInstance()->getMainCityLv(), GlobalData::shared()->playerInfo.level);
}
void PayController::callPayment(GoldExchangeItem *m_dataItem,std::string fromView){
    m_donateUID="";
    // 用户点击行为统计
    if(m_dataItem){
        if(PayRecordCommand::isSendRecord==false){
            PayRecordCommand::isSendRecord = true;
            PayRecordCommand *cmd1 = new PayRecordCommand(m_dataItem->id,GlobalData::shared()->analyticID);
            cmd1->sendAndRelease();
        }
    }
//    if(GlobalData::shared()->playerInfo.gmFlag==1 && m_dataItem){
//        PayController::getInstance()->setItemId(m_dataItem->id);
//        std::string aaa = "{\"developerPayload\":\"test|" +m_dataItem->id + "\"}";
//        double time =GlobalData::shared()->getTimeStamp()*1000.0;
//        string timestr = CC_ITOA(time);
//        auto pcmd = new PaymentCommand(CC_ITOA(atoi(m_dataItem->id.c_str())-8889),timestr,m_dataItem->product_id,aaa,"1",m_dataItem->id);
//        pcmd->sendAndRelease();
//        return;
//    }
    if(!m_dataItem){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(GlobalData::shared()->analyticID == "mycard" || GlobalData::shared()->analyticID == "gash"){
            string _userId = GlobalData::shared()->playerInfo.uid;
            PayController::getInstance()->setItemId("0");
            PayController::getInstance()->payAndroid("0","0","0",_userId);
        }
#endif
    }else{
        int itemId = atoi(m_dataItem->id.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if (!GlobalData::shared()->isGoogle()) {
            string _userId = GlobalData::shared()->playerInfo.uid;
            PayController::getInstance()->setItemId(m_dataItem->id);

            if(GlobalData::shared()->analyticID == "elex337"){
                PopupViewController::getInstance()->removeAllPopupView();
                PopupViewController::getInstance()->addPopupInView(RechargePopupView::create(m_dataItem, GlobalData::shared()->playerInfo.uid));
            }else{
                PayController::getInstance()->payAndroid(m_dataItem->id,m_dataItem->dollar,m_dataItem->gold_doller,_userId);
            }
        }
        else
        {
            PayController::getInstance()->setItemId(m_dataItem->id);
            PayController::getInstance()->payGoogle(m_dataItem->product_id);
        }
#else
        PayController::getInstance()->setItemId(m_dataItem->id);
        PayController::getInstance()->pay(m_dataItem->id,m_dataItem->gold_doller,m_dataItem->dollar,"USD",m_dataItem->product_id,NULL,m_dataItem->buyMore);
#endif
        CCArray* p = CCArray::create();
        p->addObject(CCInteger::create(GOLD_BUTTON));
        CCArray* d = CCArray::create();
        d->addObject(CCInteger::create(itemId - 8889));
        DataRecordCommand* cmd = new DataRecordCommand(OPEN_PANEL, p, d);
        cmd->sendAndRelease();
        FBUtilies::appEventGiftPackage(fromView, m_dataItem->id, atoi(m_dataItem->id.c_str()), FunBuildController::getInstance()->getMainCityLv(), GlobalData::shared()->playerInfo.level);
    }
}

void PayController::addOrderCache(string pf
                                  ,string orderId
                                  ,string sSignedData
                                  ,string productId
                                  ,string itemId
                                  ,string productType
                                  ,string purchaseTime
                                  ){
    CCLOG("---------------add order cache---------------");
    if (checkPurchaseSuccessed(orderId)) {
        CCLOG("---------------order already fulfilled---------------");
        return;
    }
    string purshaseInfoList = CCUserDefault::sharedUserDefault()->getStringForKey(COK_PURCHASE_KEY,"");
    CCLOG("------before add order %s",purshaseInfoList.c_str());
    if (!purshaseInfoList.empty())
    {
        CCArray* list = CCCommonUtils::split(purshaseInfoList.c_str(), COK_PURCHASE_DELIMITER_ORDERS);
        for (int i = 0; i < list->count(); i++)
        {
            CCArray* subList = CCCommonUtils::split(((CCString*)list->objectAtIndex(i))->getCString(), COK_PURCHASE_DELIMITER_DATA);
            if (subList->count()!=10 || orderId == ((CCString*)subList->objectAtIndex(1))->getCString()){
                CCLOG("info already exist");
                return;
            }
        }
    }
    /**
     订单号
     订单号验证参数
     gold_1,gold_2...
     9000,9001...
     **/
    CCString* tmp = CCString::createWithFormat("%s|#|%s|#|%s|#|%s|#|%s|#|%s|#|%s|#|%s|#|%s|#|%s", pf.c_str(), orderId.c_str(), sSignedData.c_str(), productId.c_str(), itemId.c_str(), productType.c_str(), purchaseTime.c_str(), "", "", "");
    if (!purshaseInfoList.empty()) {
        tmp = CCString::createWithFormat("%s%s%s", tmp->getCString(), COK_PURCHASE_DELIMITER_ORDERS, purshaseInfoList.c_str());
    }
    string info = tmp->getCString();
    CCUserDefault::sharedUserDefault()->setStringForKey(COK_PURCHASE_KEY, info);
    CCUserDefault::sharedUserDefault()->flush();
    CCLOG("------after add order %s",info.c_str());
}

void PayController::removeOrderCache(string orderId){
    CCLOG("---------------remove order cache---------------");
    string purshaseInfoList = CCUserDefault::sharedUserDefault()->getStringForKey(COK_PURCHASE_KEY,"");
    CCLOG("------remove key %s",orderId.c_str());
    CCLOG("------before remove order %s",purshaseInfoList.c_str());
    if (!purshaseInfoList.empty())
    {
        string newPurshaseInfoList = "";
        CCArray* list = CCCommonUtils::split(purshaseInfoList.c_str(), COK_PURCHASE_DELIMITER_ORDERS);
        for (int i = 0; i < list->count(); i++)
        {
            CCArray* subList = CCCommonUtils::split(((CCString*)list->objectAtIndex(i))->getCString(), COK_PURCHASE_DELIMITER_DATA);
            if (subList->count()!=10 || orderId == ((CCString*)subList->objectAtIndex(1))->getCString())
            {
                list->removeObjectAtIndex(i);
                --i;
            }
        }
        purshaseInfoList.clear();
        for (int i = 0; i < list->count(); i++)
        {
            string str =  ((CCString*)list->objectAtIndex(i))->getCString();
            if (i>0)
                purshaseInfoList += COK_PURCHASE_DELIMITER_ORDERS;
            purshaseInfoList += str;
        }
        CCUserDefault::sharedUserDefault()->setStringForKey(COK_PURCHASE_KEY, purshaseInfoList);
        CCUserDefault::sharedUserDefault()->flush();
        CCLOG("------after remove order %s",purshaseInfoList.c_str());
    }
}

//加入一成功订单列表
void PayController::addToPurchaseIdList(string orderId)
{
    if (orderId.empty())
        return;
    removeOrderCache(orderId);
    if (!checkPurchaseSuccessed(orderId)){
        string idList = CCUserDefault::sharedUserDefault()->getStringForKey(COK_PURCHASE_SUCCESSED_KEY,"");
        if (!idList.empty()) {
            idList += COK_PURCHASE_DELIMITER_ORDERS;
        }
        idList += orderId;
        
        CCLOG("orderId list:%s",idList.c_str());
        CCUserDefault::sharedUserDefault()->setStringForKey(COK_PURCHASE_SUCCESSED_KEY, idList);
        CCUserDefault::sharedUserDefault()->flush();
    }
    //检查历史订单
    checkPurchaseInfoList();
}

//检查是否订单已成功
bool PayController::checkPurchaseSuccessed(string orderId)
{
    bool isSuccessed = false;
    string idList = CCUserDefault::sharedUserDefault()->getStringForKey(COK_PURCHASE_SUCCESSED_KEY,"");
    if (!idList.empty()) {
        CCArray* arr = CCCommonUtils::split(idList.c_str(), COK_PURCHASE_DELIMITER_ORDERS);
        for (int i=0; i<arr->count(); i++) {
            if (dynamic_cast<CCString*>(arr->objectAtIndex(i))) {
                string oneid = dynamic_cast<CCString*>(arr->objectAtIndex(i))->getCString();
                if (orderId == oneid) {
                    isSuccessed = true;
                }
            }
        }
    }
    return isSuccessed;
}

void PayController::checkPurchaseInfoList()
{
    CCLOG("---------------purchase history redo---------------");
    CCLOG("check-key---->%s",COK_PURCHASE_KEY);
//    CCUserDefault::sharedUserDefault()->setStringForKey(COK_PURCHASE_KEY,"");
    string purshaseInfoList = CCUserDefault::sharedUserDefault()->getStringForKey(COK_PURCHASE_KEY,"");
    CCLOG("check-purchaseinfolist-->%s",purshaseInfoList.c_str());
    bool foundPurchaseCache = false;
    if (!purshaseInfoList.empty())
    {
        CCArray* list = CCCommonUtils::split(purshaseInfoList.c_str(), COK_PURCHASE_DELIMITER_ORDERS);
        if (list && list->count()>0) {
            for (int i = 0; i < list->count(); i++)
            {
                CCArray* subList = CCCommonUtils::split(((CCString*)list->objectAtIndex(i))->getCString(), COK_PURCHASE_DELIMITER_DATA);
                string pf = ((CCString*)subList->objectAtIndex(0))->getCString();
                string orderId = ((CCString*)subList->objectAtIndex(1))->getCString();
                string checkedPurchaseOrderId = GlobalData::shared()->historyPurchaseList[orderId];
                if(checkedPurchaseOrderId == ""){
                    foundPurchaseCache = true;
                    GlobalData::shared()->historyPurchaseList[orderId] = orderId;
                }else{
                    continue;
                }
                CCLog("---------------COK purchase cache redo--------------- %s",orderId.c_str());
                //orderid内存缓存
                if(pf == "AppStore"){
                    string sSignedData = ((CCString*)subList->objectAtIndex(2))->getCString();
                    string productId = ((CCString*)subList->objectAtIndex(3))->getCString();
                    string itemId = ((CCString*)subList->objectAtIndex(4))->getCString();
                    if(itemId.find("_")==std::string::npos){
                        auto cmd = new PaymentIOSCommand(orderId,sSignedData,productId,itemId);
                        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(PayController::onCallPaymentServerCallback), NULL));
                        cmd->sendAndRelease();
                    }else{
                        vector<string> mVect;
                        CCCommonUtils::splitString(itemId, "_", mVect);
                        if(mVect.size()>1 && !mVect.at(0).empty() && !mVect.at(1).empty()){
                            auto cmd = new PaymentIOSCommand(orderId,sSignedData,productId,mVect.at(0),mVect.at(1));
                            cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(PayController::onCallPaymentServerCallback), NULL));
                            cmd->sendAndRelease();
                        }
                    }
                    break;
                }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                if(pf == "nstore"){
                    string paymentSeq = orderId;
                    string nonce = ((CCString*)subList->objectAtIndex(2))->getCString();
                    string itemId = ((CCString*)subList->objectAtIndex(4))->getCString();
                    auto cmd = new PaymentAndroidCommand(paymentSeq,itemId);
                    cmd->putParam("nonce", CCString::create(nonce));
                    cmd->putParam("paymentSeq", CCString::create(paymentSeq));
                    cmd->putParam("pf", CCString::create("nstore"));
                    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(PayController::onCallPaymentAndroidCallback), NULL));
                    cmd->sendAndRelease();
                    break;
                }
                if(pf == "tstore"){
                    string txid = orderId;
                    string receipt = ((CCString*)subList->objectAtIndex(2))->getCString();
                    string itemId = ((CCString*)subList->objectAtIndex(4))->getCString();
                    auto cmd = new PaymentTstoreCommand(txid,receipt,itemId);
                    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(PayController::onCallPaymentAndroidCallback), NULL));
                    cmd->sendAndRelease();
                    break;
                }
                if(pf == "amazon"){
                    string amazonUserId = ((CCString*)subList->objectAtIndex(2))->getCString();
                    string sku = ((CCString*)subList->objectAtIndex(3))->getCString();
                    string itemId = ((CCString*)subList->objectAtIndex(4))->getCString();
                    string productType = ((CCString*)subList->objectAtIndex(5))->getCString();
                    string purchaseTime = ((CCString*)subList->objectAtIndex(6))->getCString();
                    if(itemId.find("_")==std::string::npos){
                        auto cmd = new PaymentAmazonCommand(productType,purchaseTime,sku,amazonUserId,orderId,itemId);
                        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(PayController::onCallPaymentAndroidCallback), NULL));
                        cmd->sendAndRelease();
                    }else{
                        vector<string> mVect;
                        CCCommonUtils::splitString(itemId, "_", mVect);
                        if(mVect.size()>1 && !mVect.at(0).empty() && !mVect.at(1).empty()){
                            auto cmd = new PaymentAmazonCommand(productType,purchaseTime,sku,amazonUserId,orderId,mVect.at(0),mVect.at(1));
                            cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(PayController::onCallPaymentAndroidCallback), NULL));
                            cmd->sendAndRelease();
                        }
                    }
                    break;
                }
#endif
            }
        }
    }
    if(!foundPurchaseCache){
        CCLog("---------------COK purchase checkHistoryPurchase---------------");
        PayController::checkHistoryPurchase();
        return;
    }
}

void PayController::callPaymentIOSSendGoods(string orderId
                                            ,string sSignedData
                                            ,string productId
                                            ,string itemId
                                            ) {
//    string _itemId = m_itemId;
    if (itemId == "") {
        itemId = CCUserDefault::sharedUserDefault()->getStringForKey(CATCH_ITEM_ID, "");
    }
    addOrderCache("AppStore", orderId, sSignedData, productId, itemId);
    if(itemId.find("_") == string::npos){
        if(m_donateUID.empty()){
            setGoldExchangeSaleBoughtFlag(itemId,true);
            auto cmd = new PaymentIOSCommand(orderId,sSignedData,productId,itemId);
            cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(PayController::onCallPaymentServerCallback), NULL));
            cmd->sendAndRelease();
            return;
        }
    }else{
        std::vector<std::string> mVect;
        CCCommonUtils::splitString(itemId, "_", mVect);
        if(mVect.size()>1){
            itemId = mVect.at(0);
            m_donateUID = mVect.at(1);
        }
    }
    if(!m_donateUID.empty()){
        auto cmd = new PaymentIOSCommand(orderId,sSignedData,productId,itemId,m_donateUID);
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(PayController::onCallPaymentServerCallback), NULL));
        cmd->sendAndRelease();
    }
}

void PayController::onPayCancel(){
    m_payState = 0;
}

void PayController::onPayCallback(int callbackType, string msg) {
    if (m_payState == 0 || m_payState == 2) {
        // idle or wait server state
        CCLOG("pay callback state error: %d state can't callback!",m_payState);
        return;
    }
    if (m_payState == 3) {
        // server finish state
        CCLOG("pay callback state: server already callback! do finish up");
        m_payState = 0;
    } else {
        // wait server state
        m_payState = 2;
    }
    
    m_payCallbackType = callbackType;
    
    // do common callback
    switch (callbackType) {
        case 1:{
            CCLOG("pay callback : not init");
        }
            break;
        case 2:{
            CCLOG("pay callback : pay failed! with error:%s",msg.c_str());
        }
            break;
        case 3:{
            CCLOG("pay callback : pay canceled!");
        }
            break;
        case 4:{
            CCLOG("pay callback : pay finished!");
        }
            break;
            
        default:
            CCLOG("pay callback type error!");
            return;
    }
    
    // do product specific callback
    if (strcmp(m_productId.c_str(), PAY_GOLD_PRODUCT_ID) == 0) {
        // bug gold
        payForGoldFinished();
    }
}
//设置金币购买的状态
void PayController::setGoldExchangeSaleBoughtFlag(string itemId,bool isCallBack){
    map<string,GoldExchangeItem*>::iterator it = GlobalData::shared()->goldExchangeList.find(itemId);
    if (it!= GlobalData::shared()->goldExchangeList.end()){
        if(it->second->type=="3"){
            it->second->isPayCallBack = true;
        }
    }
}
void PayController::pay(string productId, string amount, string gross, string currency, string description, cocos2d::CCCallFunc *callback,bool buyMore) {
    if (m_payState != 0) {
        CCLOG("current payment is not finished!");
        return;
    }
    m_productId = productId;
    m_amount = amount;
    m_gross = gross;
    m_currency = currency;
    m_description = description;
    m_callback = callback;
    
    m_payCallbackType = 0;
    m_payState = 1;
    
    if(buyMore==true){
        ToolNumSelectView *view = ToolNumSelectView::create(atoi(productId.c_str()),10);
        PopupViewController::getInstance()->addPopupView(view);
    }else{
        m_payNumber = 1;
        doPay();
    }
}
void PayController::setPayNumber(int productid,int number){
    if(m_productId.compare(CC_ITOA(productid)) == 0){
        m_payNumber = number;
        doPay();
    }
}
void PayController::resetPayState(){
    m_payState=0;
    m_payNumber = 1;
    m_productId = "";
    m_amount = "";
    m_gross = "";
    m_currency = "";
    m_description = "";
    m_callback = NULL;
    m_payCallbackType = 0;
}

// pay product method

void PayController::payForGold(int amount, string gross, string currency, cocos2d::CCCallFunc *callback) {
    string count = string(CC_ITOA(amount));
    pay(PAY_GOLD_PRODUCT_ID, count, gross, currency, count + " Gold", callback);
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
string PayController::getDollarText(string dollar,string productId){
    string payCurrency = "US $";
    string payCount = "-1";
    if(GlobalData::shared()->analyticID == "tstore"
       || GlobalData::shared()->analyticID == "nstore"){
        payCurrency = "₩";
        double price = atof(dollar.c_str());
        payCount = CC_ITOA(ceil(price * 10) * 100);
    }else if(GlobalData::shared()->analyticID == "cafebazaar"){
        payCurrency = "IRR";//﷼
        double price = atof(dollar.c_str());
        payCount = CC_CMDITOA(ceil(price) * 35 * 1000);
        return payCount + payCurrency;
    }else if(GlobalData::shared()->analyticID == "market_global"){//googlePlay
        string _price = CCUserDefault::sharedUserDefault()->getStringForKey(productId.c_str(),"");
        if (_price != "") {
            return _price;
        }else{
            string _symbole = CCUserDefault::sharedUserDefault()->getStringForKey("gp_price_symbole");
            float _exRate = CCUserDefault::sharedUserDefault()->getFloatForKey("gp_price_exchange_rate");
            bool _symBack = CCUserDefault::sharedUserDefault()->getBoolForKey("gp_price_symbole_back");
            if(!_symbole.empty() && _exRate>0){
                float payValue = atof(dollar.c_str());
                payValue *=_exRate;
                char buff[32];
                if(_symBack==false){
                    sprintf(buff, "%s%.2f",_symbole.c_str(),payValue);
                }else{
                    sprintf(buff, "%.2f%s",payValue,_symbole.c_str());
                }
                std::string returnVal = buff;
                CCLog("symbole:%s,rate:%f,final:%s",_symbole.c_str(),_exRate,returnVal.c_str());
                return returnVal;
            }
        }
        payCount = dollar;
    }else if(GlobalData::shared()->isXiaoMiPlatForm() == true || GlobalData::shared()->analyticID == "mycard" || GlobalData::shared()->analyticID == "gash" || GlobalData::shared()->analyticID == "cn1" ){
        payCurrency = (GlobalData::shared()->isXiaoMiPlatForm() == true || GlobalData::shared()->analyticID == "cn1")? "￥ ":"NT$ ";
        bool bInProduct = false;
        if(!productId.empty()){
            auto iter = mCost.find(productId);
            if(iter!=mCost.end()){
                bInProduct = true;
                payCount = CC_CMDITOA((*iter).second);
            }
        }
        if(bInProduct==false && !dollar.empty()){
            auto iter = mCost.find("gold_1");
            if(iter!=mCost.end()){
                float rate = (*iter).second * 1.0/4.99;
                int cost = ceil(rate * atoi(dollar.c_str()));
                payCount = CC_CMDITOA(cost);
            }
        }
    }else{
        payCount = dollar;
    }
    return payCurrency + payCount;
}

void PayController::payGoogle(string goldType, cocos2d::CCCallFunc *callback) {
    if (m_payState != 0) {
        CCLOG("current payment is not finished!");
        return;
    }
    m_payState = 1;
    m_productType = goldType;
    m_callback = callback;
    GlobalData::shared()->isBind = true;
    GlobalData::shared()->isPayBind = true;
    doPayGoogle();
}
#endif

void PayController::payForGoldFinished() {
    if (!m_callback.getObject()) {
        return;
    }
    auto callback = dynamic_cast<CCCallFuncO*>(m_callback.getObject());
    if (callback) {
        callback->setObject(CCInteger::create(m_payCallbackType));
        callback->execute();
    }
    m_callback = NULL;
}


void PayController::showErrorDialog(){
    std::string showString = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //根据平台显示错误提示
    if(GlobalData::shared()->analyticID == "market_global"){
        showString = "Google";
    }else{
        /**
         tstore
         nstore
         amazon
         cafebazaar
         **/
        showString = GlobalData::shared()->analyticID;
    }
#endif
    if(!showString.empty()){
        showString =_lang_1("101296", showString.c_str());
        if(!showString.empty()){
            YesNoDialog::showYesDialog(showString.c_str());
        }
    }
}

void PayController::onCallPaymentServerCallback(CCObject* obj) {
    CCUserDefault::sharedUserDefault()->setStringForKey(CATCH_ITEM_ID, "");
    m_donateUID="";
    auto ret = dynamic_cast<NetResult*>(obj);
    
    m_payState = 0;
    
    if (!ret || ret->getErrorCode() != Error_OK) {
        string error;
        if (ret->getData() && dynamic_cast<CCString*>(ret->getData())) {
            error = dynamic_cast<CCString*>(ret->getData())->getCString();
        } else {
            error = "Unknow Error!";
        }
        YesNoDialog::showYesDialog(_lang(error).c_str());
        return;
    }
}

void PayController::setItemId(string _itemId)
{
    m_itemId = _itemId;
    CCUserDefault::sharedUserDefault()->setStringForKey(CATCH_ITEM_ID, _itemId);
    CCUserDefault::sharedUserDefault()->flush();
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>

void PayController::doPay() {
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/nbg/IF/Pay", "doPay", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V") ) {
        CCLOG("JNI: pay method not found!");
        return;
    }
    
    jstring productId = minfo.env->NewStringUTF(m_productId.c_str());
    jstring gross = minfo.env->NewStringUTF(m_gross.c_str());
    jstring amount = minfo.env->NewStringUTF(m_amount.c_str());
    jstring currency = minfo.env->NewStringUTF(m_currency.c_str());
    jstring description = minfo.env->NewStringUTF(m_description.c_str());
    
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,productId,gross,amount,currency,description);
    
    minfo.env->DeleteLocalRef(productId);
    minfo.env->DeleteLocalRef(gross);
    minfo.env->DeleteLocalRef(amount);
    minfo.env->DeleteLocalRef(currency);
    minfo.env->DeleteLocalRef(description);
}

void PayController::removeWebView(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //1. 获取activity静态对象
    JniMethodInfo minfo;
    //getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
    jobject activityObj;
    if(JniHelper::getStaticMethodInfo(minfo,"org/nbg/IF/IF","getInstance","()Lorg/nbg/IF/IF;"))
    {
        activityObj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    }
    //2. 查找updateURL接口，并用jobj调用
    if (!JniHelper::getMethodInfo(minfo,"org/nbg/IF/IF","removeWebView", "()V"))
    {
        return;
    }
    else
    {
        //调用此函数
        minfo.env->CallVoidMethod(activityObj, minfo.methodID);
    }
#endif
}


// for google play
void PayController::doPayGoogle() {
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/nbg/IF/Payment", "callBuyGold", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V") ) {
        CCLOG("JNI: pay google method callBuyGold not found!");
        GlobalData::shared()->isBind = false;
        GlobalData::shared()->isPayBind = false;
        return;
    }
    jstring jtype=minfo.env->NewStringUTF(m_productType.c_str());
    jstring jitemId=minfo.env->NewStringUTF(m_itemId.c_str());
    jstring jdUId=minfo.env->NewStringUTF(m_donateUID.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jtype,jitemId,jdUId);
}

void PayController::pay337(const string &pay_id,const string &gold_num,const string &get_gold_count,const string &userId, const string &channel){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    GlobalData::shared()->isBind = true;
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "com/clash/of/jni/Jni", "payClicked", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
    jstring jpay_id=minfo.env->NewStringUTF(pay_id.c_str());
    jstring jgold_num=minfo.env->NewStringUTF(gold_num.c_str());
    jstring jget_gold_count=minfo.env->NewStringUTF(get_gold_count.c_str());
    jstring juserId=minfo.env->NewStringUTF(userId.c_str());
    jstring jchannel = minfo.env->NewStringUTF(channel.c_str());
    
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jpay_id,jgold_num,jget_gold_count,juserId,jchannel);
    minfo.env->DeleteLocalRef(minfo.classID);
    minfo.env->DeleteLocalRef(jpay_id);
    minfo.env->DeleteLocalRef(jgold_num);
    minfo.env->DeleteLocalRef(jget_gold_count);
    minfo.env->DeleteLocalRef(juserId);
    minfo.env->DeleteLocalRef(jchannel);

#endif
}

void PayController::payAndroid(const string &pay_id,const string &gold_num,const string &get_gold_count,const string &userId)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    if(GlobalData::shared()->analyticID == "tstore" ){
        if (m_payState != 0) {
            CCLOG("current payment is not finished!");
            return;
        }
        m_payState = 1;
    }
    AppLibHelper::triggerEventPurchaseInit(gold_num,pay_id);
    
    GlobalData::shared()->isBind = true;
    GlobalData::shared()->isPayBind = true;
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "com/clash/of/jni/Jni", "payClicked", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
        CCLOGFUNC("jni: no method");
        GlobalData::shared()->isBind = false;
        GlobalData::shared()->isPayBind = false;
        return;
    }
    jstring jpay_id=minfo.env->NewStringUTF(pay_id.c_str());
    jstring jgold_num=minfo.env->NewStringUTF(gold_num.c_str());
    jstring jget_gold_count=minfo.env->NewStringUTF(get_gold_count.c_str());
    jstring juserId=minfo.env->NewStringUTF(userId.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jpay_id,jgold_num,jget_gold_count,juserId);
    minfo.env->DeleteLocalRef(minfo.classID);
    minfo.env->DeleteLocalRef(jpay_id);
    minfo.env->DeleteLocalRef(jgold_num);
    minfo.env->DeleteLocalRef(jget_gold_count);
    minfo.env->DeleteLocalRef(juserId);
#endif
}
void PayController::payAndroidToUid(const string &pay_id,const string &gold_num,const string &get_gold_count,const string &userId,const string &toUID){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(GlobalData::shared()->analyticID == "tstore" ){
        if (m_payState != 0) {
            CCLOG("current payment is not finished!");
            return;
        }
        m_payState = 1;
    }
    AppLibHelper::triggerEventPurchaseInit(gold_num,pay_id);

    GlobalData::shared()->isBind = true;
    GlobalData::shared()->isPayBind = true;
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "com/clash/of/jni/Jni", "payClickedToUser", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")) {
        CCLOGFUNC("jni: no method");
        GlobalData::shared()->isBind = false;
        GlobalData::shared()->isPayBind = false;
        return;
    }
    jstring jpay_id=minfo.env->NewStringUTF(pay_id.c_str());
    jstring jgold_num=minfo.env->NewStringUTF(gold_num.c_str());
    jstring jget_gold_count=minfo.env->NewStringUTF(get_gold_count.c_str());
    jstring juserId=minfo.env->NewStringUTF(userId.c_str());
    jstring jtouid =minfo.env->NewStringUTF(toUID.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,jpay_id,jgold_num,jget_gold_count,juserId,jtouid);
    minfo.env->DeleteLocalRef(minfo.classID);
    minfo.env->DeleteLocalRef(jpay_id);
    minfo.env->DeleteLocalRef(jgold_num);
    minfo.env->DeleteLocalRef(jget_gold_count);
    minfo.env->DeleteLocalRef(juserId);
    minfo.env->DeleteLocalRef(jtouid);
#endif
}

void PayController::initGoogle() {
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/nbg/IF/Payment", "callPayInit", "()V") ) {
        CCLOG("JNI: pay google method callPayInit not found!");
        return;
    }
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
}

void PayController::checkHistoryPurchase(){
    if(GlobalData::shared()->historyPurchaseChecked)
        return;
    GlobalData::shared()->historyPurchaseChecked = true;
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "com/clash/of/jni/Jni", "queryHistoryPurchase", "()V") ) {
        CCLOG("JNI: jni method queryHistoryPurchase not found!");
        return;
    }
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
}

void PayController::callPaymentCheck(string orderId
                                     ,string purchaseTime
                                     ,string productId
                                     ,string signData
                                     ,string signature
                                     ) {
    string _itemId = m_itemId;
    if (_itemId == "") {
        _itemId = CCUserDefault::sharedUserDefault()->getStringForKey(CATCH_ITEM_ID, "");
    }
    setGoldExchangeSaleBoughtFlag(_itemId,true);
    auto cmd = new PaymentCommand(orderId,purchaseTime,productId,signData,signature,_itemId);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(PayController::onCallPaymentCheckCallback), NULL));
    cmd->sendAndRelease();
}

void PayController::callPaymentTstoreSendGoods(string txid
                                     ,string receipt
                                     ,string itemId
                                     ) {
    CCUserDefault::sharedUserDefault()->setStringForKey(TSTORE_TXID_CACHE, txid);
    CCUserDefault::sharedUserDefault()->setStringForKey(TSTORE_RECEIPT_CACHE, receipt);
    CCUserDefault::sharedUserDefault()->setStringForKey(CATCH_ITEM_ID, itemId);
    
    setGoldExchangeSaleBoughtFlag(itemId,true);
    addOrderCache("tstore", txid, receipt, "", itemId);
    if (NetController::shared()->isConnected()) {
        auto cmd = new PaymentTstoreCommand(txid,receipt,itemId);
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(PayController::onCallPaymentAndroidCallback), NULL));
        cmd->sendAndRelease();
    }
}

void PayController::callPaymentNstoreSendGoods(string paymentSeq
                                     ,string nonce
                                     ,string itemId
                                     ) {
    string orderId = paymentSeq;
    string sSignedData = nonce;
    addOrderCache("nstore", orderId, sSignedData, "", itemId);
    setGoldExchangeSaleBoughtFlag(itemId,true);
    auto cmd = new PaymentAndroidCommand(paymentSeq,itemId);
    cmd->putParam("nonce", CCString::create(nonce));
    cmd->putParam("paymentSeq", CCString::create(paymentSeq));
    cmd->putParam("pf", CCString::create("nstore"));
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(PayController::onCallPaymentAndroidCallback), NULL));
    cmd->sendAndRelease();
}

void PayController::callPaymentCafebazaarSendGoods(string orderId
                                                   ,string purchaseToken
                                                   ,string sku
                                                   ,string itemId) {
    setGoldExchangeSaleBoughtFlag(itemId,true);
    auto cmd = new PaymentAndroidCommand(orderId,itemId);
    cmd->putParam("purchaseToken", CCString::create(purchaseToken));
    cmd->putParam("sku", CCString::create(sku));
    cmd->putParam("pf", CCString::create("cafebazaar"));
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(PayController::onCallPaymentAndroidCallback), NULL));
    cmd->sendAndRelease();
}

void PayController::callPaymentTencentSendGoods(string orderId
                                                ,string openid
                                                ,string itemId) {
    setGoldExchangeSaleBoughtFlag(itemId,true);
    auto cmd = new PaymentAndroidCommand(orderId,itemId);
    cmd->putParam("openid", CCString::create(openid));
    cmd->putParam("pf", CCString::create("tencent"));
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(PayController::onCallPaymentAndroidCallback), NULL));
    cmd->sendAndRelease();
}

void PayController::callPaymentAmazonSendGoods(string productType
                                     ,string purchaseTime
                                     ,string sku
                                     ,string amazonUserId
                                     ,string orderId
                                     ,string itemId
                                     ) {
    if (itemId == "") {
        itemId = CCUserDefault::sharedUserDefault()->getStringForKey(CATCH_ITEM_ID, "");
    }
    addOrderCache("amazon", orderId, amazonUserId, sku, itemId, productType, purchaseTime);
    if(itemId.find("_")==std::string::npos){
        setGoldExchangeSaleBoughtFlag(itemId,true);
        auto cmd = new PaymentAmazonCommand(productType,purchaseTime,sku,amazonUserId,orderId,itemId);
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(PayController::onCallPaymentAndroidCallback), NULL));
        cmd->sendAndRelease();
    }else{
        vector<string> mVect;
        CCCommonUtils::splitString(itemId, "_", mVect);
        if(mVect.size()>1 && !mVect.at(0).empty() && !mVect.at(1).empty()){
            auto cmd = new PaymentAmazonCommand(productType,purchaseTime,sku,amazonUserId,orderId,mVect.at(0),mVect.at(1));
            cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(PayController::onCallPaymentAndroidCallback), NULL));
            cmd->sendAndRelease();
        }
    }
}

void PayController::onCallPaymentCheckCallback(CCObject* obj) {
    auto ret = dynamic_cast<NetResult*>(obj);
    
    m_payState = 0;
    
    if (!ret || ret->getErrorCode() != Error_OK) {
        string error;
        if (ret->getData() && dynamic_cast<CCString*>(ret->getData())) {
            error = dynamic_cast<CCString*>(ret->getData())->getCString();
        } else {
            error = "Unknow Error!";
        }
        YesNoDialog::showYesDialog(_lang(error).c_str());
        return;
    }
    
    auto retDict = _dict(ret->getData());
    int status = dynamic_cast<CCInteger*>(retDict->objectForKey("status"))->getValue();
    string orderId = dynamic_cast<CCString*>(retDict->objectForKey("orderId"))->getCString();
    
    // call back java
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/nbg/IF/Payment", "onConsumeCallback", "(Ljava/lang/String;I)V") ) {
        CCLOG("JNI: pay google method onConsumeCallback not found!");
        return;
    }
    jint callback_status = status;
    jstring tmpOrderId = minfo.env->NewStringUTF(orderId.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,tmpOrderId,callback_status);
    minfo.env->DeleteLocalRef(tmpOrderId);
}

void PayController::onCallPaymentAndroidCallback(CCObject* obj) {
    auto ret = dynamic_cast<NetResult*>(obj);
    
    m_payState = 0;
    
    if (!ret || ret->getErrorCode() != Error_OK) {
        string error;
        if (ret->getData() && dynamic_cast<CCString*>(ret->getData())) {
            error = dynamic_cast<CCString*>(ret->getData())->getCString();
        } else {
            error = "Unknow Error!";
        }
        YesNoDialog::showYesDialog(_lang(error).c_str());
        return;
    }
    
    auto retDict = _dict(ret->getData());
    int status = dynamic_cast<CCInteger*>(retDict->objectForKey("status"))->getValue();
    string orderId = dynamic_cast<CCString*>(retDict->objectForKey("orderId"))->getCString();
    
    // call back java
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "com/clash/of/jni/Jni", "onConsumeCallback", "(Ljava/lang/String;I)V") ) {
        CCLOG("JNI: pay google method onConsumeCallback not found!");
        return;
    }
    jint callback_status = status;
    jstring tmpOrderId = minfo.env->NewStringUTF(orderId.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,tmpOrderId,callback_status);
    minfo.env->DeleteLocalRef(tmpOrderId);
}

#ifdef __cplusplus
extern "C" {
#endif

    void Java_org_nbg_IF_Pay_nativeOnPayCallback(JNIEnv* env,jobject thiz,jint result,jstring msg) {
        int resultType = result;
        string resultMsg = JniHelper::jstring2string(msg);
        GlobalData::shared()->isBind = false;
        CCLOG("JNI: payment callback, result type:%d ,msg:%s",resultType,resultMsg.c_str());
        PayController::getInstance()->onPayCallback(resultType,resultMsg);
    }
    
    void Java_com_clash_of_PayGoogle_nativeCallPayInfo(JNIEnv* env,jobject thiz,jstring payInfo) {
        string _payInfo = JniHelper::jstring2string(payInfo);
        CCLOG("JNI: payment callback, payInfo:%s",_payInfo.c_str());
        if(_payInfo.empty()){
            CCUserDefault::sharedUserDefault()->setStringForKey("gold_1","");
            CCUserDefault::sharedUserDefault()->setStringForKey("gold_2","");
            CCUserDefault::sharedUserDefault()->setStringForKey("gold_3","");
            CCUserDefault::sharedUserDefault()->setStringForKey("gold_4","");
            CCUserDefault::sharedUserDefault()->setStringForKey("gold_5","");
            CCUserDefault::sharedUserDefault()->setStringForKey("gold_6","");
            CCUserDefault::sharedUserDefault()->setFloatForKey("gp_price_exchange_rate",0);
            CCUserDefault::sharedUserDefault()->setStringForKey("gp_price_symbole","");
        }else{
            vector<std::string> vector1 ;
            vector<std::string> vector2 ;
            std::string pay1Key = "gold_1";
            std::string pay1Value = "";
            CCCommonUtils::splitString(_payInfo, ";", vector1);
            int i = 0;
            while (i < vector1.size()) {
                vector2.clear();
                CCCommonUtils::splitString(vector1[i], ":", vector2);
                if (vector2.size() == 0) {
                    continue;
                }
                string productId = vector2[0];
                string price = vector2[1];
                CCUserDefault::sharedUserDefault()->setStringForKey(productId.c_str(),price.c_str());
                CCLOG("JNI: payment productID:%s,price:%s",productId.c_str(),price.c_str());
                if(productId == pay1Key){
                    pay1Value = price;
                }
                i++;
            }
            std::string checkString = "0123456789,. ";
            std::string symbole = "";
            std::string valuestring = "";
            auto iter = pay1Value.begin();
            int index = 0;
            int symIndex = 0;
            bool symBack =false;
            if(checkString.find((*iter))==string::npos){
                symIndex = 0;
                ++index;
                ++iter;
                while (iter!=pay1Value.end()) {
                    if(checkString.find((*iter))!=string::npos){
                        break;
                    }
                    ++index;
                    iter++;
                }
                symbole = pay1Value.substr(symIndex,index);
                valuestring = pay1Value.substr(index,pay1Value.length());
                symBack = false;
            }else{
                ++symIndex;
                ++iter;
                while (iter!=pay1Value.end()) {
                    if(checkString.find((*iter))==string::npos){
                        break;
                    }
                    ++symIndex;
                    iter++;
                }
                symbole = pay1Value.substr(symIndex,pay1Value.length());
                valuestring = pay1Value.substr(index,symIndex);
                symBack=true;
            }
            if(valuestring.find(",")!=string::npos){
                valuestring = valuestring.replace(valuestring.find(","), 1, ".");
            }
            while(valuestring.find(" ")!=string::npos){
                valuestring = valuestring.replace(valuestring.find(" "), 1, "");
            }
            if(valuestring.find_first_not_of("1234567890.")==std::string::npos){
                double value = atof(valuestring.c_str());
                float vPrecent = value / 4.99;
                CCUserDefault::sharedUserDefault()->setStringForKey("gp_price_symbole",symbole.c_str());
                CCUserDefault::sharedUserDefault()->setFloatForKey("gp_price_exchange_rate",vPrecent);
                CCUserDefault::sharedUserDefault()->setBoolForKey("gp_price_symbole_back", symBack);
                //CCLOG("%f",value);
            }
        }
        CCUserDefault::sharedUserDefault()->flush();
    }
    
    jobject Java_org_nbg_IF_Pay_nativeGetUserUid(JNIEnv* env, jobject thiz) {
        jstring uid = env->NewStringUTF(GlobalData::shared()->playerInfo.uid.c_str());
        return uid;
    }
    
    // for google play    
    void Java_org_nbg_IF_Payment_nativeCallPaySuccess(JNIEnv* env,jobject thiz,
                                                      jstring orderId,
                                                      jstring purchaseTime,
                                                      jstring productId,
                                                      jstring signData,
                                                      jstring signature
                                                      ) {
        string oid = JniHelper::jstring2string(orderId);
        string pct = JniHelper::jstring2string(purchaseTime);
        string prd = JniHelper::jstring2string(productId);
        string sid = JniHelper::jstring2string(signData);
        string sig = JniHelper::jstring2string(signature);

        GlobalData::shared()->isBind = false;
        
        CCLOG("JNI: payment callback success!");
        CCLOG("Order ID: %s",oid.c_str());
        CCLOG("Original Json: %s",sid.c_str());
        CCLOG("Signature: %s",sig.c_str());
        PayController::getInstance()->callPaymentCheck(oid,pct,prd,sid,sig);
    }
    void Java_org_nbg_IF_Payment_nativeCallPayFailed(JNIEnv* env, jobject thiz,jint failedType,jstring failedMsg) {
        int failCode = failedType;
        string failInfo = JniHelper::jstring2string(failedMsg);
        GlobalData::shared()->isBind = false;
        CCLOG("JNI: payment callback failed!");
        CCLOG("Fail Type :%d",failCode);
        CCLOG("Fail Info :%s",failInfo.c_str());
        // todo : call failed callback
        PayController::getInstance()->setPayState(0);
        if(failCode == -1){
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_PAY_FAILED);
        }
    }
    
    void Java_org_cocos2dx_ext_Native_nativeTstorePaymentSuccess(JNIEnv* env,jobject thiz,jstring txid,jstring receipt,jstring itemId) {
        string ctxid = JniHelper::jstring2string(txid);
        string creceipt = JniHelper::jstring2string(receipt);
        string citemId = JniHelper::jstring2string(itemId);
        
//        GlobalData::shared()->isBind = false;
        PayController::getInstance()->setPayState(0);
        CCLOG("JNI: payment tstore callback success!");
        CCLOG("txid: %s",ctxid.c_str());
        CCLOG("receipt: %s",creceipt.c_str());
        CCLOG("itemId: %s",citemId.c_str());
        PayController::getInstance()->callPaymentTstoreSendGoods(ctxid,creceipt,citemId);
    }

    void Java_org_cocos2dx_ext_Native_nativeAmazonPaymentSuccess(JNIEnv* env,jobject thiz,jstring productType,jstring purchaseTime,jstring sku,jstring amazonUserId,jstring orderId,jstring itemId) {
        string cproductType = JniHelper::jstring2string(productType);
        string cpurchaseTime = JniHelper::jstring2string(purchaseTime);
        string csku = JniHelper::jstring2string(sku);
        string camazonUserId = JniHelper::jstring2string(amazonUserId);
        string corderId = JniHelper::jstring2string(orderId);
        string citemId = JniHelper::jstring2string(itemId);
        
//        GlobalData::shared()->isBind = false;
        
        CCLOG("JNI: payment tstore callback success!");
        CCLOG("productType: %s",cproductType.c_str());
        CCLOG("purchaseTime: %s",cpurchaseTime.c_str());
        CCLOG("sku: %s",csku.c_str());
        CCLOG("amazonUserId: %s",camazonUserId.c_str());
        CCLOG("orderId: %s",corderId.c_str());
        CCLOG("itemId: %s",citemId.c_str());
        PayController::getInstance()->callPaymentAmazonSendGoods(cproductType,cpurchaseTime,csku,camazonUserId,corderId,citemId);
    }
    
    void Java_org_cocos2dx_ext_Native_nativeNstorePaymentSuccess(JNIEnv* env,jobject thiz,jstring paymentSeq,jstring nonce,jstring itemId) {
        string cpaymentSeq = JniHelper::jstring2string(paymentSeq);
        string cnonce = JniHelper::jstring2string(nonce);
        string citemId = JniHelper::jstring2string(itemId);
        
//        GlobalData::shared()->isBind = false;
        
        CCLOG("JNI: payment nstore callback success!");
        CCLOG("paymentSeq: %s",cpaymentSeq.c_str());
        CCLOG("nonce: %s",cnonce.c_str());
        CCLOG("itemId: %s",citemId.c_str());
        PayController::getInstance()->callPaymentNstoreSendGoods(cpaymentSeq,cnonce,citemId);
    }
    
    void Java_org_cocos2dx_ext_Native_nativeCafebazaarPaymentSuccess(JNIEnv* env,jobject thiz,jstring orderId,jstring purchaseToken,jstring sku,jstring itemId) {
        string corderId = JniHelper::jstring2string(orderId);
        string cpurchaseToken = JniHelper::jstring2string(purchaseToken);
        string csku = JniHelper::jstring2string(sku);
        string citemId = JniHelper::jstring2string(itemId);
        
        //        GlobalData::shared()->isBind = false;
        
        CCLOG("JNI: payment cafebazaar callback success!");
        CCLOG("orderId: %s",corderId.c_str());
        CCLOG("purchaseToken: %s",cpurchaseToken.c_str());
        CCLOG("sku: %s",csku.c_str());
        CCLOG("itemId: %s",citemId.c_str());
        PayController::getInstance()->callPaymentCafebazaarSendGoods(corderId,cpurchaseToken,csku,citemId);
    }
    
    void Java_org_cocos2dx_ext_Native_nativeTencentPaymentSuccess(JNIEnv* env,jobject thiz,jstring orderId,jstring openid,jstring itemId) {
        string corderId = JniHelper::jstring2string(orderId);
        string copenid = JniHelper::jstring2string(openid);
        string citemId = JniHelper::jstring2string(itemId);
        CCLOG("JNI: payment tencent callback success!");
        CCLOG("orderid: %s",corderId.c_str());
        CCLOG("openid: %s",copenid.c_str());
        CCLOG("itemId: %s",citemId.c_str());
        PayController::getInstance()->callPaymentTencentSendGoods(corderId,copenid,citemId);
    }
    
    void Java_org_cocos2dx_ext_Native_nativeEnableQueryHistoryPurchase(JNIEnv* env, jobject thiz)
    {
        GameController::getInstance()->enableQueryHistoryPurchase();
    }

    void Java_org_cocos2dx_ext_Native_nativeAndroidPaymentFail(JNIEnv* env,jobject thiz) {
//        GlobalData::shared()->isBind = false;
        
        CCLOG("JNI: payment android callback fail");
        PayController::getInstance()->setPayState(0);
    }

    
#ifdef __cplusplus
}
#endif

#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
