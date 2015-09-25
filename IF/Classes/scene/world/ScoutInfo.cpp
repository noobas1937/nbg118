//
//  ScoutInfo.cpp
//  IF
//
//  Created by 邹 程 on 14-3-5.
//
//

#include "ScoutInfo.h"
#include "WorldCommand.h"
#include "WorldMapView.h"
#include "YesNoDialog.h"

bool ScoutInfo::init() {
    BaseTileInfo::init();
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(502, true);
    CCLoadSprite::doResourceByCommonIndex(504, true);
    auto ccb = CCBLoadFile("ScoutInfo",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else
        this->setContentSize(ccb->getContentSize());
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(504, false);
        CCLoadSprite::doResourceByCommonIndex(502, false);
    });
    m_costFood = 1000; //default value
    m_subNode->setVisible(false);
    m_titleNew->setString(_lang("108539"));
//    m_title->setString(_lang("108539"));
    
    //setFavoButtonVisible(false);
    
//    setButtonCount(1);
//    setButtonName(1, "");
//    setButtonCallback(1, cccontrol_selector(ScoutInfo::onClickButtonScout));
    //    m_owner->setString(CCString::createWithFormat("%s %s",_lang("108549").c_str(),m_cityInfo.playerName.c_str())->getCString());
    m_owner->setString(_lang("108563"));
    
    
    m_buttonName->setString(_lang("108539"));
    
    m_country->setString("");
    m_information->setString(_lang("108738"));

    
    m_tilePoint->setString(_lang_2("108502",CC_ITOA((int)m_cityInfo.cityTilePoint.x),CC_ITOA((int)m_cityInfo.cityTilePoint.y)));
    
    m_hideNode->setVisible(false);
    
    if (m_cityInfo.cityType == Tile_allianceArea) {
        if (m_cityInfo.m_allianceAreaOwnerInfo.ownerName == "") {
            std::string nameStr = "(";
            nameStr.append(m_cityInfo.m_aArea_simpleName);
            nameStr.append(")");
            nameStr.append(m_cityInfo.m_aArea_nickname);
            m_name->setString(nameStr.c_str());
        }
        else {
            std::string nameStr = "";
            if(m_cityInfo.m_allianceAreaOwnerInfo.allianceAbbr != ""){
                nameStr = "(";
                nameStr.append(m_cityInfo.m_allianceAreaOwnerInfo.allianceAbbr);
                nameStr.append(")");
                nameStr.append(m_cityInfo.m_allianceAreaOwnerInfo.ownerName);
            }
            else
                nameStr = m_cityInfo.m_allianceAreaOwnerInfo.ownerName;
            m_name->setString(nameStr.c_str());
        }
    }
    else if (m_cityInfo.cityType == tile_banner) {
        if (m_cityInfo.m_allianceAreaOwnerInfo.ownerName == "") {
            std::string nameStr = "(";
            nameStr.append(m_cityInfo.m_aArea_simpleName);
            nameStr.append(")");
            nameStr.append(m_cityInfo.m_aArea_nickname);
            m_name->setString(nameStr.c_str());
        }
        else {
            std::string nameStr = "";
            if(m_cityInfo.m_allianceAreaOwnerInfo.allianceAbbr != ""){
                nameStr = "(";
                nameStr.append(m_cityInfo.m_allianceAreaOwnerInfo.allianceAbbr);
                nameStr.append(")");
                nameStr.append(m_cityInfo.m_allianceAreaOwnerInfo.ownerName);
            }
            else
                nameStr = m_cityInfo.m_allianceAreaOwnerInfo.ownerName;
            m_name->setString(nameStr.c_str());
        }
    }
    else {
        //对方名字
        std::string nameStr = m_cityInfo.playerName;
        if(m_cityInfo.leagueAsn != ""){
            nameStr = std::string("(") + m_cityInfo.leagueAsn + ")" + m_cityInfo.playerName;
        }
        m_name->setString(nameStr.c_str());
    }
    
    m_scoutTime->setString(_lang("137425")+":");
    m_information->setVisible(false);
    
    auto cmd = new WorldScoutDetailCommand(m_cityInfo.cityIndex);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ScoutInfo::onDetailCallback), NULL));
    cmd->sendAndRelease();
    
    return true;
}

bool ScoutInfo::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hideNode", CCNode*, this->m_hideNode)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_country", CCLabelIF*, this->m_country)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_owner", CCLabelIF*, this->m_owner)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tilePoint", CCLabelIF*, this->m_tilePoint)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cost", CCLabelIF*, this->m_cost)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_information", CCLabelIF*, this->m_information)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_time", CCLabelIF*, this->m_time)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buttonName", CCLabelIF*, this->m_buttonName);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picContainer", CCNode*, this->m_picContainer)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scoutTime", CCLabelIF*, this->m_scoutTime)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name", Label*, this->m_name)
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleNew", CCLabelIF*, this->m_titleNew)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnScout", CCControlButton*, this->m_btnScout)

    return BaseTileInfo::onAssignCCBMemberVariable(pTarget, pMemberVariableName, pNode);
}

SEL_CCControlHandler ScoutInfo::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickButtonScout", ScoutInfo::onClickButtonScout);
    return BaseTileInfo::onResolveCCBCCControlSelector(pTarget, pSelectorName);
}

void ScoutInfo::onDetailCallback(cocos2d::CCObject *obj) {
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : parse error
        return;
    }
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    m_costTime = info->valueForKey("scoutTime")->intValue();
    m_costFood = info->valueForKey("scoutCost")->intValue();
    m_costTime /= 1000;
    //对方头像
    std::string picStr="";
    if (m_cityInfo.cityType == Tile_allianceArea) {
        if (m_cityInfo.m_allianceAreaOwnerInfo.ownerName == "") {
            picStr = "territory_building_lv0";
        }
        else {
            if (info->valueForKey("pic")->getCString()!="") {
                picStr=info->valueForKey("pic")->getCString();
            }
            if (picStr=="") {
                picStr="g026";
            }
        }
    }
    else if (m_cityInfo.cityType == tile_banner) {
        if (m_cityInfo.m_allianceAreaOwnerInfo.ownerName == "") {
            picStr = "territory_tower0";
        }
        else {
            if (info->valueForKey("pic")->getCString()!="") {
                picStr=info->valueForKey("pic")->getCString();
            }
            if (picStr=="") {
                picStr="g026";
            }
        }
    }
    else {
        if (info->valueForKey("pic")->getCString()!="") {
            picStr=info->valueForKey("pic")->getCString();
        }
        if (picStr=="") {
            picStr="g026";
        }
    }
    
    
    auto picSpr = CCLoadSprite::createSprite((picStr + ".png").c_str());
    this->m_picContainer->addChild(picSpr);
    CCCommonUtils::setSpriteMaxSize(picSpr, 90);//100
    
    // update view
    m_hideNode->setVisible(true);
    m_cost->setString(CC_ITOA((int)m_costFood));
    m_time->setString(CCCommonUtils::timeLeftToCountDown(m_costTime).c_str());
    
}


void ScoutInfo::onClickButtonScout(CCObject * pSender, Control::EventType pCCControlEvent) {
    auto button = dynamic_cast<CCControlButton*>(pSender);
    if (button) {
        button->setEnabled(false);
    }
    auto food = GlobalData::shared()->resourceInfo.lFood;
    unsigned int current = WorldController::getInstance()->getCurrentMarchCount();
    unsigned int max = WorldController::getInstance()->getMaxMarchCount();
    if (current >= max) {
        WorldController::getInstance()->showMarchAlert(max);
    } else if (food < m_costFood) {
        CCCommonUtils::flyText(_lang("108654"));
    } else {
        auto cmd = new WorldMarchCommand(m_cityInfo.cityIndex,MethodScout);
        cmd->setCallback(CCCallFuncO::create(WorldMapView::instance(), callfuncO_selector(WorldMapView::onMarchCallback), NULL));
        cmd->sendAndRelease();

        GlobalData::shared()->resourceInfo.lFood -= m_costFood;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
    }
    
    closeThis();
}