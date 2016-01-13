//
//  AllianceWarView.cpp
//  IF
//
//  Created by ganxiaohua on 14-4-15.
//
//

#include "AllianceWarView.h"
#include "FunBuildController.h"
#include "PopupViewController.h"
#include "AllianceManager.h"
#include "GetAllianceWarListCommand.h"
#include "WorldController.h"
#include "AllianceWarDetailView.h"
#include "AllianceFlagPar.h"
#include "JoinAllianceView.h"
#include "AllianceWarResultView.h"
#include "SceneController.h"
#include "WorldMapView.h"
#include "ActivityController.h"
#include "ActivityEventObj.h"
#include "DynamicTiledMap.h"

#define  MSG_UPDATE_ALLIANCE_WAR_CELL_DATA       "msg_update_alliance_war_cell_data"

AllianceWarView* AllianceWarView::create(int initTab){
    AllianceWarView* ret = new AllianceWarView();
    if(ret && ret->init(initTab)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool AllianceWarView::init(int initTab)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    CCLoadSprite::doResourceByGeneralIndex(1, true);
    CCLoadSprite::doResourceByGeneralIndex(2, true);
    CCLoadSprite::doResourceByGeneralIndex(3, true);
    CCLoadSprite::doResourceByCommonIndex(205, true);
    CCLoadSprite::doResourceByCommonIndex(500, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByGeneralIndex(1, false);
        CCLoadSprite::doResourceByGeneralIndex(2, false);
        CCLoadSprite::doResourceByGeneralIndex(3, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
        CCLoadSprite::doResourceByCommonIndex(205, false);
        CCLoadSprite::doResourceByCommonIndex(500, false);
        CCLoadSprite::doResourceByCommonIndex(206, false);
    });
    if(SceneController::getInstance()->currentSceneId!=SCENE_ID_WORLD){
        CCLoadSprite::doResourceByPathIndex(WORLD_PATH, 4,true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByPathIndex(WORLD_PATH, 4,false);
            CCLoadSprite::doResourceByGeneralIndex(1, false);
            CCLoadSprite::doResourceByGeneralIndex(2, false);
            CCLoadSprite::doResourceByGeneralIndex(3, false);
            CCLoadSprite::doResourceByCommonIndex(7, false);
            CCLoadSprite::doResourceByCommonIndex(205, false);
            CCLoadSprite::doResourceByCommonIndex(500, false);
            CCLoadSprite::doResourceByCommonIndex(206, false);
            CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_MONSTERLAYERLITTLE);
        });
    }
//    auto tbg = CCLoadSprite::loadResource("technology_09.png");
//    auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
//    int maxHight = CCDirector::sharedDirector()->getWinSize().height;
//    int curHight = -500;
//    while (curHight<maxHight) {
//        auto bg = CCLoadSprite::createSprite("technology_09.png");
//        bg->setAnchorPoint(ccp(0, 1));
//        bg->setPosition(ccp(0, curHight));
//        curHight += bg->getContentSize().height;
//        tBatchNode->addChild(bg);
//    }
//    if (CCCommonUtils::isIosAndroidPad()) {
//        tBatchNode->setScaleX(1536.0 / 640.0);
//    }
//    this->addChild(tBatchNode);
    auto tmpCCB = CCBLoadFile("AllianceWarView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    m_viewBg->setVisible(false);
    int oldBgHeight = m_viewBg->getContentSize().height;
    changeBGHeight(m_viewBg);
    int newBgHeight = m_viewBg->getContentSize().height;
    m_addHeight = newBgHeight - oldBgHeight;
    if (CCCommonUtils::isIosAndroidPad()) {
        m_addHeight = CCDirector::sharedDirector()->getWinSize().height - 2048;
    }
    m_listH = m_infoList->getContentSize().height;
    m_infoList->setPositionY(m_infoList->getPositionY()-m_addHeight);
    m_infoList->setContentSize(CCSizeMake(m_infoList->getContentSize().width, m_listH+m_addHeight));
    
    m_bottomNode->setPositionY(m_bottomNode->getPositionY()-m_addHeight);
    
    m_data = CCArray::create();
    m_srcData = CCArray::create();
    
    m_tableView = CCTableView::create(this, m_infoList->getContentSize());
    m_tableView->setDirection(kCCScrollViewDirectionVertical);
    m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tableView->setTouchPriority(Touch_Popup_Item);
    m_infoList->addChild(m_tableView);
    
    if(GlobalData::shared()->playerInfo.isInAlliance()){
        m_noAllianeNode->setVisible(false);
    }else{
        m_noAllianeNode->setVisible(true);
        m_noAllianceTip->setString(_lang("115067"));
        CCCommonUtils::setButtonTitle(m_btnJoin, _lang("115068").c_str());
    }
    CCCommonUtils::setButtonTitle(m_btnResult, _lang("115203").c_str());
    m_btnResult->setVisible(GlobalData::shared()->playerInfo.isInAlliance());
    CCCommonUtils::setButtonTitle(m_tabBtn1, _lang("133102").c_str());
    CCCommonUtils::setButtonTitle(m_tabBtn2, _lang("133103").c_str());
    m_tabType = initTab;
    m_first = true;
    if (m_tabType==0) {
        onClickTab1(NULL,Control::EventType::TOUCH_DOWN);
    }else{
        onClickTab2(NULL,Control::EventType::TOUCH_DOWN);
    }
    return true;
}

void AllianceWarView::refreshTableView(){
    m_data->removeAllObjects();
    bool flag = checkOpenMonster();
    if (flag) {//有怪物的队伍，显示分栏
//        m_listH = 615;
        m_listH = 615 - 30;//fusheng 调低30
        if (CCCommonUtils::isIosAndroidPad()) {
            m_listH = 1555;
        }
        m_tabNode->setVisible(true);
        changeTableData();
    }else{
        m_tabNode->setVisible(false);
//        m_listH = 710;
        m_listH = 710 - 30;//fusheng 调低30
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_listH = 1700;
        }
        m_data->addObjectsFromArray(m_srcData);
    }
    m_infoList->setContentSize(CCSizeMake(m_infoList->getContentSize().width, m_listH+m_addHeight));
    m_tableView->setViewSize(m_infoList->getContentSize());
    m_tableView->reloadData();
    if(m_data->count()<=0 && GlobalData::shared()->playerInfo.isInAlliance() && !m_first){
        m_infoTxt->setString(_lang("115129"));
    }else{
        m_infoTxt->setString("");
    }
}

void AllianceWarView::changeTableData(){
    m_data->removeAllObjects();
    int num = m_srcData->count();
    if (m_tabType==0) {
        for (int i=0; i<num; i++) {
            AllianceTeamInfo* info = (AllianceTeamInfo*)m_srcData->objectAtIndex(i);
            if (info->getMonsterCount()<=0) {//玩家队伍
                m_data->addObject(info);
            }
        }
    }else{
        for (int i=0; i<num; i++) {
            AllianceTeamInfo* info = (AllianceTeamInfo*)m_srcData->objectAtIndex(i);
            if (info->getMonsterCount()>0) {//有怪/
                m_data->addObject(info);
            }
        }
    }
}

void AllianceWarView::updateInfo(CCObject* data)
{
    this->removeLoadingAni();
    NetResult* result = dynamic_cast<NetResult*>(data);
    auto dic = _dict(result->getData());
    m_first = false;
    if(dic){
        m_srcData->removeAllObjects();
        auto zuduiArray = CCArray::create();
        auto gerenArray = CCArray::create();
        
        CCArray* array = (CCArray*)dic->objectForKey("list");
        int monsterNum = 0;
        if(array){
            int count = array->count();
            for(int i=0;i<count;i++){
                auto item = _dict(array->objectAtIndex(i));
                AllianceTeamInfo* info = new AllianceTeamInfo();
                info->parseInfo(item);
//                m_srcData->addObject(info);
                
                int battleType = info->getBattleType();
                if (battleType == 2 || battleType ==3) {
                    zuduiArray->addObject(info);
                }else {
                    gerenArray->addObject(info);
                }
                
                if (info->getMonsterCount()>0) {
                    monsterNum+=1;
                }
                CC_SAFE_RELEASE_NULL(info);
            }
        }
        m_srcData->addObjectsFromArray(zuduiArray);
        m_srcData->addObjectsFromArray(gerenArray);
        zuduiArray->removeAllObjects();
        gerenArray->removeAllObjects();
        
        GlobalData::shared()->playerInfo.allianceInfo.militaryNum = m_srcData->count();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHANGE_ALLIANCE_MILITARY_NUM);
        if (monsterNum>0 && monsterNum==m_srcData->count()) {//没有联盟战，就直接打开怪物的tab
            onClickTab2(NULL,Control::EventType::TOUCH_DOWN);
        }else{
            refreshTableView();
        }
    }
}

void AllianceWarView::onEnter()
{
    CCNode::onEnter();
    this->setTitleName(_lang("115190").c_str());
    if((m_data->count()<=0 || WorldController::getInstance()->forceUPdate) && GlobalData::shared()->playerInfo.isInAlliance()){
        m_data->removeAllObjects();
        WorldController::getInstance()->forceUPdate = false;
        GetAllianceWarListCommand* cmd = new GetAllianceWarListCommand();
        cmd->setCallback(CCCallFuncO::create(this,callfuncO_selector(AllianceWarView::updateInfo), NULL));
        cmd->setFailCallback(CCCallFuncO::create(this,callfuncO_selector(AllianceWarView::removeLoadingAni), NULL));
        cmd->sendAndRelease();
        addLoadingAni();
    }else{
        refresh(NULL);
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceWarView::refresh), MSG_UPDATE_ALLIANCE_WAR_CELL_DATA, NULL);
}


void AllianceWarView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_UPDATE_ALLIANCE_WAR_CELL_DATA);
    CCNode::onExit();
}

void AllianceWarView::refresh(CCObject* param){
    int num = m_srcData->count();
    for(int i=0;i<num;i++){
        AllianceTeamInfo* info = (AllianceTeamInfo*)m_srcData->objectAtIndex(i);
        double marchTime = info->getMarchTime() - GlobalData::shared()->getWorldTime();
        std::string territoryFlag = info->getTerritoryFlag();
        int territoryStat = info->getTerritoryStat();
        double destroyTime = info->getTerritoryCrashTime() - GlobalData::shared()->getTimeStamp();
        if (destroyTime<=0 && territoryFlag.compare("true") == 0 && territoryStat == 6) {
            m_srcData->removeObjectAtIndex(i);
            i--;
            num--;
        }
        else if (marchTime<=0 && territoryFlag.compare("true") == 0 && territoryStat != 6 && territoryStat != -1) {
            m_srcData->removeObjectAtIndex(i);
            i--;
            num--;
        }
        else if (marchTime<=0 && territoryFlag.compare("true") != 0) {
            m_srcData->removeObjectAtIndex(i);
            i--;
            num--;
        }
    }
    GlobalData::shared()->playerInfo.allianceInfo.militaryNum = m_srcData->count();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHANGE_ALLIANCE_MILITARY_NUM);
    refreshTableView();
}

void AllianceWarView::removeLoadingAni(CCObject* p){
    if(m_loadingIcon!=NULL){
        m_loadingIcon->stopAllActions();
        m_loadingIcon->removeFromParent();
    }
}

void AllianceWarView::addLoadingAni(){
    if (m_loadingIcon==NULL) {
        m_loadingIcon = CCLoadSprite::createSprite("loading_1.png");
        m_loadingIcon->setAnchorPoint(ccp(0.5, 0.5));
        auto cSize = m_infoList->getContentSize();
        m_loadingIcon->setPosition(ccp(cSize.width/2,cSize.height/2));
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_loadingIcon->setScale(2);
        }
    }
    CCActionInterval * rotateto1 = CCRotateTo::create(0.5, 720);
    CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
    CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
    m_loadingIcon->runAction(rotateForever);
    if(m_loadingIcon->getParent()==NULL){
        m_infoList->addChild(m_loadingIcon,1000000);
    }
}

CCSize AllianceWarView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    auto info = dynamic_cast<AllianceTeamInfo*>(m_data->objectAtIndex(idx));
    auto territoryFlag = info->getTerritoryFlag();
    if (info && (info->getBattleType()==2||info->getBattleType()==3)) {
        if (CCCommonUtils::isIosAndroidPad()) {
            return CCSize(1536, 1000);
        }
        return CCSize(640,450);//215
    }
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1536, 700);
    }
    return CCSize(640, 280);
}

CCSize AllianceWarView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1536, 1000);
    }
    return CCSize(640,450);
}

CCTableViewCell* AllianceWarView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    AllianceWarCell* cell = (AllianceWarCell*)table->dequeueCell();
    if(idx <m_data->count()){
        AllianceTeamInfo* info = (AllianceTeamInfo*)m_data->objectAtIndex(idx);
        if(cell){
            cell->setData(info);
        }else{
            cell = AllianceWarCell::create(info,1);
        }
    }
    table->getContainer()->reorderChild(cell,m_data->count() - idx);
    return cell;
}

ssize_t AllianceWarView::numberOfCellsInTableView(CCTableView *table)
{
    return m_data->count();
}

void AllianceWarView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}


void AllianceWarView::onBtnAttClick(CCObject * pSender, Control::EventType pCCControlEvent){

}

void AllianceWarView::onBtnDefClick(CCObject * pSender, Control::EventType pCCControlEvent){

}

void AllianceWarView::openJoinAlliance(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->removeAllPopupView();
    PopupViewController::getInstance()->addPopupInView(JoinAllianceView::create(11));
}

void AllianceWarView::onClickResult(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->addPopupInView(AllianceWarResultView::create());
}

void AllianceWarView::onClickTab1(CCObject * pSender, Control::EventType pCCControlEvent){
    m_tabType = 0;
    m_tabBtn1->setEnabled(false);
    m_tabBtn2->setEnabled(true);
    refreshTableView();
}

void AllianceWarView::onClickTab2(CCObject * pSender, Control::EventType pCCControlEvent){
    m_tabType = 1;
    m_tabBtn1->setEnabled(true);
    m_tabBtn2->setEnabled(false);
    refreshTableView();
}

bool AllianceWarView::checkOpenMonster(){
    bool flag = false;
    int num = m_srcData->count();
    for (int i=0; i<num; i++) {
        AllianceTeamInfo* info = (AllianceTeamInfo*)m_srcData->objectAtIndex(i);
        if (info->getMonsterCount()>0) {//有怪
            flag = true;
            break;
        }
    }
    return flag;
}

SEL_CCControlHandler AllianceWarView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "openJoinAlliance", AllianceWarView::openJoinAlliance);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickResult", AllianceWarView::onClickResult);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickTab1", AllianceWarView::onClickTab1);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickTab2", AllianceWarView::onClickTab2);
    return NULL;
}

bool AllianceWarView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg", CCScale9Sprite*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoTxt", CCLabelIF*, this->m_infoTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noAllianeNode", CCNode*, this->m_noAllianeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noAllianceTip", CCLabelIF*, this->m_noAllianceTip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnJoin", CCControlButton*, this->m_btnJoin);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnResult", CCControlButton*, this->m_btnResult);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tabNode", CCNode*, this->m_tabNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tabBtn1", CCControlButton*, this->m_tabBtn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tabBtn2", CCControlButton*, this->m_tabBtn2);
    return false;
}
 AllianceWarCell* AllianceWarCell::create(AllianceTeamInfo* info,int index)
{
    auto ret = new AllianceWarCell(info,index);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AllianceWarCell::init()
{
    CCLoadSprite::doResourceByCommonIndex(504, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(504, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
    m_ccbNode= CCBLoadFile("NewAllianceWarCell",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        m_ccbNode->setPositionX(776);
    } else {
        m_ccbNode->setPositionX(323);
    }
    
    m_isClickMosnter = false;
    m_headImgNode = HFHeadImgNode::create();
    this->setData(m_info);
    return true;
}

void AllianceWarCell::setData(AllianceTeamInfo* info){
    if(info->getBattleType()==2 || info->getBattleType()==3){
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_ccbNode->setPositionY(460);
        } else {
            m_ccbNode->setPositionY(200);
        }
    }else{
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_ccbNode->setPositionY(300);
        } else {
            m_ccbNode->setPositionY(120);
        }
    }
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_teamNode2->setScale(2.4);
    } else {
        m_teamNode2->setScale(1);
    }
    m_freshRally = true;
    m_info = info;
//    CCLoadSprite::doResourceByCommonIndex(7, true);
//    CCLoadSprite::doResourceByCommonIndex(205, true);
//    CCLoadSprite::doResourceByCommonIndex(500, true);
    m_head1->removeAllChildrenWithCleanup(true);
    m_head2->removeAllChildrenWithCleanup(true);
    m_teamHeadNode1->removeAllChildrenWithCleanup(true);
    m_teamHeadNode2->removeAllChildrenWithCleanup(true);
    m_tHeadNode2->removeAllChildrenWithCleanup(true);
    m_teamNode2->setVisible(false);
    m_territoryNode->setVisible(false);
    if(m_info!=NULL){
        if (m_info->getTerritoryFlag() == "true") {
            if (m_info->getTerritoryStat() == 6) {
                m_territoryNode->setVisible(true);
                m_signNode->setVisible(false);
                m_teamNode2->setVisible(false);
                std::string posXY = "";
                CCPoint pt = WorldController::getPointByIndex(m_info->getTargetId());
                posXY.append(CC_ITOA(pt.x));
                posXY.append(",");
                posXY.append(CC_ITOA(pt.y));
                m_tCoordTxt->setString(posXY);
                m_tTitleTxt->setString(_lang("115311"));
                m_tTitleTxt->setColor({0,175,205});
                m_tStatusTxt->setColor({0,175,205});
                m_tTimeTxt->setColor({0,175,205});
                std::string pName = "";
                pName = "";
                if(m_info->getAttackAAbb()!=""){
                    pName.append("(");
                    pName.append(m_info->getAttackAAbb());
                    pName.append(")");
                }
                pName.append(m_info->getAttackName());
                m_tNameTxt->setString(pName.c_str());
                
                pName = "";
                if(m_info->getTerritoryAlAbbr()!=""){
                    pName.append("(");
                    pName.append(m_info->getTerritoryAlAbbr());
                    pName.append(")");
                }
                pName.append(m_info->getTerritoryName());
                if(pName.length()>13){
                    pName = CCCommonUtils::subStrByUtf8(pName,0,12);
                    pName.append("...");
                }
                m_tTargetTxt->setString(pName);
                
                int num = m_info->getMember()->count();
                int useNum = 0;
                double hTime = m_info->getTerritoryCrashTime() - GlobalData::shared()->getTimeStamp();
                CCArray* marchArmy = CCArray::create();
                for (int i=0; i<num; i++) {
                    YuanJunInfo* yuan = (YuanJunInfo*)m_info->getMember()->objectAtIndex(i);
                    if(hTime <=0){
                        continue;
                    }
                    marchArmy->addObject(yuan);
                    useNum += 1;
                }
                int maxNum = (GlobalData::shared()->world_alliance_war_k2 +CCCommonUtils::getEffectValueByNum(ALLIANCE_TEAM_MEMBER_MAX));
                int limitNum = useNum;
                if(useNum<5 && useNum < maxNum){
                    limitNum += 1;
                }
                if(limitNum>5) limitNum = 5;
                for (int i=0; i<limitNum; i++) {
                    string iconStr = "";
                    string uid = "";
                    int picVer = 0;
                    if(useNum>=5 && i==4 && limitNum < maxNum){
                        iconStr = "icon_hospital.png";
                    }else{
                        if(i<useNum){
                            YuanJunInfo* yuan = (YuanJunInfo*)marchArmy->objectAtIndex(i);
                            iconStr = yuan->getPic();
                            uid = yuan->getUid();
                            picVer = yuan->getpicVer();
                            if(iconStr==""||iconStr=="0"){
                                iconStr = "g044";
                            }
                            iconStr.append(".png");
                        }else{
                            iconStr = "icon_hospital.png";
                        }
                    }
                    AllianceWarHeadCell* cell = AllianceWarHeadCell::create(iconStr,i,uid,picVer);
                    cell->setPosition(ccp(-35+72.2*i,-35));
                    m_tHeadNode2->addChild(cell);
                }
                m_tNumTxt->setString(CC_ITOA(limitNum-1));
                string picStr = "territory_building_lv0";
                if (m_info->getTerritoryType() == "tower") {
                    picStr = "territory_building_lv0";
                }
                else if (m_info->getTerritoryType() == "flag") {
                    picStr = "territory_tower0";
                }
                picStr.append(".png");
                auto pic = CCLoadSprite::createSprite(picStr.c_str());
                CCCommonUtils::setSpriteMaxSize(pic, 120);
                m_tPicNode->addChild(pic);
                double haveTime = m_info->getTerritoryCrashTime() - GlobalData::shared()->getTimeStamp();
                if(haveTime<=0){
                    m_freshRally = false;
                }
                else{
                    this->unschedule(schedule_selector(AllianceWarCell::updateTime));
                    this->schedule(schedule_selector(AllianceWarCell::updateTime), 1);
                    this->updateTime(0);
                }

            }
            if (m_info->getTerritoryStat() != -1 && m_info->getTerritoryStat() != 6) {
                int type = m_info->getBattleType();//0 个人进攻， 1 个人防守，2 组队进攻 3 组队防守
                m_signNode->setVisible(type==0||type==1);
                m_teamNode2->setVisible(type==2||type==3);
                CCArray* armys = CCArray::create();
                m_targetTitle->setString(_lang("115224"));
                std::string pName = "";
                std::string posXY = "";
                if(m_info->getMySide()==1){
                    CCPoint pt = WorldController::getPointByIndex(m_info->getAttackId());
                    posXY.append(CC_ITOA(pt.x));
                    posXY.append(",");
                    posXY.append(CC_ITOA(pt.y));
                    m_sTxt3->setString(posXY);
                    pt = WorldController::getPointByIndex(m_info->getTargetId());
                    posXY = "";
                    posXY.append(CC_ITOA(pt.x));
                    posXY.append(",");
                    posXY.append(CC_ITOA(pt.y));
                    m_sTxt4->setString(posXY);
                    m_teamTargetPosTxt1->setString(posXY);

                    m_sTitleTxt->setColor({230,32,0});
                    m_sStatusTxt->setColor({230,32,0});
                    m_sTimeTxt->setColor({230,32,0});
                    
                    m_teamTitleTxt->setColor({230,32,0});
                    m_teamStatusTxt->setColor({230,32,0});
                    m_teamTimeTxt->setColor({230,32,0});
                    
                    pName = "";
                    if(m_info->getAttackAAbb()!=""){
                        pName.append("(");
                        pName.append(m_info->getAttackAAbb());
                        pName.append(")");
                    }
                    pName.append(m_info->getAttackName());
                    m_teamTxt1->setString(pName.c_str());
                    
                    pName = "";
                    if(m_info->getTargetAAbbr()!=""){
                        pName.append("(");
                        pName.append(m_info->getTargetAAbbr());
                        pName.append(")");
                    }
                    pName.append(m_info->getTargetName());
                    m_teamTxt3->setString(pName.c_str());
                    
                    m_teamAction1->initWithSpriteFrame(CCLoadSprite::loadResource("icon_Allance_team_attack01.png"));
                    m_teamAction2->initWithSpriteFrame(CCLoadSprite::loadResource("icon_Allance_team_defense02.png"));

                    m_sTitleTxt->setString(_lang("115220"));
                    m_teamTitleTxt->setString(_lang("108582"));
                    m_teamAttNode->setVisible(true);
                    m_teamAttNode2->setVisible(true);
                    m_teamDefNode->setVisible(false);
                    m_teamDefNode2->setVisible(false);
                }else{
                    CCPoint pt = WorldController::getPointByIndex(m_info->getTargetId());
                    posXY = "";
                    posXY.append(CC_ITOA(pt.x));
                    posXY.append(",");
                    posXY.append(CC_ITOA(pt.y));
                    m_sTxt3->setString(posXY);
                    m_teamTargetPosTxt1->setString(posXY);
                    pt = WorldController::getPointByIndex(m_info->getAttackId());
                    posXY = "";
                    posXY.append(CC_ITOA(pt.x));
                    posXY.append(",");
                    posXY.append(CC_ITOA(pt.y));
                    m_sTxt4->setString(posXY);
                    
                    m_sTitleTxt->setColor({0,175,205});
                    m_sStatusTxt->setColor({0,175,205});
                    m_sTimeTxt->setColor({0,175,205});
                    
                    m_teamTitleTxt->setColor({0,175,205});
                    m_teamStatusTxt->setColor({0,175,205});
                    m_teamTimeTxt->setColor({0,175,205});
                    
                    pName = "";
                    if(m_info->getAttackAAbb()!=""){
                        pName.append("(");
                        pName.append(m_info->getAttackAAbb());
                        pName.append(")");
                    }
                    pName.append(m_info->getAttackName());
                    m_teamTxt3->setString(pName.c_str());
                    
                    pName = "";
                    if(m_info->getTargetAAbbr()!=""){
                        pName.append("(");
                        pName.append(m_info->getTargetAAbbr());
                        pName.append(")");
                    }
                    pName.append(m_info->getTargetName());
                    m_teamTxt1->setString(pName.c_str());
                    
                    m_teamAction2->initWithSpriteFrame(CCLoadSprite::loadResource("icon_Allance_team_attack01.png"));
                    m_teamAction1->initWithSpriteFrame(CCLoadSprite::loadResource("icon_Allance_team_defense02.png"));
                    
                    m_sTitleTxt->setString(_lang("115221"));
                    m_teamTitleTxt->setString(_lang("115221"));
                    m_teamAttNode->setVisible(false);
                    m_teamAttNode2->setVisible(false);
                    m_teamDefNode->setVisible(true);
                    m_teamDefNode2->setVisible(true);
                }

                std::string tarName = m_info->getTerritoryName();
                if(tarName.length()>13){
                    tarName = CCCommonUtils::subStrByUtf8(tarName,0,12);
                    tarName.append("...");
                }
                m_teamTargetTxt->setString(tarName);
                
                m_cityIconNode->removeAllChildrenWithCleanup(true);
                string picStr = "territory_building_lv1";
                picStr.append(".png");
                auto tempPic = CCLoadSprite::createSprite(picStr.c_str());
                CCCommonUtils::setSpriteMaxSize(tempPic, 136);
                m_cityIconNode->addChild(tempPic);
                
                std::string nameStr = "";
                pName = "";
                int offX1 = 0;
                int offX2 = 0;
                switch (type) {
                    case 0:
                    {
                        m_sAttNode->setVisible(true);
                        m_sDefNode->setVisible(false);
                        string iconStr = m_info->getAttackPic();
                        string uid = m_info->getAttackUid();
                        int picVer = m_info->getAttackPicVer();
                        if(iconStr==""||iconStr=="0"){
                            iconStr = "g044";
                        }
                        iconStr.append(".png");
                        auto pic = CCLoadSprite::createSprite(iconStr.c_str());
                        CCCommonUtils::setSpriteMaxSize(pic, 108);
                        m_head1->addChild(pic);
                        if (CCCommonUtils::isUseCustomPic(picVer))
                        {
                            m_headImgNode->initHeadImgUrl2(m_head1, CCCommonUtils::getCustomPicUrl(uid, picVer), 1.0f, 108, true);
                        }
                        if(m_info->getAttackAAbb()!=""){
                            pName.append("(");
                            pName.append(m_info->getAttackAAbb());
                            pName.append(")");
                        }
                        pName.append(m_info->getAttackName());
                        if(pName.length()>15){
                            pName = CCCommonUtils::subStrByUtf8(pName,0,14);
                            pName.append("...");
                        }
                        m_sTxt1->setString(pName.c_str());
                        if (m_info->getTargetUid()=="") {
                            iconStr = "territory_building_lv0";
                            if (m_info->getTerritoryType() == "tower") {
                                iconStr = "territory_building_lv0";
                            }
                            else if (m_info->getTerritoryType() == "flag") {
                                iconStr = "territory_tower0";
                            }
                            iconStr.append(".png");
                            pic = CCLoadSprite::createSprite(iconStr.c_str());
                            CCCommonUtils::setSpriteMaxSize(pic, 108);
                            m_head2->addChild(pic);
                            pName = "";
                            if(m_info->getTerritoryAlAbbr()!=""){
                                pName.append("(");
                                pName.append(m_info->getTerritoryAlAbbr());
                                pName.append(")");
                            }
                            pName.append(m_info->getTerritoryName());
                            if(pName.length()>15){
                                pName = CCCommonUtils::subStrByUtf8(pName,0,14);
                                pName.append("...");
                            }
                            m_sTxt2->setString(pName.c_str());
                        }
                        else {
                            iconStr = m_info->getTargetPic();
                            uid = m_info->getTargetUid();
                            picVer = m_info->getTargetPicVer();
                            if(iconStr==""||iconStr=="0"){
                                iconStr = "g044";
                            }
                            iconStr.append(".png");
                            pic = CCLoadSprite::createSprite(iconStr.c_str());
                            CCCommonUtils::setSpriteMaxSize(pic, 108);
                            m_head2->addChild(pic);
                            if (CCCommonUtils::isUseCustomPic(picVer))
                            {
                                m_headImgNode->initHeadImgUrl2(m_head2, CCCommonUtils::getCustomPicUrl(uid, picVer), 1.0f, 108, true);
                            }
                            pName = "";
                            if(m_info->getTargetAAbbr()!=""){
                                pName.append("(");
                                pName.append(m_info->getTargetAAbbr());
                                pName.append(")");
                            }
                            pName.append(m_info->getTargetName());
                            if(pName.length()>15){
                                pName = CCCommonUtils::subStrByUtf8(pName,0,14);
                                pName.append("...");
                            }
                            m_sTxt2->setString(pName.c_str());
                        }
                        m_signSpr1->initWithSpriteFrame(CCLoadSprite::loadResource("icon_Allance_team_attack01.png"));
                        m_signSpr2->initWithSpriteFrame(CCLoadSprite::loadResource("icon_Allance_team_defense02.png"));
                    }
                        break;
                    case 1:
                    {
                        m_sAttNode->setVisible(false);
                        m_sDefNode->setVisible(true);
                        string iconStr = m_info->getAttackPic();
                        string uid = m_info->getAttackUid();
                        int picVer = m_info->getAttackPicVer();
                        if(iconStr==""||iconStr=="0"){
                            iconStr = "g044";
                        }
                        iconStr.append(".png");
                        auto pic = CCLoadSprite::createSprite(iconStr.c_str());
                        CCCommonUtils::setSpriteMaxSize(pic, 108);
                        m_head2->addChild(pic);
                        if (CCCommonUtils::isUseCustomPic(picVer))
                        {
                            m_headImgNode->initHeadImgUrl2(m_head2, CCCommonUtils::getCustomPicUrl(uid, picVer), 1.0f, 108, true);
                        }
                        if(m_info->getAttackAAbb()!=""){
                            pName.append("(");
                            pName.append(m_info->getAttackAAbb());
                            pName.append(")");
                        }
                        pName.append(m_info->getAttackName());
                        if(pName.length()>15){
                            pName = CCCommonUtils::subStrByUtf8(pName,0,14);
                            pName.append("...");
                        }
                        m_sTxt2->setString(pName.c_str());
                        if (m_info->getTargetUid()=="") {
                            iconStr = "territory_building_lv0";
                            if (m_info->getTerritoryType() == "tower") {
                                iconStr = "territory_building_lv0";
                            }
                            else if (m_info->getTerritoryType() == "flag") {
                                iconStr = "territory_tower0";
                            }
                            iconStr.append(".png");
                            pic = CCLoadSprite::createSprite(iconStr.c_str());
                            CCCommonUtils::setSpriteMaxSize(pic, 108);
                            m_head1->addChild(pic);
                            pName = "";
                            if(m_info->getTerritoryAlAbbr()!=""){
                                pName.append("(");
                                pName.append(m_info->getTerritoryAlAbbr());
                                pName.append(")");
                            }
                            pName.append(m_info->getTerritoryName());
                            if(pName.length()>15){
                                pName = CCCommonUtils::subStrByUtf8(pName,0,14);
                                pName.append("...");
                            }
                            m_sTxt1->setString(pName.c_str());
                        }
                        else {
                            iconStr = m_info->getTargetPic();
                            uid = m_info->getTargetUid();
                            picVer = m_info->getTargetPicVer();
                            if(iconStr==""||iconStr=="0"){
                                iconStr = "g044";
                            }
                            iconStr.append(".png");
                            pic = CCLoadSprite::createSprite(iconStr.c_str());
                            CCCommonUtils::setSpriteMaxSize(pic, 108);
                            m_head1->addChild(pic);
                            if (CCCommonUtils::isUseCustomPic(picVer))
                            {
                                m_headImgNode->initHeadImgUrl2(m_head2, CCCommonUtils::getCustomPicUrl(uid, picVer), 1.0f, 108, true);
                            }
                            pName = "";
                            if(m_info->getTargetAAbbr()!=""){
                                pName.append("(");
                                pName.append(m_info->getTargetAAbbr());
                                pName.append(")");
                            }
                            pName.append(m_info->getTargetName());
                            if(pName.length()>15){
                                pName = CCCommonUtils::subStrByUtf8(pName,0,14);
                                pName.append("...");
                            }
                            m_sTxt1->setString(pName.c_str());
                        }
                        m_signSpr1->initWithSpriteFrame(CCLoadSprite::loadResource("icon_Allance_team_defense02.png"));
                        m_signSpr2->initWithSpriteFrame(CCLoadSprite::loadResource("icon_Allance_team_attack01.png"));
                    }
                        break;
                    case 2:
                    {
                        int numSoldier = m_info->getReinforce()->count();
                        m_teamTxt4->setString(CC_ITOA(numSoldier));
                        int num = m_info->getMember()->count();
                        int useNum = 0;
                        double hTime = (m_info->getWaitTime() - GlobalData::shared()->getWorldTime());
                        CCArray* marchArmy = CCArray::create();
                        for (int i=0; i<num; i++) {
                            YuanJunInfo* yuan = (YuanJunInfo*)m_info->getMember()->objectAtIndex(i);
                            double arraivalTime = (yuan->getArrivalTime() - GlobalData::shared()->getWorldTime()); //出发行军了，还没有到达的，去掉
                            if(hTime <=0 && arraivalTime > 0 && !yuan->getIsLeader()){
                                continue;
                            }
                            marchArmy->addObject(yuan);
                            useNum += 1;
                        }
                        int maxNum = (GlobalData::shared()->world_alliance_war_k2 +CCCommonUtils::getEffectValueByNum(ALLIANCE_TEAM_MEMBER_MAX));
                        int limitNum = useNum;
                        if(useNum<5 && useNum < maxNum){
                            limitNum += 1;
                        }
                        if(limitNum>5) limitNum = 5;
                        for (int i=0; i<limitNum; i++) {
                            string iconStr = "";
                            string uid = "";
                            int picVer = 0;
                            if(useNum>=5 && i==4 && limitNum < maxNum){
                                iconStr = "icon_hospital.png";
                            }else{
                                if(i<useNum){
                                    YuanJunInfo* yuan = (YuanJunInfo*)marchArmy->objectAtIndex(i);
                                    iconStr = yuan->getPic();
                                    uid = yuan->getUid();
                                    picVer = yuan->getpicVer();
                                    if(iconStr==""||iconStr=="0"){
                                        iconStr = "g044";
                                    }
                                    iconStr.append(".png");
                                }else{
                                    iconStr = "icon_hospital.png";
                                }
                            }
                            AllianceWarHeadCell* cell = AllianceWarHeadCell::create(iconStr,i,uid,picVer);
                            cell->setPosition(ccp(-180+72.2*i,-35));
                            m_teamHeadNode1->addChild(cell);
                        }
                        m_teamTxt2->setString(CC_ITOA(useNum));
                        if (useNum>5) {
                            useNum = 5;
                        }
                        offX1 = useNum*58;
                        
                        armys->addObjectsFromArray(m_info->getReinforce());
                        num = armys->count();
                        if (num>5) {
                            num = 5;
                        }
                        for (int i=0; i<num; i++) {
                            YuanJunInfo* yuan = (YuanJunInfo*)armys->objectAtIndex(i);
                            string iconStr = yuan->getPic();
                            if(iconStr==""||iconStr=="0"){
                                iconStr = "g044";
                            }
                            iconStr.append(".png");
                            AllianceWarHeadCell* cell = AllianceWarHeadCell::create(iconStr,i,yuan->getUid(),yuan->getpicVer());
                            cell->setPosition(ccp(-180+72.2*i,-35));
                            m_teamHeadNode2->addChild(cell);
                        }
                    }
                        
                        break;
                    case 3:
                    {
                        int numSoldier = m_info->getMember()->count();
                        m_teamTxt4->setString(CC_ITOA(numSoldier));
                        numSoldier = m_info->getReinforce()->count();
                        m_teamTxt2->setString(CC_ITOA(numSoldier));
                        
                        int num = m_info->getMember()->count();
                        if(num>5) num = 5;
                        for (int i=0; i<num; i++) {
                            string iconStr = "";
                            string uid = "";
                            int picVer = 0;
                            if(num>=5 && i==4 && m_info->getMaxSoldiers()!=m_info->getCurrSoldiers()){
                                iconStr = "icon_hospital.png";
                            }else{
                                YuanJunInfo* yuan = (YuanJunInfo*)m_info->getMember()->objectAtIndex(i);
                                iconStr = yuan->getPic();
                                uid = yuan->getUid();
                                picVer = yuan->getpicVer();
                                if(iconStr==""||iconStr=="0"){
                                    iconStr = "g044";
                                }
                                iconStr.append(".png");
                            }
                            AllianceWarHeadCell* cell = AllianceWarHeadCell::create(iconStr,i,uid,picVer);
                            cell->setPosition(ccp(-180+72.2*i,-35));
                            m_teamHeadNode2->addChild(cell);
                        }
                        offX2 = num*58;

                        armys->addObjectsFromArray(m_info->getReinforce());
                        num = armys->count();
                        int limitNum = num;
                        if(num<5 && m_info->getMaxSoldiers()!=m_info->getCurrSoldiers()){
                            limitNum +=1;
                        }
                        if(limitNum>5) limitNum = 5;
                        for (int i=0; i<limitNum; i++) {
                            string iconStr = "";
                            string uid = "";
                            int picVer = 0;
                            if(num>=5 && i==4 && m_info->getMaxSoldiers()!=m_info->getCurrSoldiers()){
                                iconStr = "icon_hospital.png";
                            }else{
                                if(i<num){
                                    YuanJunInfo* yuan = (YuanJunInfo*)armys->objectAtIndex(i);
                                    iconStr = yuan->getPic();
                                    uid = yuan->getUid();
                                    picVer = yuan->getpicVer();
                                    if(iconStr==""||iconStr=="0"){
                                        iconStr = "g044";
                                    }
                                    iconStr.append(".png");
                                }else{
                                    iconStr = "icon_hospital.png";
                                }
                            }
                            AllianceWarHeadCell* cell = AllianceWarHeadCell::create(iconStr,i,uid,picVer);
                            cell->setPosition(ccp(-180+72.2*i,-35));
                            m_teamHeadNode1->addChild(cell);
                        }
                    }
                        break;
                }
                double haveTime = (m_info->getWaitTime() - GlobalData::shared()->getWorldTime());
                double march = m_info->getMarchTime() - GlobalData::shared()->getWorldTime();
                if(haveTime>0 || march>0){
                    if(haveTime<=0){
                        m_freshRally = false;
                    }
                    this->unschedule(schedule_selector(AllianceWarCell::updateTime));
                    this->schedule(schedule_selector(AllianceWarCell::updateTime), 1);
                    this->updateTime(0);
                }
            }
        }
        else {
            int type = m_info->getBattleType();//0 个人进攻， 1 个人防守，2 组队进攻 3 组队防守
            m_signNode->setVisible(type==0||type==1);
            m_teamNode2->setVisible(type==2||type==3);
            CCArray* armys = CCArray::create();
            m_targetTitle->setString(_lang("115224"));
            std::string flagIcon = "";
            std::string pName = "";
            std::string posXY = "";
            if(m_info->getMySide()==1){
                flagIcon = "Allance_team_jindutiao01.png";
                CCPoint pt = WorldController::getPointByIndex(m_info->getAttackId());
                posXY.append(CC_ITOA(pt.x));
                posXY.append(",");
                posXY.append(CC_ITOA(pt.y));
                m_sTxt3->setString(posXY);
                pt = WorldController::getPointByIndex(m_info->getTargetId());
                posXY = "";
                posXY.append(CC_ITOA(pt.x));
                posXY.append(",");
                posXY.append(CC_ITOA(pt.y));
                m_sTxt4->setString(posXY);
                m_teamTargetPosTxt1->setString(posXY);
                
                m_sTitleTxt->setColor({230,32,0});
                m_sStatusTxt->setColor({230,32,0});
                m_sTimeTxt->setColor({230,32,0});
                
                m_teamTitleTxt->setColor({230,32,0});
                m_teamStatusTxt->setColor({230,32,0});
                m_teamTimeTxt->setColor({230,32,0});
                
                pName = "";
                if(m_info->getAttackAAbb()!=""){
                    pName.append("(");
                    pName.append(m_info->getAttackAAbb());
                    pName.append(")");
                }
                pName.append(m_info->getAttackName());
                m_teamTxt1->setString(pName.c_str());
                
                pName = "";
                if(m_info->getTargetAAbbr()!=""){
                    pName.append("(");
                    pName.append(m_info->getTargetAAbbr());
                    pName.append(")");
                }
                pName.append(m_info->getTargetName());
                m_teamTxt3->setString(pName.c_str());
                
                m_teamAction1->initWithSpriteFrame(CCLoadSprite::loadResource("icon_Allance_team_attack01.png"));
                m_teamAction2->initWithSpriteFrame(CCLoadSprite::loadResource("icon_Allance_team_defense02.png"));
                m_sTitleTxt->setString(_lang("115220"));
                m_teamTitleTxt->setString(_lang("108582"));
                m_teamAttNode->setVisible(true);
                m_teamAttNode2->setVisible(true);
                m_teamDefNode->setVisible(false);
                m_teamDefNode2->setVisible(false);
            }else{
                flagIcon = "Allance_team_jindutiao02.png";
                CCPoint pt = WorldController::getPointByIndex(m_info->getTargetId());
                posXY = "";
                posXY.append(CC_ITOA(pt.x));
                posXY.append(",");
                posXY.append(CC_ITOA(pt.y));
                m_sTxt3->setString(posXY);
                m_teamTargetPosTxt1->setString(posXY);
                pt = WorldController::getPointByIndex(m_info->getAttackId());
                posXY = "";
                posXY.append(CC_ITOA(pt.x));
                posXY.append(",");
                posXY.append(CC_ITOA(pt.y));
                m_sTxt4->setString(posXY);
                
                m_sTitleTxt->setColor({0,175,205});
                m_sStatusTxt->setColor({0,175,205});
                m_sTimeTxt->setColor({0,175,205});
                
                m_teamTitleTxt->setColor({0,175,205});
                m_teamStatusTxt->setColor({0,175,205});
                m_teamTimeTxt->setColor({0,175,205});
                
                pName = "";
                if(m_info->getAttackAAbb()!=""){
                    pName.append("(");
                    pName.append(m_info->getAttackAAbb());
                    pName.append(")");
                }
                pName.append(m_info->getAttackName());
                m_teamTxt3->setString(pName.c_str());
                
                pName = "";
                if(m_info->getTargetAAbbr()!=""){
                    pName.append("(");
                    pName.append(m_info->getTargetAAbbr());
                    pName.append(")");
                }
                pName.append(m_info->getTargetName());
                m_teamTxt1->setString(pName.c_str());
                
                m_teamAction2->initWithSpriteFrame(CCLoadSprite::loadResource("icon_Allance_team_attack01.png"));
                m_teamAction1->initWithSpriteFrame(CCLoadSprite::loadResource("icon_Allance_team_defense02.png"));
                
                m_sTitleTxt->setString(_lang("115221"));
                m_teamTitleTxt->setString(_lang("115221"));
                m_teamAttNode->setVisible(false);
                m_teamAttNode2->setVisible(false);
                m_teamDefNode->setVisible(true);
                m_teamDefNode2->setVisible(true);
            }
            std::string tarName = m_info->getTargetName();
            if(tarName.length()>13){
                tarName = CCCommonUtils::subStrByUtf8(tarName,0,12);
                tarName.append("...");
            }
            m_teamTargetTxt->setString(tarName);
            
            m_cityIconNode->removeAllChildrenWithCleanup(true);
            
            CCClipNode* cityClipper = CCClipNode::create(CCSize(136,136));
            cityClipper->setPosition(ccp(-136/2, -136/2));
            m_cityIconNode->addChild(cityClipper);
            
            CCNode* cityNode = CCNode::create();
            cityNode->setScale(0.8);
            cityClipper->addChild(cityNode);
            
            int level = m_info->getTargetCityLevel();
            int mapIndex = 3;
            while (mapIndex >= 0) {
                auto arr = WorldMapView::getCityPicArr(mapIndex, level, false);
                int posX = _tile_width / 2;
                int posY = _tile_height / 2;
                if(mapIndex == 0 || mapIndex == 2){
                    posX = _tile_width;
                }
                if(mapIndex == 3){
                    posX = _tile_width + posX;
                }
                if(mapIndex == 1 || mapIndex == 3){
                    posY = _tile_height;
                }
                if(mapIndex == 2){
                    posY = _tile_height + posY;
                }
                CCObject *obj = NULL;
                CCARRAY_FOREACH(arr, obj){
                    std::string picName = _dict(obj)->valueForKey("pic")->getCString();
                    int addX = _dict(obj)->valueForKey("x")->intValue();
                    int addY = _dict(obj)->valueForKey("y")->intValue();
                    auto sprite = CCLoadSprite::createSprite(picName.c_str());
                    sprite->setAnchorPoint(ccp(0, 0));
                    cityNode->addChild(sprite);
                    sprite->setPosition(ccp(posX, posY) - ccp(_tile_width / 2, _tile_height / 2) + ccp(addX, addY) + ccp(-_tile_width/1.6 - 5,-_tile_height/1.6));
                }
                mapIndex--;
            }
            
            if (m_info->getTargetType() == ActBossTile) {
//                CCLoadSprite::doResourceByCommonIndex(206, true);
                auto bossSpr =  CCLoadSprite::createSprite("scws.png",true,CCLoadSpriteType_MONSTERLAYERLITTLE);
                bossSpr->setScale(1.4);
                int bsW = bossSpr->getContentSize().width*bossSpr->getScale();
                bossSpr->setPosition(ccp(bsW/2, bsW/2));
                cityNode->addChild(bossSpr);
                
                std::string tarName = _lang("111300");
                if(tarName.length()>13){
                    tarName = CCCommonUtils::subStrByUtf8(tarName,0,12);
                    tarName.append("...");
                }
                m_teamTargetTxt->setString(tarName);
            }
            
            //citySprite->cocos2d::CCNode::setPosition(posX - _tile_width/1.6 - 10, posY - _tile_height/1.6);
            
            std::string nameStr = "";
            pName = "";
            int offX1 = 0;
            int offX2 = 0;
            switch (type) {
                case 0:
                {
                    m_sAttNode->setVisible(true);
                    m_sDefNode->setVisible(false);
                    string iconStr = m_info->getAttackPic();
                    string uid = m_info->getAttackUid();
                    int picVer = m_info->getAttackPicVer();
                    if(iconStr==""||iconStr=="0"){
                        iconStr = "g044";
                    }
                    iconStr.append(".png");
                    auto pic = CCLoadSprite::createSprite(iconStr.c_str());
                    CCCommonUtils::setSpriteMaxSize(pic, 108);
                    m_head1->addChild(pic);
                    if (CCCommonUtils::isUseCustomPic(picVer))
                    {
                        m_headImgNode->initHeadImgUrl2(m_head1, CCCommonUtils::getCustomPicUrl(uid, picVer), 1.0f, 108, true);
                    }
                    iconStr = m_info->getTargetPic();
                    uid = m_info->getTargetUid();
                    picVer = m_info->getTargetPicVer();
                    if(iconStr==""||iconStr=="0"){
                        iconStr = "g044";
                    }
                    iconStr.append(".png");
                    pic = CCLoadSprite::createSprite(iconStr.c_str());
                    CCCommonUtils::setSpriteMaxSize(pic, 108);
                    m_head2->addChild(pic);
                    if (CCCommonUtils::isUseCustomPic(picVer))
                    {
                        m_headImgNode->initHeadImgUrl2(m_head2, CCCommonUtils::getCustomPicUrl(uid, picVer), 1.0f, 108, true);
                    }
                    if(m_info->getAttackAAbb()!=""){
                        pName.append("(");
                        pName.append(m_info->getAttackAAbb());
                        pName.append(")");
                    }
                    pName.append(m_info->getAttackName());
                    if(pName.length()>15){
                        pName = CCCommonUtils::subStrByUtf8(pName,0,14);
                        pName.append("...");
                    }
                    m_sTxt1->setString(pName.c_str());
                    pName = "";
                    if(m_info->getTargetAAbbr()!=""){
                        pName.append("(");
                        pName.append(m_info->getTargetAAbbr());
                        pName.append(")");
                    }
                    pName.append(m_info->getTargetName());
                    if(pName.length()>15){
                        pName = CCCommonUtils::subStrByUtf8(pName,0,14);
                        pName.append("...");
                    }
                    m_sTxt2->setString(pName.c_str());
                    m_signSpr1->initWithSpriteFrame(CCLoadSprite::loadResource("icon_Allance_team_attack01.png"));
                    m_signSpr2->initWithSpriteFrame(CCLoadSprite::loadResource("icon_Allance_team_defense02.png"));
                }
                    
                    break;
                case 1:
                {
                    m_sAttNode->setVisible(false);
                    m_sDefNode->setVisible(true);
                    string iconStr = m_info->getTargetPic();
                    string uid = m_info->getTargetUid();
                    int picVer = m_info->getTargetPicVer();
                    if(iconStr==""||iconStr=="0"){
                        iconStr = "g044";
                    }
                    iconStr.append(".png");
                    auto pic = CCLoadSprite::createSprite(iconStr.c_str());
                    pic->setScale(0.8);
                    m_head1->addChild(pic);
                    if (CCCommonUtils::isUseCustomPic(picVer))
                    {
                        m_headImgNode->initHeadImgUrl2(m_head1, CCCommonUtils::getCustomPicUrl(uid, picVer), 1.0f, 108, true);
                    }
                    iconStr = m_info->getAttackPic();
                    uid = m_info->getAttackUid();
                    picVer = m_info->getAttackPicVer();
                    if(iconStr==""||iconStr=="0"){
                        iconStr = "g044";
                    }
                    iconStr.append(".png");
                    if(m_info->getMonsterCount()>0){
                        iconStr = "tile_pop_icon21.png";
                        pic = CCLoadSprite::createSprite(iconStr.c_str());
                        CCCommonUtils::setSpriteMaxSize(pic, 108);
                        m_head2->addChild(pic);
                        int mType = 0;
                        int color = 0;
                        string monsterStr = CCCommonUtils::getPropById(m_info->getAttackUid(), "monster");
                        if(monsterStr=="heiqishi_1"){
                            mType = 3;
                            color = 0;
                        }else if (monsterStr=="heiqishi_2"){
                            mType = 3;
                            color = 1;
                        }else{
                            mType = 1;
                            color = 3;
                        }
                        auto preBg = CCLoadSprite::createSprite(CCString::createWithFormat("Activty_icon_BG%d.png",mType)->getCString());
                        CCCommonUtils::setSpriteMaxSize(preBg, 108,true);
                        m_head2->addChild(preBg);
                        auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(color).c_str());
                        CCCommonUtils::setSpriteMaxSize(iconBg, 105,true);
                        m_head2->addChild(iconBg,-10);
                    }else{
                        pic = CCLoadSprite::createSprite(iconStr.c_str());
                        CCCommonUtils::setSpriteMaxSize(pic, 108);
                        m_head2->addChild(pic);
                        if (CCCommonUtils::isUseCustomPic(picVer))
                        {
                            m_headImgNode->initHeadImgUrl2(m_head2, CCCommonUtils::getCustomPicUrl(uid, picVer), 1.0f, 108, true);
                        }
                    }
                    if(m_info->getAttackAAbb()!=""){
                        pName.append("(");
                        pName.append(m_info->getAttackAAbb());
                        pName.append(")");
                    }
                    pName.append(m_info->getAttackName());
                    if(pName.length()>15){
                        pName = CCCommonUtils::subStrByUtf8(pName,0,14);
                        pName.append("...");
                    }
                    m_sTxt2->setString(pName.c_str());
                    
                    pName = "";
                    if(m_info->getTargetAAbbr()!=""){
                        pName.append("(");
                        pName.append(m_info->getTargetAAbbr());
                        pName.append(")");
                    }
                    pName.append(m_info->getTargetName());
                    if(pName.length()>15){
                        pName = CCCommonUtils::subStrByUtf8(pName,0,14);
                        pName.append("...");
                    }
                    m_sTxt1->setString(pName.c_str());
                    
                    m_signSpr1->initWithSpriteFrame(CCLoadSprite::loadResource("icon_Allance_team_defense02.png"));
                    m_signSpr2->initWithSpriteFrame(CCLoadSprite::loadResource("icon_Allance_team_attack01.png"));
                }
                    
                    break;
                case 2:
                {
                    int numSoldier = m_info->getReinforce()->count()+1;
                    m_teamTxt4->setString(CC_ITOA(numSoldier));
                    int num = m_info->getMember()->count();
                    int useNum = 0;
                    double hTime = (m_info->getWaitTime() - GlobalData::shared()->getWorldTime());
                    CCArray* marchArmy = CCArray::create();
                    for (int i=0; i<num; i++) {
                        YuanJunInfo* yuan = (YuanJunInfo*)m_info->getMember()->objectAtIndex(i);
                        double arraivalTime = (yuan->getArrivalTime() - GlobalData::shared()->getWorldTime()); //出发行军了，还没有到达的，去掉
                        if(hTime <=0 && arraivalTime > 0 && !yuan->getIsLeader()){
                            continue;
                        }
                        marchArmy->addObject(yuan);
                        useNum += 1;
                    }
                    int maxNum = (GlobalData::shared()->world_alliance_war_k2 +CCCommonUtils::getEffectValueByNum(ALLIANCE_TEAM_MEMBER_MAX));
                    int limitNum = useNum;
                    if(useNum<5 && useNum < maxNum){
                        limitNum += 1;
                    }
                    if(limitNum>5) limitNum = 5;
                    for (int i=0; i<limitNum; i++) {
                        string iconStr = "";
                        string uid = "";
                        int picVer = 0;
                        if(useNum>=5 && i==4 && limitNum < maxNum){
                            iconStr = "icon_hospital.png";
                        }else{
                            if(i<useNum){
                                YuanJunInfo* yuan = (YuanJunInfo*)marchArmy->objectAtIndex(i);
                                iconStr = yuan->getPic();
                                uid = yuan->getUid();
                                picVer = yuan->getpicVer();
                                if(iconStr==""||iconStr=="0"){
                                    iconStr = "g044";
                                }
                                iconStr.append(".png");
                            }else{
                                iconStr = "icon_hospital.png";
                            }
                        }
                        AllianceWarHeadCell* cell = AllianceWarHeadCell::create(iconStr,i,uid,picVer);
                        cell->setPosition(ccp(-180+72.2*i,-35));
                        m_teamHeadNode1->addChild(cell);
                    }
                    m_teamTxt2->setString(CC_ITOA(useNum));
                    if (useNum>5) {
                        useNum = 5;
                    }
                    offX1 = useNum*58;
                    
                    YuanJunInfo* self = new YuanJunInfo();
                    self->setPic(m_info->getTargetPic());
                    self->setpicVer(m_info->getTargetPicVer());
                    self->setUid(m_info->getTargetUid());
                    armys->addObject(self);
                    armys->addObjectsFromArray(m_info->getReinforce());
                    CC_SAFE_RELEASE_NULL(self);
                    num = armys->count();
                    if (num>5) {
                        num = 5;
                    }
                    for (int i=0; i<num; i++) {
                        YuanJunInfo* yuan = (YuanJunInfo*)armys->objectAtIndex(i);
                        string iconStr = yuan->getPic();
                        if(iconStr==""||iconStr=="0"){
                            iconStr = "g044";
                            if (m_info->getTargetType() == ActBossTile) {
                                iconStr = "scws";
                            }
                        }
                        iconStr.append(".png");
                        AllianceWarHeadCell* cell = AllianceWarHeadCell::create(iconStr,i,yuan->getUid(),yuan->getpicVer());
                        cell->setPosition(ccp(-180+72.2*i,-35));
                        m_teamHeadNode2->addChild(cell);
                    }
                }
                    
                    break;
                case 3:
                {
                    int numSoldier = m_info->getMember()->count();
                    if(m_info->getMonsterCount()>0){
                        numSoldier = m_info->getMonsterCount();
                    }
                    m_teamTxt4->setString(CC_ITOA(numSoldier));
                    numSoldier = m_info->getReinforce()->count()+1;
                    m_teamTxt2->setString(CC_ITOA(numSoldier));
                    
                    int num = m_info->getMember()->count();
                    int mType = -1;
                    if(m_info->getMonsterCount()>0){
                        num = m_info->getMonsterCount();
                        if(num>1){
                            string monsterStr = CCCommonUtils::getPropById(m_info->getAttackUid(), "monster");
                            if(monsterStr=="heiqishi_1"){
                                mType = 3;
                            }else if (monsterStr=="heiqishi_2"){
                                mType = 2;
                            }else{
                                mType = 1;
                            }
                            
                        }
                    }
                    if(num>5) num = 5;
                    for (int i=0; i<num; i++) {
                        string iconStr = "";
                        string uid = "";
                        int picVer = 0;
                        if(m_info->getMonsterCount()>0){
                            if(num>=5 && i==4){
                                iconStr = "icon_hospital.png";
                            }else{
                                iconStr = "tile_pop_icon21.png";
                            }
                        }else{
                            if(num>=5 && i==4 && m_info->getMaxSoldiers()!=m_info->getCurrSoldiers()){
                                iconStr = "icon_hospital.png";
                            }else{
                                YuanJunInfo* yuan = (YuanJunInfo*)m_info->getMember()->objectAtIndex(i);
                                iconStr = yuan->getPic();
                                uid = yuan->getUid();
                                picVer = yuan->getpicVer();
                                if(iconStr==""||iconStr=="0"){
                                    iconStr = "g044";
                                }
                                iconStr.append(".png");
                            }
                        }
                        AllianceWarHeadCell* cell = AllianceWarHeadCell::create(iconStr,i,uid,picVer,mType);
                        cell->setPosition(ccp(-180+72.2*i,-35));
                        m_teamHeadNode2->addChild(cell);
                    }
                    offX2 = num*58;
                    YuanJunInfo* self = new YuanJunInfo();
                    self->setPic(m_info->getTargetPic());
                    self->setpicVer(m_info->getTargetPicVer());
                    self->setUid(m_info->getTargetUid());
                    armys->addObject(self);
                    armys->addObjectsFromArray(m_info->getReinforce());
                    CC_SAFE_RELEASE_NULL(self);
                    num = armys->count();
                    int limitNum = num;
                    if(num<5 && m_info->getMaxSoldiers()!=m_info->getCurrSoldiers()){
                        limitNum +=1;
                    }
                    if(limitNum>5) limitNum = 5;
                    for (int i=0; i<limitNum; i++) {
                        string iconStr = "";
                        string uid = "";
                        int picVer = 0;
                        if(num>=5 && i==4 && m_info->getMaxSoldiers()!=m_info->getCurrSoldiers()){
                            iconStr = "icon_hospital.png";
                        }else{
                            if(i<num){
                                YuanJunInfo* yuan = (YuanJunInfo*)armys->objectAtIndex(i);
                                iconStr = yuan->getPic();
                                uid = yuan->getUid();
                                picVer = yuan->getpicVer();
                                if(iconStr==""||iconStr=="0"){
                                    iconStr = "g044";
                                }
                                iconStr.append(".png");
                            }else{
                                iconStr = "icon_hospital.png";
                            }
                        }
                        AllianceWarHeadCell* cell = AllianceWarHeadCell::create(iconStr,i,uid,picVer);
                        cell->setPosition(ccp(-180+72.2*i,-35));
                        m_teamHeadNode1->addChild(cell);
                    }
                }
                    
                    break;
            }
            double haveTime = (m_info->getWaitTime() - GlobalData::shared()->getWorldTime());
            double march = m_info->getMarchTime() - GlobalData::shared()->getWorldTime();
            if(haveTime>0 || march>0){
                if(haveTime<=0){
                    m_freshRally = false;
                }
                this->unschedule(schedule_selector(AllianceWarCell::updateTime));
                this->schedule(schedule_selector(AllianceWarCell::updateTime), 1);
                this->updateTime(0);
            }
            if((type==1 || type==3) && m_info->getMonsterCount()>0){
                string mName = CCCommonUtils::getNameById(m_info->getAttackUid());
                m_sTxt2->setString(mName.c_str());
                m_teamTxt3->setString(mName.c_str());
            }
        }
    }
}

void AllianceWarCell::updateTime(float t){
    if (m_info->getTerritoryFlag() == "true" && m_info->getTerritoryStat() == 6) {
        double haveTime = m_info->getTerritoryCrashTime() - GlobalData::shared()->getTimeStamp();
        if(haveTime>0){
            m_tTimeTxt->setString(CC_SECTOA(haveTime));
            m_tStatusTxt->setString(_lang_1("115311",""));
        }
        else{
            this->unschedule(schedule_selector(AllianceWarCell::updateTime));
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_UPDATE_ALLIANCE_WAR_CELL_DATA);
        }
    }
    else{
        double haveTime = (m_info->getWaitTime() - GlobalData::shared()->getWorldTime());
        double march = m_info->getMarchTime() - GlobalData::shared()->getWorldTime();
        if(haveTime>0){
            m_sTimeTxt->setString(CC_SECTOA(haveTime));
            m_sStatusTxt->setString(_lang_1("115130",""));
            m_teamTimeTxt->setString(CC_SECTOA(haveTime));
            m_teamStatusTxt->setString(_lang_1("115130",""));
        }else if(march>=0){
            if(m_freshRally && m_info->getBattleType()==2){
                int num = m_info->getMember()->count();
                int useNum = 0;
//                CCLoadSprite::doResourceByCommonIndex(7, true);
//                CCLoadSprite::doResourceByCommonIndex(205, true);
                for (int i=0; i<num; i++) {
                    YuanJunInfo* yuan = (YuanJunInfo*)m_info->getMember()->objectAtIndex(i);
                    double arraivalTime = (yuan->getArrivalTime() - GlobalData::shared()->getWorldTime()); //出发行军了，还没有到达的，去掉
                    if(arraivalTime > 0 && !yuan->getIsLeader()){
                        continue;
                    }
                    useNum += 1;
                }
                m_teamTxt2->setString(CC_ITOA(useNum));
                m_freshRally = false;
            }
            m_sTimeTxt->setString(CC_SECTOA(march));
            m_sStatusTxt->setString(_lang_1("115131",""));
            m_teamTimeTxt->setString(CC_SECTOA(march));
            m_teamStatusTxt->setString(_lang_1("115131",""));
        }else{
            this->unschedule(schedule_selector(AllianceWarCell::updateTime));
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_UPDATE_ALLIANCE_WAR_CELL_DATA);
        }
    }
}

void AllianceWarCell::onEnter() {
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup, true);
    this->schedule(schedule_selector(AllianceWarCell::updateTime), 1);
    this->updateTime(0);
}

void AllianceWarCell::onExit() {
    this->unschedule(schedule_selector(AllianceWarCell::updateTime));
    setTouchEnabled(false);
    CCNode::onExit();
}

bool AllianceWarCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_touchPos = pTouch->getLocation();
    m_isMove = false;
    if (isTouchInside(m_clickNode,pTouch)) {
        return true;
    }
    return false;
}

void AllianceWarCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CCPoint pos = pTouch->getLocation();
    CCPoint pt ;
    if(!m_isMove){
        m_isMove = false;
        CCFadeTo* fade = CCFadeTo::create(0.25, 110);
        CCFadeTo* fade4 = CCFadeTo::create(0.2, 2);
        CCDelayTime* de = CCDelayTime::create(0.05);
        CCCallFunc* fun = CCCallFunc::create(this, callfunc_selector(AllianceWarCell::goToWorld));
        CCSequence* sc = CCSequence::create(fade,fade4,de,fun,NULL);
        if (m_info->getTerritoryFlag() == "true" && m_info->getTerritoryStat() == 6) {
            if (isTouchInside(m_tPosNode, pTouch)) {
                m_worldIndex = m_info->getTargetId();
                m_tPosNode->runAction(sc);
            }
        }
        else{
            if(m_info->getBattleType()==0 || m_info->getBattleType()==1){
                if(isTouchInside(m_signClick2,pTouch)){
                    m_isClickMosnter = false;
                    if (m_info->getMySide()==1) {
                        m_worldIndex = m_info->getAttackId();
                    }else{
                        m_worldIndex = m_info->getTargetId();
                    }
                    m_signClick2->runAction(sc);
                }else if(isTouchInside(m_signClick4,pTouch)){
                    m_isClickMosnter = true;;
                    if (m_info->getMySide()==1) {
                        m_worldIndex = m_info->getTargetId();
                    }else{
                        m_worldIndex = m_info->getAttackId();
                    }
                    m_signClick4->runAction(sc);
                }
            }else{
                if(isTouchInside(m_posNode2,pTouch)){
                    m_worldIndex = m_info->getTargetId();
                    m_posNode2->runAction(sc);
                }else{
                    //CCPoint pos = this->convertToWorldSpace(CCPoint(0, 0));
                    //this->getAnimationManager()->runAnimationsForSequenceNamed("Click");
                    //PopupViewController::getInstance()->addPopupView(AllianceTeamOpenAni::create(m_info,pos));
                    //PopupViewController::getInstance()->addPopupInView(AllianceWarDetailView::create(m_info));
                    m_teamNode2->stopAllActions();
                    CCScaleTo* sc1 = nullptr, *sc2 = nullptr, *sc3 = nullptr;
                    if (CCCommonUtils::isIosAndroidPad())
                    {
                        sc1 = CCScaleTo::create(0.05, 2.4,1.2 * 2.4);
                        sc2 = CCScaleTo::create(0.07, 2.4,0.8 * 2.4);
                        sc3 = CCScaleTo::create(0.04, 2.4,1 * 2.4);
                    } else {
                        sc1 = CCScaleTo::create(0.05, 1,1.2);
                        sc2 = CCScaleTo::create(0.07, 1,0.8);
                        sc3 = CCScaleTo::create(0.04, 1,1);
                    }
                    CCDelayTime* de = CCDelayTime::create(0.05);
                    CCCallFunc* fun = CCCallFunc::create(this, callfunc_selector(AllianceWarCell::playScaleAniEnd));
                    CCSequence* sc = CCSequence::create(sc1,sc2,sc3,de,fun,NULL);
                    m_teamNode2->runAction(sc);
                }
            }
        }
    }
}

void AllianceWarCell::goToWorld(){
    CCPoint pt = WorldController::getPointByIndex(m_worldIndex);
    PopupViewController::getInstance()->removeAllPopupView();
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
        if (m_info->getmarchType() == MethodHeiqishi && m_isClickMosnter == true) {
            if(WorldMapView::instance()->m_map){
                WorldController::getInstance()->openMarchId= m_info->getmarchId();
                WorldMapView::instance()->m_map->updateDynamicMap(WorldController::getInstance()->selfPoint);
            }
//            WorldMapView::instance()->showTroopInfo(m_info->getmarchId());
        }
        else{
            WorldMapView::instance()->gotoTilePoint(pt);
            WorldMapView::instance()->openTilePanel(m_worldIndex);
        }
        
    }else{
        if (m_info->getmarchType() == MethodHeiqishi && m_isClickMosnter == true) {
            WorldController::getInstance()->openMarchId= m_info->getmarchId();
            SceneController::getInstance()->gotoScene(SCENE_ID_WORLD);
        }
        else{
            WorldController::getInstance()->openTargetIndex = m_worldIndex;
            SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, m_worldIndex);
        }
        
    }
}

void AllianceWarCell::playScaleAniEnd(){
    if (CCCommonUtils::isIosAndroidPad()) {
        m_teamNode2->setScale(2.4);
    } else {
        m_teamNode2->setScale(1.0);
    }
    PopupViewController::getInstance()->addPopupInViewWithAnim(AllianceWarDetailView::create(m_info));
}

void AllianceWarCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CCPoint pos = pTouch->getLocation();
    if (fabs(pos.y - m_touchPos.y)>30) {
        m_isMove = true;
    }
}

SEL_CCControlHandler AllianceWarCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}


bool AllianceWarCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_posNode2", CCNode*, this->m_posNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode", CCNode*, this->m_clickNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_signNode", CCNode*, this->m_signNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sAttNode", CCNode*, this->m_sAttNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sDefNode", CCNode*, this->m_sDefNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_head1", CCNode*, this->m_head1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_head2", CCNode*, this->m_head2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_signSpr1", CCSprite*, this->m_signSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_signSpr2", CCSprite*, this->m_signSpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sStatusTxt", CCLabelIF*, this->m_sStatusTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sTitleTxt", CCLabelIF*, this->m_sTitleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sTimeTxt", CCLabelIF*, this->m_sTimeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sTxt1", CCLabelIFTTF*, this->m_sTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sTxt2", CCLabelIFTTF*, this->m_sTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sTxt3", CCLabelIF*, this->m_sTxt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sTxt4", CCLabelIF*, this->m_sTxt4);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_teamNode2", CCNode*, this->m_teamNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_teamTitleTxt", CCLabelIF*, this->m_teamTitleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_teamStatusTxt", CCLabelIF*, this->m_teamStatusTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_teamTimeTxt", CCLabelIF*, this->m_teamTimeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_targetTitle", CCLabelIF*, this->m_targetTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_teamTargetPosTxt1", CCLabelIF*, this->m_teamTargetPosTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_teamTargetTxt", CCLabelIF*, this->m_teamTargetTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cityIconNode", CCNode*, this->m_cityIconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_teamAction1", CCSprite*, this->m_teamAction1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_teamAction2", CCSprite*, this->m_teamAction2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_teamTxt1", CCLabelIFTTF*, this->m_teamTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_teamTxt2", CCLabelIF*, this->m_teamTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_teamTxt3", CCLabelIFTTF*, this->m_teamTxt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_teamTxt4", CCLabelIF*, this->m_teamTxt4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_teamHeadNode1", CCNode*, this->m_teamHeadNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_teamHeadNode2", CCNode*, this->m_teamHeadNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_teamAttNode", CCNode*, this->m_teamAttNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_teamDefNode", CCNode*, this->m_teamDefNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_teamAttNode2", CCNode*, this->m_teamAttNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_teamDefNode2", CCNode*, this->m_teamDefNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_signClick1", CCNode*, this->m_signClick1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_signClick2", CCNode*, this->m_signClick2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_signClick3", CCNode*, this->m_signClick3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_signClick4", CCNode*, this->m_signClick4);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tTargetTxt", CCLabelIF*, this->m_tTargetTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tCoordTxt", CCLabelIF*, this->m_tCoordTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tNameTxt", CCLabelIF*, this->m_tNameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tNumTxt", CCLabelIF*, this->m_tNumTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tTitleTxt", CCLabelIF*, this->m_tTitleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tStatusTxt", CCLabelIF*, this->m_tStatusTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tTimeTxt", CCLabelIF*, this->m_tTimeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tHeadNode2", CCNode*, this->m_tHeadNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_territoryNode", CCNode*, this->m_territoryNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tPosNode", CCNode*, this->m_tPosNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tPicNode", CCNode*, this->m_tPicNode);
    return false;
}

