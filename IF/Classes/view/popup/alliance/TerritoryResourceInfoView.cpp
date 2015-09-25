//
//  TerritoryResourceInfoView.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/5/27.
//
//

#include "TerritoryResourceInfoView.h"
#include "WorldCommand.h"
#include "WorldMapView.h"
#include "UseItemStatusView.h"
#include "ShowStatusItemCommand.h"
#include "TipsWithPicView.h"
#include "YesNoDialog.h"
TerritoryResourceInfoView *TerritoryResourceInfoView::create(WorldCityInfo &info){
    TerritoryResourceInfoView *ret = new TerritoryResourceInfoView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TerritoryResourceInfoView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        CCLoadSprite::doResourceByCommonIndex(502, true);
        CCLoadSprite::doResourceByCommonIndex(504, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(502, false);
            CCLoadSprite::doResourceByCommonIndex(504, false);
        });
        
        WorldMapView::instance()->m_directionIcon->setVisible(false);
        WorldMapView::instance()->m_directionLabel->setVisible(false);
        auto bg = CCBLoadFile("TerritoryResourceInfoView", this, this);
        this->setContentSize(bg->getContentSize());
        refreshView();
        ret = true;
    }
    return ret;
}

void TerritoryResourceInfoView::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void TerritoryResourceInfoView::onExit(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    WorldMapView::instance()->updateDirection();
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool TerritoryResourceInfoView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void TerritoryResourceInfoView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(!isTouchInside(m_buildBG, pTouch)){
        this->closeSelf();
    }
    if (isTouchInside(m_withdrawBtn, pTouch)) {
        if (!m_withdrawBtn->isEnabled() && GlobalData::shared()->playerInfo.allianceInfo.rank < 4) {
            CCCommonUtils::flyHint("", "", _lang("115286"), 3, 0, true);
        }
    }
}

void TerritoryResourceInfoView::onWithdrawBtnClick(cocos2d::CCObject * pSender, CCControlEvent pCCControlEvent)
{
    closeSelf();
    std::string nameStr = "";
    switch (m_info.m_superMineInfo.type) {
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
    YesNoDialog::marchAlertShow(_lang_1("115344", nameStr.c_str()), CCCallFunc::create(this, callfunc_selector(TerritoryResourceInfoView::secondConfirm)),CCCallFunc::create(this, callfunc_selector(TerritoryResourceInfoView::closeSelf)));
}

void TerritoryResourceInfoView::secondConfirm()
{
    std::string nameStr = "";
    switch (m_info.m_superMineInfo.type) {
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
    YesNoDialog::marchAlertShow(_lang_1("115355", nameStr.c_str()), CCCallFunc::create(this, callfunc_selector(TerritoryResourceInfoView::withDraw)),CCCallFunc::create(this, callfunc_selector(TerritoryResourceInfoView::closeSelf)));
}

void TerritoryResourceInfoView::withDraw()
{
    auto cmd = new WorldTerritoryResCancelCommand(m_info.m_superMineInfo.truid,m_info.parentCityIndex,m_info.m_superMineInfo.type);
    cmd->sendAndRelease();
    PopupViewController::getInstance()->removeAllPopupView();
}

SEL_CCControlHandler TerritoryResourceInfoView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onWithdrawBtnClick", TerritoryResourceInfoView::onWithdrawBtnClick);
    return NULL;
}

bool TerritoryResourceInfoView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameAndLevelText", CCLabelIF*, this->m_nameAndLevelText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_capLabel", CCLabelIF*, this->m_capLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_capText", CCLabelIF*, this->m_capText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_speedText", CCLabelIF*, this->m_speedText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_alreadyGetText", CCLabelIF*, this->m_alreadyGetText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon", CCNode*, this->m_icon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCNode*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_withdrawBtn", CCControlButton*, this->m_withdrawBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_smalIntoTxt", CCLabelIF*, this->m_smalIntoTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, this->m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defLabel", CCLabelIF*, this->m_defLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_withdrawLabel", CCLabelIF*, this->m_withdrawLabel);
    return false;
}

void TerritoryResourceInfoView::refreshView(){
    auto pt = WorldController::getInstance()->getPointByIndex(m_info.cityIndex);
    std::string nameStr = "";
    switch (m_info.m_superMineInfo.type) {
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
    this->m_nameAndLevelText->setString("("
                                        + m_info.m_aArea_simpleName
                                        + ")"
                                        + " "
                                        + nameStr
                                        + " "
                                        + "("
                                        + CC_ITOA(int(pt.x))
                                        + ","
                                        + CC_ITOA(int(pt.y))
                                        + ")"
                                        );

    if (m_info.m_superMineInfo.trstat == 3 || m_info.m_superMineInfo.trstat == 4 || m_info.m_superMineInfo.trstat == 6) {
        this->m_icon->removeAllChildren();
        auto icon = CCLoadSprite::createSprite(CCCommonUtils::getResourceIconByType(m_info.m_superMineInfo.type).c_str());
        CCCommonUtils::setSpriteMaxSize(icon, 30);
        m_icon->addChild(icon);
        this->m_capLabel->setString(_lang_1("115382", ""));
        int textW = m_capLabel->getContentSize().width * m_capLabel->getOriginScaleX();
        m_icon->setPositionX(m_capLabel->getPositionX() + textW + 20);
        double timePass = GlobalData::shared()->getTimeStamp() - m_info.m_superMineInfo.digStartTime;
        double resTemp = m_info.m_superMineInfo.digSpeed * timePass;
        this->m_capText->setString(CC_CMDITOA(double(m_info.m_superMineInfo.sum - resTemp)).c_str());
        m_capText->setPositionX(m_icon->getPositionX() + 20);
        this->m_speedText->setString(_lang_1("108869", CC_CMDITOA(0).c_str()));
        this->m_alreadyGetText->setString(_lang_1("108868", CC_CMDITOA(0).c_str()));
        this->m_defLabel->setVisible(false);
        this->m_timeLabel->setVisible(false);
    }
    else if (m_info.m_superMineInfo.trstat == 1) {
        this->m_icon->removeAllChildren();
        this->m_icon->setVisible(false);
        this->m_capLabel->setVisible(false);
        this->m_capText->setVisible(false);
        this->m_speedText->setVisible(false);
        this->m_alreadyGetText->setVisible(false);
        this->m_defLabel->setString(_lang_1("115345", CC_CMDITOA(m_info.m_superMineInfo.trdef).c_str()));
        this->m_timeLabel->setString(_lang("115306"));
    }
    else if (m_info.m_superMineInfo.trstat == 0) {
        this->m_icon->removeAllChildren();
        this->m_icon->setVisible(false);
        this->m_capLabel->setVisible(false);
        this->m_capText->setVisible(false);
        this->m_speedText->setVisible(false);
        this->m_alreadyGetText->setVisible(false);
    }
    
    this->m_capLabel->setColor({52,24,7});
    this->m_capText->setColor({52,24,7});
    this->m_speedText->setColor({52,24,7});
    this->m_alreadyGetText->setColor({52,24,7});
    this->m_defLabel->setColor({52,24,7});
    this->m_timeLabel->setColor({52,24,7});
    
    m_smalIntoTxt->setString(_lang("135010"));// 临时
    
    m_withdrawBtn->setVisible(true);
    if (GlobalData::shared()->playerInfo.allianceInfo.rank<=3) {
        m_withdrawBtn->setEnabled(false);
    }
    m_withdrawLabel->setString(_lang("115343"));
    m_withdrawLabel->setColor({219,207,197});
    
    update(0.0f);
}

void TerritoryResourceInfoView::update(float delta){///////////
    if (m_info.m_superMineInfo.trstat == 0) {
        double now = GlobalData::shared()->getTimeStamp();
        std::string timeStr = _lang("115305");
        timeStr.append(": ");
        double timeLeft = m_info.m_superMineInfo.trutime - now;
        timeLeft = MAX(0.0f, timeLeft);
        timeStr.append(CC_SECTOA(timeLeft));
        m_timeLabel->setString(timeStr);
        
        double timePass = GlobalData::shared()->getTimeStamp() - m_info.m_superMineInfo.trstime;
        double defTemp = m_info.m_superMineInfo.buildSpeed * timePass;
        double defLeft = m_info.m_superMineInfo.trdef + defTemp;
        defLeft = MIN(m_info.m_superMineInfo.trdefmax, defLeft);
        this->m_defLabel->setString(_lang_1("115345", CC_CMDITOA(defLeft).c_str()));
    }
    else if (m_info.m_superMineInfo.trstat == 6) {
        double timePass = GlobalData::shared()->getTimeStamp() - m_info.m_superMineInfo.digStartTime;
        double resTemp = m_info.m_superMineInfo.digSpeed * timePass;
        double resLeft = m_info.m_superMineInfo.sum - resTemp;
        resLeft = MAX(0.0f, resLeft);
        this->m_capText->setString(CC_CMDITOA(double(resLeft)).c_str());
    }
}
