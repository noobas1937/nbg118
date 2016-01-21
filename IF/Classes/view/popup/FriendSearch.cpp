//
//  FriendSearch.cpp
//  IF
//
//  Created by xxrdsg on 15-2-4.
//
//

#include "FriendSearch.h"
#include "SoundController.h"
#include "../../Ext/CCDevice.h"
#include "PopupViewController.h"
#include "AllianceFlagPar.h"
#include "SoundController.h"
#include "CheckAllianceInfoView.h"
#include "SearchCommand.h"
#include "RoleInfoView.h"
#include "MailController.h"
#include "AllianceManager.h"
#include "ApplyAllianceCommand.h"
#include "AllianceCommentCheck.h"
#include "CancelAllianceApplyCommand.h"
#include "ChatView.h"
#include "GameController.h"
#include "ParticleFireAni.h"
#include "AllianceInfoView.h"
#include "ChatController.h"
#include "ParticleController.h"
#include "AllianceInfoMembersView.h"
#include "YesNoDialog.h"
#include "GetAllianceMembersCommand.h"
//#include "ThreeDTouchController.h" simon
#include "UIComponent.h"

#define FriendSearchView_Collor {80,56,25}
#define FriendSearchView_BtnCollor {255,248,198}

FriendSearchView* FriendSearchView::create()
{
    FriendSearchView* ret = new FriendSearchView();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool FriendSearchView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(6, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    CCLoadSprite::doResourceByCommonIndex(205, true);
    CCLoadSprite::doResourceByCommonIndex(307, true);
    CCLoadSprite::doResourceByCommonIndex(513, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(6, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
        CCLoadSprite::doResourceByCommonIndex(205, false);
        CCLoadSprite::doResourceByCommonIndex(307, false);
        CCLoadSprite::doResourceByCommonIndex(513, false);
    });
    
    auto tmpCCB = CCBLoadFile("FriendSearchView", this, this);
    this->setContentSize(tmpCCB->getContentSize());
    int dh = CCDirector::sharedDirector()->getWinSize().height - 852;
    if (CCCommonUtils::isIosAndroidPad())
    {
        dh = CCDirector::sharedDirector()->getWinSize().height - 2048;
    }
    m_listNode->setPositionY(m_listNode->getPositionY() - dh);
    m_listNode->setContentSize(CCSize(m_listNode->getContentSize().width, m_listNode->getContentSize().height + dh));
    m_bgNode->setPositionY(m_bgNode->getPositionY() - dh);
//    auto frame = CCLoadSprite::loadResource("Mail_diban.png");//technology_09
//    auto tBatchNode = CCSpriteBatchNode::createWithTexture(frame->getTexture());
//    int height = 0;
//    float theight = CCDirector::sharedDirector()->getWinSize().height;
//    if (CCCommonUtils::isIosAndroidPad()) {
//        theight = theight / 2.4;
//    }
//    while (height < theight)
//    {
//        CCSprite* spr = CCLoadSprite::createSprite("Mail_diban.png");//FriendsList-bg
//        spr->setAnchorPoint(ccp(1, 0));
//        spr->setScaleX(1.03);
//        spr->setPositionX(0);
//        spr->setPositionY(height);
//        tBatchNode->addChild(spr);
//        
//        spr = CCLoadSprite::createSprite("Mail_diban.png");
//        spr->setAnchorPoint(ccp(0, 0));
//        spr->setFlipX(true);
//        spr->setScaleX(1.03);
//        spr->setPositionY(height);
//        spr->setPositionX(0);
//        tBatchNode->addChild(spr);
//        
//        height += spr->getContentSize().height * spr->getScaleY();
//    }
//    m_bgNode->addChild(tBatchNode);
//    tBatchNode->setPositionY(tBatchNode->getPositionY() - dh);
    
    auto spriteText = CCLoadSprite::createScale9Sprite("Commond6_Search_frame.png");
    m_editBox = CCEditBox::create(CCSizeMake(610, 44), spriteText);

    m_editBox->setMaxLength(100);
    m_editBox->setFontSize(24);
    m_editBox->setFontColor(ccBLACK);
    m_editBox->setReturnType(kKeyboardReturnTypeDone);
    m_editBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
    m_editBox->setText("");
    m_editBox->setPlaceHolder("");
    m_editBox->setDelegate(this);
    m_editNode->addChild(m_editBox);
    
    m_tableView = CCTableView::create(this, m_listNode->getContentSize());
    m_tableView->setDirection(kCCScrollViewDirectionVertical);
    m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tableView->setDelegate(this);
    m_tableView->setTouchPriority(1);
    m_listNode->addChild(m_tableView);
    
    m_loadingIcon = NULL;
    CCCommonUtils::setButtonTitle(m_searchBtn, _lang("115013").c_str());
    m_playerData.clear();
    m_waitInterface = NULL;
    m_playerNumPerPage = 0;
    m_maxPlayerNum = 0;
    
    m_searchShowTxt->setString(_lang("115013"));
    
    //获取联盟成员信息
    string allianceId = GlobalData::shared()->playerInfo.allianceInfo.uid;
    if(allianceId!=""){
        GetAllianceMembersCommand * command = new GetAllianceMembersCommand();
        command->putParam("allianceId", CCString::create(allianceId));
        command->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(FriendSearchView::onGetAllianceMembers), NULL));
        command->setFailCallback(CCCallFuncO::create(this, callfuncO_selector(FriendSearchView::removeLoadingAni), NULL));
        command->sendAndRelease();
        this->addLoadingAni();
    }
    return true;
}

void FriendSearchView::editBoxReturn(EditBox* editBox){
    CCLOGFUNC();
    onClickSearchBtn(NULL,CCControlEvent::TOUCH_DOWN);
}

void FriendSearchView::onGetAllianceMembers(CCObject *data){
    this->removeLoadingAni();
    m_playerData.clear();
    NetResult* result = dynamic_cast<NetResult*>(data);
    CCDictionary* params = _dict(result->getData());

    CCArray* members = dynamic_cast<CCArray*>(params->objectForKey("list"));
    if(members)
    {
        for (int i=0; i < members->count(); i++) {
            CCDictionary* item = _dict(members->objectAtIndex(i));
            if (item->objectForKey("uid")) {
                string uid = item->valueForKey("uid")->getCString();
                if (uid==GlobalData::shared()->playerInfo.uid) {
                    continue;
                }
            }
            FriendSearchInfo info = FriendSearchInfo(item,0);
            m_tempData.push_back(info);
        }
    }
    cellStateMap[1]=false;
    cellStateMap[2]=false;
    cellStateMap[3]=false;
    cellStateMap[4]=false;
    cellStateMap[5]=false;
    reloadDataByRank(cellStateMap[1],cellStateMap[2],cellStateMap[3],cellStateMap[4],cellStateMap[5]);
    m_tableView->reloadData();
}

void FriendSearchView::reloadDataByRank(bool showR1,bool showR2,bool showR3,bool showR4,bool showR5){
    for (int i=0; i<m_tempData.size(); i++) {
        FriendSearchInfo* info = &m_tempData[i];
        if (info->rank==1) {
            m_allianceR1.push_back(m_tempData[i]);
        }
        if (info->rank==2) {
            m_allianceR2.push_back(m_tempData[i]);
        }
        if (info->rank==3) {
            m_allianceR3.push_back(m_tempData[i]);
        }
        if (info->rank==4) {
            m_allianceR4.push_back(m_tempData[i]);
        }
        if (info->rank==5) {
            m_allianceR5.push_back(m_tempData[i]);
        }
    }
    m_playerData.clear();
    FriendSearchInfo* titleR5 = FriendSearchInfo::create(NULL, 5,cellStateMap[5],m_allianceR5.size());
    m_playerData.push_back((*titleR5));
    if (showR5) {
        for (int i=0; i<m_allianceR5.size(); i++) {
            m_playerData.push_back(m_allianceR5[i]);
        }
    }
    FriendSearchInfo* titleR4 = FriendSearchInfo::create(NULL, 4,cellStateMap[4],m_allianceR4.size());
    m_playerData.push_back((*titleR4));
    if (showR4) {
        for (int i=0; i<m_allianceR4.size(); i++) {
            m_playerData.push_back(m_allianceR4[i]);
        }
    }
    FriendSearchInfo* titleR3 = FriendSearchInfo::create(NULL, 3,cellStateMap[3],m_allianceR3.size());
    m_playerData.push_back((*titleR3));
    if (showR3) {
        for (int i=0; i<m_allianceR3.size(); i++) {
            m_playerData.push_back(m_allianceR3[i]);
        }
    }
    FriendSearchInfo* titleR2 = FriendSearchInfo::create(NULL, 2,cellStateMap[2],m_allianceR2.size());
    m_playerData.push_back((*titleR2));
    if (showR2) {
        for (int i=0; i<m_allianceR2.size(); i++) {
            m_playerData.push_back(m_allianceR2[i]);
        }
    }
    FriendSearchInfo* titleR1 = FriendSearchInfo::create(NULL, 1,cellStateMap[1],m_allianceR1.size());
    m_playerData.push_back((*titleR1));
    if (showR1) {
        for (int i=0; i<m_allianceR1.size(); i++) {
            m_playerData.push_back(m_allianceR1[i]);
        }
    }
    m_allianceR1.clear();
    m_allianceR2.clear();
    m_allianceR3.clear();
    m_allianceR4.clear();
    m_allianceR5.clear();
//    m_tableView->reloadData();
}

void FriendSearchView::onEnter()
{
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

//    setTitleName(_lang("113907"));
    UIComponent::getInstance()->showPopupView(UIPopupViewType_ArcPop_TitanUpgrade);
    m_titleTxt->setString(_lang("113907"));
    
    if (m_playerData.size() == 0) {
        generateTabView();
    } else {
        generateTabViewMore();
    }
}

void FriendSearchView::onExit()
{
    if(m_waitInterface != NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    removeLoadingAni();
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

void FriendSearchView::generateTabView()
{
    m_tableView->reloadData();
}

void FriendSearchView::generateTabViewMore()
{
    float curOffsetY = m_tableView->getContentOffset().y;
    float curMinOffsetY = m_tableView->minContainerOffset().y;
    m_tableView->reloadData();
    float newMinOffsetY = m_tableView->minContainerOffset().y;
    m_tableView->setContentOffset(ccp(0, newMinOffsetY - curMinOffsetY + curOffsetY));
}

void FriendSearchView::onClickSearchBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    string name = m_editBox->getText();
    if (name.length() < 3) {
//        m_playerData.clear();
//        m_totalPlayerNum = 0;
//        m_lastPlayerSearch = name;
//        generateTabView();
        YesNoDialog::showYesDialog(_lang_1("113909", "3"));
        return;
    }
    if (strcmp(m_lastPlayerSearch.c_str(), name.c_str()) == 0) {
        generateTabView();
        return;
    }
    m_playerData.clear();
    m_totalPlayerNum = 0;
    addLoadingAni();
    if(m_waitInterface != NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    m_waitInterface = GameController::getInstance()->showWaitInterface(m_searchBtn);
    SearchPlayerCommand* cmd = new SearchPlayerCommand(name, 1);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(FriendSearchView::getPlayerData), NULL));
    cmd->sendAndRelease();
}

void FriendSearchView::addLoadingAni(){
    if(m_loadingIcon){
        m_listNode->removeChild(m_loadingIcon);
        m_loadingIcon = NULL;
    }
    m_tableView->setTouchEnabled(false);
    m_loadingIcon = CCLoadSprite::createSprite("loading_1.png");
    m_loadingIcon->setAnchorPoint(ccp(0.5, 0.5));
    auto cSize = m_listNode->getContentSize();
    m_loadingIcon->setPosition(ccp(cSize.width/2,cSize.height/2));
    
    CCActionInterval * rotateto1 = CCRotateTo::create(0.5, 720);
    CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
    CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
    m_loadingIcon->runAction(rotateForever);
    m_listNode->addChild(m_loadingIcon,1000000);
}

void FriendSearchView::removeLoadingAni(CCObject *obj){
    if(m_loadingIcon){
        m_listNode->removeChild(m_loadingIcon);
        m_loadingIcon = NULL;
    }
    if (m_tableView) {
        m_tableView->setTouchEnabled(true);
    }
}

void FriendSearchView::getPlayerData(CCObject* param){
    if(m_waitInterface != NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    removeLoadingAni();
    NetResult* netResult = dynamic_cast<NetResult*>(param);
    
    if( netResult == NULL )
        return;
    
    auto dict = _dict(netResult->getData());
    if (dict->objectForKey("errorCode")) {
        string code = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyHint("", "", _lang(code));
        generateTabView();
        return;
    }
    if (!dict->valueForKey("page")) {
        generateTabView();
        return;
    }
    int page = dict->valueForKey("page")->intValue();
    if (page == 1) {
        
        m_playerData.clear();
        m_lastPlayerSearch = dict->valueForKey("key")->getCString();
        m_totalPlayerNum = dict->valueForKey("total")->intValue();
        if (m_totalPlayerNum == 0) {
            generateTabView();
            return;
        }
        CCArray* list = dynamic_cast<CCArray*>(dict->objectForKey("list"));
        for (int i = 0; i < list->count(); ++i) {
            CCDictionary* item = _dict(list->objectAtIndex(i));
            m_playerData.push_back(FriendSearchInfo(item,0));
        }
        if (list->count() < m_totalPlayerNum) {
            m_playerNumPerPage = list->count();
        }
        if (dict->objectForKey("max")) {
            m_maxPlayerNum = dict->valueForKey("max")->intValue();
        }
        generateTabView();
    } else {
        CCArray* list = dynamic_cast<CCArray*>(dict->objectForKey("list"));
        if (list) {
            for (int i = 0; i < list->count(); ++i) {
                CCDictionary* item = _dict(list->objectAtIndex(i));
                m_playerData.push_back(FriendSearchInfo(item,0));
            }
            generateTabViewMore();
        }
    }
}

bool FriendSearchView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    return true;
}

void FriendSearchView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}

void FriendSearchView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_tableView->getContentOffset().y > 30 && m_playerData.size() && m_playerData.size() < m_totalPlayerNum && m_playerNumPerPage && m_playerData.size() < m_maxPlayerNum) {
        addLoadingAni();
        if(m_waitInterface != NULL){
            m_waitInterface->remove();
            m_waitInterface = NULL;
        }
        m_waitInterface = GameController::getInstance()->showWaitInterface(m_searchBtn);
        m_tableView->setBounceable(false);
        this->scheduleOnce(schedule_selector(FriendSearchView::callMore), 0.0);
    }
}

void FriendSearchView::callMore(float t)
{
    m_tableView->setBounceable(true);
    int nextPage = m_playerData.size() / m_playerNumPerPage  + 1;
    SearchPlayerCommand* cmd = new SearchPlayerCommand(m_lastPlayerSearch, nextPage);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(FriendSearchView::getPlayerData), NULL));
    cmd->sendAndRelease();
}

#pragma mark tableview
void FriendSearchView::tableCellTouched(CCTableView* table, CCTableViewCell* cell){
    FriendSearchCell* fscell = (FriendSearchCell*)cell;
    int cType = fscell->cellType;
    if(cType<1){//普通条目
    }else{//标题条目
        if(cellStateMap.find(cType)!=cellStateMap.end()){
            cellStateMap[cType]=!cellStateMap[cType];
        }
        reloadDataByRank(cellStateMap[1],cellStateMap[2],cellStateMap[3],cellStateMap[4],cellStateMap[5]);
        float curOffsetY = m_tableView->getContentOffset().y;
        float curMinOffsetY = m_tableView->minContainerOffset().y;
        m_tableView->reloadData();
        float newMinOffsetY = m_tableView->minContainerOffset().y;
        m_tableView->setContentOffset(ccp(0, newMinOffsetY - curMinOffsetY + curOffsetY));
    }
}
cocos2d::CCSize FriendSearchView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if (idx > m_playerData.size())
    {
        return CCSizeZero;
    }
    if(m_playerData[idx].cellType>0){
        if (CCCommonUtils::isIosAndroidPad()){
            return CCSize(640*2.4, 50*2.4);
        }
        return CCSize(640, 50);
    } else {
        if (CCCommonUtils::isIosAndroidPad()){
            return CCSize(640*2.4, 138*2.4);
        }
        return CCSize(640, 138);
    }
}

cocos2d::CCSize FriendSearchView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()){
        return CCSize(640*2.4, 138*2.4);
    }
    return CCSize(640, 138);
}
CCTableViewCell* FriendSearchView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if (idx >= m_playerData.size())
    {
        return nullptr;
    }
    FriendSearchCell* cell = (FriendSearchCell*)table->dequeueCell();
    FriendSearchInfo* info = &m_playerData[idx];
    if (cell) {
        cell->setData(info);
    } else {
        cell = FriendSearchCell::create(info);
    }
    return cell;
}
ssize_t FriendSearchView::numberOfCellsInTableView(CCTableView *table)
{
    return m_playerData.size();
}


SEL_CCControlHandler FriendSearchView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickSearchBtn", FriendSearchView::onClickSearchBtn);
    return NULL;
}

bool FriendSearchView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTxt", CCLabelIF*, m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_searchBtn", CCControlButton*, m_searchBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_editNode", CCNode*, m_editNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_listNode", CCNode*, m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bgNode", CCNode*, m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_searchIconNode", CCNode*, m_searchIconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_searchShowTxt", CCLabelIF*, m_searchShowTxt);
    return false;
}


#pragma mark cell
FriendSearchCell* FriendSearchCell::create(FriendSearchInfo* searchInfo)
{
    auto ret = new FriendSearchCell();
    if (ret && ret->init(searchInfo))
    {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool FriendSearchCell::init(FriendSearchInfo* searchInfo)
{
    CCBLoadFile("FriendSearchCell", this, this);
//    if (CCCommonUtils::isIosAndroidPad())
//    {
//        this->setScale(2.0);
//    }
    m_vipLabel->setColor(FriendSearchView_Collor);
    m_nameLabel->setColor(FriendSearchView_Collor);
    m_levelTxt->setColor(FriendSearchView_Collor);
    m_onlineLabel->setColor(FriendSearchView_Collor);
    setData(searchInfo);
    return true;
}

void FriendSearchCell::setData(FriendSearchInfo* searchInfo){
    cellType = searchInfo->cellType;
    saveInfo = searchInfo;
    if (cellType>0) {
        this->setContentSize(CCSize(640, 50));
        m_titleNode->setVisible(true);
        m_infoNode->setVisible(false);
        if (searchInfo->cellType==1) {
            m_groupLabel->setString(_lang("115100"));//联盟阶级1
        }
        if (searchInfo->cellType==2) {
            m_groupLabel->setString(_lang("115101"));//联盟阶级2
        }
        if (searchInfo->cellType==3) {
            m_groupLabel->setString(_lang("115102"));//联盟阶级3
        }
        if (searchInfo->cellType==4) {
            m_groupLabel->setString(_lang("115103"));//联盟阶级4
        }
        if (searchInfo->cellType==5) {
            m_groupLabel->setString(_lang("115104"));//联盟阶级5
        }
        if (searchInfo->cellOpen) {
            m_arrowSpr->setRotation(90);
        } else {
            m_arrowSpr->setRotation(0);
        }
        m_numLabel->setString(CC_ITOA(saveInfo->cellNum));
    }else{
        this->setContentSize(CCSize(640, 138));
        m_titleNode->setVisible(false);
        m_infoNode->setVisible(true);
        m_iconNode->removeAllChildrenWithCleanup(true);
//        CCDictionary* dic = FriendsController::getInstance()->m_data;

        if (searchInfo->description.empty())
        {
            string name="";
            if (!searchInfo->abbr.empty())
            {
                name.append("(").append(searchInfo->abbr).append(")");
            }
            name.append(searchInfo->name);
            if (searchInfo->serverId>=0) {
                name.append(" #").append(CC_ITOA(searchInfo->serverId));
            }
            m_nameLabel->setString(name);
        } else {
            m_nameLabel->setString(searchInfo->description);
        }
        m_nameLabel->setMaxScaleXByWidth(300);//防爆框
        m_levelTxt->setVisible(false);
        if (saveInfo->mainBuildingLevel>0 && saveInfo->mainBuildingLevel<=100) {
            m_levelTxt->setVisible(true);
            m_levelTxt->setString(_lang_1("115515", CC_ITOA(saveInfo->mainBuildingLevel)));//115515=城堡等级{0}级
        }
    
        m_iconNode->removeAllChildrenWithCleanup(true);
        string picPath = searchInfo->pic;
        if (picPath.empty()) {
            picPath = "g044.png";
        } else {
            picPath.append(".png");
        }
        auto icon = CCLoadSprite::createSprite(picPath.c_str(),true,CCLoadSpriteType_HEAD_ICON);
        CCCommonUtils::setSpriteMaxSize(icon, 104, true);
        m_iconNode->addChild(icon);
        if (CCCommonUtils::isUseCustomPic(searchInfo->picVer))
        {
            m_headImgNode = HFHeadImgNode::create();
            m_headImgNode->initHeadImgUrl2(m_iconNode, CCCommonUtils::getCustomPicUrl(searchInfo->uid, searchInfo->picVer), 1.0f, 104, true);
        }
    
//            if (FriendsController::getInstance()->isApplyingMe(m_uid)) {
//                m_acceptBtn->setVisible(true);
//                m_cancelBtn->setVisible(true);
//                m_acceptBtn->setEnabled(true);
//                m_cancelBtn->setEnabled(true);
//            } else {
//                m_acceptBtn->setVisible(false);
//                m_cancelBtn->setVisible(false);
//            }
        
        if (saveInfo->uid == GlobalData::shared()->playerInfo.uid) {//自己
            m_applyBtn->setEnabled(false);
            m_applyBtn->setVisible(false);
            m_btnTxt->setString("");
            return;
        }
        
        if (FriendsController::getInstance()->isBeingAppliedByMe(saveInfo->uid)) {
            m_applyBtn->setEnabled(false);
            m_applyBtn->setVisible(false);
//            CCCommonUtils::setButtonTitle(m_applyBtn, _lang("132101").c_str());
            m_btnTxt->setColor(FriendSearchView_Collor);
            m_btnTxt->setString(_lang("132101"));
        } else if (FriendsController::getInstance()->isMyFriend(saveInfo->uid)) {
            m_applyBtn->setEnabled(false);
            m_applyBtn->setVisible(false);
            m_btnTxt->setColor(FriendSearchView_Collor);
//            CCCommonUtils::setButtonTitle(m_applyBtn, _lang("132102").c_str());
            m_btnTxt->setString(_lang("132102"));
        } else {
            m_applyBtn->setEnabled(true);
            m_applyBtn->setVisible(true);
            m_btnTxt->setColor(FriendSearchView_BtnCollor);
//            CCCommonUtils::setButtonTitle(m_applyBtn, _lang("132100").c_str());
            m_btnTxt->setString(_lang("132100"));
        }
    }
}

void FriendSearchCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FriendSearchCell::onGetMsgApplySuc), APPLY_FRIEND_SEND_SUC, NULL);
}

void FriendSearchCell::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, APPLY_FRIEND_SEND_SUC);
    setTouchEnabled(false);
    CCNode::onExit();
}

bool FriendSearchCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (isTouchInside(m_iconTouchNode, pTouch) && !isTouchInside(m_applyBtn, pTouch) && cellType==0) {//&& !isTouchInside(m_applyBtn, pTouch)
        
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS) and __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_9_0
        
//                GlobalData::shared()->peekPageType = peekType_playerInfo; simon
//                ThreeDTouchController::getInstance()->sendGetUserInfoCommand(saveInfo->uid);

#endif

        
        return true;
    }
    return false;
}

void FriendSearchCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
//    if (GlobalData::shared()->peekPageType == peekType_peek && GlobalData::shared()->peekPageType == popType_pop)
//        return; simon
    
    if (isTouchInside(m_iconTouchNode, pTouch) && !isTouchInside(m_applyBtn, pTouch) &&
        (pTouch->getLocation().distance(pTouch->getStartLocation())<20)) {//
        string playerUid = saveInfo->uid;
        if(playerUid!=""){
            RoleInfoView::createInfoByUid(playerUid);
        }
    }
}

SEL_CCControlHandler FriendSearchCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickApplyBtn", FriendSearchCell::onClickApplyBtn);
    return NULL;
}


bool FriendSearchCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoNode", CCNode*, this->m_infoNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode1", CCNode*, this->m_touchNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconTouchNode", CCNode*, this->m_iconTouchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vipLabel", CCLabelIF*, this->m_vipLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_levelTxt", CCLabelIF*, this->m_levelTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_onlineLabel", CCLabelIF*, this->m_onlineLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnTxt", CCLabelIF*, this->m_btnTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_applyBtn", CCControlButton*, this->m_applyBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleNode", CCNode*, this->m_titleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode2", CCNode*, this->m_touchNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrowSpr", CCSprite*, this->m_arrowSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_groupLabel", CCLabelIF*, this->m_groupLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, this->m_numLabel);
    return false;
}

void FriendSearchCell::onClickApplyBtn(CCObject * pSender, Control::EventType pCCControlEvent){
    CCLOGFUNC();
    m_applyBtn->setEnabled(false);
    m_applyBtn->setVisible(false);
    m_btnTxt->setColor(FriendSearchView_Collor);
//    CCCommonUtils::setButtonTitle(m_applyBtn, _lang("132101").c_str());
    m_btnTxt->setString(_lang("132101"));
    FriendsController::getInstance()->startApplyFriend(saveInfo->uid);
}

void FriendSearchCell::onGetMsgApplySuc(CCObject* obj)
{
    if (!obj) return;
    CCString* cs = dynamic_cast<CCString*>(obj);
    if (!cs) {
        return;
    }
    string uid = cs->getCString();
    if (strcmp(uid.c_str(), saveInfo->uid.c_str()) != 0) return;
    if (FriendsController::getInstance()->isMyFriend(uid)) {
        m_applyBtn->setEnabled(false);
        m_applyBtn->setVisible(false);
        m_btnTxt->setColor(FriendSearchView_Collor);
//        CCCommonUtils::setButtonTitle(m_applyBtn, _lang("132102").c_str());//已是好友
        m_btnTxt->setString(_lang("132102"));
    } else if (FriendsController::getInstance()->isBeingAppliedByMe(uid)) {
        m_applyBtn->setEnabled(false);
        m_applyBtn->setVisible(false);
        m_btnTxt->setColor(FriendSearchView_Collor);
//        CCCommonUtils::setButtonTitle(m_applyBtn, _lang("132101").c_str());//已申请
        m_btnTxt->setString(_lang("132101"));
        CCCommonUtils::flyHint("", "", _lang("132101"));//已申请
    }
}
