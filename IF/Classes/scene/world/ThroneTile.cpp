//
//  ThroneTile.cpp
//  IF
//
//  Created by 李锐奇 on 14/12/4.
//
//

#include "ThroneTile.h"
#include "AllianceManager.h"
#include "YesNoDialog.h"
#include "WorldMapView.h"
#include "ScoutInfo.h"
#include "ManagerKingdomView.h"
#include "CheckKingdomView.h"
#include "WorldActivityCommand.h"
#include "TipsView.h"
#include "TroopInformationView.h"
#include "HelpshiftCocos2dx.h"
#include "GetFightOfKingRecord.h"
#include "SoundController.h"
#include <spine/Json.h>

bool ThroneTile::init() {
    NewBaseTileInfo::init();
    m_title->setString(_lang(m_cityInfo.cityName));
    
    if(!WorldController::isInSelfServer(m_cityInfo.tileServerId)){
        setPeaceTimeButtons();
    }else{
        if(isSelfBuild()){
            auto activityIt = WorldController::getInstance()->m_worldActivity.find(FIGHT_OF_KING);
            if(activityIt != WorldController::getInstance()->m_worldActivity.end()){
                auto &activityInfo = activityIt->second;
                if(activityInfo.currentState == NotOpen || activityInfo.currentState == OpenNoKing){
                    setNotOpenTimeButtons();
                }else if(activityInfo.currentState == PeaceTime){
                    setPeaceTimeButtons();
                }else{
                    setDefenderWarTimeButtons();
                }
            }
        }else{
            auto activityIt = WorldController::getInstance()->m_worldActivity.find(FIGHT_OF_KING);
            if(activityIt != WorldController::getInstance()->m_worldActivity.end()){
                auto &activityInfo = activityIt->second;
                if(activityInfo.currentState == NotOpen || activityInfo.currentState == OpenNoKing){
                    setNotOpenTimeButtons();
                }else if(activityInfo.currentState == PeaceTime){
                    setPeaceTimeButtons();
                }else{
                    setAttackerWarTimeButtons();
                }
            }
        }
    }
    addToParent();
    return true;
}

void ThroneTile::onClickSurpport(CCObject * pSender, Control::EventType pCCControlEvent){
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    WorldController::getInstance()->openMarchDeploy(m_cityInfo.parentCityIndex, 1);
    closeThis();
}

bool ThroneTile::isSelfBuild(){
    return WorldController::getInstance()->isSelfThrone(m_cityInfo);
}

void ThroneTile::onTroopClick(CCObject * pSender, Control::EventType pCCControlEvent){
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    int index = m_cityInfo.cityIndex;
    if(m_cityInfo.parentCityIndex != -1){
        index = m_cityInfo.parentCityIndex;
    }
    std::string uuid = "";
    for (auto it = WorldController::getInstance()->m_marchInfo.begin(); it != WorldController::getInstance()->m_marchInfo.end(); it++) {
        if(it->second.ownerType == PlayerSelf && it->second.endPointIndex == index){
            uuid = it->second.uuid;
            if(it->second.teamUid != ""){
                uuid = it->second.uuid;
            }
        }
    }
    
    WorldMapView::instance()->addPopupView(TroopInformationView::create(uuid));
}

void ThroneTile::onClickRally(CCObject * pSender, Control::EventType pCCControlEvent){
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    if (isInProtect()) {
        CCCommonUtils::flyHint("", "", _lang("108623"));
        closeThis();
        return;
    }
    if(!GlobalData::shared()->playerInfo.isInAlliance()){
        CCCommonUtils::flyHint("", "", _lang("115218"));
        closeThis();
        return;
    }
    unsigned int index = m_cityInfo.cityIndex;
    if(m_cityInfo.parentCityIndex != -1){
        index = m_cityInfo.parentCityIndex;
    }
    auto &player = WorldController::getInstance()->m_playerInfo[m_cityInfo.playerName];
    if(player.cityLv <6 || m_cityInfo.leagueAsn==""){
        //todo
    }
    auto selfProtect = GlobalData::shared()->playerInfo.protectTimeStamp;
    auto now = WorldController::getInstance()->getTime();
    auto func = [&,index](){
        AllianceManager::getInstance()->openAllianceMassView(index);
        closeThis();
    };
    if (now < selfProtect) {
        YesNoDialog::show(_lang("101438").c_str(),func);
    } else {
        func();
        closeThis();
    }
}

bool ThroneTile::isInProtect() {
    return false;
}

void ThroneTile::onClickScout(CCObject * pSender, Control::EventType pCCControlEvent){
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    if (isInProtect()) {
        CCCommonUtils::flyHint("", "", _lang("108623"));
        closeThis();
        return;
    }
    // judge self protect state
    auto selfProtect = GlobalData::shared()->playerInfo.protectTimeStamp;
    auto now = WorldController::getInstance()->getTime();
    
    unsigned int index = m_cityInfo.cityIndex;
    if(m_cityInfo.parentCityIndex != -1){
        index = m_cityInfo.parentCityIndex;
    }
    
    auto func = [&,index](){
        auto world = WorldMapView::instance();
        if (world) {
            world->addPopupView(ScoutInfo::create(WorldController::getInstance()->getCityInfos()[index]));
        }
    };
    
    if (now < selfProtect) {
        YesNoDialog::show(_lang("101438").c_str(),func);
    } else {
        func();
    }
    closeThis();
}

void ThroneTile::onClickKing(CCObject * pSender, Control::EventType pCCControlEvent){
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    PopupViewController::getInstance()->removeAllPopupView();
    if(WorldController::isInSelfServer(m_cityInfo.tileServerId)){
        if(GlobalData::shared()->playerInfo.officer == KINGDOM_KING_ID){
            PopupViewController::getInstance()->addPopupInView(ManagerKingdomView::create());
        }else{
            PopupViewController::getInstance()->addPopupInView(CheckKingdomView::create());
        }
    }else{
        PopupViewController::getInstance()->addPopupInView(CheckKingdomView::create());
    }
    closeThis();
}

void ThroneTile::onInfoClick(CCObject * pSender, Control::EventType pCCControlEvent) {
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    PopupViewController::getInstance()->addPopupView(WarBuildInfoPopUpView::create(m_cityInfo));
    closeThis();
}

void ThroneTile::onClickOccupy(CCObject * pSender, Control::EventType pCCControlEvent) {
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    int hasOwner = getOwnerFlag();
    WorldController::getInstance()->openMarchDeploy(m_cityInfo.parentCityIndex, hasOwner);
    closeThis();
}

std::string ThroneTile::getShowText(){
    if(m_cityInfo.kingBuildInfo.allianceUid != ""){
        return m_cityInfo.kingBuildInfo.allianceUid;
    }else if(m_cityInfo.kingBuildInfo.uid != ""){
        return m_cityInfo.kingBuildInfo.uid;
    }
    return "";
}

int ThroneTile::getOwnerFlag(){
    int hasOwner = 0;
    if(m_cityInfo.kingBuildInfo.uid != "" || m_cityInfo.kingBuildInfo.allianceUid != ""){
        hasOwner = 1;
    }
    return hasOwner;
}

void ThroneTile::setNotOpenTimeButtons(){
    setButtonCount(1);
    setButtonName(1,_lang("110076")); // info
    setButtonState(1, ButtonInformation);
    setButtonCallback(1,cccontrol_selector(ThroneTile::onInfoClick));
}

void ThroneTile::setPeaceTimeButtons(){
    setButtonCount(2);
    setButtonName(2,_lang("110076")); //
    setButtonName(3, _lang("110021")); //
    if(WorldController::isInSelfServer(m_cityInfo.tileServerId)){
        if(GlobalData::shared()->playerInfo.officer == KINGDOM_KING_ID){
            setButtonName(3, _lang("110066")); //
        }
    }
    setButtonState(2, ButtonInformation);
    setButtonState(3, ButtonMyKing);
    setButtonCallback(2,cccontrol_selector(ThroneTile::onInfoClick));
    setButtonCallback(3,cccontrol_selector(ThroneTile::onClickKing));
}

void ThroneTile::setDefenderWarTimeButtons(){
    setButtonCount(4);
    setButtonName(2,_lang("110076")); //
    setButtonName(3,_lang("108728")); //
    setButtonName(4,_lang("110067")); //
    setButtonName(5,_lang("108724")); //
    
    setButtonState(2, ButtonInformation);
    setButtonState(3, ButtonSupport);
    setButtonState(4, ButtonRally);
    setButtonState(5, ButtonViewTroop);
    
    setButtonCallback(2,cccontrol_selector(ThroneTile::onInfoClick));
    setButtonCallback(3,cccontrol_selector(ThroneTile::onClickSurpport));
    setButtonCallback(4,cccontrol_selector(ThroneTile::onClickRally));
    setButtonCallback(5,cccontrol_selector(ThroneTile::onTroopClick));
}

void ThroneTile::setAttackerWarTimeButtons(){
    setButtonCount(4);
    setButtonName(2,_lang("110076")); //
    setButtonName(3,_lang("108723")); //
    setButtonName(4,_lang("108726")); //
    setButtonName(5,_lang("108722")); //
    setButtonState(2, ButtonInformation);
    setButtonState(3, ButtonMarch);
    setButtonState(4, ButtonRally);
    setButtonState(5, ButtonScout);
    setButtonCallback(2,cccontrol_selector(ThroneTile::onInfoClick));
    setButtonCallback(3,cccontrol_selector(ThroneTile::onClickOccupy));
    setButtonCallback(4,cccontrol_selector(ThroneTile::onClickRally));
    setButtonCallback(5,cccontrol_selector(ThroneTile::onClickScout));
}

//--

WarBuildInfoPopUpView *WarBuildInfoPopUpView::create(WorldCityInfo &info){
    WarBuildInfoPopUpView *ret = new WarBuildInfoPopUpView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void WarBuildInfoPopUpView::onEnter(){
    PopupBaseView::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(WarBuildInfoPopUpView::dataBack), GET_CROSS_WORLD_DATA_BACK, NULL);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void WarBuildInfoPopUpView::onExit(){
    GlobalData::shared()->isBind = false;
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GET_CROSS_WORLD_DATA_BACK);
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    PopupBaseView::onExit();
}

bool WarBuildInfoPopUpView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(208, true);
        CCLoadSprite::doResourceByCommonIndex(310, true);

        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(7, false);
            CCLoadSprite::doResourceByCommonIndex(208, false);
            CCLoadSprite::doResourceByCommonIndex(310, false);
        });
        setIsHDPanel(true);
        auto ccb = CCBLoadFile("WarBuildInfo.ccbi", this, this);
        if (CCCommonUtils::isIosAndroidPad()) {
            this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        }
        else
            this->setContentSize(ccb->getContentSize());
        //CCCommonUtils::setButtonTitle(m_infoBtn, _lang("108730").c_str());
        CCCommonUtils::setButtonTitle(m_historyBtn, _lang("110110").c_str());
        CCCommonUtils::setButtonTitle(m_kingRecordBtn, _lang("110134").c_str());
    
        m_currentTab = 1;
        isDataBack = false;
        type = NotOpen;
        nextRefreshTime = 0;
        m_currentNum = 0;
        m_fightInfos = CCArray::create();
        m_recordKings = CCArray::create();
        
        m_tabView = CCTableView::create(this, m_listNode->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setTouchPriority(Touch_Default);
        m_listNode->addChild(m_tabView);
        
        if (CCCommonUtils::isIosAndroidPad()) {
            m_testTxt = CCLabelIFTTF::create("", 36);
            m_testTxt->setAlignment(kCCTextAlignmentLeft);
            m_testTxt->setVerticalAlignment(kCCVerticalTextAlignmentTop);
            m_testTxt->setDimensions(CCSize(882,0));
            m_testTxt->setAnchorPoint(ccp(0, 1));
        }
        else {
            m_testTxt = CCLabelIFTTF::create("", 20);
            m_testTxt->setAlignment(kCCTextAlignmentLeft);
            m_testTxt->setVerticalAlignment(kCCVerticalTextAlignmentTop);
            m_testTxt->setDimensions(CCSize(490,0));
            m_testTxt->setAnchorPoint(ccp(0, 1));
        }
        
        hasMore = true;
        m_kTimes = "";
        m_totalPage = 1;
        m_curPage = 1;
        m_kingRecordBtn->setVisible(true);
        showTab();
        getData();
        if(!WorldController::isInSelfServer(m_info.tileServerId)){
            m_historyBtn->setVisible(false);
            m_kingRecordBtn->setPositionX(0);
        }
        ret = true;
    }
    return ret;
}

bool WarBuildInfoPopUpView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeText", CCLabelIF*, this->m_timeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeTitle", CCLabelIF*, this->m_timeTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_occupyTitle", CCLabelIF*, this->m_occupyTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_soldierText", CCLabelIF*, this->m_soldierText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noRecordText", CCLabelIF*, this->m_noRecordText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceName", CCLabelIFTTF*, this->m_allianceName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_occupyName", CCLabelIFTTF*, this->m_occupyName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoBtn", CCControlButton*, this->m_infoBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_historyBtn", CCControlButton*, this->m_historyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconContainer", CCNode*, this->m_iconContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, this->m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceNode", CCNode*, this->m_allianceNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_soldierNode", CCNode*, this->m_soldierNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_historyNode", CCNode*, this->m_historyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoNode", CCNode*, this->m_infoNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_kingRecordBtn", CCControlButton*, this->m_kingRecordBtn);

    return false;
}

SEL_CCControlHandler WarBuildInfoPopUpView::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onInfoClick", WarBuildInfoPopUpView::onInfoClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHistoryClick", WarBuildInfoPopUpView::onHistoryClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onKingRecordClick", WarBuildInfoPopUpView::onKingRecordClick);
    return NULL;
}

void WarBuildInfoPopUpView::onInfoClick(CCObject * pSender, Control::EventType pCCControlEvent){
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    GlobalData::shared()->isBind = true;
#endif
    CCCommonUtils::showHelpShiftFAQ("45201");
//    HelpshiftCocos2dx::showFAQSection("466");
}


void WarBuildInfoPopUpView::onHistoryClick(CCObject * pSender, Control::EventType pCCControlEvent){
    if(m_currentTab == 2){
        m_currentTab = 1;
    }else{
        m_currentTab = 2;
    }
    showTab();
}

void  WarBuildInfoPopUpView::onKingRecordClick(CCObject * pSender, Control::EventType pCCControlEvent){
    if(m_currentTab == 3){
        m_currentTab = 1;
    }else{
        m_currentTab = 3;
    }
    showTab();
}

void WarBuildInfoPopUpView::getData(){
    isDataBack = false;
    CrossWorldActivityGetCommand *cmd = new CrossWorldActivityGetCommand(m_info.parentCityIndex);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(WarBuildInfoPopUpView::dataBack), NULL));
    cmd->sendAndRelease();
}

void WarBuildInfoPopUpView::getHistoryData(){
    if(!hasMore){
        if(m_currentNum == 0){
            m_noRecordText->setString(_lang("110115"));
        }else{
            m_noRecordText->setString("");
        }
        return;
    }
    int start = m_currentNum;
    int end = start + _loadPerTime - 1;
    GetFightInfoCommand *cmd = new GetFightInfoCommand(start, end, m_info.parentCityIndex);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(WarBuildInfoPopUpView::historyDataBack), NULL));
    cmd->sendAndRelease();
}

void WarBuildInfoPopUpView::showTab(){
    if(m_currentTab == 1){
        m_infoNode->setVisible(true);
        m_historyNode->setVisible(false);
    }else if(m_currentTab == 2){
        m_infoNode->setVisible(false);
        m_historyNode->setVisible(true);
        m_tabView->getContainer()->removeAllChildrenWithCleanup(true);
        if(m_fightInfos->count() == 0){
            getHistoryData();
        }else{
            m_tabView->reloadData();
        }
    }else{
        m_infoNode->setVisible(false);
        m_historyNode->setVisible(true);
        m_tabView->reloadData();
        if(m_recordKings->count() <= 0){
            getKingRecordData();
        }else{
            m_tabView->reloadData();
        }
    }
}

void WarBuildInfoPopUpView::historyDataBack(CCObject *obj){
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        return;
    }
    
    auto params = _dict(ret->getData());
    auto arr = dynamic_cast<CCArray*>(params->objectForKey("result"));
    if(arr == NULL || arr->count() < _loadPerTime){
        hasMore = false;
    }
    if(arr){
        m_currentNum += arr->count();
    }
    if(!hasMore && m_currentNum == 0){
        m_noRecordText->setString(_lang("110115"));
    }else{
        m_noRecordText->setString("");
    }
    CCObject *ele;
    CCARRAY_FOREACH(arr, ele){
        auto json = Json_create(dynamic_cast<CCString*>(ele)->getCString());
        double time = Json_getFloat(json, "time", 0) / 1000;
        int type = Json_getInt(json, "type", 0);//0 攻击 1 防御 2 增援
        auto attacker = Json_getItem(json, "att");
        auto defender = Json_getItem(json, "def");
        std::string defenderUid = "";
        std::string defenderName = "";
        std::string defenderAbbr = "";
        std::string defenderAllianceUid = "";
        std::string attackerUid = "";
        std::string attackerName = "";
        std::string attackerAbbr = "";
        std::string attackerAllianceUid = "";
        std::string defenderNameStr = "";
        std::string attackerNameStr = "";
        int team = 0;
        if(defender){
            defenderUid = Json_getString(defender, "uid", "");
            defenderName = Json_getString(defender, "name", "");
            defenderAbbr = Json_getString(defender, "abbr", "");
            defenderAllianceUid = Json_getString(defender, "allianceId", "");
            if(defenderAbbr != ""){
                defenderNameStr = std::string("(") + defenderAbbr + ")" + defenderName;
            }else{
                defenderNameStr = defenderName;
            }
        }
        if(attacker){
            attackerUid = Json_getString(attacker, "uid", "");
            attackerName = Json_getString(attacker, "name", "");
            attackerAbbr = Json_getString(attacker, "abbr", "");
            attackerAllianceUid = Json_getString(attacker, "allianceId", "");
            team = Json_getInt(attacker, "team", 0);
            if(attackerAbbr != ""){
                attackerNameStr = std::string("(") + attackerAbbr + ")" + attackerName;
            }else{
                attackerNameStr = attackerName;
            }
        }
        
        int color = 2;//0 绿 1 红 2 白
        std:string selfName = GlobalData::shared()->playerInfo.name;
        std::string selfAllianceUid = "";
        if(GlobalData::shared()->playerInfo.isInAlliance()){
            selfAllianceUid = GlobalData::shared()->playerInfo.allianceInfo.uid;
        }
        std::string contentStr = "";
        std::string buildingName = "";
        
        if(m_info.cityType == Throne){
            buildingName = _lang("110070");
        }else{
            buildingName = _lang("110081");
        }
        bool isTeamAtt = !(team == 0);
        
        if(type == 0){
            if(defenderName == ""){
                contentStr = _lang_2("110111", attackerNameStr.c_str(), buildingName.c_str());
            }else{
                if(isTeamAtt){
                    contentStr = _lang_3("110106", attackerNameStr.c_str(), buildingName.c_str(), defenderNameStr.c_str());
                }else{
                    contentStr = _lang_3("110105", attackerNameStr.c_str(), buildingName.c_str(), defenderNameStr.c_str());
                }
            }
            if(attackerName == selfName || (selfAllianceUid != "" && selfAllianceUid == attackerAllianceUid)){
                color = 0;
            }else if(defenderName == selfName || (selfAllianceUid != "" && selfAllianceUid == defenderAllianceUid)){
                color = 1;
            }
        }else if(type == 1){
            if(isTeamAtt){
                contentStr = _lang_3("110108", defenderNameStr.c_str(), attackerNameStr.c_str(), buildingName.c_str());
            }else{
                contentStr = _lang_3("110107", defenderNameStr.c_str(), attackerNameStr.c_str(), buildingName.c_str());
            }
            if(attackerName == selfName || (selfAllianceUid != "" && selfAllianceUid == attackerAllianceUid)){
                color = 1;
            }else if(defenderName == selfName || (selfAllianceUid != "" && selfAllianceUid == defenderAllianceUid)){
                color = 0;
            }
        }else{
            if(attackerName == selfName || (selfAllianceUid != "" && selfAllianceUid == attackerAllianceUid)){
                contentStr = _lang_2("110109", attackerNameStr.c_str(), buildingName.c_str());
                color = 0;
            }else{
                if(GlobalData::shared()->playerInfo.gmFlag == 1){
                    contentStr = _lang_2("110109", attackerNameStr.c_str(), buildingName.c_str());
                    color = 0;
                }else{
                    continue;
                }
            }
        }
        FightInfo *info = new FightInfo();
        info->time = time_t(time);
        info->content = contentStr;
        info->color = color;
        info->type = 1;
        m_fightInfos->addObject(info);
        CC_SAFE_RELEASE(info);
    }
    CCPoint curr = m_tabView->getContentOffset();
    CCPoint min = m_tabView->minContainerOffset();
    m_tabView->reloadData();
    curr.y += m_tabView->minContainerOffset().y - min.y;
    m_tabView->setContentOffset(curr);
}

void WarBuildInfoPopUpView::sortData(){
    int num = m_recordKings->count();
    for (int i=0; i<num; i++) {
        for (int j=i; j<num; j++) {
            FightInfo* info1 = (FightInfo*)m_recordKings->objectAtIndex(i);
            FightInfo* info2 = (FightInfo*)m_recordKings->objectAtIndex(j);
            if(info1->kPeriodId < info2->kPeriodId){
                m_recordKings->swap(i, j);
            }
        }
    }
}

void WarBuildInfoPopUpView::dataBack(CCObject *obj){
    isDataBack = true;
    m_iconContainer->removeAllChildren();
    auto params = _dict(obj);
    if(params){
        std::string kingName = params->valueForKey("name")->getCString();
        double openTime = params->valueForKey("opt")->doubleValue() * 1000;
        double startTime = params->valueForKey("st")->doubleValue() * 1000;
        double endTime = params->valueForKey("et")->doubleValue() * 1000;
        std::string kingAllianceName = params->valueForKey("allianceName")->getCString();
        std::string picName = params->valueForKey("pic")->getCString();
        int picVer = params->valueForKey("picVer")->intValue();
        string uid = params->valueForKey("uid")->getCString();
        std::string alliancePicName = params->valueForKey("allianceIcon")->getCString();
        int snum = params->valueForKey("currSoldiers")->intValue();
        int tnum = params->valueForKey("maxSoldiers")->intValue();
        if(alliancePicName == "" && kingAllianceName != ""){
            alliancePicName = "Allance_flay";
        }
        type = WorldController::getInstance()->getKingActivityState(openTime, startTime, endTime);
        
        if(m_info.cityType == Throne){
            m_titleText->setString(_lang("110070"));
            if(type == WarTime){
                m_occupyTitle->setString(_lang("110028"));
            }else{
                m_occupyTitle->setString(_lang("110037"));
            }
        }else{
            m_titleText->setString(_lang("110081"));
            m_occupyTitle->setString(_lang("110028"));
        }
        if(type == NotOpen){
            m_occupyTitle->setString("");
            nextRefreshTime = 0;
            m_timeText->setString("");
        }else{
            m_picNode->removeAllChildren();
            m_soldierNode->removeAllChildren();
            m_iconContainer->removeAllChildren();
            
            if(type == OpenNoKing){
                m_occupyTitle->setString("");
                nextRefreshTime = startTime;
            }else if(type == WarTime){
                nextRefreshTime = endTime;
            }else if(type == PeaceTime){
                if(m_info.cityType == Throne && kingName != ""){
                    auto kingSprite = CCLoadSprite::createSprite("icon_216000.png");
                    auto size = kingSprite->getContentSize();
                    m_iconContainer->addChild(kingSprite);
                    int w = m_occupyTitle->getContentSize().width * m_occupyTitle->getOriginScaleX();
                    m_iconContainer->setPositionX(-w / 2 - size.width / 2 - 5);
                }
                nextRefreshTime = startTime;
            }
            if(picName != ""){
                auto pic = CCLoadSprite::createSprite((picName + ".png").c_str());
                pic->setScale(0.5);
                m_picNode->addChild(pic);
                if (CCCommonUtils::isUseCustomPic(picVer))
                {
                    if (m_headImgNode == NULL)
                    {
                        m_headImgNode = HFHeadImgNode::create();
                    }
                    m_headImgNode->initHeadImgUrl2(m_picNode, CCCommonUtils::getCustomPicUrl(uid, picVer), 1.0f, 60, true);
                }
            }
            if(alliancePicName != ""){
                m_allianceNode->removeAllChildren();
                auto alliancePic = CCLoadSprite::createSprite((alliancePicName + ".png").c_str());
                alliancePic->setScale(0.5);
                m_allianceNode->addChild(alliancePic);
            }
            if(kingName == ""){
                m_occupyTitle->setString("");
            }
            m_allianceName->setString(kingAllianceName.c_str());
            m_occupyName->setString(kingName.c_str());
            if(tnum != 0 && (GlobalData::shared()->playerInfo.allianceInfo.name == kingAllianceName
                             || kingName == GlobalData::shared()->playerInfo.name
                             || GlobalData::shared()->playerInfo.gmFlag == 1)){
                m_soldierNode->addChild(CCLoadSprite::createSprite("go_marching.png"));
                m_soldierText->setString(std::string(CC_ITOA(snum)) + " / " + CC_ITOA(tnum));
            }
        }
        
        update(1.0f);
    }
}

void WarBuildInfoPopUpView::update(float time){
    if(!isDataBack){
        return;
    }
    if(nextRefreshTime == 0){
        return;
    }
    int dTime = ceil((nextRefreshTime - WorldController::getInstance()->getTime()) / 1000);
    if(dTime >= 0){
        m_timeText->setString(CCCommonUtils::timeLeftToCountDown(dTime));
        if(type == OpenNoKing){
            m_timeTitle->setString(_lang_1("110074", ""));
        }else if(type == PeaceTime){
            m_timeTitle->setString(_lang_1("110072", ""));
        }else if(type == WarTime){
            m_timeTitle->setString(_lang_1("110071", ""));
        }else{
            m_timeText->setString("");
        }
    }else{
        m_timeText->setString("");
        getData();
    }
}

std::string WarBuildInfoPopUpView::getCellString(FightInfo *info){
    return info->content;
}

CCSize WarBuildInfoPopUpView::tableCellSizeForIndex(CCTableView *table, ssize_t idx){
    if(m_currentTab==3){
        if(idx >= m_recordKings->count()){
            return CCSizeZero;
        }
        if (CCCommonUtils::isIosAndroidPad()) {
            return CCSize(1152, 252);
        }
        return CCSize(640, 140.0);
    }
    if(idx >= m_fightInfos->count()){
        return CCSizeZero;
    }
    FightInfo *info = dynamic_cast<FightInfo*>(m_fightInfos->objectAtIndex(idx));
    std::string str = getCellString(info);
    m_testTxt->setString(str.c_str());
    int msgHeight = m_testTxt->getContentSize().height * m_testTxt->getOriginScaleY() + 6;
    if (CCCommonUtils::isIosAndroidPad()) {
        msgHeight = MAX(msgHeight, 100);
        msgHeight = msgHeight + 8;
        msgHeight += 40;
        
        return CCSize(1152, msgHeight);
    }
    else {
        msgHeight = MAX(msgHeight, 56);
        msgHeight = msgHeight + 8;
        msgHeight += 40;
        
        return CCSize(640, msgHeight);
    }
}

CCSize WarBuildInfoPopUpView::cellSizeForTable(CCTableView *table){
    if(m_currentTab==3){
        if (CCCommonUtils::isIosAndroidPad()) {
            CCSize(1152, 252);
        }
        CCSize(640, 140.0);
    }
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1152, 108);
    }
    return CCSize(640, 60);
}

CCTableViewCell* WarBuildInfoPopUpView::tableCellAtIndex(CCTableView *table, ssize_t idx){
    if(m_currentTab==3){
        if(idx >= m_recordKings->count()){
            return NULL;
        }
        FightInfoCell* kcell = (FightInfoCell*)table->dequeueCell();
        FightInfo *info = dynamic_cast<FightInfo*>(m_recordKings->objectAtIndex(idx));
        if(idx < m_recordKings->count()){
            if(kcell == NULL){
                kcell = FightInfoCell::create();
            }
            kcell->setData(info,idx);
        }
        return kcell;
    }
    if(idx >= m_fightInfos->count()){
        return NULL;
    }
    FightInfoCell* cell = (FightInfoCell*)table->dequeueCell();
    FightInfo *info = dynamic_cast<FightInfo*>(m_fightInfos->objectAtIndex(idx));
    if(idx < m_fightInfos->count()){
        if(cell == NULL){
            cell = FightInfoCell::create();
        }
        cell->setData(info);
    }
    return cell;
}

ssize_t WarBuildInfoPopUpView::numberOfCellsInTableView(CCTableView *table){
    int size = m_fightInfos->count();
    if(m_currentTab==3){
        size = m_recordKings->count();
    }
    return size;
}

void WarBuildInfoPopUpView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell){
    
}

bool WarBuildInfoPopUpView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (!isTouchInside(m_infoBtn, pTouch) && !isTouchInside(m_touchNode, pTouch)) {
        this->closeSelf();
    }
//    if(m_historyNode->isVisible() && isTouchInside(m_listNode, pTouch)){
//        m_touchPos = pTouch->getLocation();
//        return true;
//    }
    return true;
}

void WarBuildInfoPopUpView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CCPoint pos = pTouch->getLocation();
    if(fabsf(pos.y - m_touchPos.y)>30){
        CCPoint curr = m_tabView->getContentOffset();
        CCPoint min = m_tabView->minContainerOffset();
        CCPoint maxPos = m_tabView->maxContainerOffset();
        if((curr.y - maxPos.y) > 20 && m_currentTab==3 && m_curPage<=m_totalPage){
            m_tabView->setTouchEnabled(false);
            m_tabView->stopAllActions();
            m_tabView->unscheduleAllSelectors();
            m_tabView->getContainer()->unscheduleAllSelectors();
            m_tabView->getContainer()->stopAllActions();
            this->getKingRecordData();
        }else if((curr.y - min.y) > 40){
            this->getHistoryData();
        }
    }
}

void WarBuildInfoPopUpView::getKingRecordData(){
    int serverId = GlobalData::shared()->playerInfo.currentServerId;
    GetFightOfKingRecord *cmd = new GetFightOfKingRecord(serverId,m_curPage);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(WarBuildInfoPopUpView::kingRecordCallBack), NULL));
    cmd->sendAndRelease();
    m_curPage += 1;
    GameController::getInstance()->showWaitInterface();
}

void WarBuildInfoPopUpView::kingRecordCallBack(CCObject *param){
    m_tabView->setTouchEnabled(true);
    GameController::getInstance()->removeWaitInterface();
    NetResult* result = dynamic_cast<NetResult*>(param);
    auto dic = _dict(result->getData());
    if (dic->objectForKey("totalPage")) {
        m_totalPage = dic->valueForKey("totalPage")->intValue();
    }
    if ((dic && dic->objectForKey("list"))) {
        CCArray* arr = dynamic_cast<CCArray*>(dic->objectForKey("list"));
        int num = arr->count();
        for (int i=0; i<num; i++) {
            auto dic = _dict(arr->objectAtIndex(i));
            string pic = dic->valueForKey("kPic")->getCString();
            string kName = dic->valueForKey("kName")->getCString();
            string kUid = dic->valueForKey("kUid")->getCString();
            string kAbbr = dic->valueForKey("kAbbr")->getCString();
            double time = dic->valueForKey("kTime")->doubleValue()/1000;
            int kPeriodId = dic->valueForKey("kPeriodId")->intValue();
            int picVer = dic->valueForKey("kPicVer")->intValue();
            string kTimeStr = dic->valueForKey("kTime")->getCString();
            if(m_kTimes.find(kTimeStr)<m_kTimes.length()){
                //有相同的，不要
                continue;
            }
            m_kTimes.append(",");
            m_kTimes.append(kTimeStr);
            FightInfo *info = new FightInfo();
            info->time = time_t(time);
            info->content = "";
            info->color = 1;
            info->type = 2;
            info->pic = pic;
            info->picVer = picVer;
            info->kName = kName;
            info->kUid = kUid;
            info->abbr = kAbbr;
            info->kPeriodId = kPeriodId;
            m_recordKings->addObject(info);
            CC_SAFE_RELEASE(info);
        }
    }
    this->sortData();
    CCPoint curr = m_tabView->getContentOffset();
    CCPoint min = m_tabView->minContainerOffset();
    m_tabView->reloadData();
    curr.y += m_tabView->minContainerOffset().y - min.y;
    m_tabView->setContentOffset(curr);
    if(m_recordKings->count()<=0){
        m_noRecordText->setString(_lang("110135"));
    }else{
        m_noRecordText->setString("");
    }
}

void WarBuildInfoPopUpView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

//-------

//
FightInfoCell *FightInfoCell::create(){
    FightInfoCell *ret = new FightInfoCell();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool FightInfoCell::init(){
    CCLoadSprite::doResourceByCommonIndex(7, true);
    auto node = CCBLoadFile("FightInfoCellCCB", this, this);
    this->setContentSize(node->getContentSize());
    m_headImgNode = HFHeadImgNode::create();
    return true;
}

void FightInfoCell::onEnter(){
    CCNode::onEnter();
}

void FightInfoCell::onExit(){
    CCNode::onExit();
}

void FightInfoCell::setData(FightInfo* info,int idx){
    CCLoadSprite::doResourceByCommonIndex(7, true);
    m_info = info;
    m_idx = idx;
    m_icon->removeAllChildrenWithCleanup(true);
    if (m_info->type==2) {
        m_historyNode->setVisible(false);
        m_kingNode->setVisible(true);
        if (m_idx==0) {
            m_renderBg1->setVisible(false);
            m_renderBg2->setVisible(true);
            m_txt1->setColor({255,231,181});
            m_txt2->setColor({255,231,181});
            m_txt3->setColor({255,255,255});
        }else{
            m_renderBg1->setVisible(true);
            m_renderBg2->setVisible(false);
            m_txt1->setColor({218,170,58});
            m_txt2->setColor({218,170,58});
            m_txt3->setColor({218,170,58});
        }
        m_txt1->setString(_lang_1("110132", CC_ITOA(m_info->kPeriodId)));
        string timeStr = CCCommonUtils::timeStampToYMD(m_info->time);
        m_txt2->setString(_lang_1("110133", timeStr.c_str()));
        string name = "";
        if(m_info->abbr!=""){
            name.append("(");
            name.append(m_info->abbr);
            name.append(")");
        }
        name.append(m_info->kName);
        m_txt3->setString(name.c_str());
        string pic = m_info->pic;
        if (pic=="" || pic=="0") {
            pic = "g044.png";
        }else{
            pic.append(".png");
        }
        auto picSpr = CCLoadSprite::createSprite(pic.c_str());
        picSpr->setScale(0.54);
        m_icon->addChild(picSpr);
        if (CCCommonUtils::isUseCustomPic(m_info->picVer))
        {
            m_headImgNode->initHeadImgUrl2(m_icon, CCCommonUtils::getCustomPicUrl(m_info->kUid, m_info->picVer), 1.0f, 73, true);
        }
    }else{
        m_historyNode->setVisible(true);
        m_kingNode->setVisible(false);
        m_contentTxt->setString(m_info->content.c_str());
        if(info->color == 1){
            m_cellBg->initWithSpriteFrame(CCLoadSprite::loadResource("Alliance_info_Red.png"));
            m_contentTxt->setColor({255,0,0});
        }else if(info->color == 2){
            m_cellBg->initWithSpriteFrame(CCLoadSprite::loadResource("Alliance_info_blue.png"));
            m_contentTxt->setColor({255,255,255});
        }else{
            m_cellBg->initWithSpriteFrame(CCLoadSprite::loadResource("Alliance_info_green.png"));
            m_contentTxt->setColor({0,255,0});
        }
        m_timeTxt->setColor({143,143,143});
        m_node2->setVisible(true);
        
        time_t infoTime = GlobalData::shared()->getTimeStamp();
        string lastTime = CCCommonUtils::timeStampToDHM(infoTime).substr(0,10);
        string nowTime = CCCommonUtils::timeStampToDHM(m_info->time).substr(0,10);
        if (lastTime == nowTime) {
            m_timeTxt->setString(CCCommonUtils::timeStampToHour(m_info->time).c_str());
        }
        else {
            m_timeTxt->setString(CCCommonUtils::timeStampToDHM(m_info->time).c_str());
        }
        int msgHeight = m_contentTxt->getContentSize().height*m_contentTxt->getOriginScaleY() + 6;
        msgHeight = MAX(msgHeight, 56);
        m_cellBg->setContentSize(CCSizeMake(534.0, msgHeight));
        m_cellBg->setAnchorPoint(ccp(0.5,0));
        m_moveNode->setPositionY(msgHeight-52);
        m_node2->setPositionY(msgHeight -10);
        m_clickBg->setContentSize(CCSizeMake(534.0, msgHeight));
        m_clickBg->setAnchorPoint(ccp(0.5,0));
    }
}

int FightInfoCell::getMaxHeight(){
    if(m_node2->isVisible()){
        return m_cellBg->getContentSize().height + 8 + 32;
    }
    return m_cellBg->getContentSize().height+8;
}

bool FightInfoCell::checkIsEventInSide(cocos2d::CCTouch *pTouch){
    bool flag = false;
    if(isTouchInside(m_clickBg, pTouch)){
        flag = true;
    }
    return flag;
}

bool FightInfoCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moveNode", CCNode*, this->m_moveNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon", CCNode*, this->m_icon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cellBg", CCScale9Sprite*, this->m_cellBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickBg", CCScale9Sprite*, this->m_clickBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_contentTxt", CCLabelIFTTF*, this->m_contentTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node1", CCNode*, this->m_node1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node2", CCNode*, this->m_node2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt1", CCLabelIF*, this->m_txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt2", CCLabelIF*, this->m_txt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt3", CCLabelIFTTF*, this->m_txt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_historyNode", CCNode*, this->m_historyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_kingNode", CCNode*, this->m_kingNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_renderBg1", CCScale9Sprite*, this->m_renderBg1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_renderBg2", CCScale9Sprite*, this->m_renderBg2);
    return false;
}

/////
WarNoticeInfoView* WarNoticeInfoView::create()
{
    WarNoticeInfoView* ret = new WarNoticeInfoView();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_RELEASE(ret);
    }
    return ret;
}

bool WarNoticeInfoView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    
    CCLoadSprite::doResourceByCommonIndex(7, true);
    CCLoadSprite::doResourceByCommonIndex(208, true);
    CCLoadSprite::doResourceByCommonIndex(310, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(7, false);
        CCLoadSprite::doResourceByCommonIndex(208, false);
        CCLoadSprite::doResourceByCommonIndex(310, false);
    });

    CCBLoadFile("WarNoticeInfo", this, this);
    this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    
    m_titleLabel->setString(_lang("110070"));
    m_infoLabel1->setString(_lang("110136"));
    m_infoLabel2->setString(_lang("110137"));
    m_infoLabel3->setString(_lang("110138"));
    m_infoLabel4->setString(_lang("110139"));
    
    m_scrollView = CCScrollView::create(m_listNode->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_scrollView->setAnchorPoint(ccp(0, 0));
    m_scrollView->setPosition(ccp(0, 0));
    m_listNode->addChild(m_scrollView);
    
    m_infoLabel4->removeFromParent();
    m_infoLabel3->removeFromParent();
    m_infoLabel2->removeFromParent();
    m_infoLabel1->removeFromParent();
    m_scrollView->addChild(m_infoLabel4);
    m_scrollView->addChild(m_infoLabel3);
    m_scrollView->addChild(m_infoLabel2);
    m_scrollView->addChild(m_infoLabel1);
    
    int curY = 0;
    int dY = 30;
    int dX = 16;
    
    curY = dY;
    m_infoLabel4->setPosition(ccp(dX, curY));
    curY += m_infoLabel4->getContentSize().height * m_infoLabel4->getOriginScaleY();
    curY += dY;
    m_infoLabel3->setPosition(ccp(dX, curY));
    curY += m_infoLabel3->getContentSize().height * m_infoLabel3->getOriginScaleY();
    curY += dY;
    m_infoLabel2->setPosition(ccp(dX, curY));
    curY += m_infoLabel2->getContentSize().height * m_infoLabel2->getOriginScaleY();
    curY += dY;
    m_infoLabel1->setPosition(ccp(dX, curY));
    curY += m_infoLabel1->getContentSize().height * m_infoLabel1->getOriginScaleY();
    curY += dY;
    m_scrollView->setContentSize(CCSize(m_listNode->getContentSize().width, curY));
    m_scrollView->setContentOffset(ccp(0, m_listNode->getContentSize().height - m_scrollView->getContentSize().height));
    
    
    return true;
}

void WarNoticeInfoView::onEnter()
{
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void WarNoticeInfoView::onExit()
{
    GlobalData::shared()->isBind = false;
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool WarNoticeInfoView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void WarNoticeInfoView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isTouchInside(m_touchNode, pTouch)) {
        PopupViewController::getInstance()->removePopupView(this);
    }
}

void WarNoticeInfoView::onClickHelpBtn(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    GlobalData::shared()->isBind = true;
#endif
    CCCommonUtils::showHelpShiftFAQ("45201");
//    HelpshiftCocos2dx::showFAQSection("466");
}

bool WarNoticeInfoView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel1", CCLabelIF*, m_infoLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel2", CCLabelIF*, m_infoLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel3", CCLabelIF*, m_infoLabel3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel4", CCLabelIF*, m_infoLabel4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpBtn", CCControlButton*, m_helpBtn);
    return false;
}

SEL_CCControlHandler WarNoticeInfoView::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickHelpBtn", WarNoticeInfoView::onClickHelpBtn);
    return NULL;
}