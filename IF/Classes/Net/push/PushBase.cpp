//
//  PushBase.cpp
//  IF
//
//  Created by zhaohf on 13-9-6.
//
//

#include "PushBase.h"
#include "CommonInclude.h"
#include "ResourcesPush.h"
#include "PushInit.h"
#include "ChatPush.h"
#include "UserDataUpdatePush.h"
#include "MailPush.h"
#include "AdRewardPush.h"
#include "QuestDataPush.h"
#include "EnergyPush.h"
#include "AchievementPush.h"
#include "EnemyReportPush.h"
#include "WorldMarchPush.h"
#include "ArmyFoodPush.h"
#include "BeenAttackedPush.h"
#include "TavernPush.h"
#include "UIComponent.h"
#include "PlayerInfoPush.h"
#include "ActivityPush.h"
#include "ActivityScorePush.h"
#include "EmailConfirmPush.h"
#include "AllianceJoinPush.h"
#include "AllianceLeavelPush.h"
#include "DefencePush.h"
#include "HospitalReceivePush.h"
#include "GeneralPush.h"
#include "ItemPush.h"
#include "ArmyOpenPush.h"
#include "FortOpenPush.h"
#include "PayPush.h"
#include "ExplorePush.h"
#include "GeneralUpdateDataPush.h"
#include "ArmyInfoPush.h"
#include "PushAllianceUpdate.h"
#include "AlliacnePushHelpUpdate.h"
#include "AlliacnePushHelpNew.h"
#include "AllianceDismissPush.h"
#include "PushAllianceLeaderTrans.h"
#include "PushPlayerAllianceRank.h"
#include "PushAllianceFightBulletin.h"
#include "PushAllianceMarchSpd.h"
#include "PushAllianceNum.h"
#include "WorldRefresh.h"
#include "AllianceShopPush.h"
#include "GoldExchangePush.h"
#include "SystemUpdatePush.h"
#include "UpBuildingsPush.h"
#include "ResourceSpeedPush.h"
#include "AllianceEventPush.h"
#include "MineHelpPush.h"
#include "MineCallHelpPush.h"
#include "WorldActivityPush.h"
#include "PushKingdomName.h"
#include "PushKingdomKing.h"
#include "PushKingdomThroneOwne.h"
#include "PushKingdomThroneOwnerCountdown.h"
#include "PushKingdomWinner.h"
#include "MerchantePush.h"
#include "PushKingdomBanner.h"
#include "PushKingdomThronePic.h"
#include "PushKingdomRes.h"
#include "AchieveCompletePush.h"
#include "StaminaRollbackPush.h"
#include "TerritoryInformationPush.h"
#include "MonsterAttackPush.h"
#include "CustomPicPush.h"
#include "ChatRoomMsgPush.h"
#include "WorldMarchCompetePush.h"
#include "DailyActivePush.h"
#include "TerritoryResourcePush.h"
#include "PushLeaveCrossServiceActivity.h"
#include "TerritoryTowerPush.h"
#include "PushCKFTaskComplete.h"
#include "TerritoryWarehousePush.h"
#include "ChristmasPush.h"
#include "CKFWarInfoPush.h"
#include "CKFWorldWarInfoPush.h"
#include "AllianceApplyNumPush.h"
#include "AllianceApplyHandleNumPush.h"
#include "CKFWarWinEndPush.h"
#include "PushAssetsUpdate.h"
#include "PushCommonNotice.h"
#include "TerritoryBannerPush.h"
void PushBase::handleResponse(cocos2d::CCDictionary *dict)
{
    string cmd = dict->valueForKey("cmd")->getCString();
    if (GlobalData::shared()->isPause) {
        if(cmd == "push.pay"){
            if(GlobalData::shared()->pushData != NULL)
                GlobalData::shared()->pushData->release();
            GlobalData::shared()->pushData = dict;
            dict->retain();
        }
        return;
    }
    if (!GlobalData::shared()->isInitFinish && (cmd != "init" && cmd != "init.error")) {
        return;
    }
    if(cmd == "init.error"){
        PushInitError::handleResponse(dict);
    }else if (cmd == "init")
        PushInit::handleResponse(dict);
    else if (cmd == "push.city.refresh")
        ResourcesPush::handleResponse(dict);
    else if (cmd == "push.chat")
        ChatPush::handleResponse(dict);
    else if(cmd=="push.user.update"){
        UserDataUpdatePush::handleResponse(dict);
    }else if(cmd=="push.mail"){
        MailPush::handleResponse(dict);
    }else if(cmd=="push.adreward"){
        AdRewardPush::handleResponse(dict);
    }else if(cmd=="task.push"){
        QuestDataPush::handleResponse(dict);
    }else if(cmd=="lord.recover.energy"){
        EnergyPush::handleResponse(dict);
    }else if(cmd=="achieve.push"){
        AchievementPush::handleResponse(dict);
    }
    else if(cmd=="push.enemy.report"){
        EnemyReportPush::handleResponse(dict);
    }
    else if(cmd=="push.world.march"){
        WorldMarchPush::handleResponse(dict);
    }
    else if(cmd=="push.world.march.win"){
        WorldMarchWinPush::handleResponse(dict);
    }
    else if(cmd=="push.world.march.fail"){
        WorldMarchLosePush::handleResponse(dict);
    }
    else if(cmd=="push.monster.siege.def.win"){//defence win
        WorldMarchLosePush::handleResponse(dict);
    }
    else if(cmd=="push.monster.siege.def.fail"){//defence failure
        WorldMarchWinPush::handleResponse(dict);
    }
    else if(cmd=="push.world.march.spd"){
        WorldMarchSpeedUpPush::handleResponse(dict);
    }
    else if(cmd=="push.world.march.retreat"){
        WorldMarchCancelPush::handleResponse(dict);
    }
    else if(cmd == "army.food"){
        ArmyFoodPush::handleResponse(dict);
    }
    else if(cmd=="push.world.mv") {
        WorldTeleportPush::handleResponse(dict);
    }
    else if(cmd == "push.territory.create"){
        WorldAllianceAreaPush::handleResponse(dict);
    }
    else if(cmd == "push.territory.cancel"){
        WorldAllianceAreaCancelPush::handleResponse(dict);
    }
    else if (cmd == "push.territory.update"){
        TerritoryInformationPush::handleResponse(dict);
    }
    else if("push.territory.resource.create" == cmd){
        WolrdSuperMinePush::handleResponse(dict);
    }
    else if("push.territory.resource.cancel" == cmd){
        WolrdSuperMineCancelPush::handleResponse(dict);
    }
    else if (cmd == "push.territory.resource.update"){
        TerritoryResourcePush::handleResponse(dict);
    }
    else if("push.territory.turret.create" == cmd){
        if (GlobalData::shared()->alliance_territory_tower_switch == 1) {
            WolrdTowerBuildPush::handleResponse(dict);
        }
    }
    else if("push.territory.turret.cancel" == cmd){
        if (GlobalData::shared()->alliance_territory_tower_switch == 1) {
            WolrdTowerBuildCancelPush::handleResponse(dict);
        }
    }
    else if (cmd == "push.territory.turret.update"){
        if (GlobalData::shared()->alliance_territory_tower_switch == 1) {
            TerritoryTowerPush::handleResponse(dict);
        }
    }
    else if("push.territory.flag.create" == cmd){
        if (GlobalData::shared()->alliance_territory_banner_switch == 1) {
            WolrdBannerBuildPush::handleResponse(dict);
        }
    }
    else if("push.territory.flag.cancel" == cmd){
        if (GlobalData::shared()->alliance_territory_banner_switch == 1) {
            WolrdBannerBuildCancelPush::handleResponse(dict);
        }
    }
    else if (cmd == "push.territory.flag.update"){
        if (GlobalData::shared()->alliance_territory_banner_switch == 1) {
            TerritoryBannerPush::handleResponse(dict);
        }
    }
    else if (cmd == "push.territory.flag.crash"){
        if (GlobalData::shared()->alliance_territory_banner_switch == 1) {
            WolrdBannerBuildCrachPush::handleResponse(dict);
        }
    }
    else if ("push.territory.warehouse.update" == cmd){
        TerritoryWarehousePush::handleResponse(dict);
    }
    else if ("push.territory.warehouse.create" == cmd){
        WolrdWareHousePush::handleResponse(dict);
    }
    else if ("push.territory.warehouse.cancel" == cmd){
        WolrdWareHouseCancelPush::handleResponse(dict);
    }
    else if ("push.warehouse.takeback" == cmd){
        WorldWareHouseTakeBackPush::handleResponse(dict);
    }
    else if ("push.warehouse.store" == cmd){
        WorldWareHouseStorePush::handleResponse(dict);
    }
    else if ("push.warehouse.retrieve" == cmd){
        WorldWareHouseRetrievePush::handleResponse(dict);
    }
    else if (cmd == "push.world.march.compete"){
        WorldMarchCompetePush::handleResponse(dict);
    }
    else if(cmd=="push.world.user.crash") {
        WorldCrashPush::handleResponse(dict);
    }
    else if(cmd=="push.world.injured") {
        BeenAttackedPush::handleResponse(dict);
    }
    else if(cmd == "push.task.complete"){
        QuestDataPush::handleResponse(dict);
    }
    else if(cmd == "push.task.new"){
        QuestDataPush::handleResponse(dict);
    }
    else if(cmd == "tavern.push"){
        TavernPush::handleResponse(dict);
    }
    else if(cmd == "world.refresh"){
        WorldRefreshPush::handleResponse(dict);
    }else if(cmd == "player.info"){
        PlayerInfoPush::handleResponse(dict);
    }
    else if(cmd == "push.world.relic.reset.time"){
        WorldRelicUpdatePush::handleResponse(dict);
    }
    else if(cmd == "push.world.relic.finish"){
        WorldRelicFinishPush::handleResponse(dict);
    }
    else if(cmd == "push.world.tower"){
        WorldTowerPush::handleResponse(dict);
    }
    else if(cmd == "act.score.gcm"){
        ActivityPush::handleResponse(dict);
    }
    else if(cmd == "push.score.update"){
        ActivityScorePush::handleResponse(dict);
    }
    else if(cmd == "push.stamina.rollback"){
        StaminaRollbackPush::handleResponse(dict);
    }
    else if(cmd == "push.bs")
    {
        EmailConfirmPush::handleResponse(dict);
    }
    else if(cmd == "push.al.join")
    {
        AllianceJoinPush::handleResponse(dict);
    }
    else if(cmd == "push.al.update")
    {
        PushAllianceUpdate::handleResponse(dict);
    }
    else if(cmd == "push.al.leave")
    {
        AllianceLeavelPush::handleResponse(dict);
    }
    else if(cmd == "defence.push")
    {
        DefencePush::handleResponse(dict);
    }
    else if(cmd == "hospital.receive"){
        HospitalReceivePush::handleResponse(dict);
    }
    else if(cmd == "genInfo.push"){
        GeneralPush::handleResponse(dict);
    }
    else if(cmd == "push.item.add"){
        ItemAddPush::handleResponse(dict);
    }
    else if(cmd == "push.item.del"){
        ItemDelPush::handleResponse(dict);
    }
    else if(cmd == "push.world.firework"){
        ItemUsePush::handleResponse(dict);
    }
    else if(cmd == "army.open"){
        ArmyOpenPush::handleResponse(dict);
    }
    else if(cmd == "fort.open"){
        FortOpenPush::handleResponse(dict);
    }
    else if(cmd == "push.pay"){
        PayPush::handleResponse(dict);
    }
    else if(cmd == "push.world.explore"){
        ExplorePush::handleResponse(dict);
    }else if(cmd == "army.up"){
        GeneralUpdateDataPush::handleResponse(dict);
    }else if(cmd == "army.info"){
        ArmyInfoPush::handleResponse(dict);
    }
    else if(cmd == "push.server.stop")
    {
        ServerStop::handleResponse(dict);
    }
    else if(cmd == "push.al.help.update")
    {
        AlliacnePushHelpUpdate::handleResponse(dict);
    }
    else if(cmd == "push.al.help.new")
    {
        AlliacnePushHelpNew::handleResponse(dict);
    }
    else if(cmd == "push.al.dismiss")
    {
        AllianceDismissPush::handleResponse(dict);
    }
    else if(cmd == "push.al.leader.trans")
    {
        PushAllianceLeaderTrans::handleResponse(dict);
    }else if(cmd == "player.rank")
    {
        PushPlayerAllianceRank::handleResponse(dict);
    }
    else if(cmd == "push.al.science")
    {
        PushAllianceScience::handleResponse(dict);
    }
    else if(cmd == "user.same")
    {
        PushUserOff::handleResponse(dict);
    }
    else if(cmd == "push.user.off"){
        PushUserKick::handleResponse(dict);
    }
    else if (cmd == "push.user.gm.off")
    {
        PushUserGmOff::handleResponse(dict);
    }
    else if(cmd == "push.al.fight.bulletin")
    {
        PushAllianceFightBulletin::handleResponse(dict);
    }
    else if(cmd == "push.alliance.march.spd")
    {
        PushAllianceMarchSpd::handleResponse(dict);
    }
    else if(cmd == "push.alliance.num")
    {
        PushAllianceNum::handleResponse(dict);
    }
    else if(cmd == "push.alliance.apply")
    {
        AllianceApplyNumPush::handleResponse(dict);
    }
    else if (cmd == "push.alliance.apply.handle")
    {
        AllianceApplyHandleNumPush::handleResponse(dict);
    }
    else if(cmd == "push.world.al.flush")
    {
        WorldAllianceRefreshPush::handleResponse(dict);
    }else if(cmd == "al.shop.update")
    {
        AllianceShopPush::handleResponse(dict);
    }else if(cmd == "al.point.push")
    {
        AlliancePointPush::handleResponse(dict);
    }else if(cmd == "exchange.new"){
    
        GoldExchangePush::handleResponse(dict);
    }
    else if(cmd == "push.server.tostop")
    {
        SystemUpdatePush::handleResponse(dict);
    }
    else if(cmd == "resource.time.refresh")
    {
        ResBuildingsPush::handleResponse(dict);
    }
    else if(cmd == "push.world.res.spd")
    {
        ResourceSpeedPush::handleResponse(dict);
    }else if(cmd == "push.al.event")
    {
        AllianceEventPush::handleResponse(dict);
    }else if(cmd == "push.chat.ban"){
        ChatBanPush::handleResponse(dict);
    }else if(cmd == "push.world.get"){
        WorldProtectUpdatePush::handleResponse(dict);
    }else if(cmd == "push.world.resource.prt"){
        WorldResourceProtectUpdatePush::handleResponse(dict);
    }else if(cmd == "mine.help.push"){
        MineHelpPush::handleResponse(dict);
    }else if(cmd == "mine.call.push"){
        MineCallHelpPush::handleResponse(dict);
    }else if(cmd == "push.act.time"){
        WorldActivityPush::handleResponse(dict);
    }else if(cmd == "push.sock.new"){
        //不仅仅是圣诞节使用, 许愿中的活动开启功能也用到,请勿删除
        ChristmasPush::handleResponse(dict);
    }else if(cmd == "push.kingdom.name"){
        PushKingdomName::handleResponse(dict);
    }else if(cmd == "push.world.march.del"){
        WorldMarchDeletePush::handleResponse(dict);
    }else if(cmd == "push.kingdom.trebuchet.att"){
        WorldTrebuchetAttPush::handleResponse(dict);
    }else if(cmd == "push.world.march.add"){
        WorldMarchAddPush::handleResponse(dict);
    }else if(cmd == "push.kingdom.officer.cls"){
        OfficerClearPush::handleResponse(dict);
    }else if(cmd == "push.kingdom.officer.add"){
        OfficerAddPush::handleResponse(dict);
    }else if(cmd == "push.kingdom.king"){
        PushKingdomKing::handleResponse(dict);
    }else if(cmd == "push.kingdom.throne.owner"){
        PushKingdomThroneOwne::handleResponse(dict);
    }else if(cmd == "push.kingdom.throne.owner.countdown"){
        PushKingdomThroneOwnerCountdown::handleResponse(dict);
    }else if(cmd == "push.kingdom.winner"){
        PushKingdomWinner::handleResponse(dict);
    }else if(cmd == "push.businessman.refresh"){
        MerchantePush::handleResponse(dict);
    }else if(cmd == "push.kingdom.throne.pic"){
        PushKingdomThronePic::handleResponse(dict);
    }else if(cmd == "push.kingdom.banner"){
        PushKingdomBanner::handleResponse(dict);
    }else if(cmd == "push.kingdom.res"){
        PushKingdomRes::handleResponse(dict);
    }else if(cmd == "push.achieve.complete"){
        AchieveCompletePush::handleResponse(dict);
    }else if(cmd == "push.monster.attack"){
        MonsterAttackPush::handleResponse(dict);
    }else if (cmd == "push.picVer.update"){
        CustomPicPush::handleResponse(dict);
    }else if (cmd == "push.daily.quest"){
        DailyActivePush::handleResponse(dict);
    }
    else if (cmd == "push.chat.room.msg"){
        ChatRoomMsgPush::handleResponse(dict);
    }else if (cmd == "push.leave.ckf"){
        PushLeaveCrossServiceActivity::handleResponse(dict);
    }else if (cmd == "push.ckf.task.complete"){
        PushCKFTaskComplete::handleResponse(dict);
    }else if (cmd == "push.rally.point"){
        AlliacneMarkLinePush::handleResponse(dict);
    }else if (cmd == "push.win.streak"){
        CKFWorldWarInfoPush::handleResponse(dict);
    }else if (cmd == "push.streak.all"){
        CKFWarInfoPush::handleResponse(dict);
    }else if (cmd == "push.world.use"){
        WorldAddItemPush::handleResponse(dict);
    }else if (cmd == "shut.down.streak"){
        CKFWarWinEndPush::handleResponse(dict);
    }else if (cmd == "push.assets.update"){
        PushAssetsUpdate::handleResponse(dict);
    }else if (cmd == "push.common.notice"){
        PushCommonNotice::handleResponse(dict);
    }
    
}