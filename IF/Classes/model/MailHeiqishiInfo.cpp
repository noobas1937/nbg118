//
//  MailHeiqishiInfo.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/10/28.
//
//

#include "MailHeiqishiInfo.hpp"
#include "GeneralInfo.h"
#include "WorldController.h"
#include "RewardController.h"
MailHeiqishiInfo* MailHeiqishiInfo::create(){
    MailHeiqishiInfo* pret = new MailHeiqishiInfo();
    if(pret){
        pret->init();
        pret->autorelease();
    }else{
        CC_SAFE_DELETE(pret);
    }
    return pret;
}
//
void MailHeiqishiInfo::parse(CCDictionary *dict){
    uid = dict->valueForKey("uid")->getCString();
    createTime =  dict->valueForKey("createTime")->doubleValue()/1000;
    
    auto addGeneral = [](CCArray *arr) {
        CCArray *ret = CCArray::create();
        CCObject *obj;
        if (arr!=NULL) {
            CCARRAY_FOREACH(arr, obj){
                GeneralInfo *info = new GeneralInfo(dynamic_cast<CCDictionary*>(obj));
                if(info)
                {
                    ret->addObject(info);
                }
                info->release();
            }
        }
        ret->retain();
        return ret;
    };
    if(dict->objectForKey("serverType")){
        serverType = dict->valueForKey("serverType")->intValue();
    }
    if(dict->objectForKey("isResourceShieldState")){
        isResourceShieldState = dict->valueForKey("isResourceShieldState")->boolValue();
    }else{
        isResourceShieldState = false;
    }
    
    if(dict->objectForKey("atkArmyTotal")){
        atkArmyTotal = dynamic_cast<CCDictionary*>(dict->objectForKey("atkArmyTotal"));
        atkArmyTotal->retain();
    }
    if(dict->objectForKey("defArmyTotal")){
        defArmyTotal = dynamic_cast<CCDictionary*>(dict->objectForKey("defArmyTotal"));
        defArmyTotal->retain();
    }
    if(dict->objectForKey("atkAlliance")){
        atkAlliance = dynamic_cast<CCDictionary*>(dict->objectForKey("atkAlliance"));
        atkAlliance->retain();
    }
    if(dict->objectForKey("defAlliance")){
        defAlliance = dynamic_cast<CCDictionary*>(dict->objectForKey("defAlliance"));
        defAlliance->retain();
    }
    if(dict->objectForKey("atkHelpReport")){
        atkHelpReport = dynamic_cast<CCArray*>(dict->objectForKey("atkHelpReport"));
        atkHelpReport->retain();
    }
    if(dict->objectForKey("defHelpReport")){
        defHelpReport = dynamic_cast<CCArray*>(dict->objectForKey("defHelpReport"));
        defHelpReport->retain();
    }
    if(dict->objectForKey("atkHelper")){
        atkHelper =dynamic_cast<CCArray*>(dict->objectForKey("atkHelper"));
        atkHelper->retain();
    }
    if(dict->objectForKey("defHelper")){
        defHelper =dynamic_cast<CCArray*>(dict->objectForKey("defHelper"));
        defHelper->retain();
    }
    if(dict->objectForKey("defGen")){
        defGen = addGeneral(dynamic_cast<CCArray*>(dict->objectForKey("defGen")));
        atkGen = addGeneral(dynamic_cast<CCArray*>(dict->objectForKey("atkGen")));
        attReport = dynamic_cast<CCArray*>(dict->objectForKey("atkReport"));
        attReport->retain();
        defReport = dynamic_cast<CCArray*>(dict->objectForKey("defReport"));
        defReport->retain();
        if(dict->objectForKey("defProtectActivate")){
            isShowDefEndTxt = dict->valueForKey("defProtectActivate")->boolValue();//是否显示 “由于防守方等级差距过大，战斗提前结束”cell
        }else{
            isShowDefEndTxt = false;
        }
    }
    if(dict->objectForKey("defFortLost")){
        auto forts = dynamic_cast<CCArray*>(dict->objectForKey("defFortLost"));
        if(forts){
            forts->retain();
        }
        defFortLost = forts;
    }
    if(dict->objectForKey("atkUser")){
        attUser = dynamic_cast<CCDictionary*>(dict->objectForKey("atkUser"));
        attUser->retain();
    }
    if(dict->objectForKey("remainResource")){
        defRemainRes = dynamic_cast<CCDictionary*>(dict->objectForKey("remainResource"));
        defRemainRes->retain();
    }
    if(dict->objectForKey("warPoint")){
        int pos = dict->valueForKey("warPoint")->intValue();
//        if (serverType>=SERVER_BATTLE_FIELD) {
//            warPoint = WorldController::getPointByMapTypeAndIndex(pos,(MapType)serverType);
//        }else{
//            warPoint = WorldController::getPointByIndex(pos);
//        } simon
    }
    if(dict->objectForKey("defUser")){
        defUser = dynamic_cast<CCDictionary*>(dict->objectForKey("defUser"));
        defUser->retain();
    }
    if(dict->objectForKey("reward")){
        reward = dynamic_cast<CCArray*>(dict->objectForKey("reward"));
        if(reward){
            reward->retain();
        }else{
            std::string rewardStr = dict->valueForKey("reward")->getCString();
            if(rewardStr!=""){
                reward = RewardController::getInstance()->getBossReward(rewardStr);
            }
        }
    }
    if(dict->objectForKey("atkWarEffect")){
        atkWarEffect = dynamic_cast<CCArray*>(dict->objectForKey("atkWarEffect"));
        atkWarEffect->retain();
    }
    if(dict->objectForKey("dfWarEffect")){
        dfWarEffect = dynamic_cast<CCArray*>(dict->objectForKey("dfWarEffect"));
        dfWarEffect->retain();
    }
    if(dict->objectForKey("atkPowerLost")){
        atkPowerLost = dict->valueForKey("atkPowerLost")->intValue();
    }
    if(dict->objectForKey("defPowerLost")){
        defPowerLost = dict->valueForKey("defPowerLost")->intValue();
    }
    if(reward==NULL){
        CCArray* rewardArr = CCArray::create();
        rewardArr->retain();
        reward = rewardArr;
    }
    if(dict->objectForKey("winner")){
        winner = dict->valueForKey("winner")->getCString();
    }
    if(dict->objectForKey("level")){
        resLevel = dict->valueForKey("level")->intValue();
    }
    if(dict->objectForKey("pointId")){
        pointId = dict->valueForKey("pointId")->intValue();
    }
    if(dict->objectForKey("battleType")){
        battleType = dict->valueForKey("battleType")->intValue();
    }
    if(dict->objectForKey("reportUid")){
        reportUid = dict->valueForKey("reportUid")->getCString();
    }
    if(dict->objectForKey("defTowerKill")){
        auto towers = dynamic_cast<CCArray*>(dict->objectForKey("defTowerKill"));
        if(towers){
            towers->retain();
        }
        defTowerKill = towers;
    }
    if(dict->objectForKey("atkGenKill")){
        CCArray* arr =  dynamic_cast<CCArray*>(dict->objectForKey("atkGenKill"));
        arr->retain();
        atkGenKill = arr;
    }
    if(dict->objectForKey("defGenKill")){
        CCArray* arr =  dynamic_cast<CCArray*>(dict->objectForKey("defGenKill"));
        arr->retain();
        defGenKill = arr;
    }
    CCString *str = dynamic_cast<CCString*>(dict->objectForKey("pointType"));
    if(str && str->getCString()){
        pointType =  str->intValue();
    }
    
    if(dict->objectForKey("userKill")){
        userKill = dict->valueForKey("userKill")->intValue();
    }
    if(dict->objectForKey("allKill")){
        allKill = dict->valueForKey("allKill")->intValue();
    }
    if(dict->objectForKey("msReport")){
        msReport = dict->valueForKey("msReport")->intValue();
    }
    if(dict->objectForKey("failTimes")){
        failTimes = dict->valueForKey("failTimes")->intValue();
    }
    if(dict->objectForKey("winPercent")){
        winPercent = dict->valueForKey("winPercent")->intValue();
    }
    if(dict->objectForKey("userScore")){
        userScore = dict->valueForKey("userScore")->intValue();
    }
    if(dict->objectForKey("killRound")){
        killRound = dict->valueForKey("killRound")->intValue();
    }
    if(dict->objectForKey("monsterLevel")){
        monsterLevel = dict->valueForKey("monsterLevel")->intValue();
    }
    if(dict->objectForKey("ckfWarInfo")){
        ckfWarInfo = dynamic_cast<CCDictionary*>(dict->objectForKey("ckfWarInfo"));
        ckfWarInfo->retain();
    }
    if (dict->objectForKey("ckf")) {
        ckf = dict->valueForKey("ckf")->intValue();
    }
    if(dict->objectForKey("serverType")){
        serverType = dict->valueForKey("serverType")->intValue();
    }
}

MailHeiqishiInfo::~MailHeiqishiInfo(){
}