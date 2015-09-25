//
//  WatchtowerView.cpp
//  IF
//
//  Created by fubin on 14-2-24.
//
//

#include "WatchtowerView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "FunBuildController.h"
#include "YesNoDialog.h"
#include "EnemyInfoController.h"
#include "EnemyInfoMoreView.h"
#include "BuildMoreInfoView.h"
#include "NewEnemyInfoMoreView.h"
#include "WorldController.h"
#include "ActivityController.h"
#include "MonsterSiegeActivityInfoCommand.h"
#include "AllianceWarView.h"
#include "JoinAllianceView.h"

const float numPerRow = 1.0;
WatchtowerView* WatchtowerView::create(int buildId){
    WatchtowerView* ret = new WatchtowerView();
    if(ret && ret->init(buildId)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool WatchtowerView::init(int buildId)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(8, true);
    CCLoadSprite::doResourceByCommonIndex(500, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(8, false);
        CCLoadSprite::doResourceByCommonIndex(500, false);
    });
    auto tbg = CCLoadSprite::loadResource("technology_09.png");
    auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
    int maxHight = CCDirector::sharedDirector()->getWinSize().height;
    int curHight = -500;
    while (curHight<maxHight) {
        auto bg = CCLoadSprite::createSprite("technology_09.png");
        bg->setAnchorPoint(ccp(0, 1));
        bg->setPosition(ccp(0, curHight));
        curHight += bg->getContentSize().height;
        tBatchNode->addChild(bg);
        if (CCCommonUtils::isIosAndroidPad())
        {
            bg->setScale(2.4f);
        }
    }
    this->addChild(tBatchNode);
    m_buildId = buildId;
    auto tmpCCB = CCBLoadFile("WatchtowerView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
    setTitleName(_lang("102160").c_str());
    m_callCount = 10;
    m_tabView = NULL;
    m_buildBG->setVisible(false);
    if (!CCCommonUtils::isIosAndroidPad()) {
        int oldBgHeight = m_buildBG->getContentSize().height;
        changeBGHeight(m_buildBG);
        int newBgHeight = m_buildBG->getContentSize().height;
        int extH = getExtendHeight();
        int addHeight = newBgHeight - oldBgHeight;
        int oldWidth = m_infoList->getContentSize().width;
        int oldHeight = m_infoList->getContentSize().height;
        //    m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
        m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+extH));
        m_bottomNode->setPositionY(m_bottomNode->getPositionY() - extH);
        
        bool flag = false;
        if (ActivityController::getInstance()->aActivityInfo.activityST<=0 || ActivityController::getInstance()->aActivityInfo.siegeET<=0 ||ActivityController::getInstance()->aActivityInfo.round==0) {
            MonsterSiegeActivityInfoCommand* cmd = new MonsterSiegeActivityInfoCommand();
            cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(WatchtowerView::getInfoCallBack), NULL));
            cmd->sendAndRelease();
        }else{
            double prevTime = GlobalData::shared()->getWorldTime() - ActivityController::getInstance()->aActivityInfo.activityST;
            double gapTime = ActivityController::getInstance()->aActivityInfo.activityET - GlobalData::shared()->getWorldTime() ;
            double gapEnd = ActivityController::getInstance()->aActivityInfo.siegeET - GlobalData::shared()->getWorldTime();
            flag = ActivityController::getInstance()->aActivityInfo.activityST>0 && gapEnd>0 && gapTime>0 && prevTime>=0 && GlobalData::shared()->playerInfo.isInAlliance();
        }
        if(flag){
            showMonsterInfo();
        }
    }
    m_tabView = CCMultiColTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setMultiColTableViewDelegate(this);
    m_tabView->setTouchPriority(Touch_Popup);
    m_infoList->addChild(m_tabView);
    
    m_msgLabel->setString(_lang("108647"));
    
    FunBuildInfo& m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
    
    string title = _lang(m_info.name)+" "+_lang_1("102272", CC_ITOA(m_info.level));
    if(m_info.level>=GlobalData::shared()->MaxBuildLv) {
        title += _lang("102183");
    }
    setTitleName(title.c_str());
    CCCommonUtils::setButtonTitle(m_ignoreBtn, _lang("139500").c_str());
    if (EnemyInfoController::getInstance()->m_enemyInfos.size() > 0) {
        m_bottomNode->setVisible(true);
    }
    else
        m_bottomNode->setVisible(false);
    updateInfo(NULL);
    
    return true;
}

void WatchtowerView::updateTime(float _time){
    if (ActivityController::getInstance()->aActivityInfo.activityST==0) {
        return;
    }
    double prevTime = GlobalData::shared()->getWorldTime() - ActivityController::getInstance()->aActivityInfo.activityST;
    double gapTime = ActivityController::getInstance()->aActivityInfo.activityET - GlobalData::shared()->getWorldTime() ;
    double gapEnd = ActivityController::getInstance()->aActivityInfo.siegeET - GlobalData::shared()->getWorldTime();
    m_txt2->setString(_lang_1("133035", CC_ITOA(ActivityController::getInstance()->aActivityInfo.round)));
    string temp = "";
    if(ActivityController::getInstance()->aActivityInfo.siegeST>0 && gapTime>0 && gapEnd>=0){//活动开始
        auto gtime = ActivityController::getInstance()->aActivityInfo.nextRoundTime - GlobalData::shared()->getWorldTime();
        if(gtime<=0){
            if(gapEnd>0 && ActivityController::getInstance()->aActivityInfo.nextRoundTime!=0 && m_callCount>0){//活动没有结束，但到下一波怪出怪了，所以再请求下下,波怪的时间  m_callCount 是最多在打开面板后调10次
                m_callCount -= 1;
                MonsterSiegeActivityInfoCommand* cmd = new MonsterSiegeActivityInfoCommand();
                cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(WatchtowerView::getInfoCallBack), NULL));
                cmd->sendAndRelease();
            }
        }else{
            temp = _lang("133037");
            temp.append(_lang_1("133038", CC_SECTOA(gtime)));
        }
    }
    m_txt3->setString(temp);
}

void WatchtowerView::getInfoCallBack(CCObject* param){
    NetResult* result = dynamic_cast<NetResult*>(param);
    if(result==NULL) return ;
    CCDictionary* dicData = _dict(result->getData());
    ActivityController::getInstance()->aActivityInfo.parse(dicData);
    double prevTime = GlobalData::shared()->getWorldTime() - ActivityController::getInstance()->aActivityInfo.activityST;
    double gapTime = ActivityController::getInstance()->aActivityInfo.activityET - GlobalData::shared()->getWorldTime() ;
    double gapEnd = ActivityController::getInstance()->aActivityInfo.siegeET - GlobalData::shared()->getWorldTime();
    bool flag = ActivityController::getInstance()->aActivityInfo.activityST>0 && gapEnd>0 && gapTime>0 && prevTime>=0 && GlobalData::shared()->playerInfo.isInAlliance();
    if (flag) {
        showMonsterInfo();
    }
}

void WatchtowerView::showMonsterInfo(){
    if (m_monster->isVisible()) {
        return ;
    }
    if (ActivityController::getInstance()->aActivityInfo.round==0) {
        m_monster->setVisible(false);
        return ;
    }
    double prevTime = GlobalData::shared()->getWorldTime() - ActivityController::getInstance()->aActivityInfo.activityST;
    double gapTime = ActivityController::getInstance()->aActivityInfo.activityET - GlobalData::shared()->getWorldTime() ;
    double gapEnd = ActivityController::getInstance()->aActivityInfo.siegeET - GlobalData::shared()->getWorldTime();
    bool flag = ActivityController::getInstance()->aActivityInfo.activityST>0 && gapEnd>0 && gapTime>0 && prevTime>=0 && GlobalData::shared()->playerInfo.isInAlliance();
    m_monster->setVisible(true);
    if (m_tabView==NULL) {
        m_infoList->setContentSize(CCSizeMake(m_infoList->getContentSize().width, m_infoList->getContentSize().height - 300));
        m_infoList->setPositionY(m_infoList->getPositionY() - 300);
    }
    m_txt1->setString(_lang("133003"));
    m_txt4->setString(_lang("133056"));
    CCCommonUtils::setButtonTitle(m_helpBtn, _lang("133272").c_str());
    if (m_tabView!=NULL) {
        m_tabView->setContentSize(CCSize(m_infoList->getContentSize().width,m_infoList->getContentSize().height-300));
        m_tabView->setViewSize(CCSize(m_infoList->getContentSize().width,m_infoList->getContentSize().height-300));
        m_tabView->reloadData();
    }
    this->unschedule(schedule_selector(WatchtowerView::updateTime));
    this->schedule(schedule_selector(WatchtowerView::updateTime), 1);
    updateTime(0);
}

void WatchtowerView::updateInfo(CCObject* p)
{
//    refreshIgnoreList(NULL);
    m_tabView->getContainer()->removeAllChildren();
    EnemyInfoController::getInstance()->checkEnemyData();
    m_dataCnt = EnemyInfoController::getInstance()->m_enemyInfos.size();
    m_tabView->reloadData();
    m_msgLabel->setVisible(false);
    if(m_dataCnt<=0) {
        m_msgLabel->setVisible(true);
    }
}

void WatchtowerView::onEnter()
{
    CCNode::onEnter();
    refreshIgnoreList(NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(WatchtowerView::updateInfo), MSG_WATCH_TOWER, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(WatchtowerView::updateInfo), ENEMY_INFO_LIST_CHANGE, NULL);
}

void WatchtowerView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_WATCH_TOWER);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, ENEMY_INFO_LIST_CHANGE);
    CCNode::onExit();
}

void WatchtowerView::refreshIgnoreList(cocos2d::CCObject *obj){
    if (m_bottomNode->isVisible()) {
        m_ignoreBtn->setEnabled(false);
        std::string ignoreStr = GlobalData::shared()->playerInfo.uid + "WTIgnoreList";
        std::string ignoreList = CCUserDefault::sharedUserDefault()->getStringForKey(ignoreStr.c_str(), "");
        vector<std::string> vector1;
        CCCommonUtils::splitString(ignoreList, "|", vector1);
        int size = EnemyInfoController::getInstance()->m_enemyInfos.size();
        int num = vector1.size();
        for (int i = 0; i < size; i++) {
            bool flag = true;
            EnemyInfo& info = EnemyInfoController::getInstance()->m_enemyInfos[i];
            for (int j = 0; j < num; j++) {
                if (info.uuid == vector1[j]) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                m_ignoreBtn->setEnabled(true);
                break;
            }
        }
    }
}

void WatchtowerView::onClickIgnoreBtn(CCObject *pSender, CCControlEvent event){
    YesNoDialog::showVariableTitle(_lang("139502").c_str(), CCCallFunc::create(this, callfunc_selector(WatchtowerView::secondConfirm)), _lang("confirm").c_str());
}

void WatchtowerView::secondConfirm(){
    std::string ignoreStr = GlobalData::shared()->playerInfo.uid + "WTIgnoreList";
    std::string ignoreList = CCUserDefault::sharedUserDefault()->getStringForKey(ignoreStr.c_str(), "");
    vector<std::string> vector1;
    CCCommonUtils::splitString(ignoreList, "|", vector1);
    int size = EnemyInfoController::getInstance()->m_enemyInfos.size();
    int num = vector1.size();
    for (int i = 0; i < size; i++) {
        bool flag = true;
        EnemyInfo& info = EnemyInfoController::getInstance()->m_enemyInfos[i];
        for (int j = 0; j < num; j++) {
            if (info.uuid == vector1[j]) {
                flag = false;
                break;
            }
        }
        if (flag) {
            if (ignoreList == "") {
                ignoreList.append(info.uuid);
            }
            else {
                ignoreList.append("|");
                ignoreList.append(info.uuid);
            }
        }
    }
    CCUserDefault::sharedUserDefault()->setStringForKey(ignoreStr.c_str(), ignoreList.c_str());
    CCUserDefault::sharedUserDefault()->flush();
    m_ignoreBtn->setEnabled(false);
}

void WatchtowerView::onClickHelpBtn(CCObject *pSender, CCControlEvent event){
    if(GlobalData::shared()->playerInfo.isInAlliance()){
        PopupViewController::getInstance()->addPopupInView(AllianceWarView::create(1));
    }else{
        PopupViewController::getInstance()->addPopupInView(JoinAllianceView::create(11));
    }
}

SEL_CCControlHandler WatchtowerView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickIgnoreBtn", WatchtowerView::onClickIgnoreBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickHelpBtn", WatchtowerView::onClickHelpBtn);
    return NULL;
}

bool WatchtowerView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_baseContainer", CCNode*, this->m_baseContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgLabel", CCLabelIF*, this->m_msgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ignoreBtn", CCControlButton*, this->m_ignoreBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_monster", CCNode*, this->m_monster);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpBtn", CCControlButton*, this->m_helpBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt1", CCLabelIF*, this->m_txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt2", CCLabelIF*, this->m_txt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt3", CCLabelIF*, this->m_txt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt4", CCLabelIF*, this->m_txt4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);
    return false;
}

void WatchtowerView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell)
{
    EnemyInfoCell* enemyCell = dynamic_cast<EnemyInfoCell*>(cell);
    enemyCell->cellTouchEnded(m_tabView->m_pCurTouch);
}

cocos2d::CCSize WatchtowerView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1460, 280);
    }
    return CCSize(620, 150);
}

CCTableViewCell* WatchtowerView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx)
{
    if(idx > m_cellNum){
        return NULL;
    }
    
    EnemyInfoCell* cell = (EnemyInfoCell*)table->dequeueGrid();
    if(idx <= m_cellNum){
        if(cell){
            cell->setData(idx,m_buildId);
        }else{
            cell = EnemyInfoCell::create(idx, m_buildId);
        }
    }
    return cell;
}

ssize_t WatchtowerView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    m_cellNum = ceil(m_dataCnt / numPerRow);
    return m_cellNum;
}

unsigned int WatchtowerView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return numPerRow;
}

/////

EnemyInfoCell* EnemyInfoCell::create(int itemId, int buildId)
{
    auto ret = new EnemyInfoCell();
    if (ret && ret->init(itemId, buildId)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EnemyInfoCell::init(int itemId, int buildId)
{
    bool ret = true;
    CCBLoadFile("EnemyInfoCell",this,this);
    m_headImgNode = HFHeadImgNode::create();
    setData(itemId, buildId);
    if (CCCommonUtils::isIosAndroidPad()) {
        setContentSize(CCSizeMake(1500, 270));
    }
    setContentSize(CCSizeMake(620, 150));
    return ret;
}

void EnemyInfoCell::setData(int itemId, int buildId)
{
    m_itemId = itemId;
    m_buildId = buildId;
    
    m_mainNode->setVisible(true);
    EnemyInfo& info = EnemyInfoController::getInstance()->m_enemyInfos[m_itemId];
    
    std::string nameStr = info.name;
    if(info.abbr != ""){
        nameStr = std::string("(") + info.abbr + ")" + info.name;
    }
    m_nameLabel->setString(nameStr.c_str());
    m_timeLabel->setString(CCCommonUtils::timeStampToDate(info.stampTime).c_str());
//    m_timeLabel->setAnchorPoint(ccp(0.5, 0.5));
//    m_timeLabel->setPosition(ccp(302.0, 87.9));
//    m_timeLabel->setPositionX(m_barLabel->getPositionX());
    
    string picName = info.pic+".png";//默认 出征者头像
//    if (info.type == ENEMY_TYPE_BATTLE) {//被攻击
//        picName = "march_battle.png";
//    }
//    else if (info.type == ENEMY_TYPE_SCOUT) {//侦查
//        picName = "march_scout.png";
//    }
//    else if (info.type == ENEMY_TYPE_TRADE) {//交易
//        picName = "march_trade.png";
//    }
//    else if (info.type == ENEMY_TYPE_TEAM) {//集结
//        picName = "march_team.png";
//    }
//    else if (info.type == ENEMY_TYPE_MEMBER) {//协防
//        picName = "march_member.png";
//    }
//    else if (info.type == ENEMY_TYPE_MONSTER_ATTACK) {//世界活动，怪物攻城
//        picName = "tile_pop_icon21.png";
//    }
    
    string strTypeName = _lang("108586");//默认 援军
    if (info.type == ENEMY_TYPE_BATTLE) {
        strTypeName = _lang("108607");
    }
    else if (info.type == ENEMY_TYPE_SCOUT) {
        strTypeName = _lang("108605");
    }
    else if (info.type == ENEMY_TYPE_TRADE) {
        strTypeName = _lang("108606");
        m_timeLabel->setColor(ccc3(62,85,46));
    }
    else if (info.type == ENEMY_TYPE_TEAM) {
        strTypeName = _lang("115132");
    }
    else if (info.type == ENEMY_TYPE_MEMBER) {
        strTypeName = _lang("108582");
        m_timeLabel->setColor(ccc3(62,85,46));
    }
    else if (info.type == ENEMY_TYPE_MONSTER_ATTACK) {
        strTypeName = _lang("108607");
    }
    else
    {
        m_timeLabel->setColor(ccc3(62,85,46));
    }
    
    if ((info.defType == Tile_allianceArea || info.defType == tile_tower || info.defType == tile_banner) && info.allianceId == GlobalData::shared()->playerInfo.allianceInfo.uid) {
        strTypeName = _lang("108586");//援军
    }
    
//    m_nameLabel->setPositionY(95);
    m_nameLabel->setSystemFontSize(26);
    if (CCCommonUtils::isIosAndroidPad()) {
        m_nameLabel->setSystemFontSize(50);
    }
    if (info.type == ENEMY_TYPE_TEAM)
    {
        m_node1->setVisible(false);
        m_node2->setVisible(true);
        m_titleLabel0->setString(strTypeName);
        m_titleLabel1->setString(_lang("129056"));
        m_nameLabel->setPositionY(120);
        m_nameLabel->setSystemFontSize(22);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_nameLabel->setSystemFontSize(50);
            m_nameLabel->setPositionY(130);
        }
    }else if(info.type == ENEMY_TYPE_MONSTER_ATTACK){
        m_node1->setVisible(true);
        m_node2->setVisible(false);
        m_timeLabel->setString(strTypeName);
        string mName = CCCommonUtils::getNameById(info.uid);
        std::string levelStr = _lang_1("107005", CC_ITOA(info.level));
        mName +="  ";//等级
        mName +=levelStr;
        m_nameLabel->setString(mName.c_str());
    }
    else
    {
        m_node1->setVisible(true);
        m_node2->setVisible(false);
        m_timeLabel->setString(strTypeName);
    }

    m_picNode->removeAllChildren();
    auto pic = CCLoadSprite::createSprite(picName.c_str());
    if(pic){
        int size = 95;
        if (CCCommonUtils::isIosAndroidPad()) {
            size = 180;
        }
        CCCommonUtils::setSpriteMaxSize(pic, size);//80
        m_picNode->addChild(pic);
    }
    if (CCCommonUtils::isUseCustomPic(info.picVer))
    {
        int size = 95;
        if (CCCommonUtils::isIosAndroidPad()) {
            size = 180;
        }
        m_headImgNode->initHeadImgUrl2(m_picNode, CCCommonUtils::getCustomPicUrl(info.uid, info.picVer), 1.0f, size, true);
    }
    
    if(info.type == ENEMY_TYPE_MONSTER_ATTACK){
        int mType = 0;
        int color = 1;
        string monsterStr = CCCommonUtils::getPropById(info.uid, "monster");
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
        int size = 95;
        if (CCCommonUtils::isIosAndroidPad()) {
            size = 180;
        }
        CCCommonUtils::setSpriteMaxSize(preBg, size,true);
        m_picNode->addChild(preBg);
        auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(color).c_str());
        CCCommonUtils::setSpriteMaxSize(iconBg, size,true);
        m_picNode->addChild(iconBg,-100);
    }

    m_timeLabel->setVisible(true);
    
    m_sumTime = info.marchTime;
    m_arrTime = info.arrivalTime;
    m_waitTime = m_arrTime - info.createTime - m_sumTime;
    m_type = info.type;
    
    onEnterFrame(0);
}

void EnemyInfoCell::onEnter() {
    CCNode::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(EnemyInfoCell::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
}

void EnemyInfoCell::onExit() {
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(EnemyInfoCell::onEnterFrame), this);
    CCNode::onExit();
}

void EnemyInfoCell::onEnterFrame(float dt)
{
    EnemyInfo& info = EnemyInfoController::getInstance()->m_enemyInfos[m_itemId];
    if(!NewEnemyInfoMoreView::isContains("3", m_buildId)&&info.type != ENEMY_TYPE_MONSTER_ATTACK){
//        m_barLabel->setString(_lang("108611"));
        //***************************************************
        std::string progressStr="";
        if (info.type == ENEMY_TYPE_BATTLE||info.type == ENEMY_TYPE_TEAM) {
            switch (info.defType) {
                case 1://玩家城市
                    progressStr = _lang("137417");
                    break;
                case 2://扎营地
                    progressStr = _lang("137419");
                    break;
                case 3://资源
                    progressStr = _lang("137418");
                    break;
                case tile_tower:
                    if (info.allianceId == GlobalData::shared()->playerInfo.allianceInfo.uid) {
                        progressStr = _lang("115413");
                    }
                    break;
                case Tile_allianceArea:
                    if (info.allianceId == GlobalData::shared()->playerInfo.allianceInfo.uid) {
                        progressStr = _lang("115361");
                    }
                    break;
                case tile_banner:
                    if (info.allianceId == GlobalData::shared()->playerInfo.allianceInfo.uid) {
                        progressStr = _lang("115538");
                    }
                    break;
                default:
                    break;
            }
        }
        if (info.type == 7) {//援助
            switch (info.defType) {
                case 1://玩家城市
                    progressStr = _lang("137420");
                    break;
                case 2://扎营地
                    progressStr = _lang("137422");
                    break;
                case 3://资源
                    progressStr = _lang("137421");
                    break;
                default:
                    break;
            }
        }
        if (info.type == ENEMY_TYPE_SCOUT) {//侦查
            progressStr = _lang("137440");
        }
        if(progressStr==""){
            progressStr = _lang("108611");
        }
        m_barLabel->setString(progressStr);
        //***************************************************
        
        m_bar->setColor(ccBLACK);
        //组队攻击
//        m_barLabel1->setString(_lang("108611"));//5级解锁
//        m_barLabel0->setString(_lang("108611"));
        return;
    }
    int lastTime = m_arrTime - GlobalData::shared()->getWorldTime();
    
    if (m_type == ENEMY_TYPE_TEAM)
    {
        if (lastTime>m_sumTime)
        {
            //组队中
            float pro0 = 1-(lastTime-m_sumTime)*1.0/m_waitTime;
            m_barLabel1->setString(CC_SECTOA(m_sumTime));
            m_barLabel0->setString(CC_SECTOA(lastTime-m_sumTime));
            if (pro0 < 0) {
                m_bar0->setScaleX(0);
            } else {
                m_bar0->setScaleX(pro0);
            }
            m_bar1->setScaleX(0);
        }
        else
        {
            //行军中
            float pro0 = 1-lastTime*1.0/m_sumTime;
            m_barLabel1->setString(CC_SECTOA(lastTime));
            m_barLabel0->setString(CC_SECTOA(0));
            if (pro0 < 0) {
                m_bar1->setScaleX(0);
            } else {
                m_bar1->setScaleX(pro0);
            }
            m_bar0->setScaleX(1);
        }

    }
    else
    {
        float pro = 1-lastTime*1.0/m_sumTime;
        EnemyInfo& info = EnemyInfoController::getInstance()->m_enemyInfos[m_itemId];
        std::string progressStr="";
        if (info.type == ENEMY_TYPE_BATTLE||info.type == ENEMY_TYPE_TEAM) {
            switch (info.defType) {
                case 1://玩家城市
                    progressStr = _lang_1("137411", CC_SECTOA(lastTime));
//                    progressStr += "部队于";
//                    progressStr += CC_SECTOA(lastTime);
//                    progressStr += "后攻击您的城堡";
                    break;
                case 2://扎营地
                    progressStr = _lang_1("137413", CC_SECTOA(lastTime));
                    break;
                case 3://资源
                    progressStr = _lang_1("137412", CC_SECTOA(lastTime));
                    break;
                case tile_tower:
                    if (info.allianceId == GlobalData::shared()->playerInfo.allianceInfo.uid) {
                        progressStr = _lang_1("115412", CC_SECTOA(lastTime));
                    }
                    break;
                case Tile_allianceArea:
                    if (info.allianceId == GlobalData::shared()->playerInfo.allianceInfo.uid) {
                        progressStr = _lang_1("115360", CC_SECTOA(lastTime));
                    }
                    break;
                case tile_banner:
                    if (info.allianceId == GlobalData::shared()->playerInfo.allianceInfo.uid) {
                        progressStr = _lang_1("115537", CC_SECTOA(lastTime));
                    }
                    break;
                default:
                    break;
            }
        }
        if (info.type == 7) {//援助
            switch (info.defType) {
                case 1://玩家城市
                    progressStr = _lang_1("137414", CC_SECTOA(lastTime));
//                    progressStr += "部队于";
//                    progressStr += CC_SECTOA(lastTime);
//                    progressStr += "后援助您的城堡";
                    break;
                case 2://扎营地
                    progressStr = _lang_1("137416", CC_SECTOA(lastTime));
                    break;
                case 3://资源
                    progressStr = _lang_1("137415", CC_SECTOA(lastTime));
                    break;
                default:
                    break;
            }
        }
        if (info.type == ENEMY_TYPE_SCOUT) {//侦查
            switch (info.defType) {
                case 1://玩家城市
                    progressStr = _lang_1("137426", CC_SECTOA(lastTime));
//                    progressStr += "部队于";
//                    progressStr += CC_SECTOA(lastTime);
//                    progressStr += "后侦查您的城堡";
                    break;
                case 2://扎营地
                    progressStr = _lang_1("137428", CC_SECTOA(lastTime));
                    break;
                case 3://资源
                    progressStr = _lang_1("137427", CC_SECTOA(lastTime));
                    break;
                default:
                    break;
            }
        }
        if(progressStr==""){//其他情况之显示时间
            progressStr=CC_SECTOA(lastTime);
        }
        m_barLabel->setString(progressStr);
        
        float fw = m_barLabel->getContentSize().width;
        if(fw>550){//防爆框
            m_barLabel->setScaleX(550.0/fw);
        }
        
        if (pro < 0) {
            m_bar->setScaleX(0);
        } else {
            m_bar->setScaleX(pro);
        }
    }
    
    if (lastTime<=0) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_WATCH_TOWER);
    }
}

SEL_CCControlHandler EnemyInfoCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickWatchBtn", EnemyInfoCell::onClickWatchBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMoreClick", EnemyInfoCell::onMoreClick);
    return NULL;
}

void EnemyInfoCell::onClickWatchBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
//    PopupViewController::getInstance()->addPopupInView(EnemyInfoMoreView::create(m_itemId, m_buildId));
    EnemyInfo& info = EnemyInfoController::getInstance()->m_enemyInfos[m_itemId];
    PopupViewController::getInstance()->addPopupInView(NewEnemyInfoMoreView::create(info.uuid, m_buildId, info.type));
}

void EnemyInfoCell::onMoreClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->addPopupView(BuildMoreInfoView::create(m_buildId));
}

void EnemyInfoCell::cellTouchEnded(cocos2d::CCTouch *pTouch) {
//    PopupViewController::getInstance()->addPopupInView(EnemyInfoMoreView::create(m_itemId, m_buildId));
    EnemyInfo& info = EnemyInfoController::getInstance()->m_enemyInfos[m_itemId];
    PopupViewController::getInstance()->addPopupInView(NewEnemyInfoMoreView::create(info.uuid, m_buildId, info.type));
}

bool EnemyInfoCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_watchBtn", CCControlButton*, this->m_watchBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", Label*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barLabel", CCLabelIF*, m_barLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bar", CCScale9Sprite*, m_bar);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, m_mainNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node1", CCNode*, m_node1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node2", CCNode*, m_node2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel0", CCLabelIF*, m_titleLabel0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel1", CCLabelIF*, m_titleLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barLabel0", CCLabelIF*, m_barLabel0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barLabel1", CCLabelIF*, m_barLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bar0", CCScale9Sprite*, m_bar0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bar1", CCScale9Sprite*, m_bar1);
    return false;
}