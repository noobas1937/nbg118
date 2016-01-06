//
//  AllianceManager.cpp
//  IF
//
//  Created by ganxiaohua on 14-4-3.
//
//

#include "AllianceManager.h"
#include "AllianceCallHelpCommand.h"
#include "AllianceHelpAllCommand.h"
#include "YuanJunTipView.h"
#include "PopupViewController.h"
#include "WorldController.h"
#include "FunBuildController.h"
#include "AllianceInfoMembersView.h"
#include "AllianceMassTeamView.h"
#include "BattleView.h"
#include "AllianceResourceTradeView.h"
#include "SoundController.h"
#include "InviteTeleportCommand.h"
#include "MailController.h"
#include "GetAllianceListCommand.h"
#include "SceneController.h"
#include "UIComponent.h"
#include "AllianceInfoView.h"
#include "ChatController.h"
#include "QueueController.h"

static AllianceManager* _instance;

AllianceManager* AllianceManager::getInstance(){
    if(!_instance){
        _instance = new AllianceManager();
        _instance->init();
    }
    return _instance;
}

void AllianceManager::addAnnounceEvent(){
    map<string, AllianceEventInfo* >::iterator it = AllianceManager::getInstance()->eventsMap.find(ALLIANCE_NOTICE_KEY);
    if(it!=AllianceManager::getInstance()->eventsMap.end()){
        it->second->createTime = GlobalData::shared()->getTimeStamp();
        it->second->content = GlobalData::shared()->playerInfo.allianceInfo.announce;
    }else{
        AllianceEventInfo* allianceEvent = AllianceEventInfo::create();
        allianceEvent->retain();
        allianceEvent->createTime = GlobalData::shared()->playerInfo.allianceInfo.createTime;
        allianceEvent->content = GlobalData::shared()->playerInfo.allianceInfo.announce;
        allianceEvent->type = 5;
        eventsMap[ALLIANCE_NOTICE_KEY] = allianceEvent;
    }
}

void AllianceManager::init(){
    this->addAnnounceEvent();
    this->goToWorldType = 0;
    this->tmpMailUid = "";
    this->notUpDate = 0;
    this->tmpCityIndex = 0;
    auto group = LocalController::shared()->DBXMLManager()->getGroupByKey("rank");
    CCDictElement *ele;
    CCDICT_FOREACH(group, ele){
        CCDictionary *dict = dynamic_cast<CCDictionary*>(ele->getObject());
        int allmail = dict->valueForKey("allmail")->intValue();
        int member = dict->valueForKey("member")->intValue();
        int trade = dict->valueForKey("trade")->intValue();
        int help = dict->valueForKey("help")->intValue();
        int rankdetails = dict->valueForKey("rankdetails")->intValue();
        int slogan = dict->valueForKey("slogan")->intValue();
        int recruitment = dict->valueForKey("recruitment")->intValue();
        int name = dict->valueForKey("name")->intValue();
        int briefname = dict->valueForKey("briefname")->intValue();
        int flag = dict->valueForKey("flag")->intValue();
        int rankname = dict->valueForKey("rankname")->intValue();
        int announcement = dict->valueForKey("announcement")->intValue();
        int invite = dict->valueForKey("invite")->intValue();
        int dissolve = dict->valueForKey("dissolve")->intValue();
        int quit = dict->valueForKey("quit")->intValue();
        int kick = dict->valueForKey("kick")->intValue();
        int transfer = dict->valueForKey("transfer")->intValue();
        int rankup = dict->valueForKey("rankup")->intValue();
        int rankdown = dict->valueForKey("rankdown")->intValue();
        int alliancescience = dict->valueForKey("alliancescience")->intValue();
        int allianceShop = dict->valueForKey("allianceshop")->intValue();
        int allianceLanguage = dict->valueForKey("alliancelanguage")->intValue();
        int allianceActivity = dict->valueForKey("allianceactivity")->intValue();
        int comment = dict->valueForKey("comment")->intValue();
        int quest = dict->valueForKey("quest")->intValue();
        int id = dict->valueForKey("id")->intValue();
        
        switch (id) {
            case 7105:
                v_rank5.push_back(rankname);
                v_rank5.push_back(flag);
                v_rank5.push_back(briefname);
                v_rank5.push_back(name);
                v_rank5.push_back(dissolve);
                v_rank5.push_back(transfer);
                v_rank5.push_back(kick);
                v_rank5.push_back(slogan);
                v_rank5.push_back(recruitment);
                v_rank5.push_back(invite);
                v_rank5.push_back(announcement);
                v_rank5.push_back(allianceLanguage);
                v_rank5.push_back(allianceShop);
                v_rank5.push_back(alliancescience);
                v_rank5.push_back(allianceActivity);
                v_rank5.push_back(comment);
                v_rank5.push_back(rankdown);
                v_rank5.push_back(rankup);
                v_rank5.push_back(quit);
                v_rank5.push_back(rankdetails);
                v_rank5.push_back(help);
                v_rank5.push_back(trade);
                v_rank5.push_back(member);
                v_rank5.push_back(allmail);
                v_rank5.push_back(quest);
                break;
            case 7104:
                v_rank4.push_back(rankname);
                v_rank4.push_back(flag);
                v_rank4.push_back(briefname);
                v_rank4.push_back(name);
                v_rank4.push_back(dissolve);
                v_rank4.push_back(transfer);
                v_rank4.push_back(kick);
                v_rank4.push_back(slogan);
                v_rank4.push_back(recruitment);
                v_rank4.push_back(invite);
                v_rank4.push_back(announcement);
                v_rank4.push_back(allianceLanguage);
                v_rank4.push_back(allianceShop);
                v_rank4.push_back(alliancescience);
                v_rank4.push_back(allianceActivity);
                v_rank4.push_back(comment);
                v_rank4.push_back(rankdown);
                v_rank4.push_back(rankup);
                v_rank4.push_back(quit);
                v_rank4.push_back(rankdetails);
                v_rank4.push_back(help);
                v_rank4.push_back(trade);
                v_rank4.push_back(member);
                v_rank4.push_back(allmail);
                v_rank4.push_back(quest);
                break;
            case 7103:
                v_rank3.push_back(rankname);
                v_rank3.push_back(flag);
                v_rank3.push_back(briefname);
                v_rank3.push_back(name);
                v_rank3.push_back(dissolve);
                v_rank3.push_back(transfer);
                v_rank3.push_back(kick);
                v_rank3.push_back(slogan);
                v_rank3.push_back(recruitment);
                v_rank3.push_back(invite);
                v_rank3.push_back(announcement);
                v_rank3.push_back(allianceLanguage);
                v_rank3.push_back(allianceShop);
                v_rank3.push_back(alliancescience);
                v_rank3.push_back(allianceActivity);
                v_rank3.push_back(comment);
                v_rank3.push_back(rankdown);
                v_rank3.push_back(rankup);
                v_rank3.push_back(quit);
                v_rank3.push_back(rankdetails);
                v_rank3.push_back(help);
                v_rank3.push_back(trade);
                v_rank3.push_back(member);
                v_rank3.push_back(allmail);
                v_rank3.push_back(quest);
                break;
            case 7102:
                v_rank2.push_back(rankname);
                v_rank2.push_back(flag);
                v_rank2.push_back(briefname);
                v_rank2.push_back(name);
                v_rank2.push_back(dissolve);
                v_rank2.push_back(transfer);
                v_rank2.push_back(kick);
                v_rank2.push_back(slogan);
                v_rank2.push_back(recruitment);
                v_rank2.push_back(invite);
                v_rank2.push_back(announcement);
                v_rank2.push_back(allianceLanguage);
                v_rank2.push_back(allianceShop);
                v_rank2.push_back(alliancescience);
                v_rank2.push_back(allianceActivity);
                v_rank2.push_back(comment);
                v_rank2.push_back(rankdown);
                v_rank2.push_back(rankup);
                v_rank2.push_back(quit);
                v_rank2.push_back(rankdetails);
                v_rank2.push_back(help);
                v_rank2.push_back(trade);
                v_rank2.push_back(member);
                v_rank2.push_back(allmail);
                v_rank2.push_back(quest);
                break;
            case 7101:
                v_rank1.push_back(rankname);
                v_rank1.push_back(flag);
                v_rank1.push_back(briefname);
                v_rank1.push_back(name);
                v_rank1.push_back(dissolve);
                v_rank1.push_back(transfer);
                v_rank1.push_back(kick);
                v_rank1.push_back(slogan);
                v_rank1.push_back(recruitment);
                v_rank1.push_back(invite);
                v_rank1.push_back(announcement);
                v_rank1.push_back(allianceLanguage);
                v_rank1.push_back(allianceShop);
                v_rank1.push_back(alliancescience);
                v_rank1.push_back(allianceActivity);
                v_rank1.push_back(comment);
                v_rank1.push_back(rankdown);
                v_rank1.push_back(rankup);
                v_rank1.push_back(quit);
                v_rank1.push_back(rankdetails);
                v_rank1.push_back(help);
                v_rank1.push_back(trade);
                v_rank1.push_back(member);
                v_rank1.push_back(allmail);
                v_rank1.push_back(quest);
                break;
            default:
                break;
        }
    }
}

void AllianceManager::callHelp(int qid){
    if(GlobalData::shared()->allQueuesInfo.find(qid) != GlobalData::shared()->allQueuesInfo.end()){
        QueueInfo& info = GlobalData::shared()->allQueuesInfo[qid];
        GlobalData::shared()->allQueuesInfo[qid].canHelp = false;
        AllianceCallHelpCommand* cmd = new AllianceCallHelpCommand(info.uuid,info.type,CC_ITOA(info.itemId));
        cmd->sendAndRelease();
        CCCommonUtils::flyHint("", "", _lang("115095"));
    }
}

void AllianceManager::sendHelpAllToServer(){
    AllianceHelpAllCommand* cmd = new AllianceHelpAllCommand();
    cmd->sendAndRelease();
    GlobalData::shared()->playerInfo.allianceInfo.helpcount = 0;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIACNE_HELP_NUM_CHANGE);
    CCCommonUtils::flyHint("", "", _lang("115215"));
}

void AllianceManager::openYuanYunView(int pointId){
    unsigned int current =  WorldController::getInstance()->getCurrentMarchCount();
    unsigned int max = WorldController::getInstance()->getMaxMarchCount();
    if (current >= max) {
        WorldController::getInstance()->showMarchAlert(max);
        return;
    }
    if(pointId<=0) return;
    CCCommonUtils::flyHint("", "", _lang("E100008"));return;//fusheng 12.29 屏蔽士兵援助
    
    PopupViewController::getInstance()->addPopupView(YuanJunTipView::create(pointId));
}

void AllianceManager::openMarketView(){
    int buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_TAVERN);
    bool haveMarket = false;
    if (buildId>0) {
        haveMarket = true;
    }
    if(haveMarket){
        PopupViewController::getInstance()->addPopupInView(AllianceInfoMembersView::create(GlobalData::shared()->playerInfo.allianceInfo.uid));
    }else{
        CCCommonUtils::flyHint("", "", _lang("E100008"));
//        CCCommonUtils::flyHint("", "", _lang("115075"));//fusheng 资源帮助提示
    }
}

void AllianceManager::openAllianceTradeView(std::string uid,int mainLV,int pointId){
    int buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_TAVERN);
    bool haveMarket = false;
    if (buildId>0) {
        haveMarket = true;
    }
    if(haveMarket){
        AllianceResourceTradeView::createResurceTradeByUid(uid,pointId,mainLV);
    }else{
        CCCommonUtils::flyHint("", "", _lang("E100008"));
//        CCCommonUtils::flyHint("", "", _lang("115075"));//fusheng 资源帮助提示
    }
}

void AllianceManager::openAllianceMassView(int pointId){
    int buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_SMITHY);
    bool flag = false;
    if (buildId>0) {
        flag = true;
    }
    if(flag){
        PopupViewController::getInstance()->addPopupView(AllianceMassTeamView::create(pointId));
        SoundController::sharedSound()->playEffects(Music_Sfx_world_click_attack);
    }else{
        CCCommonUtils::flyHint("", "", _lang("115127"));
    }
}

void AllianceManager::onAttackView(int pointId){
    WorldController::getInstance()->openMarchDeploy(pointId,1,0,MethodBattle);
}

bool AllianceManager::checkRight(RankType type,int rank){
    bool flag = false;
    switch (rank) {
        case 5:
            flag = v_rank5[type]==1;
            break;
        case 4:
            flag = v_rank4[type]==1;
            break;
        case 3:
            flag = v_rank3[type]==1;
            break;
        case 2:
            flag = v_rank2[type]==1;
            break;
        case 1:
            flag = v_rank1[type]==1;
            break;
    }
    return flag;
}

void AllianceManager::parseEvents(CCDictionary* dic){
    
}

void AllianceManager::clearEvents(){
    map<std::string, AllianceEventInfo*>::iterator it;
    for(it = eventsMap.begin(); it != eventsMap.end(); it++){
        it->second->release();
    }
    eventsMap.clear();
}

void AllianceManager::startInviteTeleport(string targetUid, string allianceId, int cityIndex)
{
    if (targetUid == "" || allianceId == "") {
        return;
    }
    if (allianceId != GlobalData::shared()->playerInfo.allianceInfo.uid) {
        return;
    }
    int rank = GlobalData::shared()->playerInfo.allianceInfo.rank;
    if (rank < 3) {
        return;
    }
    InviteTeleportCommand* cmd = new InviteTeleportCommand(targetUid, allianceId, cityIndex);
    cmd->sendAndRelease();
}

void AllianceManager::endInviteTeleport()
{
    CCCommonUtils::flyHint("", "", _lang("115288"));
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_INVITE_TELEPORT_SEND_SUCCESS);
}

void AllianceManager::endTeleport(string mailUid)
{
    map<string, MailInfo*>::iterator it;
    it = GlobalData::shared()->mailList.find(mailUid);
    if (it != GlobalData::shared()->mailList.end()) {
        MailInfo* info = it->second;
        if (info->type != MAIL_INVITE_TELEPORT) {
            return;
        }
        info->inviteTeleDeal = 1;
    }
}

bool sortItem(AllianceInfo* itemId1,AllianceInfo* itemId2){
    if(itemId1->recruit==itemId2->recruit)
    {
        return (itemId1->totalForce > itemId2->totalForce);
    }
    else
    {
        return (itemId1->recruit > itemId2->recruit);
    }
    return false;
}

void AllianceManager::starGetRecAlliance()
{
    if (!CCCommonUtils::isTestPlatformAndServer("guide_4")) {
        return;
    }
    if (GlobalData::shared()->playerInfo.isInAlliance()) {
        return;
    }
    if (FunBuildController::getInstance()->getMainCityLv() < 3) {
        return;
    }
    if (recallianceArray.size()>0) {
        return;
    }
    
    GetAllianceListCommand* cmd = new GetAllianceListCommand("",1,2);
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceManager::endGetRecAlliance), NULL));
    cmd->sendAndRelease();
}

void AllianceManager::endGetRecAlliance(CCObject* data)
{
    recAllianceIdx = 0;
    NetResult* result = dynamic_cast<NetResult*>(data);
    auto params = _dict(result->getData());
    CCArray* arr =  (CCArray*)params->objectForKey("list");
    int m_total_receive = params->valueForKey("total")->intValue();
    
    recallianceArray.clear();
    int num = arr->count();
    for (int i=0; i<num; i++) {
        CCDictionary* dicAlliance = (CCDictionary*)arr->objectAtIndex(i);
        AllianceInfo* alliance = new AllianceInfo();
        alliance->updateAllianceInfo(dicAlliance);
        recallianceArray.push_back(alliance);
    }
    if(m_total_receive != 0){
        sort(recallianceArray.begin(), recallianceArray.end(), sortItem);
    }
}

void AllianceManager::showRecAlliance()
{
    if (!CCCommonUtils::isTestPlatformAndServer("guide_4")) {
        return;
    }
    if (QueueController::getInstance()->ShowRecAllianceTime>0) {
        return;
    }
    if (GlobalData::shared()->playerInfo.isInAlliance()) {
        return;
    }
    if (FunBuildController::getInstance()->getMainCityLv() < 3) {
        return;
    }
    if (SceneController::getInstance()->currentSceneId!=SCENE_ID_MAIN) {
        return;
    }
    if (recallianceArray.size()==0) {
        starGetRecAlliance();
        return;
    }
    
    //显示
    UIComponent::getInstance()->showAllianceTip();
    QueueController::getInstance()->ShowRecAllianceTime = 10*60;//10分钟
}

void AllianceManager::joinAllianceSuccess(CCObject* obj){
    if(GlobalData::shared()->playerInfo.isInAlliance()){
        CCCommonUtils::setIsHDViewPort(true);
        PopupViewController::getInstance()->addPopupInView(AllianceInfoView::create(&GlobalData::shared()->playerInfo.allianceInfo));
    }
    //发聊天消息，加入联盟
    ChatController::getInstance()->sendCountryChat(_lang("115188").append("  (").append(_lang("115181")).append(")").c_str(), CHAT_ALLIANCE, 2);
    
}

void AllianceManager::startGetAllianceUrl(string uuid)
{
    GetAllianceUrlCommand* cmd = new GetAllianceUrlCommand(uuid);
    cmd->sendAndRelease();
}

void AllianceManager::endGetAllianceUrl(CCDictionary* dic)
{
    string uuid = dic->valueForKey("uuid")->getCString();
    string playerId = dic->valueForKey("uid")->getCString();
    string pic = dic->valueForKey("pic")->getCString();
    string name = dic->valueForKey("name")->getCString();
    int picVer = 0;
    picVer = dic->valueForKey("picVer")->intValue();
    
    if (picVer !=0 ) {
        string url = CCCommonUtils::getCustomPicUrl(playerId, picVer);
        recAllianceUrlMap[uuid] = url;
        recAllianceNameMap[uuid] = name;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIANCE_HEAD_UPDATE, CCString::create(url));
    }else {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIANCE_HEAD_UPDATE, CCString::create(pic));
    }
}