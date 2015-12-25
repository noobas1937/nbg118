//
//  NetController.cpp
//  IF
//
//  Created by 谢文杰 on 13-2-20.
//
//

#include "CommonInclude.h"
#include "NetController.h"
#include "PushBase.h"
#include "LoginCommand.h"
#include "CCAsyncTask.h"
#include "CCSafeNotificationCenter.h"
#include "YesNoDialog.h"
#include "PopupViewController.h"
#include "UserLevelCommand.h"
#include "GuideController.h"
#include "SceneController.h"
#include "UserBindCommand.h"
#include "PushInit.h"
#include "../Ext/CCDevice.h"
#include "HelpshiftCocos2dx.h"
#include "FunBuildController.h"
#include "GuideController.h"

NetController::NetController()
: m_ip(SERVER_IP), m_port(SERVER_PORT), m_zone(SERVER_ZONE)//, m_xcPayId(SELF_XC_PAY_ID)
, m_serverId(1), m_isChangingServer(false)//默认进1服
, m_isLogout(false)
, m_isFirstReconnect(true)
, m_isRetry(false)
, m_isReConnection(0)
, m_ipConnectTime(0)
, m_isSending(false)
{
    // NBTODO
    isTecentConnectionInUse = false;
    isHKConnectionInUse = false;
    isUSConnectionInUse = true;
    
    resetConnectionUse(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(NetController::onConnectionLost), NET_CONNECTION_LOST, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(NetController::onLoginFinish), MSG_LoginInit, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(NetController::onReLogin), SFS_KICK, NULL);
}

void NetController::onReLogin(CCObject* p){
    PushUserOff::handleResponse(CCDictionary::create());
}

void NetController::connect()
{
    CCLOGFUNC();
    if (!isConnected())
        doConnect();
}

void NetController::onLoginFinish(CCObject* p)
{
//    if (!GlobalData::shared()->isLoginFlag)
//    {
//        UserLevelCommand *cmd = new UserLevelCommand();
//        cmd->sendAndRelease();
//    }

//    RequestQueue::iterator iter;
//    for (iter=m_requestPending.begin(); iter!=m_requestPending.end();++iter)
//        CCAsyncTask::sharedTaskManager()->push(CCCallFunc::create(*iter, callfunc_selector(CommandBase::send)));
//    m_requestPending.clear();
//
    RequestQueue::iterator iter = m_requestPending.begin();
    if(iter!=m_requestPending.end()){
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(NetController::onSendPendRequest), this, 0.5, false);
    }
}
void NetController::onSendPendRequest(float delta)
{
    if(isConnected())
    {
        RequestQueue::iterator iter = m_requestPending.begin();
        if(iter!=m_requestPending.end()){
            CCAsyncTask::sharedTaskManager()->push(CCCallFunc::create(*iter, callfunc_selector(CommandBase::send)));
            m_requestPending.pop_front();
            return;
        }
    }
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(NetController::onSendPendRequest), this);
}

std::string NetController::getIpByType(int type){//0 正常 1 备用
    std::string ip = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_IP);
    std::string resultStr = ip;
    if(ip != "" && ip.find(".com") != -1){
        std::string main = GET_IP_BY_TYPE_MAIN;
        std::string backup = GET_IP_BY_TYPE_BACKUP;
        std::string ipstr = ip.substr(0, ip.find("."));
        if(type == 0){
            resultStr = ipstr + main;
        }else if(type == 1){
            resultStr = ipstr + backup;
        }
    }

    return resultStr;
}

void NetController::getServerStatus(){
    CCHttpRequest* request = new CCHttpRequest();
    CCString* url;
    string _uuid = cocos2d::extension::CCDevice::getDeviceUid();
    string _platformUID = GlobalData::shared()->platformUID;
    string _Country = GlobalData::shared()->fromCountry;
    string _gameUid = CCUserDefault::sharedUserDefault()->getStringForKey(GAME_UID,"");
    string _lang = LocalController::shared()->getLanguageFileName();
    string _serverId = CCUserDefault::sharedUserDefault()->getStringForKey(SERVER_ID, "");
    string _serverIp = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_IP, "");
    url = CCString::createWithFormat("http://%s/probe.php?uuid=%s&loginFlag=%d&country=%s&gameuid=%s&lang=%s&serverId=%s&serverIp=%s",SERVERLIST_IP2,_uuid.c_str(),1,_Country.c_str(),_gameUid.c_str(),_lang.c_str(),_serverId.c_str(),_serverIp.c_str());
    
    CCLOG("server list URL: %s",url->getCString());
    
    request->setUrl(url->getCString());
    request->setRequestType(CCHttpRequest::Type::POST);
    request->setResponseCallback(this, httpresponse_selector(NetController::getServerStatusBack));
    request->setTag("get_server_status");
    CCHttpClient::getInstance()->setTimeoutForConnect(10);
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void NetController::getServerStatusBack(CCHttpClient* client, CCHttpResponse* response){
    int status = 0;//0 其他问题， 1 服务器状态有问题
    std::string _message = _lang("E100088");

    if (!response)
    {
        status = 1;
    }else if (!response->isSucceed())
    {
        status = 1;
    }else{
        std::vector<char>* iter = response->getResponseData();
    
        if(iter == NULL){
            status = 1;
        }else{
            try {
                std::string serverInfo(iter->begin(),iter->end());
                Json *c = Json_create(serverInfo.c_str());
                
                if(c){
                    int _code =Json_getInt(c,"cokerrcode",0);//新版用这个cokerrcode为0 和1  代替code 防止http 200也进来了
                    if (_code != 0) {
                        status = 1;
                    }
                    _message= Json_getString(c, "message", "");
                    Json_dispose(c);
                }
            } catch (...) {
                CCLog("data error");
            }
        }
    }
    
    if (isConnected())
    {
        auto popupview = PopupViewController::getInstance()->getCurrentPopupView();
        auto yesnoView = dynamic_cast<YesNoDialog*>(popupview);
        if (yesnoView)
        {
            PopupViewController::getInstance()->removeLastPopupView();
        }
        return;
    }
    
    auto popupview = PopupViewController::getInstance()->getCurrentPopupView();
    auto yesnoView = dynamic_cast<YesNoDialog*>(popupview);
    if (yesnoView)
    {
        PopupViewController::getInstance()->removeLastPopupView();
    }
    
    if(status == 1){
        auto _dialog = YesNoDialog::show(_message.c_str()
                                         , CCCallFunc::create(this, callfunc_selector(NetController::funQuitGame))
                                         , 0
                                         , true
                                         );
        _dialog->setNoButtonTitle(_lang("E100089").c_str());
    }else if(status == 0){
        auto _dialog = YesNoDialog::show(_lang("E100086").c_str()
                                         , CCCallFunc::create(this, callfunc_selector(NetController::funRetry))
                                         , 0
                                         , true
                                         , CCCallFunc::create(this, callfunc_selector(NetController::funFeedBack))
                                         );
        _dialog->showCancelButton();
        _dialog->setYesButtonTitle(_lang("105248").c_str());
        _dialog->setNoButtonTitle(_lang("E100075").c_str());
    }
}

void NetController::sendLog(std::string type, std::string cmd){
    if(cmd == ""){
        return;
    }
    std::string zone = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_ZONE, "");
    if (zone == "COK1" || zone == "COK100" || zone == "COK200" || zone == "COK300" || zone == "COK400") {
        CCHttpRequest* request = new CCHttpRequest();
        CCString* url;
        url = CCString::createWithFormat("http://184.173.110.107:8082/client/crash?cmd=%s&type=%s", cmd.c_str(), type.c_str());
        request->setUrl(url->getCString());
        request->setRequestType(CCHttpRequest::Type::POST);
        CCHttpClient::getInstance()->setTimeoutForConnect(1);
        CCHttpClient::getInstance()->send(request);
        request->release();
    }
}

void NetController::checkNetWork(std::string cmd){
    if (cmd == "") {
        if(m_requestPending.size() > 0){
            auto command = m_requestPending.back();
            if(command){
                cmd = command->getCommand();
            }
        }
    }
    
    sendLog("1", cmd);
    bool result = isNetWorkOK();
    if(result){
        getServerStatus();
    }
    //玩家网络有问题
    else{
        auto _dialog = YesNoDialog::show(_lang("E100087").c_str()
                                         , CCCallFunc::create(this, callfunc_selector(NetController::funRetry))
                                         , 0
                                         , true
                                         , CCCallFunc::create(this, callfunc_selector(NetController::funQuitGame))
                                         );
        _dialog->showCancelButton();
        _dialog->setYesButtonTitle(_lang("105248").c_str());
        _dialog->setNoButtonTitle(_lang("E100089").c_str());
    }
}

void NetController::funRetry(){
    doReconnection(NULL);
}

void NetController::funQuitGame(){
    GameController::getInstance()->quitGame();
}

void NetController::funFeedBack(){
    cocos2d::CCArray *tags = new cocos2d::CCArray();
    tags->init();
    cocos2d::CCDictionary *config = new cocos2d::CCDictionary();
    cocos2d::CCDictionary *meta = new CCDictionary();
    tags->addObject(new CCString("loading"));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    tags->addObject(new CCString("ios"));
#else
    if(GlobalData::shared()->analyticID != "market_global")
        tags->addObject(new CCString(GlobalData::shared()->analyticID));
#endif
    meta->setObject(tags, HS_TAGS_KEY);
    meta->setObject(CCString::create(GameController::getInstance()->getLoadingLogString()), "loadingLog");
    config->setObject(meta, HS_META_DATA_KEY);
    GlobalData::shared()->isBind = true;
    HelpshiftCocos2dx::showConversation(config);
}

void NetController::onLoginFailed(cocos2d::CCObject *obj)
{
    auto result = dynamic_cast<NetResult*>(obj);
    int errorCode = result->getErrorCode();
    CCLOG("relogin error code: %d",errorCode);
    if (errorCode == 6) {
        m_isLogout = true;
        return;
    }

    if (!m_isRetry && !GuideController::share()->isInTutorial() && !GlobalData::shared()->isPause && !isConnected()) {
        m_isRetry = true;
        checkNetWork("login 2");
//        YesNoDialog::showVariableTitle(_lang("E100048").c_str(),CCCallFuncO::create(this, callfuncO_selector(NetController::doReconnection), NULL),_lang("105248").c_str(),true);
    }
}

void NetController::onConnection(CCObject* p)
{
    CCLOGFUNC();
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("NetController", "onConnection");
    m_isChangingServer = false;
    m_ipConnectTime = 0;
    string savedServerZone = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_ZONE);
    
    if (strcmp(savedServerZone.c_str(), getZone().c_str()) != 0) {
        // 将切换的服务器配置存到本地以便下次读取
        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, getIp().c_str());
        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, getZone().c_str());
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT, getPort());
        CCUserDefault::sharedUserDefault()->flush();
    }
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, NET_CONNECTED);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(NetController::checkSendTimeout), this, 5, false);
    
    if (!m_isFirstReconnect) {
        m_isFirstReconnect = true;
        bool isSendingLogin = false;
        for (RequestQueue::iterator iter=m_requestSended.begin(); iter!=m_requestSended.end(); ++iter) {
            if ((*iter)->getCommand()=="login") {
                CCAsyncTask::sharedTaskManager()->push(CCCallFunc::create(*iter, callfunc_selector(CommandBase::send)));
                m_requestSended.erase(iter);
                isSendingLogin = true;
                break;
            }
        }
        if (!isSendingLogin) {
            for (RequestQueue::iterator iter=m_requestPending.begin(); iter!=m_requestPending.end(); ++iter) {
                if ((*iter)->getCommand()=="login") {
                    CCAsyncTask::sharedTaskManager()->push(CCCallFunc::create(*iter, callfunc_selector(CommandBase::send)));
                    m_requestPending.erase(iter);
                    isSendingLogin = true;
                    break;
                }
            }
        }
        
        // merge sended to pending for sending them again,and now sended is empty
        m_requestPending.merge(m_requestSended,[](CCSafeObject<CommandBase> &cmd1,CCSafeObject<CommandBase> &cmd2){
            return cmd1.getObject() == cmd2.getObject();
        });
        
        if (isSendingLogin) {
            return;
        }
        
    } else {
        for (RequestQueue::iterator iter=m_requestPending.begin(); iter!=m_requestPending.end(); ++iter) {
            if ((*iter)->getCommand()=="login") {
                CCAsyncTask::sharedTaskManager()->push(CCCallFunc::create(*iter, callfunc_selector(CommandBase::send)));
                m_requestPending.erase(iter);
                return;
            }
        }
    }

    if (!getIsLogout()) {
        GlobalData::shared()->isLoginFlag = false;
        LoginCommand * cmdLogin=new LoginCommand();
        cmdLogin->setDefaultUserName();
        cmdLogin->setFailCallback(CCCallFuncO::create(this, callfuncO_selector(NetController::onLoginFailed), NULL));
        cmdLogin->sendAndRelease();
    }
 
}

void NetController::clearRequestQueue()
{
//    for (RequestQueue::iterator iter=m_requestSended.begin(); iter!=m_requestSended.end(); ++iter) {
//        CCAsyncTask::sharedTaskManager()->push(CCCallFuncO::create(*iter, callfuncO_selector(CommandBase::callFail), NetResult::create(Error_Network_Lost)));
//    }
//    m_requestSended.clear();
    
    for (RequestQueue::iterator iter=m_requestPending.begin(); iter!=m_requestPending.end(); ++iter) {
        CCAsyncTask::sharedTaskManager()->push(CCCallFuncO::create(*iter, callfuncO_selector(CommandBase::callFail), NetResult::create(Error_Network_Lost)));
    }
    m_requestPending.clear();
}

void NetController::clearAllRequest(){
    m_requestSended.clear();
    m_requestPending.clear();
}

int NetController::getRealPort(){
    if(isChina()){
        int addNum = 1;
        if(m_port == 80){
            addNum = 1;
        }else if(m_port == 8080){
            addNum = 2;
        }else if(m_port == 9933){
            addNum = 3;
        }
        int serverId = 1;
        if(m_zone != ""){
            std::string subStr = m_zone.substr(3);
            serverId = atoi(subStr.c_str());
        }
        if(isTecentConnectionInUse || isHKConnectionInUse){
            int port = (1000 + serverId) * 10 + addNum;
            return port;
        }else{
            return getPort();
        }
    }
    return getPort();
}

bool NetController::isChina(){
#if COCOS2D_DEBUG != 0
    return false;
#endif

    int serverId = 1;
    if(m_zone != ""){
        std::string subStr = m_zone.substr(3);
        serverId = atoi(subStr.c_str());
    }
    
    if(GlobalData::shared()->isChinaPlatForm() && serverId < MAX_SERVER_NUM){
        return true;
    }

    std::string openStr = CCUserDefault::sharedUserDefault()->getStringForKey("connectHKServer", "");
    bool flag = false;

    if (openStr == "0") {
        flag = true;
    }else{
        std::vector<std::string> m_vector;
        CCCommonUtils::splitString(openStr.c_str(), "|", m_vector);
        int index = 0;
        while (index < m_vector.size()) {
            std::string serverStr = m_vector[index];
            std::vector<std::string> serverVector;
            CCCommonUtils::splitString(serverStr.c_str(), ",", serverVector);
            int startServer = atoi(serverVector[0].c_str());
            int endServer = startServer;
            if(serverVector.size() == 2){
                endServer = atoi(serverVector[1].c_str());
            }
            if(serverId < startServer || serverId > endServer){
                
            }else{
                flag = true;
                break;
            }
            index++;
        }
    }
    if(!flag){
        return false;
    }
#if COCOS2D_DEBUG == 0
    std::string country = GlobalData::shared()->fromCountry;
    CCLog(country.c_str());
    if(country == "CN"){
        return true;
    }
#endif
    return false;
}

std::string NetController::getRealIp(){
    if(isChina()){
        int serverId = 1;
        if(m_zone != ""){
            std::string subStr = m_zone.substr(3);
            serverId = atoi(subStr.c_str());
        }
        if(isTecentConnectionInUse){
            return REAL_IP_TENCENT;
        }else if(isHKConnectionInUse){
            return REAL_IP_HK;
        }else if(isUSConnectionInUse){
            return getIp();
        }
        return getIp();
    }
    return getIp();
}

// NBTODO
void NetController::resetConnectionUse(bool reset){
//    if(reset){
//        isTecentConnectionInUse = false;
//        isHKConnectionInUse = true;
//        isUSConnectionInUse = false;
//    }else{
//        if(isTecentConnectionInUse){
//            isTecentConnectionInUse = false;
//            isHKConnectionInUse = false;
//            isUSConnectionInUse = true;
//        }else if(isHKConnectionInUse){
//            isTecentConnectionInUse = true;
//            isHKConnectionInUse = false;
//            isUSConnectionInUse = false;
//        }else if(isUSConnectionInUse){
//            isTecentConnectionInUse = false;
//            isHKConnectionInUse = true;
//            isUSConnectionInUse = false;
//        }
//    }
    
    isTecentConnectionInUse = false;
    isHKConnectionInUse = false;
    isUSConnectionInUse = true;
}

void NetController::onConnectionLost(CCObject* p)
{
    CCLOGFUNCF("isChangingServer:%d", m_isChangingServer);
    if (m_isChangingServer) {
        return;
    }
    if(m_isReConnection == 0){
        resetConnectionUse(true);
    }
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(NetController::checkSendTimeout), this);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, NET_CONNECTED);
    
    for (RequestQueue::iterator iter=m_requestSended.begin(); iter!=m_requestSended.end(); ++iter) {
        CCAsyncTask::sharedTaskManager()->push(CCCallFuncO::create(*iter, callfuncO_selector(CommandBase::callFail), NetResult::create(Error_Network_Lost)));
    }
    m_requestSended.clear();
//    for (RequestQueue::iterator iter=m_requestPending.begin(); iter!=m_requestPending.end(); ++iter) {
//        CCAsyncTask::sharedTaskManager()->push(CCCallFuncO::create(*iter, callfuncO_selector(CommandBase::callFail), NetResult::create(Error_Network_Lost)));
//    }
//    m_requestPending.clear();

    if (!m_isRetry && !GuideController::share()->isInTutorial() && !GlobalData::shared()->isPause && m_isReConnection == 0) {
        m_isRetry = true;
        auto popupview = PopupViewController::getInstance()->getCurrentPopupView();
        auto yesnoView = dynamic_cast<YesNoDialog*>(popupview);
        if(SceneController::getInstance()->currentSceneId != SCENE_ID_LOADING)
        {
//            m_isReConnection = 0;
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(NetController::checkReConnection), this, 5, false);
        }
        else if(!yesnoView && !isConnected())
        {
            if (NetController::shared()->getPort() == SERVER_PORT) {
                NetController::shared()->setPort(9933);
                CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT, 9933);
            }
            else
            {
                NetController::shared()->setPort(SERVER_PORT);
                CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT, SERVER_PORT);
            }
            std::string ip = "";
            if(m_ipConnectTime % 6 == 4 || m_ipConnectTime % 6 == 5){
                ip = getIpByType(1);
            }else{
                ip = getIpByType(0);
            }
            CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, ip);
            NetController::shared()->setIp(ip);
            m_ipConnectTime++;
            CCUserDefault::sharedUserDefault()->flush();
            checkNetWork("");
//            YesNoDialog::showVariableTitle(_lang("E100048").c_str(),CCCallFuncO::create(this, callfuncO_selector(NetController::doConnect), NULL),_lang("105248").c_str(),true);
        }
    }
}

void NetController::onPublicMessage()
{
}

void NetController::doReconnection(CCObject* p) {
    m_isRetry = false;
    GlobalData::shared()->isLoginFlag = false;
    LoginCommand * cmdLogin=new LoginCommand();
    cmdLogin->setDefaultUserName();
    cmdLogin->setFailCallback(CCCallFuncO::create(this, callfuncO_selector(NetController::onLoginFailed), NULL));
    cmdLogin->sendAndRelease();
//    GameController::getInstance()->showWaitInterface();
}

void NetController::cancelForTarget(cocos2d::CCObject *target) {
    auto cancelCallback = [&](CommandBase* cmd,CCObject* target) {
        auto success = cmd->getSuccessCallback();
        if (success && success->getTargetCallback() == target) {
            cmd->setSuccessCallback(nullptr);
        }
        auto fail = cmd->getFailCallback();
        if (fail && fail->getTargetCallback() == target) {
            cmd->setFailCallback(nullptr);
        }
    };
    for (auto &cmd : m_requestPending) {
        cancelCallback(cmd,target);
    }
    for (auto &cmd : m_requestSended) {
        cancelCallback(cmd,target);
    }
}

bool NetController::checkIsAlreadyInQueue(CheckType type, std::string cmd, CCDictionary *param){
    if(type == CHECK_NULL){
        return false;
    }else{
        auto check = [&](CheckType type, CommandBase *command,std::string cmd, CCDictionary *param){
            if(type == CHECK_CMD){
                if(command->getCommand() == cmd){
                    return true;
                }
            }else if(type == CHECK_CMD_PARAM){
                if(command->getCommand() == cmd){
                    auto commandParam = command->getData();
                    if(commandParam == NULL && param == NULL){
                        return true;
                    }
                    if(commandParam == NULL && param != NULL){
                        return false;
                    }
                    if(param == NULL && commandParam != NULL){
                        return false;
                    }
                    CCDictElement *ele = NULL;
                    CCDICT_FOREACH(param, ele){
                        std::string key = ele->getStrKey();
                        if(key == "cmdBaseTime"){
                            continue;
                        }
                        if(!param->objectForKey(key)){
                            return false;
                        }
                        auto obj1 = commandParam->objectForKey(key);
                        auto obj2 = param->objectForKey(key);

                        auto checkInteger = [](CCObject *obj1, CCObject *obj2){
                            auto value1 = dynamic_cast<CCInteger*>(obj1);
                            auto value2 = dynamic_cast<CCInteger*>(obj2);
                            if(value1 == NULL || value2 == NULL){
                                return false;
                            }
                            if(value1->getValue() == value2->getValue()){
                                return true;
                            }
                            return false;
                        };
                        auto checkBool = [](CCObject *obj1, CCObject *obj2){
                            auto value1 = dynamic_cast<CCBool*>(obj1);
                            auto value2 = dynamic_cast<CCBool*>(obj2);
                            if(value1 == NULL || value2 == NULL){
                                return false;
                            }
                            if(value1->getValue() == value2->getValue()){
                                return true;
                            }
                            return false;
                        };
                        auto checkString = [](CCObject *obj1, CCObject *obj2){
                            auto value1 = dynamic_cast<CCString*>(obj1);
                            auto value2 = dynamic_cast<CCString*>(obj2);
                            if(value1 == NULL || value2 == NULL){
                                return false;
                            }
                            std::string str1 = value1->getCString();
                            std::string str2 = value2->getCString();

                            if(str1 == str2){
                                return true;
                            }
                            return false;
                        };
                        auto checkDouble = [](CCObject *obj1, CCObject *obj2){
                            auto value1 = dynamic_cast<CCDouble*>(obj1);
                            auto value2 = dynamic_cast<CCDouble*>(obj2);
                            if(value1 == NULL || value2 == NULL){
                                return false;
                            }
                            double str1 = value1->getValue();
                            double str2 = value2->getValue();
                            
                            if(str1 == str2){
                                return true;
                            }
                            return false;
                        };

                        if(!checkInteger(obj1, obj2)
                           && !checkBool(obj1, obj2)
                           && !checkString(obj1, obj2)
                           && !checkDouble(obj1, obj2)){
                            return false;
                        }
                    }
                    return true;
                }
            }
            return false;
        };
        for (auto &command : m_requestPending) {
            if(check(type, command, cmd, param)){
                return true;
            };
        }
        for (auto &command : m_requestSended) {
            if(check(type, command, cmd, param)){
                return true;
            };
        }
    }
    return false;
}

void NetController::send(CommandBase *request)
{
    m_isSending = true;
    if(checkIsAlreadyInQueue(request->getCheckType(), request->getCommand(), request->getCheckDict())){
        CCLOGFUNCF("skip request %s 0x%p", request->getCommand().c_str(), request);
        return;
    }
    CCLOGFUNCF("sending request %s 0x%p", request->getCommand().c_str(), request);
    if (!isConnected()) {
        CCLOGFUNCF("not connected, request stacked and do reconnect");
        m_requestPending.push_back(request);
        doConnect();
    }else if (!GlobalData::shared()->isInitFlag && request->getCommand()!="login" && request->getCommand()!="login.init")
    {
        CCLOGFUNCF("not init, request stacked and do reconnect");
        m_requestPending.push_back(request);
        CCLOG("requestPending command = %s",request->getCommand().c_str());
    }
    else if (!getIsLogout()) {
        // normal net request
        if (request->getCommand()=="login")
            doLogin((LoginCommand*)request);
        else if(request->getCommand()=="logout")
            doLogout((LogoutCommand*)request);
        else
            doSend(request);
        request->setTimeSended(time(NULL));
        m_requestSended.push_back(request);

    } else if (request->getCommand()=="login") {
        CCLOGFUNCF("connected but not login, do relogin");
        doLogin((LoginCommand*)request);
        request->setTimeSended(time(NULL));
        m_requestSended.push_back(request);

    } else {
        if (getIsLogout()) {
            // manual logout , user is during register or login , ignore all request
            return;
        }
        CCLOGFUNCF("connected but not login, request stacked");
        m_requestPending.push_back(request);
        if (std::find_if(m_requestSended.begin(), m_requestSended.end(),
            [](CCSafeObject<CommandBase> test){return test->getCommand() == "login";}
            ) == m_requestSended.end()) {
            GlobalData::shared()->isLoginFlag = false;
            LoginCommand * cmdLogin = new LoginCommand();
            cmdLogin->setDefaultUserName();
            cmdLogin->setFailCallback(CCCallFuncO::create(this, callfuncO_selector(NetController::onLoginFailed), NULL));
            cmdLogin->sendAndRelease();
        }
    }
    m_isSending = false;
}

void NetController::checkReConnection(float delta)
{
    if (getIsLogout()) {
        m_isReConnection = 0;
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(NetController::checkReConnection), this);
        return;
    }
    if(/*GlobalData::shared()->analyticID == "cn1" && */isNetWorkOK()){//网络没有问题，与服务器断开重连
        doConnect();
        if(isConnected()){
            m_isReConnection = 0;
            CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(NetController::checkReConnection), this);
            return;
        }
    }

    CCLOGFUNCF("not connected, request stacked and do reconnect %d",m_isReConnection);
    if (!isConnected()) {
        if (m_isReConnection > 2) {
            m_isReConnection = 0;
            if (!GuideController::share()->isInTutorial() && !GlobalData::shared()->isPause && !isConnected()) {
                CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(NetController::checkReConnection), this);
//                YesNoDialog::showVariableTitle(_lang("E100048").c_str(),CCCallFuncO::create(this, callfuncO_selector(NetController::clickOK), NULL),_lang("105248").c_str(),true);
                checkNetWork("");
                return;
            }
        }
        
        m_isReConnection++;

        return;
    }
    else
    {
        m_isReConnection = 0;
    }
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(NetController::checkReConnection), this);
}

void NetController::clickOK(CCObject* p)
{
    LogoutCommand* cmd = new LogoutCommand();
    CCDictionary* _params = CCDictionary::create();
    _params->setObject(CCString::create("cok"), "zoneName");
    _params->setObject(CCString::create("true"), "isReLogin");
    CCDictionary* dict = CCDictionary::create();
    dict->setObject(CCString::create("logout"), "cmd");
    dict->setObject(_params, "params");
    cmd->handleRecieve(dict);
    m_isRetry = true;
}

void NetController::checkSendTimeout(float delta)
{
    time_t timeNow;
    time(&timeNow);
    timeNow-=60;    // 默认超时90秒
    if (getIsLogout()) {
        return;
    }
    for (RequestQueue::iterator iter=m_requestSended.begin(); iter!=m_requestSended.end();) {
        if (timeNow>(*iter)->getTimeSended()) {
            string commandName = (*iter)->getCommand();
            CCLOGFUNCF("request %s 0x%p timeout time %d time send %d", commandName.c_str(), (*iter).getObject(), timeNow, (*iter)->getTimeSended());
            if(commandName == "login.init"){
                CCAsyncTask::sharedTaskManager()->push(CCCallFunc::create(*iter, callfunc_selector(CommandBase::send)));
                m_requestSended.erase(iter++);
                continue;
            }
            CCAsyncTask::sharedTaskManager()->push(CCCallFuncO::create(*iter, callfuncO_selector(CommandBase::callFail), NetResult::create(Error_Network_Lost)));
            m_requestSended.erase(iter++);
        }
        else
            ++iter;
    }
}

void NetController::checkSFSConnectTimeout(float delta)
{
    CCLOG("NetController::checkSFSConnectTimeout");
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(NetController::checkSFSConnectTimeout), this);
    if (!isConnected()) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(SFS_CONNECT_ERROR);
    }
}

void NetController::handleRecieve(cocos2d::CCDictionary *response)
{
    RequestQueue::iterator iter;
    for (iter=m_requestSended.begin(); iter!=m_requestSended.end();) {
        if ((*iter)->handleRecieve(response)) {
            m_requestSended.erase(iter);
            break;
        }
        else
            ++iter;
    }
    if (iter==m_requestSended.end())
        // 不是当前命令的返回，扔给push处理
        PushBase::handleResponse(response);
    
//    for (iter=m_requestPending.begin(); iter!=m_requestPending.end();++iter)
//        CCAsyncTask::sharedTaskManager()->push(CCCallFunc::create(*iter, callfunc_selector(CommandBase::send)));
//    m_requestPending.clear();
}



#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
void NetController::handleRecieve(CCObject* p)
{
    auto params = dynamic_cast<CCDictionary*>(p);
    handleRecieve(params);
}
#endif

static NetController * sharedController=NULL;
NetController* NetController::shared()
{
    if (sharedController==NULL){
        sharedController=new NetController();
        
        // 从本地缓存中取出服务器的连接配置
//        string accountIp = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_IP);
        string accountIp = getIpByType(0);
        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, accountIp);
        CCUserDefault::sharedUserDefault()->flush();
        string accountZone = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_ZONE);
        int accountPort = CCUserDefault::sharedUserDefault()->getIntegerForKey(ACCOUNT_PORT);
//        string accountxcpayId = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_XC_PAYID);
        CCLOG("Net>>get saved server config: ip=[%s],zone=[%s],port=[%d]", accountIp.c_str(), accountZone.c_str(), accountPort);
        
        if (accountIp.empty()) {

            // 直接获取默认配置
            accountIp = sharedController->getIp();
            accountZone = sharedController->getZone();
            accountPort = sharedController->getPort();
//            accountxcpayId = sharedController->getXCPayId();
            
            CCLOG("Net>>get default server config: ip=[%s],zone=[%s],port=[%d]", accountIp.c_str(), accountZone.c_str(), accountPort);
        }
        
        sharedController->setIp(accountIp);
        sharedController->setPort(accountPort);
        sharedController->setZone(accountZone);
//        sharedController->setXCPayId(accountxcpayId);

    }
    return sharedController;
}

void NetController::purgeShared()
{
    CC_SAFE_RELEASE(sharedController);
}

#pragma mark -
#pragma mark Platform dependent
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>

NetController::~NetController()
{
}

bool NetController::isNetWorkOK(){
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/IF/Net", "isNetWorkOK", "()Z")) {
        CCLOGFUNC("jni: no method");
        return false;
    }
    jboolean ret = minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
    return ret;
}

bool NetController::isConnected()
{
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/IF/Net", "isConnected", "()Z")) {
        CCLOGFUNC("jni: no method");
        return false;
    }
    jboolean ret = minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
    return ret;
}

bool NetController::isProcessing()
{
    return m_isSending || m_requestSended.size() > 0 || m_requestPending.size() > 0;
}

void NetController::disconnect()
{
    CCLOGFUNC();
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/IF/Net", "disconnect", "()V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
}

void NetController::doConnect(CCObject* p)
{
    m_isRetry = false;
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/nbg/IF/Net", "connect", "(Ljava/lang/String;ILjava/lang/String;)V")) {
        CCLOGFUNC("jni: no method");
        return;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(NetController::onConnection), NET_CONNECTED, NULL);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(NetController::checkSFSConnectTimeout), this);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(NetController::checkSFSConnectTimeout), this, SFS_CONNECT_TIMEOUT, false, 0, 0.0f);

    jobject javaIp=minfo.env->NewStringUTF(getRealIp().c_str());
    jobject javaZone=minfo.env->NewStringUTF(getZone().c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, javaIp, getRealPort(), javaZone);
    minfo.env->DeleteLocalRef(javaIp);
    minfo.env->DeleteLocalRef(javaZone);
    minfo.env->DeleteLocalRef(minfo.classID);
    resetConnectionUse(false);
}

static jobject getSFSObject(JNIEnv* env)
{
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/nbg/IF/Net", "getSFSObject", "()Lcom/smartfoxserver/v2/entities/data/ISFSObject;") ) {
        return NULL;
    }
    jobject jobj=minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
    jobject ret=env->NewLocalRef(jobj);
    minfo.env->DeleteLocalRef(jobj);
    return ret;
}

static jobject getSFSArray(JNIEnv* env)
{
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/nbg/IF/Net", "getSFSArray", "()Lcom/smartfoxserver/v2/entities/data/ISFSArray;") ) {
        return NULL;
    }
    jobject jobj=minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
    jobject ret=env->NewLocalRef(jobj);
    minfo.env->DeleteLocalRef(jobj);
    return ret;
}

static jobject getIntArray(JNIEnv* env)
{
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/nbg/IF/Net", "getIntArray", "()Ljava/util/Collection;") ) {
        return NULL;
    }
    jobject jobj=minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
    jobject ret=env->NewLocalRef(jobj);
    minfo.env->DeleteLocalRef(jobj);
    return ret;
}

static jobject CCIntArrayToJava(JNIEnv* env, CCIntArray *array)
{
    jobject ret = getIntArray(env);
    if (!array)
        return ret;
    CCObject* pElementInt = NULL;
    CCARRAY_FOREACH(array, pElementInt) {
        CCInteger * intValue=dynamic_cast<CCInteger*>(pElementInt);
        if (!intValue)
            continue;
        
        JniMethodInfo minfo;
        if (! JniHelper::getStaticMethodInfo(minfo, "org/nbg/IF/Net", "addToIntArray", "(Ljava/util/Collection;I)V") )
            continue;
        jobject jobj=minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID, ret, intValue->getValue());
        minfo.env->DeleteLocalRef(minfo.classID);
    }
    return ret;
}

static jobject CCArrayToSFSArray(JNIEnv* env, CCArray*arr, std::string cmd);
static jobject CCDictionaryToSFSObject(JNIEnv* env, CCDictionary*dict, std::string cmd = "")
{
    jobject javaObj=getSFSObject(env);
    if (!dict)
        return javaObj;
    
    CCDictElement* pElement = NULL;
    std::string crashId = "";

    CCDICT_FOREACH(dict, pElement) {
        jclass cls = env->GetObjectClass(javaObj);
        if (!cls)
            continue;

        crashId = pElement->getStrKey();
        jobject javaKey=env->NewStringUTF(pElement->getStrKey());
        const type_info &typeInfo=typeid(*pElement->getObject());
        if (typeInfo == typeid(CCInteger) ) {
            jmethodID mid = env->GetMethodID(cls, "putInt", "(Ljava/lang/String;I)V");
            if (mid)
                env->CallVoidMethod(javaObj, mid, javaKey, ((CCInteger*)pElement->getObject())->getValue());
        }
        else if (typeInfo == typeid(CCBoolean)) {
            jmethodID mid = env->GetMethodID(cls, "putBool", "(Ljava/lang/String;Z)V");
            if (mid)
                env->CallVoidMethod(javaObj, mid, javaKey, ((CCBoolean*)pElement->getObject())->getValue());
        }
        else if (typeInfo == typeid(CCByte)) {
            jmethodID mid = env->GetMethodID(cls, "putByte", "(Ljava/lang/String;B)V");
            if (mid)
                env->CallVoidMethod(javaObj, mid, javaKey, ((CCByte*)pElement->getObject())->getValue());
        }
        else if (typeInfo == typeid(CCLong)) {
            jmethodID mid = env->GetMethodID(cls, "putLong", "(Ljava/lang/String;J)V");
            if (mid)
                env->CallVoidMethod(javaObj, mid, javaKey, ((CCLong*)pElement->getObject())->getValue());
        }
//        else if (typeInfo == typeid(CCLongLong)) {
//            jmethodID mid = env->GetMethodID(cls, "putLong", "(Ljava/lang/String;J)V");
//            if (mid)
//                env->CallVoidMethod(javaObj, mid, javaKey, ((CCLongLong*)pElement->getObject())->getValue());
//        }
//        else if (typeInfo == typeid(CCDouble)) {
//            [obj putDouble:key value:[NSNumber numberWithDouble:(dynamic_cast<CCDouble*>(pElement->getObject()))->getValue()]];
//        }
        else if (typeInfo == typeid(CCFloat)) {
            jmethodID mid = env->GetMethodID(cls, "putFloat", "(Ljava/lang/String;F)V");
            if (mid)
                env->CallVoidMethod(javaObj, mid, javaKey, ((CCFloat*)pElement->getObject())->getValue());
        }
        else if (typeInfo == typeid(CCString)) {
            jmethodID mid = env->GetMethodID(cls, "putUtfString", "(Ljava/lang/String;Ljava/lang/String;)V");
            if (mid) {
                jobject javaValue=env->NewStringUTF(((CCString*)pElement->getObject())->getCString());
                env->CallVoidMethod(javaObj, mid, javaKey, javaValue);
                env->DeleteLocalRef(javaValue);
            }
        }
        else if (typeInfo == typeid(CCDictionary)) {
            jmethodID mid = env->GetMethodID(cls, "putSFSObject", "(Ljava/lang/String;Lcom/smartfoxserver/v2/entities/data/ISFSObject;)V");
            if (mid) {
                jobject javaValue=CCDictionaryToSFSObject(env, (CCDictionary*)pElement->getObject(), cmd + "_1");
                env->CallVoidMethod(javaObj, mid, javaKey, javaValue);
                env->DeleteLocalRef(javaValue);
            }
        }
        else if (typeInfo == typeid(CCIntArray)) {
            jmethodID mid = env->GetMethodID(cls, "putIntArray", "(Ljava/lang/String;Ljava/util/Collection;)V");
            if (mid) {
                jobject javaValue=CCIntArrayToJava(env, (CCIntArray*)pElement->getObject());
                env->CallVoidMethod(javaObj, mid, javaKey, javaValue);
                env->DeleteLocalRef(javaValue);
            }
        }
        else if (typeInfo == typeid(CCArray)) {
            jmethodID mid = env->GetMethodID(cls, "putSFSArray", "(Ljava/lang/String;Lcom/smartfoxserver/v2/entities/data/ISFSArray;)V");
            if (mid) {
                jobject javaValue=CCArrayToSFSArray(env, (CCArray*)pElement->getObject(), cmd + "_2");
                env->CallVoidMethod(javaObj, mid, javaKey, javaValue);
                env->DeleteLocalRef(javaValue);
            }
        }
        else
            CCLOG("dicionaryToSFSObject: unknown object for %s", pElement->getStrKey());
        env->DeleteLocalRef(javaKey);
        env->DeleteLocalRef(cls);
    }
    return javaObj;
}

static jobject CCArrayToSFSArray(JNIEnv* env, CCArray*array, std::string cmd = "")
{
    jobject javaObj=getSFSArray(env);
    if (!array)
        return javaObj;
    
    CCObject* pElement = NULL;
    CCARRAY_FOREACH(array, pElement) {
        jclass cls = env->GetObjectClass(javaObj);
        if (!cls)
            continue;
        
        const type_info &typeInfo=typeid(*pElement);
        if (typeInfo == typeid(CCInteger)) {
            jmethodID mid = env->GetMethodID(cls, "addInt", "(I)V");
            if (mid)
                env->CallVoidMethod(javaObj, mid, ((CCInteger*)pElement)->getValue());
        }
        else if (typeInfo == typeid(CCBoolean)) {
            jmethodID mid = env->GetMethodID(cls, "addBool", "(Z)V");
            if (mid)
                env->CallVoidMethod(javaObj, mid, ((CCBoolean*)pElement)->getValue());
        }
        else if (typeInfo == typeid(CCByte)) {
            jmethodID mid = env->GetMethodID(cls, "addByte", "(B)V");
            if (mid)
                env->CallVoidMethod(javaObj, mid, ((CCByte*)pElement)->getValue());
        }
        else if (typeInfo == typeid(CCLong)) {
            jmethodID mid = env->GetMethodID(cls, "addLong", "(J)V");
            if (mid)
                env->CallVoidMethod(javaObj, mid, ((CCLong*)pElement)->getValue());
        }
//        else if (typeInfo == typeid(CCLongLong)) {
//            jmethodID mid = env->GetMethodID(cls, "addLong", "(J)V");
//            if (mid)
//                env->CallVoidMethod(javaObj, mid, ((CCLongLong*)pElement)->getValue());
//        }
//        else if (typeInfo == typeid(CCDouble)) {
//            [obj addDouble:[NSNumber numberWithDouble:(dynamic_cast<CCDouble*>(pElement))->getValue()]];
//        }
        else if (typeInfo == typeid(CCFloat)) {
            jmethodID mid = env->GetMethodID(cls, "addFloat", "(F)V");
            if (mid)
                env->CallVoidMethod(javaObj, mid, ((CCFloat*)pElement)->getValue());
        }
        else if (typeInfo == typeid(CCString)) {
            jmethodID mid = env->GetMethodID(cls, "addUtfString", "(Ljava/lang/String;)V");
            if (mid) {
                jobject javaValue=env->NewStringUTF(((CCString*)pElement)->getCString());
                env->CallVoidMethod(javaObj, mid, javaValue);
                env->DeleteLocalRef(javaValue);
            }
        }
        else if (typeInfo == typeid(CCDictionary)) {
            jmethodID mid = env->GetMethodID(cls, "addSFSObject", "(Lcom/smartfoxserver/v2/entities/data/ISFSObject;)V");
            if (mid) {
                jobject javaValue=CCDictionaryToSFSObject(env, (CCDictionary*)pElement, cmd + "_1");
                env->CallVoidMethod(javaObj, mid, javaValue);
                env->DeleteLocalRef(javaValue);
            }
        }
        else if (typeInfo == typeid(CCIntArray)) {
            jmethodID mid = env->GetMethodID(cls, "addIntArray", "(Ljava/util/Collection;)V");
            if (mid) {
                jobject javaValue=CCIntArrayToJava(env, (CCIntArray*)pElement);
                env->CallVoidMethod(javaObj, mid, javaValue);
                env->DeleteLocalRef(javaValue);
            }
        }
        else if (typeInfo == typeid(CCArray)) {
            jmethodID mid = env->GetMethodID(cls, "addSFSArray", "(Lcom/smartfoxserver/v2/entities/data/ISFSArray;)V");
            if (mid) {
                jobject javaValue=CCArrayToSFSArray(env, (CCArray*)pElement, cmd + "_2");
                env->CallVoidMethod(javaObj, mid, javaValue);
                env->DeleteLocalRef(javaValue);
            }
        }
        else
            CCLOG("arrayToSFSArray: unknown object in SFSArray");
        env->DeleteLocalRef(cls);
    }
    return javaObj;
}

static CCDictionary * SFSObjectToCCDictionary(jobject obj)
{
    
}

void NetController::doLogin(LoginCommand *request)
{
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/nbg/IF/Net", "login", "(Lcom/smartfoxserver/v2/entities/data/ISFSObject;Ljava/lang/String;Ljava/lang/String;)V") ) {
        return;
    }
    
    jobject jdata=CCDictionaryToSFSObject(minfo.env, request->getData(), "login");
    jstring jusername=minfo.env->NewStringUTF(request->getUsername().c_str());
    jstring jpassword=minfo.env->NewStringUTF(request->getPassword().c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jdata, jusername, jpassword);
    minfo.env->DeleteLocalRef(jdata);
    minfo.env->DeleteLocalRef(jusername);
    minfo.env->DeleteLocalRef(jpassword);
    minfo.env->DeleteLocalRef(minfo.classID);
}

void NetController::recordCmd(string cmd){
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/nbg/IF/Jni", "recordCmd", "(Ljava/lang/String;)V") ) {
        CCLOGFUNC("jni: no method");
        return;
    }
    
    jstring jcmd=minfo.env->NewStringUTF(cmd.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jcmd);
    minfo.env->DeleteLocalRef(jcmd);
    minfo.env->DeleteLocalRef(minfo.classID);
}

void NetController::doSend(CommandBase *request)
{
    if (!isConnected()) {
        CCLOGFUNCF("not connected, request stacked");
        m_requestPending.push_back(request);
        doConnect();
        return;
    }
    
    CCLOGFUNCF("doSend------------->2");
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/nbg/IF/Net", "send", "(Ljava/lang/String;Lcom/smartfoxserver/v2/entities/data/ISFSObject;)V") ) {
        return;
    }
    
    CCLOGFUNCF("doSend------------->3");
    jstring jcmd=minfo.env->NewStringUTF(request->getCommand().c_str());
    recordCmd(request->getCommand().c_str());
    jobject jdata=CCDictionaryToSFSObject(minfo.env, request->getData(), request->getCommand());
    if(jdata == NULL){
        return;
    }
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jcmd, jdata);
    minfo.env->DeleteLocalRef(jcmd);
    CCLOGFUNCF("doSend------------->5");
    minfo.env->DeleteLocalRef(jdata);
    minfo.env->DeleteLocalRef(minfo.classID);
}

void NetController::doLogout(LogoutCommand *request)
{
    if (!isConnected()) {
        CCLOGFUNCF("not connected, request stacked");
        m_requestPending.push_back(request);
        doConnect();
        return;
    }
    
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/nbg/IF/Net", "logout", "()V") ) {
        return;
    }
    
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
}

void NetController::doJoinRoom()
{
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/nbg/IF/Net", "joinRoom", "(Ljava/lang/String;)V") ) {
        return;
    }
    
    string roomName = GlobalData::shared()->playerInfo.roomName;
    if (roomName=="") {
        roomName = GlobalData::shared()->playerInfo.country;
    }
    jstring jusername=minfo.env->NewStringUTF( roomName.c_str() );
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jusername);
    minfo.env->DeleteLocalRef(jusername);
    minfo.env->DeleteLocalRef(minfo.classID);
}

#ifdef __cplusplus
extern "C" {
#endif
	void Java_org_nbg_IF_Net_nativeOnResponse(JNIEnv* env, jobject thiz, jlong response)
	{
        CCDictionary * result=(CCDictionary*)response;
        CCLOGFUNCF(result->valueForKey("cmd")->getCString());
        NetController::shared()->handleRecieve(result);
        result->release();
	}
    
	jlong Java_org_nbg_IF_Net_nativeCCDictionary()
    {
        CCDictionary* dict=CCDictionary::create();
        dict->retain();
        return (long)dict;
    }
    
	void Java_org_nbg_IF_Net_nativeAddCCObjectToCCDictionary(JNIEnv* env, jobject thiz, jlong d, jstring key, jlong v)
    {
        CCDictionary* dict=(CCDictionary*)d;
        CCObject* value=(CCObject*)v;
        dict->setObject(value, JniHelper::jstring2string(key).c_str());
        value->release();
    }
    
	void Java_org_nbg_IF_Net_nativeAddStringToCCDictionary(JNIEnv* env, jobject thiz, jlong d, jstring key, jstring v)
    {
        CCDictionary* dict=(CCDictionary*)d;
        CCString* value=CCString::create(JniHelper::jstring2string(v));
        dict->setObject(value, JniHelper::jstring2string(key).c_str());
    }
    
	jlong Java_org_nbg_IF_Net_nativeCCArray()
    {
        CCArray* arr=CCArray::create();
        arr->retain();
        return (long)arr;
    }
    
    void Java_org_nbg_IF_Net_nativeConnectMode(JNIEnv* env, jobject thiz, jstring v)
    {
        GlobalData::shared()->connectMode = JniHelper::jstring2string(v);
        CCLOG("Native ConnectMode:%s",GlobalData::shared()->connectMode.c_str());
    }
    
	void Java_org_nbg_IF_Net_nativeAddStringToCCArray(JNIEnv* env, jobject thiz, jlong d, jstring v)
    {
        CCArray* arr=(CCArray*)d;
        CCString* value=CCString::create(JniHelper::jstring2string(v));
        arr->addObject(value);
    }
    
	void Java_org_nbg_IF_Net_nativeAddCCObjectToCCArray(JNIEnv* env, jobject thiz, jlong d, jlong v)
    {
        CCArray* arr=(CCArray*)d;
        CCObject* value=(CCObject*)v;
        arr->addObject(value);
        value->release();
    }
    
	jobject Java_org_nbg_IF_Jni_nativeGetAppId(JNIEnv* env)
    {
//        jstring jcmd=env->NewStringUTF(AOEUtilies::getXCAppId().c_str());
//        return jcmd;
        return NULL;
    }
    void Java_org_nbg_stac_empire_sns_FBUtil_nativeSetIsLogin(JNIEnv* env,jobject thiz,jboolean isLogin, jstring v, jstring userName){
//        CCSafeObject<CCBoolean> temp=CCBoolean::create(isLogin);
        if (isLogin) {
            CCString* value=CCString::create(JniHelper::jstring2string(v));
            CCString* _name=CCString::create(JniHelper::jstring2string(userName));
            string strName = _name->getCString();
            CCLOG("facebook name %s",strName.c_str());
            if (strName == "") {
                CCUserDefault::sharedUserDefault()->setStringForKey("tmpFaceBook_Name", "");
            }
            else
            {
                CCUserDefault::sharedUserDefault()->setStringForKey("tmpFaceBook_Name", strName);
            }
            //此处不可以调用语言文件，因为这个方法会在c++加载前调用
//            CCUserDefault::sharedUserDefault()->setStringForKey("tmpFaceBook_Name", "Logged in");
            CCUserDefault::sharedUserDefault()->setStringForKey("tmpFaceBook_uid", JniHelper::jstring2string(v));
            CCUserDefault::sharedUserDefault()->flush();
            
//            CCSafeObject<CCString> temp=value;
            CCLOG("------facebook login jni Uid = %s-----",JniHelper::jstring2string(v).c_str());
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FBLoginSucess,value);
        }else{
            CCLOG("------nativeSetIsLogin facebook login fail");
        }
    }
    void Java_org_nbg_stac_empire_sns_FBUtil_nativeSetIsFBFan(JNIEnv* env,jobject thiz,jboolean isFBFan){
        CCSafeObject<CCBoolean> temp=CCBoolean::create(isFBFan);
        CCLOG("------fb facebook fan jni-----");
        cocos2d::extension::CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FBFans,temp);
    }
    
    void Java_org_nbg_stac_empire_sns_FBUtil_nativeSetRequestResult(JNIEnv* env,jobject thiz,jint result, jstring requestID){
        //CCSafeObject<CCInteger> temp=CCInteger::create(result);
        CCInteger* temp= new CCInteger(result);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(FB_RequestResult,result);
        
        string fbReqID= JniHelper::jstring2string(requestID);
//        string tempID= "";
//        if(fbReqID.find("request")<fbReqID.length()){
//            int pos = fbReqID.find("request=");
//            pos = pos+8;
//            string str = fbReqID.substr(pos);
//            int pos2 = str.length();
//            if(str.find("}")<str.length()){
//                pos2 = str.find("}");
//            }
//            tempID = str.substr(0,pos2);
//            CCLOG(" request tempID=%s",tempID.c_str());
//        }
        CCLOG(" request tempID=%s",fbReqID.c_str());
        CCUserDefault::sharedUserDefault()->setStringForKey(FB_RequestID,fbReqID);
        CCUserDefault::sharedUserDefault()->flush();
        
//        CCLOG("------fb facebook request jni-----tempID=%s",tempID.c_str());
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FBRequestResult);
    }
    
    void Java_org_nbg_stac_empire_sns_FBUtil_nativeSetFeedDialogResult(JNIEnv* env,jobject thiz,jint result){
        CCSafeObject<CCInteger> temp=CCInteger::create(result);
        if(result==1){
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FBFeedDialogResult);
        }
        CCLOG("------facebook feeddialog request jni-----");
    }
    
    void Java_org_nbg_stac_empire_sns_FBUtil_nativeSetFBUID(JNIEnv* env,jobject thiz,jstring userId){
        string id = JniHelper::jstring2string(userId);
        CCUserDefault::sharedUserDefault()->setStringForKey(FB_REQUEST_USER_ID, id);
        CCUserDefault::sharedUserDefault()->flush();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(FB_REQUEST_USER_ID);
        CCLOG("------facebook feeddialog nativeSetFBUID jni-----");
    }
    
    void Java_org_nbg_stac_empire_sns_FBUtil_nativeSendFriendsList(JNIEnv* env,jobject thiz,jstring jstr){
        
        string installFriendsInfo = JniHelper::jstring2string(jstr);
        CCLOG("---------installFriendsInfo--%s",installFriendsInfo.c_str());
        CCUserDefault::sharedUserDefault()->setStringForKey("installFriendsInfo", installFriendsInfo);
        CCUserDefault::sharedUserDefault()->flush();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FBFriendsList);
        return ;
//        jclass cls_arraylist = env->FindClass("java/util/ArrayList");
//        //method in class ArrayList
//        jmethodID arraylist_get = env->GetMethodID(cls_arraylist,"get","(I)Ljava/lang/Object;");
//        jmethodID arraylist_size = env->GetMethodID(cls_arraylist,"size","()I");
//        jint len = env->CallIntMethod(friendList,arraylist_size);
//       // printf("get java ArrayList<friend> object by C++ , then print it...../n");
//        jclass cls_friend = env->FindClass("org/nbg/stac/empire/sns/FBFriendPoj");
//        CCSafeObject<CCArray> result=CCArray::create();
//        
//        CCLOG(CCString::createWithFormat("------arr in size  :  %d-------",len)->getCString());
//        for(int i=0;i<len;i++){
//            jobject obj_friend = env->CallObjectMethod(friendList,arraylist_get,i);            
//            CCLOG("---------1-----------");
//            jmethodID friend_getId = env->GetMethodID(cls_friend,"getUserid","()Ljava/lang/String;");
//            CCLOG("---------2-----------");
//            jmethodID friend_getName = env->GetMethodID(cls_friend,"getUsername","()Ljava/lang/String;");           
//            CCLOG("---------3-----------");
//            jstring name = (jstring)env->CallObjectMethod(obj_friend,friend_getName);
//            CCLOG(env->GetStringUTFChars(name,0));
//            CCLOG("---------4-----------");
//            jstring id = (jstring)env->CallObjectMethod(obj_friend,friend_getId);
//            CCLOG(env->GetStringUTFChars(id,0));
//            CCLOG("---------5----------");
//            CCDictionary* dic=CCDictionary::create();
//            CCLOG("---------6-----------");
//            dic->setObject(CCString::createWithFormat(env->GetStringUTFChars(name,0)),"name");
//            CCLOG("---------7-----------");
//            dic->setObject(CCString::createWithFormat(env->GetStringUTFChars(id,0)),"id");
//            dic->setObject(CCBoolean::create(false),"check");
//            CCLOG("---------8-----------");
//            result->addObject(dic);
//            CCLOG("---------9-----------");
//        }
//        CCLOG("---------10-----------");
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FBFriendsList,result);
    }
    
    void Java_org_nbg_stac_empire_sns_FBUtil_nativeSendInviteFriendsList(JNIEnv* env,jobject thiz,jstring jstr){

        CCUserDefault::sharedUserDefault()->setStringForKey("inviteFriends", JniHelper::jstring2string(jstr));
        CCUserDefault::sharedUserDefault()->flush();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FBIviteFriends);
        

        
//        jclass cls_arraylist = env->FindClass("java/util/ArrayList");
//        //method in class ArrayList
//        jmethodID arraylist_get = env->GetMethodID(cls_arraylist,"get","(I)Ljava/lang/Object;");
//        jmethodID arraylist_size = env->GetMethodID(cls_arraylist,"size","()I");
//        jint len = env->CallIntMethod(friendList,arraylist_size);
//        // printf("get java ArrayList<friend> object by C++ , then print it...../n");
//        jclass cls_friend = env->FindClass("org/nbg/stac/empire/sns/FBFriendPoj");
//        CCArray* result = new CCArray();
// 
//        CCLOG(CCString::createWithFormat("------arr in size  :  %d-------",len)->getCString());
//        for(int i=0;i<len;i++){
//            jobject obj_friend = env->CallObjectMethod(friendList,arraylist_get,i);
//            CCLOG("---------1-----------");
//            jmethodID friend_getId = env->GetMethodID(cls_friend,"getUserid","()Ljava/lang/String;");
//            CCLOG("---------2-----------");
//            jmethodID friend_getName = env->GetMethodID(cls_friend,"getUsername","()Ljava/lang/String;");
//            CCLOG("---------3-----------");
//            jstring name = (jstring)env->CallObjectMethod(obj_friend,friend_getName);
//            CCLOG(env->GetStringUTFChars(name,0));
//            CCLOG("---------4-----------");
//            jstring id = (jstring)env->CallObjectMethod(obj_friend,friend_getId);
//            CCLOG(env->GetStringUTFChars(id,0));
//            CCLOG("---------5----------");
//            CCDictionary* dic=CCDictionary::create();
//            CCLOG("---------6-----------");
//            string nameStr = JniHelper::jstring2string(name);
//            if(nameStr!=""){
//                CCLOG("nameStr=%s",nameStr.c_str());
//                dic->setObject(CCString::createWithFormat(nameStr.c_str()),"name");
//            }
//            CCLOG("---------7-----------");
//            string idStr = JniHelper::jstring2string(name);
//            if(idStr!=""){
//                CCLOG("idStr=%s",idStr.c_str());
//                dic->setObject(CCString::createWithFormat(idStr.c_str()),"id");
//            }
//            dic->setObject(CCBoolean::create(false),"check");
//            CCLOG("---------8-----------");
//            result->addObject(dic);
//            CCLOG("---------9-----------");
//        }
//        CCLOG("---------10-----------");
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FBIviteFriends,result);
    }
    
    // 提供对外的native方法便于设置初始的服务器连接配置
    void Java_org_cocos2dx_ext_Native_nativeSetConnectConfig(JNIEnv* env, jobject thiz, jstring configIp, jstring configZone, jint configPort)
    {
        //CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, JniHelper::jstring2string(configIp).c_str());
        //CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, JniHelper::jstring2string(configZone).c_str());
        //CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT, configPort);
        
    }
    // 提供对外的native方法便于设置
    void Java_org_cocos2dx_ext_Native_nativeSetConfig(JNIEnv* env, jobject thiz, jstring key, jstring value)
    {
        CCUserDefault::sharedUserDefault()->setStringForKey(JniHelper::jstring2string(key).c_str(), JniHelper::jstring2string(value).c_str());
        CCUserDefault::sharedUserDefault()->flush();
    }
    
    //读取cache
    jstring Java_org_cocos2dx_ext_Native_nativeGetConfig(JNIEnv* env, jobject thiz, jstring key)
    {
        jstring jcache=env->NewStringUTF(CCUserDefault::sharedUserDefault()->getStringForKey(JniHelper::jstring2string(key).c_str(), "").c_str());
        return jcache;
    }
    // google key
    jstring Java_org_cocos2dx_ext_Native_nativeGetGoogleEncodedPublickey(JNIEnv* env, jobject thiz)
    {
        return env->NewStringUTF("MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAgGKgG6Vm0VBg5FDi+CW9Tyc8yFVHIweANvdv+3pBwudwLGU8lc/zV4it6KRJw5qqUIY75JkJ1/tIK7gxGGp39wkPaqUYvYA0QfP2ZOaXP1+BoXwFMpEVG4usSst78MvMYp5Jqc7ziXigKkgmsC9zUzvvaunMRTRFikNPXXYmSdqpVLAxpEKhct3zwP+lzfmAWCdQDK3uL/tqEAbb2OGK3catlItWg8xfVVLvg12aziZoJiYdm0KpE6XUi0W5/iOH9eQNuTlufwhcFRwoTm2NUzbvzwEHvWRzq49IL6t9HDLyyde7G1VYNscDd/H0qc8//SWCal4ewBnLiiuq3C4sfwIDAQAB");
        
    }
    
    jstring Java_org_cocos2dx_ext_Native_nativeGetPaymentCallbackUrl(JNIEnv* env, jobject thiz)
    {
        return env->NewStringUTF(PAYMENT_CALLBACK_URL);
    }
    
    jstring Java_org_cocos2dx_ext_Native_nativeGetPaymentCallbackUrlTest(JNIEnv* env, jobject thiz)
    {
        return env->NewStringUTF(PAYMENT_CALLBACK_URL_TEST);
    }

    jstring Java_org_cocos2dx_ext_Native_nativeGetAccountUrl(JNIEnv* env, jobject thiz)
    {
        return env->NewStringUTF(GET_ACCOUNT_URL);
    }

    // 提供对外的native方法便于预先设置用户的ab test值
    void Java_org_cocos2dx_ext_Native_nativeSetABTestConfig(JNIEnv* env, jobject thiz, jstring ab_test_value)
    {
//        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_AB_TEST, JniHelper::jstring2string(ab_test_value).c_str());
    }
    // 提供对外的native方法获取用户的ab test值
    jobject Java_org_cocos2dx_ext_Native_nativeGetAbFlag(JNIEnv* env, jobject thiz)
    {
//        jstring jcmd=env->NewStringUTF(AOEUtilies::getUserAbtestFlag().c_str());
//        return jcmd;
        return NULL;
    }
    //提供对外的native方法获取当前用户登录的服务器ID
    int Java_org_cocos2dx_ext_Native_nativeGetServerId(JNIEnv* env, jobject thiz)
    {
//        return AOEUtilies::getServerId();
        if(GlobalData::shared()->playerInfo.selfServerId == 0)
            return 1;
        return GlobalData::shared()->playerInfo.selfServerId;
    }

    bool Java_org_cocos2dx_ext_Native_nativeIsGECLogEnable(JNIEnv* env, jobject thiz){
//        return AOEUtilies::isGECLogEnable();
        return false;
    }
    
    // 根据玩家离开的天数，获得通知文本
    jobject Java_org_cocos2dx_ext_Native_nativeGetAFKText(JNIEnv* env, jobject thiz, jint days)
    {
        jstring text=env->NewStringUTF("You have a free chance on the lottery. Click it to try your luck!");
        return text;
    }

    // 获得多语言文本
    jobject Java_org_cocos2dx_ext_Native_nativeGetMultiLangText(JNIEnv* env, jobject thiz, jstring key)
    {
//        jstring text=env->NewStringUTF(_(JniHelper::jstring2string(key).c_str()));
//        return text;
        return NULL;
    }
    // 设定GCM注册ID
    void Java_org_cocos2dx_ext_Native_nativeSetGcmRegisterId(JNIEnv* env, jobject thiz, jstring regId)
    {
        GlobalData::shared()->gcmRegisterId = JniHelper::jstring2string(regId);
        CCLOG("gcm_reg %s ",GlobalData::shared()->gcmRegisterId.c_str());
    }
    // 设定parse注册id
    void Java_org_cocos2dx_ext_Native_nativeSetParseRegisterId(JNIEnv* env, jobject thiz, jstring regId)
    {
        GlobalData::shared()->parseRegisterId = JniHelper::jstring2string(regId);
        CCLOG("parse_reg %s ",GlobalData::shared()->parseRegisterId.c_str());
    }
    // 设定gaid
    void Java_org_cocos2dx_ext_Native_nativeSetGaid(JNIEnv* env, jobject thiz, jstring gaid)
    {
        if(GlobalData::shared()->gaidCache == "missed"){
            GlobalData::shared()->gaid = JniHelper::jstring2string(gaid);
            CCLOG("gaid %s ",GlobalData::shared()->gaid.c_str());
            UserBindGaidCommand* cmd = new UserBindGaidCommand(GlobalData::shared()->gaid.c_str());
            cmd->sendAndRelease();
        }else{
            GlobalData::shared()->gaidCache = JniHelper::jstring2string(gaid);
            CCLOG("gaidCache %s ",GlobalData::shared()->gaidCache.c_str());
        }
    }
    //设置广告来源
    void Java_org_cocos2dx_ext_Native_nativeSetAdReferrer(JNIEnv* env, jobject thiz, jstring referrer)
    {
        GlobalData::shared()->referrer = JniHelper::jstring2string(referrer);
        CCLOG("referrer %s ",GlobalData::shared()->referrer.c_str());
    }
    //设置国家
    void Java_org_cocos2dx_ext_Native_nativeSetFromCountry(JNIEnv* env, jobject thiz, jstring fromCountry)
    {
        GlobalData::shared()->fromCountry = JniHelper::jstring2string(fromCountry);
        CCLOG("country %s ",GlobalData::shared()->fromCountry.c_str());
    }
    void Java_org_cocos2dx_ext_Native_nativeShowWaitInterface(JNIEnv* env, jobject thiz)
    {
        GameController::getInstance()->showWaitInterface();
    }
    void Java_org_cocos2dx_ext_Native_nativeRemoveWaitInterface(JNIEnv* env, jobject thiz)
    {
        GameController::getInstance()->removeWaitInterface();
    }
    
//    void Java_com_clash_of_util_MyGooglePlusUtil_nativeSetGPlusUid(JNIEnv* env,jobject thiz,jstring result){
//        CCString* value=CCString::create(JniHelper::jstring2string(result));
//        CCSafeObject<CCString> temp=value;
//        CCLOG("------GPlusUid request jni-----");
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_GPUidSet,temp);
//    }
    void Java_com_clash_of_util_MyGooglePlusUtil_nativeSetIsLogin(JNIEnv* env,jobject thiz,jboolean isLogin, jstring v){
//        CCSafeObject<CCBoolean> temp=CCBoolean::create(isLogin);
        if (isLogin)
        {
            string _gpUid = CCUserDefault::sharedUserDefault()->getStringForKey(GP_USERID,"");
            CCString* value=CCString::create(JniHelper::jstring2string(v));
            if (value != CCString::create("")) {
                CCSafeObject<CCString> temp=value;
                CCLOG("------googlePlay login jni-----");
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_GPLoginSucess,temp);
            }
        }
    }
    
    void Java_org_cocos2dx_ext_Native_nativeShowLoginPanel(JNIEnv* env, jobject thiz)
    {
        
    }
    
    void Java_org_cocos2dx_ext_Native_nativeCloseLoginPanel(JNIEnv* env, jobject thiz)
    {
        
    }
    
    void Java_org_cocos2dx_ext_Native_nativeClearGuideData(JNIEnv* env, jobject thiz)
    {
        GuideController::share()->clearGuideData();
    }
    
    // 平台sdk登录成功，返回platformUID
    void Java_org_cocos2dx_ext_Native_nativeSetPlatformUID(JNIEnv* env, jobject thiz, jstring platformUID)
    {
        //android部分平台需要先登录再进游戏
        GlobalData::shared()->platformUID = JniHelper::jstring2string(platformUID);
        CCLOG("platformUID %s ",GlobalData::shared()->platformUID.c_str());
        //传值为空是为了卡住登陆，如果记录的uid和上次取到的uid不一样则清空缓存数据调用getserverlist
        if(GlobalData::shared()->platformUID != ""){
            string _PlatformUid = CCUserDefault::sharedUserDefault()->getStringForKey(PlatformUid,"");
            if (_PlatformUid != GlobalData::shared()->platformUID) {
                CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID,"");
                CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID,"");
            }
            CCUserDefault::sharedUserDefault()->setStringForKey(PlatformUid,GlobalData::shared()->platformUID);
            CCUserDefault::sharedUserDefault()->flush();
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_PlatformUID);
        }
    }
    
    void Java_org_cocos2dx_ext_Native_nativeSetPlatformToken(JNIEnv* env, jobject thiz, jstring platformToken)
    {
        GlobalData::shared()->platformToken = JniHelper::jstring2string(platformToken);
    }
    
    jstring Java_org_cocos2dx_ext_Native_nativeGetUID(JNIEnv* env, jobject thiz)
    {
        return env->NewStringUTF(GlobalData::shared()->playerInfo.uid.c_str());
    }

    int Java_org_cocos2dx_ext_Native_nativeGetLevel(JNIEnv* env, jobject thiz)
    {
        return FunBuildController::getInstance()->getMainCityLv();
    }

    jstring Java_org_cocos2dx_ext_Native_nativeGetNickname(JNIEnv* env, jobject thiz)
    {
        return env->NewStringUTF(GlobalData::shared()->playerInfo.name.c_str());
    }

    //关闭游戏
    void Java_org_cocos2dx_ext_Native_quitGame(JNIEnv* env, jobject thiz, jstring platformUID)
    {
        GameController::getInstance()->quitGame();
    }
    
    void Java_org_cocos2dx_ext_Native_logOut(JNIEnv* env, jobject thiz){
        if (NetController::shared()->isConnected()) {
            LogoutCommand* cmd = new LogoutCommand();
            cmd->sendAndRelease();
        }
        else
        {
            LogoutCommand* cmd = new LogoutCommand();
            CCDictionary* _params = CCDictionary::create();
            _params->setObject(CCString::create("cok"), "zoneName");
            _params->setObject(CCString::create("true"), "isReLogin");
            CCDictionary* dict = CCDictionary::create();
            dict->setObject(CCString::create("logout"), "cmd");
            dict->setObject(_params, "params");
            cmd->handleRecieve(dict);
        }
    }

    
    //设置appLingk
    void Java_org_cocos2dx_ext_Native_nativeAPPLink(JNIEnv* env, jobject thiz, jstring linkStr)
    {
        string link = JniHelper::jstring2string(linkStr);
        cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("fbAPPLink", link.c_str());
        CCUserDefault::sharedUserDefault()->flush();
        CCLog("fb nativeAPPLink= %s ",link.c_str());
    }

    void Java_org_nbg_stac_empire_sns_FBUtil_nativeSendRequestFriendsList(JNIEnv* env,jobject thiz,jstring jstr){
        string appRequestFriends = JniHelper::jstring2string(jstr);
        CCLog("fb appRequestFriends= %s ",appRequestFriends.c_str());
        CCUserDefault::sharedUserDefault()->setStringForKey("appRequestFriends", appRequestFriends);
        CCUserDefault::sharedUserDefault()->flush();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(FB_APP_FriendsList);
    }
    
    void Java_org_nbg_stac_empire_sns_FBUtil_nativeSendHeadImgUrl(JNIEnv* env,jobject thiz,jstring url){
        
        string userHeadImgUrl = JniHelper::jstring2string(url);
        
        CCSafeObject<CCString> temp = CCString::create(userHeadImgUrl);
        CCLOG("------DeviceUtil userHeadImgUrl jni uid: %s-----",userHeadImgUrl.c_str());
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_GetHeadImgUrl,temp);
    }
    
    //设置get app packageName
    void Java_org_cocos2dx_ext_Native_nativeAPKpackageName(JNIEnv* env, jobject thiz, jstring pName)
    {
        string packageName = JniHelper::jstring2string(pName);
        cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("android_packageName", packageName.c_str());
        CCUserDefault::sharedUserDefault()->flush();
        CCLOG("packageName= %s ",packageName.c_str());
    }
#ifdef __cplusplus
}
#endif

#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

