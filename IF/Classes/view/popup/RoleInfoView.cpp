//
//  RoleInfoView.cpp
//  IF
//
//  Created by ganxiaohua on 14-3-21.
//
//

#include "RoleInfoView.h"
#include "PopupViewController.h"
#include "AllianceScienceView.h"
#include "TalentController.h"
#include "ChatView.h"
#include "YesNoDialog.h"
#include "GetUserInfoCommand.h"
#include "MailWritePopUpView.h"
#include "BattleManager.h"
#include "GetPowerInfoCommand.h"
#include "AllianceInfoView.h"
#include "CheckAllianceInfoView.h"
#include "MailController.h"
#include "InvitesAllianceCommand.h"
#include "ChatController.h"
#include "GeneralsPopupView.h"
#include "ChangeNickNameView.h"
#include "SoundController.h"
#include "GeneralManager.h"
#include "ChatServiceCocos2dx.h"
#include "AchievementController.h"
#include "AchievementNewView.h"
#include "MedalNewView.h"
#include "ParticleController.h"
#include "ThroneTile.h"
#include "CheckKingdomView.h"
#include "ArmyController.h"
#include "TroopsController.h"
#include "FunBuildController.h"

#define cellNum1 80
#define cellNum2 15
#define picSize 100.0

RoleInfoView* RoleInfoView::create(PlayerInfo* info,int fromType, string achievePro, string medalCntStr,string medalIconId){
    RoleInfoView* ret = new RoleInfoView(info,fromType,achievePro,medalCntStr,medalIconId);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void RoleInfoView::createInfoByUid(const string &uid) {
    GeneralsPopupView::createInfoByUid(uid);
//    auto ret = new RoleInfoView(uid);
//    
//    auto cmd = new GetUserInfoCommand(uid);
//    cmd->setCallback(CCCallFuncO::create(ret, callfuncO_selector(RoleInfoView::onGetPlayerInfoCallback), nullptr));
//    cmd->sendAndRelease();
    
}

void RoleInfoView::onGetPlayerInfoCallback(cocos2d::CCObject *obj) {
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : parse error
        this->release();
        return;
    }
    
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    if (!info) {
        this->release();
        return;
    }
    
    auto playerInfo = new PlayerInfo();
    playerInfo->updateInfo(info);
    this->m_info = playerInfo;
    if(info->objectForKey("alliance")){
        auto dict = dynamic_cast<CCDictionary*>(info->objectForKey("alliance"));
        if(dict->objectForKey("alliance")){
            auto alliance = dynamic_cast<CCDictionary*>(dict->objectForKey("alliance"));
            if(alliance){
                playerInfo->setAllianceId(alliance->valueForKey("uid")->getCString());
            }
            playerInfo->allianceInfo.updateAllianceInfo(alliance);
            playerInfo->allianceInfo.serverId = playerInfo->selfServerId;
        }
    }
    this->m_medalIconId = info->valueForKey("medal")->getCString();
    float pro = info->valueForKey("achieveNum")->floatValue();
    this->m_achievePro = CCString::createWithFormat("%.f", pro*100)->getCString();
    CCArray* medalList = dynamic_cast<CCArray*>(info->objectForKey("medalList"));
    this->m_medalCntStr = "0";
    if (medalList)
    {
        int count = medalList->count();
        this->m_medalCntStr = CC_ITOA(count);
    }
    AchievementController::getInstance()->getOtherMedalInfo(medalList);
    
    this->init();
    
    string data = "action=openFriendRoleInfoView";
    GameController::getInstance()->callXCApi(data);
    if (m_medalIconId.length() > 0)
    {
        data = "action=displayMedal.";
        data.append(m_medalIconId);
        GameController::getInstance()->callXCApi(data);
    }
    
    PopupViewController::getInstance()->addPopupInView(this);
    
    this->release();
    playerInfo->release();
}


bool RoleInfoView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    CCLoadSprite::doResourceByCommonIndex(305, true);
    CCLoadSprite::doResourceByCommonIndex(503, true);
    CCLoadSprite::doResourceByCommonIndex(5, true);
    if(SceneController::getInstance()->currentSceneId != SCENE_ID_WORLD){
        CCLoadSprite::doResourceByWorldIndex(2, true);
    }
    CCLoadSprite::doResourceByCommonIndex(11, true);
    
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(7, false);
        CCLoadSprite::doResourceByCommonIndex(305, false);
        CCLoadSprite::doResourceByCommonIndex(503, false);
        CCLoadSprite::doResourceByCommonIndex(5, false);
        if(SceneController::getInstance()->currentSceneId != SCENE_ID_WORLD){
            CCLoadSprite::doResourceByWorldIndex(2, false);
        }
        CCLoadSprite::doResourceByCommonIndex(11, false);
        
    });
    
    auto tmpCCB = CCBLoadFile("RoleInfoView",this,this);
    if (CCCommonUtils::isIosAndroidPad())
    {
        tmpCCB->setPosition(0, 0);
    }
    this->setContentSize(tmpCCB->getContentSize());
    
//    int oldBgHeight = m_viewBg->getContentSize().height;
//    changeBGHeight(m_viewBg);
//    int newBgHeight = m_viewBg->getContentSize().height;
//    int addHeight = newBgHeight - oldBgHeight;
//    //test
//    for (int i=1; i<=3; i++) {
//        auto particle = ParticleController::createParticle(CCString::createWithFormat("Loading_%d",i)->getCString());
//        particle->setPosition(CCPoint(200, 0));
//        m_midNode->addChild(particle);
//    }
    GameController::getInstance()->showWaitInterface();
    
    int addHeight = this->getExtendHeight();
    if (CCCommonUtils::isIosAndroidPad())
    {
        addHeight = CCDirector::sharedDirector()->getWinSize().height - 2048;
    }
    auto tbg = CCLoadSprite::loadResource("technology_09.png");
    auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
    int maxHight = CCDirector::sharedDirector()->getWinSize().height;
    int width = CCDirector::sharedDirector()->getWinSize().width;
    int curHight = -maxHight/2.0f;
    while (curHight<maxHight) {
        auto bg = CCLoadSprite::createSprite("technology_09.png");
        bg->setAnchorPoint(ccp(0.5, 0));
        bg->setPosition(ccp(0, curHight));
        curHight += bg->getContentSize().height;
        bg->runAction(CCFadeIn::create(0.5));
        tBatchNode->addChild(bg);
    }
    if (CCCommonUtils::isIosAndroidPad()) {
        tBatchNode->setScaleX(1536 * 1.0 / 640);
    }
    m_viewNode->addChild(tBatchNode);
    m_viewNode->setPositionY(m_viewNode->getPositionY()-addHeight);
    int oldWidth = m_infoList->getContentSize().width;
    int oldHeight = m_infoList->getContentSize().height;
    m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
    m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
    
    medalId = GlobalData::shared()->medalid;
    int medalLevel = 0;
    if (AchievementController::getInstance()->m_myMedalInfos.find(medalId) != AchievementController::getInstance()->m_myMedalInfos.end())
    {
        medalLevel = AchievementController::getInstance()->m_myMedalInfos[medalId].level;
    }
    
    if(m_info->uid != GlobalData::shared()->playerInfo.uid)
    {
        medalId = m_medalIconId;
        if (AchievementController::getInstance()->m_otherMedalInfos.find(medalId) != AchievementController::getInstance()->m_otherMedalInfos.end())
        {
            medalLevel = AchievementController::getInstance()->m_otherMedalInfos[medalId].level;
        }
    }
    if (medalId.length()>0)
    {
        m_medalTitleTTF->setString(AchievementController::getInstance()->m_myMedalInfos[medalId].name);
    }
    medalIconName = AchievementController::getInstance()->getMedalIconName(medalId, 0);
    if(m_info->uid==GlobalData::shared()->playerInfo.uid){
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight + addHeight + 143));
        } else {
            m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight+70));
        }
    }
    
//    int add = medalIconName.length()==0?178:0;
    m_infoList->setContentSize(CCSizeMake(oldWidth, m_infoList->getContentSize().height));
    
    m_scrollView = CCScrollView::create(m_infoList->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_infoList->addChild(m_scrollView);
    
    int total = GlobalData::shared()->playerInfo.buildingPower+GlobalData::shared()->playerInfo.sciencePower+GlobalData::shared()->playerInfo.armyPower+GlobalData::shared()->playerInfo.fortPower+GlobalData::shared()->playerInfo.questpower+GlobalData::shared()->playerInfo.playerPower+GlobalData::shared()->playerInfo.heroPower + GlobalData::shared()->playerInfo.equipPower;
    m_cellHeight = 44;
    if (CCCommonUtils::isIosAndroidPad()) {
        m_cellHeight = 82.5;
    }
    int contentHeight = 22*m_cellHeight+8;
    
    std::string leagueShort = "";
    if(m_info->isInAlliance()){
        leagueShort = std::string("(") + m_info->allianceInfo.shortName + ")";
    }
    
    m_midNode->setPositionY(-125);
    if (CCCommonUtils::isIosAndroidPad()) {
        m_midNode->setPositionY(-135.2);
    }
    
    if(m_info->uid==GlobalData::shared()->playerInfo.uid){
        this->setTitleName(_lang("108742"));
//        m_midNode->setPositionY(-25);
        m_midNode->setPositionY(-125);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_midNode->setPositionY(-135.2);
        }
        m_otherNode->setVisible(false);
        if (GlobalData::shared()->isCrossService) {
            m_serverTxt->setString(_lang("108755") + CC_ITOA(GlobalData::shared()->playerInfo.crossFightSrcServerId));
        }else{
            m_serverTxt->setString(_lang("108755") + CC_ITOA(GlobalData::shared()->playerInfo.selfServerId));
        }
        string str = leagueShort;
        str.append(GlobalData::shared()->playerInfo.name);
        m_nameTxt->setString(str.c_str());
        float achieve = AchievementController::getInstance()->getAchieveProgress();
        m_achieveTTF->setString(_lang("137409") + CCString::createWithFormat("%.f", achieve*100)->getCString()+"%");
        m_medalTTF->setString(_lang("137410") + CC_ITOA(AchievementController::getInstance()->getMedalComCnt()));
        
        contentHeight = cellNum1*m_cellHeight+8;
        GetPowerInfoCommand* cmd = new GetPowerInfoCommand();
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(RoleInfoView::updatePlayerInfo), NULL));
        cmd->sendAndRelease();
        
    }else{
//        m_midNode->setPositionY(0);
        m_otherNode->setVisible(true);
        m_otherNode->setPositionY(m_otherNode->getPositionY());
        string str = leagueShort;
        str.append(m_info->name);
        m_nameTxt->setString(str.c_str());
        m_serverTxt->setString(_lang("108755") + CC_ITOA(m_info->selfServerId));
        m_achieveTTF->setString(_lang("137409") + m_achievePro + "%");
        m_medalTTF->setString(_lang("137410") + m_medalCntStr);
//        m_commonTTF->setString(_lang("108552"));
        
        contentHeight = cellNum2*m_cellHeight+8;
        total = m_info->power;
        //m_info->retain();
    }
    if (CCCommonUtils::isIosAndroidPad()) {
        
    }
    if (medalIconName.length() > 0)
    {
        if (CCLoadSprite::getSF(medalIconName.c_str()) == NULL)
        {
            medalIconName = "medal_1.png";
        }
        m_medalIcon->setDisplayFrame(CCLoadSprite::loadResource(medalIconName.c_str()));
        if (medalLevel > 5)
        {
            m_starNode->setVisible(false);
            m_speStarNode->setVisible(true);
            string s = "X";
            s.append(CC_ITOA(medalLevel));
            m_speStarTTF->setString(s);
        }
        else
        {
            m_starNode->setVisible(true);
            m_speStarNode->setVisible(false);
            CCSprite* starSp = NULL;
            string nameStr = "";
            for (int i = 0; i < 5; i++)
            {
                nameStr = (i<medalLevel)?"icon_xingxing01.png":"icon_xingxing02.png";
                starSp = (CCSprite*)m_starNode->getChildByTag(i);
                starSp->setDisplayFrame(CCLoadSprite::loadResource(nameStr.c_str()));
            }
        }
    }
    m_medalNode->setVisible(medalIconName.length()>0);//medalIconName.length()>0
    m_medalNode->setPositionY(-200);
//    addMedalParticle(medalId);
    
    m_nameSpr->setPositionX(m_nameTxt->getPositionX() + m_nameTxt->getContentSize().width * m_nameTxt->getScaleX() + 20);
    m_nameTouchNode->setContentSize(CCSize(m_nameTxt->getContentSize().width + m_nameSpr->getContentSize().width + 20, m_nameTxt->getContentSize().height));
    if(!CCCommonUtils::isServerCrossOpen()){
        m_serverTxt->setString("");
    }

    m_btnBattle->setVisible(false);//GlobalData::shared()->playerInfo.gmFlag==1
    
    if(m_info->uid!=GlobalData::shared()->playerInfo.uid){
        this->initPlayerStats();
    }

    m_scrollView->setContentSize(CCSize(640,contentHeight));
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_scrollView->setContentSize(CCSize(1536, contentHeight));
    }
    m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - contentHeight));
    
    std::string str = "";
    str.append(CC_CMDITOA(m_info->exp));
    str.append("/");
    str.append(CC_CMDITOA(m_info->maxExp));
//    m_expTxt->setString(str.c_str());
    int maxExp = m_info->maxExp;
    if(maxExp<=0) maxExp = 1;
    float len = m_info->exp*1.0/maxExp;
    if(len>1) len = 1;
//    m_expBar->setContentSize(CCSize(len*374,23));
    str = _lang("102203")+":";
    str.append(CC_ITOA(m_info->level));
    m_levelTxt->setString(str.c_str());
    auto pic = CCLoadSprite::createSprite(m_info->getPic().c_str());
    if (pic) {
        m_head->addChild(pic);
        pic->setPosition(ccp(2,-2));
        pic->setScale(picSize/pic->getContentSize().width);
    }
    m_headImgNode = HFHeadImgNode::create();
    if (m_info->isUseCustomPic())
    {
        m_headImgNode->initHeadImgUrl2(m_head, m_info->getCustomPicUrl(), 1.0f, picSize, true, ccp(1, -1));
    }
    
    str = _lang("102163")+": ";
    str.append(CC_CMDITOA(total));
    m_powerTxt->setString(str.c_str());
    
    str = _lang("105001")+": ";
    str.append(CC_CMDITOA(m_info->armyKill));
    m_killTxt->setString(str.c_str());
    
    //CCCommonUtils::setButtonTitle(m_heroBtn, _lang("101200").c_str());
    CCCommonUtils::setButtonTitle(m_btnAlliance, _lang("102161").c_str());
    CCCommonUtils::setButtonTitle(m_msgBtn, _lang("105308").c_str());

    
    m_btnAlliance->setEnabled(m_info->isInAlliance());
    if(!GlobalData::shared()->playerInfo.isInSelfServer()){
        m_btnAlliance->setEnabled(false);
    }else if(m_info->isInAlliance()){
        m_btnAlliance->setEnabled(true);
    }else{
        CCCommonUtils::setButtonTitle(m_btnAlliance, _lang("115178").c_str());
        if(GlobalData::shared()->playerInfo.isInAlliance() && GlobalData::shared()->playerInfo.allianceInfo.rank>=3){
            m_btnAlliance->setEnabled(true);
        }else{
            m_btnAlliance->setEnabled(false);
        }
    }
//    if (m_info->selfServerId != GlobalData::shared()->playerInfo.selfServerId) {
//        m_btnAlliance->setEnabled(false);
//    }
    if (!m_info->isInAlliance() && m_info->selfServerId != GlobalData::shared()->playerInfo.selfServerId) {
        m_btnAlliance->setEnabled(false);
    }
    if(m_info->uid!=GlobalData::shared()->playerInfo.uid){
        m_changPicSpr->setVisible(false);
        m_nameSpr->setVisible(false);
    } else {
        m_changPicSpr->setVisible(true);
        m_nameSpr->setVisible(true);
    }
    
    string isOfficer = m_info->officer;
    if(isOfficer!=""){//isOfficer!=""
        string o_icon = CCCommonUtils::getPropById(isOfficer,"icon");
        o_icon+=".png";
        CCCommonUtils::setButtonSprite(m_flgBtn, o_icon.c_str());
        m_flgNode->setVisible(true);
    }else{
        m_flgNode->setVisible(false);
    }
    
    //m_heroBtn->setVisible(false);
    //m_msgBtn->setEnabled(false);
    
//    m_data.push_back(1);
//    m_data.push_back(2);
//    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
//    m_tabView->setDirection(kCCScrollViewDirectionVertical);
//    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
//    m_tabView->setTouchPriority(Touch_Default);
//    m_infoList->addChild(m_tabView);
//    m_tabView->reloadData();
    
    return true;
}

void RoleInfoView::hideButtons() {
    m_btnAlliance->setVisible(false);
    m_msgBtn->setVisible(false);
}

void RoleInfoView::updatePlayerInfo(CCObject* obj){
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : parse error
        

    }else{
        auto info = dynamic_cast<CCDictionary*>(ret->getData());
        if (info) {
            m_info->updateInfo(info);
        }
    }

    int total = GlobalData::shared()->playerInfo.buildingPower+GlobalData::shared()->playerInfo.sciencePower+GlobalData::shared()->playerInfo.armyPower+GlobalData::shared()->playerInfo.fortPower+GlobalData::shared()->playerInfo.questpower+GlobalData::shared()->playerInfo.playerPower+GlobalData::shared()->playerInfo.heroPower + GlobalData::shared()->playerInfo.equipPower;
    std::string str = _lang("102163")+": ";
    str.append(CC_CMDITOA(total));
    m_powerTxt->setString(str.c_str());
    str = _lang("105001")+": ";
    str.append(CC_CMDITOA(m_info->armyKill));
    m_killTxt->setString(str.c_str());
    
    str = "";
    str.append(CC_CMDITOA(m_info->exp));
    str.append("/");
    str.append(CC_CMDITOA(m_info->maxExp));
//    m_expTxt->setString(str.c_str());
    int maxExp = m_info->maxExp;
    if(maxExp<=0) maxExp = 1;
    float len = m_info->exp*1.0/maxExp;
    if(len>1) len = 1;
//    m_expBar->setContentSize(CCSize(len*435,23));
//    m_expBar->setContentSize(CCSize(len*374,23));
    str = _lang("102203")+":";
    str.append(CC_ITOA(m_info->level));
    m_levelTxt->setString(str.c_str());
    if(m_info->uid!=GlobalData::shared()->playerInfo.uid){
        m_changPicSpr->setVisible(false);
        m_nameSpr->setVisible(false);
    } else {
        m_changPicSpr->setVisible(true);
        m_nameSpr->setVisible(true);
    }
    m_scrollView->getContainer()->removeAllChildrenWithCleanup(true);
    this->initPlayerStats();
}

void RoleInfoView::onChangePic(CCObject* p)
{
    auto pic = CCLoadSprite::createSprite(m_info->getPic().c_str());
    if (pic) {
        m_head->removeAllChildren();
        m_head->addChild(pic);
        pic->setPosition(ccp(2,-2));
        pic->setScale(picSize/pic->getContentSize().width);
    }
    if (m_info->isUseCustomPic())
    {
        m_headImgNode->initHeadImgUrl2(m_head, m_info->getCustomPicUrl(), 1.0f, picSize, true, ccp(2, -2));
    }
}

void RoleInfoView::initPlayerStats(){
//    return;
    
    
    count = 0;
    int cellNum = cellNum2-1;
    if(m_info->uid==GlobalData::shared()->playerInfo.uid){
        cellNum = cellNum1-1;//21;
    }
    int bar_dy=13;
    
    if (medalIconName.length()!=0) {
        m_medalNode->removeFromParent();
        m_medalNode->setPositionY(cellNum*m_cellHeight-40);
        m_scrollView->addChild(m_medalNode);
        addMedalParticle(medalId);
        count = 4;
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_medalNode->setPositionY(cellNum * m_cellHeight - 80);
            count = 5;
        }
    }
//**************************************************************************************
////    CCScale9Sprite* spr = CCLoadSprite::createScale9Sprite("UI_Layer_Red_BG.png");
////    spr->setContentSize(CCSizeMake(625, 44));
////    spr->setPosition(ccp(1, (cellNum-count)*m_cellHeight));
////    m_scrollView->addChild(spr);
////    
////    spr = CCLoadSprite::createScale9Sprite("yellow_line.png");
////    spr->setContentSize(CCSizeMake(625, 3));
////    spr->setPosition(ccp(1, (cellNum-count)*m_cellHeight+m_cellHeight));
////    m_scrollView->addChild(spr);
////    
////    spr = CCLoadSprite::createScale9Sprite("line_grey02.png");
////    spr->setContentSize(CCSizeMake(625, 5));
////    spr->setPosition(ccp(1, (cellNum-count)*m_cellHeight));
////    m_scrollView->addChild(spr);
//    CCSprite* spr = CCLoadSprite::createSprite("Alliance_title.png");
//    spr->setPosition(ccp(0, (cellNum-count)*m_cellHeight-bar_dy));
//    spr->setScaleY(0.8);
//    m_scrollView->addChild(spr);
//    
//    float width = CCDirector::sharedDirector()->getWinSize().width;
//    
//    CCLabelIF* lable = CCLabelIF::create(_lang("105010").c_str(), 24);
//    lable->setAnchorPoint(ccp(0.5, 0.5));
//    lable->setColor({255,177,89});
//    float lableWidth = lable->getContentSize().width*lable->getOriginScaleX();
//    lable->setPosition(ccp((width-lableWidth)/2.0f,(cellNum-count)*m_cellHeight+10));
//    m_scrollView->addChild(lable);
//    count += 1;
//   
//    RoleCell* cell = RoleCell::create(_lang("105011"),  CC_CMDITOA(m_info->battleWin), 0);
//    cell->m_delegate=this;
//    cell->setAnchorPoint(ccp(0, 0.5));
//    cell->setPosition(ccp(0,(cellNum-count)*m_cellHeight));
//    m_scrollView->addChild(cell);
//    count += 1;
//    
//    cell = RoleCell::create(_lang("105012"), CC_CMDITOA(m_info->battleLose), 1);
//    cell->m_delegate=this;
//    cell->setAnchorPoint(ccp(0, 0.5));
//    cell->setPosition(ccp(0,(cellNum-count)*m_cellHeight));
//    m_scrollView->addChild(cell);
//    count += 1;
//    
//    cell = RoleCell::create(_lang("105013"), CC_CMDITOA(m_info->attackWin), 0);
//    cell->m_delegate=this;
//    cell->setAnchorPoint(ccp(0, 0.5));
//    cell->setPosition(ccp(0,(cellNum-count)*m_cellHeight));
//    m_scrollView->addChild(cell);
//    count += 1;
//
//    cell = RoleCell::create(_lang("105014"),CC_CMDITOA(m_info->attackLose), 1);
//    cell->setAnchorPoint(ccp(0, 0.5));
//    cell->m_delegate=this;
//    cell->setPosition(ccp(0,(cellNum-count)*m_cellHeight));
//    m_scrollView->addChild(cell);
//    count += 1;
//    
//    cell = RoleCell::create(_lang("105015"), CC_CMDITOA(m_info->defenseWin), 0);
//    cell->m_delegate=this;
//    cell->setAnchorPoint(ccp(0, 0.5));
//    cell->setPosition(ccp(0,(cellNum-count)*m_cellHeight));
//    m_scrollView->addChild(cell);
//    count += 1;
//    
//    std::string str = "";
//    str.append(CC_CMDITOA(m_info->battleWin));
//    str.append("/");
//    str.append(CC_CMDITOA(m_info->battleLose));
//    
//    if(m_info->battleWin>=0&&m_info->battleLose==0){
//        str = _lang("105029");
//    }else{
//        float rate = m_info->battleWin*1.0/m_info->battleLose;
//        if(rate<0.01){
//            str = _lang("105030");
//        }else{
//             str = CCString::createWithFormat("%.2f", rate)->getCString();
//        }
//    }
//    cell = RoleCell::create(_lang("105016"), str, 1);
//    cell->m_delegate=this;
//    cell->setAnchorPoint(ccp(0, 0.5));
//    cell->setPosition(ccp(0,(cellNum-count)*m_cellHeight));
//    m_scrollView->addChild(cell);
//    count += 1;
//    
//    cell = RoleCell::create(_lang("105017"), CC_CMDITOA(m_info->detectNum), 0);
//    cell->m_delegate=this;
//    cell->setAnchorPoint(ccp(0, 0.5));
//    cell->setPosition(ccp(0,(cellNum-count)*m_cellHeight));
//    m_scrollView->addChild(cell);
//    count += 1;
//
//    cell = RoleCell::create(_lang("105018"), CC_CMDITOA(m_info->armyKill), 1);
//    cell->m_delegate=this;
//    cell->setAnchorPoint(ccp(0, 0.5));
//    cell->setPosition(ccp(0,(cellNum-count)*m_cellHeight));
//    m_scrollView->addChild(cell);
//    count += 1;
//    
//    cell = RoleCell::create(_lang("105019"), CC_CMDITOA(m_info->armyDead), 0);
//    cell->m_delegate=this;
//    cell->setAnchorPoint(ccp(0, 0.5));
//    cell->setPosition(ccp(0,(cellNum-count)*m_cellHeight));
//    m_scrollView->addChild(cell);
//    count += 1;
//    
//    cell = RoleCell::create(_lang("105020"), CC_CMDITOA(m_info->armyCure), 1);
//    cell->m_delegate=this;
//    cell->setAnchorPoint(ccp(0, 0.5));
//    cell->setPosition(ccp(0,(cellNum-count)*m_cellHeight));
//    m_scrollView->addChild(cell);
//    count += 1;
//    
////    cell = RoleCell::create(_lang("105021"), CC_CMDITOA(m_info->cityDes), 0);
////    cell->setAnchorPoint(ccp(0, 0.5));
////    cell->setPosition(ccp(0,(cellNum-count)*m_cellHeight));
////    m_scrollView->addChild(cell);
////    count += 1;
//    
//    if(m_info->uid!=GlobalData::shared()->playerInfo.uid){
//        return;
//    }
////    spr = CCLoadSprite::createScale9Sprite("UI_Layer_Red_BG.png");
////    spr->setContentSize(CCSizeMake(625, 44));
////    spr->setPosition(ccp(1, (cellNum-count)*m_cellHeight));
////    m_scrollView->addChild(spr);
////
////    spr = CCLoadSprite::createScale9Sprite("yellow_line.png");
////    spr->setContentSize(CCSizeMake(625, 5));
////    spr->setPosition(ccp(1, (cellNum-count)*m_cellHeight+32));
////    m_scrollView->addChild(spr);
////
////    spr = CCLoadSprite::createScale9Sprite("line_grey02.png");
////    spr->setContentSize(CCSizeMake(625, 5));
////    spr->setPosition(ccp(1, (cellNum-count)*m_cellHeight-3));
////    m_scrollView->addChild(spr);
//    CCSprite* spr1 = CCLoadSprite::createSprite("Alliance_title.png");
//    spr1->setPosition(ccp(0, (cellNum-count)*m_cellHeight-bar_dy));
//    spr1->setScaleY(0.8);
//    m_scrollView->addChild(spr1);
//
//    lable = CCLabelIF::create(_lang("107510").c_str(), 24);
//    lable->setColor({255,177,89});
//    lable->setAnchorPoint(ccp(0.5, 0.5));
//    lableWidth = lable->getContentSize().width*lable->getOriginScaleX();
//    lable->setPosition(ccp((width-lableWidth)/2.0f,(cellNum-count)*m_cellHeight+10));
//    m_scrollView->addChild(lable);
//    count += 1;
//
//    cell = RoleCell::create(_lang("105022"), CC_CMDITOA(m_info->playerPower), 0);
//    cell->m_delegate=this;
//    cell->setAnchorPoint(ccp(0, 0.5));
//    cell->setPosition(ccp(0,(cellNum-count)*m_cellHeight));
//    m_scrollView->addChild(cell);
//    count += 1;
//
////    cell = RoleCell::create(_lang("105023"), CC_CMDITOA(m_info->heroPower), 1);
////    cell->setAnchorPoint(ccp(0, 0.5));
////    cell->setPosition(ccp(0,(cellNum-count)*m_cellHeight));
////    m_scrollView->addChild(cell);
////    count += 1;
//    
//    cell = RoleCell::create(_lang("105024"), CC_CMDITOA(m_info->armyPower), 1);
//    cell->m_delegate=this;
//    cell->setAnchorPoint(ccp(0, 0.5));
//    cell->setPosition(ccp(0,(cellNum-count)*m_cellHeight));
//    m_scrollView->addChild(cell);
//    count += 1;
//
//    cell = RoleCell::create(_lang("105025"), CC_CMDITOA(m_info->buildingPower), 0);
//    cell->m_delegate=this;
//    cell->setAnchorPoint(ccp(0, 0.5));
//    cell->setPosition(ccp(0,(cellNum-count)*m_cellHeight));
//    m_scrollView->addChild(cell);
//    count += 1;
//    
//    cell = RoleCell::create(_lang("105026"), CC_CMDITOA(m_info->sciencePower), 1);
//    cell->m_delegate=this;
//    cell->setAnchorPoint(ccp(0, 0.5));
//    cell->setPosition(ccp(0,(cellNum-count)*m_cellHeight));
//    m_scrollView->addChild(cell);
//    count += 1;
//    
//    cell = RoleCell::create(_lang("105027"), CC_CMDITOA(m_info->fortPower), 0);
//    cell->m_delegate=this;
//    cell->setAnchorPoint(ccp(0, 0.5));
//    cell->setPosition(ccp(0,(cellNum-count)*m_cellHeight));
//    m_scrollView->addChild(cell);
//    count += 1;
//    
//    cell = RoleCell::create(_lang("105104"), CC_CMDITOA(m_info->equipPower), 1);
//    cell->m_delegate=this;
//    cell->setAnchorPoint(ccp(0, 0.5));
//    cell->setPosition(ccp(0, (cellNum - count) * m_cellHeight));
//    m_scrollView->addChild(cell);
//    count += 1;
//**************************************************************************************
    
    RoleCell* cell = NULL;
    int endNum=2;
    int area=2;
    if(m_info->uid==GlobalData::shared()->playerInfo.uid){
        endNum=6;
        area=1;
    }
    for(;area<=endNum;area++){
        vector<int> attVec = GeneralManager::getInstance()->GeneralAttAreaMap[area];
        for (int i=0; i<attVec.size(); i++) {
            AttributeInfo& aInfo = GeneralManager::getInstance()->GeneralAttMap[attVec[i]];
            if (i==0) {
                CCSprite* spr = CCLoadSprite::createSprite("Alliance_title.png");
                if (CCCommonUtils::isIosAndroidPad()) {
                    spr->setScaleY(1.5);
                    spr->setScaleX(1536 * 1.0 / spr->getContentSize().width);
                    bar_dy = spr->getContentSize().height * spr->getScaleY() - 82.5;
                    spr->setAnchorPoint(CCPointZero);
                } else {
                    spr->setScaleY(0.8);
                }
                spr->setPosition(ccp(0, (cellNum-count)*m_cellHeight-bar_dy));
                m_scrollView->addChild(spr);
                float width = CCDirector::sharedDirector()->getWinSize().width;
                CCLabelIF* lable = CCLabelIF::create(_lang(aInfo.name).c_str(), 24);
                if (CCCommonUtils::isIosAndroidPad()) {
                    lable->setFontSize(45);
                }
                lable->setAnchorPoint(ccp(0.5, 0.5));
                lable->setColor({255,177,89});
                float lableWidth = lable->getContentSize().width*lable->getOriginScaleX();
                lable->setPosition(ccp((width-lableWidth)/2.0f,(cellNum-count)*m_cellHeight+10));
                if (CCCommonUtils::isIosAndroidPad())
                {
                    lable->setPositionY((cellNum - count) * m_cellHeight + 10);
                }
                m_scrollView->addChild(lable);
                count += 1;
            }else{
                int value = m_info->getAttributeByKey(aInfo.effect);
                if (value == -1) {
//                    value = CCCommonUtils::getEffectValueByNum(atoi(aInfo.effect.c_str()));
                    
                    value=0;
                    vector<std::string> vector;
                    CCCommonUtils::splitString(aInfo.effect, "|", vector);
//                    if(vector.size()==1){
//                        value = CCCommonUtils::getEffectValueByNum(atoi(vector[0].c_str()));
//                    }else if(vector.size()==2){
//                        value = CCCommonUtils::getEffectValueByNum(atoi(vector[1].c_str()));
//                        string temp = vector[0];
//                        vector.clear();
//                        CCCommonUtils::splitString(temp, ";", vector);
//                        if(vector.size()>0){
//                            value += CCCommonUtils::getEffectValueByNum(atoi(vector[0].c_str()));
//                        }
//                    }
                    for(int j=0;j<vector.size();j++){
                        value += CCCommonUtils::getEffectValueByNum(atoi(vector[j].c_str()));
                    }
                    //针对伤兵容量
                    if (aInfo.effect=="73|57") {
                        int sum = ArmyController::getInstance()->getMaxNumByType(TREAT_ARMY);
//                        int sumOri = ArmyController::getInstance()->getMaxNumByType(TREAT_ARMY, true);
                        value = sum;// - sumOri;
                    }
                    //针对行军上限，出征士兵数量
                    if (aInfo.effect=="56") {
                        value = TroopsController::getInstance()->getMaxSoilder();
                    }
                    //针对行军部队总数
                    if (aInfo.effect=="55") {
                        value += 1;
                    }
                    //针对训练士兵数量
                    if (aInfo.effect=="131") {
                        int sum = ArmyController::getInstance()->getMaxNumByType(ARMY);
                        value =value+10+sum;
                    }
                    //城防值
                    if (aInfo.effect=="125") {
                        map<int, FunBuildInfo>::iterator it;
                        map<int, FunBuildInfo> *curBuildsInfo = FunBuildController::getInstance()->curBuildsInfo;
                        for(it=curBuildsInfo->begin(); it != curBuildsInfo->end(); ++it)
                        {
                            if ((it->second).type == FUN_BUILD_WALL) {
                                value += atoi( (it->second).para[2].c_str());
                                break;
                            }
                        }
                    }
                    //陷阱上限
                    if (aInfo.effect=="88") {
                        value = ArmyController::getInstance()->getMaxNumByType(FORT);
                    }
                    //训练士兵速度
                    if (aInfo.effect=="66") {
                        map<int, FunBuildInfo>::iterator it;
                        map<int, FunBuildInfo> *curBuildsInfo = FunBuildController::getInstance()->curBuildsInfo;
                        for(it=curBuildsInfo->begin(); it != curBuildsInfo->end(); ++it)
                        {
                            if ((it->second).type == FUN_BUILD_BARRACK) {
                                string resInfo = (it->second).information;
                                std::vector<std::string> lineItems;
                                CCCommonUtils::splitString(resInfo, "|", lineItems);
                                string lvLine = lineItems[(it->second).level];
                                std::vector<std::string> items;
                                CCCommonUtils::splitString(lvLine, ";", items);
                                if (items.size()==4) {
                                    string lvItem = items[2];
                                    std::vector<std::string> item;
                                    CCCommonUtils::splitString(lvItem, ",", item);
                                    if (item.size()==2) {
                                        value+=atoi(item[1].c_str());
                                    }
                                }
                            }
                        }
                    }
                    
                }
                string valStr=CC_CMDITOA(value);
                if (aInfo.show==0) {//area>=3 && aInfo.aid!=46369 && aInfo.aid!=46370 && aInfo.aid!=46422 && aInfo.aid!=46410
                    valStr+="%";
                }
                if (aInfo.buff==1){
                    valStr="-"+valStr;
                }else if (aInfo.buff==2){
                    valStr="+"+valStr;
                }
                
                int cell_type=i%2;
                cell = RoleCell::create(_lang(aInfo.name), valStr, cell_type);
                cell->m_des = _lang(aInfo.description);
                cell->m_delegate=this;
                cell->setAnchorPoint(ccp(0, 0.5));
                cell->setPosition(ccp(0, (cellNum - count) * m_cellHeight));
                if (CCCommonUtils::isIosAndroidPad()) {
                    cell->setPosition(ccp(0, (cellNum - count) * m_cellHeight));
                }
                m_scrollView->addChild(cell);
                count += 1;
            }
        }
    }
    GameController::getInstance()->removeWaitInterface();
}

bool RoleInfoView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void RoleInfoView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_medalNode->isVisible() && isTouchInside(m_medalIcon, pTouch))
    {
        if (ccpDistance(pTouch->getLocation(), pTouch->getStartLocation()) > 10)
        {
            return;
        }
        if (m_info->uid == GlobalData::shared()->playerInfo.uid)
        {
            PopupViewController::getInstance()->addPopupInView(AchievementNewView::create(1));
        }
        else
        {
            string data = "action=clickFriendMedal";
            GameController::getInstance()->callXCApi(data);
            PopupViewController::getInstance()->addPopupInView(MedalNewView::create());
        }
        return;
    }
    if(m_info->uid!=GlobalData::shared()->playerInfo.uid){
        return;
    }
    if (isTouchInside(m_nameTouchNode, pTouch) || isTouchInside(m_nameSpr, pTouch)) {
        int dx = pTouch->getLocation().x - pTouch->getStartLocation().x;
        int dy = pTouch->getLocation().y - pTouch->getStartLocation().y;
        if (fabs(dx) > 10 || fabs(dy) > 10) {
            return;
        }
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        PopupViewController::getInstance()->addPopupView(ChangeNickNameView::create("200021"));
    }
}

void RoleInfoView::onCloseClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->removePopupView(this);
}

void RoleInfoView::onHeroCkick(CCObject * pSender, Control::EventType pCCControlEvent){
//    CCArray* array = m_info->getGenerals();
//    if (array==NULL) {
//        array = CCArray::create();
//    }
//    PopupViewController::getInstance()->addPopupInView(GeneralBagPopUpView::create(array));
}

void RoleInfoView::onAllianceClick(CCObject * pSender, Control::EventType pCCControlEvent){
//    YesNoDialog::show(_lang("E100008").c_str(), [](){});
    if (GlobalData::shared()->playerInfo.isInAlliance() && m_info->allianceInfo.uid==GlobalData::shared()->playerInfo.allianceInfo.uid) {
        PopupViewController::getInstance()->addPopupInView(AllianceInfoView::create(&m_info->allianceInfo));
    }else if(GlobalData::shared()->playerInfo.isInSelfServer() && !m_info->isInAlliance() && GlobalData::shared()->playerInfo.isInAlliance() && GlobalData::shared()->playerInfo.allianceInfo.rank>=3){
        InvitesAllianceCommand* cmd = new InvitesAllianceCommand(m_info->uid);
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(RoleInfoView::inviteFunc), NULL));
        cmd->sendAndRelease();
    }else{
        PopupViewController::getInstance()->addPopupInView(CheckAllianceInfoView::create(&m_info->allianceInfo));
    }
}

void RoleInfoView::inviteFunc(CCObject* param){
    string msg = _lang_1("115182", m_info->name.c_str());
    msg.append("  (").append(_lang("115181")).append(")");
    ChatController::getInstance()->sendCountryChat(msg.c_str(), CHAT_ALLIANCE, 2);
    CCCommonUtils::flyHint("", "", _lang("115259"));
}

void RoleInfoView::onMessageClick(CCObject * pSender, Control::EventType pCCControlEvent){
    std::string userName = m_info->name;
    std::string uid = m_info->uid;
//    PopupViewController::getInstance()->removeAllPopupView();
    //PopupViewController::getInstance()->addPopupInView(MailWritePopUpView::create(userName, ""));
    MailController::getInstance()->openMailDialogViewFirst(userName, uid);
//    PopupViewController::getInstance()->removePopupView(this);
    
}

void RoleInfoView::onGiftClick(CCObject * pSender, Control::EventType pCCControlEvent){
    YesNoDialog::show(_lang("E100008").c_str(), [](){});
}
void RoleInfoView::onBattle(CCObject * pSender, Control::EventType pCCControlEvent){
    BattleManager::shared()->requestBattleInfo(1);
}

void RoleInfoView::onClickPicBtn(CCObject * pSender, Control::EventType pCCControlEvent){
    if(m_info->uid!=GlobalData::shared()->playerInfo.uid){
        return;
    }
    m_scrollView->setTouchEnabled(false);
    PopupViewController::getInstance()->addPopupView(ChangePicPopupView::create());
}

void RoleInfoView::onFlgBtnPressed(CCObject * pSender, CCControlEvent pCCControlEvent){
    CCLOGFUNC();
//    unsigned int index=WorldController::getIndexByPoint(CCPoint(600, 600));
//    auto &info = WorldController::getInstance()->getCityInfos()[index];
//    PopupViewController::getInstance()->addPopupView(WarBuildInfoPopUpView::create(info));
    //跳转到官职页面
    PopupViewController::getInstance()->addPopupInView(CheckKingdomView::create());
}
void RoleInfoView::showTipNode(string name,string des,CCTouch *pTouch){
    m_desName->setString(name);
    m_desLabel->setString(des);
    m_tipNode->setVisible(true);
    float tp=pTouch->getLocation().y;
    float top=m_tipNode->getPositionY();
    m_tipNode->setPositionY(tp-680);
    if (CCCommonUtils::isIosAndroidPad()) {
        m_tipNode->setPositionY(tp - 924);
    }
    CCLOGFUNC();
}
void RoleInfoView::hideTipNode(){
    m_tipNode->setVisible(false);
}

void RoleInfoView::updateInfo()
{
    
}

void RoleInfoView::updatePlayerName(CCObject* p){
    std::string name = GlobalData::shared()->playerInfo.name;
    m_nameTxt->setString(name.c_str());
    m_nameSpr->setPositionX(m_nameTxt->getPositionX() + m_nameTxt->getContentSize().width * m_nameTxt->getScaleX() + 20);
    m_nameTouchNode->setContentSize(CCSize(m_nameTxt->getContentSize().width + m_nameSpr->getContentSize().width + 20, m_nameTxt->getContentSize().height));
}

void RoleInfoView::enableScroll(CCObject* p){
    if (m_scrollView) {
        m_scrollView->setTouchEnabled(true);
    }
}

void RoleInfoView::addMedalParticle(string medalId)
{
    if (m_medalNode->isVisible())
    {
        m_particleNode->removeAllChildren();
        map<int, vector<float>> colorMap;
        for (int i=1; i<=2; i++) {
            colorMap.clear();
            AchievementController::getInstance()->getMedalIconColor(medalId, colorMap);
            auto particle = ParticleController::createParticle(CCString::createWithFormat("Achievement_%d",i)->getCString());
            particle->setAnchorPoint(ccp(0.5, 0.5));
            vector<float> v = colorMap[i];
            ccColor4F color = {v.at(0), v.at(1), v.at(2), v.at(3)};
            particle->setStartColor(color);
            color = {v.at(4), v.at(5), v.at(6), v.at(7)};
            particle->setEndColor(color);
            particle->setScale(1.2f);
            
            m_particleNode->addChild(particle);
        }
    }
}

void RoleInfoView::onEnter()
{
    CCNode::onEnter();
    CCLOG("RoleInfoView::onEnter()");
//#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
//    if(ChatServiceCocos2dx::enableNativeMail)
//    {
//        ChatServiceCocos2dx::m_curRoleInfoViewCount++;
//        CCLOG("RoleInfoView::onEnter() %d",ChatServiceCocos2dx::m_curRoleInfoViewCount);
//    }
//#endif
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(RoleInfoView::onChangePic), MSG_CHANGE_PLAYER_PIC, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(RoleInfoView::updatePlayerName), MSG_CHANGE_PLAYER_NAME, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(RoleInfoView::enableScroll), CHANGE_PIC_REMOVE, NULL);
    this->setTitleName(_lang("105000"));
    
    CCLoadSprite::doResourceByCommonIndex(503, true);
    if(m_info->uid == GlobalData::shared()->playerInfo.uid)
    {
        string medalId = GlobalData::shared()->medalid;
        if (medalId.length()>0)
        {
            MedalInfo* m = &AchievementController::getInstance()->m_myMedalInfos[medalId];
            m_medalTitleTTF->setString(m->name);
            string medalIconName = AchievementController::getInstance()->getMedalIconName(medalId, 0);
            if (CCLoadSprite::getSF(medalIconName.c_str()) == NULL)
            {
                medalIconName = "medal_1.png";
            }
            m_medalIcon->setDisplayFrame(CCLoadSprite::loadResource(medalIconName.c_str()));
            int medalLevel = m->level;
            if (medalLevel > 5)
            {
                m_starNode->setVisible(false);
                m_speStarNode->setVisible(true);
                string s = "X";
                s.append(CC_ITOA(medalLevel));
                m_speStarTTF->setString(s);
            }
            else
            {
                m_starNode->setVisible(true);
                m_speStarNode->setVisible(false);
                CCSprite* starSp = NULL;
                string nameStr = "";
                for (int i = 0; i < 5; i++)
                {
                    nameStr = (i<medalLevel)?"icon_xingxing01.png":"icon_xingxing02.png";
                    starSp = (CCSprite*)m_starNode->getChildByTag(i);
                    starSp->setDisplayFrame(CCLoadSprite::loadResource(nameStr.c_str()));
                }
            }
            addMedalParticle(medalId);
        }
    }
    m_tipNode->setVisible(false);
    onChangePic(NULL);
//    int cellNum = cellNum2-1;
//    if(m_info->uid==GlobalData::shared()->playerInfo.uid){
//        cellNum = cellNum1-1;//21;
//    }
//    int bar_dy=13;
//    RoleCell* cell = NULL;
//    int endNum=1;
//    if(m_info->uid==GlobalData::shared()->playerInfo.uid){
//        endNum=6;
//    }
//    for(int area=2;area<=endNum;area++){
//        vector<int> attVec = GeneralManager::getInstance()->GeneralAttAreaMap[area];
//        for (int i=0; i<attVec.size(); i++) {
//            AttributeInfo& aInfo = GeneralManager::getInstance()->GeneralAttMap[attVec[i]];
//            if (i==0) {
//                CCSprite* spr = CCLoadSprite::createSprite("Alliance_title.png");
//                spr->setPosition(ccp(0, (cellNum-count)*m_cellHeight-bar_dy));
//                spr->setScaleY(0.8);
//                m_scrollView->addChild(spr);
//                float width = CCDirector::sharedDirector()->getWinSize().width;
//                CCLabelIF* lable = CCLabelIF::create(_lang(aInfo.name).c_str(), 24);
//                lable->setAnchorPoint(ccp(0.5, 0.5));
//                lable->setColor({255,177,89});
//                float lableWidth = lable->getContentSize().width*lable->getOriginScaleX();
//                lable->setPosition(ccp((width-lableWidth)/2.0f,(cellNum-count)*m_cellHeight+10));
//                m_scrollView->addChild(lable);
//                count += 1;
//            }else{
//                int value = m_info->getAttributeByKey(aInfo.effect);
//                if (value == -1) {
//                    //                    value = CCCommonUtils::getEffectValueByNum(atoi(aInfo.effect.c_str()));
//                    
//                    vector<std::string> vector;
//                    CCCommonUtils::splitString(aInfo.effect, "|", vector);
//                    if(vector.size()==1){
//                        value = CCCommonUtils::getEffectValueByNum(atoi(vector[0].c_str()));
//                    }else if(vector.size()==2){
//                        value = CCCommonUtils::getEffectValueByNum(atoi(vector[1].c_str()));
//                        string temp = vector[0];
//                        vector.clear();
//                        CCCommonUtils::splitString(temp, ";", vector);
//                        if(vector.size()>0){
//                            value += CCCommonUtils::getEffectValueByNum(atoi(vector[0].c_str()));
//                        }
//                    }
//                }
//                string valStr=CC_CMDITOA(value);
//                if (aInfo.show==0) {//area>=3 && aInfo.aid!=46369 && aInfo.aid!=46370 && aInfo.aid!=46422 && aInfo.aid!=46410
//                    valStr+="%";
//                }
//                int cell_type=i%2;
//                cell = RoleCell::create(_lang(aInfo.name), valStr, cell_type);
//                cell->m_des = _lang(aInfo.description);
//                cell->m_delegate=this;
//                cell->setAnchorPoint(ccp(0, 0.5));
//                cell->setPosition(ccp(0, (cellNum - count) * m_cellHeight));
//                m_scrollView->addChild(cell);
//                count += 1;
//            }
//        }
//    }

}

void RoleInfoView::onExit()
{
    CCLOG("RoleInfoView::onExit()");
//#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
//    if(ChatServiceCocos2dx::enableNativeMail)
//    {
//        ChatServiceCocos2dx::m_curRoleInfoViewCount--;
//        CCLOG("RoleInfoView::onExit() %d",ChatServiceCocos2dx::m_curRoleInfoViewCount);
//    }
//#endif
    CCLoadSprite::doResourceByCommonIndex(503, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CHANGE_PLAYER_PIC);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CHANGE_PLAYER_NAME);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, CHANGE_PIC_REMOVE);
    setTouchEnabled(false);
    CCNode::onExit();
}

SEL_CCControlHandler RoleInfoView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAllianceClick", RoleInfoView::onAllianceClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMessageClick", RoleInfoView::onMessageClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBattle", RoleInfoView::onBattle);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickPicBtn", RoleInfoView::onClickPicBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFlgBtnPressed", RoleInfoView::onFlgBtnPressed);
    
    return NULL;
}

bool RoleInfoView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewNode", CCNode*, this->m_viewNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_head", CCNode*, this->m_head);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_otherNode", CCNode*, this->m_otherNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", Label*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerTxt", CCLabelIF*, this->m_powerTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_killTxt", CCLabelIF*, this->m_killTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_serverTxt", CCLabelIF*, this->m_serverTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_midNode", CCNode*, this->m_midNode);
    
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_expTxt", CCLabelIF*, this->m_expTxt);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_expBar", CCScale9Sprite*, this->m_expBar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_levelTxt", CCLabelIF*, this->m_levelTxt);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnAlliance", CCControlButton*, this->m_btnAlliance);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgBtn", CCControlButton*, this->m_msgBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_heroBtn", CCControlButton*, this->m_heroBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnBattle", CCControlButton*, this->m_btnBattle);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBtn", CCControlButton*, this->m_picBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTouchNode", CCNode*, this->m_nameTouchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameSpr", CCSprite*, this->m_nameSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_changPicSpr", CCSprite*, this->m_changPicSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_achieveTTF", CCLabelIF*, this->m_achieveTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_medalTTF", CCLabelIF*, this->m_medalTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_medalIcon", CCSprite*, this->m_medalIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_medalNode", CCNode*, this->m_medalNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_commonTTF", CCLabelIF*, this->m_commonTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_medalTitleTTF", CCLabelIF*, this->m_medalTitleTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleNode", CCNode*, this->m_particleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_starNode", CCNode*, this->m_starNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_speStarNode", CCNode*, this->m_speStarNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_speStarTTF", CCLabelIF*, this->m_speStarTTF);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_medalUpNode", CCNode*, this->m_medalUpNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flgNode", CCNode*, this->m_flgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flgBtn", CCControlButton*, this->m_flgBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipNode", CCNode*, this->m_tipNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desName", CCLabelIF*, this->m_desName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, this->m_desLabel);
    
    return false;
}

RoleInfoView::~RoleInfoView(){
    if(m_fromType==1){
       m_fromType = 0;
//       PopupViewController::getInstance()->addPopupView(ChatView::create()); 
    }
}

CCSize RoleInfoView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data.size()){
        return CCSizeZero;
    }
    
    int dataCnt = GeneralManager::getInstance()->GeneralAttAreaMap[m_data[idx]].size();
    int cellH = 120;//单个属性高度
    int numPerLine = 4;//每行个数
    int tmpHang = (dataCnt-1)/numPerLine;
    int tmpLast = (dataCnt-1)%numPerLine;
    if (tmpLast>0) {
        tmpHang += 1;
    }
    
    return CCSize(640, cellH*tmpHang+100);
}

CCSize RoleInfoView::cellSizeForTable(CCTableView *table)
{
    return CCSize(640, 360);
}

CCTableViewCell* RoleInfoView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data.size()){
        return NULL;
    }
    
    AttSiteCell* cell = (AttSiteCell*)table->dequeueCell();
    if(cell){
        cell->setData(m_data[idx], m_info);
    }else{
        cell = AttSiteCell::create(m_data[idx], m_info);
    }
    return cell;
}

ssize_t RoleInfoView::numberOfCellsInTableView(CCTableView *table)
{
    int num = ceil(m_data.size() / 1.0f);
    return num;
}

//////////class RoleCell
RoleCell* RoleCell::create(std::string name,std::string value,int type)
{
    auto ret = new RoleCell(name,value,type);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void RoleCell::onEnter() {
    CCNode::onEnter();
    setSwallowsTouches(false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void RoleCell::onExit() {
    setTouchEnabled(false);
    CCNode::onExit();
}

bool RoleCell::init()
{
    CCBLoadFile("RoleCell",this,this);
    m_nameTxt->setString(m_name.c_str());
    //处理爆框
    std::string defLan = LocalController::shared()->getLanguageFileName();
    if (defLan=="fr") {
        m_nameTxt->setMaxScaleXByWidth(310.0);
    }else if (defLan=="en") {
        m_nameTxt->setMaxScaleXByWidth(400.0);
    }else{
        m_nameTxt->setMaxScaleXByWidth(310.0);
    }
    m_valueTxt->setString(m_value.c_str());
    m_bg1->setVisible(m_type==0?false:true);
//    m_bg2->setVisible(m_type==0?false:true);
    m_bg2->setVisible(false);
//    m_tipNode->setVisible(false);
    return true;
}

bool RoleCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (isTouchInside(m_bg1, pTouch)){
//        string temp ="";
//        temp +=m_name;
//        temp +=" ";
//        temp +=m_value;
//        if (m_delegate&&m_des!="") {
//            m_delegate->showTipNode(temp, m_des, pTouch);
            m_isShowTip=true;
//        }
        CCLOGFUNC();
        m_touch=pTouch;
        this->scheduleOnce(schedule_selector(RoleCell::showTip), 0.1f);
        m_py=pTouch->getLocation().y;
    }
    return true;
}

void RoleCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    if (m_isShowTip) {
        int py=pTouch->getLocation().y;
        int dy=py>m_py?(py-m_py):(m_py-py);
//        CCLOG("%d",dy);
        if (!isTouchInside(m_bg1, pTouch)||dy>20){
            unschedule(schedule_selector(RoleCell::showTip));
            m_delegate->hideTipNode();
            m_isShowTip=false;
        }
    }
}

void RoleCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    unschedule(schedule_selector(RoleCell::showTip));
    m_delegate->hideTipNode();
    m_isShowTip=false;
}

void RoleCell::showTip(float t){
    string temp ="";
    temp +=m_name;
    temp +=" ";
    temp +=m_value;
    if (m_delegate&&m_des!="") {
        m_delegate->showTipNode(temp, m_des, m_touch);
        m_isShowTip=true;
    }
}

SEL_CCControlHandler RoleCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}

bool RoleCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_valueTxt", CCLabelIF*, this->m_valueTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg1", CCScale9Sprite*, this->m_bg1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg2", CCScale9Sprite*, this->m_bg2);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipNode", CCNode*, this->m_tipNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desName", CCLabelIF*, this->m_desName);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, this->m_desLabel);
    
    return false;
}

///////////class AttSiteCell
AttSiteCell* AttSiteCell::create(int area, PlayerInfo* info)
{
    auto ret = new AttSiteCell();
    if (ret && ret->init(area, info)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AttSiteCell::init(int area, PlayerInfo* info)
{
    CCBLoadFile("AttSiteCell",this,this);
    setData(area, info);
    return true;
}

void AttSiteCell::setData(int area, PlayerInfo* info)
{
    m_area = area;
    m_info = info;
    vector<int> attVec = GeneralManager::getInstance()->GeneralAttAreaMap[m_area];
    
    AttributeInfo& aInfo = GeneralManager::getInstance()->GeneralAttMap[attVec[0]];
    m_nameLabel->setString(_lang(aInfo.name));
    string iconPath = CCCommonUtils::getIcon(CC_ITOA(attVec[0]));
    auto icon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_DEFAULT);
    CCCommonUtils::setSpriteMaxSize(icon, 50, true);
    m_iconNode->addChild(icon);
    
    int cellW = 130;//单个属性宽度
    int cellH = 110;//单个属性高度
    int numPerLine = 4;//每行个数
    
    int maxH = 0;
    int tmpHang = (attVec.size()-1)/numPerLine;
    int tmpLast = (attVec.size()-1)%numPerLine;
    if (tmpLast>0) {
        tmpHang += 1;
    }
    
    if (tmpHang>1) {
        m_touchNode->setContentSize(CCSizeMake(600, 120+ cellH*(tmpHang-1)) );
        auto size = CCSizeMake(604, 180+ cellH*(tmpHang-1));
        m_itemBg->setContentSize( size );
        maxH += cellH*(tmpHang-1);
        
        m_mainNode->setPosition(ccp(0, cellH*(tmpHang-1)));
    }
    
    for (int i=1; i<attVec.size(); i++) {
        auto cell = AttributeCell::create(attVec[i], m_info);
        cell->setPosition(ccp( ((i-1)%numPerLine)*cellW , maxH - ((i-1)/numPerLine)*cellH ));
        m_touchNode->addChild(cell);
    }
}

void AttSiteCell::onEnter() {
    CCNode::onEnter();
   
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}
void AttSiteCell::onExit() {
    setTouchEnabled(false);
    CCNode::onExit();
}

SEL_CCControlHandler AttSiteCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}
bool AttSiteCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemBg", CCScale9Sprite*, this->m_itemBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    return false;
}

bool AttSiteCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_startPoint = pTouch->getStartLocation();
    if (isTouchInside(m_touchNode,pTouch)){
        return true;
    }
    return false;
}

void AttSiteCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint endPoint = pTouch->getLocation();
    if(fabs(endPoint.x-m_startPoint.x)>10||fabs(endPoint.y-m_startPoint.y)>10){
        return;
    }
    
    auto arr = m_touchNode->getChildren();
    for (auto child : arr)
    {
        AttributeCell* node = dynamic_cast<AttributeCell*>(child);
        if (node && isTouchInside(node, pTouch)) {
            PopupViewController::getInstance()->addPopupView(AttInfoView::create(node->m_attId));
        }
    }
//    for (int i=0; i<arr->count(); i++) {
//        AttributeCell* node = dynamic_cast<AttributeCell*>(arr->objectAtIndex(i));
//        if (node && isTouchInside(node, pTouch)) {
//            PopupViewController::getInstance()->addPopupView(AttInfoView::create(node->m_attId));
//        }
//    }
}

///////////class AttributeCell
AttributeCell* AttributeCell::create(int attId, PlayerInfo* info)
{
    auto ret = new AttributeCell();
    if (ret && ret->init(attId, info)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AttributeCell::init(int attId, PlayerInfo* info)
{
    m_attId = attId;
    m_info = info;
    auto ccb = CCBLoadFile("AttributeCell",this,this);
    setContentSize(ccb->getContentSize());
    
    AttributeInfo& aInfo = GeneralManager::getInstance()->GeneralAttMap[m_attId];
//    m_nameLabel->setString( _lang(aInfo.name) );
    
    string iconPath = CCCommonUtils::getIcon(CC_ITOA(m_attId));
    auto icon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_DEFAULT);
    CCCommonUtils::setSpriteMaxSize(icon, 60, true);
    m_iconNode->addChild(icon);
    
    int value = m_info->getAttributeByKey(aInfo.effect);
    if (value == -1) {
        value = CCCommonUtils::getEffectValueByNum(atoi(aInfo.effect.c_str()));
    }
    string valueStr = CC_ITOA(value);
    if (aInfo.show == 0) {
        valueStr = valueStr+"%";
    }
    m_valueLabel->setString(valueStr);
    
    return true;
}

void AttributeCell::onEnter() {
    CCNode::onEnter();
}
void AttributeCell::onExit() {
    CCNode::onExit();
}

SEL_CCControlHandler AttributeCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}
bool AttributeCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_valueLabel", CCLabelIF*, this->m_valueLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    return false;
}

//////////class AttInfoView
AttInfoView* AttInfoView::create(int attId)
{
    auto ret = new AttInfoView();
    if (ret && ret->init(attId)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AttInfoView::init(int attId)
{
    bool ret = true;
    CCLoadSprite::doResourceByCommonIndex(11, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
    });
    
    m_attId = attId;
    CCBLoadFile("AttInfoView",this,this);
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    setContentSize(size);
    
    AttributeInfo& aInfo = GeneralManager::getInstance()->GeneralAttMap[m_attId];
    m_nameLabel->setString( _lang(aInfo.name) );
    m_infoLabel->setString(_lang(aInfo.description));
    
    string iconPath = CCCommonUtils::getIcon(CC_ITOA(m_attId));
    auto icon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_DEFAULT);
    CCCommonUtils::setSpriteMaxSize(icon, 130, true);
    m_picNode->addChild(icon);
    
    return ret;
}

void AttInfoView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}
void AttInfoView::onExit()
{
    setTouchEnabled(false);
    CCNode::onExit();
}
bool AttInfoView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_touchNode,pTouch)){
        return false;
    }
    return true;
}
void AttInfoView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isTouchInside(m_touchNode,pTouch)){
        PopupViewController::getInstance()->removePopupView(this);
    }
}
SEL_CCControlHandler AttInfoView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}
bool AttInfoView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel", CCLabelIF*, m_infoLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    return false;
}