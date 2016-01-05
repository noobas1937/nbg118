//
//  TerritoryWarehouseDetailView.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/7/9.
//
//

#include "TerritoryWarehouseDetailView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "YesNoDialog.h"
#include "WorldMapView.h"
#include "AllianceTerritoryShowCommand.h"
#include "SceneController.h"
#include "WorldController.h"
#include "AllianceAreaPopupView.h"
#include "HelpshiftCocos2dx.h"
#include "UIComponent.h"
#include "FunBuildController.h"
#include "TerritoryTowerTipsView.h"
#include "TerritoryWarehouseView.h"
#include "ParticleFireAni.h"
#include "YuanJunInfo.h"
#define  MSG_WAREHOUSE_DETAIL_CELL_CLICK       "msg_warehouse_detail_cell_click"

TerritoryWarehouseDetailView::TerritoryWarehouseDetailView(WorldCityInfo& info):m_cityInfo(info){
};

TerritoryWarehouseDetailView::~TerritoryWarehouseDetailView(){
};

TerritoryWarehouseDetailView *TerritoryWarehouseDetailView::create(WorldCityInfo& info){
    TerritoryWarehouseDetailView *ret = new TerritoryWarehouseDetailView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void TerritoryWarehouseDetailView::onEnter(){
    UIComponent::getInstance()->showPopupView(UIPopupViewType_ArcPop_TitanUpgrade);
    PopupBaseView::onEnter();
    CCLoadSprite::doResourceByCommonIndex(504, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(504, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
    m_tabView->reloadData();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TerritoryWarehouseDetailView::onCellClick), MSG_WAREHOUSE_DETAIL_CELL_CLICK, NULL);
}

void TerritoryWarehouseDetailView::onExit(){
    setTouchEnabled(false);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_WAREHOUSE_DETAIL_CELL_CLICK);
    PopupBaseView::onExit();
}

bool TerritoryWarehouseDetailView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void TerritoryWarehouseDetailView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (isTouchInside(m_withdrawBtn, pTouch) && GlobalData::shared()->playerInfo.allianceInfo.rank<=3) {
        CCCommonUtils::flyHint("", "", _lang("115286"), 3, 0, true);
    }
}

bool TerritoryWarehouseDetailView::init(){
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
//        auto tbg = CCLoadSprite::loadResource("technology_09.png");
//        auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
//        auto picBg1 = CCLoadSprite::createSprite("technology_09.png");
//        picBg1->setAnchorPoint(ccp(0, 0));
//        picBg1->setPosition(ccp(0, 0));
//        if (CCCommonUtils::isIosAndroidPad()) {
//            picBg1->setScaleX(2.4);
//        }
//        tBatchNode->addChild(picBg1);
//        int maxHeight = CCDirector::sharedDirector()->getWinSize().height - 79;
//        int curHeight = picBg1->getContentSize().height;
//        while(curHeight < maxHeight)
//        {
//            auto picBg2 = CCLoadSprite::createSprite("technology_09.png");
//            picBg2->setAnchorPoint(ccp(0, 0));
//            picBg2->setPosition(ccp(0, curHeight));
//            if (CCCommonUtils::isIosAndroidPad()) {
//                picBg2->setScaleX(2.4);
//            }
//            tBatchNode->addChild(picBg2);
//            curHeight += picBg2->getContentSize().height;
//        }
//        m_bgNode->addChild(tBatchNode);
        
        ParticleFireAni* par = ParticleFireAni::create();
        m_fireNode1->addChild(par);
        
        ParticleFireAni* par2 = ParticleFireAni::create();
        m_fireNode2->addChild(par2);
        
        m_tabView = CCTableView::create(this, m_infoList->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setTouchPriority(Touch_Default);
        m_infoList->addChild(m_tabView);
        
//        m_titleTxt->setString(_lang("115331"));
        std::string picStr = "";
        if (m_cityInfo.cityType == tile_wareHouse) {
            picStr = "territory_warehouse0.png";
        }
        auto picSprite = CCLoadSprite::createSprite(picStr.c_str());
        if (CCCommonUtils::isIosAndroidPad()) {
            picSprite->setScale(1.5);
        }
        if (picSprite) {
            m_picNode->addChild(picSprite);
        }
        
        if (!WorldController::isInSelfServer(m_cityInfo.tileServerId)) {
            removeLoadingAni();
            refreshView();
        }
        
        m_isInUpdate = false;
        return true;
    }
    
    return false;
}

void TerritoryWarehouseDetailView::requestDetail() {
    m_isInUpdate = true;
    unsigned index = m_cityInfo.cityIndex;
    if (m_cityInfo.parentCityIndex != -1) {
        index = m_cityInfo.parentCityIndex;
    }
    if (WorldController::isInSelfServer(m_cityInfo.tileServerId)) {
        auto cmd = new AllianceTerritoryDetailCommand(index);
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(TerritoryWarehouseDetailView::onDetailCallback), nullptr));
        cmd->sendAndRelease();
    }
}

void TerritoryWarehouseDetailView::onDetailCallback(cocos2d::CCObject *obj) {
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
        m_cityInfo.m_warehouseInfo.state = (AAreaState)info->valueForKey("stat")->intValue();
    }
    
    if (!info->objectForKey("owner")) {
        m_cityInfo.m_allianceAreaOwnerInfo.ownerId = "";
        m_cityInfo.m_allianceAreaOwnerInfo.ownerName = "";
        m_cityInfo.m_allianceAreaOwnerInfo.allianceId = "";
        m_cityInfo.m_allianceAreaOwnerInfo.allianceAbbr = "";
        m_cityInfo.m_allianceAreaOwnerInfo.ownerPic = "";
        m_cityInfo.m_allianceAreaOwnerInfo.team = false;
    }
    if (info->objectForKey("owner")) {
        CCDictionary* owner = _dict(info->objectForKey("owner"));
        if (owner->objectForKey("uid")) {
            m_cityInfo.m_allianceAreaOwnerInfo.ownerId = owner->valueForKey("uid")->getCString();
        }
        if (owner->objectForKey("name")) {
            m_cityInfo.m_allianceAreaOwnerInfo.ownerName = owner->valueForKey("name")->getCString();
        }
        if (owner->objectForKey("allianceId")) {
            m_cityInfo.m_allianceAreaOwnerInfo.allianceId = owner->valueForKey("allianceId")->getCString();
        }
        if (owner->objectForKey("abbr")) {
            m_cityInfo.m_allianceAreaOwnerInfo.allianceAbbr = owner->valueForKey("abbr")->getCString();
        }
        if (owner->objectForKey("team")) {
            m_cityInfo.m_allianceAreaOwnerInfo.team = owner->valueForKey("team")->boolValue();
        }
        if (owner->objectForKey("pic")) {
            m_cityInfo.m_allianceAreaOwnerInfo.ownerPic = owner->valueForKey("pic")->getCString();
        }
    }
    
    if (info->objectForKey("curDef")) {
        m_cityInfo.m_warehouseInfo.defence = info->valueForKey("curDef")->intValue();
    }
    if (info->objectForKey("maxDef")) {
        m_cityInfo.m_warehouseInfo.defenceMax = info->valueForKey("maxDef")->intValue();
    }
    if (info->objectForKey("updateTime")) {
        m_cityInfo.m_warehouseInfo.updateTime = info->valueForKey("updateTime")->doubleValue() / 1000;
    }
    if (info->objectForKey("startTime")) {
        m_cityInfo.m_warehouseInfo.startTime = info->valueForKey("startTime")->doubleValue() / 1000;
    }
    if (info->objectForKey("buildingSpd")) {
        m_cityInfo.m_warehouseInfo.buildSpeed = info->valueForKey("buildingSpd")->doubleValue() * 1000;
    }
    if (info->objectForKey("name")) {
        m_cityInfo.m_aArea_nickname = info->valueForKey("name")->getCString();
    }
    if (!info->objectForKey("name")) {
        m_cityInfo.m_aArea_nickname = _lang("115364");
    }
    if (info->objectForKey("allianceAbbr")) {
        m_cityInfo.m_aArea_simpleName = info->valueForKey("allianceAbbr")->getCString();
    }
    
    m_data->removeAllObjects();
    if (info->objectForKey("resourceList")) {
        CCArray* array = (CCArray*)info->objectForKey("resourceList");
        m_totalResource = 0;
        if(array){
            int count = array->count();
            for(int i=0;i<count;i++){
                auto item = _dict(array->objectAtIndex(i));
                if (item->objectForKey("uid")) {
                    std::string playerUid = item->valueForKey("uid")->getCString();
                    std::string playerName = item->valueForKey("name")->getCString();
                    std::string pic = item->valueForKey("pic")->getCString();
                    int picVer = item->valueForKey("picVer")->intValue();
                    m_playerStorage = 0;
                    PlayerResInfo* resInfo = new PlayerResInfo();
                    resInfo->uid = playerUid;
                    resInfo->name = playerName;
                    resInfo->pic = pic;
                    resInfo->picVer = picVer;
                    resInfo->index = 1;
                    resInfo->open = false;
                    if (item->objectForKey("wood")) {
                        m_totalResource += item->valueForKey("wood")->longValue() * GlobalData::shared()->resources_weight_1;
                        m_playerStorage += item->valueForKey("wood")->longValue() * GlobalData::shared()->resources_weight_1;
                        resInfo->wood = item->valueForKey("wood")->longValue();
                    }
                    else
                        resInfo->wood = 0;
                    
                    if (item->objectForKey("food")) {
                        m_totalResource += item->valueForKey("food")->longValue() * GlobalData::shared()->resources_weight_4;
                        m_playerStorage += item->valueForKey("food")->longValue() * GlobalData::shared()->resources_weight_4;
                        resInfo->food = item->valueForKey("food")->longValue();
                    }
                    else
                        resInfo->food = 0;
                    
                    if (item->objectForKey("stone")) {
                        m_totalResource += item->valueForKey("stone")->longValue() * GlobalData::shared()->resources_weight_2;
                        m_playerStorage += item->valueForKey("stone")->longValue() * GlobalData::shared()->resources_weight_2;
                        resInfo->stone = item->valueForKey("stone")->longValue();
                    }
                    else
                        resInfo->stone = 0;
                    
                    if (item->objectForKey("iron")) {
                        m_totalResource += item->valueForKey("iron")->longValue() * GlobalData::shared()->resources_weight_3;
                        m_playerStorage += item->valueForKey("iron")->longValue() * GlobalData::shared()->resources_weight_3;
                        resInfo->iron = item->valueForKey("iron")->longValue();
                    }
                    else
                        resInfo->iron = 0;

                    resInfo->total = m_playerStorage;
                    if (playerUid == GlobalData::shared()->playerInfo.uid) {
                        m_data->insertObject(resInfo, 0);
                    }
                    else
                        m_data->addObject(resInfo);
                    
                    resInfo->release();
                }
            }
            m_totalNum = m_data->count();
            if (m_totalNum == 0) {
                PlayerResInfo* resInfo = new PlayerResInfo();
                resInfo->index = 2;
                resInfo->open = false;
                m_data->addObject(resInfo);
                resInfo->release();
            }
        }
    }
    
    refreshView();
}

void TerritoryWarehouseDetailView::removeLoadingAni(CCObject* p)
{
    if(m_loadingIcon!=NULL){
        m_loadingIcon->stopAllActions();
        m_loadingIcon->removeFromParent();
    }
}

void TerritoryWarehouseDetailView::addLoadingAni(){
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

void TerritoryWarehouseDetailView::refreshView() {
    if (m_cityInfo.cityType == tile_wareHouse) {
        m_territoryStat = m_cityInfo.m_warehouseInfo.state;
    }
    
    if (m_cityInfo.m_aArea_id == GlobalData::shared()->playerInfo.allianceInfo.uid && GlobalData::shared()->playerInfo.allianceInfo.rank >= 4) {
        m_withdrawBtn->setEnabled(true);
    }
    
    if (m_cityInfo.m_aArea_id != GlobalData::shared()->playerInfo.allianceInfo.uid || GlobalData::shared()->playerInfo.allianceInfo.rank < 4) {
        m_withdrawBtn->setEnabled(false);
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
            strStat = _lang("115448");
            m_statLabel->setString(strStat);
            std::string strDef = "X: ";
            CCPoint posByIndex = WorldController::getInstance()->getPointByIndex(m_cityInfo.parentCityIndex);
            strDef.append(CC_ITOA(posByIndex.x));
            strDef.append(" Y: ");
            strDef.append(CC_ITOA(posByIndex.y));
            m_defLabel1->setString(_lang_1("115469",""));
            m_defLabel2->setString(CC_CMDITOA(m_totalResource*1.0));
            m_timeLabel1->setString(_lang_1("115470",""));
            m_timeLabel2->setString(CC_ITOA(m_totalNum));
            m_troopLabel1->setVisible(false);
            m_troopLabel2->setVisible(false);
            m_tipsLabel->setString(_lang("115472"));
            m_stat = 3;
        }
            break;
        default:
            break;
    }
    
    m_tabView->reloadData();
}

void TerritoryWarehouseDetailView::onCellClick(CCObject* data){
    CCString* uuid = dynamic_cast<CCString*>(data);
    if(uuid){
        int num = m_data->count();
        for (int i=0; i<num; i++) {
            PlayerResInfo* info = (PlayerResInfo*)m_data->objectAtIndex(i);
            if(info->uid==uuid->getCString()){
                info->open = !info->open;
                break;
            }
        }
        m_tabView->reloadData();
    }
}

void TerritoryWarehouseDetailView::onTipBtnClick(CCObject *pSender, CCControlEvent event){////查看哨塔功能面板按钮，原有升级按钮取消
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    GlobalData::shared()->isBind = true;
#endif
    CCCommonUtils::showHelpShiftSingleFAQ("45208");
    //    CCCommonUtils::showHelpShiftFAQ("45203");
    //    HelpshiftCocos2dx::showFAQSection("468");

}

void TerritoryWarehouseDetailView::onWithdrawClick(CCObject *pSender, CCControlEvent event){
    YesNoDialog::marchAlertShow(_lang("115459").c_str(), CCCallFunc::create(this, callfunc_selector(TerritoryWarehouseDetailView::secondConfirm)),NULL);
}

void TerritoryWarehouseDetailView::secondConfirm(){
    YesNoDialog::marchAlertShow(_lang("115460").c_str(), CCCallFunc::create(this, callfunc_selector(TerritoryWarehouseDetailView::withdraw)),NULL);
}

void TerritoryWarehouseDetailView::withdraw(){
    string tempTuid = m_cityInfo.m_warehouseInfo.uid;
    auto cmd = new WorldTerritoryWarehouseCancelCommand(tempTuid,m_cityInfo.parentCityIndex);
    cmd->sendAndRelease();
    PopupViewController::getInstance()->removeAllPopupView();
}

bool TerritoryWarehouseDetailView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_picNode", CCNode*, this->m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_fireNode1", CCNode*, this->m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_fireNode2", CCNode*, this->m_fireNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipBtn", CCControlButton*, this->m_tipBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_withdrawBtn", CCControlButton*, this->m_withdrawBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleLabel", CCLabelIF*, this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_statLabel", CCLabelIF*, this->m_statLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_defLabel1", CCLabelIF*, this->m_defLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeLabel1", CCLabelIF*, this->m_timeLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_troopLabel1", CCLabelIF*, this->m_troopLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_defLabel2", CCLabelIF*, this->m_defLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeLabel2", CCLabelIF*, this->m_timeLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_troopLabel2", CCLabelIF*, this->m_troopLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipsLabel", CCLabelIF*, this->m_tipsLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTxt", CCLabelIF*, this->m_titleTxt);
    return false;
}

SEL_CCControlHandler TerritoryWarehouseDetailView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTipBtnClick", TerritoryWarehouseDetailView::onTipBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onWithdrawClick", TerritoryWarehouseDetailView::onWithdrawClick);
    return NULL;
}

CCSize TerritoryWarehouseDetailView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return CCSizeZero;
    }
    PlayerResInfo* obj = dynamic_cast<PlayerResInfo*>(m_data->objectAtIndex(idx));
    if (obj->open) {
        if (CCCommonUtils::isIosAndroidPad()) {
            return CCSize(1470, 450);
        }
        return CCSize(620, 230);
    }
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1470, 250);
    }
    return CCSize(620, 125);
}

CCSize TerritoryWarehouseDetailView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1470, 250);
    }
    return CCSize(620, 125);
}

CCTableViewCell* TerritoryWarehouseDetailView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    TerritoryWarehouseDetailCell* cell = (TerritoryWarehouseDetailCell*)table->dequeueCell();
    PlayerResInfo* info = (PlayerResInfo*)m_data->objectAtIndex(idx);
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info,m_stat);
        }else{
            cell = TerritoryWarehouseDetailCell::create(info,m_infoList,m_stat,m_cityInfo.parentCityIndex);
        }
    }
    return cell;
}

ssize_t TerritoryWarehouseDetailView::numberOfCellsInTableView(CCTableView *table)
{
    return m_data->count();
}

void TerritoryWarehouseDetailView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}

TerritoryWarehouseDetailCell *TerritoryWarehouseDetailCell::create(PlayerResInfo* info,CCNode* clickNode,int stat,int index){
    TerritoryWarehouseDetailCell *ret = new TerritoryWarehouseDetailCell(info,clickNode,stat,index);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void TerritoryWarehouseDetailCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void TerritoryWarehouseDetailCell::onExit(){
    setTouchEnabled(false);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCNode::onExit();
}

bool TerritoryWarehouseDetailCell::init(){
    auto node = CCBLoadFile("TerritoryInfoDetailCell", this, this);
    this->setContentSize(node->getContentSize());
    m_clickFlag = false;
    m_headImgNode = HFHeadImgNode::create();
    setData(m_info,m_stat);
    return true;
}

void TerritoryWarehouseDetailCell::setData(PlayerResInfo* info,int stat){
    m_stat = stat;
    m_info = info;
    
    CCLoadSprite::doResourceByCommonIndex(204, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(204, false);
    });
    m_nameTxt->setString(m_info->name.c_str());
    if (m_info->uid == GlobalData::shared()->playerInfo.uid) {
        m_armyNum->setString(_lang_1("115473", CC_ITOA_K(m_info->total*1.0)));
    }
    else {
//        std::string str = _lang("115473");
//        str.append(CC_ITOA_K(m_info->total*1.0));
//        m_armyNum->setString(str.c_str());
        m_armyNum->setString(_lang_1("115473", CC_ITOA_K(m_info->total*1.0)));
    }

    m_renderBg->setVisible(false);
    m_unUseNode->setVisible(false);
    m_moveNode->setVisible(false);
    m_scienceNode->setVisible(false);
    if(m_info->index==2){
        m_tipTxt->setString(_lang("115471"));
        m_tipTxt->setColor({169,183,189});
        m_unUseNode->setVisible(true);
    }else{
        m_moveNode->setVisible(true);
    }
    m_statusTxt->setString(_lang("115448"));
//    m_soldierNode->removeAllChildrenWithCleanup(true);
    std::string useArmyId = "";
    if(m_info->open){
        int addH = 105;
        if (CCCommonUtils::isIosAndroidPad()) {
            addH = 200;
        }
        m_moveNode->setPosition(ccp(0, addH));
        m_resNode->setPosition(ccp(0, addH));
        m_bgNodee->setPosition(ccp(0, addH));
        m_resNode->setVisible(true);
        m_woodTxt->setString(CC_ITOA(m_info->wood*1.0));
        m_foodTxt->setString(CC_ITOA(m_info->food*1.0));
        m_ironTxt->setString(CC_ITOA(m_info->iron*1.0));
        m_stoneTxt->setString(CC_ITOA(m_info->stone*1.0));
    }else{
        m_moveNode->setPosition(ccp(0, 0));
        m_bgNodee->setPosition(ccp(0, 0));
        m_tipTxt->setColor({120,120,120});
        m_resNode->setVisible(false);
    }
    
    m_icon->removeAllChildren();
    std::string iconStr = m_info->pic;
    string uid = m_info->uid;
    int picVer = m_info->picVer;
    if(iconStr==""||iconStr=="0"){
        iconStr = "g044";
    }
    iconStr.append(".png");
    auto pic = CCLoadSprite::createSprite(iconStr.c_str());
    CCCommonUtils::setSpriteMaxSize(pic, 78);
    m_icon->addChild(pic);
    if (CCCommonUtils::isUseCustomPic(picVer))
    {
        m_headImgNode->initHeadImgUrl2(m_icon, CCCommonUtils::getCustomPicUrl(uid, picVer), 1.0f, 78, true);
    }
    m_arrow->setRotation(m_info->open?270:180);
}


bool TerritoryWarehouseDetailCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_touchPos = pTouch->getLocation();
    if(isTouchInside(m_clickArea, pTouch)){
        return true;
    }
    return false;
}

void TerritoryWarehouseDetailCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint end = pTouch->getLocation();
    if(isTouchInside(m_clickArea, pTouch) && isTouchInside(m_clickNode, pTouch) && fabsf(end.y - m_touchPos.y)<30){
        m_clickFlag = !m_clickFlag;
        if(m_info->index==2){
            unsigned int current = WorldController::getInstance()->getCurrentMarchCount();
            unsigned int max = WorldController::getInstance()->getMaxMarchCount();
            if (current >= max) {
                WorldController::getInstance()->showMarchAlert(max);
                return;
            }
            else
                attendRally();
        }else{
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_WAREHOUSE_DETAIL_CELL_CLICK,CCString::create(m_info->uid.c_str()));
        }
    }
}

void TerritoryWarehouseDetailCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void TerritoryWarehouseDetailCell::attendRally()
{
    PopupViewController::getInstance()->removeAllPopupView();
    PopupViewController::getInstance()->addPopupInView(TerritoryWarehouseView::create(m_targetIndex, true));
}

bool TerritoryWarehouseDetailCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_armyNum", CCLabelIF*, this->m_armyNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_statusTxt", CCLabelIF*, this->m_statusTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_woodTxt", CCLabelIF*, this->m_woodTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_foodTxt", CCLabelIF*, this->m_foodTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_ironTxt", CCLabelIF*, this->m_ironTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_stoneTxt", CCLabelIF*, this->m_stoneTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_renderBg", CCScale9Sprite*, this->m_renderBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_icon", CCNode*, this->m_icon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_resNode", CCNode*, this->m_resNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_unUseNode", CCNode*, this->m_unUseNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipTxt", CCLabelIF*, this->m_tipTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_moveNode", CCNode*, this->m_moveNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bgNodee", CCSprite*, this->m_bgNodee);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_arrow", CCNode*, this->m_arrow);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickArea", CCNode*, this->m_clickArea);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_soldierNode", CCNode*, this->m_soldierNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_scienceNode", CCNode*, this->m_scienceNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_scienceTxt", CCLabelIF*, this->m_scienceTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_joinNode", CCSprite*, this->m_joinNode);
    return false;
}

SEL_CCControlHandler TerritoryWarehouseDetailCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}