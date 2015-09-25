//
//  GoldExchangeItem.cpp
//  IF
//
//  Created by wangzhenlei on 13-12-17.
//
//

#include "GoldExchangeItem.h"
#include "GlobalData.h"

GoldExchangeItem::GoldExchangeItem(CCDictionary* dict){
    id = dict->valueForKey("id")->getCString();
    type = dict->valueForKey("type")->getCString();
    gold_doller = dict->valueForKey("gold_doller")->getCString();
    dollar = dict->valueForKey("dollar")->getCString();
    name_doller = dict->valueForKey("name_doller")->getCString();
    gift = dict->valueForKey("gift")->getCString();
    item = dict->valueForKey("item")->getCString();
    percent = dict->valueForKey("percent")->getCString();
    oldprice = dict->valueForKey("price")->getCString();
    product_id = dict->valueForKey("product_id")->getCString();
    if(dict->objectForKey("sales")){
        buyMore = (dict->valueForKey("sales")->intValue() == 1)?true:false;
    }else{
        buyMore = false;
    }

    if(dict->valueForKey("popup_image")){
        popup_image = dict->valueForKey("popup_image")->getCString();
    }else{
        popup_image = "1";
    }
    if(dict->valueForKey("price")){
        price = dict->valueForKey("price")->intValue();
    }else{
        price = 0;
    }
    if(dict->valueForKey("name")){
        name = dict->valueForKey("name")->getCString();
    }else{
        name = "";
    }
    if(dict->valueForKey("time")){
        time = dict->valueForKey("time")->intValue();
    }else{
        if(type=="3"){
            time = 1;
        }else{
            time = 0;
        }
        
    }
    if(type=="3"){
        time = 0;
    }
    if(dict->valueForKey("popup")){
        popup = dict->valueForKey("popup")->intValue();
    }else{
        popup = 0;
    }
    if(dict->valueForKey("bought")){
        bought = dict->valueForKey("bought")->boolValue();
    }else{
        bought = false;
    }
    start =0;
    if (dict->objectForKey("start")) {
        int tmpT = dict->valueForKey("start")->doubleValue()/1000;
        if (tmpT>0) {
            start = GlobalData::shared()->changeTime(tmpT);
        }
    }

    end =0;
    if (dict->objectForKey("end")) {
        int tmpT = dict->valueForKey("end")->doubleValue()/1000;
        if (tmpT>0) {
            end = GlobalData::shared()->changeTime(tmpT);
        }
    }
    if(dict->objectForKey("md5")){
        this->md5 = dict->valueForKey("md5")->getCString();
    }
    if(dict->objectForKey("equipment")){
        this->equipment = dict->valueForKey("equipment")->getCString();
    }/*else{
        this->equipment = "1001015;10|1001014;5|1001013;1";
    }*/
    if(dict->objectForKey("send_mail") && (GlobalData::shared()->analyticID == "market_global" || GlobalData::shared()->analyticID == "AppStore" || GlobalData::shared()->analyticID == "tstore" || GlobalData::shared()->analyticID == "nstore" || GlobalData::shared()->analyticID == "amazon")){
        std::string sendMail = dict->valueForKey("send_mail")->getCString();
        this->send_mail = sendMail.empty()?false:true;
    }else{
        this->send_mail=false;
    }
    isPayCallBack = false;
}
std::string GoldExchangeItem::toString(){
    std::string returnStr = "";
    returnStr = returnStr + id + ",";
    returnStr = returnStr + this->type +",";
    returnStr = returnStr + this->gold_doller + ",";
    returnStr = returnStr + this->dollar + ",";
    returnStr = returnStr + this->name_doller +",";
    returnStr = returnStr + this->gift + ",";
    returnStr = returnStr + this->item + ",";
    returnStr = returnStr + this->percent + ",";
    returnStr = returnStr + this->oldprice + ",";
    returnStr = returnStr + this->name + ",";
    returnStr = returnStr + this->product_id +",";
    returnStr = returnStr + CC_ITOA(this->start) + ",";
    returnStr = returnStr + CC_ITOA(this->end) + ",";
    returnStr = returnStr + CC_ITOA(this->time) + ",";
    returnStr = returnStr + (this->bought==true?"1":"0") + ",";
    returnStr = returnStr + CC_ITOA(this->popup) + ",";
    returnStr = returnStr + CC_ITOA(this->price) + ",";
    returnStr = returnStr + (this->isPayCallBack==true?"1":"0")+ ",";
    returnStr = returnStr + this->popup_image + ",";
    if(GlobalData::shared()->analyticID == "market_global" || GlobalData::shared()->analyticID == "AppStore" || GlobalData::shared()->analyticID == "tstore" || GlobalData::shared()->analyticID == "nstore" || GlobalData::shared()->analyticID == "amazon"){
        returnStr = returnStr + this->equipment + ",";
        returnStr = returnStr + (this->send_mail==true?"1":"0");
    }else{
        returnStr = returnStr + this->equipment;
    }
    CCLog(returnStr.c_str());
    return returnStr;
}
MonthCardInfo::MonthCardInfo(CCDictionary* dict){
    this->setServerInfo(dict);
}
void MonthCardInfo::setServerInfo(CCDictionary* dict){
    if(!dict){
        mItemId = "";
        mTimes = 0;
        mAccepted = false;
        return;
    }
    mItemId = dict->valueForKey("itemId")->getCString();
    mTimes = dict->valueForKey("times")->intValue(); //领过的次数
    mAccepted = dict->valueForKey("accepted")->boolValue();
    mBuy = dict->valueForKey("available")->boolValue();
    if(dict->objectForKey("count")){
        mCount = dict->valueForKey("count")->intValue();
    }
    if(dict->objectForKey("reward")){
        rewards = dynamic_cast<CCArray*>(dict->objectForKey("reward"));
        rewards->retain();
    }
}
void MonthCardInfo::addTime(){
    ++mTimes;
    if(canReward()){
        mAccepted=true;
    }else{
        mBuy = false;
        mTimes = 0;
        mAccepted = false;
    }
}
bool MonthCardInfo::canReward(){
    return ((mBuy==true) && (mTimes<mCount))?true:false;
}
bool MonthCardInfo::isAlreadyReward(){
    return (canReward() && mAccepted ==false)?true:false;
}
void MonthCardInfo::resetCard(){
    if(canReward()){
        mAccepted = false;
    }
}
void MonthCardInfo::buyCard(){
    if(mBuy==false){
        mTimes = 0;
        mAccepted = false;
        mBuy = true;
    }
}