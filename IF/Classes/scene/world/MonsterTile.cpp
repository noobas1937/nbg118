//
//  MonsterTile.cpp
//  IF
//
//  Created by 李锐奇 on 14-8-5.
//
//

#include "MonsterTile.h"
#include "BlankTile.h"
#include "WorldCommand.h"
#include "WorldMapView.h"
#include "YesNoDialog.h"
#include "AddFavorite.h"
#include "MonsterDetailView.h"
#include "TipsView.h"
#include "TipsWithPicView.h"
#include "AllianceActivityView.h"
#include "ActivityController.h"
#include "JoinAllianceView.h"
bool MonsterTile::init() {
    NewBaseTileInfo::init();
    m_title->setString(_lang(m_cityInfo.cityName));
    
    if(!WorldController::isInSelfServer(m_cityInfo.tileServerId)){
        setButtonCount(1);
        setButtonName(1,_lang("108730")); // info
        setButtonState(1, ButtonInformation);
        setButtonCallback(1,cccontrol_selector(MonsterTile::onInfoClick));
    }else{
        bool flag = false;
        if(ActivityController::getInstance()->activityArr){
            int num = ActivityController::getInstance()->activityArr->count();
            for (int i=0; i<num; i++) {
                ActivityEventObj* event = dynamic_cast<ActivityEventObj*>(ActivityController::getInstance()->activityArr->objectAtIndex(i));
                if (event && event->type==3) {
                    flag = true;
                    break;
                }
            }
        }
        if(flag){
            setButtonCount(3);
            setButtonName(1,_lang("133003")); // 联盟活动
            setButtonName(2,_lang("108730")); // info
            setButtonName(3,_lang("108731")); // occupy
            setButtonState(1, ButtonAllianceAct);
            setButtonState(2, ButtonInformation);
            setButtonState(3, ButtonExplore);
            setButtonCallback(1,cccontrol_selector(MonsterTile::onAllianceActClick));
            setButtonCallback(2,cccontrol_selector(MonsterTile::onInfoClick));
            setButtonCallback(3,cccontrol_selector(MonsterTile::onClickOccupy));
        }else{
            setButtonCount(2);
            setButtonName(2,_lang("108730")); // info
            setButtonName(3,_lang("108731")); // occupy
            setButtonState(2, ButtonInformation);
            setButtonState(3, ButtonExplore);
            setButtonCallback(2,cccontrol_selector(MonsterTile::onInfoClick));
            setButtonCallback(3,cccontrol_selector(MonsterTile::onClickOccupy));
        }
    }
    addToParent();
    return true;
}

bool MonsterTile::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    return NewBaseTileInfo::onAssignCCBMemberVariable(pTarget, pMemberVariableName, pNode);
}

SEL_CCControlHandler MonsterTile::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NewBaseTileInfo::onResolveCCBCCControlSelector(pTarget, pSelectorName);
}

BaseTileInfo* MonsterTile::getFavoriteView() {
    return AddFavorite::create(m_cityInfo.cityIndex,_lang(m_cityInfo.cityName));
}

void MonsterTile::onInfoClick(CCObject * pSender, Control::EventType pCCControlEvent) {
    //PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("108656")));
    PopupViewController::getInstance()->addPopupView(TipsWithPicView::create("Monster"));
    closeThis();
}

void MonsterTile::onClickOccupy(CCObject * pSender, Control::EventType pCCControlEvent) {
    auto view = MonsterDetailView::create(m_cityInfo.parentCityIndex);
    PopupViewController::getInstance()->addPopupView(view);
    closeThis();
}

void MonsterTile::onAllianceActClick(CCObject * pSender, Control::EventType pCCControlEvent){
    if(!GlobalData::shared()->playerInfo.isInAlliance()){
        YesNoDialog::showVariableTitle(_lang("133032").c_str(), CCCallFunc::create(this, callfunc_selector(MonsterTile::yesFun)), _lang("115001").c_str());
    }else{
        if (ActivityController::getInstance()->activityArr) {
            int num = ActivityController::getInstance()->activityArr->count();
            for (int i=0; i<num; i++) {
                ActivityEventObj* event = dynamic_cast<ActivityEventObj*>(ActivityController::getInstance()->activityArr->objectAtIndex(i));
                if (event && event->type==3) {
                    PopupViewController::getInstance()->addPopupView(AllianceActivityView::create(event));
                    break;
                }
            }
        }
    }
    closeThis();
}

void MonsterTile::yesFun(){
    PopupViewController::getInstance()->addPopupInView(JoinAllianceView::create(11));
}
