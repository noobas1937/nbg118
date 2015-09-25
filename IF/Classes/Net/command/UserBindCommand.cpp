//
//  UserBindCommand.cpp
//  IF
//
//  Created by wangzhenlei on 13-9-29.
//
//
#include "cocos2d.h"
#include "UserBindCommand.h"
#include "UIComponent.h"
#include "CCFloatingText.h"
#include "SceneController.h"
#include "PopupViewController.h"
#include "LogoutCommand.h"
#include "NetController.h"
#include "CCCommonUtils.h"
#include "YesNoDialog.h"
#include "../../Ext/CCDevice.h"

bool UserBindCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(User_Bind_COMMAND) != 0)
    {
        return false;
    }
    GameController::getInstance()->removeWaitInterface();
    CCDictionary *params=_dict(dict->objectForKey("params"));
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0)
    {
        CCLOG("errocode: '%s'", pStr->getCString());
        string accountName = "";
        if(m_facebook!=""){
            accountName =_lang("105269");
        }else if(m_googlePlay!=""){
            accountName =_lang("105270");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            accountName = _lang("113900");
#endif
        }
        
        if(m_optType==1)
        {
            CCCommonUtils::flyText(_lang("105291"));//绑定失败
            changeAccountCancelCallback();
        }
        else if (m_optType==2) {
            CCCommonUtils::flyText(_lang_1("105275", accountName.c_str()));
            changeAccountCancelCallback();
        }
    }
    else
    {
        const CCString *pGameUid = params->valueForKey("gameUid");
        if (pGameUid->compare("")!=0)
        {//登录的账号 之前绑定过一个游戏账号， 返回改游戏账号数据了
            if (m_optType==2)
            {//切换账号 提示 是否覆盖数据
                m_gameUid = pGameUid->getCString();
                m_ip = params->valueForKey("ip")->getCString();
                m_port = atoi(params->valueForKey("port")->getCString());
                m_zone = params->valueForKey("zone")->getCString();
                m_uuid = params->valueForKey("uuid")->getCString();
                string accountName = "";
                if(m_facebook!=""){
                    accountName =_lang("105269");
                }else if(m_googlePlay!=""){
                    accountName =_lang("105270");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                    accountName = _lang("113900");
#endif
                }
                
                string tmpInfo = _lang("105296") + "\n";
                tmpInfo += "          " + _lang("105297") + "  " + params->valueForKey("gameUserName")->getCString() + "\n";
                tmpInfo += "          " + _lang("105298") + "  " + params->valueForKey("gameUserLevel")->getCString() + "\n";
                tmpInfo += _lang("105286") + "\n";
                YesNoDialog::show(tmpInfo.c_str(), CCCallFunc::create(this, callfunc_selector(UserBindCommand::changeAccountOKCallback)),0,false,CCCallFunc::create(this, callfunc_selector(UserBindCommand::changeAccountCancelCallback)));
            }
            else if (m_optType==1)
            {//提示绑定失败
                CCCommonUtils::flyText(_lang("4100012"));//105233
                changeAccountCancelCallback();
            }
        }
        else
        {
//            如果是绑定账号，就是绑定成功
//            如果是切换账号，就提示切换失败
            
            int status = params->valueForKey("status")->intValue();
            if (status) {
                CCUserDefault::sharedUserDefault()->setStringForKey(CUSTOM_UID, "");
                CCUserDefault::sharedUserDefault()->flush();
                switch (status) {
                    case 1:
                        CCCommonUtils::flyText("email not confirm!");
                        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_USER_BIND_OK);
                        break;
                    case 2:
                        CCCommonUtils::flyText("passsword error!");
                        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_USER_BIND_OK);
                        break;
                    default:
                        break;
                }
            }
            else
            {
                //CCCommonUtils::flyText(_lang("105278"));
                if (m_optType==1)
                {//绑定成功
                    string _fbUid = CCUserDefault::sharedUserDefault()->getStringForKey("tmpFaceBook_uid","");
                    string _gpUid = CCUserDefault::sharedUserDefault()->getStringForKey("tmpGooglePlay_uid","");
                    if ((m_facebook!="" || _fbUid!="") && m_googlePlay=="") {
                        if (m_facebook!="") {
                            CCUserDefault::sharedUserDefault()->setStringForKey(FB_USERID, m_facebook);
                        }
                        else {
                            CCUserDefault::sharedUserDefault()->setStringForKey(FB_USERID, _fbUid);
                        }
                        string fbName = CCUserDefault::sharedUserDefault()->getStringForKey("tmpFaceBook_Name","");
                        if (fbName!="") {
                            CCUserDefault::sharedUserDefault()->setStringForKey(FB_USERNAME, fbName);
                        }
                    }
                    if ((m_googlePlay!="" || _gpUid!="") && m_facebook=="") {
                        if (m_googlePlay!="") {
                            CCUserDefault::sharedUserDefault()->setStringForKey(GP_USERID, m_googlePlay);
                        }
                        else {
                            CCUserDefault::sharedUserDefault()->setStringForKey(GP_USERID, _gpUid);
                        }
                        string gpName = CCUserDefault::sharedUserDefault()->getStringForKey("tmpGooglePlayName","");
                        if (gpName!="") {
                            CCUserDefault::sharedUserDefault()->setStringForKey(GP_USERNAME, gpName);
                        }
                    }
                    if(m_bindPf != ""){
                        int pf = CCCommonUtils::getBindType(m_bindPf);
                        auto it = GlobalData::shared()->playerInfo.m_platFormBindInfo.find(pf);
                        if(it != GlobalData::shared()->playerInfo.m_platFormBindInfo.end()){
                            it->second.bindPf = m_bindPf;
                            it->second.bindId = m_bindId;
                            it->second.setBindName(m_bindName);
                        }
                    }
                    GlobalData::shared()->playerInfo.bindFlag = true;
                    CCCommonUtils::flyHint("", "", _lang("105290"));
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_USER_BIND_OK);
                }
                else
                {//切换账号失败
                    YesNoDialog::show(_lang("105299").c_str(), NULL);
                    changeAccountCancelCallback();
                }
            }
        }
    }

    return true;
}
void UserBindCommand::changeAccountSecondOKCallback(){
    string fbName = CCUserDefault::sharedUserDefault()->getStringForKey("tmpFaceBook_Name","");
    CCUserDefault::sharedUserDefault()->setStringForKey(FB_USERNAME, fbName);
    CCUserDefault::sharedUserDefault()->setStringForKey(FB_USERID, m_facebook);
    CCUserDefault::sharedUserDefault()->setStringForKey(GP_USERID, m_googlePlay);
    
    CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, m_gameUid);
    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, m_ip);
    CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT, m_port);
    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, m_zone);
    CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID, m_uuid);
    CCUserDefault::sharedUserDefault()->flush();
    
    cocos2d::extension::CCDevice::cancelNotice(-1);
    CCLog("cancel all notice!");
    
    UserCleanPostCommand* cleanCmd = new UserCleanPostCommand();
    cleanCmd->sendAndRelease();
    
    NetController::shared()->setIp(m_ip);
    NetController::shared()->setPort(m_port);
    NetController::shared()->setZone(m_zone);
    NetController::shared()->disconnect();
    
//    LogoutCommand* cmd = new LogoutCommand();
//    cmd->sendAndRelease();
    
    LogoutCommand* cmd = new LogoutCommand();
    CCDictionary* _params = CCDictionary::create();
    _params->setObject(CCString::create("cok"), "zoneName");
    _params->setObject(CCString::create("true"), "isReLogin");
    CCDictionary* dict = CCDictionary::create();
    dict->setObject(CCString::create("logout"), "cmd");
    dict->setObject(_params, "params");
    cmd->handleRecieve(dict);
}
void UserBindCommand::changeAccountOKCallback()
{
    this->retain();
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(UserBindCommand::onEnterFrame), this, 1.0,1, 0.0f, false);
}
void UserBindCommand::onEnterFrame(float t){
    YesNoDialog::show(_lang("105294").c_str(), CCCallFunc::create(this, callfunc_selector(UserBindCommand::changeAccountSecondOKCallback)),0,false,CCCallFunc::create(this, callfunc_selector(UserBindCommand::changeAccountCancelCallback)));
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(UserBindCommand::onEnterFrame), this);
    this->release();
}
void UserBindCommand::continueAccountOKCallback()
{
    UserBindCommand* cmd = new UserBindCommand(m_googlePlay,m_facebook,"","","",3);
    cmd->sendAndRelease();
 //   LogoutCommand* cmd = new LogoutCommand();
 //   cmd->sendAndRelease();
}
void UserBindCommand::cancelContinueAccountOKCallback()
{
    changeAccountCancelCallback();

}
void UserBindCommand::changeAccountCancelCallback()
{
    if (m_facebook != "") {
//        CCUserDefault::sharedUserDefault()->setStringForKey(FB_USERID, m_preUid);
        //CCUserDefault::sharedUserDefault()->setStringForKey(FB_USERNAME, m_preName);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_USER_BIND_CANCEL,CCString::create("fb"));
    }
    else if(m_googlePlay != "")
    {
//        CCUserDefault::sharedUserDefault()->setStringForKey(GP_USERID, m_preUid);
        //CCUserDefault::sharedUserDefault()->setStringForKey(GP_USERNAME, m_preName);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_USER_BIND_CANCEL,CCString::create("gp"));
    }
    else if(m_mail != "")
    {
        CCUserDefault::sharedUserDefault()->setStringForKey(CUSTOM_UID, "");
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_USER_BIND_CANCEL,CCString::create("cu"));
    }
    CCUserDefault::sharedUserDefault()->flush();
}

bool UserBindCancelCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(user_Bind_Cancel) != 0)
        return false;
    GameController::getInstance()->removeWaitInterface();
    CCDictionary *params=_dict(dict->objectForKey("params"));
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
        CCCommonUtils::flyText(_lang(pStr->getCString()));
    }else{
        //CCCommonUtils::flyText(_lang("105274"));
        CCCommonUtils::flyHint("", "", _lang("105293"));
        int iType = atoi(m_type.c_str());
        switch (iType) {
            case DEVICE:
                CCUserDefault::sharedUserDefault()->setStringForKey(DEVICE_UID, "");
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_USER_BIND_CANCEL,CCString::create("de"));
                break;
            case FACEBOOK:
                CCUserDefault::sharedUserDefault()->setStringForKey(FB_USERID, "");
                CCUserDefault::sharedUserDefault()->setStringForKey(FB_USERNAME, "");
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_USER_BIND_CANCEL,CCString::create("fb"));
                break;
            case GOOGLEPLAY:
                CCUserDefault::sharedUserDefault()->setStringForKey(GP_USERID, "");
                CCUserDefault::sharedUserDefault()->setStringForKey(GP_USERNAME, "");
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_USER_BIND_CANCEL,CCString::create("gp"));
                break;
            case CUSTOM:
                CCUserDefault::sharedUserDefault()->setStringForKey(CUSTOM_UID, "");
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_USER_BIND_CANCEL,CCString::create("cu"));
                break;
            case APPSTORE:
                CCUserDefault::sharedUserDefault()->setStringForKey(GP_USERID, "");
                CCUserDefault::sharedUserDefault()->setStringForKey(GP_USERNAME, "");
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_USER_BIND_CANCEL,CCString::create("gp"));
                break;
            case WEIBO:
            case VK:
                {
                    auto it = GlobalData::shared()->playerInfo.m_platFormBindInfo.find(iType);
                    if(it != GlobalData::shared()->playerInfo.m_platFormBindInfo.end()){
                        it->second.bindId = "";
                        it->second.setBindName("");
                        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_USER_BIND_CANCEL,    CCString::create(it->second.bindPf));
                    }
                }
                break;
            default:
                break;
        }
    }
    
    return true;
}

//
bool UserCleanPostCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(User_Clean_Post) != 0)
        return false;
    return true;
}
bool UserBindGaidCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(USER_BIND_GAID) != 0)
        return false;
    return true;
}