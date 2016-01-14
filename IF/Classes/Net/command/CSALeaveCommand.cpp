//
//  CSALeaveCommand.cpp
//  IF
//
//  Created by ganxiaohua on 15/6/10.
//
//

#include "CSALeaveCommand.h"
#include "SceneController.h"
#include "WorldMapView.h"
#include "NetController.h"
#include "LogoutCommand.h"
#include "FlyHint.h"
#include "ActivityController.h"

bool CSALeaveCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(CROSS_SERVER_LEAVE) != 0)
        return false;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    GameController::getInstance()->removeWaitInterface();
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
    }else{
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(REMOVE_ALL_HINT);
        ActivityController::getInstance()->ckfNeedLeave = 0;
        if(SceneController::getInstance()->showBG){
            CC_SAFE_RELEASE(SceneController::getInstance()->showBG);
            SceneController::getInstance()->showBG = NULL;
        }
        if (WorldMapView::instance()) {
            WorldMapView::instance()->removeCover();
        }
        auto scene = CCDirector::sharedDirector()->getRunningScene();
        auto texture = CCRenderTexture::create(CCDirector::sharedDirector()->getWinSize().width, CCDirector::sharedDirector()->getWinSize().height);
        texture->beginWithClear(0, 0, 0, 1.0);
        scene->visit();
        texture->end();
        SceneController::getInstance()->showBG = texture;
        SceneController::getInstance()->showBG->retain();
        
        auto serverInfo = _dict(params->objectForKey("serverInfo"));
        std::string ip = serverInfo->valueForKey("ip")->getCString();
        std::string zone = serverInfo->valueForKey("zone")->getCString();
        int port = serverInfo->valueForKey("port")->intValue();
        std::string gameUid = params->valueForKey("uid")->getCString();
        
        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, ip);
        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, zone);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT, port);
        CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, gameUid);
        CCUserDefault::sharedUserDefault()->flush();
        
        NetController::shared()->setIp(ip);
        NetController::shared()->setZone(zone);
        NetController::shared()->setPort(port);
        NetController::shared()->setIsLogout(true);
        NetController::shared()->disconnect();

        LogoutCommand* cmd = new LogoutCommand();
        CCDictionary* _params = CCDictionary::create();
        _params->setObject(CCString::create(zone), "zoneName");
        CCDictionary* dict = CCDictionary::create();
        dict->setObject(CCString::create("logout"), "cmd");
        dict->setObject(_params, "params");
        cmd->handleRecieve(dict);
    }
    return true;
}

