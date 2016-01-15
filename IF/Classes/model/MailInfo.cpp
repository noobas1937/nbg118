//
//  MailInfo.cpp
//  IF
//
//  Created by 李锐奇 on 13-10-11.
//
//

#include "MailInfo.h"
#include "LocalController.h"
#include "MailController.h"
#include "WorldController.h"
#include "MailDialogInfo.h"
#include "MailResourceInfo.h"
#include "RewardController.h"

MailInfo* MailInfo::create(){
    MailInfo* ret = new MailInfo();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void MailInfo::parse(cocos2d::CCDictionary *dic){
    saveDict=NULL;
    isShare=false;
    contents = "";
    ckfContents = "";
    pic = "";
    allianceLang = "";
    msReport = 0;
    picVer = 0;
    ckf = 0;
    isWorldBossKillRewardMail = false;
    if(dic->objectForKey("pic")){
        pic = dic->valueForKey("pic")->getCString();
    }
    
    if (dic->objectForKey("picVer"))
    {
        picVer = dic->valueForKey("picVer")->intValue();
    }
    type = dic->valueForKey("type")->intValue();
    //define tabType
    if(type==MAIL_BATTLE_REPORT||type==MAIL_DETECT||type==MAIL_DETECT_REPORT||type==MAIL_ENCAMP||type==MAIL_WORLD_BOSS){
        tabType = 4;
    }else if(type==ALL_SERVICE){
        tabType = 3;
    }else if(type== MAIL_FRESHER||type==MAIL_SYSNOTICE||type==MAIL_SYSUPDATE){
        tabType = 2;
        
    }else if(type <=MAIL_USER||type==MAIL_Alliance_ALL||type==CHAT_ROOM){
        tabType = 0;
    }else if(type == MAIL_MOD_SEND || type == MAIL_MOD_PERSONAL){
        if(GlobalData::shared()->playerInfo.gmFlag == 2 || GlobalData::shared()->playerInfo.gmFlag == 5){
            tabType = 5;
        }else{
            tabType = 0;
        }
    }else{
        tabType = 1;
    }
    if(dic->objectForKey("uid"))
        uid = dic->valueForKey("uid")->getCString();
    else
        uid="";
    createTime =  dic->valueForKey("createTime")->doubleValue() / 1000;
    CCLOGFUNCF("createTime: %ld",createTime);
    rewardId = dic->valueForKey("rewardId")->getCString();
    status = dic->valueForKey("status")->intValue();
    fromName = dic->valueForKey("oldName")->getCString();
    nowName = dic->valueForKey("fromName")->getCString();
    modLanguage = dic->valueForKey("modLanguage")->getCString();
    if(fromName.empty() && !nowName.empty()){
        fromName = nowName;
    }
    if(dic->objectForKey("fromUid")){
        fromUid = dic->valueForKey("fromUid")->getCString();
    }
    else if(dic->objectForKey("fromUser"))
    {
        fromUid = dic->valueForKey("fromUser")->getCString();
    }
    else{
        fromUid = "";
    }
    if(dic->objectForKey("share")){
        isShare = dic->valueForKey("share")->boolValue();
    }
    if (dic->objectForKey("donate")) {
        goToDonate = dic->valueForKey("donate")->intValue();
    }
    reply = dic->valueForKey("reply")->intValue();
    alliance = dic->valueForKey("alliance")->getCString();
    version = dic->valueForKey("version")->getCString();
    rewardStatus = dic->valueForKey("rewardStatus")->intValue();
    battleType = dic->valueForKey("battleType")->intValue();
    title = dic->valueForKey("title")->getCString();
    itemIdFlag = dic->valueForKey("itemIdFlag")->intValue();
    unreadDialogNum = dic->valueForKey("unread")->intValue();
    dialogNum = dic->valueForKey("totalNum")->intValue();
    userKill = dic->valueForKey("userKill")->intValue();
    allKill = dic->valueForKey("allKill")->intValue();
    msReport = dic->valueForKey("msReport")->intValue();
    failTimes = dic->valueForKey("failTimes")->intValue();
    winPercent = dic->valueForKey("winPercent")->intValue();
    killRound = dic->valueForKey("killRound")->intValue();
    userScore = dic->valueForKey("userScore")->intValue();
    monsterLevel = dic->valueForKey("monsterLevel")->intValue();
    like = dic->valueForKey("like")->intValue();
    if(dic->objectForKey("contents")){
        showContent = dic->valueForKey("contents")->getCString();
    }
    if(type <= MAIL_USER || type == MAIL_MOD_SEND || type == MAIL_MOD_PERSONAL || type == CHAT_ROOM){
        
    }else if(type == MAIL_RESOURCE_HELP){
        fromName = _lang("114121");
    }else if(type == MAIL_RESOURCE){
        fromName = _lang("105516");
        if(title == ""){
            title = "1,1";
        }
        vector<std::string> vector;
        CCCommonUtils::splitString(title, ",", vector);
        int level = 0;
        int type = 0;
        if(vector.size()>=2){
            level = atoi(vector[0].c_str());
            type = atoi(vector[1].c_str());
        }
        title = string("Lv.") + CC_ITOA(level) + " " + CCCommonUtils::getResourceProductByType(type);
        showContent = _lang_1("105517", title.c_str());
    }else if(type == MAIL_DETECT){
        if(itemIdFlag == 1){
            std::string contentId = dic->valueForKey("contents")->getCString();
            if(!contentId.empty() && contentId.length()>6){
                contentId = contentId.substr(6);
                if(contentId=="105554"){
                    fromName = _lang("105523");
                    title = showContent;
                }
            }
        }else{
            std::string contentId = dic->valueForKey("contents")->getCString();
    //        if(contentId=="105554"){
    //            fromName = _lang("105523");
    //            title = _lang("105554");
    //        }else{
    //            fromName = _lang("105522");
           // }
            fromName = _lang("105522");
            if(title.compare("1") == 0){
                title = _lang("105523");
            }else{
                title = _lang("105567");
            }
        }
    }
    else if(type == MAIL_DETECT_REPORT){
        std::string contentId = dic->valueForKey("contents")->getCString();
        if(contentId.compare("114005") == 0){
            pointType = MAIL_DETECT_REPORT_PROTECT;
            fromName = _lang("105522");
            title = _lang("114102");
            showContent = _lang("114005");
        }else{
            fromName = _lang("105522");
            //title = _lang("105523");
            showContent = "";
            switch (pointType) {
                case Throne:{ //王座
                    showContent = _lang("105708");
                    break;
                }
                case Trebuchet:{//投石机
                    showContent = _lang("105709");
                    break;
                }
                default:{
                    showContent = _lang("105527");
                    break;
                }
            }
        }
    }else if(type == MAIL_ENCAMP){
        fromName = _lang("105536");
        switch (pointType) {
            case Throne:{
                title = _lang("105710");
                break;
            }
            case Trebuchet:{
                title = _lang("105711");
                break;
            }
            default:{
                if(this->isTreasureMap){
                    fromName = _lang("111504");
                    title = _lang("111504");
                }else{
                    title = _lang("105537");
                }
                break;
            }
        }
    }else if(type == MAIL_WOUNDED){
        fromName = _lang("105305");
        title = _lang("105550");
    }else if(type == MAIL_BATTLE_REPORT){
        if(dic->objectForKey("contents")){
            auto tempDic = _dict(dic->objectForKey("contents"));
            if(tempDic!=NULL){
                int pos = tempDic->valueForKey("warPoint")->intValue();
                CCPoint pt = WorldController::getPointByIndex(pos);
                //fromName = _lang_2("105518",CC_ITOA(pt.x),CC_ITOA(pt.y));
                std::string npcId = "";
                if( tempDic->objectForKey("npcId"))
                    npcId= tempDic->valueForKey("npcId")->getCString();
                std::string defName ="";
                if(npcId!=""){
                    defName = CCCommonUtils::getNameById(tempDic->valueForKey("npcId")->getCString());
                }else{
                    if (tempDic->objectForKey("defName")) {
                        defName = tempDic->valueForKey("defName")->getCString();
                    }
                }
                std::string attName = tempDic->valueForKey("atkName")->getCString();
                
                if(GlobalData::shared()->playerInfo.name==attName){
                    title = _lang_2("105547",_lang("102187").c_str(),defName.c_str());
                }else{
                    title = _lang_2("105547",attName.c_str(),_lang("108678").c_str());
                }
                
                int win = tempDic->valueForKey("win")->intValue();
                std::string result = "";
                if(win==0){
                    result = _lang("105117");
                }else if(win==1){
                    result = _lang("105118");
                }else{
                    result = _lang("105549");
                }
                showContent = _lang_1("105548", result.c_str());
                fromName = result;
            }
        }
    }else if(type == MAIL_DIGONG || type == WORLD_NEW_EXPLORE){
        title = _lang("108675");
    }else if(type == MAIL_ATTACKMONSTER){
        fromName = _lang("103715");
    }else if(type==MAIL_Alliance_ALL || type == MAIL_GIFT){
    
    }else if(type == MAIL_DONATE){
        fromName = _lang("105305");
        std::string contentId = dic->valueForKey("contents")->getCString();
        vector<string> mVStr;
        CCCommonUtils::splitString(contentId, "|", mVStr);
        if(mVStr.size()>4){
            std::string userName = mVStr.at(1);
            if (!mVStr.at(2).empty()) {
                userName = "(" + mVStr.at(2) +")" + userName;
            }
            title = _lang_1("101007",userName.c_str());
            contents = _lang_3(mVStr.at(0), userName.c_str(), mVStr.at(3).c_str(), mVStr.at(4).c_str());
            showContent = contents;
        }
        itemIdFlag = 0;
    }else{
        fromName = _lang("105305");
    }
    
    prepareToDelete = false;
    save = dic->valueForKey("save")->intValue();
    isRead = false;
    if(itemIdFlag == 1){
        //杀死世界boss领奖邮件
        if (title=="137460") {
            type=MAIL_WORLD_BOSS;
            isWorldBossKillRewardMail = true;
        }
        title = _lang(title);
        
        string fromdl = dic->valueForKey("fromName")->getCString();
        if(fromdl != ""){
            string fromNL = _lang(fromdl);
            if(fromNL!=""){
                fromName = fromNL;
            }
        }
        if(showContent != ""){
            showContent = _lang(showContent);
        }
    }
    if(type == MAIL_FRESHER){
        fromName = _lang("3000001");
    }
//    std::string contentId = dic->valueForKey("contents")->getCString();
//    if(contentId=="114005"){
//        fromName = _lang("114102");
//        title = _lang("114005");
//    }else if(contentId!=""&&contentId.length()>6){
//        contentId = contentId.substr(6);
//        if(contentId=="105554"){
//            fromName = _lang("105523");
//            title = showContent;
//        }
//    }
    if(tabType==0 || type == MAIL_MOD_PERSONAL || type == MAIL_MOD_SEND || type==CHAT_ROOM){
        if(dialogs==NULL)
        {
            dialogs=CCArray::create();
            dialogs->retain();
        }
        contents = showContent;
        title = contents;
        if(dic->objectForKey("chats")){
            CCArray *arr = dynamic_cast<CCArray*>(dic->objectForKey("chats"));
            CCObject* obj;
            int count = arr->count();
            int i = 0;
            CCARRAY_FOREACH(arr, obj){
                CCDictionary* dictDialog = dynamic_cast<CCDictionary*>(obj);
                MailDialogInfo* dialogInfo = MailDialogInfo::create();
                dialogInfo->parse(dictDialog);
                this->dialogs->addObject(dialogInfo);
                if(i==count-1){
                    title = dialogInfo->contents;
                }
                i++;
            }
        }
    }else{
        dialogs=NULL;
    }
    if(type==MAIL_ALLIANCEINVITE){
        title = _lang_1("103739", alliance.c_str());
        fromName = dic->valueForKey("fromName")->getCString();
    }else if(type==MAIL_ALLIANCEAPPLY){
        //todo
        fromName = dic->valueForKey("fromName")->getCString();
    }
    if (type == MAIL_INVITE_TELEPORT) {
        fromName = dic->valueForKey("fromName")->getCString();
        if (pic == "" || pic == "0") {
            pic = "g044";
        }
        title = _lang(title);
    }
    if (type == MAIL_ALLIANCE_KICKOUT || type == MAIL_REFUSE_ALL_APPLY) {
        fromName = dic->valueForKey("fromName")->getCString();
        if (pic == "" || pic == "0") {
            pic = "g044";
        }
        title = dic->valueForKey("title")->getCString();
        title = _lang(title);
    }
    
    
    preSaveContent="";
    attUser = NULL;
    defUser = NULL;
    atkGen = NULL;
    defGen = NULL;
    atkWarEffect = NULL;
    dfWarEffect = NULL;
    reward = NULL;
    winner = "";
    resLevel = 0;
    deal = -1;
    attReport = NULL;
    defReport = NULL;
    trainGenerals = NULL;
    trainGeneralExp = NULL;
    detectReport = NULL;
    occupyGeneral = NULL;
    isReadContent = false;
    mazeReward = NULL;
    mazeSodiler = NULL;
    monsters = NULL;

    atkAlliance = NULL;
    defAlliance = NULL;
    atkArmyTotal = NULL;
    defArmyTotal = NULL;
    atkHelpReport = NULL;
    defHelpReport = NULL;
    atkHelper = NULL;
    defHelper = NULL;
    checkMail = false;
    shareTime = 0;
    shareUid = "";
    isFromChat = false;
    ckfWarInfo = NULL;
    isShowDefEndTxt = false;
}

void MailInfo::parseBattleMail(CCDictionary* dict){
    checkMail = true;
    auto addGeneral = [](CCArray *arr) {
        CCArray *ret = CCArray::create();
        CCObject *obj;
        CCARRAY_FOREACH(arr, obj){
            GeneralInfo *info = new GeneralInfo(dynamic_cast<CCDictionary*>(obj));
            ret->addObject(info);
            CC_SAFE_RELEASE(info);
        }
        ret->retain();
        return ret;
    };
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
    }
    if(dict->objectForKey("atkWarEffect")){
        atkWarEffect = dynamic_cast<CCArray*>(dict->objectForKey("atkWarEffect"));
        atkWarEffect->retain();
    }
    if(dict->objectForKey("dfWarEffect")){
        dfWarEffect = dynamic_cast<CCArray*>(dict->objectForKey("dfWarEffect"));
        dfWarEffect->retain();
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
    }else{
        attUser = CCDictionary::create();
        attUser->retain();
    }
    if(dict->objectForKey("warPoint")){
        int pos = dict->valueForKey("warPoint")->intValue();
        warPoint = WorldController::getPointByIndex(pos);
    }
    if(dict->objectForKey("defUser")){
        defUser = dynamic_cast<CCDictionary*>(dict->objectForKey("defUser"));
        defUser->retain();
    }else{
        defUser = CCDictionary::create();
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
    createTime =  dict->valueForKey("createTime")->doubleValue() / 1000;
    if(dict->objectForKey("ckfWarInfo")){
        ckfWarInfo = dynamic_cast<CCDictionary*>(dict->objectForKey("ckfWarInfo"));
        ckfWarInfo->retain();
    }
    if(dict->objectForKey("ckf")){
        ckf = dict->valueForKey("ckf")->intValue();
    }
}

MailInfo::~MailInfo(){

    if(detectReport){
        detectReport->removeAllObjects();
        CC_SAFE_RELEASE(detectReport);
        detectReport = NULL;
    }
    if(attUser){
        CC_SAFE_RELEASE(attUser);
        attUser = NULL;
    }
    if(defUser){
        CC_SAFE_RELEASE(defUser);
        defUser = NULL;
    }
    if(defAlliance){
        CC_SAFE_RELEASE(defAlliance);
        defAlliance = NULL;
    }
    if(atkAlliance){
        CC_SAFE_RELEASE(atkAlliance);
        atkAlliance = NULL;
    }
    if(atkHelpReport){
        CC_SAFE_RELEASE(atkHelpReport);
        atkHelpReport = NULL;
    }
    if(defHelpReport){
        CC_SAFE_RELEASE(defHelpReport);
        defHelpReport = NULL;
    }
    if(atkHelper){
        CC_SAFE_RELEASE(atkHelper);
        atkHelper = NULL;
    }
    if(defHelper){
        CC_SAFE_RELEASE(defHelper);
        defHelper = NULL;
    }
    if(atkArmyTotal){
        CC_SAFE_RELEASE(atkArmyTotal);
        atkArmyTotal = NULL;
    }
    if(defArmyTotal){
        CC_SAFE_RELEASE(defArmyTotal);
        defArmyTotal = NULL;
    }
    if(trainGenerals){
        trainGenerals->removeAllObjects();
        CC_SAFE_RELEASE(trainGenerals);
        trainGenerals = NULL;
    }
    if(trainGeneralExp){
        trainGeneralExp->removeAllObjects();
        CC_SAFE_RELEASE(trainGeneralExp);
        trainGeneralExp = NULL;
    }
    if(attReport){
        attReport->removeAllObjects();
        CC_SAFE_RELEASE(attReport);
    }
    if(defReport){
        defReport->removeAllObjects();
        CC_SAFE_RELEASE(defReport);
        defReport = NULL;
    }
    if(atkGen){
        atkGen->removeAllObjects();
        CC_SAFE_RELEASE(atkGen);
        atkGen = NULL;
    }
    if(defGen){
        defGen->removeAllObjects();
        CC_SAFE_RELEASE(defGen);
        defGen = NULL;
    }
    if(reward){
        reward->removeAllObjects();
        CC_SAFE_RELEASE(reward);
        reward = NULL;
    }
    if(occupyGeneral){
        occupyGeneral->removeAllObjects();
        CC_SAFE_RELEASE(occupyGeneral);
        occupyGeneral = NULL;
    }
    if(mazeReward){
        mazeReward->removeAllObjects();
        CC_SAFE_RELEASE(mazeReward);
        mazeReward = NULL;
    }
    if(mazeSodiler){
        CC_SAFE_RELEASE(mazeSodiler);
        mazeSodiler = NULL;
    }
    if(monsters){
        monsters->removeAllObjects();
        CC_SAFE_RELEASE(monsters);
        monsters = NULL;
    }
    if(dialogs){
        dialogs->removeAllObjects();
        CC_SAFE_RELEASE(dialogs);
        dialogs = NULL;
    }
    if(ckfWarInfo){
        CC_SAFE_RELEASE(ckfWarInfo);
        ckfWarInfo = NULL;
    }
//    if(crMemberUidArray)
//    {
//        crMemberUidArray->removeAllObjects();
//        crMemberUidArray->release();
//        crMemberUidArray=NULL;
//    }
}
