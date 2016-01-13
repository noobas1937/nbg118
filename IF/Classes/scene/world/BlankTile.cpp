//
//  BlankTile.cpp
//  IF
//
//  Created by 邹 程 on 14-2-26.
//
//

#include "BlankTile.h"
#include "WorldCommand.h"
#include "WorldMapView.h"
#include "YesNoDialog.h"
#include "AddFavorite.h"
#include "MoveCityPopUpView.h"
#include "CrossServerMoveCityPopUpView.h"
#include "FunBuildController.h"
#include "SoundController.h"
#include "AllianceInfoMembersView.h"
#include "AllianceManager.h"
#include "AllianceTerritoryView.h"
#include "TerritoryInformationView.h"
bool BlankTile::init() {
    NewBaseTileInfo::init();
    m_title->setString(_lang(m_cityInfo.cityName));
    
    if(!WorldController::isInSelfServer(m_cityInfo.tileServerId)){
        setButtonCount(1);
        setButtonName(1,_lang("108719")); // teleport
        setButtonState(1, ButtonTeleport);
        setButtonCallback(1,cccontrol_selector(BlankTile::onClickTeleport));
    }else{
//        setButtonCount(2);
//        setButtonName(3,_lang("108720")); // occupy
//        setButtonName(2,_lang("108719")); // teleport
//        setButtonState(3, ButtonOccupy);
//        setButtonState(2, ButtonTeleport);
//        setButtonCallback(3,cccontrol_selector(BlankTile::onClickOccupy));
//        setButtonCallback(2,cccontrol_selector(BlankTile::onClickTeleport));
        int rank = GlobalData::shared()->playerInfo.allianceInfo.rank;
        if (rank >= 4) {
            if (GlobalData::shared()->isCrossService && GlobalData::shared()->playerInfo.currentServerId>CROSS_SERVER_START_INDEX) {
                setButtonCount(3);
                setButtonName(3, _lang("108720")); // occupy
                setButtonName(2, _lang("108719")); // teleport
                setButtonName(1, _lang("115285")); // invite teleport
                setButtonState(3, ButtonOccupy);
                setButtonState(2, ButtonTeleport);
                setButtonState(1, ButtonWorldInviteTeleport);
                setButtonCallback(3, cccontrol_selector(BlankTile::onClickOccupy));
                setButtonCallback(2,cccontrol_selector(BlankTile::onClickTeleport));
                setButtonCallback(1,cccontrol_selector(BlankTile::onClickInviteTeleport));
            }else{
                setButtonCount(4);
                setButtonName(3, _lang("108720")); // occupy
                setButtonName(2, _lang("108719")); // teleport
                setButtonName(4, _lang("115285")); // invite teleport
                setButtonName(5, _lang("115302"));// territory
                setButtonState(3, ButtonOccupy);
                setButtonState(2, ButtonTeleport);
                setButtonState(4, ButtonWorldInviteTeleport);
                setButtonState(5, ButtonPlace);
                setButtonCallback(3, cccontrol_selector(BlankTile::onClickOccupy));
                setButtonCallback(2,cccontrol_selector(BlankTile::onClickTeleport));
                setButtonCallback(4,cccontrol_selector(BlankTile::onClickInviteTeleport));
                setButtonCallback(5,cccontrol_selector(BlankTile::onClickPlace));
            }

        }else if (rank >= 3){
            setButtonCount(3);
            setButtonName(3, _lang("108720")); // occupy
            setButtonName(2, _lang("108719")); // teleport
            setButtonName(1, _lang("115285")); // invite teleport
            setButtonState(3, ButtonOccupy);
            setButtonState(2, ButtonTeleport);
            setButtonState(1, ButtonWorldInviteTeleport);
            setButtonCallback(3, cccontrol_selector(BlankTile::onClickOccupy));
            setButtonCallback(2,cccontrol_selector(BlankTile::onClickTeleport));
            setButtonCallback(1,cccontrol_selector(BlankTile::onClickInviteTeleport));
        }
        else {
            setButtonCount(2);
            setButtonName(3, _lang("108720")); // occupy
            setButtonName(2, _lang("108719")); // teleport
            setButtonState(3, ButtonOccupy);
            setButtonState(2, ButtonTeleport);
            setButtonCallback(3, cccontrol_selector(BlankTile::onClickOccupy));
            setButtonCallback(2,cccontrol_selector(BlankTile::onClickTeleport));
        }
    }
    setBtnState();
    
    std::string str = "";
    if(m_cityInfo.tileGid < 98){
        int index = m_cityInfo.tileGid % 98;
        if(index < 10){
            str = CCString::createWithFormat("00%s.png", CC_ITOA(index))->getCString();
        }else{
            str = CCString::createWithFormat("0%s.png", CC_ITOA(index))->getCString();
        }
    }else{
        int index = m_cityInfo.tileGid % 98;
        if(index < 10){
            str = CCString::createWithFormat("000%s.png", CC_ITOA(index))->getCString();
        }else{
            str = CCString::createWithFormat("00%s.png", CC_ITOA(index))->getCString();
        }
    };

    addToParent();
    return true;
}

bool BlankTile::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    return NewBaseTileInfo::onAssignCCBMemberVariable(pTarget, pMemberVariableName, pNode);
}

SEL_CCControlHandler BlankTile::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NewBaseTileInfo::onResolveCCBCCControlSelector(pTarget, pSelectorName);
}

void BlankTile::setBtnState(){
    if (!canOccupy()) {
        setButtonEnable(3);
    }
}

bool BlankTile::canOccupy(){
    if (m_cityInfo.cityType == OriginTile && (m_cityInfo.cityOriginType == "stop" || m_cityInfo.cityOriginType == "neutrally")) {
        return false;
    }
    return true;
}

BaseTileInfo* BlankTile::getFavoriteView() {
    return AddFavorite::create(m_cityInfo.cityIndex,_lang(m_cityInfo.cityName));
}

void BlankTile::onClickTeleport(CCObject * pSender, Control::EventType pCCControlEvent) {
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    if(WorldController::getInstance()->isInFresherForbidenArea(m_cityInfo.cityIndex) && FunBuildController::getInstance()->getMainCityLv() <= _limit_cross_server){
        if(!WorldController::isInSelfServer(m_cityInfo.tileServerId)){
            CCCommonUtils::flyText(_lang("104948"));
        }else{
            CCCommonUtils::flyText(_lang("108878"));
        }
        return;
    }
    
    if(!WorldController::isInSelfServer(m_cityInfo.tileServerId)){
        if(FunBuildController::getInstance()->getMainCityLv() > _limit_cross_server){
            CCCommonUtils::flyHint("", "", _lang("108751"));
            return;
        }

        PopupViewController::getInstance()->addPopupView(CrossServerMoveCityPopUpView::create());
    }else{
        PopupViewController::getInstance()->addPopupView(MoveCityPopUpView::create(MoveCity_Castle));
    }

    closeThis();
}

void BlankTile::onClickInviteTeleport(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    AllianceManager::getInstance()->tmpCityIndex = m_cityInfo.cityIndex;
    PopupViewController::getInstance()->addPopupInView(AllianceInfoMembersView::create(GlobalData::shared()->playerInfo.allianceInfo.uid, false, 1));
    closeThis();
}
void BlankTile::onClickPlace(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    AllianceManager::getInstance()->tmpCityIndex = m_cityInfo.cityIndex;
    //fusheng 屏蔽联盟领地放置功能
    CCCommonUtils::flyHint("", "", _lang("E100008"));
   
    if (false) {
        PopupViewController::getInstance()->addPopupInView(AllianceTerritoryView::create(m_cityInfo.cityIndex,true));
    }
    
    closeThis();
}
void BlankTile::onClickOccupy(CCObject * pSender, Control::EventType pCCControlEvent) {
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    if (!canOccupy()) {
        CCCommonUtils::flyHint("", "", _lang("108733"));
        return;
    }
    auto button = dynamic_cast<CCControlButton*>(pSender);
    if (button) {
        button->setEnabled(false);
    }
    WorldController::getInstance()->alertProectFlag = true;
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
    WorldController::getInstance()->openMarchDeploy(m_cityInfo.cityIndex,0);

    closeThis();
}