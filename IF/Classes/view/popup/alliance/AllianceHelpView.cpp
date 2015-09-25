//
//  AllianceHelpView.cpp
//  IF
//
//  Created by chenliang on 14-4-15.
//
//

#include "AllianceHelpView.h"
#include "SoundController.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "AllianceShowHelpCommand.h"
#include "AllianceRenderHelpCommand.h"
#include "AllianceHelpAllCommand.h"
#include "JoinAllianceView.h"
#include "UIComponent.h"
#include "AllianceManager.h"
#include "ParticleFireAni.h"

static const float FUNCTION_BUTTON_AREA_HEIGHT = 80;

static const CCSize CELL_SIZE(620, 170);
static const CCSize CELL_SIZE_HD(1450, 330);

bool AllianceHelpView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_background", CCScale9Sprite*, this->m_background);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipTxt", CCLabelIF*, this->m_tipTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpAll", CCControlButton*, this->m_helpAll);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnJoin", CCControlButton*, this->m_btnJoin);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noAllianceTip", CCLabelIF*, this->m_noAllianceTip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buttonNode", CCNode*, this->m_buttonNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noAllianeNode", CCNode*, this->m_noAllianeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode1", CCNode*, this->m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode2", CCNode*, this->m_fireNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    return true;
}

SEL_CCControlHandler AllianceHelpView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "openJoinAlliance", AllianceHelpView::openJoinAlliance);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickHellpAll", AllianceHelpView::onClickHellpAll);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHelpBtnClick", AllianceHelpView::onHelpBtnClick);
    return NULL;
}

AllianceHelpView::~AllianceHelpView()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_POPUP_REMOVE,CCString::create(CC_ITOA(PVT_alliance)));
}

void AllianceHelpView::scrollViewDidScroll(CCScrollView *view)
{
    float mindy = m_tableView->minContainerOffset().y;
    float dy = m_tableView->getContentOffset().y;
    if(dy<mindy){
        m_tableView->setContentOffset(ccp(0, mindy));
    }
}

void AllianceHelpView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
    m_cells.pushBack(cell);
}

CCSize AllianceHelpView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CELL_SIZE_HD;
    }
    return CELL_SIZE;
}

CCSize AllianceHelpView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CELL_SIZE_HD;
    }
    return CELL_SIZE;
}

CCTableViewCell* AllianceHelpView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    AllianceHelpViewCell* cell = NULL;
    
    if (m_cells.size() > 0 && 0)
    {
        cell = dynamic_cast<AllianceHelpViewCell*>(m_cells.at(0));
        m_cells.erase(0);
    }
    else
    {
        cell = AllianceHelpViewCell::create();
    }
    
    cell->bind(&m_allianceHelps[idx]);
    return cell;
}

ssize_t AllianceHelpView::numberOfCellsInTableView(CCTableView *table)
{
    return m_allianceHelps.size();
}

void AllianceHelpView::openJoinAlliance(CCObject * pSender, Control::EventType pCCControlEvent){
//    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    PopupViewController::getInstance()->removeAllPopupView();
    PopupViewController::getInstance()->addPopupInView(JoinAllianceView::create(11));
}

void AllianceHelpView::onHelpBtnClick(cocos2d::CCObject *pSender, CCControlEvent event)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    GlobalData::shared()->isBind = true;
#endif
    CCCommonUtils::showHelpShiftSingleFAQ("45206");
}

void AllianceHelpView::onClickHellpAll(CCObject * pSender, CCControlEvent pCCControlEvent){
    int num = m_allianceHelps.size();
    m_helpAll->setEnabled(num>0);
    if(num<=0){
        return ;
    }
//    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
//    AllianceHelpAllCommand* cmd = new AllianceHelpAllCommand();
//    cmd->sendAndRelease();
    std::vector<AllianceHelp> ::iterator it;
    if(num>0){
        for (it = m_allianceHelps.begin(); it!=m_allianceHelps.end(); it++) {
            if(it!=m_allianceHelps.end() && it->getStats()==1){
                m_allianceHelps.erase(it);
                it--;
            }
        }
    }
    m_tableView->reloadData();
    GlobalData::shared()->playerInfo.allianceInfo.helpcount = 0;
    AllianceManager::getInstance()->sendHelpAllToServer();
    m_tipTxt->setString(_lang("115091"));
    m_tipTxt->setVisible(m_allianceHelps.size()<=0);
    m_helpAll->setEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIACNE_HELP_NUM_CHANGE);
}


bool AllianceHelpView::init(){
    bool ret = false;
    if(PopupBaseView::init())
    {
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(8, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(8, false);
        });
        auto tmpCCB = CCBLoadFile("AllianceHelpView", this, this);
        if (CCCommonUtils::isIosAndroidPad()) {
            this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        }
        else
            this->setContentSize(tmpCCB->getContentSize());
        m_loadingIcon = NULL;
        
        if (!CCCommonUtils::isIosAndroidPad()) {
            int oldBgHeight = m_background->getContentSize().height;
            changeBGHeight(m_background);
            int newBgHeight = m_background->getContentSize().height;
            int addHeight = newBgHeight - oldBgHeight;
            int oldWidth = m_infoList->getContentSize().width;
            int oldHeight = m_infoList->getContentSize().height;
            m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
            m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
            m_buttonNode->setPositionY(m_buttonNode->getPositionY() - addHeight);
            m_bgNode->setPositionY(m_bgNode->getPositionY() - addHeight);
        }
        
        auto tbg = CCLoadSprite::createSprite("technology_09.png");
        auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
        int maxHeight = CCDirector::sharedDirector()->getWinSize().height;
        int curHeight = 0;
        while (curHeight < maxHeight) {
            auto bg = CCLoadSprite::createSprite("technology_09.png");
            bg->setAnchorPoint(ccp(0, 0));
            bg->setPosition(ccp(0, curHeight));
            if (CCCommonUtils::isIosAndroidPad()) {
                bg->setScaleX(2.4);
                bg->setScaleY(2.16);
            }
            tBatchNode->addChild(bg);
            curHeight += bg->getContentSize().height;
        }
        m_bgNode->addChild(tBatchNode);
        
        CCSize designSize = CCDirector::sharedDirector()->getWinSize();
        float scrollAreaHeight = m_infoList->getContentSize().height;
        
        m_tableView = CCTableView::create(this, m_infoList->getContentSize());
        m_tableView->setDirection(kCCScrollViewDirectionVertical);
        m_tableView->setAnchorPoint(CCPoint(0,0));
        //m_tableView->setPosition(CCPoint(10, 100));
        
        if (CCCommonUtils::isIosAndroidPad()) {
            CCSize contentHeight = CCSize(designSize.width, CELL_SIZE_HD.height*m_allianceHelps.size());
            m_tableView->setContentOffset(CCPoint(0, scrollAreaHeight - contentHeight.height));
            m_tableView->setContentSize(contentHeight);
        }
        else {
            CCSize contentHeight = CCSize(designSize.width, CELL_SIZE.height*m_allianceHelps.size());
            m_tableView->setContentOffset(CCPoint(0, scrollAreaHeight - contentHeight.height));
            m_tableView->setContentSize(contentHeight);
        }
        
        m_tableView->setDelegate(this);
        
        m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_infoList->addChild(m_tableView);
        
        
        CCCommonUtils::setButtonTitle(m_helpAll, _lang("115079").c_str());
        m_tipTxt->setString(_lang("115091"));
        m_tipTxt->setVisible(false);
        if(GlobalData::shared()->playerInfo.isInAlliance()){
            m_noAllianeNode->setVisible(false);
            m_buttonNode->setVisible(true);
            
            AllianceShowHelpCommand* cmd = new AllianceShowHelpCommand();
            cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceHelpView::updateData), NULL));
            cmd->sendAndRelease();
            this->addLoadingAni();
        }else{
            m_buttonNode->setVisible(false);
            m_noAllianeNode->setVisible(true);
            m_noAllianceTip->setString(_lang("115096"));
            CCCommonUtils::setButtonTitle(m_btnJoin, _lang("115068").c_str());
        }
        m_helpAll->setEnabled(false);
        
        ParticleFireAni* par = ParticleFireAni::create();
        m_fireNode1->addChild(par);
        
        ParticleFireAni* par2 = ParticleFireAni::create();
        m_fireNode2->addChild(par2);
        ret = true;
    }
    return ret;
    
}

void AllianceHelpView::addLoadingAni(){
    m_loadingIcon = CCLoadSprite::createSprite("loading_1.png");
    m_loadingIcon->setAnchorPoint(ccp(0.5, 0.5));
    auto cSize = m_infoList->getContentSize();
    m_loadingIcon->setPosition(ccp(cSize.width/2,cSize.height/2));
    
    CCActionInterval * rotateto1 = CCRotateTo::create(0.5, 720);
    CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
    CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
    m_loadingIcon->runAction(rotateForever);
    m_infoList->addChild(m_loadingIcon,1000000);
}

void AllianceHelpView::updateData(CCObject* param){
    if(m_loadingIcon!=NULL){
        m_loadingIcon->removeFromParent();
    }
    if(!GlobalData::shared()->playerInfo.isInAlliance()) return ;
    m_allianceHelps.clear();
    NetResult* result = dynamic_cast<NetResult*>(param);
    CCArray* members = dynamic_cast<CCArray*>(result->getData());
    int helpCount = 0;
    if(members)
    {
        for (int i=0; i < members->count(); i++) {
            CCDictionary* member = (CCDictionary*)members->objectAtIndex(i);
            string name = member->valueForKey("name")->getCString();
            string itemId = member->valueForKey("itemId")->getCString();
            string allianceId = member->valueForKey("allianceId")->getCString();
            int currentHelp = member->valueForKey("nowcount")->intValue();
            int amount = member->valueForKey("maxcount")->intValue();
            bool isValid = member->valueForKey("isValid")->boolValue();
            int level = member->valueForKey("level")->intValue();
            int type = member->valueForKey("qType")->intValue();
            int stats = member->valueForKey("stats")->intValue();
            std::string helpId = member->valueForKey("helpId")->getCString();
            std::string senderId = member->valueForKey("senderId")->getCString();
            std::string pic = member->valueForKey("pic")->getCString();
            int picVer = member->valueForKey("picVer")->intValue();
            
            std::string buildingName = CCCommonUtils::getNameById(itemId);
            
            AllianceHelp helpInfo;
            helpInfo.setName(name);
            helpInfo.setIsValid(isValid);
            helpInfo.setCurrentHelp(currentHelp);
            helpInfo.setAmount(amount);
            helpInfo.setAllianceHelpType(type);
            helpInfo.setLevel(level);
            helpInfo.setBuildingName(buildingName);
            helpInfo.setSenderId(senderId);
            helpInfo.setAllianceId(allianceId);//
            helpInfo.setStats(stats);
            helpInfo.setHelpId(helpId);
            helpInfo.setPic(pic);
            helpInfo.setPicVer(picVer);
            if(stats==1){//可以帮别人的
                helpCount += 1;
                m_allianceHelps.push_back(helpInfo);
            }else{
                m_allianceHelps.insert(m_allianceHelps.begin(), helpInfo);
            }
        }
    }
    GlobalData::shared()->playerInfo.allianceInfo.helpcount = helpCount;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIACNE_HELP_NUM_CHANGE);
    m_tableView->reloadData();
    m_tipTxt->setVisible(m_allianceHelps.size()<=0);
    m_helpAll->setEnabled(helpCount>0);
}

void AllianceHelpView::addOneHelp(CCObject* param){
    CCDictionary* member = _dict(param);
    if(member==NULL) return ;
    string name = member->valueForKey("name")->getCString();
    string itemId = member->valueForKey("itemId")->getCString();
    string allianceId = member->valueForKey("allianceId")->getCString();
    int currentHelp = member->valueForKey("nowcount")->intValue();
    int amount = member->valueForKey("maxcount")->intValue();
    bool isValid = member->valueForKey("isValid")->boolValue();
    int level = member->valueForKey("level")->intValue();
    int type = member->valueForKey("qType")->intValue();
    std::string helpId = member->valueForKey("helpId")->getCString();
    std::string senderId = member->valueForKey("senderId")->getCString();
    std::string pic = member->valueForKey("pic")->getCString();
    int picVer = member->valueForKey("picVer")->intValue();
    int stats = 0;
    if(senderId!=GlobalData::shared()->playerInfo.uid){
        stats = 1;
    }else{
        stats = 0;
    }
    std::string buildingName = CCCommonUtils::getNameById(itemId);
    AllianceHelp helpInfo;;
    helpInfo.setName(name);
    helpInfo.setIsValid(isValid);
    helpInfo.setCurrentHelp(currentHelp);
    helpInfo.setAmount(amount);
    helpInfo.setAllianceHelpType(type);
    helpInfo.setLevel(level);
    helpInfo.setBuildingName(buildingName);
    helpInfo.setSenderId(senderId);
    helpInfo.setAllianceId(allianceId);//
    helpInfo.setStats(stats);
    helpInfo.setHelpId(helpId);
    helpInfo.setPic(pic);
    helpInfo.setPicVer(picVer);
    if(stats==1){
        m_allianceHelps.push_back(helpInfo);
    }else{
        m_allianceHelps.insert(m_allianceHelps.begin(), helpInfo);
    }
    m_tableView->reloadData();
    m_tipTxt->setVisible(m_allianceHelps.size()<=0);
    int num = m_allianceHelps.size();
    int helpCount = 0;
    for(int i=0;i<num;i++){
        AllianceHelp help = m_allianceHelps[i];
        if(help.getStats()==1){
            helpCount += 1;
        }
    }
    m_helpAll->setEnabled(helpCount>0);
}

void AllianceHelpView::removeOneHelp(CCObject* param){
    CCString* strHelpId = dynamic_cast<CCString*>(param);
    if(strHelpId==NULL) return ;
    std::string helpId = strHelpId->getCString();
    std::vector<AllianceHelp> ::iterator it;
    for (it = m_allianceHelps.begin(); it!=m_allianceHelps.end(); it++) {
        if(it->getHelpId()==helpId && it->getStats()==1){
            m_allianceHelps.erase(it);
            break;
        }
    }
    m_tableView->reloadData();
    m_tipTxt->setVisible(m_allianceHelps.size()<=0);
    
    int count = m_allianceHelps.size();
    int helpCount = 0;
    for(int i=0;i<count;i++){
        AllianceHelp help = m_allianceHelps[i];
        if(help.getStats()==1){
            helpCount += 1;
        }
    }
    m_helpAll->setEnabled(helpCount>0);
}

void AllianceHelpView::onEnter(){
    UIComponent::getInstance()->showPopupView(1);
    CCNode::onEnter();
//    UIComponent::getInstance()->showCloseBtn(true);
//    this->setTitleName(_lang("115077"));
    m_titleTxt->setString(_lang("115077"));
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceHelpView::removeOneHelp), MSG_UPDATE_ALLIANCE_HELP_DATA, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceHelpView::addOneHelp), MSG_ALLIACNE_NEW_HELP, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceHelpView::updateHelpData), MSG_ALLIACNE_HELP_ONE_NUM, NULL);
}

void AllianceHelpView::onExit(){
//    UIComponent::getInstance()->showCloseBtn(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_UPDATE_ALLIANCE_HELP_DATA);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ALLIACNE_NEW_HELP);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ALLIACNE_HELP_ONE_NUM);
    CCNode::onExit();
}

void AllianceHelpView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    
}

void AllianceHelpView::updateHelpData(CCObject* param){
    auto dic = _dict(param);
    if(dic && dic->objectForKey("helpId")){
        std::string helpId = dic->valueForKey("helpId")->getCString();
        std::vector<AllianceHelp> ::iterator it;
        for (it = m_allianceHelps.begin(); it!=m_allianceHelps.end(); it++) {
            if(it->getHelpId()==helpId){
                int current = dic->valueForKey("nowCount")->intValue();
                if(it->getAmount()>current){
                    it->setCurrentHelp(current);
                }else if(it->getStats()==1){
                    m_allianceHelps.erase(it);//不是自己的，当前值等于最大值时删除
                }
                break;
            }
        }
        m_tableView->reloadData();
        m_tipTxt->setVisible(m_allianceHelps.size()<=0);
        int count = m_allianceHelps.size();
        int helpCount = 0;
        for(int i=0;i<count;i++){
            AllianceHelp help = m_allianceHelps[i];
            if(help.getStats()==1){
                helpCount += 1;
            }
        }
        m_helpAll->setEnabled(helpCount>0);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIACNE_HELP_NUM_CHANGE);
    }
}

bool AllianceHelpViewCell::init()
{
    bool ret = true;
    
    if (CCTableViewCell::init()) {
        CCNode* node = CCBLoadFile("AllianceHelpViewCell", this, this);
        CCSize size = node->getContentSize();
        setContentSize(size);
        
        m_bgNode->removeAllChildrenWithCleanup(true);
        auto tbg = CCLoadSprite::loadResource("cell_bg1.png");
        auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
        CCSprite* spr = nullptr;
        spr = CCLoadSprite::createSprite("cell_bg1.png");
        spr->setAnchorPoint(ccp(0, 0));
        spr->setPosition(ccp(0, 0));
        if (CCCommonUtils::isIosAndroidPad())
        {
            spr->setScaleX(2);
            spr->setScaleY(1.9);
        }
        tBatchNode->addChild(spr);
        CCSize size1 = spr->getContentSize();
        int wi = size1.width;
        while (wi < size.width - size1.width) {
            spr = CCLoadSprite::createSprite("cell_bg2.png");
            spr->setAnchorPoint(ccp(0, 0));
            spr->setPosition(CCSize(wi, 0));
            if (CCCommonUtils::isIosAndroidPad()) {
                spr->setScaleX(2);
                spr->setScaleY(1.9);
            }
            tBatchNode->addChild(spr);
            wi += spr->getContentSize().width;
        }
        spr = CCLoadSprite::createSprite("cell_bg1.png");
        spr->setAnchorPoint(ccp(1, 0));
        spr->setPosition(ccp(size.width, 0));
        if (CCCommonUtils::isIosAndroidPad()) {
            spr->setScaleX(2);
            spr->setScaleY(1.9);
        }
        spr->setFlipX(true);
        tBatchNode->addChild(spr);
        m_bgNode->addChild(tBatchNode);
        
        m_headImgNode = HFHeadImgNode::create();
        CCCommonUtils::setButtonTitle(m_helpBtn, _lang("115078").c_str());
        return true;
    }
    
    return ret;
}


void AllianceHelpViewCell::onHelpClick(CCObject *pSender, CCControlEvent event)
{
    std::string helperName = GlobalData::shared()->playerInfo.name;
    
    AllianceRenderHelpCommand* cmd = new AllianceRenderHelpCommand(m_allianceHelp->getHelpId());
    cmd->sendAndRelease();
    m_helpBtn->setEnabled(false);
    GlobalData::shared()->playerInfo.allianceInfo.helpcount = GlobalData::shared()->playerInfo.allianceInfo.helpcount-1;
    if(GlobalData::shared()->playerInfo.allianceInfo.helpcount<=0){
        GlobalData::shared()->playerInfo.allianceInfo.helpcount = 0;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIACNE_HELP_NUM_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_UPDATE_ALLIANCE_HELP_DATA,CCString::create(m_allianceHelp->getHelpId()));
}

void AllianceHelpViewCell::helpSuccess(){
}

void AllianceHelpViewCell::bind(AllianceHelp* allianceHelp)
{
    setAllianceHelp(allianceHelp);
    if (allianceHelp->getAllianceHelpType() == TYPE_BUILDING) {
        m_descTxt->setString(_lang_2("115082", CC_ITOA(allianceHelp->getLevel()),allianceHelp->getBuildingName().c_str()));
    }
    else if (allianceHelp->getAllianceHelpType() == TYPE_SCIENCE) {
        m_descTxt->setString(_lang_1("115080", allianceHelp->getBuildingName().c_str()));
    }
    else if (allianceHelp->getAllianceHelpType() == TYPE_FORGE) {
        m_descTxt->setString(_lang_1("115080", allianceHelp->getBuildingName().c_str()));
    }
    else if (allianceHelp->getAllianceHelpType() == TYPE_HOSPITAL) {
        m_descTxt->setString(_lang("115081"));
    }else{
        m_descTxt->setString(CCString::createWithFormat("type%d",allianceHelp->getAllianceHelpType())->getCString());
    }
    m_descTxt->setMaxScaleXByWidth(380);
    m_picNode->removeAllChildrenWithCleanup(true);
    CCSprite* portrait = NULL;
    if(allianceHelp->getPic()==""||allianceHelp->getPic()=="0"){
        portrait = CCLoadSprite::createSprite("g044.png");
    }else{
        std::string pic = allianceHelp->getPic();
        pic.append(".png");
        portrait = CCLoadSprite::createSprite(pic.c_str());
    }
    if (CCCommonUtils::isIosAndroidPad()) {
        CCCommonUtils::setSpriteMaxSize(portrait, 200);
    }
    else
        CCCommonUtils::setSpriteMaxSize(portrait, 95);
    m_picNode->addChild(portrait);
    if (CCCommonUtils::isUseCustomPic(allianceHelp->getPicVer())) {
        if (CCCommonUtils::isIosAndroidPad()) {
            m_headImgNode->initHeadImgUrl2(m_picNode, CCCommonUtils::getCustomPicUrl(allianceHelp->getSenderId(), allianceHelp->getPicVer()), 1.0f, 200, true);
        }
        else
            m_headImgNode->initHeadImgUrl2(m_picNode, CCCommonUtils::getCustomPicUrl(allianceHelp->getSenderId(), allianceHelp->getPicVer()), 1.0f, 95, true);
    }
    m_nameLabel->setString(allianceHelp->getName().c_str());
    
    std::string str = _lang("115078");
    str.append(" ");
    str.append(CC_ITOA(allianceHelp->getCurrentHelp()));
    str.append("/");
    str.append(CC_ITOA(allianceHelp->getAmount()));
    m_progressTxt->setString(str);
    float len = allianceHelp->getCurrentHelp()/(allianceHelp->getAmount()*1.0);
    len = MIN(len,1);
    len = MAX(len,0);
    m_progress->setPreferredSize(CCSize(286*len,23));
    if(allianceHelp->getStats()==1){
        m_helpBtn->setVisible(true);
    }else{
        m_helpBtn->setVisible(false);
    }
}

void AllianceHelpViewCell::onEnter(){
    CCNode::onEnter();
}

void AllianceHelpViewCell::onExit(){
    CCNode::onExit();
}

SEL_CCControlHandler AllianceHelpViewCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHelpClick", AllianceHelpViewCell::onHelpClick);
    return NULL;
}

bool AllianceHelpViewCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIFTTF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_descTxt", CCLabelIF*, this->m_descTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progressTxt", CCLabelIF*, this->m_progressTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconPlaceHolder", CCNode*, this->m_iconPlaceHolder);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, this->m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpBtn", CCControlButton*, this->m_helpBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progress", CCScale9Sprite*, this->m_progress);
    return false;
}

