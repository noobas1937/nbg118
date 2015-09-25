//
//  TerritoryInformationView.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/4/14.
//
//

#include "TerritoryInformationView.h"
#include "PopupViewController.h"
#include "UIComponent.h"
#include "ChangePicCommand.h"
#include "WorldController.h"
#include "WorldCommand.h"
#include "SceneController.h"
#include "WorldMapView.h"
#include "YesNoDialog.h"
#include "AllianceTerritoryShowCommand.h"
#include "YuanJunInfo.h"
#include "ParticleFireAni.h"
#include "YuanJunDetailView.h"
#include "AllianceScienceView.h"
#include "TerritoryFunctionView.h"
#include "TerritoryTowerTipsView.h"
#define  MSG_TERRITORY_TOWER_DETAIL_CELL_CLICK       "msg_territory_tower_detail_cell_click"

TerritoryInformationView::TerritoryInformationView(WorldCityInfo& info):m_cityInfo(info){
};

TerritoryInformationView::~TerritoryInformationView(){
};

TerritoryInformationView *TerritoryInformationView::create(WorldCityInfo& info){
    TerritoryInformationView *ret = new TerritoryInformationView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void TerritoryInformationView::onEnter(){
    UIComponent::getInstance()->showPopupView(1);
    PopupBaseView::onEnter();
    CCLoadSprite::doResourceByCommonIndex(504, true);
    CCLoadSprite::doResourceByCommonIndex(500, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(504, false);
        CCLoadSprite::doResourceByCommonIndex(500, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
    m_tabView->reloadData();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TerritoryInformationView::onCellClick), MSG_TERRITORY_TOWER_DETAIL_CELL_CLICK, NULL);
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void TerritoryInformationView::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TERRITORY_TOWER_DETAIL_CELL_CLICK);
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    PopupBaseView::onExit();
}

bool TerritoryInformationView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void TerritoryInformationView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (WorldController::isInSelfServer(m_cityInfo.tileServerId) && isTouchInside(m_withdrawBtn, pTouch) && GlobalData::shared()->playerInfo.allianceInfo.rank<=3) {
        CCCommonUtils::flyHint("", "", _lang("115286"), 3, 0, true);
    }
}

bool TerritoryInformationView::init(){
    if(PopupBaseView::init()){
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(504, true);
        CCLoadSprite::doResourceByCommonIndex(500, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(7, false);
            CCLoadSprite::doResourceByCommonIndex(504, false);
            CCLoadSprite::doResourceByCommonIndex(500, false);
        });
        setIsHDPanel(true);
        m_data = CCArray::create();
        
        requestDetail();
        auto node = CCBLoadFile("TerritoryInfoDetailView", this, this);
        if (CCCommonUtils::isIosAndroidPad()) {
            this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        }
        else {
            this->setContentSize(node->getContentSize());
            int extH = getExtendHeight();
            m_infoList->setPositionY(m_infoList->getPositionY() - extH);
            m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height + extH));
            m_bottomNode->setPositionY(m_bottomNode->getPositionY() - extH);
            m_bgNode->setPositionY(m_bgNode->getPositionY() - extH);

        }
        
        CCCommonUtils::setButtonTitle(m_withdrawBtn, _lang("115343").c_str());
        m_withdrawBtn->setVisible(true);
        m_withdrawBtn->setEnabled(false);
        addLoadingAni();
        
        auto tbg = CCLoadSprite::loadResource("technology_09.png");
        auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
        auto picBg1 = CCLoadSprite::createSprite("technology_09.png");
        picBg1->setAnchorPoint(ccp(0, 0));
        picBg1->setPosition(ccp(0, 0));
        if (CCCommonUtils::isIosAndroidPad()) {
            picBg1->setScaleX(2.4);
        }
        tBatchNode->addChild(picBg1);
        int maxHeight = CCDirector::sharedDirector()->getWinSize().height - 79;
        int curHeight = picBg1->getContentSize().height;
        while(curHeight < maxHeight)
        {
            auto picBg2 = CCLoadSprite::createSprite("technology_09.png");
            picBg2->setAnchorPoint(ccp(0, 0));
            picBg2->setPosition(ccp(0, curHeight));
            if (CCCommonUtils::isIosAndroidPad()) {
                picBg2->setScaleX(2.4);
            }
            tBatchNode->addChild(picBg2);
            curHeight += picBg2->getContentSize().height;
        }
        m_bgNode->addChild(tBatchNode);
        
        ParticleFireAni* par = ParticleFireAni::create();
        m_fireNode1->addChild(par);
        
        ParticleFireAni* par2 = ParticleFireAni::create();
        m_fireNode2->addChild(par2);
        
        m_tabView = CCTableView::create(this, m_infoList->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setTouchPriority(Touch_Default);
        m_infoList->addChild(m_tabView);
        std::string picStr = "";
        picStr = "territory_tower0.png";
        if (CCCommonUtils::isIosAndroidPad()) {
            m_picNode->setScale(1.8);
        }
        else
            m_picNode->setScale(1.2);
        auto picSprite = CCLoadSprite::createSprite(picStr.c_str());
        if (picSprite) {
            m_picNode->addChild(picSprite);
        }
        
        if (!WorldController::isInSelfServer(m_cityInfo.tileServerId)) {
            removeLoadingAni();
            m_territoryFlag = false;
            refreshView();
        }
        m_isInUpdate = false;
        return true;
    }
    
    return false;
}

void TerritoryInformationView::requestDetail() {
    m_isInUpdate = true;
    unsigned index = m_cityInfo.cityIndex;
    if (m_cityInfo.parentCityIndex != -1) {
        index = m_cityInfo.parentCityIndex;
    }
    if (WorldController::isInSelfServer(m_cityInfo.tileServerId)) {
        auto cmd = new AllianceTerritoryDetailCommand(index);
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(TerritoryInformationView::onDetailCallback), nullptr));
        cmd->sendAndRelease();
    }
}

void TerritoryInformationView::onDetailCallback(cocos2d::CCObject *obj) {
    removeLoadingAni();
    m_isInUpdate = false;
    if(SceneController::getInstance()->currentSceneId != SCENE_ID_WORLD){
        return;
    }
    if(!WorldController::getInstance()->isInWorld){
        return;
    }
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : parse error
        return;
    }
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    if (!info) {
        return;
    }
    if(!this->getParent()){
        return;
    }
    
    if (info->objectForKey("stat")) {
        m_cityInfo.m_towerInfo.state = (AAreaState)info->valueForKey("stat")->intValue();
    }
    
    if (info->objectForKey("maxSoldier")) {
        soldierMax = info->valueForKey("maxSoldier")->intValue();
    }
    else
        soldierMax = 0;
    
    if (info->objectForKey("alreadySoldier")) {
        soldierAlready = info->valueForKey("alreadySoldier")->intValue();
    }
    else
        soldierAlready = 0;
    
    if (info->objectForKey("curDef")) {
        m_cityInfo.m_towerInfo.defence = info->valueForKey("curDef")->intValue();
    }
    if (info->objectForKey("maxDef")) {
        m_cityInfo.m_towerInfo.defenceMax = info->valueForKey("maxDef")->intValue();
    }
    if (info->objectForKey("updateTime")) {
        m_cityInfo.m_towerInfo.updateTime = info->valueForKey("updateTime")->doubleValue() / 1000;
    }
    if (info->objectForKey("startTime")) {
        m_cityInfo.m_towerInfo.startTime = info->valueForKey("startTime")->doubleValue() / 1000;
    }
    if (info->objectForKey("buildingSpd")) {
        m_cityInfo.m_towerInfo.buildSpeed = info->valueForKey("buildingSpd")->doubleValue() * 1000;
    }
    if (info->objectForKey("name")) {
        m_cityInfo.m_aArea_nickname = info->valueForKey("name")->getCString();
    }
    if (!info->objectForKey("name")) {
        if (info->valueForKey("count")) {
            int num = info->valueForKey("count")->intValue();
            m_cityInfo.m_aArea_nickname = _lang_1("115406", CC_ITOA(num));
        }
    }
    if (info->objectForKey("allianceAbbr")) {
        m_cityInfo.m_aArea_simpleName = info->valueForKey("allianceAbbr")->getCString();
    }
    
    m_data->removeAllObjects();
    m_totalKill = 0;
    if (info->objectForKey("kill")) {
        CCArray* array = (CCArray*)info->objectForKey("kill");
        if(array){
            int count = array->count();
            for(int i=0;i<count;i++){
                auto item = _dict(array->objectAtIndex(i));
                YuanJunInfo* info = new YuanJunInfo();
                if (item->objectForKey("picVer")) {
                    info->setpicVer(item->valueForKey("picVer")->intValue());
                }
                if (item->objectForKey("uid")) {
                    info->setUid(item->valueForKey("uid")->getCString());
                }
                if (item->objectForKey("pic")) {
                    info->setPic(item->valueForKey("pic")->getCString());
                }
                if (item->objectForKey("name")) {
                    info->setName(item->valueForKey("name")->getCString());
                }
                if (item->objectForKey("dead")) {
                    m_arr = CCArray::create();
                    m_arr->removeAllObjects();
                    CCDictionary* dead = dynamic_cast<CCDictionary*>(item->objectForKey("dead"));
                    CCDictElement *element;
                    CCDICT_FOREACH(dead, element){
                        std::string armyId = element->getStrKey();
                        int value = dead->valueForKey(armyId)->intValue();
                        m_totalKill += value;
                        CCDictionary* dic = CCDictionary::create();
                        dic->setObject(CCString::create(armyId), "armyId");
                        dic->setObject(CCString::create(CC_ITOA(value)), "count");
                        m_arr->addObject(dic);
                    }
                    info->setSoldiers(m_arr);
                }
                info->setIndex(1);
                info->setOpen(false);
                m_data->addObject(info);
                info->release();
            }
        }
    }
    m_noDeadFlag = false;
    if (m_data->count() == 0) {
        m_noDeadFlag = true;
    }
    refreshView();
}

void TerritoryInformationView::removeLoadingAni(CCObject* p)
{
    if(m_loadingIcon!=NULL){
        m_loadingIcon->stopAllActions();
        m_loadingIcon->removeFromParent();
    }
}

void TerritoryInformationView::addLoadingAni(){
    if (m_loadingIcon==NULL) {
        m_loadingIcon = CCLoadSprite::createSprite("loading_1.png");
        m_loadingIcon->setAnchorPoint(ccp(0.5, 0.5));
        auto cSize = m_infoList->getContentSize();
        m_loadingIcon->setPosition(ccp(cSize.width/2,cSize.height/2));
    }
    CCActionInterval * rotateto1 = CCRotateTo::create(0.5, 720);
    CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
    CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
    m_loadingIcon->runAction(rotateForever);
    if(m_loadingIcon->getParent()==NULL){
        m_infoList->addChild(m_loadingIcon,1000000);
    }
}

void TerritoryInformationView::refreshView() {
    m_territoryStat = m_cityInfo.m_towerInfo.state;
    
    if (m_cityInfo.m_aArea_id == GlobalData::shared()->playerInfo.allianceInfo.uid && GlobalData::shared()->playerInfo.allianceInfo.rank >= 4) {
        m_withdrawBtn->setEnabled(true);
    }
    
    if (m_cityInfo.m_aArea_id != GlobalData::shared()->playerInfo.allianceInfo.uid || GlobalData::shared()->playerInfo.allianceInfo.rank < 4) {
        m_withdrawBtn->setEnabled(false);
    }
    
    if (m_noDeadFlag) {
        m_noDeadLabel->setVisible(true);
        m_noDeadLabel->setPositionY(m_infoList->getContentSize().height / 2);
        m_noDeadLabel->setString(_lang("115486"));
    }
    std::string strTitle = "(";
    strTitle.append(m_cityInfo.m_aArea_simpleName);
    strTitle.append(")");
    strTitle.append(m_cityInfo.m_aArea_nickname);
    m_titleTxt->setString(strTitle);
    m_statLabel->setColor({243,69,0});
    std::string strStat = "";//状态
    switch (m_territoryStat) {
        case 3: {
            strStat = _lang("115409");
            m_statLabel->setString(strStat);
            std::string strDef = CC_ITOA(m_cityInfo.m_towerInfo.defence);
            strDef = strDef + "/" + CC_ITOA(m_cityInfo.m_towerInfo.defenceMax);
            m_defLabel1->setString(_lang_1("115345",""));
            m_defLabel2->setString(strDef);
            m_tipsLabel->setString(_lang("115485"));
            m_timeLabel1->setString(_lang("115483"));
            m_timeLabel2->setString(CC_CMDITOA(m_totalKill));
            m_troopLabel1->setVisible(false);
            m_troopLabel2->setVisible(false);
            m_stat = 3;
        }
            break;
        default:
            break;
    }

    m_tabView->reloadData();
}

void TerritoryInformationView::onCellClick(CCObject* data){
    CCString* uid = dynamic_cast<CCString*>(data);
    if(uid){
        ssize_t num = m_data->count();
        for (int i=0; i<num; i++) {
            YuanJunInfo* info = (YuanJunInfo*)m_data->objectAtIndex(i);
            if(info->getUid()==uid->getCString()){
                info->setOpen(!info->getOpen());
                break;
            }
        }
        m_tabView->reloadData();
    }
}

void TerritoryInformationView::onTipBtnClick(CCObject *pSender, CCControlEvent event){
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    GlobalData::shared()->isBind = true;
#endif
    CCCommonUtils::showHelpShiftSingleFAQ("45208");
    //    CCCommonUtils::showHelpShiftFAQ("45203");
    //    HelpshiftCocos2dx::showFAQSection("468");
}

void TerritoryInformationView::onWithdrawClick(CCObject *pSender, CCControlEvent event){
    YesNoDialog::marchAlertShow(_lang_1("115344", (m_cityInfo.m_aArea_nickname).c_str()), CCCallFunc::create(this, callfunc_selector(TerritoryInformationView::secondConfirm)),NULL);
}

void TerritoryInformationView::secondConfirm(){
    YesNoDialog::marchAlertShow(_lang_1("115355", (m_cityInfo.m_aArea_nickname).c_str()), CCCallFunc::create(this, callfunc_selector(TerritoryInformationView::withdraw)),NULL);
}

void TerritoryInformationView::withdraw(){
    string tempTuid = m_cityInfo.m_towerInfo.uid;
    auto cmd = new WorldTowerCancelCommand(tempTuid,m_cityInfo.cityIndex);
    cmd->sendAndRelease();
    PopupViewController::getInstance()->removeAllPopupView();
}

bool TerritoryInformationView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_picNode", CCNode*, this->m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_fireNode1", CCNode*, this->m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_fireNode2", CCNode*, this->m_fireNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_withdrawBtn", CCControlButton*, this->m_withdrawBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipBtn", CCControlButton*, this->m_tipBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleLabel", CCLabelIF*, this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_statLabel", CCLabelIF*, this->m_statLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_defLabel1", CCLabelIF*, this->m_defLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeLabel1", CCLabelIF*, this->m_timeLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_troopLabel1", CCLabelIF*, this->m_troopLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_defLabel2", CCLabelIF*, this->m_defLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeLabel2", CCLabelIF*, this->m_timeLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_troopLabel2", CCLabelIF*, this->m_troopLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipsLabel", CCLabelIF*, this->m_tipsLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_noDeadLabel", CCLabelIF*, this->m_noDeadLabel);
    return false;
}

SEL_CCControlHandler TerritoryInformationView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onWithdrawClick", TerritoryInformationView::onWithdrawClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTipBtnClick", TerritoryInformationView::onTipBtnClick);
    return NULL;
}

CCSize TerritoryInformationView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return CCSizeZero;
    }
    YuanJunInfo* obj = dynamic_cast<YuanJunInfo*>(m_data->objectAtIndex(idx));
    if (obj->getOpen()) {
        int num = obj->getSoldiers()->count();
        int row = num/2 + (num%2==0?0:1);
        int addH = row*100;
        if (CCCommonUtils::isIosAndroidPad()) {
            addH = row*200;
            return CCSize(1470, 250+addH+40);
        }
        return CCSize(620, 125+addH+20);
    }
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1470, 250);
    }
    return CCSize(620, 125);
}

CCSize TerritoryInformationView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1470, 250);
    }
    return CCSize(620, 125);
}

CCTableViewCell* TerritoryInformationView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    TerritoryInformationCell* cell = (TerritoryInformationCell*)table->dequeueCell();
    YuanJunInfo* info = (YuanJunInfo*)m_data->objectAtIndex(idx);
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info,m_stat);
        }else{
            cell = TerritoryInformationCell::create(info,m_infoList,m_stat,m_cityInfo.cityIndex,m_cityInfo.cityType);
        }
    }
    return cell;
}

ssize_t TerritoryInformationView::numberOfCellsInTableView(CCTableView *table)
{
    return m_data->count();
}

void TerritoryInformationView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}

TerritoryInformationCell *TerritoryInformationCell::create(YuanJunInfo* info,CCNode* clickNode,int stat,int targetIndex,WorldCityType type){
    TerritoryInformationCell *ret = new TerritoryInformationCell(info,clickNode,stat,targetIndex,type);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void TerritoryInformationCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void TerritoryInformationCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool TerritoryInformationCell::init(){
    CCLoadSprite::doResourceByCommonIndex(504, true);
    CCLoadSprite::doResourceByCommonIndex(500, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(504, false);
        CCLoadSprite::doResourceByCommonIndex(500, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
    auto node = CCBLoadFile("TerritoryInfoDetailCell", this, this);
    this->setContentSize(node->getContentSize());
    m_clickFlag = false;
    m_headImgNode = HFHeadImgNode::create();
    setData(m_info,m_stat);
    return true;
}

void TerritoryInformationCell::setData(YuanJunInfo* info,int stat){
    m_stat = stat;
    m_info = info;
    
    CCLoadSprite::doResourceByCommonIndex(204, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(204, false);
    });
    m_nameTxt->setString(m_info->getName().c_str());
    m_statusTxt->setString(_lang("115484"));
    m_bgNodee->setVisible(true);
    m_unUseNode->setVisible(false);
    m_moveNode->setVisible(false);
    m_scienceNode->setVisible(false);
    m_resNode->setVisible(false);
    m_renderBg->setVisible(false);
    if(m_info->getIndex()==2){
        m_tipTxt->setString(_lang("115486"));
        m_joinNode->setVisible(false);
        m_jiaruBg->setVisible(false);
        m_unUseNode->setVisible(true);
    }else{
        m_moveNode->setVisible(true);
    }
    
    if (m_info->getIndex() == 1) {
        int num = m_info->getSoldiers()->count();
        int maxCount = 0;
        int count = 0;
        for(int i=0; i<num; i++){
            auto dic = _dict(m_info->getSoldiers()->objectAtIndex(i));
            count = dic->valueForKey("count")->intValue();
            maxCount += count;
        }
        std::string numStr = _lang("108557");
        numStr.append(":");
        numStr.append(CC_CMDITOA(maxCount));
        m_armyNum->setString(numStr);
        if(m_info->getOpen()){
            m_renderBg->setVisible(true);
            num = m_info->getSoldiers()->count();
            int row = num/2 + (num%2==0?0:1);
            int addH = row*100 + 20;
            if (CCCommonUtils::isIosAndroidPad()) {
                addH = row*200 + 40;
            }
            m_moveNode->setPosition(ccp(0, addH));
            m_renderBg->setPosition(ccp(0, addH));
            m_bgNodee->setPosition(ccp(0, addH));
            if (CCCommonUtils::isIosAndroidPad()) {
                m_renderBg->setContentSize(CCSize(1470,addH));
            }
            else
                m_renderBg->setContentSize(CCSize(620,addH));
            m_renderBg->removeAllChildrenWithCleanup(true);
            for (int i=0; i<num; i++) {
                auto dic = _dict(m_info->getSoldiers()->objectAtIndex(i));
                YuanJunSoldierCell* cell = YuanJunSoldierCell::create(dic);
                int rowIndex = i/2;
                int col = i%2;
                if (CCCommonUtils::isIosAndroidPad()) {
                    cell->setPosition(ccp(col==0?40:680, m_renderBg->getContentSize().height-220-rowIndex*200));
                }
                else
                    cell->setPosition(ccp(col==0?20:340, m_renderBg->getContentSize().height-110-rowIndex*100));
                m_renderBg->addChild(cell);
            }
        }else{
            m_moveNode->setPosition(ccp(0, 0));
            m_bgNodee->setPosition(ccp(0, 0));
//            if(!m_info->getCanUse()){
//                m_tipTxt->setColor({120,120,120});
//                
//            }else{
//                m_tipTxt->setColor({169,183,189});
//            }
            m_tipTxt->setColor({169,183,189});
            m_renderBg->setVisible(false);
        }
    }
    m_tipTxt->setColor({169,183,189});
    std::string head = m_info->getPic();
    if(head==""||head=="0"){
        head = "g044";
    }
    head.append(".png");
    std::string uid = m_info->getUid();
    int picVer = m_info->getpicVer();
    m_icon->removeAllChildrenWithCleanup(true);
    CCSprite* headSpr = CCLoadSprite::createSprite(head.c_str());
    CCCommonUtils::setSpriteMaxSize(headSpr, 78);
    m_icon->addChild(headSpr);
    if (CCCommonUtils::isUseCustomPic(picVer))
    {
        m_headImgNode->initHeadImgUrl2(m_icon, CCCommonUtils::getCustomPicUrl(uid, picVer), 1.0f, 78, true);
    }
    m_arrow->setRotation(m_info->getOpen()?270:180);
}

bool TerritoryInformationCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_touchPos = pTouch->getLocation();
    if(isTouchInside(m_clickArea, pTouch)){
        return true;
    }
    return false;
}

void TerritoryInformationCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint end = pTouch->getLocation();
    if(isTouchInside(m_clickArea, pTouch) && (isTouchInside(m_clickNode, pTouch) || isTouchInside(m_arrow, pTouch)) && fabsf(end.y - m_touchPos.y)<30 && m_info->getIndex() == 1){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TERRITORY_TOWER_DETAIL_CELL_CLICK,CCString::create(m_info->getUid().c_str()));
    }
}

void TerritoryInformationCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

bool TerritoryInformationCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_armyNum", CCLabelIF*, this->m_armyNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_statusTxt", CCLabelIF*, this->m_statusTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_woodTxt", CCLabelIF*, this->m_woodTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_foodTxt", CCLabelIF*, this->m_foodTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_ironTxt", CCLabelIF*, this->m_ironTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_stoneTxt", CCLabelIF*, this->m_stoneTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_renderBg", CCScale9Sprite*, this->m_renderBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_jiaruBg", CCScale9Sprite*, this->m_jiaruBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_icon", CCNode*, this->m_icon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_resNode", CCNode*, this->m_resNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_unUseNode", CCNode*, this->m_unUseNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipTxt", CCLabelIF*, this->m_tipTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_moveNode", CCNode*, this->m_moveNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_arrow", CCNode*, this->m_arrow);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickArea", CCNode*, this->m_clickArea);
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_soldierNode", CCNode*, this->m_soldierNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_scienceNode", CCNode*, this->m_scienceNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bgNodee", CCNode*, this->m_bgNodee);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_scienceTxt", CCLabelIF*, this->m_scienceTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_joinNode", CCSprite*, this->m_joinNode);
    return false;
}

SEL_CCControlHandler TerritoryInformationCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}