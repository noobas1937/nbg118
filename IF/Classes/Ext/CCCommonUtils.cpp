//
//  CCCommonUtils.cpp
//  IF
//
//  Created by 邹 程 on 13-9-16.
//
//

#include "CCCommonUtils.h"
#include "SceneController.h"
#include "GuideRecordCommand.h"
#include "GeneralManager.h"
#include "YesNoDialog.h"
#include "PopupViewController.h"
#include "FlyHint.h"
#include "GuideController.h"
#include "GeneralManager.h"
#include "FunBuildController.h"
#include "ScienceController.h"
#include "FlySystemUpdateHint.h"
#include "InitCommand.h"
#include "SceneController.h"
#include "MailController.h"
#include "NetController.h"
#include "PopupViewController.h"
#include "FunBuildController.h"
#include "DragonBuildingController.h"
#include "DragonController.h"
#include "FunBuildInfo.h"
#include "QuestController.h"
#include "GeneralManager.h"
#include "PayController.h"
#include "AppLibHelper.h"
#include "GCMRewardController.h"
#include "ChatController.h"
#include "WorldController.h"
#include "ChatShieldInfo.h"
#include "QueueController.h"
#include "WorldActivityCommand.h"
#include "EnemyInfoController.h"
#include "ToolController.h"
#include "TroopsController.h"
#include "CCDevice.h"
#include "CityCrashView.h"
#include "TrainInfo.h"
#include "TalentController.h"
#include "ActivityController.h"
#include "ScienceController.h"
#include "ActivityController.h"
#include "SocialController.h"
#include "Utf8Utils.h"
#include "RewardController.h"
#include "FreeListView.h"
#include "TimerController.h"
#include "EquipmentController.h"
#include "ShowStatusItemCommand.h"
#include "LotteryController.h"
#include "FilterSprite.h"
#include "ChatServiceCocos2dx.h"
#include "AchievementController.h"
#include "AchieveGetCommand.h"
#include "../view/guiExt/md5.h"
#include "UploadImageController.h"
#include "LuaController.h"
#include "GetInviteeCommand.h"
#include "DailyActiveController.h"
#include "HelpshiftCocos2dx.h"
#include "VipUtil.h"
#include "UserBindCommand.h"
#include "DragonBuildingController.h"
#include "PhoneDeviceCommand.h"
#include "AllianceManager.h"
#include "TitanController.h"

//std::string CCCommonUtils::to_string(int d) {
//    char buffer[50];
//    snprintf(buffer, 49, "%d",d);
//    return std::string(buffer);
//}
//
//std::string CCCommonUtils::to_string(double d) {
//    char buffer[50];
//    snprintf(buffer, 49, "%.3f",d);
//    return std::string(buffer);
//}
//
//std::string CCCommonUtils::to_string(float d) {
//    char buffer[50];
//    snprintf(buffer, 49, "%.3f",d);
//    return std::string(buffer);
//}
std::string CCCommonUtils::timeStampToUTCDate(time_t &t){
    struct tm tm;
    char s[100];
    
    tm = *gmtime(&t);
    strftime(s, sizeof(s), "%Y-%m-%d  %H:%M:%S", &tm);
    return s;
}
std::string CCCommonUtils::timeStampToUTCHSM(time_t &t)
{
    struct tm tm;
    char s[100];
    
    tm = *gmtime(&t);
    strftime(s, sizeof(s), "%H:%M:%S", &tm);
    return s;
}
std::string CCCommonUtils::timeStampToDate(time_t &t){
    struct tm tm;
    char s[100];
    
    tm = *localtime(&t);
    strftime(s, sizeof(s), "%Y-%m-%d  %H:%M:%S", &tm);
    return s;
}
std::string CCCommonUtils::timeStampToHMS(time_t &t){
    struct tm tm;
    char s[100];
    
    tm = *localtime(&t);
    strftime(s, sizeof(s), "%H:%M:%S", &tm);
    return s;
}
std::string CCCommonUtils::timeStampToDHM(time_t &t){
    struct tm tm;
    char s[100];
    
    tm = *localtime(&t);
    strftime(s, sizeof(s), "%Y-%m-%d  %H:%M", &tm);
    return s;
}

std::string CCCommonUtils::timeStampToYMD(time_t &t){
    struct tm tm;
    char s[100];
    
    tm = *localtime(&t);
    strftime(s, sizeof(s), "%Y-%m-%d", &tm);
    return s;
}

std::string CCCommonUtils::timeStampToHour(time_t &t){
    struct tm tm;
    char s[100];
    
    tm = *localtime(&t);
    strftime(s, sizeof(s), "%H:%M", &tm);
    return s;
}

std::string CCCommonUtils::timeStampToMD(time_t &t){
    struct tm tm;
    char s[100];
    
    tm = *localtime(&t);
    strftime(s, sizeof(s), "%m-%d", &tm);
    return s;
}

long CCCommonUtils::UTCDataToTimeStamp(const char* timeStamp)
{
    //timeStamp like this:"2015-03-21-01-00-00"
    struct tm tm;
    strptime(timeStamp,"%Y-%m-%d-%H-%M-%S", &tm);
    time_t ft=mktime(&tm);
    return ft;
}

string CCCommonUtils::subStrByUtf8(const string &str,int index,int len){
    int totalLen = Utf8Utils::strlen(str.c_str());
    string retStr = "";
    if(index<=totalLen-1){
        int realLen = len<(totalLen-index) ? len:(totalLen-index);
        retStr = Utf8Utils::utf8sub(str.c_str(),index,realLen);
    }
    return retStr;
}
CCDictionary* CCCommonUtils::castDict(cocos2d::CCObject *obj) {
    auto dict = dynamic_cast<CCDictionary*>(obj);
    if (dict) {
        return dict;
    } else {
        return CCDictionary::create();
    }
}

CCPoint CCCommonUtils::getPointByString(std::string string,std::string separator) {
    vector<std::string> tmp;
    CCPoint ret;
    CCCommonUtils::splitString(string, separator, tmp);
    ret.x = atof(tmp.front().c_str());
    ret.y = atof(tmp.back().c_str());
    return ret;
}

int CCCommonUtils::getNormalPray(){
    auto& build = GlobalData::shared()->imperialInfo.find(423000)->second;
    double effect = (GlobalData::shared()->scienceEffect["210"] + 100) / 100.0;
    int product = build.para1 * effect;
    return product;
}

int CCCommonUtils::getIronPray(){
    auto& build = GlobalData::shared()->imperialInfo.find(423000)->second;
    double effect = (GlobalData::shared()->scienceEffect["210"] + 100) / 100.0;
    int product = build.para3 * effect;
    return product;
}

bool CCCommonUtils::checkGeneralState(int _state)
{
    map<string,GeneralInfo> *generals = &(GlobalData::shared()->generals);
    if(!generals->empty()){
        map<string, GeneralInfo>::iterator it;
        for(it=generals->begin(); it != generals->end(); ++it)
        {
            if (it->second.state == _state) {
                return true;
            }
        }
    }
    return false;
}

int CCCommonUtils::getGoldByTime(int t)
{
    if(t == 0){
        return 0;
    }
    int ret = 0;
//    int tmpTime = t;
//    bool _bTmp = true;
//    int maxIdx = GlobalData::shared()->cd_time.size()-1;
//    while (_bTmp) {
//        if (tmpTime > GlobalData::shared()->cd_time[maxIdx]) {
//            ret += GlobalData::shared()->cd_cost[maxIdx];
//            tmpTime -= GlobalData::shared()->cd_time[maxIdx];
//            _bTmp = true;
//        }
//        else{
//            _bTmp = false;
//            for (int idx = 0; idx<=maxIdx; idx++) {
//                if (tmpTime <= GlobalData::shared()->cd_time[idx]) {
//                    ret += GlobalData::shared()->cd_cost[idx];
//                    break;
//                }
//            }
//        }
//    }
//    return ret;
    
    //秒CD花费金币数=max（time（秒）/(k1*(time（秒）/3600)^(k2/100))，1）
    float k1 = GlobalData::shared()->cd_gold["k1"];
    float k2 = GlobalData::shared()->cd_gold["k2"];
    float tmpv = k1*powf(t*1.0/3600, k2/100);
    ret = t/tmpv;
    ret = MAX(ret, 1);
    return ret;
}

int CCCommonUtils::getResGlodByType(int type, int num)
{
    int ret = 0;
    float a,b,c,d;
    if (num <= GlobalData::shared()->res_cost["k1"]) {
        a = GlobalData::shared()->res1_num["k1"];
        b = GlobalData::shared()->res1_num["k2"];
        c = GlobalData::shared()->res1_num["k3"];
    }else if (num > GlobalData::shared()->res_cost["k1"] && num <= GlobalData::shared()->res_cost["k2"]) {
        a = GlobalData::shared()->res1_num["k4"];
        b = GlobalData::shared()->res1_num["k5"];
        c = GlobalData::shared()->res1_num["k6"];
    }else if (num > GlobalData::shared()->res_cost["k2"] && num <= GlobalData::shared()->res_cost["k3"]) {
        a = GlobalData::shared()->res1_num["k7"];
        b = GlobalData::shared()->res1_num["k8"];
        c = GlobalData::shared()->res1_num["k9"];
    }else if (num > GlobalData::shared()->res_cost["k3"] && num <= GlobalData::shared()->res_cost["k4"]) {
        a = GlobalData::shared()->res1_num["k10"];
        b = GlobalData::shared()->res1_num["k11"];
        c = GlobalData::shared()->res1_num["k12"];
    }else if (num > GlobalData::shared()->res_cost["k4"]) {
        a = GlobalData::shared()->res2_num["k1"];
        b = GlobalData::shared()->res2_num["k2"];
        c = GlobalData::shared()->res2_num["k3"];
    }
    
    if (type == Food) {
        d = GlobalData::shared()->res3_num["k1"];
    }else if (type == Wood) {
        d = GlobalData::shared()->res3_num["k2"];
    }else if (type == Iron) {
        d = GlobalData::shared()->res3_num["k3"];
    }else if (type == Stone) {
        d = GlobalData::shared()->res3_num["k4"];
    }
    
    //金币数=int（a*（资源*d）^b+c）
    ret = powf(num*d, b)*a+c;
    ret = MAX(ret, 1);
    return ret;
    
    //////
//    map<string,float>* tmpVec = NULL;
//    if (type == Food || type == Wood) {
//        tmpVec = &(GlobalData::shared()->res1_num);
//    }
//    else if (type == Stone) {
//        tmpVec = &(GlobalData::shared()->res3_num);
//    }
//    else if (type == Iron) {
//        tmpVec = &(GlobalData::shared()->res2_num);
//    }
//    else {
//        return 0;
//    }
//    
//    int tmpNum = num;
//    bool _bTmp = true;
//    int maxIdx = tmpVec->size();
//    while (_bTmp) {
//        string key = "k";
//        key = key+CC_ITOA(maxIdx);
//        if (tmpVec->find(key) != tmpVec->end()) {
//            float curNum = (*tmpVec)[key];
//            if (tmpNum > curNum) {
//                ret += GlobalData::shared()->res_cost[key];
//                tmpNum -= curNum;
//                _bTmp = true;
//            }
//            else{
//                _bTmp = false;
//                for (int idx = 1; idx<=maxIdx; idx++) {
//                    string ck = "k";
//                    ck = ck+CC_ITOA(idx);
//                    if (tmpVec->find(ck) != tmpVec->end()) {
//                        float cn = (*tmpVec)[ck];
//                        if (tmpNum <= cn) {
//                            ret += GlobalData::shared()->res_cost[ck];
//                            break;
//                        }
//                    }
//                }
//            }
//        }
//    }
//    return ret;
}

CCSprite *CCCommonUtils::createGraySprite(const char* imageName)
{
    // create CCSprite
    CCSprite* sprite = CCLoadSprite::createSprite(imageName);
    return sprite;
    unsigned int width = sprite->getTexture()->getPixelsWide();
    unsigned int height = sprite->getTexture()->getPixelsHigh();
    
    // create CCRenderTexture base on the CCSprite
    CCRenderTexture* r = CCRenderTexture::create(width, height);
    r->beginWithClear(1, 1, 1, 0);
    sprite->setPosition(ccp(width / 2.0, height/ 2.0)); // Node: set position here!
    sprite->visit();
    r->end();
    
    // create a new CCImage
    CCImage* image = r->newCCImage();
    
    width = image->getWidth();
    height= image->getHeight();
    
    // this data is the texture data in memery
    unsigned char* data = image->getData();
    
    typedef enum {
        RED = 0,
        GREEN = 1,
        BLUE = 2,
        ALPHA = 3
    } PIXELS;
    
    // convert unsigned char*(1 Byte) to uint_32_t(4 Bytes)
    uint32_t *pixels = (uint32_t *)data;
    
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            uint8_t *rgbaPixel = (uint8_t *) &pixels[y * width + x];
            
            // convert to grayscale using recommended method: http://en.wikipedia.org/wiki/Grayscale#Converting_color_to_grayscale
            uint32_t gray = 0.3 * rgbaPixel[RED] + 0.59 * rgbaPixel[GREEN] + 0.11 * rgbaPixel[BLUE];
            
            // set the pixels to gray
            rgbaPixel[RED] = gray;
            rgbaPixel[GREEN] = gray;
            rgbaPixel[BLUE] = gray;
        }
    }
    
    // create a new CCTexture2D based on the CCImage data modified above
    CCTexture2D* texture = new CCTexture2D();
    texture->initWithImage(image);
    
    // create a new CCSprite to return
    CCSprite* s = CCSprite::create();
    s->initWithTexture(texture);
    // release other resources
    //    r->release();
    //    image->release();
    //    sprite->release();
    
    return s;
}


void CCCommonUtils::flyText(std::string floatText, cocos2d::ccColor3B color, float time){
    if(GuideController::share()->hasGuide()){
        return;
    }
    YesNoDialog::showYesDialog(floatText.c_str());
//    CCFloatingText* fly = CCFloatingText::create(floatText.c_str());
//    fly->setDuration(time);
//    fly->setColor(color);
//    auto topLayer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_POPUP);
//    fly->cocos2d::CCNode::setPosition(CCDirector::sharedDirector()->getWinSize().width / 2, CCDirector::sharedDirector()->getWinSize().height / 2);
//    topLayer->addChild(fly);
//    fly->startAnimation();
}
void CCCommonUtils::flyTextOnParent(std::string floatText, cocos2d::CCNode *parent,cocos2d::CCPoint point,cocos2d::ccColor3B color){
    if(!parent){
        flyText(floatText,color);
    }else{
        CCFloatingText* fly = CCFloatingText::create(floatText.c_str());
        fly->setColor(color);
        fly->cocos2d::CCNode::setPosition(point);
        parent->addChild(fly);
        fly->startAnimation();
    }
}
void CCCommonUtils::flyUiResText(std::string floatText, cocos2d::CCNode *parent,cocos2d::CCPoint point,cocos2d::ccColor3B color, FloatingTypeEnum floatType, int fontSize){
    if(!parent){
        flyText(floatText,color);
    }else{
        CCFloatingText* fly = CCFloatingText::create(floatText.c_str(), fontSize);
        fly->setColor(color);
        fly->setFloatingType(floatType);
        fly->cocos2d::CCNode::setPosition(point);
        parent->addChild(fly);
        fly->startAnimation();
    }
}
void CCCommonUtils::createGoodsIcon(int toolID, cocos2d::CCNode *mParent,CCSize defSize,CCLabelIF* mNum,CCLabelIF* mName, CCLabelIF* mDes, float op){
    if(ToolController::getInstance()->m_toolInfos.find(toolID)!=ToolController::getInstance()->m_toolInfos.end()){
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(toolID);
        auto pic = CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(info.itemId)).c_str(),true,CCLoadSpriteType_GOODS);
        pic->setOpacity(op);
        CCSize pSize = mParent->getContentSize();
        if (pic) {
            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(info.color).c_str());
            CCCommonUtils::setSpriteMaxSize(iconBg, defSize.width,true);
            mParent->addChild(iconBg);
            iconBg->setTag(GOODS_BG_TAG);
            CCCommonUtils::setSpriteMaxSize(pic, defSize.width - 5,true);
            mParent->addChild(pic);
            pic->setTag(GOODS_ICON_TAG);
            
            if(info.type==3 && !info.getPara().empty()){
                CCScale9Sprite* numBG = CCLoadSprite::createScale9Sprite("BG_quatnity.png");
                numBG->setColor(getItemColor(info.color));
                numBG->setOpacity(200);
                CCLabelBMFont* numIF = CCLabelBMFont::create(CC_ITOA_K(atol(info.getPara().c_str())), "pve_fnt_boss.fnt");
                CCSize numSize = numIF->getContentSize();
                
                const float constScale = 0.3;
                float scale = defSize.height*constScale/numSize.height;
                if((numSize.width * scale) > defSize.width){
                    scale = defSize.width/numSize.width;
                }
                numIF->setScale(scale);
                numSize.height *= scale;
                numBG->setPreferredSize(CCSize(defSize.width, defSize.height*constScale));
                mParent->addChild(numBG);
                mParent->addChild(numIF);
                
                if(!pSize.equals(CCSizeZero)){
                    float pos = defSize.width*0.5;
                    numIF->setPosition(ccp(pos, pos + defSize.height * constScale * 0.5));
                }else{
                    numIF->setPositionY(defSize.height * constScale * 0.5);
                }
                numBG->setPosition(numIF->getPosition());
            }
            if(!pSize.equals(CCSizeZero)){
                float pos = defSize.width*0.5;
                iconBg->setPosition(ccp(pos, pos));
                pos = (defSize.width - 5)*0.5;
                pic->setPosition(ccp(pos, pos));
            }
        }
        
        if(mName){
            mName->setString(info.getName().c_str());
        }
        if(mDes){
            mDes->setString(_lang(info.des).c_str());
        }
        if(mNum){
            mNum->setString(CC_CMDITOA(info.getCNT()));
        }
    }else{
        if(mName){
            mName->setString("");
        }
        if(mDes){
            mDes->setString("");
        }
        if(mNum){
            mNum->setString("");
        }
    }
}
void CCCommonUtils::setButtonTitle(CCControlButton *button, const char *str) {
    string title = str;
    button->setTitleForState(title, CCControlStateNormal);
    button->setTitleForState(title, CCControlStateHighlighted);
    button->setTitleForState(title, CCControlStateDisabled);
}

void CCCommonUtils::setButtonTitleColor(CCControlButton *button, ccColor3B color) {
    button->setTitleColorForState(color, CCControlStateNormal);
    button->setTitleColorForState(color, CCControlStateHighlighted);
    button->setTitleColorForState(color, CCControlStateDisabled);

}
void CCCommonUtils::setButtonTitlePoint(CCControlButton *button, const CCPoint point) {
    button->getTitleLabel()->setAnchorPoint(CCPoint(0.5, 0.5));
    button->getTitleLabel()->setPosition(point);
}

void CCCommonUtils::setButtonSprite(CCControlButton *button, const char *str) {
    if (button && str) {
        button->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite(str), CCControlStateNormal);
        button->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite(str), CCControlStateHighlighted);
        button->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite(str), CCControlStateDisabled);
    }
}

CCArray* CCCommonUtils::split(const char* src, const char* sym)
{
    CCArray * ret = CCArray::create();
    
    std::string s = src;
    
    int nend=0;
    int nbegin=0;
    while(nend != -1)
    {
        nend = s.find(sym, nbegin);
        if(nend == -1)
            ret->addObject(CCString::create(s.substr(nbegin, s.length()-nbegin)));
        else
            ret->addObject(CCString::create(s.substr(nbegin, nend-nbegin)));
        nbegin = nend + strlen(sym);
    }
    return ret;
}

void CCCommonUtils::splitString(const std::string& strSrc, const std::string& strFind, std::vector<std::string>& arSplit) {
    string tmpStr(strSrc.data(),strSrc.length());
    
    if(tmpStr.length() > strFind.length())
    {
        while (!tmpStr.empty() && tmpStr.find(strFind.c_str()) == 0) {
            tmpStr = tmpStr.substr(strFind.length(), tmpStr.length()-strFind.length());
        }
        while (!tmpStr.empty() && tmpStr.rfind(strFind.c_str()) == (tmpStr.length()-strFind.length())) {
            tmpStr = tmpStr.substr(0, tmpStr.length()-strFind.length());
        }
    }
    
    char* src = const_cast<char*>(tmpStr.c_str());
    while (src != NULL && !tmpStr.empty()) {
        arSplit.push_back(std::string(strtok_r(src, strFind.c_str(), &src)));
    }
}
std::string CCCommonUtils::replaceString(const std::string &str, const std::string &src, const std::string &dest){
    string ret;
    
    string::size_type pos_begin = 0;
    string::size_type pos       = str.find(src);
    while (pos != string::npos)
    {
        ret.append(str.data() + pos_begin, pos - pos_begin);
        ret += dest;
        pos_begin = pos + 1;
        pos       = str.find(src, pos_begin);
    }
    if (pos_begin < str.length())
    {
        ret.append(str.begin() + pos_begin, str.end());
    }
    return ret;
}
int CCCommonUtils::getGoldBuyItem(std::string itemId){
    int value = atoi(CCCommonUtils::getPropById(itemId, "price").c_str());
    return value;
}

void CCCommonUtils::recordStep(std::string index){
    if(index == ""){
        return;
    }
    GuideRecordCommand* cmd = new GuideRecordCommand(index);
    cmd->sendAndRelease();
}

//void CCCommonUtils::recordStepByHttp(std::string index){
//    string _uuid = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_UUID,"");
//    if(_uuid != ""){
//        return;
//    }
//    CCHttpRequest* request = new CCHttpRequest();
//    
//    CCString* url;
//    url = CCString::createWithFormat("http://%s:8080/gameservice/settutorialstep?id=%s",SERVERLIST_IP, index.c_str());
//    
//    request->setUrl(url->getCString());
//    request->setRequestType(CCHttpRequest::Type::POST);
//    CCHttpClient::getInstance()->setTimeoutForConnect(1);
//    CCHttpClient::getInstance()->send(request);
//    request->release();
//}
std::string CCCommonUtils::getBonusString(cocos2d::CCArray *rewards){
    std::string ret = "";
    CCObject* reward;
    CCARRAY_FOREACH(rewards, reward){
        auto bon = _dict(reward);
        int type = bon->valueForKey("type")->intValue();
        std::string typeString = "";
        std::string itemId = "";
        switch (type) {
            case Silver:
                typeString = "105002";
                break;
            case Wood:
                typeString = "105003";
                break;
            case Iron:
                typeString = "105005";
                break;
            default:
                break;
        }
    }
    if(ret != ""){
       return ret.substr(0, ret.length() - 1);
    }
    return ret;
    rewards->release();
}
std::string CCCommonUtils::getMoreLanguageType(std::string sysTemKey){
    string type = "";
    if(sysTemKey=="zh_TW"){
        type = "1";
    }else if(sysTemKey=="zh_CN"){
        type = "1";
    }else if(sysTemKey=="pt"){
        type = "1";
    }else if(sysTemKey=="tr"){
        type = "1";
    }else if(sysTemKey=="no"){
        type = "1";
    }else if(sysTemKey=="ja"){
        type = "1";
    }else if(sysTemKey=="nl"){
        type = "1";
    }else if(sysTemKey=="it"){
        type = "1";
    }else if(sysTemKey=="de"){
        type = "1";
    }else if(sysTemKey=="es"){
        type = "1";
    }else if(sysTemKey=="th"){
        type = "1";
    }else if(sysTemKey=="ru"){
        type = "1";
    }else if(sysTemKey=="en"){
        type = "1";
    }
    return type;
}
std::string CCCommonUtils::getGeneralPicById(std::string generalId){
    std::string face = CCCommonUtils::getPropById(generalId, "icon");
    return face.append(".png");
}

std::string CCCommonUtils::getGeneralBustPicById(std::string generalId){
    std::string face = CCCommonUtils::getPropById(generalId, "icon");
    return face.append("_bust.png");
}

std::string CCCommonUtils::getNameById(std::string xmlId){
    std::string dialogId = CCCommonUtils::getPropById(xmlId, "name");
    return _lang(dialogId);
}

std::string CCCommonUtils::getPropById(std::string xmlId, std::string propName){
    auto retDict = LocalController::shared()->DBXMLManager()->getObjectByKey(xmlId);    
    if (!retDict) {
        return string("");
    }
    auto ret = retDict->valueForKey(propName);
    if (!ret->getCString()) {
        return string("");
    }
    return string(ret->getCString());
}

std::string CCCommonUtils::getResourceIconByType(int resourceType) {
    std::string fix = ".png";
    switch (resourceType) {
        case Silver:
            return "ui_steel" + fix;
        case Wood:
            return "ui_wood" + fix;
        case Iron:
            return "ui_iron" + fix;
        case Food:
            return "ui_food" + fix;
        case Stone:
            return "ui_stone" + fix;
        case Gold:
            return "ui_gold" + fix;
        case 6:
            return "icon_exp" + fix;
        case 10://R_HONOR
            return "Contribution_icon" + fix;
        case 11://R_ALLIANCE_POINT
            return "Contribution_icon1" + fix;
        case 14://R_EQUIP
//            return "item410" + fix;
        case DragonFood:
            return "ui_food" + fix;
        case DragonGold:
            return "ui_gold" + fix;
        case LongJing:
            return "shuijing" + fix;
        default:
            break;
    }
    return std::string("");
}
CCSprite* CCCommonUtils::getRomanSprite(int num, int type){
    string path = "Roman_";
    if (type == 1) {
        path = "Roman_g_";
    }
    path.append(CC_ITOA(num));
    path.append(".png");
    CCSprite* spr = CCLoadSprite::createSprite(path.c_str());
    return spr;
}
int CCCommonUtils::getResourceLoadByType(int type){
    switch (type) {
        case Wood:
            return GlobalData::shared()->resources_weight_1;
        case Stone:
            return GlobalData::shared()->resources_weight_2;
        case Iron:
            return GlobalData::shared()->resources_weight_3;
        case Food:
            return GlobalData::shared()->resources_weight_4;
        case Silver:
            return GlobalData::shared()->resources_weight_5;
        case Gold:
            return GlobalData::shared()->resources_weight_6;
        default:
            break;
    }
    return 1;
}

std::string CCCommonUtils::getArmIconByType(int armType) {
    std::string fix = "arm";
    fix = fix+CC_ITOA(armType)+"_icon.png";
    return fix;
}

std::string CCCommonUtils::getQueueIconByType(int queueType) {
    std::string fix = "_icon.png";
    switch (queueType) {
        case QueueType::TYPE_BUILDING:
            return "building" + fix;
        case QueueType::TYPE_FORCE:
        case QueueType::TYPE_RIDE_SOLDIER:
        case QueueType::TYPE_BOW_SOLDIER:
        case QueueType::TYPE_CAR_SOLDIER:
            return "force" + fix;
        case QueueType::TYPE_FORT:
            return "fort" + fix;
        case QueueType::TYPE_HOSPITAL:
            return "hospital" + fix;
        case QueueType::TYPE_AFFAIRS:
            return "affairs" + fix;
        case QueueType::TYPE_ALLIANCE_AFFAIRS:
            return "affairs" + fix;
        case QueueType::TYPE_OCCUPY_RESOURCE:
        case QueueType::TYPE_OCCUPY_ALLIANCE:
        case QueueType::TYPE_OCCUPY_MAZE:
        case QueueType::TYPE_OCCUPY_CAMP:
        case QueueType::TYPE_MARCH:
            return "march"+fix;
        case QueueType::TYPE_ARMY_MASS:{
            return "jijie.png";
        }
        case QueueType::TYPE_SCIENCE:
            return "science"+fix;
        case QueueType::TYPE_FORGE:
            return "science"+fix;
        case QueueType::TYPE_MATE:
            return "science"+fix;
        case QueueType::TYPE_BUILDING_TERRITORY:
            return "tile_pop_icon23.png";
        case QueueType::TYPE_REPAIR_TERRITORY:
            return "tile_pop_icon25.png";
        case QueueType::TYPE_STATION_TERRITORY:
            return "tile_pop_icon24.png";
        case QueueType::TYPE_DESTROY_TERRITORY:
            return "territory_destroy.png";
        case QueueType::TYPE_OCCUPY_TERRITORY_RESOURCE:
            return "tile_pop_icon27.png";
        default:
            break;
    }
    return std::string("");
}

std::string CCCommonUtils::getQueueNameByType(int queueType) {
    switch (queueType) {
        case QueueType::TYPE_BUILDING:
            return _lang("103650");
        case QueueType::TYPE_MARCH:
            return _lang("103651");
        case QueueType::TYPE_FORCE:
            return _lang("103652");
        case QueueType::TYPE_FORT:
            return _lang("103655");
        case QueueType::TYPE_HOSPITAL:
            return _lang("103653");
        case QueueType::TYPE_AFFAIRS:
            return _lang("103654");
        case TYPE_ALLIANCE_AFFAIRS:
            return _lang("103668");
        case QueueType::TYPE_SCIENCE:
            return _lang("103656");
        case QueueType::TYPE_FORGE:
            return _lang("103656");
        case QueueType::TYPE_MATE:
            return _lang("103656");
        default:
            break;
    }
    return std::string("");
}

int CCCommonUtils::getQueueSortByType(int queueType) {
    if(queueType == 15)
    {
        return 15;//fusheng 泰坦15 和TYPE_DRAGON_BRON,//龙孵化 一个值  不敢加enum
    }
    switch (queueType) {
        case QueueType::TYPE_BUILDING://建筑
            return 1;
        case QueueType::TYPE_SCIENCE://科技
            return 3;
        case QueueType::TYPE_FORCE://兵力
            return 4;
        case QueueType::TYPE_FORT://城防
            return 8;
        case QueueType::TYPE_HOSPITAL://医院
            return 5;
        case QueueType::TYPE_AFFAIRS://事务
            return 6;
        case TYPE_ALLIANCE_AFFAIRS:
            return 9;
        case QueueType::TYPE_MARCH://出征
            return 2;
        case QueueType::TYPE_RIDE_SOLDIER://骑兵
            return 10;
        case QueueType::TYPE_BOW_SOLDIER://弓兵
            return 11;
        case QueueType::TYPE_CAR_SOLDIER://车兵
            return 12;
        case QueueType::TYPE_FORGE://造装备
            return 13;
        case QueueType::TYPE_MATE://造材料
            return 14;
        default:
            break;
    }
    return 0;
}

std::string CCCommonUtils::getResourceProductByType(int resourceType){
    switch (resourceType) {
        case Silver:
            return _lang("108580");
        case Wood:
            return _lang("108576");
        case Iron:
            return _lang("108578");
        case Food:
            return _lang("108579");
        case Stone:
            return _lang("108577");
        case Gold:
            return _lang("108581");
        default:
            break;
    }
    return std::string("");
}

std::string CCCommonUtils::getResourceNameByType(int resourceType) {
    switch (resourceType) {
        case Silver:
            return _lang("102212");
        case Wood:
            return _lang("102209");
        case Iron:
            return _lang("102210");
        case Food:
            return _lang("102211");
        case Stone:
            return _lang("102208");
        case Gold:
            return _lang("107518");
        case Chip:
            return _lang("111101");
        case Diamond:
            return _lang("111102");
        case DragonFood:
            return _lang("102211");
        case DragonGold:
            return _lang("107518");
        case LongJing:
            return _lang("111654");//水晶
        default:
            break;
    }
    return std::string("");
}


bool CCCommonUtils::isEnoughResourceByType(int resourceType, int num) {
    switch (resourceType) {
        case Silver:
            if (GlobalData::shared()->resourceInfo.lMoney >= num) {
                return true;
            }
            return false;
        case Gold:
            if (GlobalData::shared()->playerInfo.gold >= num) {
                return true;
            }
            return false;

        case Wood:
            if (GlobalData::shared()->resourceInfo.lWood >= num) {
                return true;
            }
            return false;
        case Iron:
            if (GlobalData::shared()->resourceInfo.lIron >= num) {
                return true;
            }
            return false;
        case Food:
            if (GlobalData::shared()->resourceInfo.lFood >= num) {
                return true;
            }
            return false;
        case Stone:
            if (GlobalData::shared()->resourceInfo.lStone >= num) {
                return true;
            }
            return false;
        case LongJing:
            if (GlobalData::shared()->playerInfo.longJing >= num) {
                return true;
            }
            return false;
        default:
            break;
    }
    return false;
}

int CCCommonUtils::getCurResourceByType(int resourceType) {
    switch (resourceType) {
        case Silver:
            return GlobalData::shared()->resourceInfo.lMoney;
        case Wood:
            return GlobalData::shared()->resourceInfo.lWood;
        case Iron:
            return GlobalData::shared()->resourceInfo.lIron;
        case Food:
            return GlobalData::shared()->resourceInfo.lFood;
        case Stone:
            return GlobalData::shared()->resourceInfo.lStone;
        default:
            break;
    }
    return 0;
}

void CCCommonUtils::fitContainerSizeAndAddChild(cocos2d::CCNode *container, cocos2d::CCNode *child , int type) {
    auto containerSize = container->getContentSize();
    auto childSize = child->getContentSize();
    switch (type) {
        case 0:{
            child->setScaleX(containerSize.width/childSize.width);
            child->setScaleY(containerSize.height/childSize.height);
        }
            break;
        case 1:{
            child->setScale(containerSize.width/childSize.width);
        }
            break;
        case 2:{
            child->setScale(containerSize.height/childSize.height);
        }
            break;
            
        default:
            return;
    }
    child->setAnchorPoint(ccp(0.5, 0.5));
    child->setPosition(ccp(containerSize.width/2, containerSize.height/2));
    container->addChild(child);
}

std::string CCCommonUtils::getIcon(std::string id){
    return CCCommonUtils::getPropById(id, "icon").append(".png");
}

bool CCCommonUtils::isEquip(int itemId){
    if ( itemId >= 1000000 && itemId <= 1999999) {
        return true;
    }
    return false;
}

cocos2d::ccColor3B CCCommonUtils::getItemColor(int color){
    switch (color) {
        case WHITE:
            return ccc3(227, 227, 227);
            break;
        case GREEN:
            return ccc3(36, 255, 0);
            break;
        case BLUE:
            return ccc3(0, 132, 255);
            break;
        case PURPLE:
            return ccc3(246, 0, 255);
            break;
        case ORANGE:
            return ccc3(255, 138, 0);
            break;
        case GOLDEN:
            return ccc3(255, 210, 0);
            break;
        case RED:
            return ccc3(255, 0, 0);
            break;
        default:
            break;
    }
    return ccWHITE;
}

string CCCommonUtils::getColorName(int color)
{
    switch (color) {
        case WHITE:
            return _lang("119043");
            break;
        case GREEN:
            return _lang("119044");
            break;
        case BLUE:
            return _lang("119045");
            break;
        case PURPLE:
            return _lang("119046");
            break;
        case ORANGE:
            return _lang("119047");
            break;
        case GOLDEN:
            return _lang("119048");
            break;
        case RED:
            return _lang("119049");
            break;
        default:
            break;
    }
    return "";
}

std::string CCCommonUtils::getToolBgByColor(int color)
{
    switch (color) {
        case WHITE:
            return "tool_1.png";
            break;
        case GREEN:
            return "tool_2.png";
            break;
        case BLUE:
            return "tool_3.png";
            break;
        case PURPLE:
            return "tool_4.png";
            break;
        case ORANGE:
            return "tool_5.png";
            break;
        case GOLDEN:
            return "tool_6.png";
            break;
        case RED:
            return "tool_7.png";
            break;
        default:
            break;
    }
    return "tool_1.png";
}

std::string CCCommonUtils::getGenHeadBgByColor(int color)
{
    string ret = "general_pic_bg_";
    ret = ret + CC_ITOA(color)+".png";
    return ret;
}

bool CCCommonUtils::isBtnOpen(int btnIndex){
    vector<std::string>vector;
    if (GlobalData::shared()->playerInfo.openedSystem == "") {
        return false;
    }
    splitString(GlobalData::shared()->playerInfo.openedSystem, ",", vector);
    int i = 0;
    while(i < vector.size()){
        if(atoi(vector[i].c_str()) == btnIndex){
            return true;
        }
        i++;
    }
    return false;
}

bool CCCommonUtils::isLackOfGold(int i){
    if(GlobalData::shared()->playerInfo.gold < i){
        return true;
    }
    return false;
}
std::string CCCommonUtils::timeLeftToCountDown(int time,const char* separator) {
    unsigned int tmpTime = time < 0 ? 0 : time;

    std::string ret = string("");
    int hours = tmpTime/3600;
    if (hours < 10) {
        ret = ret + "0";
    }
    ret = ret + CC_ITOA(hours) + separator;
    int tmp = tmpTime%3600;
    int minutes = tmp/60;
    if (minutes < 10) {
        ret = ret + "0";
    }
    ret = ret + CC_ITOA(minutes) +separator;
    int seconds = tmp%60;
    if (seconds < 10) {
        ret = ret + "0";
    }
    ret = ret + CC_ITOA(seconds);
    return ret;
}
std::string CCCommonUtils::getTalentStr(std::string talent, int type){
    std::string ret = "";
    if(strstr(talent.c_str(), "|") != NULL){
        std::string talentTypeStr = talent.substr(talent.find_first_of("|") + 1);
        std::string valueStr = talent.substr(0, talent.find_first_of("|"));
        vector<std::string> vector;
        CCCommonUtils::splitString(talentTypeStr, ";", vector);
        int i = 0;
        while(i < vector.size()){
            int talentType = atoi(vector[i].c_str());
            std::string dialogId = "1054";
            dialogId.append(CC_ITOA(talentType + 2)).append(CC_ITOA(type));
            ret.append(_lang(dialogId.c_str())).append("\n");
            i++;
        }
    }
    return ret;
}

std::string CCCommonUtils::getTacticsStr(std::string talent, int type){
    if(talent == ""){
        return "";
    }
    std::string valueStr = talent.substr(0, talent.find_first_of("|"));
    std::string dialogId = "";
    switch (type) {
        case MOUNTAIN:
            dialogId = "105411";
            break;
        case WATER:
            dialogId = "105412";
            break;
        case PLAIN:
            dialogId = "105413";
            break;
        case CITY:
            dialogId = "105414";
            break;
        default:
            return "";
            break;
    }
    return _lang(dialogId).append(" + ").append(valueStr).append("%");
}

std::string CCCommonUtils::getItemPropStr(EquipInfo* info, std::string att, int value){
    std::string result = "";
    if(att == ""){
        return "";
    }
    int max = atoi(CCCommonUtils::getPropById(info->itemId, "max").c_str());
    result = _lang(CC_ITOA(atoi(info->attr1.c_str()) + 105800));
    result = _lang_2("105800", result.c_str(), CC_ITOA(value));
    if(info->attrLevel1 >= max){
        result.append(_lang("105815"));
    }
    return result;
}

int CCCommonUtils::getSoulExChangeNeedByColor(int color){
    int num = 1;
    if(color == GREEN){
        num = GlobalData::shared()->generalConfig.greenNum;
    }else if(color == BLUE){
        num = GlobalData::shared()->generalConfig.blueNum;
    }else if(color == PURPLE){
        num = GlobalData::shared()->generalConfig.purpleNum;
    }
    return num;
}

std::string CCCommonUtils::getGeneralSkill(std::string generalId){
    vector<std::string> vector;
    std::string skillId = CCCommonUtils::getPropById(generalId, "skill");
    CCCommonUtils::splitString(skillId, ";", vector);
    return vector[1];
}

bool CCCommonUtils::isFormationOpen(int index){
    return true;
}

int CCCommonUtils::getFormationOpenNum(){
    int level = GlobalData::shared()->playerInfo.level;
    int count = 0;
    map<int, int>::iterator it;
    for(it = GlobalData::shared()->generalConfig.formationOpen.begin()
        ; it != GlobalData::shared()->generalConfig.formationOpen.end()
        ; it++){
        if(level >= it->first && it->second > count){
            count = it->second;
        }
    }
    return count;
}

int CCCommonUtils::getNextFormationOpenLevel(){
    int level = GlobalData::shared()->playerInfo.level;
    int result = -1;
    map<int, int>::iterator it;
    for(it = GlobalData::shared()->generalConfig.formationOpen.begin()
        ; it != GlobalData::shared()->generalConfig.formationOpen.end()
        ; it++){
        if(level < it->first && (result > it->first || result == -1)){
            result = it->first;
        }
    }
    return result;
}

float CCCommonUtils::getEffectValueByNum(int num, bool isInludeGeneral)
{
    float ret = 0;
    if (1) {
        if(GlobalData::shared()->effectStateMap.find(num) != GlobalData::shared()->effectStateMap.end())
        {
            auto& info = GlobalData::shared()->effectStateMap[num];
            for (int i=0; i<info.size(); i++)
            {
                if (GlobalData::shared()->getWorldTime()<GlobalData::shared()->statusMap[info[i].stateId]) {
                    ret += info[i].value;
                }
            }
        }
    }
    if(isInludeGeneral){
//        if(num == ARMY_LOAD_EFFECT){
//            ret += GlobalData::shared()->effectValues[num] + getVipEffectValueByNum(num) + GeneralManager::getInstance()->getValueByEffect(GENERAL_LOAD_EFFECT);
//        }else{
            ret += GlobalData::shared()->effectValues[num] + getVipEffectValueByNum(num) + GeneralManager::getInstance()->getValueByEffect(num) + getOfficerValueByNum(num) + getTerritoryEffectValueByNum(num);
//        }
    }else{
        ret += GlobalData::shared()->effectValues[num] + getVipEffectValueByNum(num) + getOfficerValueByNum(num);
    }
    
    ret += EquipmentController::getInstance()->EquipEffMap[num];
    
    if(GlobalData::shared()->playerInfo.isInAlliance()) {
        ret += ScienceController::getInstance()->alScEffMap[num];
    }
    
    return ret;
}
float CCCommonUtils::getOfficerValueByNum(int num){
    float ret = 0.0;
    if(GlobalData::shared()->playerInfo.officer != ""){
        std::string param1 = getPropById(GlobalData::shared()->playerInfo.officer, "para1");
        std::string param2 = getPropById(GlobalData::shared()->playerInfo.officer, "para2");
        if(param1 != "" && param2 != ""){
            std::vector<std::string> effect;
            std::vector<std::string> value;
            CCCommonUtils::splitString(param1, ";", effect);
            CCCommonUtils::splitString(param2, ";", value);
            if(effect.size() != value.size()){
                return ret;
            }
            int index = 0;
            while (index < effect.size()) {
                int effectValue = atoi(effect[index].c_str());
                if(effectValue == num){
                    float valueValue = atoi(value[index].c_str());
                    return valueValue;
                }
                index++;
            }
        }
    }
    return ret;
}

float CCCommonUtils::getTerritoryEffectValueByNum(int num){
    float ret = 0.0;
    if (GlobalData::shared()->alliance_territory_positive_effect > 0 || GlobalData::shared()->alliance_territory_positive_effect_gather == 1) {
        map<int, int> territoryEffectMap;
        CCDictElement *element;
        auto dictEffect = LocalController::shared()->DBXMLManager()->getGroupByKey("territory_effect");
        CCDICT_FOREACH(dictEffect, element){
            std::string effectId = element->getStrKey();
            std::string buffStr = CCCommonUtils::getPropById(effectId, "k2");
            std::string numStr = CCCommonUtils::getPropById(effectId, "k1");
            std::vector<std::string> effect;
            CCCommonUtils::splitString(numStr, ";", effect);
            if (effect.size() == 1) {
                int value = atoi(buffStr.c_str());
                int numTemp = atoi(effect[0].c_str());
                territoryEffectMap[numTemp] = value;
            }
            else if (effect.size() == 4) {
                for (int i = 0; i < 4; i++) {
                    int value = atoi(buffStr.c_str());
                    int numTemp = atoi(effect[i].c_str());
                    territoryEffectMap[numTemp] = value;
                }
            }
        }
        if (num != 65) {
            map<int, int>::iterator tItr;
            tItr = territoryEffectMap.find(num);
            if (tItr != territoryEffectMap.end()) {
                ret += territoryEffectMap[num] * GlobalData::shared()->alliance_territory_positive_effect;
                return ret;
            }
            else
                return ret;
        }
        else {
            if (GlobalData::shared()->alliance_territory_positive_effect_gather == 1) {
                ret += territoryEffectMap[num] * GlobalData::shared()->alliance_territory_positive_effect_gather;
                GlobalData::shared()->alliance_territory_positive_effect_gather = 0;
                return ret;
            }
            else
                return ret;
        }
    }
    return ret;
}

float CCCommonUtils::getTerritoryNegativeEffectValueByNum(int num){
    float ret = 0.0;
    if (GlobalData::shared()->alliance_territory_negative_effect > 0) {
        map<int, int> territoryEffectMap;
        CCDictElement *element;
        auto dictEffect = LocalController::shared()->DBXMLManager()->getGroupByKey("territory_effect");
        CCDICT_FOREACH(dictEffect, element){
            std::string effectId = element->getStrKey();
            std::string buffStr = CCCommonUtils::getPropById(effectId, "k2");
            std::string numStr = CCCommonUtils::getPropById(effectId, "k1");
            std::vector<std::string> effect;
            CCCommonUtils::splitString(numStr, ";", effect);
            if (effect.size() == 1) {
                int value = atoi(buffStr.c_str());
                int numTemp = atoi(effect[0].c_str());
                territoryEffectMap[numTemp] = value;
            }
            else if (effect.size() == 4) {
                for (int i = 0; i < 4; i++) {
                    int value = atoi(buffStr.c_str());
                    int numTemp = atoi(effect[i].c_str());
                    territoryEffectMap[numTemp] = value;
                }
            }
        }
        return territoryEffectMap[num] * GlobalData::shared()->alliance_territory_negative_effect;
    }
    return ret;
}

float CCCommonUtils::getVipEffectValueByNum(int num)
{
    float ret = 0;
    if (GlobalData::shared()->playerInfo.vipEndTime > GlobalData::shared()->getWorldTime()) {
        if(GlobalData::shared()->vipEffectMap.find(num) != GlobalData::shared()->vipEffectMap.end())
        {
            ret += GlobalData::shared()->vipEffectMap[num];
        }
    }
    return ret;
}

int CCCommonUtils::getQueueTypeByBuildType(int buildType)
{
    if (buildType == FUN_BUILD_SCIENE) {
        return TYPE_SCIENCE;
    }
    else if (buildType == FUN_BUILD_BARRACK1) {
        return TYPE_FORCE;
    }
    else if (buildType == FUN_BUILD_BARRACK2) {
        return TYPE_RIDE_SOLDIER;
    }
    else if (buildType == FUN_BUILD_BARRACK3) {
        return TYPE_BOW_SOLDIER;
    }
    else if (buildType == FUN_BUILD_BARRACK4) {
        return TYPE_CAR_SOLDIER;
    }
    else if (buildType == FUN_BUILD_FORT) {
        return TYPE_FORT;
    }
    else if (buildType == FUN_BUILD_HOSPITAL) {
        return TYPE_HOSPITAL;
    }
    else if (buildType == FUN_BUILD_FORGE) {
        return TYPE_FORGE;
    }
    else if (buildType == FUN_BUILD_WORKSHOP) {
        return TYPE_MATE;
    }
    else if (buildType == DBUILDING_HATCHER) {
        return TYPE_DRAGON_BRON;
    }
    else if (buildType == DBUILDING_CAVE) {
        return TYPE_DRAGON_EGG;
    }

    return -1;
}

CCSprite* CCCommonUtils::setSpriteMaxSize(CCSprite* spr, int limitNum, bool isForce)
{
    if (isForce || spr->getContentSize().width>limitNum || spr->getContentSize().height>limitNum) {
        float sc1 = limitNum*1.0/spr->getContentSize().width;
        float sc2 = limitNum*1.0/spr->getContentSize().height;
        spr->setScale( sc1<sc2?sc1:sc2 );
    }
    return spr;
}

void CCCommonUtils::setSpriteGray(CCSprite *sprite, bool gray){
    if (sprite == NULL) {
        return;
    }
    if (gray)
    {
//        TODO 暂时去掉etc
        if (sprite->getTexture()->getAlphaName() > 0)
        {
            sprite->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(GLProgram::SHADER_NAME_POSITION_ETC1_GRAY_NO_MVP));
        }
        else
        {
            sprite->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureGray));
        }
    }
    else
    {
        if (sprite->getTexture()->getAlphaName() > 0)
        {
            sprite->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(GLProgram::SHADER_NAME_POSITION_ETC1_A_NO_MVP));
        }
        else
        {
//            sprite->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture));
            sprite->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
    
        }
    }
}

void CCCommonUtils::setSprite3DGray(Sprite3D *pSprite3D, bool gray){
    if (pSprite3D == NULL) {
        return;
    }
    if (gray)
    {
        pSprite3D->setGLProgram(GLProgramCache::getInstance()->programForKey(kCCShader_PositionTextureGray));
    }
    else
    {
        pSprite3D->setGLProgram(GLProgramCache::getInstance()->programForKey(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
    }
}

CCSprite* CCCommonUtils::graylightWithCCSprite(CCSprite* oldSprite,bool isLight)
{
    //CCSprite转成CCimage
    CCPoint p = oldSprite->getAnchorPoint();
    oldSprite->setAnchorPoint(ccp(0,0));
    CCRenderTexture *outTexture = CCRenderTexture::create((int)oldSprite->getContentSize().width,(int)oldSprite->getContentSize().height);
    outTexture->begin();
    oldSprite->visit();
    outTexture->end();
    oldSprite->setAnchorPoint(p);
    
    CCImage* finalImage = outTexture->newCCImage();
    unsigned char *pData = finalImage->getData();
    int iIndex = 0;
    
    if(isLight)
    {
        for (int i = 0; i < finalImage->getHeight(); i ++)
        {
            for (int j = 0; j < finalImage->getWidth(); j ++)
            {
                // highlight
                int iHightlightPlus = 50;
                int iBPos = iIndex;
                unsigned int iB = pData[iIndex];
                iIndex ++;
                unsigned int iG = pData[iIndex];
                iIndex ++;
                unsigned int iR = pData[iIndex];
                iIndex ++;
                //unsigned int o = pData[iIndex];
                iIndex ++;  //原来的示例缺少
                iB = (iB + iHightlightPlus > 255 ? 255 : iB + iHightlightPlus);
                iG = (iG + iHightlightPlus > 255 ? 255 : iG + iHightlightPlus);
                iR = (iR + iHightlightPlus > 255 ? 255 : iR + iHightlightPlus);
                //            iR = (iR < 0 ? 0 : iR);
                //            iG = (iG < 0 ? 0 : iG);
                //            iB = (iB < 0 ? 0 : iB);
                pData[iBPos] = (unsigned char)iB;
                pData[iBPos + 1] = (unsigned char)iG;
                pData[iBPos + 2] = (unsigned char)iR;
            }
        }
    }else{
        for (int i = 0; i < finalImage->getHeight(); i ++)
        {
            for (int j = 0; j < finalImage->getWidth(); j ++)
            {
                // gray
                int iBPos = iIndex;
                unsigned int iB = pData[iIndex];
                iIndex ++;
                unsigned int iG = pData[iIndex];
                iIndex ++;
                unsigned int iR = pData[iIndex];
                iIndex ++;
                //unsigned int o = pData[iIndex];
                iIndex ++; //原来的示例缺少
                unsigned int iGray = 0.3 * iR + 0.4 * iG + 0.2 * iB;
                pData[iBPos] = pData[iBPos + 1] = pData[iBPos + 2] = (unsigned char)iGray;
            }
        }
    }
    
    CCTexture2D *texture = new CCTexture2D;
    texture->initWithImage(finalImage);
    CCSprite* newSprite = CCSprite::createWithTexture(texture);
    delete finalImage;
    texture->release();
    return newSprite;
}



string CCCommonUtils::getResourceStr(int resource) {
    if (resource < 0) {
        resource = 0;
    }
    string ret = CC_ITOA(resource);
    unsigned int size = ret.size();
    while (size > 3) {
        size -= 3;
        ret.insert(size, ",");
    }
    return ret;
}




bool CCCommonUtils::ButtonNode::isTouchInNode(cocos2d::CCTouch *pTouch) {
    auto parent = getParent();
    if (parent) {
        auto touch = parent->convertTouchToNodeSpace(pTouch);
        auto position = getPosition();
        float offsetX = m_touchSize.width/2.0;
        float offsetY = m_touchSize.height/2.0;
        if (touch.x > (position.x - offsetX)
            && touch.x < (position.x + offsetX)
            && touch.y > (position.y - offsetY)
            && touch.y < (position.y + offsetY))
        {
            return true;
        }
    }
    return false;
}

bool CCCommonUtils::ButtonNode::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if (m_touchBegan != NULL) {
        return m_touchBegan(this,pTouch,pEvent);
    }
    bool touchInNode = isTouchInNode(pTouch);
    if (touchInNode) {
        if (m_touchDoBegin != NULL) {
            m_touchDoBegin(touchInNode);
        }
        return true;
    }
    return false;
}

void CCCommonUtils::ButtonNode::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if (m_touchMoved != NULL) {
        return m_touchMoved(this,pTouch,pEvent);
    }
    if (m_touchDoMove != NULL) {
        m_touchDoMove(isTouchInNode(pTouch));
    }
}

void CCCommonUtils::ButtonNode::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if (m_touchEnded != NULL) {
        return m_touchEnded(this,pTouch,pEvent);
    }
    if (m_touchDoEnd != NULL) {
        m_touchDoEnd(isTouchInNode(pTouch));
    }
}

void CCCommonUtils::ButtonNode::onTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if (m_touchCancelled != NULL) {
        return m_touchCancelled(this,pTouch,pEvent);
    }
    if (m_touchDoCancel != NULL) {
        m_touchDoCancel(isTouchInNode(pTouch));
    }
}

void CCCommonUtils::ButtonNode::onEnter() {
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_priority, m_swallow);
}

void CCCommonUtils::ButtonNode::onExit() {
    setTouchEnabled(false);
    CCNode::onExit();
}

void CCCommonUtils::ButtonNode::setPriority(int priority) {
    if (m_priority != priority) {
        m_priority = priority;
        resetTouch();
    }
}

void CCCommonUtils::ButtonNode::setSwallow(bool isSwallow) {
    if (m_swallow != isSwallow) {
        m_swallow = isSwallow;
        resetTouch();
    }
}

void CCCommonUtils::ButtonNode::resetTouch() {
    // CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    // CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_priority, m_swallow);
}

void CCCommonUtils::flyHint(std::string icon, std::string titleText, std::string contentText, float time, float dy, bool isMid, float proY, std::string mailId){
    CCLOG("CCCommonUtils::flyHint");
    auto hint = FlyHint::create(icon, titleText, contentText, time,dy);
    
    if (isMid && hint != NULL) {
        auto size = CCDirector::sharedDirector()->getWinSize();
        hint->setPosition(size.width/2, size.height/2);
    }
    if (!isMid && proY>0 && hint != NULL) {
        auto size = CCDirector::sharedDirector()->getWinSize();
        hint->setPosition(size.width/2, size.height*proY);
    }

    if (mailId!="" && hint != NULL) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(!ChatServiceCocos2dx::isChatShowing)
        {
            hint->setMailId(mailId);
        }
#else
        if (ChatServiceCocos2dx::isChatShowing_fun()) {
              hint->setMailId(mailId);
        }
      
#endif
        
    }
    //hint->show();
}
void CCCommonUtils::flyHintWithDefault(std::string icon, std::string titleText, std::string contentText,float time, float dy){
    auto hint = FlyHint::create(icon, titleText,contentText,time,dy,true);
}
void CCCommonUtils::flySystemUpdateHint(double countDown, bool isLogin,string tip,FlyHintType type,string icon){
    LuaController::getInstance()->addSystemUpdate(countDown,isLogin,tip,type,icon);
}

void CCCommonUtils::makeEffectSpr(CCSprite* spr, string path, int num, float dt, int idx)
{
    int size = num;
    spr->setAnchorPoint(ccp(0,0));
    spr->setScale(1.0);
    Vector<SpriteFrame*> myArray;
    CCSpriteFrame *myframe[size];
    for (int i=0; i<size; i++) {
        myframe[i] = CCLoadSprite::loadResource(CCString::createWithFormat(path.c_str(), i+1)->getCString());
        myframe[i]->getTexture()->setAntiAliasTexParameters();
        myArray.pushBack(myframe[i]);
    }
    spr->setDisplayFrame(myframe[idx]);
    CCAnimation *animation=CCAnimation::createWithSpriteFrames(myArray, dt);
    CCAnimate* animate = CCAnimate::create(animation);
   // myArray->release();
    CCRepeatForever* action = CCRepeatForever::create(animate);
    spr->stopAllActions();
    spr->runAction(action);
}
void CCCommonUtils::makeInternalEffectSpr(CCSprite* spr, string path, int num, float dt, int idx,float delaytime){
    int size = num;
    spr->setAnchorPoint(ccp(0,0));
    spr->setScale(1.0);
    Vector<SpriteFrame*> myArray;
    CCSpriteFrame *myframe[size];
    for (int i=0; i<size; i++) {
        myframe[i] = CCLoadSprite::loadResource(CCString::createWithFormat(path.c_str(), i+1)->getCString());
        myframe[i]->getTexture()->setAntiAliasTexParameters();
        myArray.pushBack(myframe[i]);
    }
    spr->setDisplayFrame(myframe[idx]);
    CCAnimation *animation=CCAnimation::createWithSpriteFrames(myArray, dt);
    CCAnimate* animate = CCAnimate::create(animation);
   // myArray->release();
    CCActionInterval * delayAction = CCDelayTime::create(delaytime);
    CCSequence* sequence = CCSequence::create(delayAction,animate,NULL);
    CCRepeatForever* action = CCRepeatForever::create(sequence);
    spr->stopAllActions();
    spr->runAction(action);

}
void CCCommonUtils::makeOnceEffectSpr(CCSprite* spr, string path, int num, float delate, float dt)
{
    int size = num;
    spr->setAnchorPoint(ccp(0,0));
    spr->setScale(1.0);
    Vector<SpriteFrame*> myArray;
    CCSpriteFrame *myframe[size];
    for (int i=0; i<size; i++) {
        myframe[i] = CCLoadSprite::loadResource(CCString::createWithFormat(path.c_str(), i+1)->getCString());
        myframe[i]->getTexture()->setAntiAliasTexParameters();
        myArray.pushBack(myframe[i]);
    }
    spr->setDisplayFrame(myframe[0]);
    CCAnimation *animation=CCAnimation::createWithSpriteFrames(myArray, dt);
    CCAnimate* animate = CCAnimate::create(animation);
   // myArray->release();
    CCActionInterval * delaytime = CCDelayTime::create(delate);
    CCSequence* sequence = CCSequence::create(delaytime,animate,NULL);
    spr->stopAllActions();
    spr->runAction(sequence);
}

void CCCommonUtils::makeTwoAnimatSpr(CCSprite* spr, string path, int num, int splitNum, float dt, int idx)
{
    int size = num;
    spr->setAnchorPoint(ccp(0,0));
    spr->setScale(1.0);
    Vector<SpriteFrame*> myArray;
    Vector<SpriteFrame*> myArray1;
    CCSpriteFrame *myframe[size];
    for (int i=0; i<size; i++) {
        myframe[i] = CCLoadSprite::loadResource(CCString::createWithFormat(path.c_str(), i+1)->getCString());
        myframe[i]->getTexture()->setAntiAliasTexParameters();
        if (i<splitNum) {
            myArray.pushBack(myframe[i]);
        }
        else {
            myArray1.pushBack(myframe[i]);
        }
    }
    spr->setDisplayFrame(myframe[idx]);
    CCAnimation *animation=CCAnimation::createWithSpriteFrames(myArray, dt);
    CCAnimate* animate = CCAnimate::create(animation);
   // myArray->release();
    
    CCAnimation *animation1=CCAnimation::createWithSpriteFrames(myArray1, dt);
    CCAnimate* animate1 = CCAnimate::create(animation1);
   // myArray1->release();
    
    CCRepeat* repet = CCRepeat::create(animate, 4+rand()%20);
    CCRepeat* repet1 = CCRepeat::create(animate1, 1);
    CCActionInterval * delaytime = CCDelayTime::create(2+rand()%3);
    
    CCSequence* sequence = CCSequence::create(repet,repet1,delaytime,NULL);
    CCActionInterval * seqForever =CCRepeatForever::create((CCActionInterval* )sequence);
    spr->stopAllActions();
    spr->runAction(seqForever);
}

void CCCommonUtils::getRGBAProtocols(CCNode* parent, vector<CCNode*> * proVec)
{
    auto arr = parent->getChildren();
    if (arr.size()>0) {
        for (auto child : arr)
        {
            CCRGBAProtocol *pRGBAProtocol = dynamic_cast<CCRGBAProtocol*>(child);
            CCNode *pNode = dynamic_cast<CCNode*>(child);
            if (pRGBAProtocol && pNode) {
                proVec->push_back(pNode);
            }
            else {
                if (pNode) {
                    getRGBAProtocols(pNode, proVec);
                }
            }
        }
//        
//        for (int i=0; i<arr->count(); i++) {
//            CCRGBAProtocol *pRGBAProtocol = dynamic_cast<CCRGBAProtocol*>(arr->objectAtIndex(i));
//            CCNode *pNode = dynamic_cast<CCNode*>(arr->objectAtIndex(i));
//            if (pRGBAProtocol && pNode) {
//                proVec->push_back(pNode);
//            }
//            else {
//                if (pNode) {
//                    getRGBAProtocols(pNode, proVec);
//                }
//            }
//        }
    }
}

CCPoint CCCommonUtils::getCrossPoint(vector<CCPoint> &vector, CCPoint &c, CCPoint &d){
    int index = 0;
    while(index < vector.size()){
        CCPoint a = vector[index++];
        CCPoint b = vector[index++];
        
        // 三角形abc 面积的2倍
        float area_abc = (a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);
        
        // 三角形abd 面积的2倍
        float area_abd = (a.x - d.x) * (b.y - d.y) - (a.y - d.y) * (b.x - d.x);
        
        // 面积符号相同则两点在线段同侧,不相交 (对点在线段上的情况,本例当作不相交处理);
        if ( area_abc*area_abd>=0 ) {
            //                    return false;
            continue;
        }
        
        // 三角形cda 面积的2倍
        float area_cda = (c.x - a.x) * (d.y - a.y) - (c.y - a.y) * (d.x - a.x);
        // 三角形cdb 面积的2倍
        // 注意: 这里有一个小优化.不需要再用公式计算面积,而是通过已知的三个面积加减得出.
        float area_cdb = area_cda + area_abc - area_abd ;
        if (  area_cda * area_cdb >= 0 ) {
            //                    return false;
            continue;
        }
        
        //计算交点坐标
        float t = area_cda / ( area_abd- area_abc );
        float dx= t*(b.x - a.x),
        dy = t*(b.y - a.y);
        return ccp(a.x + dx, a.y + dy);
    }
    return ccp(-1, -1);
};

std::vector<CCPoint> CCCommonUtils::getCrossPoints(vector<CCPoint> &vector, CCPoint &c, CCPoint &d){
    std::vector<CCPoint> m_pts;
    int index = 0;
    while(index < vector.size()){
        CCPoint a = vector[index++];
        CCPoint b = vector[index++];
        
        // 三角形abc 面积的2倍
        float area_abc = (a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);
        
        // 三角形abd 面积的2倍
        float area_abd = (a.x - d.x) * (b.y - d.y) - (a.y - d.y) * (b.x - d.x);
        
        // 面积符号相同则两点在线段同侧,不相交 (对点在线段上的情况,本例当作不相交处理);
        if ( area_abc*area_abd>=0 ) {
            //                    return false;
            continue;
        }
        
        // 三角形cda 面积的2倍
        float area_cda = (c.x - a.x) * (d.y - a.y) - (c.y - a.y) * (d.x - a.x);
        // 三角形cdb 面积的2倍
        // 注意: 这里有一个小优化.不需要再用公式计算面积,而是通过已知的三个面积加减得出.
        float area_cdb = area_cda + area_abc - area_abd ;
        if (  area_cda * area_cdb >= 0 ) {
            //                    return false;
            continue;
        }
        
        //计算交点坐标
        float t = area_cda / ( area_abd- area_abc );
        float dx= t*(b.x - a.x),
        dy = t*(b.y - a.y);
        CCPoint pt = ccp(a.x + dx, a.y + dy);
        m_pts.push_back(pt);
    }
    return m_pts;
}

std::string CCCommonUtils::changeTimeAddUnit(int secs){
    int day = secs/(24*3600);
    int hour = secs/3600%24;
    int min = secs/60%60;
    std::string dayStr = _lang("105592");
    std::string hourStr = _lang("105591");
    std::string minStr = _lang("105590");
    if(day > 0){
        return CCString::createWithFormat("%d%s",day,dayStr.c_str())->getCString();
    }else if(hour>0){
        if(min>0){
           return CCString::createWithFormat("%d%s:%d%s",hour,hourStr.c_str(),min,minStr.c_str())->getCString();
        }else{
            return CCString::createWithFormat("%d%s",hour,hourStr.c_str())->getCString();
        }
    }
    if(min<=0){
        min = 1;
    }
    return CCString::createWithFormat("%d%s",min,minStr.c_str())->getCString();
}

bool CCCommonUtils::isAdriodPad(){
    auto target = CCApplication::sharedApplication()->getTargetPlatform();
    if (target == kTargetAndroid) {
        auto winSize =Director::getInstance()->getWinSize() ;
        int dpi = Device::getDPI();
        float size = sqrt(pow(winSize.width, 2) + pow(winSize.height, 2)) / dpi;
        if(size >= 7.0f){
            return true;
        }
    }
    return false;
}

bool CCCommonUtils::isIOSPad() {
    
    return kTargetIpad == CCApplication::sharedApplication()->getTargetPlatform();
}

static bool __isUseHD = false;  // 本地开关
static bool __curIsHD = true;

bool CCCommonUtils::isPad()
{
    
    return (__isUseHD && (isIOSPad() || isAdriodPad()));
}

bool CCCommonUtils::isIosAndroidPad()
{
    // tao.yu 目前不开放高清资源
    return false;
    
    int __hd_switch =  CCUserDefault::sharedUserDefault()->getIntegerForKey(SERVER_SWITCH_HD_2, 0);  // 服务器开关
//    bool __ccbFinish =  CCUserDefault::sharedUserDefault()->getBoolForKey(DOWNLOAD_HD_CCB, false);  // ccb下载完毕
    bool __isResFinish = CCUserDefault::sharedUserDefault()->getBoolForKey(DOWNLOAD_HD_RES, false); //资源是否下载完毕
    
    bool __isFlag = (__isUseHD && __isResFinish && __hd_switch>0 && (isIOSPad() || isAdriodPad()));
    
//    if (__isFlag)
//    {
//        addResSearchPath();
//    }
    
    return __isFlag;
}


void CCCommonUtils::setIsUseHD(bool isHD)
{
    __isUseHD = isHD;
}

void CCCommonUtils::setServerHDSwitch()
{
    int _switch = CCUserDefault::sharedUserDefault()->getIntegerForKey(SERVER_SWITCH_HD, 0);
    string  _filepath = CCFileUtils::sharedFileUtils()->getWritablePath() + "dresource/" + "Common/Common_101_hd.plist";
    if (!CCFileUtils::sharedFileUtils()->isFileExist(_filepath))
    {
        _switch = 0;
    }
    CCUserDefault::sharedUserDefault()->setIntegerForKey(SERVER_SWITCH_HD_2, _switch);
    CCUserDefault::sharedUserDefault()->flush();
}

bool CCCommonUtils::getIsUseHD()
{
    return __isUseHD;
}

void CCCommonUtils::setIsHDViewPort(bool isHD)
{
    __curIsHD = isHD;
    
    if(isHD && isIosAndroidPad()) {
        Director::getInstance()->getOpenGLView()->setDesignResolutionSize(1536, 2048, kResolutionFixedWidth);
    }else{
        Director::getInstance()->getOpenGLView()->setDesignResolutionSize(640, 852, kResolutionFixedWidth);
    }
}

bool CCCommonUtils::getIsHDViewPort()
{

    return __curIsHD;
}

void CCCommonUtils::addResSearchPath()
{
    static bool isFirstAdd = false;
    if (!isFirstAdd)
    {
        isFirstAdd = true;
        std::vector<std::string> _paths;
        std::string m_writeablePath = CCFileUtils::sharedFileUtils()->getWritablePath();
        _paths.push_back(m_writeablePath + "dresource/");
        _paths.push_back(m_writeablePath);
        _paths.push_back("");
        CCFileUtils::sharedFileUtils()->setSearchPaths(_paths);
    }
    
}

void CCCommonUtils::resetData(CCDictionary *params){
    GlobalData::shared()->isInDataParsing = true;
    GlobalData::shared()->m_harvestInfo = "";
    GlobalData::shared()->m_skillFortInfo = "";
    GlobalData::shared()->installFriendsInfo = "";
    GlobalData::shared()->inviteFriendsInfo = "";
    GlobalData::shared()->playerInfo.setAllianceId("");
    ChatController::getInstance()->m_chatAlliancePool.clear();
    ChatController::getInstance()->m_latestAllianceMsg.sequenceId=0;
    ActivityController::getInstance()->ckfNeedLeave = 0;
    
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::resetPlayerIsInAlliance();
//    ChatServiceCocos2dx::clearCountryMsg();
//    ChatServiceCocos2dx::clearMailMsg();
    ChatServiceCocos2dx::onPlayerChanged();
    ChatServiceCocos2dx::notifyChangeLanguage();
    ChatServiceCocos2dx::m_curSendChatIndex=0;
    ChatServiceCocos2dx::m_curSendMailIndex=0;
    ChatServiceCocos2dx::m_curAllianceMemberIndex=0;
    ChatServiceCocos2dx::m_curUserInfoIndex=0;
    ChatServiceCocos2dx::postChannelNoMoreData(0,false);
    ChatServiceCocos2dx::postChannelNoMoreData(2,false);
    ChatServiceCocos2dx::postChannelNoMoreData(3,false);
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    ChatServiceCocos2dx::resetIOS();
#endif
    ChatController::getInstance()->isFirstRequestCountry=true;
    ChatController::getInstance()->canCountryRequest=true;
    ChatController::getInstance()->canAllianceRequest=true;
    ChatController::getInstance()->m_commentPool.clear();
    ChatController::getInstance()->m_chatCountryPool.clear();
    ChatController::getInstance()->m_chatInfoSendDic->removeAllObjects();
    MailController::getInstance()->m_mailInfoSendDic->removeAllObjects();
    MailController::getInstance()->m_mailDataDic->removeAllObjects();
    MailController::getInstance()->m_allianceMemberInfoDic->removeAllObjects();
    ChatController::getInstance()->m_userInfoDic->removeAllObjects();
    
    PortActController::getInstance()->m_loginDayMap.clear();
    PortActController::getInstance()->m_timeExMap.clear();
    AllianceManager::getInstance()->clearEvents();
    
    
    TimerController::getInstance();
    CCUserDefault::sharedUserDefault()->setStringForKey(ALLIANCE_MEMBER_BTN_STATUS, "");
}

void CCCommonUtils::initConfigData(CCDictionary *params){
    //初始化dataconfig
    CCDictionary* dataConfig = _dict(params->objectForKey("dataConfig"));
    CCDictionary* account = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("account"));
    if (account) {
        if(account->objectForKey("k1"))
        {
            GlobalData::shared()->maxGameCnt = account->valueForKey("k1")->intValue();
        }
        if (account->objectForKey("k2")) {
            GlobalData::shared()->cnt_forbidden_swith = account->valueForKey("k2")->intValue();
        }
        if (account->objectForKey("k3")) {
            GlobalData::shared()->cnt_forbidden_level = account->valueForKey("k3")->intValue();
        }
        if (account->objectForKey("k4")) {
            GlobalData::shared()->cnt_forbidden_playingtime = account->valueForKey("k4")->doubleValue();
        }
        if (account->objectForKey("k5")) {
            GlobalData::shared()->trans_res_forbidden_swith = account->valueForKey("k5")->intValue();
        }
        if (account->objectForKey("k6")) {
            GlobalData::shared()->trans_res_limit = account->valueForKey("k6")->longValue();
        }
    }
    if(dataConfig->objectForKey("pop_rate")){
        ActivityController::getInstance()->initViewConfig(_dict(dataConfig->objectForKey("pop_rate")));
    }else{
        ActivityController::getInstance()->initViewConfig(NULL);
    }
    if(dataConfig->objectForKey("cross_open")){
        WorldController::getInstance()->isServerCrossOpen = dataConfig->valueForKey("cross_open")->boolValue();
    }else{
        WorldController::getInstance()->isServerCrossOpen = false;
    }
    if (dataConfig->objectForKey("wl_res_tile")) {
        auto wlResTile = _dict(dataConfig->objectForKey("wl_res_tile"));
        WorldController::getInstance()->resTileKey["k1"] = wlResTile->valueForKey("k1")->intValue();
        WorldController::getInstance()->resTileKey["k2"] = wlResTile->valueForKey("k2")->intValue();
        WorldController::getInstance()->resTileKey["k3"] = wlResTile->valueForKey("k3")->intValue();
        WorldController::getInstance()->resTileKey["k4"] = wlResTile->valueForKey("k4")->intValue();
        WorldController::getInstance()->resTileKey["k5"] = wlResTile->valueForKey("k5")->intValue();
        WorldController::getInstance()->resTileKey["k6"] = wlResTile->valueForKey("k6")->intValue();
        WorldController::getInstance()->resTileKey["k7"] = wlResTile->valueForKey("k7")->intValue();
        WorldController::getInstance()->resTileKey["k8"] = wlResTile->valueForKey("k8")->intValue();
    }
    
    auto aAreaConfigDic = _dict(dataConfig->objectForKey("alliance_territory"));
    if (aAreaConfigDic) {
        WorldController::getInstance()->m_bAAreaCost = aAreaConfigDic->valueForKey("k3")->intValue();
        WorldController::getInstance()->m_aTerritory10 = aAreaConfigDic->valueForKey("k10")->intValue();
        WorldController::getInstance()->m_aTerritory11 = aAreaConfigDic->valueForKey("k11")->intValue();
        WorldController::getInstance()->m_aTerritory12 = aAreaConfigDic->valueForKey("k12")->intValue();
    }
    auto aAreaConfigDic1 = _dict(dataConfig->objectForKey("alliance_territory1"));
    WorldController::getInstance()->m_territoryBannerCost = 0;
    GlobalData::shared()->alliance_territory_supermine_limit = 1;
    if (aAreaConfigDic1) {
        WorldController::getInstance()->m_territoryBannerCost = aAreaConfigDic1->valueForKey("k3")->intValue();
        GlobalData::shared()->alliance_territory_supermine_limit = aAreaConfigDic1->valueForKey("k9")->intValue();
        
    }
    //联盟领地增减益初始化
    if (params->objectForKey("territory_positive_effect")) {
        GlobalData::shared()->alliance_territory_positive_effect = params->valueForKey("territory_positive_effect")->intValue();
    }
    else
        GlobalData::shared()->alliance_territory_positive_effect = 0;
    
    if (params->objectForKey("territory_negative_effect")) {
        GlobalData::shared()->alliance_territory_negative_effect = params->valueForKey("territory_negative_effect")->intValue();
    }
    else
        GlobalData::shared()->alliance_territory_negative_effect = 0;
    
    if (params->objectForKey("storage")) {
        GlobalData::shared()->territory_warehouse_dayStorage = params->valueForKey("storage")->longValue();
    }
    else
        GlobalData::shared()->territory_warehouse_dayStorage = 0;
    
    if (params->objectForKey("storageTotal")) {
        GlobalData::shared()->territory_warehouse_totalStorage =  params->valueForKey("storageTotal")->longValue();
    }
    else
        GlobalData::shared()->territory_warehouse_totalStorage = 0;
    
    auto allianceCost = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("alliance_cost"));
    if(allianceCost){
        GlobalData::shared()->alliance_cost_k1 = allianceCost->valueForKey("k1")->intValue();
        GlobalData::shared()->alliance_cost_k2 = allianceCost->valueForKey("k2")->intValue();
        GlobalData::shared()->alliance_cost_k3 = allianceCost->valueForKey("k3")->intValue();
        GlobalData::shared()->alliance_cost_k5 = allianceCost->valueForKey("k5")->intValue();
        GlobalData::shared()->alliance_cost_k6 = allianceCost->valueForKey("k6")->intValue();
        GlobalData::shared()->alliance_cost_k7 = allianceCost->valueForKey("k7")->intValue();
        GlobalData::shared()->alliance_cost_k8 = allianceCost->valueForKey("k8")->intValue();
    }
    auto allianceScience = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("alliance_science"));
    if(allianceScience){
        GlobalData::shared()->alliance_science_k1 = allianceScience->valueForKey("k1")->intValue();
        GlobalData::shared()->alliance_science_k2 = allianceScience->valueForKey("k2")->intValue();
        GlobalData::shared()->alliance_science_k3 = allianceScience->valueForKey("k3")->intValue();
        GlobalData::shared()->alliance_science_k4 = allianceScience->valueForKey("k4")->intValue();
        GlobalData::shared()->alliance_science_k5 = allianceScience->valueForKey("k5")->intValue();
        GlobalData::shared()->alliance_science_k6 = allianceScience->valueForKey("k6")->intValue();
        GlobalData::shared()->alliance_science_k7 = allianceScience->valueForKey("k7")->intValue();
        GlobalData::shared()->alliance_science_k8 = allianceScience->valueForKey("k8")->intValue();
        GlobalData::shared()->alliance_science_k9 = allianceScience->valueForKey("k9")->intValue();
        GlobalData::shared()->alliance_science_k10 = allianceScience->valueForKey("k10")->intValue();
        GlobalData::shared()->alliance_science_k11 = allianceScience->valueForKey("k11")->intValue();
        GlobalData::shared()->alliance_science_k12 = allianceScience->valueForKey("k12")->intValue();
    }
    auto alliance_gift = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("alliance_gift"));
    if(alliance_gift){
        GlobalData::shared()->alliance_gift_k2 = alliance_gift->valueForKey("k2")->intValue();
        GlobalData::shared()->alliance_gift_k3 = alliance_gift->valueForKey("k3")->longValue();
    }
    
    auto resourcesWeight = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("resources_weight"));
    if(resourcesWeight){
        GlobalData::shared()->resources_weight_1 = resourcesWeight->valueForKey("k1")->intValue();
        GlobalData::shared()->resources_weight_2 = resourcesWeight->valueForKey("k2")->intValue();
        GlobalData::shared()->resources_weight_3 = resourcesWeight->valueForKey("k3")->intValue();
        GlobalData::shared()->resources_weight_4 = resourcesWeight->valueForKey("k4")->intValue();
        GlobalData::shared()->resources_weight_5 = resourcesWeight->valueForKey("k5")->intValue();
        GlobalData::shared()->resources_weight_6 = resourcesWeight->valueForKey("k6")->intValue();
        GlobalData::shared()->resources_weight_7 = resourcesWeight->valueForKey("k7")->intValue();
    }
    
    auto worldRally = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("world_rally"));
    if(worldRally){
        GlobalData::shared()->world_rally_k1 = worldRally->valueForKey("k1")->intValue();
        GlobalData::shared()->world_rally_k2 = worldRally->valueForKey("k2")->intValue();
        GlobalData::shared()->world_rally_k3 = worldRally->valueForKey("k3")->intValue();
        GlobalData::shared()->world_rally_k4 = worldRally->valueForKey("k4")->intValue();
        GlobalData::shared()->world_rally_k5 = worldRally->valueForKey("k5")->intValue();
        GlobalData::shared()->world_rally_k6 = worldRally->valueForKey("k6")->intValue();
        GlobalData::shared()->world_rally_k7 = worldRally->valueForKey("k7")->intValue();
        GlobalData::shared()->world_rally_k8 = worldRally->valueForKey("k8")->intValue();
    }
    auto worldAllianceWar = _dict(dataConfig->objectForKey("world_alliancewar"));
    if(worldAllianceWar){
        GlobalData::shared()->world_alliance_war_k1 = worldAllianceWar->valueForKey("k1")->intValue();
        GlobalData::shared()->world_alliance_war_k2 = worldAllianceWar->valueForKey("k2")->intValue();
    }
    
    auto inviteCost = _dict(dataConfig->objectForKey("invite_cost"));
    if(inviteCost){
        GlobalData::shared()->invite_cost_k1 = inviteCost->valueForKey("k1")->intValue();
    }
    
    auto fbGift = _dict(dataConfig->objectForKey("facebook_gift"));
    if(fbGift){
        GlobalData::shared()->facebook_gift_k7 = fbGift->valueForKey("k7")->intValue();
    }
    
    auto glodmine_fb = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("goldmine_fb"));
    if(glodmine_fb){
        GlobalData::shared()->glodMine_fb_k1 = glodmine_fb->valueForKey("k1")->intValue();
        GlobalData::shared()->glodMine_fb_k2 = glodmine_fb->valueForKey("k2")->intValue();
        GlobalData::shared()->glodMine_fb_k3 = glodmine_fb->valueForKey("k3")->intValue();
        GlobalData::shared()->glodMine_fb_k4 = glodmine_fb->valueForKey("k4")->intValue();
    }
    
    auto &worldConfig = GlobalData::shared()->worldConfig;
    if(dataConfig->objectForKey("ckf_world_row")){
        worldConfig.ckf_world_row = dataConfig->valueForKey("ckf_world_row")->intValue();
    }
    if(dataConfig->objectForKey("monster_count")){
        worldConfig.stamineMax = dataConfig->valueForKey("monster_count")->intValue();
    }
    if(dataConfig->objectForKey("monster_interval")){
        vector<std::string> vector;
        CCCommonUtils::splitString(dataConfig->valueForKey("monster_interval")->getCString(), ";", vector);
        worldConfig.stamineIntervalTime = atoi(vector[1].c_str());
        worldConfig.stamineIntervalNum = atoi(vector[0].c_str());
    }
    if(dataConfig->objectForKey("monster_decr")){
        worldConfig.stamineCostPerTime = dataConfig->valueForKey("monster_decr")->intValue();
    }
    if(dataConfig->objectForKey("boss_decr")){
        worldConfig.boss_decr = dataConfig->valueForKey("boss_decr")->intValue();
    }
    if(dataConfig->objectForKey("boss_max_member")){
        worldConfig.bossLimitNum = dataConfig->valueForKey("boss_max_member")->intValue();
    }
    
    auto worldMapTime = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("worldmap_time"));
    if(worldMapTime){
        for(int i= 0; i < 4; i++){
            auto key = CCString::createWithFormat("k%d",i+1)->getCString();
            int value = worldMapTime->valueForKey(key)->intValue();
            worldConfig.monsterAttackTime.push_back(value);
        }
    }
    
    auto worldMapMap = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("worldmap_map"));
    if(worldMapMap){
        for(int i= 0; i < 4; i++){
            auto key = CCString::createWithFormat("k%d",i+1)->getCString();
            int value = worldMapMap->valueForKey(key)->intValue();
            worldConfig.relicConfig.push_back(value);
        }
    }
    
    auto worldDistance = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("world_marchtime"));
    if (worldDistance) {
        for (int i=0; i<6; i++) {
            auto key = CCString::createWithFormat("k%d",i+1)->getCString();
            auto ki = worldDistance->valueForKey(key)->getCString();
            worldConfig.distance[i] = atof(ki);
        }
    }
    auto march = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("march"));
    if(march){
        TroopsController::getInstance()->m_limitHeroNum = march->valueForKey("k1")->intValue();
    }else{
        TroopsController::getInstance()->m_limitHeroNum = 1;
    }
    
    auto worldFire = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("world_fire2"));
    if (worldFire) {
        for (int i=0; i<3; i++) {
            auto key = CCString::createWithFormat("k%d",i+1)->getCString();
            auto ki = worldFire->valueForKey(key)->intValue();
            worldConfig.fire[i] = ki;
        }
    }
    auto wFire1 = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("world_fire1"));
    if (wFire1) {
        for (int i=0; i<4; i++) {
            auto key = CCString::createWithFormat("k%d",i+1)->getCString();
            auto ki = wFire1->valueForKey(key)->intValue();
            worldConfig.fire1[i] = ki;
        }
    }
    
    if(dataConfig->objectForKey("resKey")){
        GlobalData::shared()->worldConfig.reskey = dataConfig->valueForKey("resKey")->intValue();
    }
    
    auto worldPve = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("worldmap_pve"));
    if (worldPve) {
        worldConfig.pveHitMax = worldPve->valueForKey("k1")->intValue();
        worldConfig.pveBuyMax = worldPve->valueForKey("k2")->intValue();
    }
    auto worldPveBuy = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("worldmap_pvenum"));
    if (worldPveBuy) {
        CCDictElement* pEle;
        CCDICT_FOREACH(worldPveBuy, pEle) {
            string key = pEle->getStrKey();
            unsigned int order = atoi(&key[1]);
            unsigned int val = dynamic_cast<CCString*>(pEle->getObject())->intValue();
            worldConfig.pveBuyPrice[order] = val;
        }
    }
    
    GlobalData::shared()->cityUpdateDefenseDelayMin = dataConfig->valueForKey("cityDefInterval")->intValue();
    
    CCDictionary* buildingBase = _dict(dataConfig->objectForKey("building_base"));
    FunBuildController::getInstance()->building_base_k1 = buildingBase->valueForKey("k1")->intValue();
    FunBuildController::getInstance()->building_base_k2 = buildingBase->valueForKey("k2")->intValue();
    FunBuildController::getInstance()->building_base_k3 = buildingBase->valueForKey("k3")->intValue();
    FunBuildController::getInstance()->building_base_k4 = buildingBase->valueForKey("k4")->intValue();
    FunBuildController::getInstance()->building_base_k5 = buildingBase->valueForKey("k5")->intValue();
    FunBuildController::getInstance()->building_base_k6 = buildingBase->valueForKey("k6")->intValue();
    FunBuildController::getInstance()->building_base_k7 = buildingBase->valueForKey("k7")->intValue();
    FunBuildController::getInstance()->building_base_k8 = buildingBase->valueForKey("k8")->intValue();
    FunBuildController::getInstance()->building_base_k9 = buildingBase->valueForKey("k9")->intValue();
    
    GlobalData::shared()->freeSpdT = dataConfig->valueForKey("freeSpdT")->intValue();
    FunBuildController::getInstance()->dataConfig_freeSpdT = GlobalData::shared()->freeSpdT;
    FunBuildController::getInstance()->checkFreeTime();
    CCDictionary* mailConfig = _dict(dataConfig->objectForKey("mail"));
    if(mailConfig){
        GlobalData::shared()->mailConfig.userMailMax = mailConfig->valueForKey("k1")->intValue();
        GlobalData::shared()->mailConfig.sysMailMax = mailConfig->valueForKey("k2")->intValue();
        GlobalData::shared()->mailConfig.saveMailMax = mailConfig->valueForKey("k3")->intValue();
    }
    CCDictionary* generalConfig = _dict(dataConfig->objectForKey("hero"));
    
    GlobalData::shared()->generalConfig.greenNum = generalConfig->valueForKey("k1")->intValue();
    GlobalData::shared()->generalConfig.blueNum = generalConfig->valueForKey("k2")->intValue();
    GlobalData::shared()->generalConfig.purpleNum = generalConfig->valueForKey("k3")->intValue();
    GlobalData::shared()->generalConfig.orangeNum = generalConfig->valueForKey("k4")->intValue();
    GlobalData::shared()->generalConfig.k5 = generalConfig->valueForKey("k5")->floatValue();

    if(dataConfig->objectForKey("hero_yutao")){
        CCDictionary* picConfigDic  = _dict(dataConfig->objectForKey("hero_yutao"));
        GlobalData::shared()->generalConfig.picUploadLv_limit = picConfigDic->valueForKey("k9")->intValue();
        GlobalData::shared()->generalConfig.picModelTime = picConfigDic->valueForKey("k8")->floatValue();
    }
        

    
    CCDictionary* equip_make = _dict(dataConfig->objectForKey("equip"));
    if(equip_make){
        EquipmentController::getInstance()->retInitConf(equip_make);
    }
    CCDictionary* generalConfig1 = _dict(dataConfig->objectForKey("hero_up"));
    GlobalData::shared()->generalConfig.up1 = generalConfig1->valueForKey("k1")->floatValue();
    GlobalData::shared()->generalConfig.up2 = generalConfig1->valueForKey("k2")->floatValue();
    GlobalData::shared()->generalConfig.up3 = generalConfig1->valueForKey("k3")->floatValue();
    GlobalData::shared()->generalConfig.up4 = generalConfig1->valueForKey("k4")->floatValue();
    
    CCDictionary* generalConfig2 = _dict(dataConfig->objectForKey("hero_max"));
    GlobalData::shared()->generalConfig.maxGeneral = generalConfig2->valueForKey("k1")->intValue();
    
    CCDictionary* generalConfig3 = _dict(dataConfig->objectForKey("fight_number"));
    CCDictElement* element1;
    CCDICT_FOREACH(generalConfig3, element1){
        std::string value = dynamic_cast<CCString*>(element1->getObject())->getCString();
        vector<std::string> vector;
        CCCommonUtils::splitString(value, "|", vector);
        GlobalData::shared()->generalConfig.formationOpen[atoi(vector[0].c_str())] = atoi(vector[1].c_str());
    }
    CCDictionary* generalConfig4 = _dict(dataConfig->objectForKey("hero_number"));
    
    GlobalData::shared()->generalConfig.bagMaxGeneralNum = generalConfig4->valueForKey("k1")->intValue();
    GlobalData::shared()->generalConfig.bagMaxGoldBuyNum = generalConfig4->valueForKey("k3")->intValue();
    GlobalData::shared()->generalConfig.bagFreeNum = generalConfig4->valueForKey("k2")->intValue();
    std::string goldStr = generalConfig4->valueForKey("k4")->getCString();
    vector<std::string> goldVector;
    CCCommonUtils::splitString(goldStr, "|", goldVector);
    int i = 0;
    while (i < goldVector.size()) {
        GlobalData::shared()->generalConfig.posPrice[i + 1] = atoi(goldVector[i].c_str());
        i++;
    }
    
    auto soulDict = _dict(dataConfig->objectForKey("hero_soulstone"));
    GlobalData::shared()->generalConfig.soul0 = soulDict->valueForKey("k1")->intValue();
    GlobalData::shared()->generalConfig.soul1 = soulDict->valueForKey("k2")->intValue();
    GlobalData::shared()->generalConfig.soul2 = soulDict->valueForKey("k3")->intValue();
    GlobalData::shared()->generalConfig.soul3 = soulDict->valueForKey("k4")->intValue();
    
    CCDictionary* flushHero = _dict(dataConfig->objectForKey("flush_hero"));
    GlobalData::shared()->generalConfig.freePerDay = flushHero->valueForKey("k3")->intValue();
    GlobalData::shared()->generalConfig.oneTimeCost = flushHero->valueForKey("k1")->intValue();
    GlobalData::shared()->generalConfig.tenTimeCost = flushHero->valueForKey("k2")->intValue();
    CCDictionary* cd_cost = _dict(dataConfig->objectForKey("cd_cost"));
    CCDictionary* cd_time = _dict(dataConfig->objectForKey("cd_time"));
    for (int i=0; ; i++) {
        string _tmpKey = "k";
        _tmpKey += CC_ITOA(i+1);
        if (cd_time->objectForKey(_tmpKey) && cd_cost->objectForKey(_tmpKey)) {
            GlobalData::shared()->cd_time.push_back(cd_time->valueForKey(_tmpKey)->intValue());
            GlobalData::shared()->cd_cost.push_back(cd_cost->valueForKey(_tmpKey)->intValue());
        }
        else {
            break;
        }
    }
    sort(GlobalData::shared()->cd_time.begin(), GlobalData::shared()->cd_time.end());
    sort(GlobalData::shared()->cd_cost.begin(), GlobalData::shared()->cd_cost.end());
    
    CCDictionary* cd_gold = _dict(dataConfig->objectForKey("cd_gold"));
    for (int i=0; ; i++) {
        string _tmpKey = "k";
        _tmpKey += CC_ITOA(i+1);
        if (cd_gold->objectForKey(_tmpKey)) {
            GlobalData::shared()->cd_gold[_tmpKey] = cd_gold->valueForKey(_tmpKey)->floatValue();
        }
        else {
            break;
        }
    }
    
    CCDictionary* resource_cost = _dict(dataConfig->objectForKey("resource_cost"));
    CCDictionary* resource_num1 = _dict(dataConfig->objectForKey("resource_num1"));
    CCDictionary* resource_num2 = _dict(dataConfig->objectForKey("resource_num2"));
    CCDictionary* resource_num3 = _dict(dataConfig->objectForKey("resource_num3"));
    CCDictElement* ele;
    CCDICT_FOREACH(resource_cost, ele)
    {
        string key = ele->getStrKey();
        GlobalData::shared()->res_cost[key] = resource_cost->valueForKey(key)->floatValue();
    }
    CCDICT_FOREACH(resource_num1, ele)
    {
        string key = ele->getStrKey();
        GlobalData::shared()->res1_num[key] = resource_num1->valueForKey(key)->floatValue();
    }
    CCDICT_FOREACH(resource_num2, ele)
    {
        string key = ele->getStrKey();
        GlobalData::shared()->res2_num[key] = resource_num2->valueForKey(key)->floatValue();
    }
    CCDICT_FOREACH(resource_num3, ele)
    {
        string key = ele->getStrKey();
        GlobalData::shared()->res3_num[key] = resource_num3->valueForKey(key)->floatValue();
    }
    CCDictionary* abilityCost = _dict(dataConfig->objectForKey("ability_cost"));
    if(abilityCost){
        GlobalData::shared()->generalConfig.abilityChangeGold = abilityCost->valueForKey("k1")->intValue();
        GlobalData::shared()->generalConfig.skillResetGold = abilityCost->valueForKey("k2")->intValue();
        GlobalData::shared()->generalConfig.abilityResetGold = abilityCost->valueForKey("k3")->intValue();
    }
    
    CCDictionary* physical = _dict(dataConfig->objectForKey("physical"));
    GlobalData::shared()->lordInfo.physical_k1 = physical->valueForKey("k1")->intValue();
    GlobalData::shared()->lordInfo.physical_k2 = physical->valueForKey("k2")->intValue();
    GlobalData::shared()->lordInfo.physical_k3 = physical->valueForKey("k3")->intValue();
    GlobalData::shared()->lordInfo.physical_k4 = physical->valueForKey("k4")->intValue();
    CCDictionary* physical_bug = _dict(dataConfig->objectForKey("physical_bug"));
    CCDictElement* phy_buy;
    CCDICT_FOREACH(physical_bug, phy_buy){
        string key = phy_buy->getStrKey();
        int value = dynamic_cast<CCString*>(phy_buy->getObject())->intValue();
        GlobalData::shared()->lordInfo.physical_bug[key] = value;
    }
    
    auto &userLvAboutInfo = GlobalData::shared()->userLvAboutInfo;
    
    userLvAboutInfo.cd_cost_k1 = cd_cost->valueForKey("k1")->floatValue();
    userLvAboutInfo.cd_cost_k2 = cd_cost->valueForKey("k2")->floatValue();
    userLvAboutInfo.cd_cost_k3 = cd_cost->valueForKey("k3")->floatValue();
    userLvAboutInfo.cd_cost_k4 = cd_cost->valueForKey("k4")->floatValue();
    
    CCDictionary* ranking = _dict(dataConfig->objectForKey("ranking"));
    if(ranking!=NULL){
        
        userLvAboutInfo.ranking_k3 = ranking->valueForKey("k3")->intValue();
    }
    
    CCDictionary* essence = _dict(dataConfig->objectForKey("essence"));
    if(essence!=NULL){
        GlobalData::shared()->essenceK1 = essence->valueForKey("k1")->intValue();
        GlobalData::shared()->essenceK2 = essence->valueForKey("k2")->intValue();
        GlobalData::shared()->essenceK3 = essence->valueForKey("k3")->intValue();
    }
    
    GlobalData::shared()->quickEndLevel = dataConfig->valueForKey("quickEndBattle")->intValue();//k5
    GlobalData::shared()->recruitedGeneralCount = params->valueForKey("recruitedGeneralCount")->intValue();
    
    CCDictionary* serverConfig = _dict(dataConfig->objectForKey("serverConfig"));
    if (serverConfig) {
        ChatController::getInstance()->chat_interval = serverConfig->valueForKey("chat_interval")->intValue();
        GlobalData::shared()->lordInfo.feed_max = serverConfig->valueForKey("feed_maxCount")->intValue();
    }
    
    CCDictionary* activityconfig = _dict(dataConfig->objectForKey("activityconfig"));
    if (activityconfig) {
        ActivityController::getInstance()->parseActConfig(activityconfig);
    }
    
    CCDictionary *heroAbility = _dict(dataConfig->objectForKey("hero_ability"));
    if(heroAbility){
        GlobalData::shared()->generalConfig.generalMaxAbility[0] = heroAbility->valueForKey("k1")->intValue();
        GlobalData::shared()->generalConfig.generalMaxAbility[1] = heroAbility->valueForKey("k2")->intValue();
        GlobalData::shared()->generalConfig.generalMaxAbility[2] = heroAbility->valueForKey("k3")->intValue();
        GlobalData::shared()->generalConfig.generalMaxAbility[3] = heroAbility->valueForKey("k4")->intValue();
        GlobalData::shared()->generalConfig.generalMaxAbility[4] = heroAbility->valueForKey("k5")->intValue();
        GlobalData::shared()->generalConfig.generalMaxAbility[5] = heroAbility->valueForKey("k6")->intValue();
    }
    
    CCDictionary *abilityLv = _dict(dataConfig->objectForKey("ability_lv"));
    if(abilityLv){
        int i = 1;
        while(abilityLv->objectForKey(std::string("k") + CC_ITOA(i))){
            int level = abilityLv->valueForKey(std::string("k") + CC_ITOA(i))->intValue();
            GlobalData::shared()->generalConfig.abilityOpenedLevel[level] = i;
            i++;
        }
    }
    
    CCDictionary *heroSkillPoint = _dict(dataConfig->objectForKey("hero_skillpoint"));
    if(heroSkillPoint){
        std::string l = heroSkillPoint->valueForKey("k1")->getCString();
        GlobalData::shared()->generalConfig.skillPointAddPerLevel[0] = heroSkillPoint->valueForKey("k1")->intValue();
        GlobalData::shared()->generalConfig.skillPointAddPerLevel[1] = heroSkillPoint->valueForKey("k2")->intValue();
        GlobalData::shared()->generalConfig.skillPointAddPerLevel[2] = heroSkillPoint->valueForKey("k3")->intValue();
        GlobalData::shared()->generalConfig.skillPointAddPerLevel[3] = heroSkillPoint->valueForKey("k4")->intValue();
        GlobalData::shared()->generalConfig.skillPointAddPerLevel[4] = heroSkillPoint->valueForKey("k5")->intValue();
        GlobalData::shared()->generalConfig.skillPointAddPerLevel[5] = heroSkillPoint->valueForKey("k6")->intValue();
    }
    
    CCDictionary* timeDiv = _dict(dataConfig->objectForKey("online_reward"));
    if (timeDiv) {
        PortActController::getInstance()->parseTimeDiv(timeDiv);
    }
    
    CCDictionary* cargo = _dict(params->objectForKey("cargo"));
    if (cargo) {
        if (cargo->objectForKey("rewardTime")) {
            PortActController::getInstance()->m_nextRewardTime = cargo->valueForKey("rewardTime")->doubleValue() / 1000;
        }
        if (cargo->objectForKey("rewardInfo")) {
            PortActController::getInstance()->m_rewardInfo = cargo->valueForKey("rewardInfo")->getCString();
        }
    }
    CCDictionary* flushAttr = _dict(params->objectForKey("flushAttr"));
    if(flushAttr!=NULL){
        GlobalData::shared()->flushFreeTimes = flushAttr->valueForKey("freeTimes")->intValue();
        GlobalData::shared()->flushGoldTimes = flushAttr->valueForKey("goldTimes")->intValue();
        GlobalData::shared()->flushCurrentTime = flushAttr->valueForKey("time")->intValue();
    }
    
    GlobalData::shared()->MaxPlayerLv = 99999;
    GlobalData::shared()->MaxHeroLv = 99999;
    GlobalData::shared()->MaxBuildLv = 99999;
    CCDictionary* player_max = _dict(dataConfig->objectForKey("player_max"));
    if(player_max){
        GlobalData::shared()->MaxPlayerLv = player_max->valueForKey("k1")->intValue();
        GlobalData::shared()->MaxHeroLv = player_max->valueForKey("k2")->intValue();
        GlobalData::shared()->MaxBuildLv = player_max->valueForKey("k3")->intValue();
    }
    
    if (dataConfig->objectForKey("shield_base_2")) {
        GlobalData::shared()->shield_base = dataConfig->valueForKey("shield_base_2")->intValue();
    }
    
    if (dataConfig->objectForKey("chat_switch"))
    {
        CCDictionary* chat = _dict(dataConfig->objectForKey("chat_switch"));
        if(chat)
        {
            if (chat->objectForKey("k1")) {
                CCLOGFUNCF("chat_switch K1: %d",chat->valueForKey("k1")->intValue());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//                ChatServiceCocos2dx::m_autoTranslateWithGoogle=2;
                ChatServiceCocos2dx::m_autoTranslateWithGoogle=chat->valueForKey("k1")->intValue();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
/** zyt */
                //谷歌HTTP翻译
                ChatServiceCocos2dx::m_autoTranslateWithGoogle=chat->valueForKey("k2")->intValue();
                //iOS端 多人聊天管理
                ChatServiceCocos2dx::enableChatRoom =chat->valueForKey("k4")->intValue();
                
                string db_k5 = chat->valueForKey("k7")->getCString();
                vector<string>strVec;
                CCCommonUtils::splitString(db_k5, "|", strVec);
                if (strVec.size()>=1) {
                    
                   bool isOpen =  ChatServiceCocos2dx::compareVersionWithKeyVersionString(strVec[0]);
                    
                        ChatServiceCocos2dx::DB_UserInfo_switch = isOpen;

                }
                if (strVec.size()>=2)
                {
                    bool isOpen =  ChatServiceCocos2dx::compareVersionWithKeyVersionString(strVec[1]);
                        ChatServiceCocos2dx::DB_ChatChannel_switch = isOpen;
                    

                }
                if (strVec.size()>=3){
                      bool isOpen =  ChatServiceCocos2dx::compareVersionWithKeyVersionString(strVec[2]);
                        ChatServiceCocos2dx::DB_MsgItem_switch = isOpen;
                   
                }

#endif
            }
            
            if (chat->objectForKey("k3")) {
                CCLOGFUNCF("chat_switch K3: %s",chat->valueForKey("k3")->getCString());
                string k3=chat->valueForKey("k3")->getCString();
                std::vector<string> strVector;
                splitString(k3, "|", strVector);
                if(strVector.size()>0)
                {
                    CCLOGFUNCF("android switch:%s",strVector[0].c_str());
                    if (strVector[0]=="0") {
                        MailController::getInstance()->setIsNewMailListEnable(false);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                        ChatServiceCocos2dx::setIsNewMailListEnable(false);
#endif
                    }
                    else if (strVector[0]=="1") {
                        MailController::getInstance()->setIsNewMailListEnable(true);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                        ChatServiceCocos2dx::setIsNewMailListEnable(true);
#endif
                    }
                }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                //iOS原生邮件开关设置
/** zyt */
                if (strVector.size()>1){
                    if (strVector[1] == "0") {
                        ChatServiceCocos2dx::Mail_OC_Native_Enable = false;
                    }else{
                         ChatServiceCocos2dx::Mail_OC_Native_Enable = true;
                    }
                }
 
#endif
            }
            if (chat->objectForKey("k6")) {
                CCLOGFUNCF("chat_switch k6: %s",chat->valueForKey("k6")->getCString());
                string k6=chat->valueForKey("k6")->getCString();
                std::vector<string> strVector;
                splitString(k6, "|", strVector);
                if(strVector.size()>0)
                {
                    bool isOn = false;
                    if (strVector[0]==""||strVector[0]=="0") {
                        isOn=false;
                    }else if (strVector[0]=="1"){
                        isOn=true;
                    }else {
                        if (ActivityController::getInstance()->checkOpenVersion(strVector[0])){
                            isOn=true;
                        }else{
                            isOn=false;
                        }
                    }
                    CCLOGFUNCF("mailUI android switch:%s isOn :%d",strVector[0].c_str(),isOn);
                    if (!isOn) {
                        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                        ChatServiceCocos2dx::setMailNewUIEnable(false);
                        MailController::getInstance()->setIsNewMailUIEnable(false);
#endif
                    }
                    else {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                        ChatServiceCocos2dx::setMailNewUIEnable(true);
                        MailController::getInstance()->setIsNewMailUIEnable(true);
#endif
                    }
                }
                if  (strVector.size() >=2){
                    //iOS
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

                     bool isOpen =  ChatServiceCocos2dx::compareVersionWithKeyVersionString(strVector[1]);
                    CCLog("%d",isOpen);
#endif
                }
            }
        }
    }
    
    if (dataConfig->objectForKey("function_on")) {
        CCDictionary* function_on = _dict(dataConfig->objectForKey("function_on"));
        CCDictionary* function_on3 = _dict(dataConfig->objectForKey("function_on3"));
        GlobalData::shared()->android_native_chat = function_on->valueForKey("k2")->intValue();
        LotteryController::shared()->setOpenFlag(function_on->valueForKey("k3")->intValue());
        PortActController::getInstance()->m_isNewTimeRwd = function_on->valueForKey("k4")->boolValue();
        GlobalData::shared()->isOpenForum = function_on->valueForKey("k5")->boolValue();
        LotteryController::shared()->setOpenFlag2(function_on->valueForKey("k6")->intValue());
        LotteryController::shared()->setOpenFlag2_ko(function_on->valueForKey("k8")->intValue());
        AchievementController::getInstance()->setOpenFlag(function_on->valueForKey("k9")->intValue());
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
        if(function_on->objectForKey("k10")){
            GlobalData::shared()->isInviteOpen = function_on->valueForKey("k10")->boolValue();
        }else{
            GlobalData::shared()->isInviteOpen = false;
        }
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(function_on->objectForKey("k11")){
            GlobalData::shared()->isInviteOpen = function_on->valueForKey("k11")->boolValue();
        }else{
            GlobalData::shared()->isInviteOpen = false;
        }
#endif
        
        CCLOG("GlobalData::shared()->android_native_chat %d,%d",GlobalData::shared()->android_native_chat,LotteryController::shared()->getOpenFlag());
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ChatServiceCocos2dx::enableNativeChat = function_on->valueForKey("k2")->intValue();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        //原生聊天开关
        
/** zyt */
        
        int  keyValue =function_on3->valueForKey("k8")->intValue();
        if (keyValue == 0){
            ChatServiceCocos2dx::enableNativeChat =false;
        }else if (keyValue == 1){
            ChatServiceCocos2dx::enableNativeChat = true;
        }else if (keyValue == 2){
            //判断是否为4S，为4S，就关掉
            if (CCDirector::sharedDirector()->getWinSize().height<= 960) {
                ChatServiceCocos2dx::enableNativeChat = false;
            }else{
                ChatServiceCocos2dx::enableNativeChat = true;
            }
        }
        
 
#endif
    }else{
        GlobalData::shared()->android_native_chat = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ChatServiceCocos2dx::enableNativeChat = false;
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ChatServiceCocos2dx::enableNativeChat = false;
#endif
    }
    
    if (dataConfig->objectForKey("function_on2")) {
        CCLOG("dataConfig->objectForKey(function_on2");
        CCDictionary* function_on2 = _dict(dataConfig->objectForKey("function_on2"));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(function_on2->objectForKey("k6"))
        {
            CCLOG("function_on2->objectForKey k6");
            ChatServiceCocos2dx::enableNativeMail = function_on2->valueForKey("k6")->intValue();
            CCLOG("ChatServiceCocos2dx::enableNativeMail %d",(int)(ChatServiceCocos2dx::enableNativeMail));
        }
        
#endif
        if (function_on2->objectForKey("k10"))
        {
            UploadImageController::shared()->setUploadImageFlag(function_on2->valueForKey("k10")->intValue());
        }
        
    }
    else{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ChatServiceCocos2dx::enableNativeMail =false;
#endif
    }
    
    // chat notice
    if(dataConfig->objectForKey("function_on2")){
        CCDictionary* function_on2 = _dict(dataConfig->objectForKey("function_on2"));
        if(function_on2){
            bool isOpen = function_on2->valueForKey("k4")->boolValue();
            ChatController::getInstance()->setIsNoticeOpen(isOpen);
        }
    }
    GlobalData::shared()->isOpenTaiwanFlag = false;
    if (dataConfig->objectForKey("function_on3"))
    {
        CCDictionary* function_on3 = _dict(dataConfig->objectForKey("function_on3"));
        GlobalData::shared()->alliance_fun_on3_k5 =function_on3->valueForKey("k5")->intValue();
        if(function_on3->objectForKey("k9")){
            std::string connectHKServer = function_on3->valueForKey("k9")->getCString();
            CCUserDefault::sharedUserDefault()->setStringForKey("connectHKServer", connectHKServer);
        }else
        {
            CCUserDefault::sharedUserDefault()->setStringForKey("connectHKServer", "");
        }

        if (function_on3->objectForKey("k3"))
        {
            DailyActiveController::shared()->setDailyActiveOpenFlag(function_on3->valueForKey("k3")->intValue());
        }
        if (function_on3->objectForKey("k4")) {
            GlobalData::shared()->isOpenTaiwanFlag = function_on3->valueForKey("k4")->boolValue();
        }
        if(function_on3->objectForKey("k5")){
            int flag=function_on3->valueForKey("k5")->intValue();
            CCLOGFUNCF("function_on3 K5:%d",flag);
            if(flag==1)
                MailController::getInstance()->setChatRoomEnable(true);
            else
                MailController::getInstance()->setChatRoomEnable(false);
        }
        if (function_on3->objectForKey("k6")) {
//            GlobalData::shared()->isOpenTaiwanFlag = function_on3->valueForKey("k4")->boolValue();
            int flag=function_on3->valueForKey("k6")->intValue();
            CCLOGFUNCF("function_on3 K6:%d",flag);
        }
        //联盟领地超级矿开门初始化
        if (function_on3->objectForKey("k7")) {
            GlobalData::shared()->alliance_territory_tower_switch = function_on3->valueForKey("k7")->intValue();
        }
        if (!function_on3->objectForKey("k7")) {
            GlobalData::shared()->alliance_territory_tower_switch = 0;
        }
        //GlobalData::shared()->alliance_territory_tower_switch = 0;
        
        //周年庆开关1开0关
        GlobalData::shared()->isOpenAnniversary = false;
        if (function_on3->objectForKey("k10")) {
            GlobalData::shared()->isOpenAnniversary = function_on3->valueForKey("k10")->boolValue();
        }
        
        GlobalData::shared()->march_formation_switch = 0;
        if (function_on3->objectForKey("k11")) {
            GlobalData::shared()->march_formation_switch = function_on3->valueForKey("k11")->intValue();
        }
        //周年庆分享照片开关1开0关
        GlobalData::shared()->isAnniSharePhoto = false;
        if(function_on3->objectForKey("k12")){
            GlobalData::shared()->isAnniSharePhoto = function_on3->valueForKey("k12")->boolValue();
        }
    }
    if(dataConfig->objectForKey("function_on4")){
        CCDictionary* function_on4 = _dict(dataConfig->objectForKey("function_on4"));
        //Branch开关
        GlobalData::shared()->isBranchOn = false;
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(function_on4->objectForKey("k1")){
            GlobalData::shared()->isBranchOn = function_on4->valueForKey("k1")->boolValue();
        }
        #endif
        if(function_on4->objectForKey("k3")){
            GlobalData::shared()->anniSharePhoto = function_on4->valueForKey("k3")->intValue();
        }
        if(function_on4->objectForKey("k4")){
            GlobalData::shared()->branchOnStr = function_on4->valueForKey("k4")->getCString();
        }
        if(function_on4->objectForKey("k8")){
            GlobalData::shared()->shakeFlag = function_on4->valueForKey("k8")->intValue();
        }
        if(function_on4->objectForKey("k7")){
            GlobalData::shared()->shakeGuideFlag = function_on4->valueForKey("k7")->intValue();
        }
        
        GlobalData::shared()->alliance_territory_banner_switch = 0;
        if(function_on4->objectForKey("k9")){
            GlobalData::shared()->alliance_territory_banner_switch = function_on4->valueForKey("k9")->intValue();
        }

        if(function_on4->objectForKey("k10")){
//            GlobalData::shared()->hd_switch = function_on4->valueForKey("k10")->intValue();
//            CCUserDefault::sharedUserDefault()->setIntegerForKey(SERVER_SWITCH_HD, GlobalData::shared()->hd_switch);
//            CCUserDefault::sharedUserDefault()->flush();
        }
        if(function_on4->objectForKey("k11")){
            GlobalData::shared()->replayTime = function_on4->valueForKey("k11")->intValue();
        }
        if(function_on4->objectForKey("k12")){
            GlobalData::shared()->cnReplayFlag = function_on4->valueForKey("k12")->intValue();
        }
    }
    GlobalData::shared()->chinaSwitchFlag = 0;
    if(dataConfig->objectForKey("function_on5")){
    CCDictionary* function_on5 = _dict(dataConfig->objectForKey("function_on5"));
        if(function_on5->objectForKey("k1")){
            GlobalData::shared()->shakeAllianceFlag = function_on5->valueForKey("k1")->intValue();
        }
        if(function_on5->objectForKey("k2")){
            GlobalData::shared()->picUploadFlag = function_on5->valueForKey("k2")->intValue();
        }
        if(function_on5->objectForKey("k3")){
            GlobalData::shared()->sceneResourceStepLoad = function_on5->valueForKey("k3")->intValue();
        }
        if(function_on5->objectForKey("k4")){
            GlobalData::shared()->chinaSwitchFlag = function_on5->valueForKey("k4")->intValue();
        }
        if(function_on5->objectForKey("k5")){
            CCUserDefault::sharedUserDefault()->setStringForKey("forbiddenDevice", function_on5->valueForKey("k5")->getCString());
        }
        if(function_on5->objectForKey("k6")){
            CCUserDefault::sharedUserDefault()->setStringForKey("deviceServer", function_on5->valueForKey("k6")->getCString());
        }
        
        GlobalData::shared()->equipShare_control = "0";
        if(function_on5->objectForKey("k7")){
            GlobalData::shared()->equipShare_control = function_on5->valueForKey("k7")->getCString();
        }
        if (function_on5->objectForKey("k9"))
        {
            GlobalData::shared()->hd_switch_version = function_on5->valueForKey("k9")->getCString();
            if (CCCommonUtils::checkVersion(GlobalData::shared()->hd_switch_version)) {
                GlobalData::shared()->hd_switch = 1;
            } else {
                GlobalData::shared()->hd_switch = 0;
            }
            CCUserDefault::sharedUserDefault()->setIntegerForKey(SERVER_SWITCH_HD, GlobalData::shared()->hd_switch);
            CCUserDefault::sharedUserDefault()->flush();
        }
    }
    GlobalData::shared()->dragonglass_control = "0";
    if(dataConfig->objectForKey("dragonglass_control")){
        CCDictionary* dragonglass_control = _dict(dataConfig->objectForKey("dragonglass_control"));
        if(dragonglass_control->valueForKey("k2")){
            string dragonglass_k2 = dragonglass_control->valueForKey("k2")->getCString();
            if (dragonglass_k2=="1"){
                if(dragonglass_control->valueForKey("k1")){
                    string dragonglass_k1 = dragonglass_control->valueForKey("k1")->getCString();
                    GlobalData::shared()->dragonglass_control = dragonglass_k1;
                }
            }
        }
    }
//    GlobalData::shared()->dragonglass_control = "1";
    
    if(dataConfig->objectForKey("shake")){
        CCDictionary* shakeDic = _dict(dataConfig->objectForKey("shake"));
        if(shakeDic->objectForKey("k5")){
            GlobalData::shared()->shakeCmdDelayTime = shakeDic->valueForKey("k5")->intValue();
        }
        if(shakeDic->objectForKey("k7")){
            GlobalData::shared()->shakePicStr = shakeDic->valueForKey("k7")->getCString();
        }
    }
    
    if(dataConfig->objectForKey("notice")){
        CCDictionary* chatNotice = _dict(dataConfig->objectForKey("notice"));
        if(chatNotice){
            int showT = chatNotice->valueForKey("k1")->doubleValue() / 1000;
            ChatController::getInstance()->setNoticeShowTime(showT);
            int maxNum = chatNotice->valueForKey("k2")->intValue();
            ChatController::getInstance()->setNoticeCharMax(maxNum);
            int price = chatNotice->valueForKey("k3")->intValue();
            ChatController::getInstance()->setNoticePrice(price);
            
        }
    }
    //抽奖数据初始化
    CCDictionary* lotteryData = _dict(params->objectForKey("lottery"));
    LotteryController::shared()->parseLotteryData(lotteryData);
    LotteryController::shared()->parseLotteryData2(dataConfig);
    //聊天屏蔽
    auto &chatShieldInfo = GlobalData::shared()->chatShieldInfo;
    CCArray* arr = dynamic_cast<CCArray*>(params->objectForKey("chatShield"));
    if(arr){
        chatShieldInfo.parse(arr);
    }
    if (dataConfig->objectForKey("chatShieldMax")) {
        chatShieldInfo.maxNum = dataConfig->valueForKey("chatShieldMax")->intValue();
    }
    
//    CCDictionary *tavernConfig = _dict(dataConfig->objectForKey("tavern_base"));
//    GlobalData::shared()->tavernConfig.parse(tavernConfig);
    
    CCArray* tmpArr = dynamic_cast<CCArray*>(params->objectForKey("exchange_gift"));
    if(tmpArr){
        LuaController::getInstance()->initExchangeGiftInfo(tmpArr);
    }
    if(params->objectForKey("repayinfo")){
        CCDictionary* repayConfig = _dict(params->objectForKey("repayinfo"));
        if(repayConfig){
            RepayController::getInstance()->initRepayInfo(repayConfig);
        }
    }else{
        RepayController::getInstance()->initRepayInfo(NULL);
    }
    if (dataConfig->objectForKey("wb_fire")) {
        CCDictionary *wb_fire = _dict(dataConfig->objectForKey("wb_fire"));
        int k1 = wb_fire->valueForKey("k1")->intValue();
        GlobalData::shared()->wb_fire_k1 = k1;//k1表示每小时的燃烧城防值
    }
    if (dataConfig->objectForKey("wb_condition")) {//控制跨服进入条件
        CCDictionary *wb_condition = _dict(dataConfig->objectForKey("wb_condition"));
        int k1 = wb_condition->valueForKey("k1")->intValue();
        int k2 = wb_condition->valueForKey("k2")->intValue();
        int k3 = wb_condition->valueForKey("k3")->intValue();
        ActivityController::getInstance()->wb_condition_k1 = k1;
        ActivityController::getInstance()->wb_condition_k2 = k2;
        ActivityController::getInstance()->wb_condition_k3 = k3;
    }
    if (dataConfig->objectForKey("wb_info")) {
        CCDictionary *wb_info = _dict(dataConfig->objectForKey("wb_info"));
        int k7 = wb_info->valueForKey("k7")->intValue();
        int k6 = wb_info->valueForKey("k6")->intValue();
        int k9 = wb_info->valueForKey("k9")->intValue();
        float k10 = wb_info->valueForKey("k10")->floatValue();
        int k11 = wb_info->valueForKey("k11")->intValue();
        ActivityController::getInstance()->wb_info_k2 = wb_info->valueForKey("k2")->getCString();
        if(k7<=0){
            k7 = 1;
        }
        if(k9<=0){
            k9 = 1;
        }
        if(k10<=0){
            k10 = 1;
        }
        ActivityController::getInstance()->wb_info_k6 = k6;
        ActivityController::getInstance()->wb_info_k7 = k7;
        ActivityController::getInstance()->wb_info_k9 = k9;
        ActivityController::getInstance()->wb_info_k10 = k10;
        ActivityController::getInstance()->wb_info_k11 = k11;
    }
    ActivityController::getInstance()->wbScoreMap.clear();
    if (dataConfig->objectForKey("wb_score")) {
        CCDictionary *wb_score = _dict(dataConfig->objectForKey("wb_score"));
        CCDictElement *pObj;
        CCDICT_FOREACH(wb_score, pObj) {
            string key = pObj->getStrKey();
            string value = dynamic_cast<CCString*>(pObj->getObject())->getCString();
            ActivityController::getInstance()->wbScoreMap[key] = value;
        }
    }
    if (dataConfig->objectForKey("wb_timing")) {
        CCDictionary *wb_timing = _dict(dataConfig->objectForKey("wb_timing"));
        int k1 = wb_timing->valueForKey("k1")->intValue();
        int k2 = wb_timing->valueForKey("k2")->intValue();
        ActivityController::getInstance()->wb_timing_k1 = k1;
        ActivityController::getInstance()->wb_timing_k2 = k2;
    }
    if (dataConfig->objectForKey("wb_switch")) {
        CCDictionary *wb_switch = _dict(dataConfig->objectForKey("wb_switch"));
        int k1 = wb_switch->valueForKey("k1")->intValue();
        string k2 = wb_switch->valueForKey("k2")->getCString();
        int k3 = wb_switch->valueForKey("k3")->intValue();
        string k4 = wb_switch->valueForKey("k4")->getCString();
        int k6 = wb_switch->valueForKey("k6")->intValue();
        string k7 = wb_switch->valueForKey("k7")->getCString();
        ActivityController::getInstance()->wb_switch_k1 = k1;
        ActivityController::getInstance()->wb_switch_k2 = k2;
        ActivityController::getInstance()->wb_switch_k3 = k3;
        ActivityController::getInstance()->wb_switch_k4 = k4;
        ActivityController::getInstance()->wb_switch_k6 = k6;
        ActivityController::getInstance()->wb_switch_k7 = k7;
    }
}

void CCCommonUtils::initEffect(CCDictionary *params){
    // effectValues map init
    if (params->objectForKey("effect")) {
        auto &effectMap = GlobalData::shared()->effectValues;
        auto effectDict = _dict(params->objectForKey("effect"));
        CCDictElement *pObj;
        CCDICT_FOREACH(effectDict, pObj) {
            string key = pObj->getStrKey();
            float value = atoi((dynamic_cast<CCString*>(pObj->getObject())->getCString()));
            effectMap[atoi(key.c_str())] = value;
        }
    }
}

void CCCommonUtils::initTime(CCDictionary *params){
    //时间初始化
    GlobalData::shared()->setWorldTime(params->valueForKey("db_timezone_offset")->intValue(), params->valueForKey("db_utc_timestamp")->intValue());
    
    // init timeStamp
    auto timeStampForever = TimeStamp();
    timeStampForever.type = Stamp_FOREVER;
    timeStampForever.finishTime = std::numeric_limits<int>::max() - 1;
    GlobalData::shared()->timeStampMap[0] = timeStampForever;
}

void CCCommonUtils::initTranslation(CCDictionary *params){
    //自动翻译
    if(params->objectForKey("translation")){
        GlobalData::shared()->translation = params->valueForKey("translation")->boolValue();
    }
    if(params->objectForKey("mail_translation")){
        GlobalData::shared()->mail_translation = params->valueForKey("mail_translation")->boolValue();
    }else{
        GlobalData::shared()->mail_translation = false;
    }
}

void CCCommonUtils::initGaid(CCDictionary *params){
    //gaid
    if(params->objectForKey("gaid")){
        GlobalData::shared()->gaid = params->valueForKey("gaid")->getCString();
    }
}

void CCCommonUtils::initEffectState(CCDictionary *params){
    // play state
    if (params->objectForKey("effectState")) {
        // search for type 500000 for protect stamp
        auto stateDict = dynamic_cast<CCDictionary*>(params->objectForKey("effectState"));
        if (stateDict) {
            for(int i=0;i<10;i++){
                string temp = CC_ITOA(PLAYER_PROTECTED_TIME1+i);
                if(stateDict->objectForKey(temp)){
                    GlobalData::shared()->playerInfo.protectTimeStamp = stateDict->valueForKey(temp)->doubleValue();
                }
            }
            GlobalData::shared()->playerInfo.resourceProtectTimeStamp = stateDict->valueForKey(CC_ITOA(LORD_SKILL_PROTECTED))->doubleValue();
            
            CCDictElement* element;
            CCDICT_FOREACH(stateDict, element)
            {
                string key = element->getStrKey();
                double time = stateDict->valueForKey(key)->doubleValue()/1000;
                if (time>0) {
                    time = GlobalData::shared()->changeTime(time);
                }
                GlobalData::shared()->statusMap[atoi(key.c_str())] = time;
            }
            ToolController::getInstance()->checkMaincityCustom();
        }
    }
}

void CCCommonUtils::initStatus(CCDictionary *params){
    if (params->objectForKey("status")) {
        auto arr = dynamic_cast<CCArray*>(params->objectForKey("status"));
        CCDictionary *item = NULL;
        GlobalData::shared()->effectStateMap.clear();
        for (int i=0; i<arr->count(); i++) {
            item = _dict(arr->objectAtIndex(i));
            auto effState = stateEffect();
            effState.effectId = item->valueForKey("effNum")->intValue();
            effState.value = item->valueForKey("effVal")->intValue();
            effState.stateId = item->valueForKey("stateId")->intValue();
            GlobalData::shared()->effectStateMap[effState.effectId].push_back(effState);
        }
    }
}

void CCCommonUtils::initWorld(CCDictionary *params){
    TimeUpMarchProcesser::getInstance()->clear();
    // world info
    if (params->objectForKey("world")) {
        auto worldInfo = _dict(params->objectForKey("world"));
        unsigned int point = worldInfo->valueForKey("point")->intValue();
        WorldController::getInstance()->enterMap();
        if (!point) {
            WorldController::getInstance()->selfPoint = ccp(-1, -1);
        } else {
            WorldController::getInstance()->selfPoint = WorldController::getPointByIndex(point);
        }
        // init world time
        double stamp = (double)params->valueForKey("db_timezone_offset")->intValue() * 1000.0;
        WorldController::getInstance()->resetTime(stamp);
        GlobalData::shared()->cityDefenseVal = worldInfo->valueForKey("cityDefValue")->intValue();
        GlobalData::shared()->cityFireStamp = worldInfo->valueForKey("ft")->doubleValue();
        GlobalData::shared()->cityStartFireStamp = WorldController::getInstance()->getTime();
        GlobalData::shared()->cityLastUpdateDefenseStamp = worldInfo->valueForKey("lastCityDefTime")->doubleValue();
        GlobalData::shared()->cityTileCountry = worldInfo->valueForKey("gridType")->intValue();
        ActivityController::getInstance()->ckfNeedLeave = worldInfo->valueForKey("ckfNeedLeave")->intValue();
        ActivityController::getInstance()->ckfProtectTime = 0;
        if(worldInfo->objectForKey("ckfProtectTime")){
            double ckfProtectTime = worldInfo->valueForKey("ckfProtectTime")->doubleValue() / 1000;
            if (ckfProtectTime!=0) {
                ActivityController::getInstance()->ckfProtectTime = GlobalData::shared()->changeTime(ckfProtectTime);
            }
        }
        
        if(GlobalData::shared()->cityTileCountry == NEUTRALLY)
        {
            if(WorldController::getInstance()->selfPoint.x<574||WorldController::getInstance()->selfPoint.x>626||WorldController::getInstance()->selfPoint.y<574||WorldController::getInstance()->selfPoint.y>626)
            {
                GlobalData::shared()->cityTileCountry=SPACE;
            }
        }
        WorldController::getInstance()->m_mainThreadID = pthread_self();
        
        // init self march info
        WorldController::getInstance()->initMarchInfo(dynamic_cast<CCArray*>(worldInfo->objectForKey("m")));
        if(worldInfo->objectForKey("stamina")){
            WorldController::getInstance()->currentStamine = worldInfo->valueForKey("stamina")->intValue();
        }
        if(worldInfo->objectForKey("lastStaminaTime")){
            WorldController::getInstance()->lastStamineTime = worldInfo->valueForKey("lastStaminaTime")->doubleValue();
        }
    }
   
    if(params->objectForKey("treasureMap")){
        WorldController::getInstance()->initTreasureInfo(dynamic_cast<CCArray*>(params->objectForKey("treasureMap")));
    }else{
        WorldController::getInstance()->clearTreasureInfo();
    }
}

void CCCommonUtils::initPray(CCDictionary *params){
    if (params->objectForKey("pray")) {
        auto sacrificeDict = _dict(params->objectForKey("pray"));
        auto &sacrifice = GlobalData::shared()->sacrificeInfo;
        sacrifice.parse(sacrificeDict);
        
    }
}

void CCCommonUtils::initLord(CCDictionary *params){
    CCDictionary* lord = _dict(params->objectForKey("lord"));
    
    GlobalData::shared()->generalConfig.oneTimeFree = lord->valueForKey("singleRndGenFreeNum")->intValue();
    GlobalData::shared()->generalConfig.firstRndGenFlag = lord->valueForKey("firstRndGenFlag")->intValue();
    GlobalData::shared()->bagGoldBuyNum = lord->valueForKey("buyGeneralTimes")->intValue();
    WorldController::getInstance()->currentMonsterLevel = lord->valueForKey("pveLevel")->intValue();
    GlobalData::shared()->medalid = lord->valueForKey("medal")->getCString();
    
    if (lord->objectForKey("nextDay")) {
        int tmpTime = lord->valueForKey("nextDay")->doubleValue()/1000;
        if (tmpTime>0) {
            ToolController::getInstance()->m_lastTime = GlobalData::shared()->changeTime(tmpTime);
        }
    }
    if (lord->objectForKey("exchange")) {
        ToolController::getInstance()->m_canBuy = lord->valueForKey("exchange")->boolValue();
    }
    //马厩领取时间
    GlobalData::shared()->lastAcceptEnergyTime = lord->valueForKey("lastAcceptEnergyTime")->doubleValue() / 1000;
    
    if (lord->objectForKey("firstRndGenFlag")) {
        GlobalData::shared()->generalConfig.firstRndGenFlag = lord->valueForKey("firstRndGenFlag")->intValue();
    }
    if (lord->objectForKey("levelUp")) {
        GlobalData::shared()->lordInfo.levelUp= lord->valueForKey("levelUp")->intValue();
    }
    auto &loginReward = GlobalData::shared()->loginReward;
    loginReward.canDoLottery = lord->valueForKey("loginAward")->boolValue();
    
    GlobalData::shared()->lordInfo.energy = lord->valueForKey("energy")->intValue();
    GlobalData::shared()->lordInfo.nextEnergyRecoverTime = lord->valueForKey("nextEnergyRecoverTime")->doubleValue();
    GlobalData::shared()->lordInfo.buyEnergyTimesDaily = lord->valueForKey("buyEnergyTimesDaily")->intValue();
    if (lord->objectForKey("feedCount")) {
        GlobalData::shared()->lordInfo.feed_count = lord->valueForKey("feedCount")->intValue();
    }
}

void CCCommonUtils::initPushSetting(CCDictionary *params){
    if (params->objectForKey("pushSetting")) {
        auto pushSettingArr = dynamic_cast<CCArray*>(params->objectForKey("pushSetting"));
        CCObject* pushSettingObj = NULL;
        CCARRAY_FOREACH(pushSettingArr, pushSettingObj){
            PushSettingInfo* pushSettingInfo = PushSettingInfo::create();
            pushSettingInfo->retain();
            pushSettingInfo->parse(dynamic_cast<CCDictionary*>(pushSettingObj));
            GlobalData::shared()->pushSettingVector.push_back(pushSettingInfo);
        }
    }
}

void CCCommonUtils::initBusinessMan(CCDictionary *params){
    //行走商人
    if(params->objectForKey("businessMan")){
        ToolController::getInstance()->initMerchanteInfo(dynamic_cast<CCDictionary*>(params->objectForKey("businessMan")));
    }
}

void CCCommonUtils::initMonthLyCards(CCDictionary *params){
    //月卡信息
    GlobalData::shared()->tomorrow_time = params->valueForKey("tomorrow")->doubleValue();
    GlobalData::shared()->monthCardInfoList.clear();
    if(params->objectForKey("monthLyCards")){
        auto arr = dynamic_cast<CCArray*>(params->objectForKey("monthLyCards"));
        CCDictionary *item = NULL;
        for (int i=0; i<arr->count(); i++) {
            item = _dict(arr->objectAtIndex(i));
            std::string cardid = item->valueForKey("itemId")->getCString();
            if(!cardid.empty()){
                auto cardInfo = new MonthCardInfo(item);
                GlobalData::shared()->monthCardInfoList[cardInfo->getItemId()] =  cardInfo;
            }
        }
    }
}

void CCCommonUtils::initPlayerInfo(CCDictionary *params){
    //用户信息初始化
    auto &playerInfo = GlobalData::shared()->playerInfo;
    // new user
    if (params->objectForKey("newAccount")) {
        GlobalData::shared()->isFirstLoginGame = params->valueForKey("newAccount")->boolValue();
    } else {
        GlobalData::shared()->isFirstLoginGame = false;
    }
    
    if (params->objectForKey("isShowCDkey")) {
        GlobalData::shared()->isShowCDkey = params->valueForKey("isShowCDkey")->boolValue();
    } else {
        GlobalData::shared()->isShowCDkey = false;
    }
    CCDictionary* pUserInfo = _dict(params->objectForKey("user"));
    GlobalData::shared()->serverMax = pUserInfo->valueForKey("serverMax")->intValue();
    if(pUserInfo->objectForKey("phoneDevice")){
        GlobalData::shared()->phoneDevice = pUserInfo->valueForKey("phoneDevice")->getCString();
    }
    WorldController::getInstance()->initServerMapPos();
    if(pUserInfo->objectForKey("serverId")){
        playerInfo.selfServerId = pUserInfo->valueForKey("serverId")->intValue();
        if(!GlobalData::shared()->isPressedHomeKey || playerInfo.currentServerId == -1)//(for change serve && press home btn;for first login should init)
        {
            playerInfo.currentServerId = playerInfo.selfServerId;
        }
    }
    if(pUserInfo->objectForKey("chNameCount")){
        playerInfo.renameTime = pUserInfo->valueForKey("chNameCount")->intValue();
    }
    playerInfo.level = pUserInfo->valueForKey("level")->intValue();
    playerInfo.exp = pUserInfo->valueForKey("exp")->intValue();
    playerInfo.maxExp = pUserInfo->valueForKey("maxExp")->intValue();
    playerInfo.gold = pUserInfo->valueForKey("gold")->intValue();
    playerInfo.longJing = pUserInfo->valueForKey("crystal")->intValue();//龙晶
    playerInfo.payTotal = pUserInfo->valueForKey("payTotal")->intValue();
    playerInfo.name = pUserInfo->valueForKey("name")->getCString();
    playerInfo.nickName = pUserInfo->valueForKey("nickName")->getCString();
    playerInfo.uid = pUserInfo->valueForKey("uid")->getCString();
    playerInfo.pic = pUserInfo->valueForKey("pic")->getCString();
    playerInfo.country = pUserInfo->valueForKey("country")->intValue();
    playerInfo.openedSystem = pUserInfo->valueForKey("openedSystem")->getCString();
    playerInfo.gmFlag = pUserInfo->valueForKey("gmFlag")->intValue();
//    playerInfo.gmFlag = 1;
    playerInfo.isfirstJoin = pUserInfo->valueForKey("isfirstJoin")->intValue();
    playerInfo.registerTime = pUserInfo->valueForKey("registerTime")->doubleValue() / 1000;
    playerInfo.bindFlag = pUserInfo->valueForKey("bindFlag")->boolValue();
    playerInfo.inviCode = pUserInfo->valueForKey("inviCode")->getCString();
    playerInfo.regCountry = pUserInfo->valueForKey("regCountry")->getCString();
    playerInfo.officer = pUserInfo->valueForKey("officer")->getCString();
    playerInfo.relinquishFlag = pUserInfo->valueForKey("relinquishFlag")->boolValue();
    playerInfo.relinquishEndTime = pUserInfo->valueForKey("relinquishEndTime")->doubleValue();
    if (pUserInfo->objectForKey("allianceId")) {
        playerInfo.setAllianceId(pUserInfo->valueForKey("allianceId")->getCString());
    }
    if (pUserInfo->objectForKey("lastUpdateTime")) {
        playerInfo.lastUpdateTime = pUserInfo->valueForKey("lastUpdateTime")->intValue();
    }
    
    if (pUserInfo->objectForKey("openedPos")) {
        FunBuildController::getInstance()->initOpenTile(pUserInfo->valueForKey("openedPos")->getCString());
    }
    if (pUserInfo->objectForKey("picVer")) {
        playerInfo.picVer = pUserInfo->valueForKey("picVer")->intValue();
    }
    cocos2d::extension::CCDevice::setTestinUser(playerInfo.uid);
    if (pUserInfo->objectForKey("deviceBindTimes")) {
        GlobalData::shared()->nowGameCnt = pUserInfo->valueForKey("deviceBindTimes")->intValue();
    }
    if(pUserInfo->objectForKey("noticeBanTime")){
        ChatController::getInstance()->setNoticeBanTime(pUserInfo->valueForKey("noticeBanTime")->doubleValue()/1000);
    }
    CCDictionary* allianceInfo = _dict(params->objectForKey("alliance"));
    if (allianceInfo)
    {
        playerInfo.allianceInfo.updateAllianceInfo(allianceInfo);
    }
    if (pUserInfo->objectForKey("crossFightSrcServerId")) {
        int crossFightSrcServerId = pUserInfo->valueForKey("crossFightSrcServerId")->intValue();
        GlobalData::shared()->playerInfo.crossFightSrcServerId = crossFightSrcServerId;
        GlobalData::shared()->isCrossService = crossFightSrcServerId>=0?true:false;//crossFightSrcServerId = -1 表示没有跨服， >=0表示现在处于跨服状态
    }
    
//#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
//    ChatServiceCocos2dx::setPlayerAllianceInfo(playerInfo.allianceInfo.shortName.c_str(),playerInfo.getAllianceId().c_str(),playerInfo.allianceInfo.rank,playerInfo.isfirstJoin);
//#endif
    
    if(pUserInfo->objectForKey("signIn")){
        PortActController::getInstance()->m_isRdLoginDay = pUserInfo->valueForKey("signIn")->intValue();
    }
    
    CCDictionary* roleInfo = _dict(params->objectForKey("playerInfo"));
    playerInfo.updateInfo(roleInfo);
    //查看装备
    CCDictionary* seeUserEquip = _dict(params->objectForKey("mixedInfo"));
    if(seeUserEquip->objectForKey("showEquip")){//是否装备别人可见
        playerInfo.showEquip = seeUserEquip->valueForKey("showEquip")->boolValue();
    }
    //材料工坊 材料建造位置数
    if(seeUserEquip->objectForKey("materialBackup")){//材料建造位置数
        playerInfo.materialBackup = seeUserEquip->valueForKey("materialBackup")->intValue();
    }
    
    //账号绑定
    string _gp = pUserInfo->valueForKey("googlePlay")->getCString();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    _gp = pUserInfo->valueForKey("appStoreId")->getCString();
#endif
    string _localGP = CCUserDefault::sharedUserDefault()->getStringForKey(GP_USERID,"");
    if (_gp != _localGP) {
        CCUserDefault::sharedUserDefault()->setStringForKey(GP_USERID, _gp);
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#else
    string _gpAccountname = pUserInfo->valueForKey("googleAccountName")->getCString();
    string _localAccountname = CCUserDefault::sharedUserDefault()->getStringForKey(GP_USERNAME,"");
    if (_gpAccountname != _localAccountname) {
        CCUserDefault::sharedUserDefault()->setStringForKey(GP_USERNAME, _gpAccountname);
    }
#endif
    
    string _fb = pUserInfo->valueForKey("facebook")->getCString();
    string _localFB = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
    if (_fb != _localFB) {
        CCUserDefault::sharedUserDefault()->setStringForKey(FB_USERID, _fb);
    }
    string _fbAccountname = pUserInfo->valueForKey("facebookAccountName")->getCString();
    string _localFBAccountname = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERNAME,"");
    if (_fbAccountname != _localFBAccountname) {
        CCUserDefault::sharedUserDefault()->setStringForKey(FB_USERNAME, _fbAccountname);
    }
    playerInfo.m_platFormBindInfo.clear();
    if(GlobalData::shared()->isWeiboEnabled()){
        playerInfo.m_platFormBindInfo[WEIBO].bindPf = "weibo";
    }
    if(GlobalData::shared()->isVKEnabled()){
        playerInfo.m_platFormBindInfo[VK].bindPf = "vk";
    }
    if(pUserInfo->objectForKey("bindInfo")){
        CCLog("bindInfo:");
        auto arr = dynamic_cast<CCArray*>(pUserInfo->objectForKey("bindInfo"));
        if(arr){
            CCObject *obj;
            CCARRAY_FOREACH(arr, obj) {
                CCDictionary *bindDict = _dict(obj);
                std::string bindPf = bindDict->valueForKey("bindPf")->getCString();
                CCLog("bindPf:%s", bindPf.c_str());
                int type = getBindType(bindPf);
                if(playerInfo.m_platFormBindInfo.find(type) == playerInfo.m_platFormBindInfo.end()){
                    playerInfo.m_platFormBindInfo[type] = UserPlatformInfo();
                }
                playerInfo.m_platFormBindInfo[type].parse(bindDict);
            }
        }
    }
    
    string _device = pUserInfo->valueForKey("deviceId")->getCString();
    string _localDevice = CCUserDefault::sharedUserDefault()->getStringForKey(DEVICE_UID,"");
    if (_device != _localDevice) {
        CCUserDefault::sharedUserDefault()->setStringForKey(DEVICE_UID, _device);
    }
    string _mail = pUserInfo->valueForKey("email")->getCString();
    string _localMail = CCUserDefault::sharedUserDefault()->getStringForKey(CUSTOM_UID,"");
    if (_mail != _localMail) {
        CCUserDefault::sharedUserDefault()->setStringForKey(CUSTOM_UID, _mail);
    }
    int _emailConfirm = pUserInfo->valueForKey("emailConfirm")->intValue();
    int _localEmailConfirm = CCUserDefault::sharedUserDefault()->getIntegerForKey(Email_Confirm,0);
    if (_emailConfirm != _localEmailConfirm) {
        CCUserDefault::sharedUserDefault()->setIntegerForKey(Email_Confirm, _emailConfirm);
    }
    
    playerInfo.roomName = "";
    if (params->objectForKey("roomName")) {
        playerInfo.roomName = params->valueForKey("roomName")->getCString();
    }
    
    GlobalData::shared()->focusBuildId = -1;
    //uuid
    string _uuid = pUserInfo->valueForKey("uuid")->getCString();
    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID, _uuid);
    if(playerInfo.uid != "")
        CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, playerInfo.uid);
    CCUserDefault::sharedUserDefault()->flush();
    GlobalData::shared()->contryResType = 0;
    if (playerInfo.country == 1) {
        GlobalData::shared()->contryResType = 0;
    }
    else if (playerInfo.country == 2)
    {
        GlobalData::shared()->contryResType = 1;
    }
    else if (playerInfo.country == 3)
    {
        GlobalData::shared()->contryResType = 2;
    }
    GlobalData::shared()->contryResType = 2;
    
    
    // add midnight update
    GameController::getInstance()->addMidnightObserver(0.0);
    
    CCDictionary* vipInfo = dynamic_cast<CCDictionary*>(params->objectForKey("vip"));
    if (vipInfo)
    {
        playerInfo.vipPoints = vipInfo->valueForKey("score")->intValue();
        playerInfo.vipEndTime = vipInfo->valueForKey("vipEndTime")->doubleValue()/1000;
        if (playerInfo.vipEndTime>0) {
            playerInfo.vipEndTime = GlobalData::shared()->changeTime(playerInfo.vipEndTime);
        }
        playerInfo.nextDayLoginVipReward = vipInfo->valueForKey("nextDayScore")->intValue();
        playerInfo.consecutiveLoginDays = vipInfo->valueForKey("loginDays")->intValue();
        playerInfo.updateVipEffect(true);
    }
    if(params->objectForKey("firstPayReward")){
        GlobalData::shared()->payRewardInfo.parse(params);
        
    }
    
    if(pUserInfo->objectForKey("guideStep")){
        GuideController::share()->m_netSaveStep = pUserInfo->valueForKey("guideStep")->getCString();
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::setCurrentUserId(playerInfo.uid);
    ChatServiceCocos2dx::initDatabase();
    ChatServiceCocos2dx::setPlayerInfo();
    ChatServiceCocos2dx::setPlayerAllianceInfo(playerInfo.allianceInfo.shortName.c_str(),playerInfo.getAllianceId().c_str(),playerInfo.allianceInfo.rank,playerInfo.isfirstJoin);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ChatServiceCocos2dx::setCurrentUserId(playerInfo.uid);
//初始化db暂时不需要
    ChatServiceCocos2dx::initDatabase();
    ChatServiceCocos2dx::setPlayerInfo();
    ChatServiceCocos2dx::setPlayerAllianceInfo(playerInfo.allianceInfo.shortName.c_str(),playerInfo.getAllianceId().c_str(),playerInfo.allianceInfo.rank,playerInfo.isfirstJoin);
#endif
    //登陆成功android记录玩家uid，用于xingcloud数据统计
    GameController::getInstance()->platformCollectUserInfo(GlobalData::shared()->playerInfo.uid,GlobalData::shared()->playerInfo.name,CC_ITOA(GlobalData::shared()->playerInfo.level));
    AppLibHelper::triggerEventLoginComplete(GlobalData::shared()->playerInfo.uid,GlobalData::shared()->playerInfo.name,CC_ITOA(GlobalData::shared()->playerInfo.level));
}

void CCCommonUtils::initMails(CCDictionary *params){
    //邮件列表初始化
    CCArray* mailArr = dynamic_cast<CCArray*>(params->objectForKey("mails"));
    if(mailArr){
        int length = mailArr->count();
        if(length>0 && GlobalData::shared()->mailList.size()>0){
            GlobalData::shared()->mailList.clear();
        }
        CCDictionary* info = NULL;
        for(int i = 0; i < length; i++){
            info = _dict(mailArr->objectAtIndex(i));
            MailController::getInstance()->addMail(info,false,false);
        }
    }
    
    CCDictionary* mailcountobj = _dict(params->objectForKey("mailCount"));
    GlobalData::shared()->mailCountObj.saveR = mailcountobj->valueForKey("saveR")->intValue();
    GlobalData::shared()->mailCountObj.perR = mailcountobj->valueForKey("perR")->intValue();
    GlobalData::shared()->mailCountObj.sysR = mailcountobj->valueForKey("sysR")->intValue();
    GlobalData::shared()->mailCountObj.saveT = mailcountobj->valueForKey("saveT")->intValue();
    GlobalData::shared()->mailCountObj.sysT = mailcountobj->valueForKey("sysT")->intValue();
    GlobalData::shared()->mailCountObj.perT = mailcountobj->valueForKey("perT")->intValue();
    GlobalData::shared()->mailCountObj.upR = mailcountobj->valueForKey("upR")->intValue();
    
    GlobalData::shared()->mailCountObj.studioR = mailcountobj->valueForKey("studioR")->intValue();
    GlobalData::shared()->mailCountObj.studioT = mailcountobj->valueForKey("studioT")->intValue();
    GlobalData::shared()->mailCountObj.fightR = mailcountobj->valueForKey("fightR")->intValue();
    GlobalData::shared()->mailCountObj.fightT = mailcountobj->valueForKey("fightT")->intValue();
    GlobalData::shared()->mailCountObj.modR = mailcountobj->valueForKey("modR")->intValue();
    GlobalData::shared()->mailCountObj.modT = mailcountobj->valueForKey("modT")->intValue();
    GlobalData::shared()->mailCountObj.total = mailcountobj->valueForKey("total")->intValue();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(MailController::getInstance()->getIsNewMailListEnable())
    {
        ChatServiceCocos2dx::setGlobalMailCount(GlobalData::shared()->mailCountObj.saveR,GlobalData::shared()->mailCountObj.studioR,
                                                GlobalData::shared()->mailCountObj.fightR,GlobalData::shared()->mailCountObj.modR);
    }
#endif
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
}

void CCCommonUtils::initTrain(CCDictionary *params){
    //训练的英雄
    auto trainDic = LocalController::shared()->DBXMLManager()->getGroupByKey("train");
    if(trainDic)
    {
        CCDictElement* element;
        CCDICT_FOREACH(trainDic, element)
        {
            CCDictionary* dictInfo = _dict(element->getObject());
            TrainInfo* train = new TrainInfo(dictInfo);
            GlobalData::shared()->trainList[train->m_trainId] = train;
        }
    }
    CCArray* trainArr = dynamic_cast<CCArray*>(params->objectForKey("train"));
    if(trainArr!=NULL){
        int trainNum = trainArr->count();
        CCDictionary* trainInfo = NULL;
        for(int i=0;i<trainNum;i++){
            trainInfo = _dict(trainArr->objectAtIndex(i));
            std::string id = trainInfo->valueForKey("trainId")->getCString();
            map<string,TrainInfo* >::iterator iter = GlobalData::shared()->trainList.find(id);
            if(iter!=GlobalData::shared()->trainList.end()){
                iter->second->updateTrainInfo(trainInfo);
            }
        }
    }
}

void CCCommonUtils::initResource(CCDictionary *params){
    //资源初始化
    CCDictionary* cityInfo = _dict(params->objectForKey("resource"));
    GlobalData::shared()->resourceInfo.setResourceData(cityInfo);
}

void CCCommonUtils::initEnemyInfo(CCDictionary *params){
    //世界信息初始化
    CCDictionary* worldDict = _dict(params->objectForKey("world"));
    EnemyInfoController::getInstance()->initEnemyData(worldDict);
    if (worldDict->objectForKey("srcServerPointId")) {
        int srcServerPointId = worldDict->valueForKey("srcServerPointId")->intValue();
        GlobalData::shared()->playerInfo.srcServerPointId = srcServerPointId;
    }
}

void CCCommonUtils::initAffair(CCDictionary *params){
//    CCArray *affairArr = dynamic_cast<CCArray*>(params->objectForKey("affairs"));
//    if(affairArr){
//        AffairsController::getInstance()->addAffairs(affairArr);
//        CCDictionary *affair = dynamic_cast<CCDictionary*>(params->objectForKey("affairsValue"));
//        AffairsController::getInstance()->peopleReward = affair->valueForKey("reward")->getCString();
//        AffairsController::getInstance()->setPeople(affair->valueForKey("people")->intValue());
//        AffairsController::getInstance()->setRefreshTime(affair->valueForKey("time")->doubleValue() / 1000, NORMAL_AFFAIR);
//        AffairsController::getInstance()->setRefreshTime(affair->valueForKey("time")->doubleValue() / 1000, LEAGUE_AFFAIR);
//    }
}

void CCCommonUtils::initWall(CCDictionary *params){
    //城墙信息初始化
    CCDictionary* dictWall = _dict(params->objectForKey("wall"));
    if (dictWall->objectForKey("autoType")) {
        TroopsController::getInstance()->m_autoType = dictWall->valueForKey("autoType")->intValue();
    }
    if (dictWall->objectForKey("defense")) {
        TroopsController::getInstance()->initDefenceData(_dict(dictWall->objectForKey("defense")));
    }
}

void CCCommonUtils::initActivity(CCDictionary *params){
    CCDictionary* scoreAct = _dict(params->objectForKey("scoreAct"));
    ActivityController::getInstance()->eventInfo.parse(scoreAct);
    
    CCArray* activityEvents = dynamic_cast<CCArray*>(params->objectForKey("activity"));
    ActivityController::getInstance()->paraseEventObj(activityEvents);
    
    CCDictionary* mine = _dict(params->objectForKey("mine"));
    SocialController::getInstance()->parse(mine);
    
    CCDictionary* trialDic = _dict(params->objectForKey("trialField"));
    ActivityController::getInstance()->parseTrialInfo(trialDic);
}

void CCCommonUtils::initQueue(CCDictionary *params){
    //建造队列信息初始化
    QueueController::getInstance()->initQueueData(params);
    QueueController::getInstance()->initLoginFinishQueues(params);
}

void CCCommonUtils::initBuilding(CCDictionary *params){
    //建筑信息初始化
    FunBuildController::getInstance()->retDataFromServer(params);
}
void CCCommonUtils::initDragonBuilding(CCDictionary *params){
    //初始化龙建筑信息
    DragonBuildingController::getInstance()->retDataFromServer(params);
}
void CCCommonUtils::initDragonsInfo(CCDictionary *params){
    //初始化龙信息
    DragonController::getInstance()->setDragonDataFromServer(params);
}

void CCCommonUtils::initAppLibHelper(CCDictionary *params){
    //主城等级统计
    AppLibHelper::triggerEventMainBuildingLevel(FunBuildController::getInstance()->getMainCityLv());
    AppLibHelper::updatePlatformScore(FunBuildController::getInstance()->getMainCityLv());
}

void CCCommonUtils::initGeneral(CCDictionary *params){
    //将军信息初始化
    CCArray *generalArr = NULL;
    generalArr = dynamic_cast<CCArray*>(params->objectForKey("userGenerals"));
    auto &generals = GlobalData::shared()->generals;
    if (generalArr != NULL && generals.empty()) {
        std::map<std::string, SkillCDInfo*>::iterator cdIt;
        for(cdIt = GeneralManager::getInstance()->SkillCDMap.begin(); cdIt != GeneralManager::getInstance()->SkillCDMap.end(); cdIt++){
            cdIt->second->release();
        }
        GeneralManager::getInstance()->SkillCDMap.clear();
        int length = generalArr->count();
        CCDictionary *generalItem = NULL;
        for (int i=0; i<length; i++) {
            generalItem = _dict(generalArr->objectAtIndex(i));
            GeneralManager::getInstance()->addGeneral(generalItem);
            if(i==0 && generalItem->objectForKey("skill")){
                GeneralInfo* info = &GlobalData::shared()->generals.begin()->second;
                info->parseCDTime(generalItem);
            }
        }
        GeneralManager::getInstance()->resetGeneralSkillEffectValue();
    }
}

void CCCommonUtils::initAllianceSceice(CCDictionary *params){
    //联盟科技初始化
    CCArray *alScienceArr = dynamic_cast<CCArray*>(params->objectForKey("allianceScience"));
    ScienceController::getInstance()->initAllianceScienceData(alScienceArr);
}

void CCCommonUtils::initArmy(CCDictionary *params){
    //army初始化
    CCArray *armyArr = dynamic_cast<CCArray*>(params->objectForKey("army"));
    CCObject *armyObject;
    CCARRAY_FOREACH(armyArr, armyObject){
        CCDictionary *armyDic = _dict(armyObject);
        std::string itemId = armyDic->valueForKey("id")->getCString();
        int unLockLv = 0;
        if (GlobalData::shared()->armyList.find(itemId) != GlobalData::shared()->armyList.end()) {
            unLockLv = GlobalData::shared()->armyList[itemId].unlockLevel;
        }
        GlobalData::shared()->armyList[itemId] = ArmyInfo(armyDic);
        GlobalData::shared()->armyList[itemId].unlockLevel = unLockLv;
    }
}

void CCCommonUtils::initFort(CCDictionary *params){
    //fort 初始化
    CCArray *fortArr = dynamic_cast<CCArray*>(params->objectForKey("fort"));
    CCObject *fortObject;
    CCARRAY_FOREACH(fortArr, fortObject){
        CCDictionary *forDic = _dict(fortObject);
        std::string itemId = forDic->valueForKey("id")->getCString();
        int unLockLv = 0;
        if (GlobalData::shared()->fortList.find(itemId) != GlobalData::shared()->fortList.end()) {
            unLockLv = GlobalData::shared()->fortList[itemId].unlockLevel;
        }
        GlobalData::shared()->fortList[itemId] = ArmyInfo(forDic);
        GlobalData::shared()->fortList[itemId].isArmy = false;
        GlobalData::shared()->fortList[itemId].unlockLevel = unLockLv;
    }
}

void CCCommonUtils::initTreat(CCDictionary *params){
    //treat 初始化
    CCArray *treatArr = dynamic_cast<CCArray*>(params->objectForKey("hospital"));
    CCObject *treatObject;
    CCARRAY_FOREACH(treatArr, treatObject){
        CCDictionary *treatDic = _dict(treatObject);
        std::string itemId = treatDic->valueForKey("id")->getCString();
        GlobalData::shared()->treatList[itemId] = TreatInfo(treatDic);
    }
}

void CCCommonUtils::initExchange(CCDictionary *params){
    //兑换金币初始化goldExchangeList
    CCArray *goldExchangeArr = dynamic_cast<CCArray*>(params->objectForKey("exchange"));
    if(GlobalData::shared()->goldExchangeList.empty()){
        CCObject *obj;
        CCARRAY_FOREACH(goldExchangeArr, obj){
            CCDictionary* dict = _dict(obj);
            GoldExchangeItem* goldExchangeItem = new GoldExchangeItem(dict);
            GlobalData::shared()->goldExchangeList[goldExchangeItem->id] = goldExchangeItem;
        }
        GlobalData::shared()->goldExchageRefreshStamp = GlobalData::shared()->getWorldTime();
    }
}

void CCCommonUtils::initEquip(CCDictionary *params){
    //装备初始化
    if (params->objectForKey("equip")) {
        EquipmentController::getInstance()->retInitData(params);
    }
}

void CCCommonUtils::initItems(CCDictionary *params){
    //新商店和背包
    if (params->objectForKey("items")) {
        ToolController::getInstance()->initToolData(params);
    }
    //保存礼包道具奖励内容数据
    if (params->objectForKey("store")) {
        ToolController::getInstance()->initToolReward(params);
    }
}

void CCCommonUtils::initScience(CCDictionary *params){
    //science info init
    GlobalData::shared()->scienceInfo.initSelf();
    if (params->objectForKey("science")) {
        auto scienceInfo = dynamic_cast<CCArray*>(params->objectForKey("science"));
        GlobalData::shared()->scienceInfo.initScienceState(scienceInfo);
    }
}

void CCCommonUtils::initTalent(CCDictionary *params){
    //天赋数据
    if (params->objectForKey("talent")) {
        TalentController::getInstance()->initController(params);
    }
}

void CCCommonUtils::initGCMReward(CCDictionary *params){
    //初始化GCM奖励
    GCMRewardController::getInstance()->initGCMRewardData(params);
}

void CCCommonUtils::initNewPlayerReward(CCDictionary *params){
    if (params->objectForKey("newSignIn")) {
        CCDictionary* newSignIn = dynamic_cast<CCDictionary*>(params->objectForKey("newSignIn"));
        if (newSignIn) {
            PortActController::getInstance()->initNewPlayerDailyRwdData(newSignIn);
        }
    }
}

void CCCommonUtils::initTask(CCDictionary *params){
    CCArray* tasks = dynamic_cast<CCArray*>(params->objectForKey("task"));
    QuestController::getInstance()->initQuest(tasks);
    CCArray* ckfTask = dynamic_cast<CCArray*>(params->objectForKey("ckfTask"));
    QuestController::getInstance()->initCKFTask(ckfTask);
}

void CCCommonUtils::initServerStopTime(CCDictionary *params){
    //停服更新计时
    if(params->objectForKey("serverStoptime")){
        auto time = params->valueForKey("serverStoptime")->doubleValue();
        long curTime = GlobalData::shared()->getWorldTime();
        auto m_countDown = GlobalData::shared()->changeTime(time/1000);
        auto m_time = (long)floor( m_countDown - curTime );
        string delayTime = CC_ITOA(m_time/60);
        string tip = CCString::createWithFormat("%s",_lang_1("105324", delayTime.c_str()))->getCString();
        CCCommonUtils::flySystemUpdateHint(time, true,tip,FLY_HINT_LOGIN);
    }
}

void CCCommonUtils::initMixInfo(CCDictionary *params){
    CCDictionary* mixedInfo = dynamic_cast<CCDictionary*>(params->objectForKey("mixedInfo"));
    GlobalData::shared()->isOpenEvaluate = false;
    GlobalData::shared()->popupTimes = 7;
    if (mixedInfo)
    {
        GlobalData::shared()->isOpenEvaluate = mixedInfo->valueForKey("scoreSwitch")->boolValue();
        if (mixedInfo->objectForKey("popupTimes"))
        {
            GlobalData::shared()->popupTimes = mixedInfo->valueForKey("popupTimes")->intValue();
        }
    }
}

void CCCommonUtils::initFBShare(CCDictionary *params){
    CCDictionary* fbShare = _dict(params->objectForKey("fbShare"));
    if (params->objectForKey("fbShare")) {
        CCArray* shareArr = dynamic_cast<CCArray*>(params->objectForKey("fbShare"));
        if (shareArr) {
            int sNum = shareArr->count();
            for (int i=0; i<sNum; i++) {
                auto oneDic = _dict(shareArr->objectAtIndex(i));
                oneDic->retain();
                string idkey = oneDic->valueForKey("id")->getCString();
                GlobalData::shared()->shareFbmap[idkey] = oneDic;
            }
        }
    }
}

void CCCommonUtils::getGameDataAfterInit(){
    if(isServerCrossOpen()){
        WorldController::getInstance()->getServerList();
    }
    WorldActivityGetCommand *cmd = new WorldActivityGetCommand();
    cmd->sendAndRelease();
    
    ShowStatusItemCommand *statusCmd = new ShowStatusItemCommand();
    statusCmd->sendAndRelease();
    
    AchievementController::getInstance()->purgeData();
    AchievementController::getInstance()->getDataFromServer();
    AchievementController::getInstance()->getMedalDataFormServer();
    
    DailyActiveController::shared()->getDataFormServer();
    
    
    ChatController::getInstance()->setDisableTranslateLang();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::setGameLanguage();
    ChatServiceCocos2dx::setAutoTranlateEnable();
    if(ChatServiceCocos2dx::enableNativeChat){
        string param = ChatServiceCocos2dx::getChannelInfo();
        ChatController::getInstance()->getNewMsg(param);
    }
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
//结构    "id|seqid|channelType,id|seqid|channelType,id|seqid|channelType"
    
    string param = ChatServiceCocos2dx::getChannelInfo();
//     string param = "0|0|4";
    ChatController::getInstance()->getNewMailMsg(param);
#endif
}

void CCCommonUtils::doWhenInitComplete(CCDictionary *params){
    if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if (GlobalData::shared()->isGoogle()) {
            PayController::getInstance()->initGoogle();
        }
#endif
        if (params->objectForKey("first_login")) {
            AppLibHelper::triggerEventCompletedRegistration();
        }
        if (params->objectForKey("two_days_login")) {
            AppLibHelper::sendAdjustTrack("two_days_login");
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_INIT_FINISH);
    }
    else {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
    }
    GlobalData::shared()->isInitFlag = true;
    GlobalData::shared()->isInitFinish = true;
    GlobalData::shared()->isInDataParsing = false;
    
    std::string device = CCUserDefault::sharedUserDefault()->getStringForKey("phoneDevice", "");
    if(device != "" && device != GlobalData::shared()->phoneDevice){
        PhoneDeviceCommand *cmd = new PhoneDeviceCommand(device);
        cmd->sendAndRelease();
        CCLog("%s", ("phoneDevice = " + device).c_str());
    }
}

void CCCommonUtils::initTestSwitches(CCDictionary *params)
{
    auto arr = dynamic_cast<CCArray*>(params->objectForKey("switches"));
    if( arr==NULL ){
        return;
    }
    
    GlobalData::shared()->testSwitchMap.clear();
    CCDictionary *item = NULL;
    for (int i=0; i<arr->count(); i++) {
        item = _dict(arr->objectAtIndex(i));
        string name = item->valueForKey("name")->getCString();
        int stat = item->valueForKey("stat")->intValue();
        GlobalData::shared()->testSwitchMap[name] = stat;
    }
}

void CCCommonUtils::initAssets(CCDictionary *params)
{
    auto assets = dynamic_cast<CCDictionary*>(params->objectForKey("assets"));
    if( assets==NULL ){
        return;
    }
    long winPoint = (long)(assets->valueForKey("winPoint")->doubleValue());
    GlobalData::shared()->playerInfo.winPoint = winPoint;
}
void CCCommonUtils::initTitan(CCDictionary *params)
{
    auto titan = dynamic_cast<CCDictionary*>(params->objectForKey("titan"));
    if( titan==NULL ){
        return;
    }
     //fusheng 泰坦
    //GlobalData::shared()->titanInfo.resetTitanInfo(titan) ;
    TitanController::getInstance()->parse(titan);
    
}

void CCCommonUtils::initOperation(int initType, CCDictionary *params){
    if(initType == InitStart || initType == InitEnd){
        return;
    }
    auto start = clock();
    if(initType == InitTime){
        initTime(params);
    }
    else if(initType == InitEffect){
        initEffect(params);
    }
    else if(initType == InitTranslation){
        initTranslation(params);
    }
    else if(initType == InitGaid){
        initGaid(params);
    }
    else if(initType == InitEffectState){
        initEffectState(params);
    }
    else if(initType == InitStatus){
        initStatus(params);
    }
    else if(initType == InitWorld){
        initWorld(params);
    }
    else if(initType == InitPray){
        initPray(params);
    }
    else if(initType == InitLord){
        initLord(params);
    }
    else if(initType == InitPushSetting){
        initPushSetting(params);
    }
    else if(initType == InitBusinessMan){
        initBusinessMan(params);
    }
    else if(initType == InitMonthLyCards){
        initMonthLyCards(params);
    }
    else if(initType == InitPlayerInfo){
        initPlayerInfo(params);
    }
    else if(initType == InitMails){
        initMails(params);
    }
    else if(initType == InitTrain){
        initTrain(params);
    }
    else if(initType == InitResource){
        initResource(params);
    }
    else if(initType == InitEnemyInfo){
        initEnemyInfo(params);
    }
    else if(initType == InitConfigData){
        initConfigData(params);
    }
    else if(initType == InitAffair){
        initAffair(params);
    }
    else if(initType == InitWall){
        initWall(params);
    }
    else if(initType == InitActivity){
        initActivity(params);
    }
    else if(initType == InitQueue){
        initQueue(params);
    }
    else if(initType == InitBuilding){
        initBuilding(params);
    }
    else if(initType == InitDragonBuilding){
        initDragonBuilding(params);
    }
    else if(initType == InitDragonsInfo){
        initDragonsInfo(params);
    }
    else if(initType == InitAppLibHelper){
        initAppLibHelper(params);
    }
    else if(initType == InitGeneral){
        initGeneral(params);
    }
    else if(initType == InitAllianceSceice){
        initAllianceSceice(params);
    }
    else if(initType == InitArmy){
        initArmy(params);
    }
    else if(initType == InitFort){
        initFort(params);
    }
    else if(initType == InitTreat){
        initTreat(params);
    }
    else if(initType == InitExchange){
        initExchange(params);
    }
    else if(initType == InitEquip){
        initEquip(params);
    }
    else if(initType == InitItems){
        initItems(params);
    }
    else if(initType == InitScience){
        initScience(params);
    }
    else if(initType == InitTalent){
        initTalent(params);
    }
    else if(initType == InitGCMReward){
        initGCMReward(params);
    }
    else if(initType == InitNewPlayerReward){
        initNewPlayerReward(params);
    }
    else if(initType == InitTask){
        initTask(params);
    }
    else if(initType == InitServerStopTime){
        initServerStopTime(params);
    }
    else if(initType == InitMixInfo){
        initMixInfo(params);
    }
    else if(initType == InitFBShare){
        initFBShare(params);
    }
    else if(initType == DoWhenInitComplete){
        doWhenInitComplete(params);
    }
    else if(initType == GetGameDataAfterInit){
        getGameDataAfterInit();
    }
    else if(initType == InitOpenUUID){
        refreshOpenUUID(params);
    }
    else if(initType == InitTestSwitches){
        initTestSwitches(params);
    }else if(initType == InitAssets){
        initAssets(params);
    }else if(initType == InitTitan){
        initTitan(params);
    }
}

void CCCommonUtils::refreshOpenUUID(CCDictionary *params){
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    if(params->objectForKey("updateDeviceId")){
        cocos2d::extension::CCDevice::getOpenUDID();
        CCLOG("deviceId update");
    }
#endif
}

void CCCommonUtils::parseData(cocos2d::CCDictionary *params){
    resetData(params);
    int i = InitStart;
    while(i < InitEnd){
        initOperation(i, params);
        i++;
    }
    
//    initTime(params);
//    initEffect(params);
//    initTranslation(params);
//    initGaid(params);
//    initEffectState(params);
//    initStatus(params);
//    initWorld(params);
//    initPray(params);
//    initLord(params);
//    initPushSetting(params);
//    initBusinessMan(params);
//    initMonthLyCards(params);
//    initPlayerInfo(params);
//    initMails(params);
//    initTrain(params);
//    initResource(params);
//    initEnemyInfo(params);
//    initConfigData(params);
//    initAffair(params);
//    initWall(params);
//    initActivity(params);
//    initQueue(params);
//    initBuilding(params);
//    initAppLibHelper(params);
//    initGeneral(params);
//    initAllianceSceice(params);
//    initArmy(params);
//    initFort(params);
//    initTreat(params);
//    initExchange(params);
//    initEquip(params);
//    initItems(params);
//    initScience(params);
//    initTalent(params);
//    initGCMReward(params);
//    initTask(params);
//    initServerStopTime(params);
//    initMixInfo(params);
//    initFBShare(params);
//
//    // to notice the push init finished , this must be the last call
//    // 以下代码必须在最后执行，所有新加的都加在上面去
//    doWhenInitComplete(params);
//    
//    getGameDataAfterInit();
}

int CCCommonUtils::getBindType(std::string bindStr){
    if(bindStr == BIND_QQ){
        return QQ;
    }else if(bindStr == BIND_WEIBO){
        return WEIBO;
    }else if(bindStr == BIND_WEIXIN){
        return WEIXIN;
    }else if(bindStr == BIND_VK){
        return VK;
    }
    return -1;
}

bool CCCommonUtils::payParseData(cocos2d::CCDictionary *params)
{
    int status = params->valueForKey("status")->intValue();
    string orderId = params->valueForKey("orderId")->getCString();
    CCLOG("payment check callback,status is %d",status);
    CCLOG("payment check callback,order id is %s",orderId.c_str());
    switch (status) {
        case 0:{
            PayController::getInstance()->addToPurchaseIdList(orderId);
            // pay check success
            // update gold
            bool isFirstPay = false;
            int intPayTotal = GlobalData::shared()->playerInfo.payTotal;
            UIComponent::getInstance()->updateGold(params->valueForKey("gold")->intValue());
            GlobalData::shared()->playerInfo.payTotal = params->valueForKey("payTotal")->intValue();
            if(intPayTotal<=0&&GlobalData::shared()->playerInfo.payTotal>0){
                isFirstPay = true;
            }
            
            string key = params->valueForKey("itemId")->getCString();
            string itemName = "";
            bool sendGift = false;
            if(params->objectForKey("exchangegift") && params->valueForKey("exchangegift")->boolValue() == true){
                sendGift = true;
            }
            
            if ( GlobalData::shared()->goldExchangeList.find(key.c_str()) != GlobalData::shared()->goldExchangeList.end()) {
                ToolController::getInstance()->m_canBuy = 0;
                map<string, GoldExchangeItem*>::iterator it =GlobalData::shared()->goldExchangeList.find(key.c_str());
                itemName = _lang(it->second->name);
                if(sendGift==false){
                    if(it->second->type == "5"){
                        it->second->bought = true;
                        if(GlobalData::shared()->monthCardInfoList.find(key)!=GlobalData::shared()->monthCardInfoList.end()){
                                GlobalData::shared()->monthCardInfoList[key]->buyCard();
                        }
                    }else if(it->second->type!="1"){
                        it->second->bought = true;
                        PayController::getInstance()->setGoldExchangeSaleBoughtFlag(key, true);
                    }
                    
                }
                if (params->objectForKey("reward")) {
                    CCArray* arr = dynamic_cast<CCArray*>(params->objectForKey("reward"));
                    if(arr){
                        PopupViewController::getInstance()->removeAllPopupView();
                        PopupViewController::getInstance()->addPopupView(FreeListView::create(atoi(key.c_str())));
                        RewardController::getInstance()->retReward(arr, true);
                    }else if (isFirstPay){
                        PopupViewController::getInstance()->removeAllPopupView();
                    }
                }else if (isFirstPay){
                    if(sendGift==false){
                        PopupViewController::getInstance()->removeAllPopupView();
                    }
                }
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(PAYMENT_COMMAND_RETURN, CCString::create(key));
                if (params->objectForKey("exchange")) {
                    
                    CCDictionary* dict = _dict(params->objectForKey("exchange"));
                    GoldExchangeItem* goldExchangeItem = new GoldExchangeItem(dict);
                    map<string, GoldExchangeItem*>::iterator it =GlobalData::shared()->goldExchangeList.find(goldExchangeItem->id);
                    if ( it!= GlobalData::shared()->goldExchangeList.end()){
                        GoldExchangeItem* goldExchangeItemtemp =it->second;
                        delete goldExchangeItemtemp;
                        goldExchangeItemtemp = NULL;
                    }
                    GlobalData::shared()->goldExchangeList[goldExchangeItem->id] = goldExchangeItem;
                    
                    
                }
                if(params->objectForKey("equip")){
                    CCArray *equipArray = dynamic_cast<CCArray*>(params->objectForKey("equip"));
                    if(equipArray){
                        EquipmentController::getInstance()->addEquip(equipArray);
                    }
                }
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GOLDEXCHANGE_LIST_CHANGE);
                
            }
            
            // send log to AF
            AppLibHelper::sendPayLog(params->valueForKey("cost")->getCString(),key);
            AppLibHelper::triggerEventPurchase(params->valueForKey("cost")->getCString(),key);
            if (params->objectForKey("first_pay")) {
                AppLibHelper::sendAdjustTrack("first_pay");
            }
            if (params->objectForKey("first_pay_today")) {
                AppLibHelper::sendAdjustTrack("first_pay_today");
            }
            if (params->objectForKey("repay_add")){
                int addValue = params->valueForKey("repay_add")->intValue();
                RepayController::getInstance()->addPoint(addValue);
            }
            if (sendGift==true){
                if(!itemName.empty() && params->objectForKey("exchangeto")){
                    auto exchangeTo = _dict(params->objectForKey("exchangeto"));
                    if(exchangeTo){
                        if(exchangeTo->objectForKey("receiverName")){
                            string content = _lang_1("101005",itemName.c_str());
                            string toName = exchangeTo->valueForKey("receiverName")->getCString();
                            MailController::getInstance()->sendMail(toName, "", content,"","",false,MAIL_SELF_SEND,"",false);
                        }
                        
                        if(exchangeTo->objectForKey("sendTime") && exchangeTo->valueForKey("sendTime")->doubleValue()>0){
                            if(exchangeTo->objectForKey("receiverId")){
                                LuaController::getInstance()->updateExchangeGiftInfo(exchangeTo->valueForKey("receiverId")->getCString(),key,exchangeTo->valueForKey("sendTime")->doubleValue()/1000);
                                
                            }
                        }
                    }
                }
                LuaController::toSelectUser(key, false, 0);
                YesNoDialog::showYesDialog(_lang("101004").c_str());
//                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFRESH_INVITE_DATA);
            }else{
                if (!params->objectForKey("google_code")) {
                    YesNoDialog::showYesDialog(_lang("E100076").c_str());
                }
            }
        }
            break;
        case 1:{
            PayController::getInstance()->removeOrderCache(orderId);
            // pay check fail
            YesNoDialog::showYesDialog(_lang("E100042").c_str());
        }
            break;
        case 2:{
            PayController::getInstance()->addToPurchaseIdList(orderId);
            // order exist
        }
            break;
            
        default:
            CCLOG("Payment net cmd error: error status :%d",status);
            return false;
    }
    return true;
}

bool CCCommonUtils::isServerCrossOpen(){
    return true;
}
bool CCCommonUtils::isPushNotify(int type){
    vector<PushSettingInfo*>::iterator it;
    for (it = GlobalData::shared()->pushSettingVector.begin(); it!=GlobalData::shared()->pushSettingVector.end(); it++) {
        if((*it)->type==type){
            if((*it)->status==0){
                return false;
            }
        }
    }

    return true;
}

std::string CCCommonUtils::getLanguage()
{
    
    std::string preferredLanguage = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_PREFERRED_LANGUAGE,"");
    if(preferredLanguage==""){
        preferredLanguage = LocalController::shared()->getLanguageFileNameBasedOnUserSystem();
    }
    return preferredLanguage;
}

bool CCCommonUtils::hasSpeedEffect(int type){
    map<int, CCDictionary* >::iterator it = ToolController::getInstance()->m_statusItems.find(type);
    double time = 0;
    if(it!=ToolController::getInstance()->m_statusItems.end()){
        time =(it->second->valueForKey("endTime")->doubleValue() - WorldController::getInstance()->getTime())/1000;
    }
    
    return time > 0;
}

double CCCommonUtils::getSpeedEffectTime(int type){
    map<int, CCDictionary* >::iterator it = ToolController::getInstance()->m_statusItems.find(type);
    double speedTime = 0;
    if(it!=ToolController::getInstance()->m_statusItems.end()){
        speedTime =(it->second->valueForKey("endTime")->doubleValue() - WorldController::getInstance()->getTime())/1000;
    }
    return speedTime;
}

//根据用户头像名称获取加载哪个大图
int CCCommonUtils::getBustIdxByName(string name)
{
    if (name=="g008" || name=="g015" || name=="g024" || name=="g026") {
        return 1;
    }else if (name=="g032" || name=="g038" || name=="g041" || name=="g044") {
        return 2;
    }else if (name=="g007" || name=="g012" || name=="g045" || name=="g046") {
        return 3;
    }
    return 1;
}

std::string CCCommonUtils::getLanguageFNByLocalSN(std::string localN){
    string langId = "";
    CCDictionary* dic_all = LocalController::shared()->shared()->DBXMLManager()->getGroupByKey("language");
    int idx = 9500;
    int num = dic_all->count();
    if(localN.find("zh_CN")<localN.length()||localN.find("zh-Hans")<localN.length() || localN.find("zh_TW")<localN.length() || localN.find("zh-Hant")<localN.length()){
        localN = "zh";
    }
    for (int i=0; i<num; ++i) {
        CCDictionary* dic_one = _dict(dic_all->objectForKey(CC_ITOA(idx+i)));
        string mark = dic_one->valueForKey("mark")->getCString();
        string lang_ma = dic_one->valueForKey("lang_ma")->getCString();
        string lang_id = dic_one->valueForKey("lang_id")->getCString();
        if(mark!="" && lang_ma!="" && localN.find(lang_ma)<localN.length()){
            langId = lang_id;
            break;
        }
    }
    if(langId==""){
        langId = "115601";
    }
    return _lang(langId);
}

CCSprite *CCCommonUtils::addFilterSprite(std::string fileName, const ccColor3B &color, float bright, float contrast, float saturation){
    auto sprite = FilterSprite::createWithSpriteFrameName(fileName);
    sprite->setParam(color, bright, contrast, saturation);
    return sprite;
}

ccColor3B CCCommonUtils::covertHSBToRGB(int h, float s, float v){
    float r = 0, g = 0, b = 0;
    int i = (int) ((h / 60) % 6);
    float f = (h * 1.0 / 60) - i;
    float p = v * (1.0 - s);
    float q = v * (1.0 - f * s);
    float t = v * (1.0 - (1.0 - f) * s);
    switch (i) {
        case 0:
            r = v;
            g = t;
            b = p;
            break;
        case 1:
            r = q;
            g = v;
            b = p;
            break;
        case 2:
            r = p;
            g = v;
            b = t;
            break;
        case 3:
            r = p;
            g = q;
            b = v;
            break;
        case 4:
            r = t;
            g = p;
            b = v;
            break;
        case 5:
            r = v;
            g = p;
            b = q;
            break;
        default:
            break;  
    }
    int resultR = floor(r * 255.0);
    int resultG = floor(g * 255.0);
    int resultB = floor(b * 255.0);
    return ccc3(resultR, resultG, resultB);
}

CCSprite *CCCommonUtils::getItemBGByColor(int color){
    int hValue = 0;
    CCSprite *sprite = NULL;
    if(color == WHITE){
        sprite = CCLoadSprite::createSprite(CCString::createWithFormat("tool_%d.png", 1)->getCString());
    }else if(color == GREEN){
        hValue = 137;
        sprite = CCCommonUtils::addFilterSprite("tool_1.png", CCCommonUtils::covertHSBToRGB(hValue, 0.7, 1.0), 0.02, 1.06, 0.94);
    }else if(color == BLUE){
        hValue = 201;
        sprite = CCCommonUtils::addFilterSprite("tool_1.png", CCCommonUtils::covertHSBToRGB(hValue, 0.81, 1.0), 0.02, 1.06, 2.45);
    }else if(color == PURPLE){
        hValue = 293;
        sprite = CCCommonUtils::addFilterSprite("tool_1.png", CCCommonUtils::covertHSBToRGB(hValue, 0.61, 1.0), 0.02, 1.04, 0.94);
    }else if(color == ORANGE){
        hValue = 27;
        sprite = CCCommonUtils::addFilterSprite("tool_1.png", CCCommonUtils::covertHSBToRGB(hValue, 0.7, 1.0), 0.06, 1.06, 0.94);
    }else if(color == GOLDEN){
        hValue = 48;
        sprite = CCCommonUtils::addFilterSprite("tool_1.png", CCCommonUtils::covertHSBToRGB(hValue, 0.75, 1.0), 0.02, 1.24, 1.64);
    }else{
        sprite = CCLoadSprite::createSprite("no_iconFlag.png");
    }
    return sprite;
}

void CCCommonUtils::changeAllianceFlagBGByRank(CCSprite *sprite, int index, bool isGray){
    if(sprite->getParent() == NULL){
        return;
    }
    auto parent = sprite->getParent();
    CCSprite *newSprite = NULL;
    if(isGray){
        if(index == 5){
            newSprite = CCCommonUtils::addFilterSprite("Alliance_Flag_Gre.png", CCCommonUtils::covertHSBToRGB(0, 0, 1.0), 0.04, 1.01, 0);
        }else{
            newSprite = CCLoadSprite::createSprite("Alliance_Flag_Gre.png");
        }
    }else{
        if(index == 4){
            newSprite = CCLoadSprite::createSprite("Alliance_Flag_04.png");
        }else if(index == 5){
            newSprite = CCLoadSprite::createSprite("Alliance_Flag_05.png");
        }else{
            if(index == 1){
                newSprite = CCCommonUtils::addFilterSprite("Alliance_Flag_Gre.png", CCCommonUtils::covertHSBToRGB(48, 1, 1.0), 0, 1, 3.12);
            }else if(index == 2){
                newSprite = CCCommonUtils::addFilterSprite("Alliance_Flag_Gre.png", CCCommonUtils::covertHSBToRGB(76, 1, 1.0), 0, 1, 3.12);
            }else{
                newSprite = CCCommonUtils::addFilterSprite("Alliance_Flag_Gre.png", CCCommonUtils::covertHSBToRGB(197, 1, 1.0), 0, 1, 3.12);
            }
        }
    }
    if(newSprite){
        sprite = newSprite;
        parent->addChild(newSprite);
    }
}

CCArray* CCCommonUtils::getTypeArray(int type){
    int city_lv = FunBuildController::getInstance()->getMainCityLv();
    auto goodDic = LocalController::shared()->DBXMLManager()->getGroupByKey("goods");
    CCArray* array = new CCArray();
    array->init();
    int num = 0;
    if(goodDic)
    {
        CCDictElement* element;
        CCDICT_FOREACH(goodDic, element)
        {
            CCDictionary* dictInfo = _dict(element->getObject());
            if(type==dictInfo->valueForKey("type2")->intValue() && dictInfo->valueForKey("type")->intValue()==4 && city_lv>=dictInfo->valueForKey("lv")->intValue()){
                array->addObject(dictInfo);
            }
        }
    }
    num = array->count();
    for (int i=0; i<num; i++) {
        for (int j=i; j<num; j++) {
            CCDictionary* dict1 = _dict(array->objectAtIndex(i));
            CCDictionary* dict2 = _dict(array->objectAtIndex(j));
            if(dict1->valueForKey("order_num")->intValue()<dict2->valueForKey("order_num")->intValue()){
                array->swap(i, j);
            }
        }
    }
    return array;
}

void CCCommonUtils::onUploadPhoto(string uid, int srcCode, int idx)
{
    int idd = atoi(uid.c_str());
    cocos2d::extension::CCDevice::onUploadPhoto(idd, srcCode, idx);
}

string CCCommonUtils::getCustomPicUrl(string uid, int picVer)
{
    if (uid.length() == 0)
    {
        return "";
    }
    //http://cok.eleximg.com/cok/img/710001/bd65ca6d7d40cf95a3fe6b7fbb5cef7c.jpg
    string url = "http://cok.eleximg.com/cok/img/";
    
    string md5Str = uid + "_" + CC_ITOA(picVer);
    MD5 md5;
    md5.update(md5Str.c_str(), md5Str.length());
    
    string tempStr = uid;
    url.append(tempStr.substr(tempStr.length()-6,tempStr.length()));
    url.append("/");
    url.append(md5.toString().c_str()).append(".jpg");
    
    return url;
}
string CCCommonUtils::getShakePicUrl(string picName){
    string num = string("000") + picName.substr(4,3);
    
    string url = "http://cok.eleximg.com/cok/img/";
    MD5 md5;
    md5.update(picName.c_str(), picName.length());
    
    url = url + num;
    url.append("/");
    url.append(md5.toString().c_str()).append(".jpg");
    return url;
}

bool CCCommonUtils::isUseCustomPic(int picVer)
{
    if (UploadImageController::shared()->getUploadImageFlag() == 0)
    {
        return false;
    }
    if (picVer>0 && picVer<1000000)
    {
        return true;
    }
    return false;
}

//字符串替换函数
std::string& CCCommonUtils::StringReplace(std::string& strBig, const std::string& strsrc, const std::string& strdst)
{
    std::string::size_type pos = 0;
    std::string::size_type srclen = strsrc.size();
    std::string::size_type dstlen = strdst.size();
    while( (pos = strBig.find(strsrc, pos)) != std::string::npos)
    {
        strBig.replace(pos, srclen, strdst);
        pos += dstlen;
    }
    return strBig;
}

bool CCCommonUtils::isTestPlatformAndServer(string key)
{
    bool ret = false;
//    string zone = NetController::shared()->getZone();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if ( GlobalData::shared()->testSwitchMap.find(key) != GlobalData::shared()->testSwitchMap.end() )
    {
        if (GlobalData::shared()->testSwitchMap[key] == 1) {
            ret = true;
        }
    }
//#endif
    return ret;
}

//通用hs的上传
cocos2d::CCDictionary* CCCommonUtils::getHelpShiftDefaultMeta(){
    cocos2d::CCDictionary *meta = new CCDictionary();
    meta->setObject(new CCString(cocos2d::extension::CCDevice::getVersionName()), "VersionName");
    meta->setObject(new CCString(cocos2d::extension::CCDevice::getVersionCode()), "VersionCode");
    return meta;
}

//通用hs的tag
cocos2d::CCArray* CCCommonUtils::getHelpShiftDefaultTags(){
    cocos2d::CCArray *tags = new cocos2d::CCArray();
    tags->init();
    if(GlobalData::shared()->playerInfo.vipEndTime>GlobalData::shared()->getWorldTime())
        tags->addObject(CCString::createWithFormat("VIP %d",VipUtil::getVipLevel(GlobalData::shared()->playerInfo.vipPoints)));
    if(GlobalData::shared()->playerInfo.payTotal>6000000)//总充值金币数大于600万
        tags->addObject(new CCString("whale"));
    else if(GlobalData::shared()->playerInfo.payTotal>0)
        tags->addObject(new CCString("paid"));
    tags->addObject(CCString::createWithFormat("s%d",GlobalData::shared()->playerInfo.selfServerId));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    tags->addObject(new CCString("ios"));
#else
    if(GlobalData::shared()->isXiaoMiPlatForm())
        tags->addObject(new CCString("mi"));
    else if(GlobalData::shared()->analyticID != "market_global")
        tags->addObject(new CCString(GlobalData::shared()->analyticID));
    else
        tags->addObject(new CCString("google"));
#endif
    return tags;
}

void CCCommonUtils::showBanHelpShift(){
    cocos2d::CCDictionary *config = new cocos2d::CCDictionary();
    cocos2d::CCDictionary *meta = getHelpShiftDefaultMeta();
    cocos2d::CCArray *tags = getHelpShiftDefaultTags();
    tags->addObject(new CCString("ban"));
    meta->setObject(tags, HS_TAGS_KEY);
    config->setObject(meta, HS_META_DATA_KEY);
    GlobalData::shared()->isBind = true;
    HelpshiftCocos2dx::showConversation(config);
}

void CCCommonUtils::showHelpShiftFAQ()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    GameController::getInstance()->platformCollectUserInfo(GlobalData::shared()->playerInfo.uid,GlobalData::shared()->playerInfo.name,CC_ITOA(GlobalData::shared()->playerInfo.level));
#else
    HelpshiftCocos2dx::setUserIdentifier(GlobalData::shared()->playerInfo.uid.c_str());
    HelpshiftCocos2dx::setNameAndEmail(GlobalData::shared()->playerInfo.name.c_str(),"");
#endif
    cocos2d::CCDictionary *config = new cocos2d::CCDictionary();
    cocos2d::CCDictionary *meta = getHelpShiftDefaultMeta();
    cocos2d::CCArray *tags = getHelpShiftDefaultTags();
    meta->setObject(tags, HS_TAGS_KEY);
    config->setObject(meta, HS_META_DATA_KEY);
    GlobalData::shared()->isBind = true;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    GlobalData::shared()->isBind = false;
#endif
    
    HelpshiftCocos2dx::showFAQs(config);
}

void CCCommonUtils::showHelpShiftFAQ(string itemId)
{
    string key = CCCommonUtils::getPropById(itemId, "help_id");
    if (key.length() == 0)
    {
        showHelpShiftFAQ();
        return;
    }
    HelpshiftCocos2dx::showFAQSection(key.c_str());
}

void CCCommonUtils::showHelpShiftSingleFAQ(string itemId)
{
    string key = CCCommonUtils::getPropById(itemId, "help_id");
    if (key.length() == 0) {
        showHelpShiftFAQ();
        return;
    }
    HelpshiftCocos2dx::showSingleFAQ(key.c_str());
}

bool CCCommonUtils::checkTaiWanFlag(){
    bool flag = false;//true为关
    if(GlobalData::shared()->playerInfo.regCountry=="CN" && !GlobalData::shared()->isOpenTaiwanFlag && GlobalData::shared()->playerInfo.gmFlag!=1){
        flag = true;
    }
    return flag;
}

std::string CCCommonUtils::changeChinaFlag(string flag){
    string banner = flag;
    std::string language = LocalController::shared()->getLanguageFileName();
    if(flag=="CN" && GlobalData::shared()->playerInfo.regCountry=="CN" && language == "zh_CN" && GlobalData::shared()->chinaSwitchFlag==1 && GlobalData::shared()->playerInfo.gmFlag!=1){
        banner = "CN_2";
    }
    return banner;
}

bool CCCommonUtils::isShowFlagForChinese(){
    bool flag = true;
    std::string language = LocalController::shared()->getLanguageFileName();
    if(GlobalData::shared()->playerInfo.regCountry=="CN" && language == "zh_CN" && GlobalData::shared()->chinaSwitchFlag==2 && GlobalData::shared()->playerInfo.gmFlag!=1){
        flag = false;
    }
    return flag;
}

bool CCCommonUtils::checkVersion(string version){
    bool flag = false;
    std::vector<std::string> _updateVersion;
    CCCommonUtils::splitString(version,".", _updateVersion);
    //当前app版本
    std::string _appVersion = cocos2d::extension::CCDevice::getVersionName();
    std::vector<std::string> _appXMLVersion;
    CCCommonUtils::splitString(_appVersion,".", _appXMLVersion);
    //如果app的版本大于成功下载xml版本，使用apk内xml
    if (_updateVersion.size() == 3 && _appXMLVersion.size()==3) {
        if(_appXMLVersion[0] > _updateVersion[0]){
            flag = true;
        }else if(_appXMLVersion[0] == _updateVersion[0]){
            if (_appXMLVersion[1]>_updateVersion[1]) {
                flag = true;
            }else if(_appXMLVersion[1]==_updateVersion[1]){
                if(_appXMLVersion[2]>=_updateVersion[2]){
                    flag = true;
                }
            }
        }
    }
    return flag;
}