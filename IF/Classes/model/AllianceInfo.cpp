//
//  AllianceInfo.cpp
//  IF
//
//  Created by ganxiaohua on 14-3-24.
//
//

#include "AllianceInfo.h"
#include "ScienceController.h"
#include "UIComponent.h"
#include "ChatController.h"
#include "ChatServiceCocos2dx.h"

void AllianceInfo::updateAllianceInfo(CCDictionary* dict){
    if(dict==NULL) return;
    if(createTime==0){
         createTime = GlobalData::shared()->getTimeStamp();
    }
    if(dict==NULL) return ;
    if(dict->objectForKey("alliancename")){
        name = dict->valueForKey("alliancename")->getCString();
    }
    if(dict->objectForKey("fightpower")){
        fightpower = (long)dict->valueForKey("fightpower")->doubleValue();
    }
    if(dict->objectForKey("giftexp1")){
        giftexp1 = dict->valueForKey("giftexp1")->intValue();
    }
    if(dict->objectForKey("giftexp2")){
        giftexp2 = dict->valueForKey("giftexp2")->intValue();
    }
    if(dict->objectForKey("giftexp3")){
        giftexp3 = dict->valueForKey("giftexp3")->intValue();
    }
    if(dict->objectForKey("giftexp4")){
        giftexp4 = dict->valueForKey("giftexp4")->intValue();
    }
    if(dict->objectForKey("giftexp5")){
        giftexp5 = dict->valueForKey("giftexp5")->intValue();
    }
    if(dict->objectForKey("giftexp6")){
        giftexp6 = dict->valueForKey("giftexp6")->intValue();
    }
    if(dict->objectForKey("uid")){
        uid = dict->valueForKey("uid")->getCString();
    }
    if(dict->objectForKey("abbr")){
        shortName = dict->valueForKey("abbr")->getCString();
    }
    if(dict->objectForKey("language")){
        language = dict->valueForKey("language")->getCString();
    }
    if(dict->objectForKey("curMember")){
        currentNum = dict->valueForKey("curMember")->intValue();
    }
    if(dict->objectForKey("icon")){
        icon = dict->valueForKey("icon")->getCString();
    }
    if(dict->objectForKey("applied")){
        isApplied = dict->valueForKey("applied")->intValue();
    }
    if(dict->objectForKey("learderName")){
        leader = dict->valueForKey("learderName")->getCString();
    }
    if(dict->objectForKey("learderUid")){
        leaderUid = dict->valueForKey("learderUid")->getCString();
    }
    if(dict->objectForKey("country")){
        country = dict->valueForKey("country")->getCString();
    }
    if(dict->objectForKey("recruit")){
        recruit = dict->valueForKey("recruit")->intValue();
    }
    
    rallyPoint = 0;
    if(dict->objectForKey("rallyPoint")){
        rallyPoint = dict->valueForKey("rallyPoint")->intValue();
    }
    if(dict->objectForKey("leaderPoint")){
        leaderPoint = dict->valueForKey("leaderPoint")->intValue();
    }
    
    if(dict->objectForKey("rank")){
        rank = dict->valueForKey("rank")->intValue();
    }
    
    if(dict->objectForKey("giftlevel")){
        giftlevel = dict->valueForKey("giftlevel")->intValue();
    }
    if(dict->objectForKey("giftlevel")){
        currentGiftExp = dict->valueForKey("giftlevel")->intValue();
    }
    if(dict->objectForKey("currentlevel")){
        currentGiftLevel = dict->valueForKey("currentlevel")->intValue();
    }
    if(dict->objectForKey("nextlevel")){
        giftNextExp = dict->valueForKey("nextlevel")->intValue();
    }
    if(dict->objectForKey("intro")){
        intro = dict->valueForKey("intro")->getCString();
    }
    if(dict->objectForKey("point")){
        point = dict->valueForKey("point")->intValue();
    }
    if(dict->objectForKey("accPoint")){
        accPoint = dict->valueForKey("accPoint")->intValue();
    }
    if(dict->objectForKey("donateAvailable")){
        donateAvailable = dict->valueForKey("donateAvailable")->intValue();
    }
    if(dict->objectForKey("donateCDTime")){
        int tmpT = dict->valueForKey("donateCDTime")->doubleValue()/1000;
        if (tmpT>0) {
            donateCDTime = GlobalData::shared()->changeTime(tmpT);
        }
        else {
            donateCDTime = 0;
        }
    }
    if(dict->objectForKey("jointime")){
        int tmpT = dict->valueForKey("jointime")->doubleValue()/1000;
        if (tmpT>0) {
            joinTime = GlobalData::shared()->changeTime(tmpT);
        }
        else {
            joinTime = 0;
        }
    }
    if(dict->objectForKey("alliancepoint")){
        alliancepoint = dict->valueForKey("alliancepoint")->intValue();
    }
    if(dict->objectForKey("announce")){
        announce = dict->valueForKey("announce")->getCString();
    }
    if(dict->objectForKey("fightpower")){
        totalForce = (unsigned long)dict->valueForKey("fightpower")->doubleValue();
    }
    if(dict->objectForKey("maxMember")){
        maxNum = dict->valueForKey("maxMember")->intValue();
    }
    if(dict->objectForKey("abbrRename")){
        abbrRename = dict->valueForKey("abbrRename")->intValue();
    }
    if(dict->objectForKey("helpcount")){
        helpcount = dict->valueForKey("helpcount")->intValue();
    }
    if(dict->objectForKey("reicon")){
        reicon = dict->valueForKey("reicon")->intValue();
    }
    if(dict->objectForKey("rankone")){
        strRank1 = dict->valueForKey("rankone")->getCString();
    }
    if(dict->objectForKey("ranktwo")){
        strRank2 = dict->valueForKey("ranktwo")->getCString();
    }
    if(dict->objectForKey("rankthree")){
        strRank3 = dict->valueForKey("rankthree")->getCString();
    }
    if(dict->objectForKey("rankfour")){
        strRank4 = dict->valueForKey("rankfour")->getCString();
    }
    if(dict->objectForKey("rankfive")){
        strRank5 = dict->valueForKey("rankfive")->getCString();
    }
    if(dict->objectForKey("territory")) {
        CCArray* tNameArr = dynamic_cast<CCArray*>(dict->objectForKey("territory"));
        if (tNameArr) {
            int num = tNameArr->count();
            for (int i = 0; i<num; i++) {
                auto dic = _dict(tNameArr->objectAtIndex(i));
                if (dic->objectForKey("uid")) {
                    std::string tuid = dic->valueForKey("uid")->getCString();
                    std::string tname = dic->valueForKey("name")->getCString();
                    int index = dic->valueForKey("count")->intValue();
                    if (tname == "") {
                        tname = _lang_1("115312", CC_ITOA(index));
                    }
                    territoryNameMap[index] = make_pair(tuid, tname);
                }
            }
        }
    }
    if(dict->objectForKey("militaryNum")){
        militaryNum = dict->valueForKey("militaryNum")->intValue();
    }
    if(dict->objectForKey("applyCount")){
        applyNum = dict->valueForKey("applyCount")->intValue();
    }
    if(dict->objectForKey("allianceScience") && uid == GlobalData::shared()->playerInfo.allianceInfo.uid){
        CCArray* arr = dynamic_cast<CCArray*>(dict->objectForKey("allianceScience"));
        ScienceController::getInstance()->initAllianceScienceData(arr);
    }
    if (dict->objectForKey("serverId")) {
        serverId = dict->valueForKey("serverId")->intValue();
    }
    if (dict->objectForKey("powerRestriction")) {
        powerRestriction = dict->valueForKey("powerRestriction")->intValue();
    }
    if (dict->objectForKey("castleRestriction")) {
        castleRestriction = dict->valueForKey("castleRestriction")->intValue();
    }
}

void AllianceInfo::reset(){
    name = "";
    fightpower = 0;
    uid = "";
    shortName = "";
    language = "";
    currentNum = 0;
    icon = "";
    isApplied = "";
    leader = "";
    leaderUid = "";
    country = "";
    recruit = 1;
    rank = 0;
    intro = "";
    announce = "";
    totalForce = 0;
    maxNum = 0;
    helpcount = 0;
    createTime = 0;
    serverId = -1;
    castleRestriction = 0;
    powerRestriction = 0;
    territoryNameMap.clear();
    ScienceController::getInstance()->onQuitAlliance();
    UIComponent::getInstance()->changeChatChannel(0);//世界频道
    ChatController::getInstance()->m_chatAlliancePool.clear();//清空联盟聊天记录
    ChatController::getInstance()->m_latestAllianceMsg.sequenceId=0;
    GlobalData::shared()->playerInfo.isfirstJoin = 0;//退出联盟时，将记录为0
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::resetPlayerFirstJoinAlliance();
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    ChatServiceCocos2dx::resetPlayerIsInAlliance();
#endif
}

std::string AllianceInfo::getAllianceIcon(){
//    return "Allance_flay.png";
    std::string allianceIcon = "";
    if(icon==""){
        allianceIcon = "Allance_flay.png";
    }else{
        allianceIcon = icon;
        allianceIcon.append(".png");
    }
    return allianceIcon;
}

int AllianceInfo::getSelfAllianceNum(){
    int num = maxNum;
    if(GlobalData::shared()->playerInfo.isInAlliance() && uid == GlobalData::shared()->playerInfo.allianceInfo.uid){
        num = maxNum + CCCommonUtils::getEffectValueByNum(ALLIANCE_MEMBER_MAX_FACTOR);
    }
    return num;
}