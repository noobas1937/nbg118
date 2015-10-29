//
//  PlayerTile.cpp
//  IF
//
//  Created by 邹 程 on 14-2-26.
//
//

#include "PlayerTile.h"
#include "WorldMapView.h"
#include "ScoutInfo.h"
#include "YesNoDialog.h"
#include "AddFavorite.h"
#include "TroopInformationView.h"
#include "RoleInfoView.h"
#include "AllianceMarketView.h"
#include "FunBuildController.h"
#include "AllianceMassTeamView.h"
#include "AllianceManager.h"
#include "AllianceWarView.h"
#include "CheckKingdomView.h"
#include "ManagerKingdomView.h"
#include "KingdomAppointView.h"
#include "ItemStatusView.h"
#include "SoundController.h"
#include "EnemyInfoController.h"
#include "AllianceManagerFunView.h"

bool PlayerTile::init() {
    CCLoadSprite::doResourceByCommonIndex(8, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(8, false);
    });

    NewBaseTileInfo::init();
    m_playerInfo = &WorldController::getInstance()->m_playerInfo[m_cityInfo.playerName];
    m_playerUid = "";
    // set player info
    requestDetail();
    refreshView(NULL);
    addToParent();
    return true;
}

void PlayerTile::requestDetail() {
    int index = m_cityInfo.cityIndex;
    if(m_cityInfo.cityType == MonsterTile || m_cityInfo.cityType == CityTile){
        if(m_cityInfo.parentCityIndex != -1){
            index = m_cityInfo.parentCityIndex;
        }
    }
    auto cmd = new WorldDetailCommand(index,m_cityInfo.tileServerId);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(PlayerTile::onDetailCallback), nullptr));
    cmd->sendAndRelease();
}

void PlayerTile::onDetailCallback(cocos2d::CCObject *obj) {
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : parse error
        return;
    }
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    if (!info) {
        return;
    }
    m_playerUid = info->valueForKey("uid")->getCString();
    
    if (info->valueForKey("targetTooStrong")) {
        targetTooStrong=info->valueForKey("targetTooStrong")->boolValue();
    }else{
        targetTooStrong=false;
    }
    if (info->valueForKey("slowDown")) {
        slowDown=info->valueForKey("slowDown")->boolValue();
    }else{
        slowDown=false;
    }
    
    if (info->objectForKey("registerTime")) {
        registerTime = info->valueForKey("registerTime")->doubleValue() / 1000;
    }
    else
        registerTime = 0;
}

bool PlayerTile::canBeInvited() {
    return false;
}

bool PlayerTile::canBeRallyAttacked() {
    return false;
}

bool PlayerTile::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    return NewBaseTileInfo::onAssignCCBMemberVariable(pTarget, pMemberVariableName, pNode);
}

SEL_CCControlHandler PlayerTile::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NewBaseTileInfo::onResolveCCBCCControlSelector(pTarget, pSelectorName);
}

bool PlayerTile::isInProtect() {
    auto now = WorldController::getInstance()->getTime();
    int left = (m_playerInfo->protectStamp - now)/1000;
    return (left >= 1);
}

bool PlayerTile::isInResourceProtect(){
    auto now = WorldController::getInstance()->getTime();
    int left = (m_playerInfo->resourceProtectStamp - now)/1000;
    return (left >= 1);
}

void PlayerTile::viewTroopInfo(void)
{
    auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
    
    int index = m_cityInfo.cityIndex;
    if(m_cityInfo.parentCityIndex != -1){
        index = m_cityInfo.parentCityIndex;
    }
    
    auto it = selfMarch.find(index);
    
    if (it == selfMarch.end()) {
        // todo : show error
        return;
    }
    string uuid = it->second;
    
    if (uuid.empty()) {
        // todo : show error
        return;
    }
    
    WorldMapView::instance()->addPopupView(TroopInformationView::create(uuid));
}

BaseTileInfo* PlayerTile::getFavoriteView() {
    return AddFavorite::create(m_cityInfo.cityIndex,m_cityInfo.playerName);
}

void PlayerTile::onClickButton(CCObject * pSender, Control::EventType pCCControlEvent) {
    // todo
    auto button = dynamic_cast<CCControlButton*>(pSender);
    auto buttonState = getButtonState(button);
    switch (buttonState) {
        case ButtonSupport:{
            if (m_viewReinforceFlag)
            {
                viewTroopInfo();
            }
            else
            {
                unsigned int index = m_cityInfo.cityIndex;
                if(m_cityInfo.parentCityIndex != -1){
                    index = m_cityInfo.parentCityIndex;
                }
                AllianceManager::getInstance()->openYuanYunView(index);
            }
            closeThis();
            return;
        }
            break;
        case ButtonResourceHelp:{
            unsigned int index = m_cityInfo.cityIndex;
            if(m_cityInfo.parentCityIndex != -1){
                index = m_cityInfo.parentCityIndex;
            }
            auto &player = WorldController::getInstance()->m_playerInfo[m_cityInfo.playerName];
            
            int buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_TAVERN);
            bool haveMarket = false;
            if (buildId>0) {
                haveMarket = true;
            }
            if(haveMarket){
                double now = GlobalData::shared()->getTimeStamp();
                double playingTime = (now - GlobalData::shared()->playerInfo.registerTime) / 3600;
                if (GlobalData::shared()->cnt_forbidden_swith == 1) {
                    if (playingTime <= GlobalData::shared()->cnt_forbidden_playingtime) {
                        YesNoDialog::showVariableTitle(_lang("108793").c_str(),CCCallFunc::create(this, callfunc_selector(YesNoDialog::closeSelf)),_lang("confirm").c_str());
                    }
                    else {
                        AllianceManager::getInstance()->openAllianceTradeView(m_playerUid,player.cityLv,index);
                    }
                }
                else {
                    AllianceManager::getInstance()->openAllianceTradeView(m_playerUid,player.cityLv,index);
                }
            }else{
                CCCommonUtils::flyHint("", "", _lang("115075"));
            }
        }
            break;
        case ButtonMarch: {
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            if (isInProtect() && m_cityInfo.cityType!=CampTile) {
                CCCommonUtils::flyHint("", "", _lang("108623"));
                closeThis();
                return;
            }
            
            if (!m_cityInfo.playerName.empty())
            {
                // have owner
                WorldController::getInstance()->m_alertStateFlag = 2;
            }
            else
            {
                // no owner
                WorldController::getInstance()->m_alertStateFlag = 0;
                
                for(auto it :WorldController::getInstance()->m_marchInfo)
                {
                    if(it.second.stateType == StateMarch && it.second.endPointIndex == this->m_cityInfo.cityIndex)
                    {
                        WorldController::getInstance()->m_alertStateFlag = 1;
                        break;
                    }
                }
            }
            
            // judge self protect state
            auto selfProtect = GlobalData::shared()->playerInfo.protectTimeStamp;
            auto now = WorldController::getInstance()->getTime();
            
            unsigned int index = m_cityInfo.cityIndex;
            if(m_cityInfo.parentCityIndex != -1){
                index = m_cityInfo.parentCityIndex;
            }
            auto func = [&,index](){
                //判断是否是小号速成
                auto &player = WorldController::getInstance()->m_playerInfo[m_cityInfo.playerName];
                double currentTime = GlobalData::shared()->getTimeStamp();
                double playingTime = (currentTime - registerTime) / 3600;
                if (GlobalData::shared()->cnt_forbidden_swith == 1) {
                    if (player.cityLv < GlobalData::shared()->cnt_forbidden_level && playingTime <= GlobalData::shared()->cnt_forbidden_playingtime) {
                        YesNoDialog::marchAlertShow(_lang("108792").c_str(), CCCallFunc::create(this, callfunc_selector(PlayerTile::openMarchDeployWithTips)), CCCallFunc::create(this, callfunc_selector(YesNoDialog::closeSelf)));
                    }
                    else {
                        openMarchDeployWithTips();
                    }
                }
                else {
                    openMarchDeployWithTips();
                }

//                //判断是否显示提示
//                if(targetTooStrong){
//                    YesNoDialog* dialog = YesNoDialog::showVariableTitle(_lang("105152").c_str(),CCCallFunc::create(this, callfunc_selector(PlayerTile::gotoOpenMarchDeploy)),_lang("105153").c_str());
//                    CCCommonUtils::setButtonTitle(dialog->m_btnCancel, _lang("cancel_btn_label").c_str());
//                    dialog->showCancelButton();
//                }else{
//                    gotoOpenMarchDeploy();
//                }
////                WorldController::getInstance()->openMarchDeploy(index,1);
            };
            
            if (now < selfProtect) {
                YesNoDialog::show(_lang("101438").c_str(),func);
            }else if (isInResourceProtect()){
                YesNoDialog::show(_lang("103764").c_str(),func);
            }else {
                func();
            }
        }
            break;
        case ButtonProfile: {
            if(m_playerUid!=""){
                SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
                RoleInfoView::createInfoByUid(m_playerUid);
            }
        }
            break;
        case ButtonScout: {
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
                YesNoDialog::show(_lang("101438").c_str(),func);//E100050
            } else {
                func();
            }
        }
            break;
        case ButtonRally: {
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
            //CCCommonUtils::flyHint("", "", _lang("E100008"));
            auto selfProtect = GlobalData::shared()->playerInfo.protectTimeStamp;
            auto now = WorldController::getInstance()->getTime();
            auto func = [&,index](){
                //判断是否为小号速成
                auto &player = WorldController::getInstance()->m_playerInfo[m_cityInfo.playerName];
                double currentTime = GlobalData::shared()->getTimeStamp();
                double playingTime = (currentTime - registerTime) / 3600;
                if (GlobalData::shared()->cnt_forbidden_swith == 1) {
                    if (player.cityLv < GlobalData::shared()->cnt_forbidden_level && playingTime <= GlobalData::shared()->cnt_forbidden_playingtime) {
                        YesNoDialog::marchAlertShow(_lang("108792").c_str(), CCCallFunc::create(this, callfunc_selector(PlayerTile::openAllianceMassView)), CCCallFunc::create(this, callfunc_selector(YesNoDialog::closeSelf)));
                    }
                    else {
                        openAllianceMassView();
                    }
                }
                else {
                    openAllianceMassView();
                }
//                AllianceManager::getInstance()->openAllianceMassView(index);
//                closeThis();
            };
            if (now < selfProtect) {
                YesNoDialog::show(_lang("101438").c_str(),func);
            } else {
                func();
                closeThis();
            }
            return;
        }
            break;
        case ButtonViewTroop: {
            viewTroopInfo();
        }
            break;
        case ButtonGoHome: {
            auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
            int index = m_cityInfo.cityIndex;
            if(m_cityInfo.parentCityIndex != -1){
                index = m_cityInfo.parentCityIndex;
            }
            auto it = selfMarch.find(index);
            if (it == selfMarch.end()) {
                // todo : show error
                return;
            }
            string uuid = it->second;
            
            if (uuid.empty()) {
                // todo : show error
                return;
            }
            
            if(m_cityInfo.cityType == CampTile){
                showCampHomeBackTip();
            }else{
                auto dict = CCDictionary::create();
                dict->setObject(CCString::create(uuid), "marchId");
                WorldMapView::instance()->afterMarchCancel(dict);
            }
        }
            break;
        case ButtonEnterCity: {
            WorldMapView::instance()->leaveWorld();
        }
            break;
        case ButtonAddEffect:{
            PopupViewController::getInstance()->addPopupInView(ItemStatusView::create());
        }
            break;
        case ButtonOrderIcon: {
            GameController::getInstance()->callXCApi("action=alOrder_open_w");
            PopupViewController::getInstance()->addPopupView(AllianceOrderView::create(m_playerUid));
        }
            break;
        case ButtonInvite: {
            
        }
            break;
        case ButtonRalliedTroops: {
            PopupViewController::getInstance()->addPopupInView(AllianceWarView::create());
        }
            break;
        case AppointOfficals: {
            //RoleInfoView::createInfoByUid(m_playerUid);
            //PopupViewController::getInstance()->addPopupInView(CheckKingdomView::create());
            //PopupViewController::getInstance()->addPopupInView(ManagerKingdomView::create());
            
            PopupViewController::getInstance()->addPopupInView(KingdomAppointView::create(m_playerUid,m_cityInfo.playerName));
        }
        case ButttonTreasureMapSPD:{
            double now = WorldController::getInstance()->getTime();
            auto iter = WorldController::getInstance()->m_marchInfo.begin();
            while (iter!=WorldController::getInstance()->m_marchInfo.end()) {
                if((*iter).second.targetType == CampTile && (*iter).second.ownerType == PlayerSelf){
                    if(((*iter).second.endPointIndex == m_cityInfo.cityIndex) && ((*iter).second.treasureMapFinishTime>0)){
                        double now = WorldController::getInstance()->getTime();
                        if((*iter).second.treasureMapFinishTime > now){
                            m_marchId = (*iter).first;
                            int m_curTime = ((*iter).second.treasureMapFinishTime-now)/1000;
                            int cost = CCCommonUtils::getGoldByTime(m_curTime);
                            YesNoDialog::showTime( _lang("102283").c_str() , CCCallFunc::create(this, callfunc_selector(PlayerTile::onCostGoldBack)), m_curTime, _lang("104903").c_str(), false);
                            break;
                        }
                    }
                }
                ++iter;
            }
            break;
        }
        default:
            break;
    }
    if(buttonState != ButtonEnterCity){
        closeThis();
    }
}
void PlayerTile::onCostGoldBack(){
    double now = WorldController::getInstance()->getTime();
    auto iter = WorldController::getInstance()->m_marchInfo.begin();
    while (iter!=WorldController::getInstance()->m_marchInfo.end()) {
        if((*iter).second.targetType == CampTile && (*iter).second.ownerType == PlayerSelf){
            if(((*iter).second.endPointIndex == m_cityInfo.cityIndex) && ((*iter).second.treasureMapFinishTime>0)){
                double now = WorldController::getInstance()->getTime();
                if((*iter).second.treasureMapFinishTime > now){
                    m_marchId = (*iter).first;
                    int m_curTime = ((*iter).second.treasureMapFinishTime -now)/1000;
                    int cost = CCCommonUtils::getGoldByTime(m_curTime);
                    if (GlobalData::shared()->playerInfo.gold < cost) {
                        YesNoDialog::gotoPayTips();
                        return;
                    }
                    WorldController::getInstance()->speedupTreasureMap(m_marchId);
                    break;
                }
            }
        }
        ++iter;
    }
}
void PlayerTile::onEnter(){
    NewBaseTileInfo::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(PlayerTile::refreshView), WORLD_MAP_REFRESH, NULL);
}

void PlayerTile::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, WORLD_MAP_REFRESH);
    NewBaseTileInfo::onExit();
}

void PlayerTile::refreshView(CCObject * obj){
    unsigned int buttonCount = 0;
    if(!WorldController::isInSelfServer(m_cityInfo.tileServerId) || m_cityInfo.cross==1 || (GlobalData::shared()->isCrossService && m_cityInfo.srcServer>0 && m_cityInfo.srcServer == GlobalData::shared()->playerInfo.crossFightSrcServerId)){
        if (m_cityInfo.srcServer>0 && m_cityInfo.srcServer == GlobalData::shared()->playerInfo.crossFightSrcServerId && m_playerInfo->type==PlayerAlliance && GlobalData::shared()->playerInfo.isInAlliance()) {
            buttonCount = 2;
            setButtonCount(buttonCount);
            setButtonName(2, _lang("108721"));
            setButtonName(3, _lang("108729"));
            setButtonState(2, ButtonProfile);
            setButtonState(3, ButtonResourceHelp);
        }else{
            buttonCount = 1;
            setButtonCount(buttonCount);
            setButtonName(1, _lang("108721"));
            setButtonState(1, ButtonProfile);
        }
    }else if(GlobalData::shared()->playerInfo.currentServerId>CROSS_SERVER_START_INDEX && m_cityInfo.cityType == CampTile && GlobalData::shared()->isCrossService && m_cityInfo.srcServer>0 && m_cityInfo.srcServer == GlobalData::shared()->playerInfo.crossFightSrcServerId){
        m_title->setString(_lang("108632"));
        if(m_cityInfo.playerName != "" && m_cityInfo.playerName != GlobalData::shared()->playerInfo.name){
            addNameNode();
        }
        buttonCount = 1;
        setButtonCount(buttonCount);
        setButtonName(1, _lang("108721"));
        setButtonState(1, ButtonProfile);
    }else{
        if (m_cityInfo.cityType == CampTile) {
            m_title->setString(_lang("108632"));
            if(m_cityInfo.playerName != "" && m_cityInfo.playerName != GlobalData::shared()->playerInfo.name){
                addNameNode();
            }
            // camp
            switch (m_playerInfo->type) {
                case PlayerSelf:{
                    buttonCount = 2;
                    setButtonCount(buttonCount);
                    setButtonName(2, _lang("108724"));
                    setButtonName(3, _lang("108725"));
                    setButtonState(2, ButtonViewTroop);
                    setButtonState(3, ButtonGoHome);
                    
                    auto iter = WorldController::getInstance()->m_marchInfo.begin();
                    while (iter!=WorldController::getInstance()->m_marchInfo.end()) {
                        if((*iter).second.targetType == CampTile && (*iter).second.ownerType == PlayerSelf){
                            if(((*iter).second.endPointIndex == m_cityInfo.cityIndex) && ((*iter).second.treasureMapFinishTime>0)){
                                double now = WorldController::getInstance()->getTime();
                                if((*iter).second.treasureMapFinishTime > now){
                                    m_marchId = (*iter).first;
                                    setButtonName(1, _lang("108558"));
                                    setButtonState(1, ButttonTreasureMapSPD);
                                    break;
                                }
                            }
                        }
                        ++iter;
                    }
                }
                    break;
                case PlayerAlliance:{
                    buttonCount = 1;
                    setButtonCount(buttonCount);
                    setButtonName(1, _lang("108721"));
                    setButtonState(1, ButtonProfile);
                }
                    break;
                
                case PlayerOther:{
                    buttonCount = 3;
                    int bid = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_WATCH_TOWER);
                    int watchTowerLv = GlobalData::shared()->imperialInfo[bid].level;
                    if (watchTowerLv>1) {//如果瞭望塔等级等于1就不显示侦查按钮
                        setButtonCount(buttonCount);
                        setButtonName(1, _lang("108722"));
                        setButtonName(2, _lang("108721"));
                        setButtonName(3, _lang("108723"));
                        setButtonState(1, ButtonScout);
                        setButtonState(2, ButtonProfile);
                        setButtonState(3, ButtonMarch);
                    }else{
                        setButtonCount(buttonCount-1);
                        setButtonName(2, _lang("108721"));
                        setButtonName(3, _lang("108723"));
                        setButtonState(2, ButtonProfile);
                        setButtonState(3, ButtonMarch);
                    }
                    
                }
                    break;
                default: {
                    return;
                }
            }
        } else {
            std::string str = "";
            if(m_cityInfo.leagueAsn != ""){
                str = str + "(" + m_cityInfo.leagueAsn + ")";
            }
            str += m_cityInfo.playerName;
            m_title->setString(str);
        
        // city
            switch (m_playerInfo->type) {
                case PlayerSelf:{
                    buttonCount = 3;
                    setButtonCount(buttonCount);
                    setButtonName(2, _lang("108742"));
                    setButtonState(2, ButtonProfile);
                
                    setButtonName(1, _lang("102282"));
                    setButtonState(1, ButtonAddEffect);

                    setButtonName(3, _lang("108727"));
                    setButtonState(3, ButtonEnterCity);
                }
                    break;
                case PlayerAlliance:{
                    bool rallyFlag = false;
                    m_viewReinforceFlag = false;
                    for(auto it = GlobalData::shared()->allQueuesInfo.begin(); it != GlobalData::shared()->allQueuesInfo.end(); it++){
                        if(it->second.type == TYPE_OCCUPY_ALLIANCE){
                            auto marchIt = WorldController::getInstance()->m_marchInfo.find(it->second.uuid);
                            if(marchIt != WorldController::getInstance()->m_marchInfo.end()){
                                if(marchIt->second.endPointIndex == m_cityInfo.parentCityIndex){
                                    m_viewReinforceFlag = true;
                                }
                            }
                        }
                        else if (it->second.type == TYPE_ARMY_MASS){
                            auto marchIt = WorldController::getInstance()->m_marchInfo.find(it->second.uuid);
                            if(marchIt != WorldController::getInstance()->m_marchInfo.end()){
                                if(marchIt->second.endPointIndex == m_cityInfo.parentCityIndex
                                   || marchIt->second.startPointIndex == m_cityInfo.parentCityIndex){
                                    rallyFlag = true;
                                }
                            }
                        }
                    }
                    if (rallyFlag)
                    {
                        buttonCount = 4;
                    }
                    else
                    {
                        buttonCount = 3;
                    }
                    
                    int rank = GlobalData::shared()->playerInfo.allianceInfo.rank;
                    string leaderUid = GlobalData::shared()->playerInfo.allianceInfo.leaderUid;
                    if (CCCommonUtils::isTestPlatformAndServer("al_order") && rank>=4 && m_playerUid != leaderUid)
                    {
                        if ( (buttonCount == 4  &&  GlobalData::shared()->playerInfo.uid!=leaderUid) || buttonCount == 3) {
                            if (buttonCount == 4) {
                                buttonCount = 5;
                            }
                            else {
                                buttonCount = 4;
                            }
                        }
                    }
                
                    setButtonCount(buttonCount);
                    setButtonName(2, _lang("108552"));
                    setButtonState(2, ButtonProfile);
                    setButtonName(3, _lang("108729"));
                    setButtonState(3, ButtonResourceHelp);
                
                    if (rallyFlag)
                    {
                        if (m_viewReinforceFlag)
                        {
                            setButtonName(4, _lang("108741"));   //view Reinforce
                        }
                        else
                        {
                            setButtonName(4, _lang("108728"));   //Reinforce
                        }
                        setButtonName(5, _lang("108739"));   //Rallied troops
                        setButtonState(5, ButtonRalliedTroops);
                        setButtonState(4, ButtonSupport);
                        
                        if (buttonCount == 5) {
                            //显示指令按钮
                            setButtonName(1, _lang("115429"));
                            setButtonState(1, ButtonOrderIcon);
                        }
                    }
                    else
                    {
                        if (buttonCount == 4) {
                            //显示指令按钮
                            setButtonName(4, _lang("115429"));
                            setButtonState(4, ButtonOrderIcon);
                            if (m_viewReinforceFlag) {
                                setButtonName(5, _lang("108741"));
                            }else{
                                setButtonName(5, _lang("108728"));
                            }
                            setButtonState(5, ButtonSupport);
                            setButtonEnable(5);
                        }
                        else {
                            if (m_viewReinforceFlag) {
                                setButtonName(1, _lang("108741"));
                            }else {
                                setButtonName(1, _lang("108728"));
                            }
                            setButtonState(1, ButtonSupport);
                            setButtonEnable(1);
                        }
                    }
                    
                    if(GlobalData::shared()->playerInfo.officer==KINGDOM_KING_ID && WorldController::getInstance()->isInSelfServer(m_cityInfo.tileServerId) && !GlobalData::shared()->isCrossService){//&&
                        buttonCount = buttonCount+1;
                        setButtonName(6, _lang("110064"));
                        setButtonState(6, AppointOfficals);
                    }

                }
                    break;
                
                case PlayerOther:{
                    int warBuildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_SMITHY);
                    bool haveWarBuild = false;
                    if (warBuildId>0) {
                        FunBuildInfo& info = FunBuildController::getInstance()->getFunbuildById(warBuildId);
                        if(&info!=NULL){
                            haveWarBuild = true;
                        }
                    }
                    auto &player = WorldController::getInstance()->m_playerInfo[m_cityInfo.playerName];
                    buttonCount = 3;
                    if(haveWarBuild && GlobalData::shared()->playerInfo.isInAlliance()){
                        if(player.cityLv >= GlobalData::shared()->world_alliance_war_k1 || m_cityInfo.leagueAsn!=""){
                            buttonCount = 4;
                        }
                    }
                    int bid = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_WATCH_TOWER);
                    int watchTowerLv = GlobalData::shared()->imperialInfo[bid].level;
                    if (watchTowerLv>1) {//如果瞭望塔等级等于1就不显示侦查按钮
                        if(buttonCount==3){
                            setButtonCount(buttonCount);
                            setButtonName(3, _lang("108723"));
                            setButtonName(2, _lang("108721"));
                            setButtonName(1, _lang("108722"));
                            setButtonState(3, ButtonMarch);
                            setButtonState(2, ButtonProfile);
                            setButtonState(1, ButtonScout);
                        }else{
                            buttonCount = 4;
                            setButtonCount(buttonCount);
                            setButtonName(3, _lang("108723"));
                            setButtonName(2, _lang("108721"));
                            setButtonName(5, _lang("108722"));
                            setButtonName(4, _lang("108726"));
                            setButtonState(3, ButtonMarch);
                            setButtonState(2, ButtonProfile);
                            setButtonState(5, ButtonScout);
                            setButtonState(4, ButtonRally);
                        }
                    }else{
                        if(buttonCount==3){
                            setButtonCount(buttonCount-1);
                            setButtonName(3, _lang("108723"));
                            setButtonName(2, _lang("108721"));
//                            setButtonName(1, _lang("108722"));
                            setButtonState(3, ButtonMarch);
                            setButtonState(2, ButtonProfile);
//                            setButtonState(1, ButtonScout);
                        }else{
                            buttonCount = 4;
                            setButtonCount(buttonCount-1);
                            setButtonName(3, _lang("108723"));
                            setButtonName(2, _lang("108721"));
//                            setButtonName(5, _lang("108722"));
                            setButtonName(1, _lang("108726"));
                            setButtonState(3, ButtonMarch);
                            setButtonState(2, ButtonProfile);
//                            setButtonState(5, ButtonScout);
                            setButtonState(1, ButtonRally);
                        }
                    }
                    if(!GlobalData::shared()->isCrossService && GlobalData::shared()->playerInfo.officer==KINGDOM_KING_ID && WorldController::isInSelfServer(m_cityInfo.tileServerId)){
                        buttonCount = buttonCount+1;
                        setButtonName(6, _lang("110064"));
                        setButtonState(6, AppointOfficals);
                    }
                }
                    break;
                default: {
                    return;
                }
            }
        }
    }
    for (int i=1; i<=TOTAL_BUTTON_NUM; ++i) {
        setButtonCallback(i, cccontrol_selector(PlayerTile::onClickButton));
    }
}

void PlayerTile::isPlayerUseSlowDownSkill(){
    if(slowDown){
        YesNoDialog* dialog = YesNoDialog::showVariableTitle(_lang("118008").c_str(),CCCallFunc::create(this, callfunc_selector(PlayerTile::gotoOpenMarchDeploy)),_lang("105153").c_str());
        CCCommonUtils::setButtonTitle(dialog->m_btnCancel, _lang("cancel_btn_label").c_str());
        dialog->showCancelButton();
    }else{
        gotoOpenMarchDeploy();
    }
}

void PlayerTile::gotoOpenMarchDeploy(){
    unsigned int index = m_cityInfo.cityIndex;
    if(m_cityInfo.parentCityIndex != -1){
        index = m_cityInfo.parentCityIndex;
    }
    float slow = slowDown?5.0:1.0;
    //void openMarchDeploy(unsigned int targetIndex,unsigned int haveOwner,int rally=0,int bType=-1,int wtIndex=-1,std::string other="", int targetType=0,float slow=1.0);
    WorldController::getInstance()->openMarchDeploy(index,1,0,-1,-1,"",0,slow);
}

void PlayerTile::openMarchDeployWithTips(){
    if(targetTooStrong){
        YesNoDialog* dialog = YesNoDialog::showVariableTitle(_lang("105152").c_str(),CCCallFunc::create(this, callfunc_selector(PlayerTile::isPlayerUseSlowDownSkill)),_lang("105153").c_str());
        CCCommonUtils::setButtonTitle(dialog->m_btnCancel, _lang("cancel_btn_label").c_str());
        dialog->showCancelButton();
    }else{
        isPlayerUseSlowDownSkill();
    }
}

void PlayerTile::openAllianceMassView(){
    unsigned int index = m_cityInfo.cityIndex;
    if(m_cityInfo.parentCityIndex != -1){
        index = m_cityInfo.parentCityIndex;
    }
    AllianceManager::getInstance()->openAllianceMassView(index);
    closeThis();
}
void PlayerTile::showCampHomeBackTip(){
    auto marchIt = WorldController::getInstance()->m_marchInfo.begin();
    while (marchIt != WorldController::getInstance()->m_marchInfo.end()){
        if((*marchIt).second.endPointIndex == m_cityInfo.cityIndex && (*marchIt).second.treasureMapFinishTime>0 && (*marchIt).second.treasureMapTime>0 && (*marchIt).second.treasureMapFinishTime > WorldController::getInstance()->getTime()){
            YesNoDialog::show( _lang("111503").c_str() , CCCallFunc::create(this, callfunc_selector(PlayerTile::onCampBachHome)), 0);
            return;
        }
        ++marchIt;
    }
    YesNoDialog::show( _lang("110098").c_str() , CCCallFunc::create(this, callfunc_selector(PlayerTile::onCampBachHome)), 0);
}
void PlayerTile::onCampBachHome(){
    
    auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
    int index = m_cityInfo.cityIndex;
    if(m_cityInfo.parentCityIndex != -1){
        index = m_cityInfo.parentCityIndex;
    }
    auto it0 = selfMarch.find(index);
    if (it0 == selfMarch.end()) {
        // todo : show error
        return;
    }
    string uuid = it0->second;
    
    if (uuid.empty()) {
        // todo : show error
        return;
    }
    
    vector<EnemyInfo>::iterator ite = EnemyInfoController::getInstance()->m_enemyInfos.begin();
    for(auto &enemyInfo : EnemyInfoController::getInstance()->m_enemyInfos)
    {
        for(auto &marchInfo : WorldController::getInstance()->m_marchInfo)
        {
            if((enemyInfo.type == ENEMY_TYPE_BATTLE || enemyInfo.type == ENEMY_TYPE_TEAM) && enemyInfo.uuid == marchInfo.second.uuid)
            {
                if(marchInfo.second.endPointIndex == m_cityInfo.cityIndex)
                {
                    // 停止闪红
                    //                                enemyInfo.type = ENEMY_TYPE_TARGET_RETREAT;
                    EnemyInfoController::getInstance()->m_enemyInfos.erase(ite);
                    break;
                }
            }
        }
        ite++;
    }
    
    auto dict = CCDictionary::create();
    dict->setObject(CCString::create(uuid), "marchId");
    WorldMapView::instance()->afterMarchCancel(dict);
}