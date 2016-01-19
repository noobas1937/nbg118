//
//  QuestController.cpp
//  IF
//
//  Created by ganxiaohua on 13-11-4.
//
//

#include "QuestController.h"
#include "GlobalData.h"
#include "CCSafeNotificationCenter.h"
#include "QuestRewardCommand.h"
#include "FunBuildController.h"
#include "ScienceController.h"
#include "PopupViewController.h"
#include "AchieveFinishView.h"
#include "GuideController.h"
#include "UserUpgradeView.h"
#include "SceneController.h"
#include "ImperialScene.h"
#include "WorldMapView.h"
#include "UIComponent.h"
#include "AppLibHelper.h"
#include "FBUtilies.h"
#include "FiveStarTaskView.h"
#include "GeneralTitanPopupView.h"

static QuestController* _instance;

QuestController *QuestController::getInstance(){
    if(_instance == NULL){
        _instance = new QuestController();
    }
    return _instance;
}
void QuestController::purgeData() {
    delete _instance;
    _instance = NULL;
}
QuestController::QuestController()
{
    m_init = true;
    m_returnFromWorld = false;
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(QuestController::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(QuestController::delayPopUpUerUpdate), User_LV_REWARD_DELAY, NULL);
}

QuestController::~QuestController() {
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(QuestController::onEnterFrame), this);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, User_LV_REWARD_DELAY);
}
void QuestController::delayPopUpUerUpdate(CCObject* p){
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(QuestController::popUPUserUPdateView), this, 1.5,1, 0.0f, false);

}
void QuestController::popUPUserUPdateView(float t){
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(QuestController::popUPUserUPdateView), this);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_PLAYER_LEVELUP);
}
void QuestController::onEnterFrame(float t){
    if(currentAchList.size()<=0) return ;//|| m_init
    map<std::string, QuestInfo*>::iterator it;
    std::string openId = "";
    for (it=currentAchList.begin(); it!=currentAchList.end(); it++) {
        if(it->second->state==COMPLETE && !it->second->haveOpen){
            if(openId==""){
                openId = it->second->itemId;
                break;
            }else{
                if(it->second->guide==""){
                    openId = it->second->itemId;
                    break;
                }
            }
        }
    }
//    auto node = PopupViewController::getInstance()->getCurrentPopupView();
//    if(openId!="" && PopupViewController::getInstance()->CanPopPushView && node==NULL){
//        PopupViewController::getInstance()->addPopupInView(AchieveFinishView::create(openId));
//        std::string guideId = "";
//        map<std::string, QuestInfo*>::iterator it = QuestController::getInstance()->achList.find(openId);
//        if(it!=QuestController::getInstance()->achList.end()){
//            guideId = it->second->guide;
//        }
//        if(guideId!=""){
//            GuideController::share()->setGuide(guideId);
//        }
//    }
}

void QuestController::postQuestNumChangeNotification(){
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(QUEST_NUM_CHANGE);
}

void QuestController::postQuestStateChangeNotification(){
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(QUEST_STATE_UPDATE);
}

void QuestController::initQuest(CCArray* showTask){
    config.clear();
    isGettingReward = false;
    m_init = false;
    auto group = LocalController::shared()->DBXMLManager()->getGroupByKey("quest");

    CCDictElement *ele;
    CCDICT_FOREACH(group, ele){
        std::string itemId = ele->getStrKey();
        CCDictionary *dict = dynamic_cast<CCDictionary*>(ele->getObject());
        QuestInfo* quest = QuestInfo::create();
        quest->retain();
        quest->parse(dict);
        achList[itemId] = quest;
        map<int, int>::iterator it = achMap.find(quest->type);
        if(it!=achMap.end()){
            it->second = it->second+1;
        }else{
            achMap[quest->type] = 1;
        }
    }
    if(showTask){
        for (int i=0; i<showTask->count(); i++) {
            auto  dic = _dict(showTask->objectAtIndex(i));
            std::string itemId = dic->valueForKey("id")->getCString();
            map<std::string, QuestInfo*>::iterator it = achList.find(itemId);
            if(it!=achList.end()){
                it->second->parseServerDic(dic);
                it->second->maxStar = achMap[it->second->type];
                currentAchList[itemId] = it->second;
            }
        }
    }
    //resetBuildQuest();

//    resetResearchQuest();
}

void QuestController::initCKFTask(CCArray* showTask){
    if(showTask){
        for (int i=0; i<showTask->count(); i++) {
            auto  dic = _dict(showTask->objectAtIndex(i));
            std::string itemId = dic->valueForKey("id")->getCString();
            map<std::string, QuestInfo*>::iterator it = achList.find(itemId);
            if(it!=achList.end()){
                it->second->parseServerDic(dic);
                it->second->maxStar = achMap[it->second->type];
                currentCKFList[itemId] = it->second;
            }
        }
    }
}

void QuestController::updateTask(CCArray* tasks){
    if(tasks){
        std::string curItemId = "";
        for (int i=0; i<tasks->count(); i++) {
            auto  dic = _dict(tasks->objectAtIndex(i));
            std::string itemId = dic->valueForKey("id")->getCString();
            curItemId = itemId;
            map<std::string, QuestInfo*>::iterator it = currentAchList.find(itemId);
            if(it!=currentAchList.end()){
                int beforeS = it->second->state;
                it->second->parseServerDic(dic);
                int afterS = it->second->state;
                if (beforeS==0 && afterS==1) {
                    //完成一个任务;
                    if (it->second->guide != "") {
                        GuideController::share()->willGuide(it->second->guide);
                    }
                }
            }else{
                it = achList.find(itemId);
                if(it!=achList.end()){
//                    int idKey = dic->valueForKey("id")->intValue();
//                    int type1 = (idKey/100)%100;
//                    map<std::string, QuestInfo*>::iterator sameIt;
//                    std::string sameId = "";
//                    for (sameIt=currentAchList.begin(); sameIt!=currentAchList.end(); sameIt++) {
//                        if(sameIt->second->maxtype==type1){
//                            sameId = sameIt->second->itemId;
//                            break;
//                        }
//                    }
//                    if(sameId!=""){
//                        currentAchList.erase(sameId);
//                    }
                    
                    int beforeS = it->second->state;
                    it->second->parseServerDic(dic);
                    map<int, int>::iterator itNum = achMap.find(it->second->type);
                    if(itNum!=achMap.end()){
                        it->second->maxStar = itNum->second;
                    }
                    currentAchList[itemId] = it->second;
                    
                    int afterS = it->second->state;
                    if (beforeS==0 && afterS==1) {
                        //完成一个任务;
                        if (it->second->guide != "") {
                            GuideController::share()->willGuide(it->second->guide);
                        }
                    }
                    
//                    if( itemId == "2900101" && currentAchList[itemId]->state==ACCEPT) {
//                        if (PopupViewController::getInstance()->getCurrViewCount()<=0) {
//                            PopupViewController::getInstance()->addPopupView(FiveStarTaskView::create());
//                        }
//                        else {
//                            PopupViewController::getInstance()->pushPop(FiveStarTaskView::create());
//                        }
//                    }
                }
            }
        }
        
        if (currentAchList.find(curItemId)!=currentAchList.end() && currentAchList[curItemId]->state == COMPLETE) {
            auto node = PopupViewController::getInstance()->getCurrentPopupView();
            UserUpgradeView* view = dynamic_cast<UserUpgradeView*>(node);
            if (view || node==NULL)
            {
                std::string guideId = "";
                map<std::string, QuestInfo*>::iterator it = QuestController::getInstance()->achList.find(curItemId);
                if(it!=QuestController::getInstance()->achList.end()){
                    guideId = it->second->guide;
                }
                if(guideId!="")
                {
                    GuideController::share()->setGuide(guideId);
                    if (view)
                    {
                        view->retain();
                        PopupViewController::getInstance()->removePopupView(view);
                        PopupViewController::getInstance()->pushPop(view, true);
                        CC_SAFE_RELEASE(view);
                    }
                }
            }
        }
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(QUEST_STATE_UPDATE);
    }
}

void QuestController::updateCKFTask(CCArray* tasks){
    if(tasks){
        std::string curItemId = "";
        for (int i=0; i<tasks->count(); i++) {
            auto  dic = _dict(tasks->objectAtIndex(i));
            std::string itemId = dic->valueForKey("id")->getCString();
            curItemId = itemId;
            map<std::string, QuestInfo*>::iterator it = currentAchList.find(itemId);
            it = achList.find(itemId);
            if(it!=achList.end()){
                int beforeS = it->second->state;
                it->second->parseServerDic(dic);
                map<int, int>::iterator itNum = achMap.find(it->second->type);
                if(itNum!=achMap.end()){
                    it->second->maxStar = itNum->second;
                }
                currentCKFList[itemId] = it->second;
            }
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(QUEST_STATE_UPDATE);
    }
}

void QuestController::addQuest(std::string itemId, int state, int type){
    QuestInfo *info = NULL;
    if(type == QUEST_BUILDING_AUTO){
        info = getQuestBuilding(atoi(itemId.c_str()), state);
    }else if(type == QUEST_SCIENCE_AUTO){
//        info = getQuestBuilding(atoi(itemId.c_str()), state);
    }else{
        info = QuestInfo::create();
        info->itemId = itemId;
        info->state = state;
        info->type = type;
        int recommed = atoi(CCCommonUtils::getPropById(itemId, "recommend").c_str());
        info->recomment = recommed;
        info->name = CCCommonUtils::getNameById(itemId).c_str();
        info->target = _lang(CCCommonUtils::getPropById(itemId, "target").c_str());
        info->description = _lang(CCCommonUtils::getPropById(itemId, "description").c_str());
    }
    info->retain();
    questList[itemId] = info;
}

void QuestController::removeQuest(std::string itemId, bool postNotification){
    QuestInfo *info = getQuestFormMapById(itemId);
    if(info){
        CC_SAFE_RELEASE(info);
        questList.erase(itemId);
        buildQuestList.erase(itemId);
        researchQuestList.erase(itemId);
        if(postNotification){
            postQuestNumChangeNotification();
        }
    }
}

bool QuestController::onCheckSpeTask(std::string itemId)
{
    bool st = false;
    map<std::string, QuestInfo*>::iterator it;
    std::string openId = "";
    for (it=currentAchList.begin(); it!=currentAchList.end(); it++) {
        if(it->second->state==ACCEPT && itemId == it->second->itemId){
                st = true;
                break;
        }
    }
    
    if (st) {
        if (itemId == "2900101") {
            return true;
        }else if (itemId == "2900102") {
            return true;
        }else {
            return false;
        }
    } else {
        return false;
    }
}

void QuestController::getRewardSuccessHandle(std::string itemId){
    map<std::string, QuestInfo*>::iterator it = achList.find(itemId);
    if( it!=achList.end() )// && it->second->level!=it->second->maxStar
    {
        if (itemId == "2100101") {
            if (PopupViewController::getInstance()->getCurrViewCount() == 0) {
                UIComponent::getInstance()->CheckGuideUIShow();
            }
            AppLibHelper::sendAdjustTrack("tutorial_over");
            AppLibHelper::triggerEventCompletedTutorial();
        }
        currentAchList.erase(itemId);
    }
    map<std::string, QuestInfo*>::iterator it2 = currentCKFList.find(itemId);
    if (it2 != currentCKFList.end()) {
        currentCKFList.erase(it2);
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(QUEST_STATE_UPDATE);
}

void QuestController::updateQuestState(std::string itemId, int state, bool postNotification){
    QuestInfo *info = getQuestFormMapById(itemId);
    if(info){
        int preState = info->state;
        info->state = state;
        if(info->type == QUEST_BUILDING_AUTO){
            resetBuildQuest();
            QuestInfo *info = getQuestFormMapById(itemId);
            buildQuestList.erase(itemId);
            if(info){
                info->state = state;
                questList[itemId] = info;
            }
        }else if(info->type == QUEST_SCIENCE_AUTO){
//            researchQuestList.erase(itemId);
//            questList[itemId] = info;
//            info->retain();
//            resetResearchQuest();
        }
        if(postNotification && state != preState){
            postQuestStateChangeNotification();
        }
    }
}


std::string QuestController::getTypeName(int type){
    std::string did = "";
    switch (type) {
        case SORT_RECOMMED:
            did = "107501";
            break;
        case SORT_COMMPLETE:
            did = "107517";
            break;
        case SORT_TRAING:
            did = "107504";
            break;
        case SORT_BUILDING:
            did = "107502";
            break;
        case SORT_RESERCH:
            did = "107503";
            break;
        case SORT_RES:
            did = "107505";
            break;
        case SORT_MISC:
            did = "107506";
            break;
        default:
            break;
    }
    if(did == ""){
        return "";
    }
    return _lang(did);
}

std::string QuestController::getTypeIcon(int type){
    std::string path = CCString::createWithFormat("quest_icon_%d.png", type)->getCString();
    return path;
}

std::string QuestController::getQuestIcon(QuestInfo *info){
    std::string iconPath = "temp_icon.png";
    if(info->type == QUEST_BUILDING_AUTO){
        int id = atoi(info->itemId.c_str()) / 100 * 100;
        map<int, CCDictionary*>::iterator it = config.find(id);
        if(it != config.end()){
            std::string para1 = it->second->valueForKey("para1")->getCString();
            iconPath = CCCommonUtils::getPropById(para1, "pic")  + "_" + CC_ITOA(GlobalData::shared()->contryResType)+".png";
        }
    }if(info->type == QUEST_TRAING){
        std::string para1 = CCCommonUtils::getPropById(info->itemId, "para1");
        iconPath = CCCommonUtils::getPropById(para1, "ico") + ".png";
    }
    return iconPath;
}

void QuestController::getReward(std::string itemId, bool isMain){
    isGettingReward = true;
    QuestRewardCommand *cmd = new QuestRewardCommand(itemId, isMain);
    if (isMain && GuideController::share()->getCurGuideID() == GUIDE_REWD_ED) {
        cmd->putParam("guideStep", CCString::create(GUIDE_REWD_ED));
    }
    cmd->sendAndRelease();
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(QuestController::resetGettingRewardFlag), this);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(QuestController::resetGettingRewardFlag), this, 5.0, 1, 0.0f, false);
    if (FBUtilies::fbIsLogin()) {
        double lastTime = 0.0;
        string strTime = CCUserDefault::sharedUserDefault()->getStringForKey("facebookShareDataTime", "");
        if (strTime!="") {
            lastTime = atof(strTime.c_str());
        }
        std:: map<string, CCDictionary*>::iterator it = GlobalData::shared()->shareFbmap.find("finish_quest");
        auto ctime = GlobalData::shared()->getTimeStamp();
        if (it!=GlobalData::shared()->shareFbmap.end()) {
            CCDictionary* dict = it->second;
            if (dict) {
                int on = dict->valueForKey("ON")->intValue();
                double limit = dict->valueForKey("Limit")->doubleValue();
                string permission = dict->valueForKey("permissions")->getCString();
                string ref = dict->valueForKey("ref")->getCString();
                if (lastTime > 0) {
                    lastTime = lastTime + limit;
                }
                if (on==1 && ctime > lastTime && FBUtilies::fbHasGranted(permission)) {
                    CCUserDefault::sharedUserDefault()->setStringForKey("facebookShareDataTime", CC_ITOA(ctime));
                    CCUserDefault::sharedUserDefault()->flush();
                    string action = dict->valueForKey("action")->getCString();
                    string actionType = dict->valueForKey("object_type")->getCString();
                    string actionObject = dict->valueForKey("object")->getCString();
                    if (action=="" || actionType=="" || actionObject=="") {
                        return ;
                    }
                    FBUtilies::fbPostAction(action, actionType, actionObject);

                    std::string data = "action=publish_feed";
                    if (ref!="") {
                        data.append(".");
                        data.append(ref);
                    }
                    GameController::getInstance()->callXCApi(data);
                }
            }
        }
    }
}

void QuestController::resetGettingRewardFlag(float t){
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(QuestController::resetGettingRewardFlag), this);
    isGettingReward = false;
}

void QuestController::setQuestReward(std::string questId, CCArray *arr){
    auto quest = getQuestFormMapById(questId);
    if(quest){
        CCArray *tmpArr = CCArray::create();
        tmpArr->retain();
        int i = 0;
        while (i < arr->count()) {
            auto dic = dynamic_cast<CCDictionary*>(arr->objectAtIndex(i));
            int type = dic->valueForKey("type")->intValue();
            int value = dic->valueForKey("value")->intValue();
            if(value != 0){
                if(type == R_EXP){
                    tmpArr->insertObject(dic, 0);
                }else if(type == R_POWER){
                    if(tmpArr->count() == 0){
                        tmpArr->insertObject(dic, 0);
                    }else{
                        auto dic1 = dynamic_cast<CCDictionary*>(tmpArr->objectAtIndex(0));
                        int type1 = dic1->valueForKey("type")->intValue();
                        if(type1 == R_EXP){
                            tmpArr->insertObject(dic, 1);
                        }else{
                            tmpArr->insertObject(dic, 0);
                        }
                    }
                }else{
                    tmpArr->addObject(dic);
                }
            }
            i++;
        }
        arr->removeAllObjects();
        quest->reward = tmpArr;
    }
}

void QuestController::resetBuildQuest(bool postNotification){
    map<std::string, QuestInfo*>::iterator it;
    for(it = buildQuestList.begin(); it != buildQuestList.end(); it++){
        CC_SAFE_RELEASE(it->second);
    }
    buildQuestList.clear();
    auto group = LocalController::shared()->DBXMLManager()->getGroupByKey("building");
    CCDictElement *ele;
    int i = 0;
    int level = 0;
    CCArray *arr = CCArray::create();
    CCDICT_FOREACH(group, ele){
        int itemId = atoi(ele->getStrKey());
        i = 0;
        int id = FunBuildController::getInstance()->getMaxLvBuildByType(itemId);
        if(id != 0){
            level = GlobalData::shared()->imperialInfo[id].level;
        }
        else{
            level = 0; 
        }
        while(i < numPerTypeShow && level < GlobalData::shared()->MaxBuildLv){
            addQuestToBuildingList(itemId, level + 1, arr);
            i++;
            level++;
        }
    }
    
    i = 0;
    while(i < arr->count()){
        auto info = dynamic_cast<QuestInfo*>(arr->objectAtIndex(i++));
        info->retain();
        buildQuestList[info->itemId] = info;
    }
    
    arr->removeAllObjects();
    if(postNotification){
        postQuestStateChangeNotification();
    }
}
bool QuestController::goToQuestTarget(QuestInfo* quest, bool isSt){
    if(quest==NULL){
        return false;
    }
    bool ret = false;
    int type = quest->gotype;
    int itemId = quest->go;

    switch (type) {
        case 1:{
        
        }
        case 2:{
            if(SceneController::getInstance()->currentSceneId==SCENE_ID_WORLD){
                m_returnFromWorld = true;
                PopupViewController::getInstance()->removeAllPopupView();
                auto world = WorldMapView::instance();
                if (world) {
                    world->leaveWorld();
                }
            }else{
                
                m_returnFromWorld = false;
                auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
                if(layer) {
                    if(type==1){
                        itemId =layer->findCanBuildTile(itemId);
                        if(itemId==-1){
                            return ret;
                        }
                        ret = true;
                        layer->onMoveToBuildAndPlay(itemId);
                    }else{
                        int bid = FunBuildController::getInstance()->getMaxLvBuildByType(itemId);
                        if(bid>0){
                            ret = true;
                            if(bid == 400000000)
                                PopupViewController::getInstance()->addPopupInView(GeneralTitanPopupView::create(),true);//fusheng 从外面任务索引点击龙升级 直接打开龙界面
                            else
                                layer->onMoveToBuildAndPlay(bid);
                        }
                    }
                }
            }
            break;
        }
        case 3:
            //layer->onMoveToBuildAndPlay(itemId);
            if (isSt) {
                break;
            }
            if(SceneController::getInstance()->currentSceneId==SCENE_ID_MAIN){
                PopupViewController::getInstance()->removeAllPopupView();
                SceneController::getInstance()->gotoScene(SCENE_ID_WORLD);
            }
            break;
        case 4:{
            if(SceneController::getInstance()->currentSceneId==SCENE_ID_WORLD){
                m_returnFromWorld = true;
                PopupViewController::getInstance()->removeAllPopupView();
                auto world = WorldMapView::instance();
                if (world) {
                    world->leaveWorld();
                }
            }else{
                m_returnFromWorld = false;
                auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
                if(layer) {
                    layer->onMoveToSpeBuildAndPlay(itemId);
                }
            }
            break;
        }
        default:
            break;
    }
    return ret;
}

void QuestController::resetResearchQuest(){
    map<std::string, QuestInfo*>::iterator it;
    for(it = researchQuestList.begin(); it != researchQuestList.end(); it++){
        CC_SAFE_RELEASE(it->second);
    }
    buildQuestList.clear();
    int i = 0;
    int level = 0;
    CCArray *arr = CCArray::create();
    map<int, ScienceItemInfo>::iterator sIt;
    for(sIt = GlobalData::shared()->scienceInfo.infoMap.begin(); sIt != GlobalData::shared()->scienceInfo.infoMap.end(); sIt++){
        int itemId = sIt->first;
        i = 0;
        sIt->second.scienceMaxLv = 10;
        while(i < numPerTypeShow && level < sIt->second.scienceMaxLv){
            addQuestToBuildingList(itemId, level + 1, arr);
            i++;
            level++;
        }
    }
    
    i = 0;
    while(i < arr->count()){
        auto info = dynamic_cast<QuestInfo*>(arr->objectAtIndex(i++));
        info->retain();
        researchQuestList[info->itemId] = info;
    }
    
    arr->removeAllObjects();
}

QuestInfo *QuestController::getQuestBuilding(int itemId, int state){
    int bid = itemId / 100 * 100;
    int level = itemId % 100;
    QuestInfo *insertInfo = QuestInfo::create();
    insertInfo->itemId = CC_ITOA(itemId);
    insertInfo->state = state;
    auto dict = config[bid];
    int recommed = atoi(dict->valueForKey("recommend")->getCString()) + (level - 1) * 1000000;
    insertInfo->type = dict->valueForKey("type2")->intValue();

    insertInfo->recomment = recommed;
    vector<std::string> vector2;
    vector<std::string> vector1;
    vector<std::string> vector;
    CCCommonUtils::splitString(dict->valueForKey("name")->getCString(), "|", vector);
    CCCommonUtils::splitString(dict->valueForKey("target")->getCString(), "|", vector1);
    CCCommonUtils::splitString(dict->valueForKey("description")->getCString(), "|", vector2);
    
    if(level == 1){
        insertInfo->name = _lang(vector[0]);
        insertInfo->target = _lang(vector1[0]);
        insertInfo->description = _lang(vector2[0]);
    }else{
        insertInfo->name = _lang_1(vector[1], CC_ITOA(level));
        insertInfo->target = _lang_1(vector1[1], CC_ITOA(level));
        insertInfo->description = _lang_1(vector2[1], CC_ITOA(level));
    }
    return insertInfo;
}

void QuestController::addQuestToBuildingList(int bid, int level, CCArray *arr){
    int itemId = bid + level;
    QuestInfo *insertInfo = getQuestBuilding(itemId);
    arr->addObject(insertInfo);
}

void QuestController::addQuestToByScienceList(int sid, int level){
    
}

QuestInfo *QuestController::getQuestFormMapById(std::string itemId){
    map<std::string, QuestInfo*>::iterator it = questList.find(itemId);
    if(it != questList.end()){
        return it->second;
    }
    it = buildQuestList.find(itemId);
    if(it != buildQuestList.end()){
        return it->second;
    }
    it = researchQuestList.find(itemId);
    if(it != researchQuestList.end()){
        return it->second;
    }
    return NULL;
}

CCDictionary *QuestController::getShowQuest(){
    CCDictionary *dict = CCDictionary::create();
    int max = 0;
    map<std::string, QuestInfo*>::iterator it;
    
    CCArray *buildingArr = CCArray::create();
    dict->setObject(buildingArr, SORT_BUILDING);
    for(it = QuestController::getInstance()->buildQuestList.begin(); it != QuestController::getInstance()->buildQuestList.end(); it++){
        insertToArr(buildingArr, it->first, false);
    }
    
    CCArray *scienceArr = CCArray::create();
    dict->setObject(scienceArr, SORT_RESERCH);
    for(it = QuestController::getInstance()->researchQuestList.begin(); it != QuestController::getInstance()->researchQuestList.end(); it++){
        insertToArr(scienceArr, it->first, false);
    }

    //把任务按order1放进dic
    for(it = QuestController::getInstance()->questList.begin(); it != QuestController::getInstance()->questList.end(); it++){
        int order = atoi(CCCommonUtils::getPropById(it->second->itemId, "order1").c_str());
        if(it->second->state == COMPLETE){
            order = 1;
        }
        max = order > max ? order : max;
        CCArray *arr = dynamic_cast<CCArray*>(dict->objectForKey(order));
        if(arr == NULL){
            arr = CCArray::create();
            dict->setObject(arr, order);
        }
        //按order2排序
        insertToArr(arr, it->first, order == 1);
    }
    dict->setObject(CCString::create(CC_ITOA(max)), 100);
//    //推荐任务生成
//    generateRecommedQuest(dict);
    return dict;
}

void QuestController::insertToArr(CCArray *arr, std::string itemId, bool isComplete){
    //每类最多5个
    if(arr->count() == 0){
        arr->addObject(CCString::create(itemId));
    }else{
        //推荐显示编号低的
        auto questInsert = getQuestFormMapById(itemId);
        int order = questInsert->recomment;
        int index = 0;
        while(index < arr->count()){
            std::string currentId = dynamic_cast<CCString*>(arr->objectAtIndex(index))->getCString();
            auto questCurrent = getQuestFormMapById(currentId);
            int questCurrentOrder = questCurrent->recomment;
            if(questCurrentOrder > order){
                arr->insertObject(CCString::create(itemId), index);
                if(arr->count() > numPerTypeShow && !isComplete){
                    arr->removeLastObject();
                }
                return;
            }
            index++;
        }
        if(arr->count() < numPerTypeShow || isComplete){
            arr->addObject(CCString::create(itemId));
        }
    }
}

void QuestController::generateRecommedQuest(CCDictionary *dict){
    map<std::string, QuestInfo*>::iterator it;
    
    QuestInfo *info = getRecommendQuest();
    
    if(info){
        CCArray *arr = CCArray::create();
        arr->addObject(CCString::create(info->itemId));
        dict->setObject(arr, SORT_RECOMMED);
        if(info->state == COMPLETE){
            CCArray *arr = dynamic_cast<CCArray*>(dict->objectForKey(1));
            if(arr){
                CCObject *obj;
                CCARRAY_FOREACH(arr, obj){
                    std::string str = dynamic_cast<CCString*>(obj)->getCString();
                    if(str == info->itemId){
                        arr->removeObject(obj);
                        return;
                    }
                }
            }
        }
    }
}

QuestInfo* QuestController::getRecommendQuest(bool includeComplete){
    CCArray* m_data = CCArray::create();
    bool isaccept = true;
    map<std::string, QuestInfo*>::iterator it;
    for(it = QuestController::getInstance()->currentAchList.begin();it!=QuestController::getInstance()->currentAchList.end();it++){
        if(it->second->order<=0){
            continue;
        }
        if(includeComplete){
            if(it->second->type==1&&(it->second->state == ACCEPT||it->second->state == COMPLETE)){
                m_data->addObject(it->second);
                isaccept = false;
            }
        }else if(it->second->type==1&&it->second->state == ACCEPT){
            m_data->addObject(it->second);
            isaccept = false;
        }
    }
//    bool isorder = true;
//    if(isaccept){
//        for(it = QuestController::getInstance()->currentAchList.begin();it!=QuestController::getInstance()->currentAchList.end();it++){
//            if(it->second->order!=0&&it->second->state != COMPLETE){
//                m_data->addObject(it->second);
//                isorder = false;
//            }
//        }
//    }
//    if(isorder&&isaccept){
//        for(it = QuestController::getInstance()->currentAchList.begin();it!=QuestController::getInstance()->currentAchList.end();it++){
//            if(it->second->order!=0){
//                m_data->addObject(it->second);
//            }
//        }
//    }
    int num = m_data->count()-1;
    for(int j=0;j<num;j++){
        for(int i=0;i<num-j;i++)
        {
            QuestInfo* quest1 = (QuestInfo*)m_data->objectAtIndex(i);
            QuestInfo* quest2 = (QuestInfo*)m_data->objectAtIndex(i+1);
            if(quest1->order > quest2->order)
               //|| (quest1->state == ACCEPT && quest2->state != ACCEPT))//未接受的在前
            {
                m_data->swap(i, i+1);
            }
        }
    }
    QuestInfo* info = NULL;
    if(m_data->count()>0){
        info = (QuestInfo*)m_data->objectAtIndex(0);
    }
    return info;
}

int QuestController::getCompleteQuestNum(){
    map<std::string, QuestInfo*>::iterator it;
    int num = 0;
//    for(it = currentAchList.begin(); it != currentAchList.end(); it++){
//        if(it->second->state == COMPLETE){
//            num++;
//        }
//    }
    CCArray* arr = CCArray::create();
    //setRecommendTaskInfo();
    for(it = QuestController::getInstance()->currentAchList.begin();it!=QuestController::getInstance()->currentAchList.end();it++){
        if(it->second->type==2){
            if(it->second->order2<=0){
                continue;
            }
            arr->addObject(it->second);
            //break;
        }
    }
    int leng = arr->count()-1;
    for(int j=0;j<leng;j++){
        for(int i=0;i<leng-j;i++)
        {
            QuestInfo* quest1 = (QuestInfo*)arr->objectAtIndex(i);
            QuestInfo* quest2 = (QuestInfo*)arr->objectAtIndex(i+1);
            if(quest1->order2 > quest2->order2 || (quest1->state == ACCEPT && quest2->state != ACCEPT))//未接受的在前
            {
                arr->swap(i, i+1);
            }
        }
    }
    int length1 =arr->count();
    if(length1>3){
        length1 = 3;
    }
    for (int i=0; i<length1; i++) {
        QuestInfo* quest = (QuestInfo*)arr->objectAtIndex(i);
        if(quest->state==COMPLETE){
            num++;
        }
    }
    QuestInfo* recommand = getRecommendQuest();
    if(recommand&&recommand->state==COMPLETE){
        num++;
    }
    return num;
}

void QuestController::startGetRewardById(int itemId)
{
    if (tmpRewardId != 0) {
        return;
    }
    tmpRewardId = itemId;
    GetRdCommand* cmd = new GetRdCommand(itemId);
    cmd->sendAndRelease();
}

void QuestController::endGetRewardById(CCDictionary* dict)
{
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyHint("", "", _lang(error));
        return;
    }
    else
    {
        if (dict->objectForKey("reward") && tmpRewardId!=0) {
            m_stageRDMap[ tmpRewardId ] = dynamic_cast<CCArray*>(dict->objectForKey("reward"));
            tmpRewardId = 0;
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GETRT_REWARD_INFO);
    }
}

void QuestController::startGetStageRd()
{
    QuestStageRDCommand* cmd = new QuestStageRDCommand();
    cmd->sendAndRelease();
}

void QuestController::endGetStateRd(CCDictionary* dict)
{
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyHint("", "", _lang(error));
        return;
    }
    else
    {
        m_stageRdId = dict->valueForKey("taskCompleteId")->intValue();
        string rewardId = CCCommonUtils::getPropById(CC_ITOA(m_stageRdId), "reward");
        m_stageRDMap[ atoi(rewardId.c_str()) ] = dynamic_cast<CCArray*>(dict->objectForKey("nextRdInfo"));
        
        CCArray* reward = dynamic_cast<CCArray*>(dict->objectForKey("reward"));
        
        // TODO: guojiang
//        GCMRewardController::getInstance()->retReward2(reward, true);
        
        //        putSFSArray("reward")
        //        putUtfString("taskCompleteId")
        //        putSFSArray("nextRdInfo")
        //        putBool("finished")
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(QUEST_STAGE_UPDATE);
    }
}
