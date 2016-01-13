//
//  TerritoryTowerTipsView.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/6/10.
//
//

#include "TerritoryTowerTipsView.h"
#include "YesNoDialog.h"
#include "WorldMapView.h"
#include "CheckAllianceInfoView.h"
#include "GetAllianceListCommand.h"
TerritoryTowerTipsView *TerritoryTowerTipsView::create(WorldCityInfo& info,bool flag){
    TerritoryTowerTipsView *ret = new TerritoryTowerTipsView(info,flag);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TerritoryTowerTipsView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        CCLoadSprite::doResourceByCommonIndex(502, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(502, false);
        });
        setIsHDPanel(true);
        WorldMapView::instance()->m_directionIcon->setVisible(false);
        WorldMapView::instance()->m_directionLabel->setVisible(false);
        auto bg = CCBLoadFile("TerritoryTowerTipsView", this, this);
        this->setContentSize(bg->getContentSize());

        if (m_flag == true) {
            m_node1->setVisible(true);
            m_node2->setVisible(false);
            if (m_cityInfo.cityType == tile_tower) {
                m_TitleLabel1->setString(_lang_1("115406", ""));
                m_TipLabel1->setString(_lang("115404"));
            }
            else if (m_cityInfo.cityType == tile_wareHouse){
                m_TitleLabel1->setString(_lang("115364"));
                m_TipLabel1->setString(_lang("115444"));
            }
            else if (m_cityInfo.cityType == tile_superMine){
                std::string nameStr = "";
                switch (m_cityInfo.m_superMineInfo.type) {
                    case Food:
                        nameStr = _lang("115376");
                        break;
                    case Wood:
                        nameStr = _lang("115377");
                        break;
                    case Iron:
                        nameStr = _lang("115378");
                        break;
                    case Stone:
                        nameStr = _lang("115379");
                        break;
                    default:
                        break;
                }
                m_TitleLabel1->setString(nameStr);
                m_TipLabel1->setString(_lang("115374"));
            }
        }
        else {
            m_node1->setVisible(false);
            m_node2->setVisible(true);
            if (m_cityInfo.cityType == tile_tower) {
                m_TitleLabel2->setString(_lang_1("115406", ""));
                m_TipLabel2->setString(_lang("115404"));
            }
            else if (m_cityInfo.cityType == tile_wareHouse){
                m_TitleLabel2->setString(_lang("115364"));
                m_TipLabel2->setString(_lang("115444"));
            }
            else if (m_cityInfo.cityType == tile_superMine){
                std::string nameStr = "";
                switch (m_cityInfo.m_superMineInfo.type) {
                    case Food:
                        nameStr = _lang("115376");
                        break;
                    case Wood:
                        nameStr = _lang("115377");
                        break;
                    case Iron:
                        nameStr = _lang("115378");
                        break;
                    case Stone:
                        nameStr = _lang("115379");
                        break;
                    default:
                        break;
                }
                m_TitleLabel2->setString(nameStr);
                m_TipLabel2->setString(_lang("115374"));
            }
        }
        
        m_withdrawLabel->setString(_lang("115385"));
        m_withdrawLabel->setColor({219,207,197});
        
        ret = true;
    }
    return ret;
}

void TerritoryTowerTipsView::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void TerritoryTowerTipsView::onExit(){
    WorldMapView::instance()->updateDirection();
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool TerritoryTowerTipsView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void TerritoryTowerTipsView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(!isTouchInside(m_buildBG, pTouch)){
        this->closeSelf();
        return;
    }
    if (isTouchInside(m_withdrawBtn, pTouch)) {
//        if (!m_withdrawBtn->isEnabled() && GlobalData::shared()->playerInfo.allianceInfo.rank < 4) {
//            CCCommonUtils::flyHint("", "", _lang("115286"), 3, 0, true);
//        }
    }
}

void TerritoryTowerTipsView::onWithdrawBtnClick(cocos2d::CCObject * pSender, CCControlEvent pCCControlEvent)
{
    GetAllianceListCommand* cmd = new GetAllianceListCommand(m_cityInfo.m_aArea_name,1,1);
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(TerritoryTowerTipsView::showAllianceInfo), NULL));
    cmd->sendAndRelease();
    closeSelf();
}

void TerritoryTowerTipsView::showAllianceInfo(CCObject* obj)
{
    std::string uid = "";
    unsigned int index = m_cityInfo.cityIndex;
    if (m_cityInfo.parentCityIndex != -1) {
        index = m_cityInfo.parentCityIndex;
    }
    auto cityIt = WorldController::getInstance()->getCityInfos().find(index);
    if (cityIt != WorldController::getInstance()->getCityInfos().end()) {
        uid = cityIt->second.m_aArea_id;
    }
    
    NetResult* result = dynamic_cast<NetResult*>(obj);
    auto params = _dict(result->getData());
    CCArray* arr =  (CCArray*)params->objectForKey("list");
    int num = arr->count();
    for (int i=0; i<num; i++) {
        CCDictionary* dicAlliance = (CCDictionary*)arr->objectAtIndex(i);
        
        AllianceInfo* alliance = new AllianceInfo();
        alliance->updateAllianceInfo(dicAlliance);
        if(alliance->uid==uid){
            PopupViewController::getInstance()->addPopupInView(CheckAllianceInfoView::create(alliance));
            CC_SAFE_RELEASE_NULL(alliance);
            break;
        }
        CC_SAFE_RELEASE_NULL(alliance);
    }
}

void TerritoryTowerTipsView::secondConfirm()
{
    std::string nameStr = m_cityInfo.m_aArea_nickname;

    YesNoDialog::marchAlertShow(_lang_1("115355", nameStr.c_str()), CCCallFunc::create(this, callfunc_selector(TerritoryTowerTipsView::withDraw)),CCCallFunc::create(this, callfunc_selector(TerritoryTowerTipsView::closeSelf)));
}

void TerritoryTowerTipsView::withDraw()
{
    string tempTuid = m_cityInfo.m_towerInfo.uid;
    auto cmd = new WorldTowerCancelCommand(tempTuid,m_cityInfo.cityIndex);
    cmd->sendAndRelease();
    PopupViewController::getInstance()->removeAllPopupView();
}

void TerritoryTowerTipsView::onTipBtnClick1(CCObject * pSender, CCControlEvent pCCControlEvent)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    GlobalData::shared()->isBind = true;
#endif
    CCCommonUtils::showHelpShiftSingleFAQ("45208");
//    CCCommonUtils::showHelpShiftFAQ("45203");
    //    HelpshiftCocos2dx::showFAQSection("468");
}

void TerritoryTowerTipsView::onTipBtnClick2(CCObject * pSender, CCControlEvent pCCControlEvent)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    GlobalData::shared()->isBind = true;
#endif
    CCCommonUtils::showHelpShiftSingleFAQ("45208");
//    CCCommonUtils::showHelpShiftFAQ("45203");
    //    HelpshiftCocos2dx::showFAQSection("468");
}

SEL_CCControlHandler TerritoryTowerTipsView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onWithdrawBtnClick", TerritoryTowerTipsView::onWithdrawBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTipBtnClick1", TerritoryTowerTipsView::onTipBtnClick1);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTipBtnClick2", TerritoryTowerTipsView::onTipBtnClick2);
    return NULL;
}

bool TerritoryTowerTipsView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCNode*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node1", CCNode*, this->m_node1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node2", CCNode*, this->m_node2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_withdrawBtn", CCControlButton*, this->m_withdrawBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_TipLabel1", CCLabelIF*, this->m_TipLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_TipLabel2", CCLabelIF*, this->m_TipLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_withdrawLabel", CCLabelIF*, this->m_withdrawLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_TitleLabel1", CCLabelIF*, this->m_TitleLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_TitleLabel2", CCLabelIF*, this->m_TitleLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipBtn1", CCControlButton*, this->m_tipBtn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipBtn2", CCControlButton*, this->m_tipBtn2);
    return false;
}