///
//  LoginCommand.cpp
//  IF
//
//  Created by zhaohf on 13-9-04.
//
//

#include "LoginCommand.h"
#include "CommonInclude.h"
#include "../../Ext/CCDevice.h"
#include "NetController.h"
#include "YesNoDialog.h"
#include "../../view/guiExt/md5.h"
#include "GmController.h"
#include "LuaController.h"
#include "DynamicResourceController.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "AppLibHelper.h"
#endif
//#include "UserLevelCommand.h"
//语言种类
//typedef enum ClientLanguage {
//    cl_ChineseS         = 0,			//简体中文
//    cl_English          = 1,			//英文
//    cl_German           = 2,			//德文
//    cl_French           = 3,			//法文
//    cl_Japanese         = 4,			//日文
//    cl_Ltalian          = 5,			//意大利文
//    cl_Russian          = 6,			//俄文
//    cl_Spanish          = 7,			//西班牙文
//    cl_Dutch            = 8,			//荷兰文
//    cl_Portuguese       = 9,			//葡萄牙文
//    cl_Korean           = 10,			//韩文
//    cl_ChineseT         = 11,			//繁体中文
//} ClientLanguage;

LoginCommand::LoginCommand(int loginType/*= 5*/,bool bIsResume /*= false*/)
: CommandBase("login"), m_bIsResume(bIsResume)
{
    string _gameUid = CCUserDefault::sharedUserDefault()->getStringForKey(GAME_UID, "");
    //新号
    if (_gameUid == "")
    {
        
        //国家
//        if(GlobalData::shared()->loginServerInfo.country != 0){
//            //选择国家
//            putParam("country", CCInteger::create(GlobalData::shared()->loginServerInfo.country));
//            //推荐国家
//            if(GlobalData::shared()->loginServerInfo.country==GlobalData::shared()->loginServerInfo.recommandCountry){
//                putParam("suggestCountry", CCBool::create(true));
//            }
//        }
        GlobalData::shared()->loginServerInfo.country = 1;
        putParam("country", CCInteger::create(GlobalData::shared()->loginServerInfo.country));
        putParam("suggestCountry", CCBool::create(false));
        
        //设备信息
        string _handSetInfo = cocos2d::extension::CCDevice::getHandSetInfo();
        putParam("mt", CCString::create(_handSetInfo));
        //分辨率
        CCSize screenSize = Director::getInstance()->getOpenGLView()->getFrameSize();
        putParam("phone_screen", CCString::createWithFormat("%f*%f",screenSize.width,screenSize.height));
        
        // 客户端类型 0 ios 1 android
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        putParam("platform", CCString::create("1"));
        putParam("gcmRegisterId", CCString::create(GlobalData::shared()->gcmRegisterId));
        putParam("referrer", CCString::create(GlobalData::shared()->referrer));
#else
        putParam("platform", CCString::create("0"));
#endif
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string phoneDevice = CCUserDefault::sharedUserDefault()->getStringForKey("phoneDevice", "");
    putParam("phoneDevice", CCString::create(phoneDevice));
#endif

    //设备ID
    string _deviceId = cocos2d::extension::CCDevice::getDeviceUid();
    putParam("deviceId", CCString::create(_deviceId));
    CCUserDefault::sharedUserDefault()->setStringForKey(DEVICE_UID, _deviceId);
    
    MD5 md5;
    string _time = m_data->valueForKey("cmdBaseTime")->getCString();
    string key = _time + "a0bd01c976818c62b0570b5b7feedf12" + _gameUid;
    md5.update(key.c_str(), key.length());
    putParam("SecurityCode",CCString::create(md5.toString()));
    
    //传入设备ID或者GooglePlay Id  getAccountInfo
    string _googlePlay = cocos2d::extension::CCDevice::getAccountInfo();
    putParam("googlePlay", CCString::create(_googlePlay));
    
    
    if(CCCommonUtils::isIOSPad() || CCCommonUtils::isAdriodPad()) // lzy 判断是否是hd版本登录
    {
        putParam("isHDLogin", CCString::create("1"));
    }
    else
    {
         putParam("isHDLogin", CCString::create("0"));
    }
    
    putParam("pf", CCString::create(GlobalData::shared()->analyticID));
    putParam("pfId", CCString::create(GlobalData::shared()->platformUID));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    putParam("parseRegisterId", CCString::create(GlobalData::shared()->parseRegisterId));
    putParam("fromCountry", CCString::create(GlobalData::shared()->fromCountry));
#else
    putParam("parseRegisterId", CCString::create(cocos2d::extension::CCDevice::getParseInstallationId()));
    putParam("fromCountry", CCString::create(GlobalData::shared()->fromCountry));
#endif
//    _gameUid = "e5b123011c9a4d18a96c3f9f22441be7";
    putParam("gameUid", CCString::create(_gameUid));
    //uuid
    string _uuid = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_UUID,"");
    putParam("uuid", CCString::create(_uuid));
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    string _openUUID = cocos2d::extension::CCDevice::getOpenUDID(false);
    if(_openUUID != _deviceId){
        putParam("newDeviceId", CCString::create(_openUUID));
    }
#endif
    //前台版本
    putParam("appVersion", CCString::create(cocos2d::extension::CCDevice::getVersionName()));
    // 语言
    string language= CCCommonUtils::getLanguage();
    putParam("lang", CCString::create(language));
    //服ID
    string server_id = CCUserDefault::sharedUserDefault()->getStringForKey(SERVER_ID,"");
    putParam("serverId", CCString::create(server_id));
    //gm登陆
    putParam("gmLogin", CCInteger::create(GmController::getInstance()->isGmLogin));
    GmController::getInstance()->isGmLogin = 0;
    string packageName = CCUserDefault::sharedUserDefault()->getStringForKey("android_packageName","");
    putParam("packageName", CCString::create(packageName));
//    //fb 数据来源，做一个记录
//    string appLink = CCUserDefault::sharedUserDefault()->getStringForKey("fbAPPLink","");
//    CCLog("appLink=%s",appLink.c_str());
//    if(appLink!=""){
//        int ftype = 1;
//        if(appLink=="request"){
//            ftype = 2;
//        }
//        putParam("fbType",CCInteger::create(ftype));
//        CCUserDefault::sharedUserDefault()->setStringForKey("fbAPPLink", "");
//        CCUserDefault::sharedUserDefault()->flush();
//    }
 
}

bool LoginCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    CCLOG("LoginCommand::handleRecieve");
    if (dict->valueForKey("cmd")->compare(LOGIN_COMMAND) != 0)
        return false;

    CCLOG("login success");
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (params && params->objectForKey("errorMessage")) {
        string errorCode = params->valueForKey("errorMessage")->getCString();
        if (errorCode == "4") {
            // ban user
//            LocalController::shared()->init();
//            YesNoDialog::showYesDialog("Your account has been banned!",false,CCCallFuncO::create(GameController::getInstance(), callfuncO_selector(GameController::quitGame),NULL),true);
            
            auto _dialog = YesNoDialog::show("Your account has been banned!"
                                             , CCCallFuncO::create(GameController::getInstance(), callfuncO_selector(GameController::quitGame),NULL)
                                             , 0
                                             , true
                                             , CCCallFunc::create(this, callfunc_selector(LoginCommand::feedBack))
                                             );
            _dialog->showCancelButton();
            _dialog->setYesButtonTitle(_lang("E100089").c_str());
            _dialog->setNoButtonTitle(_lang("E100075").c_str());

            return true;
        }
        else if (errorCode == "E001") {
            CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID,"");
            CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID,"");
            CCUserDefault::sharedUserDefault()->flush();
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_RELOGIN);
            return true;
        }
        else if (errorCode == "E002"){
            string zone = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_ZONE,"");
            int len = zone.length() - 3;
            string str2 = zone.substr(3,len);
            int srcServerId = atoi(str2.c_str());
            if (srcServerId>CROSS_SERVER_START_INDEX) {
                CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID,"");
                CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID,"");
                CCUserDefault::sharedUserDefault()->flush();
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_RELOGIN);
            }else{
                CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID,"");
                CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID,"");
                CCUserDefault::sharedUserDefault()->flush();
                NetController::shared()->disconnect();
                LogoutCommand* cmd = new LogoutCommand();
                cmd->sendAndRelease();
            }
            return true;
        }
        else if (errorCode == "110093"){
            YesNoDialog::showYesDialog(_lang(errorCode).c_str());
//            CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID,"");
//            CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID,"");
//            CCUserDefault::sharedUserDefault()->flush();
//            NetController::shared()->disconnect();
//            LogoutCommand* cmd = new LogoutCommand();
//            cmd->sendAndRelease();
            return true;
        }else if(errorCode == "E100094"){
            auto _dialog = YesNoDialog::show(_lang("E100094").c_str()
                                             , CCCallFuncO::create(GameController::getInstance(), callfuncO_selector(GameController::quitGame),NULL)
                                             , 0
                                             , true
                                             , CCCallFunc::create(this, callfunc_selector(LoginCommand::feedBack))
                                             );
            _dialog->showCancelButton();
            _dialog->setYesButtonTitle(_lang("E100089").c_str());
            _dialog->setNoButtonTitle(_lang("E100075").c_str());
            return true;
        }
    }
    
    
    CCDictionary* data = NULL;
    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    data = params;
//#else
//    data = _dict( params->objectForKey("data"));
//#endif
    //版本更新
    CCString* url = dynamic_cast<CCString*>(data->objectForKey("downloadurl"));
    if (url && url->length() > 0) {
        GlobalData::shared()->downloadUrl = data->valueForKey("downloadurl")->getCString();
        GlobalData::shared()->updateType = data->valueForKey("updateType")->intValue();
    } else {
        GlobalData::shared()->updateType = 0;
    }
    CCLog("downloadurl = %s",GlobalData::shared()->downloadUrl.c_str());
    CCLog("xmlVersion server = %s",data->valueForKey("xmlVersion")->getCString());
    CCLog("serverVersion = %s",data->valueForKey("serverVersion")->getCString());
    // xml version
    if (data->objectForKey("xmlVersion")) {
        GlobalData::shared()->xmlVersion = data->valueForKey("xmlVersion")->getCString();
    } else {
        GlobalData::shared()->xmlVersion = "";
    }
    //客户端版本号
    if (data->objectForKey("serverVersion")) {
        GlobalData::shared()->serverVersion = data->valueForKey("serverVersion")->getCString();
    } else {
        GlobalData::shared()->serverVersion = "";
    }
    //lua版本号
    if (data->objectForKey("luaVersion")) {
        std::string luaV = data->valueForKey("luaVersion")->getCString();
        if(!luaV.empty()){
            LuaController::getInstance()->luaVersion = luaV;
        }
    }
    //lua zip文件md5校验码
    if (data->objectForKey("luaCode")) {
       LuaController::getInstance()->luaMD5Code = data->valueForKey("luaCode")->getCString();
    }
    //lua zip包大小
    if (data->objectForKey("luazipSize")) {
        LuaController::getInstance()->luazipSize = data->valueForKey("luazipSize")->intValue();
    }
    //lua解压后文件大小
    if (data->objectForKey("luaSize")) {
        LuaController::getInstance()->luaSize = data->valueForKey("luaSize")->intValue();
    }
    // tao.yu 暂时取消下载更新
//    DynamicResourceController::getInstance()->initServerFightInfo(data);
//    DynamicResourceController::getInstance()->initGoodsInfo(data);
//    DynamicResourceController::getInstance()->initMonsterInfo(data);
//    DynamicResourceController::getInstance()->initThorneInfo(data);
//    DynamicResourceController::getInstance()->initMonsterLayerInfo(data);
//    DynamicResourceController::getInstance()->initMinimapViewInfo(data);
    if((CCCommonUtils::isIOSPad() || CCCommonUtils::isAdriodPad()))  // lzy 下载高清资源
    {
//        DynamicResourceController::getInstance()->initHDCCBInfo(data);
        DynamicResourceController::getInstance()->initHDResourceInfo(data);
    }
    CCLog("xmlVersion = %s",GlobalData::shared()->xmlVersion.c_str());
    callSuccess(NetResult::create());
    
//    if (!GlobalData::shared()->isLoginFlag) {
//        UserLevelCommand *cmd = new UserLevelCommand();
//        cmd->sendAndRelease();
//    }
    
    // record login success
    NetController::shared()->setIsLogout(false);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_LoginSucess);
    return true;
}

void LoginCommand::feedBack(){
    CCCommonUtils::showBanHelpShift();
}






