//
//  LuaController.cpp
//  IF
//
//  Created by Émilie.Jiang on 15/1/26.
//
//

#include "LuaController.h"
#include "cocosbuilder/lua_cocos2dx_cocosbuilder_manual.h"
#include "CCLuaEngine.h"
#include "PopupViewController.h"
#include "RechargeACTVCell.h"
#include "CCTypesExt.h"
#include "PayController.h"
#include "ParticleController.h"
#include "ToolController.h"
#include "GoldExchangeMoreListView.h"
#include "GoldExchangeAdvertisingView.h"
#include "ButtonLightEffect.h"
#include "AllianceInviteView.h"
#include "../Ext/CCDevice.h"

#include <curl/easy.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "../view/guiExt/md5.h"
#include <stdio.h>
#include "external/unzip/unzip.h"
#include "ActivityController.h"

#include "spine/SkeletonAnimation.h"
#include "lua_cocos2dx_LuaController.hpp"
#include "EquipmentController.h"
#include "LuaCommand.h"
#include "SceneController.h"
#include "ChatController.h"
#include "Utf8Utils.h"
#include "FlySystemUpdateHint.h"
#include "GuideController.h"
#include "ChatServiceCocos2dx.h"
#include "ActivityDetailView.h"
#include "YesNoDialog.h"
#include "UIComponent.h"
//#include "CCNotificationCenter.h"
#include "SoundController.h"
#include "RewardController.h"
#include "StoreView.h"
#include "TipsView.h"
#include "ChatServiceCocos2dx.h"

#define LUA_DOWNLOADED_VERSION "lua_downloaded_version"
#define LUA_CLIENT_VERSION "lua_client_version"
#define LUA_ZIP "luaTmp.zip"
#define LUA_BUFFER_SIZE    8192
#define LUA_MAX_FILENAME   512
#define LUA_VERSION_FILE "luaVersion.bin"

#define CHAT_NOTICE_TAG 99999

static LuaController *_luainstance = NULL;
LuaController* LuaController::getInstance() {
    if (!_luainstance) {
        _luainstance = new LuaController();
    }
    return _luainstance;
}
LuaController::LuaController():m_advCellContainer(NULL),m_advCell(NULL),m_iconContainer(NULL),m_isAddPath(false),m_saleViewContainer(NULL),m_cmdDic(nullptr),m_clientVersion(""),m_serverVersion(""),m_downedVersion(""),m_isDownload(false),m_initLuaEngine(false),luazipSize(350),luaSize(500),luaMD5Code(""),luaVersion(""),m_initVersion(""),m_downloadType(LuaDownloadType_NONE),luapackMD5Code(""),m_packName(""),mChatNoticeNode(NULL),m_canTranslate(false),m_orgMsg("")/*,m_storageEnough(true),m_isCheckStorage(false)*/{
    m_downedVersion = CCUserDefault::sharedUserDefault()->getStringForKey(LUA_DOWNLOADED_VERSION);
    m_clientVersion = CCUserDefault::sharedUserDefault()->getStringForKey(LUA_CLIENT_VERSION);
    CCLog("lua___client_version: %s",m_clientVersion.c_str());
    CCLog("lua___download_version: %s",m_downedVersion.c_str());
    m_writeablePath = CCFileUtils::sharedFileUtils()->getWritablePath();
    m_initVersion = m_clientVersion;
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LuaController::ChatNoticeInView), MSG_POPUP_VIEW_IN, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LuaController::ChatNoticeInSceen), MSG_POPUP_VIEW_OUT, NULL);
    m_cmdDic = CCDictionary::create();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    cocos2d::extension::CCDevice::addSkipBackupAttributeToItemAtURL(m_writeablePath);
#endif


}
LuaController::~LuaController(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_POPUP_VIEW_IN);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_POPUP_VIEW_OUT);
}
bool LuaController::isLuaOpen(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(this->luaVersion.empty())
        return false;
    if(this->luaVersion.compare("0.0.0") == 0){
        return false;
    }
    return true;
#endif
    return false;
}
bool LuaController::initLua(){
    return false;
    if(!m_initLuaEngine){
        lua_State *lState = lua_open();
        if(!lState)
            return false;
        lua_close(lState);
        m_initLuaEngine=true;
        // register lua engine
        auto engine = LuaEngine::getInstance();
        ScriptEngineManager::getInstance()->setScriptEngine(engine);
        lua_State* L = engine->getLuaStack()->getLuaState();
        register_cocosbuilder_module(L);
        register_all_LuaController(L);
//        LuaEngine* pEngine = LuaEngine::getInstance();
//        CCScriptEngineManager::sharedManager()->setScriptEngine(pEngine);
//        // register lua ccb
//        tolua_extensions_ccb_open(pEngine->getLuaStack()->getLuaState());
    }
    return true;
}

bool LuaController::checkActLuaValid(ActivityEventObj* obj)
{
    if (m_isDownload == true) {
        return false;
    }
    if (isLuaOpen() == false) {
        return false;
    }
    if (obj->type != 6) {
        return false;
    }
    if(initLua()==false)
        return false;
    if(initMD5Info()==false || checkLuaFiles() == false){
        resetVersionAndUpdate();
        return false;
    }
    if (m_clientVersion.compare(luaVersion) != 0) {
        resetVersionAndUpdate();
        return false;
    }
    string path = getLuaPath();
    string id = obj->id;
    if (!CCFileUtils::sharedFileUtils()->isFileExist(path + "/ccbi/Activity" + id +"AdLuaCell.ccbi")) {
        resetVersionAndUpdate();
        return false;
    }
    if (!CCFileUtils::sharedFileUtils()->isFileExist(path + "/resources/activity_" + id + ".plist")) {
        resetVersionAndUpdate();
        return false;
    }
    if (!CCFileUtils::sharedFileUtils()->isFileExist(path + "/resources/activity_" + id + ".pvr.ccz")) {
        resetVersionAndUpdate();
        return false;
    }
    return true;
}
bool LuaController::showNoramlIcon(std::string popImg){
    return true; //a by ljf
    string file1 = CCFileUtils::sharedFileUtils()->getWritablePath() + "lua/ccbi/GoldExchange"+popImg+ "LuaView.ccbi";
    string file2 = CCFileUtils::sharedFileUtils()->getWritablePath() + "lua/ccbi/GoldExchange"+popImg+ "LuaIcon.ccbi";
    string bImage1 = CCFileUtils::sharedFileUtils()->getWritablePath() + "lua/resources/"+popImg + ".pvr.ccz";
    string bImagePlist1 = CCFileUtils::sharedFileUtils()->getWritablePath() + "lua/resources/"+popImg + ".plist";
    string bImage2 = CCFileUtils::sharedFileUtils()->getWritablePath() + "lua/resources/"+popImg+ "icon.pvr.ccz";
    string bImagePlist2 = CCFileUtils::sharedFileUtils()->getWritablePath() + "lua/resources/"+ popImg + "icon.plist";
    
    if(CCFileUtils::sharedFileUtils()->isFileExist(file1) && CCFileUtils::sharedFileUtils()->isFileExist(file2) && CCFileUtils::sharedFileUtils()->isFileExist(bImage1) && CCFileUtils::sharedFileUtils()->isFileExist(bImagePlist1)&& CCFileUtils::sharedFileUtils()->isFileExist(bImage2) && CCFileUtils::sharedFileUtils()->isFileExist(bImagePlist2)){
        return true;
    }
    return false;
}
bool LuaController::checkLuaValid(std::string popImg,std::string md5,std::string type){
    if(m_isDownload==true)
        return false;
    
    if(isLuaOpen()==false)
        return false;
    
//    //test code
//    md5 = "7217bc7bfb649dfe3aac788a1fe27a69";
    bool bTest = false;
    if(!bTest){
        if(popImg.empty() || md5.empty())
        return false;
    }
    if(popImg.compare("1") == 0 || popImg.compare("2")==0 || popImg.compare("new_recharge") == 0 || popImg.compare("month") == 0 ||popImg.compare("throne") == 0 || popImg.compare("equip") == 0)
        return false;
    if(initLua()==false)
        return false;
    if(initMD5Info()==false || checkLuaFiles() == false){
        resetVersionAndUpdate();
        return false;
    }
    if(m_clientVersion.compare(this->luaVersion) != 0){
        checkUpdate();
        return false;
    }
    
    
    string path = getLuaPath();
    m_packName = popImg;
    this->luapackMD5Code = md5;
    if(!bTest){
        if(m_md5Map.find(popImg)==m_md5Map.end()){
            updateNewResource();
            return false;
        }
        if((*m_md5Map.find(popImg)).second.compare(md5) != 0){
            updateNewResource();
            return false;
        }
    }
    bool bCCB1 = CCFileUtils::sharedFileUtils()->isFileExist(path + "/ccbi/GoldExchange" + popImg + "LuaView.ccbi");
    if(!bCCB1){
        updateNewResource();
        return false;
    }
    bool bCCB2 = CCFileUtils::sharedFileUtils()->isFileExist(path + "/ccbi/GoldExchangeAdv" + popImg + "LuaCell.ccbi");
    if(!bCCB2){
        updateNewResource();
        return false;
    }
    if(type.compare("1")!=0){
        bool bCCB3 =CCFileUtils::sharedFileUtils()->isFileExist(path + "/ccbi/GoldExchangeLuaCell.ccbi");
        if(!bCCB3){
            updateNewResource();
            return false;
        }
        bool bCCB4 =CCFileUtils::sharedFileUtils()->isFileExist(path + "/ccbi/GoldExchangeLuaCell1.ccbi");
        if(!bCCB4){
            updateNewResource();
            return false;
        }
    }
    bool bCCB5 =CCFileUtils::sharedFileUtils()->isFileExist(path + "/ccbi/GoldExchange" + popImg + "LuaIcon.ccbi");
    if(!bCCB5){
        updateNewResource();
        return false;
    }
    bool bImage = CCFileUtils::sharedFileUtils()->isFileExist(path + "/resources/"+popImg + ".pvr.ccz");
    if(!bImage){
        updateNewResource();
        return false;
    }
    bool bImagePlist = CCFileUtils::sharedFileUtils()->isFileExist(path + "/resources/"+popImg + ".plist");
    if(!bImagePlist){
        updateNewResource();
        return false;
    }
//    if(type.compare("1")!=0){
        bool bImage1 = CCFileUtils::sharedFileUtils()->isFileExist(path + "/resources/"+popImg + "adv.pvr.ccz");
        if(!bImage1){
            updateNewResource();
            return false;
        }
        bool bImagePlist1 = CCFileUtils::sharedFileUtils()->isFileExist(path + "/resources/"+popImg + "adv.plist");
        if(!bImagePlist1){
            updateNewResource();
            return false;
        }
//    }
    bool bImage2 = CCFileUtils::sharedFileUtils()->isFileExist(path + "/resources/"+popImg + "icon.pvr.ccz");
    if(!bImage2){
        updateNewResource();
        return false;
    }
    bool bImagePlist2 = CCFileUtils::sharedFileUtils()->isFileExist(path + "/resources/"+popImg + "icon.plist");
    if(!bImagePlist2){
        updateNewResource();
        return false;
    }
    
    m_packName="";
    return true;
}
void LuaController::addLuaPath(){
    if(m_isAddPath==false){
        LuaEngine* pEngine = LuaEngine::getInstance();
        string path = getLuaPath();
        string filepath = getLuaFile();
        pEngine->addSearchPath(path.c_str());
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//            CCLOG(filepath.c_str());
//            CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
//            if (pstrFileContent){
//                pEngine->executeString(pstrFileContent->getCString());
//            }
//#else
//        filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
//        pEngine->executeScriptFile(filepath.c_str());
//#endif
//        lua_State* ls = pEngine->getLuaStack()->getLuaState();
//        lua_getglobal(ls, "addPackagePath");
//        lua_pushstring(ls, path.c_str());
//        lua_call(ls, 1, 0);
        m_isAddPath=true;
    }
}
std::string LuaController::getLuaPath(){
    string path = m_writeablePath+"lua";
    return path;
}
std::string LuaController::getLuaFile(){
    string path = getLuaPath();
    string filepath = path + "/main.lua";
    return filepath;
}

std::string LuaController::getActTips(std::string actId)
{
    addLuaPath();
    if ("" == actId) {
        return "";
    }
    LuaEngine* pEngine = LuaEngine::getInstance();
    string filepath = getLuaFile();
    string path = getLuaPath();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG(filepath.c_str());
    CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
    if (pstrFileContent){
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
#endif
    lua_State* ls = LuaEngine::defaultEngine()->getLuaStack()->getLuaState();
    lua_getglobal(ls, "getActTips");
    lua_pushstring(ls, actId.c_str());
    lua_call(ls, 1, 1);
    string ret = (string)lua_tostring(ls, -1);
    return ret;
}

std::string LuaController::getCollectLabel(std::string actId)
{
    addLuaPath();
    if ("" == actId) {
        return "";
    }
    LuaEngine* pEngine = LuaEngine::getInstance();
    string filepath = getLuaFile();
    string path = getLuaPath();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG(filepath.c_str());
    CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
    if (pstrFileContent){
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
#endif
    lua_State* ls = LuaEngine::defaultEngine()->getLuaStack()->getLuaState();
    lua_getglobal(ls, "getCollectLabel");
    lua_pushstring(ls, actId.c_str());
    lua_call(ls, 1, 1);
    string ret = (string)lua_tostring(ls, -1);
    return ret;
}

std::string LuaController::getBuyBtnLabel(std::string actId)
{
    addLuaPath();
    if ("" == actId) {
        return "";
    }
    LuaEngine* pEngine = LuaEngine::getInstance();
    string filepath = getLuaFile();
    string path = getLuaPath();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG(filepath.c_str());
    CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
    if (pstrFileContent){
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
#endif
    lua_State* ls = LuaEngine::defaultEngine()->getLuaStack()->getLuaState();
    lua_getglobal(ls, "getBuyBtnLabel");
    lua_pushstring(ls, actId.c_str());
    lua_call(ls, 1, 1);
    string ret = (string)lua_tostring(ls, -1);
    return ret;
}

std::string LuaController::getActRefTip(std::string actId)
{
    addLuaPath();
    if ("" == actId) {
        return "";
    }
    LuaEngine* pEngine = LuaEngine::getInstance();
    string filepath = getLuaFile();
    string path = getLuaPath();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG(filepath.c_str());
    CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
    if (pstrFileContent){
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
#endif
    lua_State* ls = LuaEngine::defaultEngine()->getLuaStack()->getLuaState();
    lua_getglobal(ls, "getActRefTip");
    lua_pushstring(ls, actId.c_str());
    lua_call(ls, 1, 1);
    string ret = (string)lua_tostring(ls, -1);
    return ret;
}

CCNode* LuaController::createActivityView(ActivityEventObj *obj)
{
    addLuaPath();
    if (!obj) {
        return NULL;
    }
    
    CCDictionary* dic = obj->toDic();
    LuaEngine* pEngine = LuaEngine::getInstance();
    string filepath = getLuaFile();
    string path = getLuaPath();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG(filepath.c_str());
    CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
    if (pstrFileContent){
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
#endif
    lua_State* ls = LuaEngine::defaultEngine()->getLuaStack()->getLuaState();
    lua_getglobal(ls, "createActivityView");
    lua_pushstring(ls, path.c_str());
    lua_newtable(ls);
    CCDictElement* ele = NULL;
    CCDICT_FOREACH(dic, ele)
    {
        if (ele && ele->getObject()) {
            CCString* value = dynamic_cast<CCString*>(ele->getObject());
            std::string key = ele->getStrKey();
            if (!key.empty() && value && value->getCString()) {
                lua_pushstring(ls, key.c_str());
                lua_pushstring(ls, value->getCString());
                lua_settable(ls, -3);
                CCLOG("notice --- key ---%s,value --- %s",key.c_str(),value->getCString());
            }
        }
    }
    lua_call(ls, 2, 1);
    CCNode* ret =  *(CCNode**)lua_touserdata(ls, -1);
    return ret;
}

CCNode* LuaController::createActivityRwdView(ActivityEventObj *obj)
{
    addLuaPath();
    if (!obj) {
        return NULL;
    }
    CCDictionary* dic = obj->toDic();
    
    LuaEngine* pEngine = LuaEngine::getInstance();
    string filepath = getLuaFile();
    string path = getLuaPath();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG(filepath.c_str());
    CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
    if (pstrFileContent){
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
#endif
    
    lua_State* ls = pEngine->getLuaStack()->getLuaState();
    lua_getglobal(ls, "createActivityRwdView");
    lua_pushstring(ls, path.c_str());
    lua_newtable(ls);
    CCDictElement* ele = NULL;
    CCDICT_FOREACH(dic, ele)
    {
        if (ele && ele->getObject()) {
            CCString* value = dynamic_cast<CCString*>(ele->getObject());
            std::string key = ele->getStrKey();
            if (!key.empty() && value && value->getCString()) {
                lua_pushstring(ls, key.c_str());
                lua_pushstring(ls, value->getCString());
                lua_settable(ls, -3);
                CCLOG("notice --- key ---%s,value --- %s",key.c_str(),value->getCString());
            }
        }
    }
    lua_call(ls, 2, 1);
    CCNode* ret =  *(CCNode**)lua_touserdata(ls, -1);
    return ret;
}

CCNode* LuaController::createActivityAdCell(ActivityEventObj *obj)
{
    addLuaPath();
    if (!obj) {
        return NULL;
    }
    CCDictionary* dic = obj->toDic();
    
    LuaEngine* pEngine = LuaEngine::getInstance();
    string filepath = getLuaFile();
    string path = getLuaPath();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG(filepath.c_str());
    CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
    if (pstrFileContent){
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
#endif
    
    lua_State* ls = pEngine->getLuaStack()->getLuaState();
    lua_getglobal(ls, "createActivityAdCell");
    lua_pushstring(ls, path.c_str());
    lua_newtable(ls);
    CCDictElement* ele = NULL;
    CCDICT_FOREACH(dic, ele)
    {
        if (ele && ele->getObject()) {
            CCString* value = dynamic_cast<CCString*>(ele->getObject());
            std::string key = ele->getStrKey();
            if (!key.empty() && value && value->getCString()) {
                lua_pushstring(ls, key.c_str());
                lua_pushstring(ls, value->getCString());
                lua_settable(ls, -3);
                CCLOG("notice --- key ---%s,value --- %s",key.c_str(),value->getCString());
            }
        }
    }
    lua_call(ls, 2, 1);
    CCNode* ret =  *(CCNode**)lua_touserdata(ls, -1);
    return ret;
}

CCNode* LuaController::createActivityListCellSprite(ActivityEventObj *obj)
{
    addLuaPath();
    if (!obj) {
        return NULL;
    }
    CCDictionary* dic = obj->toDic();
    
    LuaEngine* pEngine = LuaEngine::getInstance();
    string filepath = getLuaFile();
    string path = getLuaPath();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG(filepath.c_str());
    CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
    if (pstrFileContent){
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
#endif
    
    lua_State* ls = pEngine->getLuaStack()->getLuaState();
    lua_getglobal(ls, "createActivityListCellSprite");
    lua_pushstring(ls, path.c_str());
    lua_newtable(ls);
    CCDictElement* ele = NULL;
    CCDICT_FOREACH(dic, ele)
    {
        if (ele && ele->getObject()) {
            CCString* value = dynamic_cast<CCString*>(ele->getObject());
            std::string key = ele->getStrKey();
            if (!key.empty() && value && value->getCString()) {
                lua_pushstring(ls, key.c_str());
                lua_pushstring(ls, value->getCString());
                lua_settable(ls, -3);
                CCLOG("notice --- key ---%s,value --- %s",key.c_str(),value->getCString());
            }
        }
    }
    lua_call(ls, 2, 1);
    CCNode* ret =  *(CCNode**)lua_touserdata(ls, -1);
    return ret;
}

void LuaController::getActExcData()
{
    auto& arr = ActivityController::getInstance()->activityArr;
    if (arr->count() == 0) {
        return;
    }
    CCObject* item = nullptr;
    ActivityEventObj* obj = nullptr;
    std::vector<std::string> vec;
    bool st = false;
    CCARRAY_FOREACH(arr, item)
    {
        obj = dynamic_cast<ActivityEventObj*>(item);
        if (obj->type == 6)
        {
            if (obj->exchange == "2" || obj->exchange == "1") {
                vec.push_back(obj->id);
            }
        }
    }
    std::string ts("");
    if (vec.size()) {
        for (int i = 0; i < vec.size() - 1; ++i) {
            ts += vec[i];
            ts += "|";
        }
        ts += vec[vec.size() - 1];
        CCDictionary* dic = CCDictionary::create();
        dic->setObject(CCString::create(ts), "ids");
        sendCMD("act.exchange", dic);
    }
}

void LuaController::showActRwdViewByActId(std::string actId)
{
    PopupViewController::getInstance()->addPopupView(ActivityRwdView::createByActId(actId));
}

void LuaController::setTitleName(std::string _name)
{
    UIComponent::getInstance()->setPopupTitleName(_name);
}

std::string LuaController::retReward(CCArray* arr)
{
    return RewardController::getInstance()->retReward(arr);
}

void LuaController::flyHint(std::string icon, std::string titleText, std::string context)
{
    CCCommonUtils::flyHint(icon, titleText, context);
}

void LuaController::showTipsView(std::string context, CCTextAlignment align)
{
    PopupViewController::getInstance()->addPopupView(TipsView::create(context, align));
}

void LuaController::showExchangeSale(CCNode* node,GoldExchangeItem* item){
    addLuaPath();
    if(!node)
        return;
    m_saleViewContainer = node;
    GoldExchangeItem* dataItem=item;
    if (dataItem) {
        if(dataItem->popup_image!="new_recharge" && dataItem->popup_image !="month"){//既不是首充,也不是月卡
            
            string pushStr = dataItem->toString();
            LuaEngine* pEngine = LuaEngine::getInstance();
            string path = getLuaPath();
            string filepath = getLuaFile();
            pEngine->addSearchPath(path.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            CCLOG(filepath.c_str());
            CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
            if (pstrFileContent){
                pEngine->executeString(pstrFileContent->getCString());
            }
#else
            filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
            pEngine->executeScriptFile(filepath.c_str());
#endif
            lua_State* ls = pEngine->getLuaStack()->getLuaState();
            lua_getglobal(ls, "showGoldExchangeView");
            lua_pushstring(ls, path.c_str());
            lua_pushlstring(ls, pushStr.c_str(), pushStr.length());
            lua_call(ls, 2, 0);
            return;
        }
    }
}
CCRect LuaController::createExchangeAdvCell(CCNode* node,std::string &info){
    addLuaPath();
    if(node == NULL)
        return CCRectZero;
    this->m_advCellContainer = node;
    LuaEngine* pEngine = LuaEngine::getInstance();
    string path = getLuaPath();
    string filepath = getLuaFile();
    pEngine->addSearchPath(path.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG(filepath.c_str());
    CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
    if (pstrFileContent){
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
#endif
    if(m_initVersion.compare("1.0.1") == 0 ){
        lua_State* ls = pEngine->getLuaStack()->getLuaState();
        lua_getglobal(ls, "createGoldExchangeAdvCell");
        lua_pushstring(ls, path.c_str());
        lua_pushlstring(ls, info.c_str(), info.length());
        lua_call(ls, 2, 0);
    }else{
        lua_State* ls = pEngine->getLuaStack()->getLuaState();
        lua_getglobal(ls, "createGoldExchangeAdvCell1");
        lua_pushstring(ls, path.c_str());
        lua_pushlstring(ls, info.c_str(), info.length());
        lua_call(ls, 2, 4);
        float sizeH  = lua_tonumber(ls, -1);
        lua_pop(ls,1);
        float sizeW  = lua_tonumber(ls, -1);
        lua_pop(ls,1);
        float posY = lua_tonumber(ls, -1);
        lua_pop(ls,1);
        float posX = lua_tonumber(ls, -1);
        lua_pop(ls,1);
        return CCRect(posX,posY+3,sizeW,sizeH);
    }
    return CCRectZero;
}
void LuaController::updateExchangeAdvCell(CCNode* node,std::string &info){
    if(node == NULL)
        return;
    this->m_advCell = node;
    LuaEngine* pEngine = LuaEngine::getInstance();
    string path = getLuaPath();
    string filepath = getLuaFile();
    pEngine->addSearchPath(path.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG(filepath.c_str());
    CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
    if (pstrFileContent){
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
#endif
    lua_State* ls = pEngine->getLuaStack()->getLuaState();
    lua_getglobal(ls, "updateGoldExchangeAdvCell");
    lua_pushlstring(ls, info.c_str(), info.length());
    lua_call(ls, 1, 0);
}
void LuaController::createExchangeIcon(CCNode *node,std::string &info){
    addLuaPath();
    
    if(node == NULL)
        return;
    this->m_iconContainer=node;
    LuaEngine* pEngine = LuaEngine::getInstance();
    string path = getLuaPath();
    string filepath = getLuaFile();
    pEngine->addSearchPath(path.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG(filepath.c_str());
    CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
    if (pstrFileContent){
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
#endif
    lua_State* ls = pEngine->getLuaStack()->getLuaState();
    lua_getglobal(ls, "createGoldExchangeIcon");
    lua_pushstring(ls, path.c_str());
    lua_pushlstring(ls, info.c_str(), info.length());
    lua_call(ls, 2, 0);
}
void LuaController::doResourceByCommonIndex(int commonIndex, bool isLoad){
    CCLoadSprite::doResourceByCommonIndex(commonIndex, isLoad);
}
CCParticleSystemQuad* LuaController::createParticleForLua(const char* name){
//    ParticleController::initParticle();
    return ParticleController::createParticleForLua(name);
}

std::string LuaController::getLang(std::string &msg){
    std::string returnVal = _lang(msg);
    return  returnVal;
}
std::string LuaController::getLang1(std::string &msg, std::string &param1){
    std::string returnVal = _lang_1(msg.c_str(), param1.c_str());
    return  returnVal;
}
std::string LuaController::getDollarString(string &dollar,string &productId){
    std::string returnVal = PayController::getInstance()->getDollarText(dollar,productId);
    return returnVal;
}
std::string LuaController::getCMDLang(std::string &msg){
    int aaa = atoi(msg.c_str());
    std::string returnVal = CC_CMDITOA(aaa);
    return returnVal;
}
void LuaController::addButtonLight(cocos2d::CCNode *node){
    if(!node || !node->getParent())
        return;
    CCNode *animNode = CCNode::create();
    node->getParent()->addChild(animNode);
    animNode->setPosition(node->getPosition());
    CCSize btnSize = node->getContentSize();
    ButtonLightEffect* btnEffect = ButtonLightEffect::create(btnSize);
    animNode->addChild(btnEffect);
//    CCSprite* Spr = CCSprite::create();
//    animNode->addChild(Spr);
//    string curPath = "ButtonLight_%d.png";
//    CCCommonUtils::makeInternalEffectSpr(Spr, curPath, 12, 0.07,0,2.0);
//    Spr->setAnchorPoint(ccp(0.5, 0.5));
//    CCSize size = node->getContentSize();
//    CCSize size1 = Spr->getContentSize();
//    float scalex = node->getContentSize().width/Spr->getContentSize().width;
//    float scaley = 1.0;
//    scaley =(node->getContentSize().height+10)/Spr->getContentSize().height;
//    Spr->setPositionY(3);
//    Spr->setScaleX(scalex);
//    Spr->setScaleY(scaley);
//    CCSize size2 = Spr->getContentSize();
//    Spr->setOpacity(175);
}
std::string LuaController::getResourceNameByType(int itemid){
    return CCCommonUtils::getResourceNameByType(itemid);
}
void LuaController::addItemIcon(cocos2d::CCNode *node,std::string &itemid,CCLabelTTF *lable){
    if(!node)
        return;
//    CCLoadSpriteType loadType = CCLoadSpriteType_DEFAULT;
    string name ="";
    string picStr = "";
    int itemId = atoi(itemid.c_str());
    if (itemId < 10) {// 资源
        picStr = CCCommonUtils::getResourceIconByType(itemId);
        name = CCCommonUtils::getResourceNameByType(itemId);
        CCSprite *iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(2).c_str());
        CCCommonUtils::setSpriteMaxSize(iconBg, 52,true);
        node->addChild(iconBg);
        iconBg->setPosition(ccp(26, 26));
        auto icon = CCLoadSprite::createSprite(picStr.c_str(),true,CCLoadSpriteType_DEFAULT);
        CCCommonUtils::setSpriteMaxSize(icon, 50, true);
        node->addChild(icon);
        icon->setPosition(ccp(25, 25));
    }else {//道具
        CCCommonUtils::createGoodsIcon(itemId, node, CCSize(52, 52));
        auto& toolInfo = ToolController::getInstance()->getToolInfoById(itemId);
        name = toolInfo.getName();
//        picStr = CCCommonUtils::getIcon(itemid);
//        auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(toolInfo.color).c_str());
//        CCCommonUtils::setSpriteMaxSize(iconBg, 52,true);
//        node->addChild(iconBg);
//        iconBg->setPosition(ccp(26, 26));
//        loadType = CCLoadSpriteType_GOODS;
    }
    if(lable){
        lable->setString(name.c_str());
    }
}
void LuaController::addIconByType(cocos2d::CCNode *node,std::string &itemid,CCLabelTTF *lable,int type,int size){
    if(!node)
        return;
    int bgSize = size + 10;
    string name ="";
    string picStr = "";
    if(type == 0){ // goods or resource
        int itemId = atoi(itemid.c_str());
        if (itemId < 10) {// 资源
            picStr = CCCommonUtils::getResourceIconByType(itemId);
            name = CCCommonUtils::getResourceNameByType(itemId);
            CCSprite *iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(2).c_str());
            CCCommonUtils::setSpriteMaxSize(iconBg, bgSize,true);
            node->addChild(iconBg);
            iconBg->setPosition(ccp(bgSize*0.5, bgSize*0.5));
            auto icon = CCLoadSprite::createSprite(picStr.c_str(),true,CCLoadSpriteType_DEFAULT);
            CCCommonUtils::setSpriteMaxSize(icon, size, true);
            node->addChild(icon);
            icon->setPosition(ccp(size*0.5, size*0.5));
        }else {//道具
            CCCommonUtils::createGoodsIcon(itemId, node, CCSize(bgSize, bgSize));
            auto& toolInfo = ToolController::getInstance()->getToolInfoById(itemId);
            name = toolInfo.getName();
        }
    }else if(type == 1){
        auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[atoi(itemid.c_str())];
        name = _lang(eInfo.name);
        string bgPath = CCCommonUtils::getToolBgByColor(eInfo.color);
        auto bgIcon = CCLoadSprite::createSprite(bgPath.c_str());
        CCCommonUtils::setSpriteMaxSize(bgIcon, bgSize, true);
        node->addChild(bgIcon);
        bgIcon->setPosition(ccp(bgSize*0.5, bgSize*0.5));
        picStr = CCCommonUtils::getIcon(itemid);
        auto icon = CCLoadSprite::createSprite(picStr.c_str(),true,CCLoadSpriteType_EQUIP);
        CCCommonUtils::setSpriteMaxSize(icon, size, true);
        node->addChild(icon);
        icon->setPosition(ccp(size*0.5, size*0.5));
    }
    if(lable){
        lable->setString(name.c_str());
    }
}
//CCSprite* LuaController::createRwdIcon(cocos2d::CCDictionary *dict, int size)
//{
//    if (!dict) return nullptr;
//    int type = dict->valueForKey("type")->intValue();
//    int num = 0;
//    
//    string picStr = "";
//    bool dPath = false;
//    if (type == R_GOODS) {
//        auto value = _dict(dict->objectForKey("value"));
//        string rid = value->valueForKey("id")->getCString();
//        num = value->valueForKey("num")->intValue();
//        picStr = CCCommonUtils::getIcon(rid);
//        dPath = true;
//    } else {
//        num = dict->valueForKey("value")->intValue();
//        picStr = RewardController::getInstance()->getPicByType(type, 0);
//    }
//    auto icon = dPath? CCLoadSprite::createSprite(picStr.c_str(),true,CCLoadSpriteType_GOODS):CCLoadSprite::createSprite(picStr.c_str());
//    CCCommonUtils::setSpriteMaxSize(icon, size, true);
//    return icon;
//}
CCSprite* LuaController::createGoodsIcon(std::string &itemId, int size)
{
    std::string picStr = CCCommonUtils::getIcon(itemId);
    auto icon = CCLoadSprite::createSprite(picStr.c_str(),true,CCLoadSpriteType_GOODS);
    if (!icon) return nullptr;
    CCCommonUtils::setSpriteMaxSize(icon, size, true);
    return icon;
}
CCDictionary* LuaController::getGoodsInfo(std::string &itemId)
{
    int toolID = atoi(itemId.c_str());
    if(ToolController::getInstance()->m_toolInfos.find(toolID)!=ToolController::getInstance()->m_toolInfos.end())
    {
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(toolID);
        return info.toDic();
    } else {
        return nullptr;
    }
}
std::string LuaController::getObjectType(CCObject* obj)
{
    if (!obj) return "NULL";
    if(dynamic_cast<CCString*>(obj)) {
        return "CCString";
    } else if (dynamic_cast<CCArray*>(obj)) {
        return "CCArray";
    } else if (dynamic_cast<CCDictionary*>(obj)) {
        return "CCDictionary";
    } else if (dynamic_cast<CCInteger*>(obj)) {
        return "CCInteger";
    } else {
        return "NULL";
    }
}
int LuaController::getWorldTime(){
   return (int)GlobalData::shared()->getWorldTime();
}
int LuaController::getTimeStamp()
{
    return (int)GlobalData::shared()->getTimeStamp();
}
void LuaController::playEffects(std::string name)
{
    SoundController::sharedSound()->playEffects(name.c_str());
}
std::string LuaController::getSECLang(int time){
    return CC_SECTOA(time);
}

void LuaController::callPaymentToFriend(std::string &itemid, std::string touid,std::string toName){
    
    GoldExchangeItem *dataItem = NULL;
    for (map<string, GoldExchangeItem*>::iterator it = GlobalData::shared()->goldExchangeList.begin(); it != GlobalData::shared()->goldExchangeList.end(); it++) {
        dataItem = it->second;
        if(dataItem==NULL)
            continue;
        if(dataItem->type == "3"){
            if(dataItem->bought||dataItem->popup_image=="close"){
                continue;
            }
            if(dataItem->end>GlobalData::shared()->getWorldTime()){
                if(dataItem!=NULL && dataItem->id.compare(itemid) == 0){
                    if(LuaController::getInstance()->canSendExchange(itemid,touid,toName,3,false)){
                        PayController::getInstance()->callPaymentToFriend(dataItem, touid);
                    }
                    return;
                }
            }
        }
    }
}
void LuaController::callPayment(std::string &itemid){
    GoldExchangeItem *dataItem = NULL;
    for (map<string, GoldExchangeItem*>::iterator it = GlobalData::shared()->goldExchangeList.begin(); it != GlobalData::shared()->goldExchangeList.end(); it++) {
        dataItem = it->second;
        if(dataItem==NULL)
            continue;
        if(dataItem->type == "3"){
            if(dataItem->bought||dataItem->popup_image=="close"){
                continue;
            }
            if(dataItem->end>GlobalData::shared()->getWorldTime()){
                if(dataItem!=NULL && dataItem->id.compare(itemid) == 0){
                    PayController::getInstance()->callPayment(dataItem,"GoldExchangePopupView");
                    return;
                }
            }
        }else if(dataItem->type == "1"){
            if(dataItem->bought || dataItem->popup_image=="hide" || dataItem->popup_image.empty()){
                continue;
            }
            if(dataItem->end>GlobalData::shared()->getWorldTime()){
                if(dataItem!=NULL && dataItem->id.compare(itemid) == 0){
                    PayController::getInstance()->callPayment(dataItem,"GoldExchangePopupView");
                    return;
                }
            }
        }
    }
}
void LuaController::removeAllPopup(){
    PopupViewController::getInstance()->removeAllPopupView();
//    ActivityController::getInstance()->enterGameOpenView();
}
void LuaController::removeLastPopup()
{
    PopupViewController::getInstance()->removeLastPopupView();
}
void LuaController::removeAllPopupView()
{
    PopupViewController::getInstance()->removeAllPopupView();
}
void LuaController::openBagView(int type)
{
    auto view = StoreView::create();
    if (type == 0)
    {
        view->onClickBagBtn(NULL, Control::EventType::TOUCH_DOWN);
    }
    else
    {
        view->onClickStoreBtn(NULL, Control::EventType::TOUCH_DOWN);
    }
    PopupViewController::getInstance()->addPopupInView(view);
}
void LuaController::showDetailPopup(std::string &itemid){
    PopupViewController::getInstance()->addPopupView(GoldExchangeMoreListView::create(atoi(itemid.c_str())));
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GOLDEXCHANGE_ADVERTISING_STOP_SCROLL);
}
bool LuaController::checkSkeletonFile(const char *fileName){
    std::string path = LuaController::getInstance()->getLuaPath() + "/skeleton/" + fileName;
    std::string path1 = path + ".atlas";
    std::string path2 = path + ".json";
    if(CCFileUtils::sharedFileUtils()->isFileExist(path1) && CCFileUtils::sharedFileUtils()->isFileExist(path2)){
        return true;
    }
    return false;
}
void LuaController::addSkeletonAnimation(cocos2d::CCNode *node, const char* skeletonDataFile, const char* atlasFile,const char* aniName,float scale){
    spine::SkeletonAnimation* skAni = spine::SkeletonAnimation::createWithFile(skeletonDataFile,atlasFile,scale);
    node->addChild(skAni);
    skAni->setAnimation(0, aniName, true);
}
int LuaController::getStringLength(std::string &str){
    return Utf8Utils::strlen(str.c_str());
}
std::string LuaController::getUTF8SubString(std::string &str,int start,int end){
    return  CCCommonUtils::subStrByUtf8(str,start,end-start+1);
}
void LuaController::toSelectUser(std::string itemid,bool removeAllPop,int backTo){
    if(removeAllPop){
        PopupViewController::getInstance()->removeAllPopupView();
    }
    PayController::getInstance()->setItemId(itemid);
    auto pop = AllianceInviteView::create(0, ITEM_DONATE,itemid);
    pop->setBackTo(backTo);
    PopupViewController::getInstance()->addPopupInView(pop);
}
std::string LuaController::getPlatform(){
    return GlobalData::shared()->analyticID;
}
std::string LuaController::getLanguage(){
    return LocalController::shared()->getLanguageFileName();
}
CCNode* LuaController::getSaleViewContainer(){
    return this->m_saleViewContainer;
}
CCNode* LuaController::getAdvCellContainer(){
    return this->m_advCellContainer;
}
CCNode* LuaController::getAdvCell(){
    return this->m_advCell;
}
CCNode* LuaController::getIconContainer(){
    return this->m_iconContainer;
}

//下载相关内容
void* LuaFileDownloadAndUncompress(void *data)
{
    LuaController* self = (LuaController*)data;
    do
    {
        if (!self->downloadFile()) {
            break;
        }
        self->downloadFinish();
        // Uncompress zip file.
        if (!self->unZipFile()){
            break;
        }
        self->unzipFinish();
        
    } while (0);
    
    if (self->m_tid)
    {
        delete self->m_tid;
        self->m_tid = NULL;
    }
    return NULL;
}
static size_t downLoadLuaPackage(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    FILE *fp = (FILE*)userdata;
    size_t written = fwrite(ptr, size, nmemb, fp);
    return written;
}
int LuaDownloadProgressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
{
    //    UpdateManager* manager = (UpdateManager*)ptr;
    //    UpdateManager::UMMessage *msg = new UpdateManager::UMMessage();
    //    msg->what = UpdateMANAGER_MESSAGE_PROGRESS;
    //
    //    ProgressMessage *progressData = new ProgressMessage();
    auto percent = (int)(nowDownloaded/totalToDownload*100);
    percent = percent < 0 ? 0 : percent;
    //    progressData->percent = percent;
    //    progressData->manager = manager;
    //    msg->obj = progressData;
    //    manager->_schedule->sendMessage(msg);
//    CCLOG("lua___downloading... %d%%", percent);
    return 0;
}

bool LuaController::checkUpdate(){
    CCLOG("lua___checkUpdate_____start");
    m_serverVersion = this->luaVersion;
    if(m_clientVersion.compare(m_serverVersion) == 0){
        CCLOG("lua___checkUpdate_____cV[%s],sV[%s]__isEqual",m_clientVersion.c_str(),m_serverVersion.c_str());
        return false;
    }
    if(m_downedVersion.compare(m_serverVersion) == 0){
        CCLOG("lua___checkUpdate_____dV[%s],sV[%s]__isEqual",m_downedVersion.c_str(),m_serverVersion.c_str());
        int needSize = this->luazipSize;
        if(!cocos2d::extension::CCDevice::hasEnoughSpace(needSize)){
            CCLOG("do not has enough space to unzip lua file!");
            return false;
        }
        if(unZipFile()){
            unzipFinish();
            return false;
        }else{
            CCLOG("lua___checkUpdate_____zipFIle_error");
            //delete error zip
            string path = m_writeablePath + LUA_ZIP;
            std::remove(path.c_str());
        }
    }
    m_downloadType = LuaDownloadType_LUA;
    startDownload();
    return true;
}
void LuaController::startDownload(){
    if(m_isDownload==false){
        int needSize = this->luaSize + this->luazipSize;
        if(!cocos2d::extension::CCDevice::hasEnoughSpace(needSize))
            return;
        // COCOS2D_DEBUG : 0是外网,1是内网,2是beta
#if COCOS2D_DEBUG == 1
        string hostIP = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_IP);
        CCLOG("lua_host_url [%s]",hostIP.c_str());
        if (hostIP.empty()) {
            hostIP = SERVER_IP;
        }
        m_packagePath = "http://"+hostIP+":8080/gameservice/getfile?luaVersion=" + this->luaVersion;
#else
        m_packagePath = "http://cok.eleximg.com/cok/lua/lua_" + this->luaVersion + ".zip";
#endif
        CCLOG("lua_package_url [%s]",m_packagePath.c_str());
        CCLOG("lua___download_start");
        m_isDownload = true;
        m_tid = new pthread_t();
        pthread_create(&(*m_tid), NULL, LuaFileDownloadAndUncompress, this);
    }
}
bool LuaController::unZipFile(){
    if(LuaDownloadType_NONE == m_downloadType)
        return false;
    
    CCLOG("lua___unzip_start");
    // Open the zip file
    string outFileName = m_writeablePath + LUA_ZIP;
    if(!CCFileUtils::sharedFileUtils()->isFileExist(outFileName)){
        CCLOG("zip file %s is not exit",outFileName.c_str());
        return false;
    }
    if(true){
        std::ifstream ifs;
        ifs.open(outFileName);
        MD5 md5;
        md5.update(ifs);
        std::string zipStr = md5.toString();
        std::string checkStr = "";
        if(this->m_downloadType == LuaDownloadType_LUA){
            checkStr = this->luaMD5Code;
        }else if(this->m_downloadType == LuaDownloadType_RESOURCE){
            checkStr = this->luapackMD5Code;
        }
        if(zipStr.compare(checkStr) != 0){
            CCLOG("zip file %s is not correct. check code [%s], download code [%s]",outFileName.c_str(),checkStr.c_str(),zipStr.c_str());
            return false;
        }
    }
    unzFile zipfile = unzOpen(outFileName.c_str());
    if (! zipfile)
    {
        CCLOG("can not open downloaded zip file %s", outFileName.c_str());
        return false;
    }
    
    // Get info about the zip file
    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
    {
        CCLOG("can not read file global info of %s", outFileName.c_str());
        unzClose(zipfile);
        return false;
    }
    
    // Buffer to hold data read from the zip file
    char readBuffer[LUA_BUFFER_SIZE];
    
    CCLOG("start uncompressing");
    
    // Loop to extract all files.
    uLong i;
    for (i = 0; i < global_info.number_entry; ++i)
    {
        // Get info about current file.
        unz_file_info fileInfo;
        char fileName[LUA_MAX_FILENAME];
        if (unzGetCurrentFileInfo(zipfile,
                                  &fileInfo,
                                  fileName,
                                  LUA_MAX_FILENAME,
                                  NULL,
                                  0,
                                  NULL,
                                  0) != UNZ_OK)
        {
            CCLOG("can not read file info");
            unzClose(zipfile);
            return false;
        }
        
        string fullPath = m_writeablePath + fileName;
        
        // Check if this entry is a directory or a file.
        const size_t filenameLength = strlen(fileName);
        if (fileName[filenameLength-1] == '/')
        {
            // Entry is a direcotry, so create it.
            // If the directory exists, it will failed scilently.
            if (!createDirectory(fullPath.c_str()))
            {
                CCLOG("can not create directory %s", fullPath.c_str());
                unzClose(zipfile);
                return false;
            }
        }
        else
        {
            // Entry is a file, so extract it.
            
            // Open current file.
            if (unzOpenCurrentFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not open file %s", fileName);
                unzClose(zipfile);
                return false;
            }
            
            // Create a file to store current file.
            FILE *out = fopen(fullPath.c_str(), "w+");
            if (! out)
            {
                CCLOG("can not open destination file %s", fullPath.c_str());
                unzCloseCurrentFile(zipfile);
                unzClose(zipfile);
                return false;
            }
            
            // Write current file content to destinate file.
            int error = UNZ_OK;
            do
            {
                error = unzReadCurrentFile(zipfile, readBuffer, LUA_BUFFER_SIZE);
                if (error < 0)
                {
                    CCLOG("can not read zip file %s, error code is %d", fileName, error);
                    unzCloseCurrentFile(zipfile);
                    unzClose(zipfile);
                    return false;
                }
                
                if (error > 0)
                {
                    fwrite(readBuffer, error, 1, out);
                }
            } while(error > 0);
            
            fclose(out);
        }
        
        unzCloseCurrentFile(zipfile);
        
        // Goto next entry listed in the zip file.
        if ((i+1) < global_info.number_entry)
        {
            if (unzGoToNextFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not read next file");
                unzClose(zipfile);
                return false;
            }
        }
    }
    CCLOG("end uncompressing");
    return true;
}
bool LuaController::createDirectory(const char *path)
{
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    
    return true;
}

bool LuaController::downloadFile(){
    m_curl = curl_easy_init();
    if (! m_curl)
    {
        CCLOG("can not init curl");
        return false;
    }
    
    // Create a file to save package.
    string outFileName = m_writeablePath + LUA_ZIP;
//    CCLOG("lua_package_save_path [%s]",outFileName.c_str());
    FILE *fp = fopen(outFileName.c_str(), "w+");
    if (! fp){
//        CCLOG("can not create file %s", outFileName.c_str());
        return false;
    }
 
    // Download pacakge
    CURLcode res;
    curl_easy_setopt(m_curl, CURLOPT_URL, m_packagePath.c_str());
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, downLoadLuaPackage);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, false);
    curl_easy_setopt(m_curl, CURLOPT_PROGRESSFUNCTION, LuaDownloadProgressFunc);
    curl_easy_setopt(m_curl, CURLOPT_PROGRESSDATA, this);
    res = curl_easy_perform(m_curl);
    curl_easy_cleanup(m_curl);
    if (res != 0)
    {
        CCLOG("error when download package");
        fclose(fp);
        return false;
    }
    
    CCLOG("succeed downloading package %s", m_packagePath.c_str());
    
    fclose(fp);
    return true;
}
void LuaController::downloadFinish(){
    if(m_downloadType == LuaDownloadType_LUA){
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(LuaController::saveDownloadVersion), this, 1, 0, 0, false);
    }else if(m_downloadType == LuaDownloadType_RESOURCE){
        
    }
}
void LuaController::unzipFinish(){
    if(m_downloadType == LuaDownloadType_LUA){
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(LuaController::saveClientVersion), this, 1, 0, 0, false);
    }else if(m_downloadType == LuaDownloadType_RESOURCE){
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(LuaController::saveClientVersion1), this, 1, 0, 0, false);
    }
}
void LuaController::changeUI(float t){
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GOLDEXCHANGE_LIST_CHANGE);
    setTranslateMark();
}
void LuaController::resetVersionAndUpdate(){
    m_downedVersion="";
    m_clientVersion="";
//    string path = getLuaPath() + "/version.bin";
//    std::remove(path.c_str());
    checkUpdate();
}
void LuaController::saveDownloadVersion(float fUpdate){
    m_downedVersion = m_serverVersion;
    CCUserDefault::sharedUserDefault()->setStringForKey(LUA_DOWNLOADED_VERSION, m_downedVersion);
    CCUserDefault::sharedUserDefault()->flush();
    std::string aaaa = CCUserDefault::sharedUserDefault()->getStringForKey(LUA_DOWNLOADED_VERSION);
    CCLOG("lua___download_____end:%s,%s",m_downedVersion.c_str(),aaaa.c_str());
}
void LuaController::saveClientVersion(float fUpdate){
    std::string oVersionFile = getLuaPath() + "/"+LUA_VERSION_FILE;
    if(CCFileUtils::sharedFileUtils()->isFileExist(oVersionFile)){
        ifstream ifile(oVersionFile);
        string line;
        vector<string> mVector;
        while(getline(ifile,line)){
            if(!line.empty()){
                mVector.push_back(line);
            }
        }
        ifile.close();
        auto iter = mVector.begin();
        std::string version = (*iter);
        ++iter;
        while (iter!=mVector.end()) {
            vector<std::string> mTmpV;
            CCCommonUtils::splitString((*iter), "=", mTmpV);
            if(mTmpV.size()>1){
                m_md5Map[mTmpV.at(0)] = mTmpV.at(1);
            }
            ++iter;
        }
        ofstream ofile(oVersionFile,ofstream::out);
        ofile<<version<<endl;
        auto iterMD5 = m_md5Map.begin();
        while (iterMD5!=m_md5Map.end()) {
            string writeStr =(*iterMD5).first + "=" + (*iterMD5).second;
            ofile<<writeStr<<endl;
            ++iterMD5;
        }
        ofile.close();
    }
    m_clientVersion = m_serverVersion;
    m_downedVersion = "";
    CCUserDefault::sharedUserDefault()->setStringForKey(LUA_CLIENT_VERSION, m_clientVersion);
    CCUserDefault::sharedUserDefault()->setStringForKey(LUA_DOWNLOADED_VERSION, "");
    CCUserDefault::sharedUserDefault()->flush();
    std::string aaaa = CCUserDefault::sharedUserDefault()->getStringForKey(LUA_CLIENT_VERSION);
    CCLOG("lua___unzip_____end:%s,%s",m_clientVersion.c_str(),aaaa.c_str());
    string path = m_writeablePath + LUA_ZIP;
    std::remove(path.c_str());
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(LuaController::changeUI), this, 2, 0, 0, false);
    m_isDownload = false;
    m_downloadType = LuaDownloadType_NONE;
}
void LuaController::saveClientVersion1(float fUpdate){
    std::string nVersionFile = getLuaPath() + "/pack/version.bin";
    int newVInt = getVersionData(nVersionFile);
    std::string oVersionFile = getLuaPath() + "/"+LUA_VERSION_FILE;
    int oldVInt = getVersionData(oVersionFile);
    ofstream ofile(oVersionFile,ofstream::out);
    m_md5Map[m_packName] = luapackMD5Code;
    if(newVInt>oldVInt){
        std::string version = CC_ITOA(newVInt);
        ofile<<version<<endl;
        replaceNewLuaFile();
    }else{
        std::string version = CC_ITOA(oldVInt);
        ofile<<version<<endl;
    }
    std::remove(nVersionFile.c_str());
    auto iterMD5 = m_md5Map.begin();
    while (iterMD5!=m_md5Map.end()) {
        string writeStr =(*iterMD5).first + "=" + (*iterMD5).second;
        ofile<<writeStr<<endl;
        ++iterMD5;
    }
    ofile.close();
    string path = m_writeablePath + LUA_ZIP;
    std::remove(path.c_str());
    CCLOG("lua___unzip_____end");
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(LuaController::changeUI), this, 2, 0, 0, false);
    m_isDownload = false;
    m_downloadType = LuaDownloadType_NONE;
}
void LuaController::updateNewResource(){
    if(m_isDownload==false){
        if(m_packName.empty())
            return;
        
        m_downloadType = LuaDownloadType_RESOURCE;
        int needSize = 850;
        if(!cocos2d::extension::CCDevice::hasEnoughSpace(needSize))
            return;
        // COCOS2D_DEBUG : 0是外网,1是内网,2是beta
#if COCOS2D_DEBUG == 1
        string hostIP = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_IP);
        CCLOG("lua_host_url [%s]",hostIP.c_str());
        if (hostIP.empty()) {
            hostIP = SERVER_IP;
        }
        m_packagePath = "http://"+hostIP+":8080/gameservice/getfile?luaVersion=" + m_packName;
#else
        m_packagePath = "http://cok.eleximg.com/cok/lua/lua_" + m_packName+"_"+luapackMD5Code + ".zip";
#endif
        CCLOG("lua_package_pack_url [%s]",m_packagePath.c_str());
        CCLOG("lua____pack_download_start");
        m_isDownload = true;
        m_tid = new pthread_t();
        pthread_create(&(*m_tid), NULL, LuaFileDownloadAndUncompress, this);
    }
}

//#define LUA_FILE_COUNT 8
//const std::string luaFileList[LUA_FILE_COUNT] ={"main","GoldExchangeView","GoldExchangeIcon","GoldExchangeCell","GoldExchangeAdvCell","extern","common","CCBReaderLoad"};
void LuaController::replaceNewLuaFile(){
    auto iter = m_md5Map.find("files");
    vector<std::string> tmpFiles;
    if(iter!=m_md5Map.end()){
        CCCommonUtils::splitString((*iter).second, ",", tmpFiles);
    }
    int index = 0;
    std::string packPath =getLuaPath() + "/pack";
    
    auto iter1 = tmpFiles.begin();
    while (iter1!=tmpFiles.end()) {
        std::string path = packPath + "/"+(*iter1)+".lua";
        if(CCFileUtils::sharedFileUtils()->isFileExist(path)){
            unsigned char* buf = NULL;
            ssize_t bufSize = 0;
            buf = CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "r", &bufSize);
            std::string newPath = getLuaPath() + "/" + (*iter1)+".lua";
            FILE* file = fopen(newPath.c_str(), "w");
            if(file){
                fwrite(buf, bufSize, 1, file);
                fclose(file);
            }
            std::remove(path.c_str());
        }
        ++iter1;
    }
    std::remove(packPath.c_str());
}
bool LuaController::checkLuaFiles(){
    auto iter = m_md5Map.find("files");
    vector<std::string> tmpFiles;
    if(iter!=m_md5Map.end()){
        CCCommonUtils::splitString((*iter).second, ",", tmpFiles);
    }else{
        return false;
    }
    auto iter1 = tmpFiles.begin();
    while (iter1!=tmpFiles.end()) {
        std::string filePath =getLuaPath() + "/" +(*iter1)+".lua";
        if(!CCFileUtils::sharedFileUtils()->isFileExist(filePath)){
            return false;
        }
        ++iter1;
    }
    return true;
}
int LuaController::getVersionData(std::string filePath){
    if(CCFileUtils::sharedFileUtils()->isFileExist(filePath)){
        ifstream ifile(filePath);
        string line;
        vector<string> mVector;
        getline(ifile,line);
        ifile.close();
        if(!line.empty()){
            return atoi(line.c_str());
        }
    }
    return 0;
}
bool LuaController::initMD5Info(){
    if(m_md5Map.size()>0)
        return true;
    std::string oVersionFile = getLuaPath() + "/"+LUA_VERSION_FILE;
    if(CCFileUtils::sharedFileUtils()->isFileExist(oVersionFile)){
        ifstream ifile(oVersionFile);
        string line;
        vector<string> mVector;
        while(getline(ifile,line)){
            if(!line.empty()){
                mVector.push_back(line);
            }
        }
        ifile.close();
        auto iter = mVector.begin()+1;
        while (iter!=mVector.end()) {
            string tmpStr = (*iter);
            if(!tmpStr.empty() && tmpStr.find("=")!=string::npos){
                vector<std::string> mTmpV;
                CCCommonUtils::splitString(tmpStr, "=", mTmpV);
                if(mTmpV.size()>1){
                    m_md5Map[mTmpV.at(0)]=mTmpV.at(1);
                }
            }
            ++iter;
        }
        return true;
    }
    return false;
}

//
//Chat Notice 相关代码
//
void LuaController::addSystemUpdate(double countDown, bool isLogin,string tip,FlyHintType type,string icon){
    if (GuideController::share()->isInTutorial()) {
        return;
    }
    CCDictionary *dict = CCDictionary::create();
    dict->retain();
    dict->setObject(CCDouble::create(countDown), "countDown");
    dict->setObject(CCBool::create(isLogin), "isLogin");
    dict->setObject(CCString::create(tip), "tip");
    dict->setObject(CCInteger::create(type), "type");
    dict->setObject(CCString::create(icon), "icon");
    dict->setObject(CCString::create("SystemUpdate"), "hintType");
    
    if(type == FLY_HINT_LOGIN){ //停机公告优先处理
        if(mChatNoticeNode){
            mChatNoticeNode->removeAllChildren();
            mChatNoticeNode->stopAllActions();
//            mChatNoticeNode->removeFromParent();
//            mChatNoticeNode=NULL;
        }
        createChatNotice(dict);
    }else{
        if(mChatNoticeNode && mChatNoticeNode->getChildByTag(CHAT_NOTICE_TAG)){
            mChatNoticeV.push_back(dict);
        }else{
            createChatNotice(dict);
        }
    }
}
void LuaController::addChatNotice(cocos2d::CCDictionary *dict){
    if(ChatController::getInstance()->getIsNoticeOpen() == false){
        return;
    }
    if (GuideController::share()->isInTutorial()) {
        return;
    }
    dict->retain();
    if(mChatNoticeNode && mChatNoticeNode->getChildByTag(CHAT_NOTICE_TAG)){
        bool st = true;
        if (dict->objectForKey("tag"))
        {
            string tag = dict->valueForKey("tag")->getCString();
            if (tag == "alliance_mark")
            {
                for (int i=0; i<mChatNoticeV.size(); i++)
                {
                    if (mChatNoticeV[i]->objectForKey("tag")) {
                        string tagV = mChatNoticeV[i]->valueForKey("tag")->getCString();
                        if (tagV == "alliance_mark")
                        {
                            string msg = dict->valueForKey("msg")->getCString();
                            mChatNoticeV[i]->setObject(CCString::create(msg.c_str()), "msg");
                            st = false;
                        }
                    }
                }
            }
        }
        if (st) {
            mChatNoticeV.push_back(dict);
        }
    }else{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(!ChatServiceCocos2dx::isChatShowing)
            createChatNotice(dict);
        else
            mChatNoticeV.push_back(dict);
#else
         createChatNotice(dict);
#endif
    }
}
void LuaController::createChatNotice(CCDictionary *dict){
    if(mChatNoticeNode && mChatNoticeNode->getChildByTag(CHAT_NOTICE_TAG)){
        return;
    }
    float needTime = 0;
    bool bSys = false;
    if(dict->objectForKey("hintType")){
        bSys = true;
        std::string hintType = dict->valueForKey("hintType")->getCString();
        if(hintType.compare("SystemUpdate") == 0){
            double countDown = dynamic_cast<CCDouble*>(dict->objectForKey("countDown"))->getValue();
            bool isLogin = dynamic_cast<CCBool*>(dict->objectForKey("isLogin"))->getValue();
            string tip = dynamic_cast<CCString*>(dict->objectForKey("tip"))->getCString();
            FlyHintType type = (FlyHintType)dynamic_cast<CCInteger*>(dict->objectForKey("type"))->getValue();
            string icon = dynamic_cast<CCString*>(dict->objectForKey("icon"))->getCString();
            if(type == FLY_HINT_LOGIN){
                double curTime = GlobalData::shared()->getWorldTime();
                double cTime = GlobalData::shared()->changeTime(countDown/1000);
                if( (long)floor( cTime - curTime ) <0)
                    return;
            }
            FlySystemUpdateHint *flySys = FlySystemUpdateHint::create(countDown, isLogin,tip,type,icon);
            if(flySys){
                needTime = flySys->getRollTime();
                flySys->setTag(CHAT_NOTICE_TAG);
                getChatNoticeContainer()->addChild(flySys);
                CCSize winSize = CCDirector::sharedDirector()->getWinSize();
                if (CCCommonUtils::isIosAndroidPad()) {
                    flySys->setPosition(ccp(winSize.width*0.5, winSize.height-420));
                } else {
                    flySys->setPosition(ccp(winSize.width*0.5, winSize.height-210));
                }
            }else{
                return;
            }
        }
    }else{
//        if (CCCommonUtils::isIosAndroidPad())
//        {
//            return;
//        }
        if(m_isDownload == true)
            return;
        if(initLua()==false)
            return;
        if(initMD5Info()==false || checkLuaFiles() == false){
            resetVersionAndUpdate();
            return;
        }
        if(m_clientVersion.compare(this->luaVersion) != 0){
            checkUpdate();
            return;
        }
        auto iter = m_md5Map.find("noticeFiles");
        vector<std::string> tmpFiles;
        if(iter!=m_md5Map.end()){
            CCCommonUtils::splitString((*iter).second, ",", tmpFiles);
        }else{
            resetVersionAndUpdate();
            return;
        }
        auto iter1 = tmpFiles.begin();
        while (iter1!=tmpFiles.end()) {
            std::string filePath =getLuaPath() + "/" +(*iter1);
            if(!CCFileUtils::sharedFileUtils()->isFileExist(filePath)){
                resetVersionAndUpdate();
                return;
            }
            ++iter1;
        }
        
        CCModelLayer* modelLayer = CCModelLayer::create();
        modelLayer->setContentSize(CCSize(CCDirector::sharedDirector()->getWinSize().width, 80));
        modelLayer->setTag(CHAT_NOTICE_TAG - 2);
        modelLayer->setTouchEnabled(true);
        modelLayer->setTouchPriority(0);
        getChatNoticeContainer()->addChild(modelLayer);
        LuaEngine* pEngine = LuaEngine::getInstance();
        string path = getLuaPath();
        string filepath = getLuaFile();
        pEngine->addSearchPath(path.c_str());
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        CCLOG(filepath.c_str());
        CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
        if (pstrFileContent){
            pEngine->executeString(pstrFileContent->getCString());
        }
    #else
        filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
        pEngine->executeScriptFile(filepath.c_str());
    #endif
        
        lua_State* ls = pEngine->getLuaStack()->getLuaState();
        lua_getglobal(ls, "addChatNotice");
        lua_newtable(ls);
        CCDictElement *ele = NULL;
        CCDICT_FOREACH(dict, ele){
            if (ele && ele->getObject()) {
                CCString *value = dynamic_cast<CCString*>(ele->getObject());
                std::string key = ele->getStrKey();
                if(!key.empty() && value && value->getCString()){
                    string valueStr = value->getCString();
                    if(key == "msg"){
                        auto iter = valueStr.find("\n");
                        while (iter!=string::npos) {
                            valueStr = valueStr.replace(iter, 1, "  ");
                            iter = valueStr.find("\n");
                        }
                    }
                    lua_pushstring(ls, key.c_str());//压入key
                    lua_pushstring(ls,valueStr.c_str());//压入value
                    lua_settable(ls,-3);//弹出key,value，并设置到table里面去
                    CCLOG("notice --- key ---%s,value --- %s",key.c_str(),value->getCString());
                }
            }
        }
        if (CCCommonUtils::isIosAndroidPad())
        {
            lua_pushstring(ls, "isPadHD");
            lua_pushstring(ls, "true");
            lua_settable(ls, -3);
        }
        lua_pushstring(ls, path.c_str());
        int showT = ChatController::getInstance()->getNoticeShowTime();
        lua_pushnumber(ls, showT);
        lua_call(ls, 3, 1);
        needTime  = lua_tonumber(ls, -1);
    }
    dict->release();
    if(bSys==false){
        if(PopupViewController::getInstance()->getCurrViewCount()>0){
            showSceneNotice(false);
        }else{
            showSceneNotice(true);
        }
    }
    if(mChatNoticeNode){
        CCDelayTime *delay = CCDelayTime::create(needTime);
        CCLOG("chat_notice_play_time [%f]",needTime);
        mChatNoticeNode->runAction(CCSequence::create(delay,CCCallFuncO::create(this, callfuncO_selector(LuaController::changeChatNotice), NULL),NULL));
    }
}
void LuaController::closeChatNotice(){
    if(mChatNoticeNode){
        mChatNoticeNode->removeAllChildren();
        if(mChatNoticeV.size() == 0){
            mChatNoticeNode->stopAllActions();
//            mChatNoticeNode->removeFromParent();
//            mChatNoticeNode=NULL;
        }
    }
}
void LuaController::changeChatNotice(CCObject *ccObj){
    if(mChatNoticeNode){
        mChatNoticeNode->removeAllChildren();
        mChatNoticeNode->stopAllActions();
//        mChatNoticeNode->removeFromParent();
//        mChatNoticeNode=NULL;
    }
    if(mChatNoticeV.size()>0){
        auto iter = mChatNoticeV.begin();
        createChatNotice((*iter));
        mChatNoticeV.erase(iter);
    }
}
CCNode* LuaController::getChatNoticeContainer(){
    
    if(!mChatNoticeNode){
        mChatNoticeNode = CCNode::create();
        auto layer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_GUIDE);
        layer->addChild(mChatNoticeNode,CHAT_NOTICE_TAG);
//        UIComponent::getInstance()->addChild(mChatNoticeNode);
    }
    return mChatNoticeNode;
}
void LuaController::showSceneNotice(bool b){
    if(mChatNoticeNode && mChatNoticeNode->getChildByTag(CHAT_NOTICE_TAG-1) && mChatNoticeNode->getChildByTag(CHAT_NOTICE_TAG) && mChatNoticeNode->getChildByTag(CHAT_NOTICE_TAG-2)){
        CCModelLayer *model=dynamic_cast<CCModelLayer*>(mChatNoticeNode->getChildByTag(CHAT_NOTICE_TAG-2));
        if(b==true){
            mChatNoticeNode->getChildByTag(CHAT_NOTICE_TAG)->setVisible(true);
            mChatNoticeNode->getChildByTag(CHAT_NOTICE_TAG-1)->setVisible(false);
            model->setPosition(mChatNoticeNode->getChildByTag(CHAT_NOTICE_TAG)->getPosition());
        }else{
            mChatNoticeNode->getChildByTag(CHAT_NOTICE_TAG)->setVisible(false);
            mChatNoticeNode->getChildByTag(CHAT_NOTICE_TAG-1)->setVisible(true);
            model->setPosition(mChatNoticeNode->getChildByTag(CHAT_NOTICE_TAG-1)->getPosition());
        }
    }
}
void LuaController::ChatNoticeInView(CCObject *ccObj){
    showSceneNotice(false);
}
void LuaController::ChatNoticeInSceen(CCObject *ccObj){
    if(PopupViewController::getInstance()->getCurrViewCount()==0 && PopupViewController::getInstance()->getGoBackViewCount() == 0){
        showSceneNotice(true);
    }
}
void LuaController::clearChatNotice(){
    if(mChatNoticeNode){
        mChatNoticeNode->stopAllActions();
        mChatNoticeNode->removeFromParent();
        mChatNoticeNode=NULL;
    }
    if(mChatNoticeV.size()>0){
        auto iter = mChatNoticeV.begin();
        while (iter!=mChatNoticeV.end()) {
            (*iter)->release();
            ++iter;
        }
        mChatNoticeV.clear();
    }
}
//
//礼包赠送
//
void LuaController::updateExchangeGiftInfo(std::string toUID,std::string itemid, double sendTime){
    auto iter = mExGiftList.begin();
    while (iter!=mExGiftList.end()) {
        if(toUID.compare((*iter).receiverId) == 0 && itemid.compare((*iter).exchangeId) == 0){
            (*iter).sendTime = sendTime;
            return;
        }
        ++iter;
    }
    ExchangeGiftStruct exStr;
    exStr.sendTime = sendTime;
    exStr.uid = GlobalData::shared()->playerInfo.uid;
    exStr.exchangeId=itemid;
    exStr.receiverId = toUID;
    mExGiftList.push_back(exStr);
}
void LuaController::initExchangeGiftInfo(CCArray *arr){
    CCObject *ccObj = NULL;
    CCARRAY_FOREACH(arr, ccObj){
        CCDictionary *ccDict = _dict(ccObj);
        if(!ccDict)
            continue;
        ExchangeGiftStruct egStr;
        if(ccDict->objectForKey("uid")){
            egStr.uid = ccDict->valueForKey("uid")->getCString();
        }
        if(ccDict->objectForKey("receiverId")){
            egStr.receiverId = ccDict->valueForKey("receiverId")->getCString();
        }
        if(ccDict->objectForKey("exchangeId")){
            egStr.exchangeId = ccDict->valueForKey("exchangeId")->getCString();
        }
        if(ccDict->objectForKey("sendTime")){
            egStr.sendTime = ccDict->valueForKey("sendTime")->doubleValue()/1000;
        }
        mExGiftList.push_back(egStr);
    }
}
bool LuaController::canSendExchange(std::string &itemid,std::string uid,std::string toName,int type,bool showTip){
    if(itemid.empty() || toName.empty() || uid.empty())
        return false;
    if(type==0){
        auto iter= GlobalData::shared()->goldExchangeList.find(itemid);
        if(iter==GlobalData::shared()->goldExchangeList.end()){
            return false;
        }
        type = atoi((*iter).second->type.c_str());
    }
    
    auto iter = mExGiftList.begin();
    while (iter!=mExGiftList.end()) {
        if((*iter).exchangeId.compare(itemid) == 0 && (*iter).receiverId.compare(uid) == 0){
            switch (type) {
                case 2:{
                    double lastTime = GlobalData::shared()->changeTime((*iter).sendTime) - GlobalData::shared()->getWorldTime();
                    if(lastTime>0){
                        if(showTip){
                            YesNoDialog::show(_lang_1("101009", toName.c_str()), NULL);
                        }
                        return false;
                    }else{
                        return true;
                    }
                    break;
                }
                case 3:{
                    if(showTip){
                        YesNoDialog::show(_lang_1("101009", toName.c_str()), NULL);
                    }
                    return false;
                    break;
                }
                default:
                    break;
            }
        }
        ++iter;
    }
    return true;
}
void LuaController::backToExchange(int backTo){
    switch (backTo) {
        case 1:{//弹出礼包
            PopupViewController::getInstance()->addPopupView(RechargeACTVCell::create());
            break;
        }
        case 2:{
            UIComponent::getInstance()->showGoldBuyPanel();
            break;
        }
        default:
            break;
    }
}

void LuaController::sendCMD(std::string cmdName,CCDictionary *dict){
    LuaCommand *luacmd = new LuaCommand(cmdName,dict);
    luacmd->sendAndRelease();
}
void LuaController::backCMD(std::string cmdName,CCDictionary *dict){
    if (!dict) {
        return;
    }
    m_cmdDic->setObject(dict, cmdName);
//    CCNotificationCenter::sharedNotificationCenter()->postNotification(cmdName.c_str());
}
CCDictionary* LuaController::getCMDParams(std::string cmdName){
    auto ret = LuaController::getInstance()->m_cmdDic->objectForKey(cmdName.c_str());
    if (ret) {
        CCDictionary* retDic = _dict(ret);
        return retDic;
    }
    return NULL;
}

//google translate
void LuaController::setTranslateMark(){
    CCLog("setTranslateMark 1");
    CCLOG("setTranslateMark 1");
    if(canTranslate()){
        CCLOG("setTranslateMark 2");
        CCLog("setTranslateMark 2");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ChatServiceCocos2dx::postTranslateByLuaStart();
#else
        m_canTranslate=true;
#endif
    }else{
        CCLOG("setTranslateMark 3");
        CCLog("setTranslateMark 3");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        //todo set java value
#else
        m_canTranslate=false;
#endif
    }
}
bool LuaController::canTranslate(){
    if(initLua()==false)
        return false;
    if (m_clientVersion.compare(luaVersion) != 0) {
        return false;
    }
    if (m_isDownload == true) {
        return false;
    }
    string checkV = "1.0.53";
    std::vector<string> vector1;
    std::vector<string> vector2;
    CCCommonUtils::splitString(checkV, ".", vector1); //要求的版本
    CCCommonUtils::splitString(m_clientVersion, ".", vector2); //当前的版本
    if(vector1.size() == vector2.size()){
        int index = 0;
        while (index<vector1.size()) {
            int tmpV1 = atoi(vector1.at(index).c_str());
            int tmpV2 = atoi(vector2.at(index).c_str());
            if(tmpV1<tmpV2){
                return true;
            }else if(tmpV1>tmpV2){
                return false;
            }
            ++index;
        }
        return true;
    }
    return false;
}
std::string LuaController::encodeUrl(const std::string& szToEncode){
    std::string src = szToEncode;
    char hex[] = "0123456789ABCDEF";
    string dst;
    for (size_t i = 0; i < src.size(); ++i){
        unsigned char cc = src[i];
        if (isascii(cc)){
            if (cc == ' '){
                dst += "%20";
            }else
                dst += cc;
        }else{
            unsigned char c = static_cast<unsigned char>(src[i]);
            dst += '%';
            dst += hex[c / 16];
            dst += hex[c % 16];
        }
    }
    return dst;
}

void LuaController::translate(std::string orgContent,std::string targeLan,int linkidx){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#else
    if(m_canTranslate==false)
        return;
#endif
    
    addLuaPath();
    LuaEngine* pEngine = LuaEngine::defaultEngine();
    string path = getLuaPath();
    string filepath = getLuaFile();
    pEngine->addSearchPath(path.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG(filepath.c_str());
    CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
    if (pstrFileContent){
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
#endif
    m_orgMsg = orgContent;
    
    lua_State* ls = pEngine->getLuaStack()->getLuaState();
    lua_getglobal(ls, "getTranslateLink");
    lua_pushstring(ls, path.c_str());
    lua_pushlstring(ls, orgContent.c_str(), orgContent.length());
    lua_pushlstring(ls, targeLan.c_str(), targeLan.length());
    lua_pushinteger(ls, linkidx);
    lua_call(ls, 4, 4);
    std::string httpType  = lua_tostring(ls, -1);
    lua_pop(ls,1);
    std::string urlParams  = lua_tostring(ls, -1);
    lua_pop(ls,1);
    std::string url  = lua_tostring(ls, -1);
    lua_pop(ls,1);
    std::string agent  = lua_tostring(ls, -1);
    lua_pop(ls,1);
    
    if(urlParams.empty() && url.empty())
        return;
    
    CURL *curl;
    CURLcode res;
    string cc;
    curl=curl_easy_init();
    if(curl)
    {
        if(httpType.compare("get")==0){
            url = url + "?"+urlParams;
            url = encodeUrl(url);
//            CCLog("translate url: [%s]", url.c_str());
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); //设置请求的地址
        }else{
            url = encodeUrl(url);
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); //设置请求的地址
            curl_easy_setopt(curl, CURLOPT_POST, true); //设置数据类型
            urlParams = encodeUrl(urlParams);
//            CCLog("translate url: [%s],[%s]", url.c_str(),urlParams.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS,urlParams.c_str()); //将操作代码，和连接的网站组合，一起发送！
        }
       
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,LuaController::onTranslate); //数据处理回调函数
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &cc);//缓冲的内存
        curl_easy_setopt(curl,CURLOPT_TIMEOUT_MS,2000);    //设置连接超时时间
        
        curl_easy_setopt(curl,CURLOPT_USERAGENT,agent.c_str());
        
        res=curl_easy_perform(curl);
        if(res!=CURLE_OK){
//            if(res!=CURLE_WRITE_ERROR){
            if(res == CURLE_OPERATION_TIMEDOUT){
                CCLog("curl is error");
                linkidx++;
                LuaController::getInstance()->translate(orgContent, targeLan,linkidx);
            }
        }
        curl_easy_cleanup(curl);
    }else{
        CCLog("curl is null");
    }
}
size_t LuaController::onTranslate(uint8_t* ptr,size_t size,size_t number,void *stream){
    CCString* a=CCString::createWithFormat("%s",ptr);
    std::string serverInfo=a->getCString();
    LuaController::getInstance()->onTranslateBack(serverInfo);
    return  size*number;
}
void LuaController::onTranslateBack(std::string backStr){
    if(initLua()==false)
        return;
//    CCLog("translate back string: [%s]", backStr.c_str());
    LuaController::getInstance()->addLuaPath();
    LuaEngine* pEngine = LuaEngine::defaultEngine();
    string path = LuaController::getInstance()->getLuaPath();
    string filepath = LuaController::getInstance()->getLuaFile();
    pEngine->addSearchPath(path.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG(filepath.c_str());
    CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
    if (pstrFileContent){
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
#endif
    lua_State* ls = pEngine->getLuaStack()->getLuaState();
    lua_getglobal(ls, "praseTranslateMsg");
    lua_pushstring(ls, path.c_str());
    lua_pushlstring(ls, backStr.c_str(), backStr.length());
    
    lua_call(ls, 2, 3);
    
    string json = "{\"originalLang\":\"";
    
    std::string orgContent  = lua_tostring(ls, -1);
    CCLOGFUNCF("orgContent %s",orgContent.c_str());
    lua_pop(ls,1);
    std::string orgLan  = lua_tostring(ls, -1);
    CCLOGFUNCF("orgLan %s",orgLan.c_str());
    lua_pop(ls,1);
    std::string targeContent  = lua_tostring(ls, -1);
    CCLOGFUNCF("targeContent %s",targeContent.c_str());
    lua_pop(ls,1);
    if(m_orgMsg=="" || orgContent=="" || targeContent=="")
        return;
    json.append(orgLan).append("\",\"originalMsg\":\"").append(m_orgMsg).append("\",\"translatedMsg\":\"").append(targeContent).append("\"}");
    CCLOGFUNCF("lua translateTet %s",json.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::postTranslatedResult(json);
#endif
    
    m_orgMsg="";
//    CCLog("translate data [%s],[%s],[%s]",orgContent.c_str(),orgLan.c_str(),targeContent.c_str());
    
}

bool LuaController::isIosAndroidPad()
{
    return CCCommonUtils::isIosAndroidPad();
}
