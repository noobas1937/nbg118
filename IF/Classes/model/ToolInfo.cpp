//
//  ToolInfo.cpp
//  IF
//
//  Created by fubin on 14-2-27.
//
//

#include "ToolInfo.h"
#include "CCCommonUtils.h"
#include "ToolController.h"

ToolInfo::ToolInfo(CCDictionary* dict)
{//<ItemSpec id="200330" name="104126" icon="ico200330" type="3" type2="3" lv="1" page="3" use="1" order_num="1" price="40"/>
    this->itemId = dict->valueForKey("id")->intValue();
    this->name = dict->valueForKey("name")->getCString();
    this->icon = dict->valueForKey("icon")->getCString();
    this->des = dict->valueForKey("description")->getCString();
    this->page = dict->valueForKey("page")->intValue();
    this->use = dict->valueForKey("use")->intValue();
    this->orderNum = dict->valueForKey("order_num")->intValue();
    this->color = dict->valueForKey("color")->intValue();
    this->type = dict->valueForKey("type")->intValue();//5:礼包
    this->type2 = dict->valueForKey("type2")->intValue();
    this->price = dict->valueForKey("price")->intValue();
    this->price_all = dict->valueForKey("price_all")->intValue();
    this->onsale = dict->valueForKey("onsale")->intValue();
    this->limitLv = dict->valueForKey("lv")->intValue();
    this->sales = dict->valueForKey("sales")->getCString();
    this->limitUseLv = dict->valueForKey("lv_limit")->intValue();
    this->useall = dict->valueForKey("useall")->intValue();
    this->price_hot = dict->valueForKey("price_hot")->intValue();
    
    if(dict->objectForKey("is_show")){
        this->is_show = dict->valueForKey("is_show")->intValue();
    }else{
        this->is_show = 0;
    }
    
    if(dict->objectForKey("is_gift")){
        if(dict->valueForKey("is_gift")->intValue() == 1){
            this->is_gift = false;
        }else{
            this->is_gift = true;
        }
    }else{
        this->is_gift = true;
    }
    
    if(dict->objectForKey("pages")){
        this->pages = dict->valueForKey("pages")->intValue();
    }else{
        this->pages = 0;
    }
    if(dict->objectForKey("store_order")){
        this->store_order = dict->valueForKey("store_order")->intValue();
    }
    if(dict->objectForKey("alliance_order")){
        this->alliance_order = dict->valueForKey("alliance_order")->intValue();
    }
    if(dict->objectForKey("version")){
        this->version = dict->valueForKey("version")->getCString();
    }
    if(dict->objectForKey("pagehot")){
        this->pagehot = dict->valueForKey("pagehot")->intValue();
    }else{
        this->pagehot = 0;
    }
    
    crt_order = -1;
    crt_cd = -1;
    crt_gold = -1;
    if (dict->objectForKey("make")) {
        string makeMsg = dict->valueForKey("make")->getCString();
        vector<string> makeVec;
        CCCommonUtils::splitString(makeMsg, "|", makeVec);
        if (makeVec.size() >= 3) {
            crt_order = atoi(makeVec[0].c_str());
            crt_cd = atoi(makeVec[1].c_str());
            crt_gold = atoi(makeVec[2].c_str());
        }
    }
    
    this->cnt = 0;
    this->allianceCnt = 0;
    this->buyType = 0;
    this->uuid = "";
    ishot = false;
    isShowHot = false;
    
    string strParas = dict->valueForKey("para")->getCString();
    para = strParas;
    if (strParas != "") {
        vector<string> vecParas;
        CCCommonUtils::splitString(strParas, ";", vecParas);
        for (int i=0; i<vecParas.size(); i++) {
            string value = vecParas[i];
            this->paras[i+1] = value;
        }
    }
    //龙晶加个
    if (this->type==9) {
        if (dict->valueForKey("price_crystal")->intValue()) {
            LongJing_price = dict->valueForKey("price_crystal")->intValue();
        }
        //对应的装备id
        if(dict->objectForKey("para")){
            this->equipId = dict->valueForKey("para")->getCString();
        }else{
            this->equipId = this->itemId;
        }
        string show1 = CCCommonUtils::getPropById(this->equipId, "show1");
        string num1 = CCCommonUtils::getPropById(this->equipId, "num1");
        
        vector<string> sVec;
        CCCommonUtils::splitString(show1, "|", sVec);
        vector<string> nVec;
        CCCommonUtils::splitString(num1, "|", nVec);
        
        this->des = "";
        if (sVec.size()>0 && num1.size()>0) {
            string desStr = _lang(sVec[0].c_str());
            string desNum = "+"+nVec[0]+"%";
            this->des=this->des+desStr+" "+desNum+"\n";
        }
        if (this->color>=2) {
            this->des=this->des+_lang("111655")+"???"+"\n";
        }
        if (this->color>=3 && sVec.size()>1 && num1.size()>1) {
            string desStr = _lang(sVec[1].c_str());
            string desNum = "+"+nVec[1]+"%";
            this->des=this->des+desStr+" "+desNum+"\n";
        }
        if (this->color>=4) {
            this->des=this->des+_lang("111655")+"???"+"\n";
        }
        if (this->color>=5 && sVec.size()>2 && num1.size()>2) {
            string desStr = _lang(sVec[2].c_str());
            string desNum = "+"+nVec[2]+"%";
            this->des=this->des+desStr+" "+desNum;
        }
    }
}

string ToolInfo::getActId()
{
    string tPara = para;
    vector<string> vec;
    CCCommonUtils::splitString(para, ";", vec);
    if (vec.size() == 2 && vec[0] == "a") {
        return vec[1];
    } else {
        return "";
    }
}

CCDictionary* ToolInfo::toDic()
{
    auto dic = CCDictionary::create();
    dic->setObject(CCString::create(CC_ITOA(itemId)), "itemId");
    dic->setObject(CCString::create(CC_ITOA(getCNT())), "cnt");
    dic->setObject(CCString::create(name), "name");
    dic->setObject(CCString::create(des), "des");
    return dic;
}

void ToolInfo::SetInfoFromServer(CCDictionary* dict,bool isinit)
{
    if(isinit){
        this->cnt = dict->valueForKey("count")->intValue();
    }else{
        setCNT(dict->valueForKey("count")->intValue());
    }
    this->uuid = dict->valueForKey("uuid")->getCString();
    if (dict->objectForKey("para1")) {
        this->para1 = dict->valueForKey("para1")->getCString();
    }
    if (dict->objectForKey("para2")) {
        this->para2 = dict->valueForKey("para2")->getCString();
    }
    if (dict->objectForKey("para3")) {
        this->para3 = dict->valueForKey("para3")->getCString();
    }

    for (int i=1; ; i++) {
        string key = "para";
        key = key+CC_ITOA(i);
        if (dict->objectForKey(key)) {
            this->paras[i] = dict->valueForKey(key)->getCString();
        }
        else{
            break;
        }
    }
}
int ToolInfo::getCNT(){
    return cnt;
}
void ToolInfo::setCNT(int var){
    if(var>cnt){
        ToolController::getInstance()->addToNewGoodsList(this->itemId);
    }
    cnt = MAX(var,0);
    if (cnt == 0) {
        ToolController::getInstance()->removeFromNewGoodsList(this->itemId);
    }
}
string ToolInfo::getName() const{
    if(type == 3 && !para.empty()){
        if(para.find_first_not_of("1234567890")==string::npos){
            return _lang_1(name, CC_CMDITOA(atoi(para.c_str())).c_str());
        }
    }
    return _lang(name);
}

ToolRewardInfo::ToolRewardInfo(CCDictionary* dict)
{
    this->resId = dict->valueForKey("type")->intValue();
    this->itemId=0;
    if(this->resId == R_GOODS) {
        auto toolDict = _dict(dict->objectForKey("value"));
        this->itemId = toolDict->valueForKey("id")->intValue();
        this->num = toolDict->valueForKey("num")->intValue();
    } else {
        this->num = dict->valueForKey("value")->intValue();
    }
}
MerchantToolInfo::MerchantToolInfo(CCDictionary* dict){
    SetInfoFromServer(dict);
}
void MerchantToolInfo::SetInfoFromServer(CCDictionary* dict){
    itemId = dict->valueForKey("itemId")->intValue();
    this->itemNum = dict->valueForKey("itemNum")->intValue();
    this->price = dict->valueForKey("price")->intValue();
    this->priceType = dict->valueForKey("priceType")->intValue();
    this->num = dict->valueForKey("num")->intValue();
    this->color = dict->valueForKey("color")->intValue();
    if(dict->objectForKey("type")){
        this->type = (MerchantToolType)dict->valueForKey("type")->intValue();
    }else{
        this->type = MerchantTool_GOODS;
    }
    if(dict->objectForKey("price_hot")){
        this->priceHot = dict->valueForKey("price_hot")->intValue();
    }else{
        this->priceHot = 0;
    }
}