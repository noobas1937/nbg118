//
//  MailHeiqishiListView.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/10/28.
//
//

#include "MailHeiqishiListView.hpp"
#include "SceneController.h"
#include "PopupViewController.h"
#include "CCTypesExt.h"
#include "CCCommonUtils.h"
#include "ChatController.h"
#include "CCSafeNotificationCenter.h"
#include "CCLabelIF.h"
#include "ChatCell.h"
#include "YesNoDialog.h"
#include "ChatLockCommand.h"
#include "ChatUnLockCommand.h"
#include "MailWritePopUpView.h"
#include "GetUserInfoCommand.h"
#include "../../Ext/CCDevice.h"
#include "RoleInfoView.h"
#include "UIComponent.h"
#include "CCEditText.h"
#include "InvitesAllianceCommand.h"
#include "InputFieldMultiLine.h"
#include "JoinAllianceView.h"
#include "AlertAddAllianceView.h"
#include "CheckAllianceInfoView.h"
#include "MailController.h"
#include "MailDeleteCommand.h"
#include "MailReadCommand.h"
#include "PlayerInfoBtnPopUpView.h"
#include "Utf8Utils.h"
#include "RewardController.h"
#include "WorldController.h"
#include "WorldMapView.h"
#include "DynamicResourceController.h"
#include "ChatServiceCocos2dx.h"
#include "MailWritePopUpView.h"
#include "UIComponent.h"
#include "MailBatchCommand.h"
#include "ChatServiceCocos2dx.h"
MailHeiqishiListView* MailHeiqishiListView::create(MailHeiqishiCellInfo* info ){
    MailHeiqishiListView* ret = new MailHeiqishiListView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
MailHeiqishiListView::~MailHeiqishiListView(){
    
}
bool MailHeiqishiListView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
//    auto cf = CCLoadSprite::getSF("Mail_diban.png");
    auto cf = CCLoadSprite::getSF("Mail_BG1.png");
    if (cf==NULL) {
        CCLoadSprite::doResourceByCommonIndex(206, true);
        CCLoadSprite::doResourceByCommonIndex(6, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(206, false);
            CCLoadSprite::doResourceByCommonIndex(6, false);
            CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_MONSTERLAYERLITTLE);
            CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
        });
    }
    else {
        CCLoadSprite::doResourceByCommonIndex(206, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(206, false);
            CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_MONSTERLAYERLITTLE);
            CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
        });
    }
    auto tmpCCB = CCBLoadFile("NEW_MailResourceCCB",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else
        this->setContentSize(tmpCCB->getContentSize());
    
    m_titleText->setString(_lang("133053"));
    if (CCCommonUtils::isIosAndroidPad()) {
        int extH = getExtendHeight();
        this->m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height + extH));
        m_downNode->setPositionY(m_downNode->getPositionY() - extH);
        m_bgNode->setPositionY(m_bgNode->getPositionY() - extH);
        auto tbg = CCLoadSprite::loadResource("Mail_diban.png");
        auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
        auto picBg1 = CCLoadSprite::createSprite("Mail_diban.png");
        picBg1->setAnchorPoint(ccp(0, 0));
        picBg1->setPosition(ccp(0, 0));
        picBg1->setScaleX(2.4);
        tBatchNode->addChild(picBg1);
        int maxHeight = CCDirector::sharedDirector()->getWinSize().height;
        int curHeight = picBg1->getContentSize().height;
        while(curHeight < maxHeight)
        {
            auto picBg2 = CCLoadSprite::createSprite("Mail_diban.png");
            picBg2->setAnchorPoint(ccp(0, 0));
            picBg2->setPosition(ccp(0, curHeight));
            picBg2->setScaleX(2.4);
            tBatchNode->addChild(picBg2);
            curHeight += picBg2->getContentSize().height;
        }
        m_bgNode->addChild(tBatchNode);
    }
    else {
        int extH = getExtendHeight();
        this->m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height + extH));
        m_downNode->setPositionY(m_downNode->getPositionY() - extH);
        m_bgNode->setPositionY(m_bgNode->getPositionY() - extH);
        auto tbg = CCLoadSprite::loadResource("Mail_diban.png");
        auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
        auto picBg1 = CCLoadSprite::createSprite("Mail_diban.png");
        picBg1->setAnchorPoint(ccp(0, 0));
        picBg1->setPosition(ccp(0, 0));
        tBatchNode->addChild(picBg1);
        int maxHeight = CCDirector::sharedDirector()->getWinSize().height;
        int curHeight = picBg1->getContentSize().height;
        while(curHeight < maxHeight)
        {
            auto picBg2 = CCLoadSprite::createSprite("Mail_diban.png");
            picBg2->setAnchorPoint(ccp(0, 0));
            picBg2->setPosition(ccp(0, curHeight));
            tBatchNode->addChild(picBg2);
            curHeight += picBg2->getContentSize().height;
        }
        m_bgNode->addChild(tBatchNode);
    }
    m_battlePicNode->removeAllChildren();
    auto battlePic = CCLoadSprite::createSprite("Mail_monster.png");
    this->m_battlePicNode->addChild(battlePic);
    
    m_data = CCArray::create();
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setDelegate(this);
    m_tabView->setTouchPriority(Touch_Default);
    m_infoList->addChild(m_tabView);
    refresh(NULL);
    return true;
}
void MailHeiqishiListView::refresh(CCObject* obj){
    CCLOGFUNCF("knight refresh");
    if(m_mailInfo->save ==0){
        m_addSaveBtn->setHighlighted(false);
    }else{
        m_addSaveBtn->setHighlighted(true);
    }
    setData();
    bool ishasChild = false;
    CCPoint pos;
    if(m_tabView->getContainer()->getChildrenCount()==0){
        ishasChild = true;
    }else{
        pos = m_tabView->getContentOffset();
    }
    m_tabView->reloadData();
    CCPoint minPt = m_tabView->minContainerOffset();
    CCPoint maxPt = m_tabView->maxContainerOffset();
    if (pos.y > maxPt.y) {
        pos.y = maxPt.y;
    }
    if (pos.y < minPt.y) {
        pos.y = minPt.y;
    }
    if(!ishasChild){
        m_tabView->setContentOffset(pos);
    }
}
void MailHeiqishiListView::readDialogContent(){
    if(m_mailInfo->unread>0){
        if (m_mailInfo->knight->count() > 0) {
            int i = 0;
            std::string uids = "";
            while (i < m_mailInfo->knight->count()) {
                MailHeiqishiInfo* info = dynamic_cast<MailHeiqishiInfo*>(m_mailInfo->knight->objectAtIndex(i));
                uids += info->uid;
                if (i != m_mailInfo->knight->count() - 1) {
                    uids += ",";
                }
            }
//            MailReadStatusBatchCommand* cmd = new MailReadStatusBatchCommand(uids);  simon
//            cmd->sendAndRelease();
            GlobalData::shared()->mailCountObj.sysR -=m_mailInfo->unread;
            m_mailInfo->unread = 0;
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
        }
    }
}
void MailHeiqishiListView::onEnter()
{
    CCNode::onEnter();
    UIComponent::getInstance()->showPopupView(UIPopupViewType_ArcPop_TitanUpgrade);
    UIComponent::getInstance()->hideReturnBtn();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailHeiqishiListView::refresh), MAIL_LIST_DELETE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailHeiqishiListView::refreshAddList), MAIL_LIST_ADD, NULL);
    setTouchEnabled(true);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::setChannelPopupOpen("knight");
#endif
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -2, false);
}

void MailHeiqishiListView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_LIST_DELETE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_LIST_ADD);
    
    setTouchEnabled(false);
//    readDialogContent();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::setChannelPopupOpen("");
#endif
    PopupBaseView::onExit();
}

bool MailHeiqishiListView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (isTouchInside(m_returnSpr, pTouch)) {
        return true;
    }
    m_tabView->setBounceable(true);
    if(m_isLoadMore&&isTouchInside( m_infoList,pTouch))
        return true;
    else
        return false;
}
void MailHeiqishiListView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(!m_isLoadMore){
        return;
    }
    m_currMinOffsetY = m_tabView->minContainerOffset().y;
    float offsetY = m_tabView->getContentOffset().y;
    if(offsetY>0&&offsetY>m_currMinOffsetY){
        float y = (pTouch->getLocation().y - pTouch->getStartLocation().y)/5;
        if(y<m_currMinOffsetY){
            y += m_currMinOffsetY;
        }
        m_tabView->setContentOffset(ccp(0,y));
    }
    if(m_isLoadMore && offsetY>-30){
        auto cell = dynamic_cast<MailHeiqishiCell*>(m_tabView->cellAtIndex(m_data->count()-1));
        if(cell==NULL){
            return;
        }
        CCLabelIF* txt = (CCLabelIF*)cell->getChildByTag(10001);
        if (txt && txt->isVisible()) {
            if(offsetY>30&&offsetY>m_currMinOffsetY)
                txt->setString(_lang("105318").c_str());
            else
                txt->setString(_lang("115189").c_str());
        }
    }
    return;
    
}
void MailHeiqishiListView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    AutoSafeRef tmp(this);
    
    if (isTouchInside(m_returnSpr, pTouch)) {
        PopupViewController::getInstance()->goBackPopupView();
    }
    if(m_isLoadMore&&m_tabView){
        m_currMinOffsetY = m_tabView->minContainerOffset().y;
        m_currOffsetY = m_tabView->getContentOffset().y;
        int dy = m_currMinOffsetY+30;
        if(m_currOffsetY >30&&m_currOffsetY>dy){
            addLoadingAni();
            m_tabView->setBounceable(false);
            this->scheduleOnce(schedule_selector(MailHeiqishiListView::sendReloadMoreMail),0.0);
        }
    }
}
void MailHeiqishiListView::sendReloadMoreMail(float _time){
    int realcount = m_mailInfo->knight->count();///////////////
    addLoadingAni();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (MailController::getInstance()->getIsNewMailListEnable()) {
        ChatServiceCocos2dx::loadMoreMailFromAndroid("knight");
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ChatServiceCocos2dx::loadMoreMailFromIOS("knight");
#endif
}
void MailHeiqishiListView::removeLoadingAni(){
    if(m_loadingIcon){
        m_infoList->removeChild(m_loadingIcon);
        m_loadingIcon = NULL;
    }
}
void MailHeiqishiListView::addLoadingAni(){
    if(m_loadingIcon){
        m_infoList->removeChild(m_loadingIcon);
        m_loadingIcon = NULL;
    }
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

void MailHeiqishiListView::refreshAddList(cocos2d::CCObject *obj){
    CCLOGFUNC("knight refreshAddList");
    setData();
    removeLoadingAni();
    int add = dynamic_cast<CCInteger*>(obj)->getValue();
    int MinOffsetY = m_tabView->minContainerOffset().y;
    CCPoint pos = m_tabView->getContentOffset();
    int MinOffsetY1 = pos.y;
    if(add==0){
        m_tabView->reloadData();
        m_tabView->setContentOffset(ccp(0,0));
        return;
    }
    m_tabView->reloadData();
    CCPoint minPt = m_tabView->minContainerOffset();
    CCPoint maxPt = m_tabView->maxContainerOffset();
    
    if (MinOffsetY1 > maxPt.y) {
        MinOffsetY1 = maxPt.y;
    }
    
    if (MinOffsetY1 < minPt.y) {
        MinOffsetY1 = minPt.y;
    }
    
    m_newMinOffsetY = m_tabView->minContainerOffset().y;
    if(m_newMinOffsetY<0){
        m_tabView->setContentOffset(ccp(0,m_newMinOffsetY-m_currMinOffsetY));
        float mvdh =m_newMinOffsetY-m_currMinOffsetY + 200;
        if(mvdh>0){
            mvdh = 0;
        }
        m_tabView->setContentOffsetInDuration(ccp(0,mvdh), 0.7);
    }
    
}
SEL_CCControlHandler MailHeiqishiListView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddSaveClick", MailHeiqishiListView::onAddSaveClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDeleteClick", MailHeiqishiListView::onDeleteClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onReturnClick", MailHeiqishiListView::onReturnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onWriteClick", MailHeiqishiListView::onWriteClick);
    return NULL;
}

bool MailHeiqishiListView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downNode", CCNode*, this->m_downNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addSaveBtn", CCControlButton*, this->m_addSaveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_battlePicNode", CCNode*, m_battlePicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_deleteBtn", CCControlButton*, this->m_deleteBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_writeBtn", CCControlButton*, this->m_writeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_returnBtn", CCControlButton*, this->m_returnBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_returnSpr", CCSprite*, this->m_returnSpr);
    return false;
}
void MailHeiqishiListView::onDeleteClick(cocos2d::CCObject *pSender, CCControlEvent event){
    if(m_mailInfo->save ==1){
        CCCommonUtils::flyHint("quest_icon_1.png", "", _lang("105575"));
        return;
    }
    map<string, MailInfo*>::iterator it;
    it = GlobalData::shared()->mailList.find(m_mailInfo->uid);
    if (GlobalData::shared()->mailList.end() == it) {
        return;
    }
    MailHeiqishiCellInfo* tempInfo = dynamic_cast<MailHeiqishiCellInfo*>(it->second);
    if( tempInfo == NULL )
    {
        GlobalData::shared()->mailList.erase(it);
        return;
    }
    if (m_mailInfo->knight->count() > 0) {
        int i = 0;
        std::string uids = "";
        while (i < m_mailInfo->knight->count()) {
            MailHeiqishiInfo* info = dynamic_cast<MailHeiqishiInfo*>(m_mailInfo->knight->objectAtIndex(i));
            if (info->uid != "") {
                uids.append(info->uid);
                if (i != m_mailInfo->knight->count() - 1) {
                    uids.append(",");
                }
            }
            i++;
        }
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        //黑骑士删除回掉
        ChatServiceCocos2dx::deleteingKnightMail();
        #endif
        
        MailBatchDelCommand* command = new MailBatchDelCommand(uids, "");
        command->sendAndRelease();
        m_isLoadMore = false;
        GlobalData::shared()->mailCountObj.sysT -=1;
        GlobalData::shared()->mailCountObj.sysR -=tempInfo->unread;
        GlobalData::shared()->mailList.erase(it);
        tempInfo->unread = 0;
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
        tempInfo->release();
        PopupViewController::getInstance()->goBackPopupView();
    }
}
void MailHeiqishiListView::onAddSaveClick(cocos2d::CCObject *pSender, CCControlEvent event){
    if(m_mailInfo->knight->count()==0)
        return;
    if(m_mailInfo->save ==1){
        if (m_mailInfo->knight->count() > 0) {
            int i = 0;
            std::string uids = "";
            while (i < m_mailInfo->knight->count()) {
                MailHeiqishiInfo* info = dynamic_cast<MailHeiqishiInfo*>(m_mailInfo->knight->objectAtIndex(i));
                if (info->uid != "") {
                    uids.append(info->uid);
                    if (i != m_mailInfo->knight->count() - 1) {
                        uids.append(",");
                    }
                }
                i++;
            }
//            MailBatchSaveCommand *cmd = new MailBatchSaveCommand(uids, 0);  simon
//            cmd->sendAndRelease();
            m_mailInfo->save = 0;
            CCCommonUtils::flyHint("quest_icon_1.png", "", _lang("105576"));
            m_addSaveBtn->setHighlighted(false);
        }
    }else{
        if (m_mailInfo->knight->count() > 0) {
            int i = 0;
            std::string uids = "";
            while (i < m_mailInfo->knight->count()) {
                MailHeiqishiInfo* info = dynamic_cast<MailHeiqishiInfo*>(m_mailInfo->knight->objectAtIndex(i));
                if (info->uid != "") {
                    uids.append(info->uid);
                    if (i != m_mailInfo->knight->count() - 1) {
                        uids.append(",");
                    }
                }
                i++;
            }
//            MailBatchSaveCommand *cmd = new MailBatchSaveCommand(uids, 1);  simon
//            cmd->sendAndRelease();
            m_mailInfo->save = 1;
            CCCommonUtils::flyHint("quest_icon_1.png", "", _lang("105575"));
            m_addSaveBtn->setHighlighted(true);
        }
    }
}
void MailHeiqishiListView::onReturnClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent){
    PopupViewController::getInstance()->goBackPopupView();
}

void MailHeiqishiListView::onWriteClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent){
    PopupViewController::getInstance()->addPopupInView(MailWritePopUpView::create("", ""));
}
void MailHeiqishiListView::setData(){
    m_data->removeAllObjects();
    if(m_mailInfo->knight == NULL)
        return;
    if(m_mailInfo->knight->count()<m_mailInfo->totalNum){
        m_isLoadMore = true;
        CCLOGFUNCF("knight m_isLoadMore:%d",1);
    }else{
        m_isLoadMore = false;
        CCLOGFUNCF("knight m_isLoadMore:%d",0);
    }
    
    CCObject* obj;
    CCARRAY_FOREACH(m_mailInfo->knight, obj){
        m_data->addObject(obj);
    }
    if(m_isLoadMore){
        CCLOGFUNCF("knight arr:%d",m_data->count());
        MailHeiqishiInfo* info = MailHeiqishiInfo::create();
        info->type = -3;
        m_data->addObject(info);
    }
}

void MailHeiqishiListView::scrollViewDidScroll(CCScrollView* view)
{
    
}

void MailHeiqishiListView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    ChatCell* tmpCell = dynamic_cast<ChatCell*>(cell);
    if (tmpCell) {
        //        tmpCell->cellTouchEnded(m_tabView->m_pTouchedCell);
    }
}
CCSize MailHeiqishiListView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    int height = 500;
    if (CCCommonUtils::isIosAndroidPad())
    {
        height = 1200;
    }
    if(m_isLoadMore&&idx==m_data->count()-1){
        if (CCCommonUtils::isIosAndroidPad()) {
            return CCSize(1464, 120);
        }
        return CCSize(614,50);
    }
    if (CCCommonUtils::isIosAndroidPad()) {
        height += 24;
        return CCSize(1464, height);
    }else {
        height+=10;
        return CCSize(614,height);
    }
}

cocos2d::CCSize MailHeiqishiListView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1464, 1200);
    }
    return CCSize(614, 500);
}

CCTableViewCell* MailHeiqishiListView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx>=m_data->count()){
        return NULL;
    }
    MailHeiqishiInfo* dialogInfo = dynamic_cast<MailHeiqishiInfo*>(m_data->objectAtIndex(idx));
    MailHeiqishiCell* cell = (MailHeiqishiCell*)table->dequeueCell();
    if(cell){
        cell->setData(dialogInfo,m_mailInfo,idx);
    }else{
        cell = MailHeiqishiCell::create(dialogInfo,m_mailInfo,idx);
    }
    return cell;
}

ssize_t MailHeiqishiListView::numberOfCellsInTableView(CCTableView *table){
    int cellNum = 0;
    cellNum = m_data->count();
    return cellNum;
}

//.victory kill
MailHeiqishiCell* MailHeiqishiCell::create(MailHeiqishiInfo* dialogInfo,MailHeiqishiCellInfo* mailInfo,int index){
    MailHeiqishiCell* ret = new MailHeiqishiCell(dialogInfo,mailInfo,index);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MailHeiqishiCell::init()
{
    bool ret = true;
    m_ccbNode = CCBLoadFile("NEW_MailHeiqishiCell",this,this);
    m_tipLoadingText = CCLabelIF::create();
    if (CCCommonUtils::isIosAndroidPad()) {
        m_tipLoadingText->setFontSize(45);
    }
    else
        m_tipLoadingText->setFontSize(22);
    m_tipLoadingText->setVerticalAlignment(kCCVerticalTextAlignmentTop);
    m_tipLoadingText->setHorizontalAlignment(kCCTextAlignmentLeft);
    m_tipLoadingText->setColor({117,62,15});
    m_tipLoadingText->setString(_lang("115189").c_str());
    this->addChild(m_tipLoadingText,10001,10001);
    if (CCCommonUtils::isIosAndroidPad()) {
        m_tipLoadingText->setPosition(725, 50);
    }
    else
        m_tipLoadingText->setPosition(290, 20);
    m_tipLoadingText->setVisible(false);
    setData(m_dialogInfo,m_mailInfo, m_idx);
    return ret;
}

void MailHeiqishiCell::setData(MailHeiqishiInfo* dialogInfo,MailHeiqishiCellInfo* mailInfo,int index)
{
    m_dialogInfo=dialogInfo;
    m_mailInfo = mailInfo;
    m_idx = index;
    if(m_dialogInfo->type==-3){
        m_tipLoadingText->setVisible(true);
        m_ccbNode->setVisible(false);
        if (CCCommonUtils::isIosAndroidPad()) {
            this->setContentSize(CCSizeMake(1464, 120));
        }
        else
            this->setContentSize(CCSizeMake(614, 50));
        return;
    }
    m_ccbNode->setVisible(true);
    m_tipLoadingText->setVisible(false);
    ownerUid = GlobalData::shared()->playerInfo.uid;
    m_timeText->setFntFile("Arial_Bold_Regular.fnt");
    m_nameText->setFntFile("Arial_Bold_Regular.fnt");
    m_posTxt->setFntFile("Arial_Bold_Regular.fnt");
    m_numText->setFntFile("Arial_Bold_Regular.fnt");
    m_pointTitleText->setFntFile("Arial_Bold_Regular.fnt");
    m_pointText->setFntFile("Arial_Bold_Regular.fnt");
    m_numTitleText->setFntFile("Arial_Bold_Regular.fnt");
    m_powerText->setFntFile("Arial_Bold_Regular.fnt");
    m_powerTitleText->setFntFile("Arial_Bold_Regular.fnt");
    m_woundedText->setFntFile("Arial_Bold_Regular.fnt");
    m_woundedTitleText->setFntFile("Arial_Bold_Regular.fnt");
    m_posTxt1->setFntFile("Arial_Bold_Regular.fnt");
    m_totalTitleText->setFntFile("Arial_Bold_Regular.fnt");
    m_totalText->setFntFile("Arial_Bold_Regular.fnt");
    m_killTotalText->setFntFile("Arial_Bold_Regular.fnt");
    m_killText->setFntFile("Arial_Bold_Regular.fnt");
    m_perTitleText->setFntFile("Arial_Bold_Regular.fnt");
    m_perText->setFntFile("Arial_Bold_Regular.fnt");
    m_tipText->setFntFile("Arial_Bold_Regular.fnt");
    m_timeText->setString(CCCommonUtils::timeStampToDate(m_dialogInfo->createTime).c_str());
    
    HeiqishiResult result = getReportResult();
    if (result == HWIN) {
        m_titleText->setString(_lang("105117"));
        m_titleText->setColor(ccBLACK);
        m_timeText->setColor(ccBLACK);
    }
    else if (result == HLOOSE) {
        m_titleText->setString(_lang("105118"));
    }
    /////玩家信息
    int dead = 0;
    int num = 0;
    int hurt = 0;
    int kill = 0;
    int total = 0;
    if (m_dialogInfo->defUser) {
        std::string nameStr = "";
        if(m_dialogInfo->defUser->objectForKey("alliance")){
            std::string allianceStr =m_dialogInfo->defUser->valueForKey("alliance")->getCString();
            nameStr.append("("+allianceStr+")");
        }
        if (m_dialogInfo->defUser->objectForKey("name")) {
            nameStr.append(m_dialogInfo->defUser->valueForKey("name")->getCString());
        }
        m_nameText->setString(nameStr);
        int pointId1 = 0;
        if (m_dialogInfo->defUser->objectForKey("pointId")) {
            pointId1 = m_dialogInfo->defUser->valueForKey("pointId")->intValue();
        }
        CCPoint pt1 = WorldController::getPointByIndex(pointId1);
        m_posTxt->setString(_lang_2("105521", CC_ITOA(pt1.x), CC_ITOA(pt1.y)));
        string picstr = "";
        int picVer = 0;
        string uid = "";
        if (m_dialogInfo->defUser->objectForKey("pic")) {
            picstr = m_dialogInfo->defUser->valueForKey("pic")->getCString();
        }
        if (m_dialogInfo->defUser->objectForKey("picVer")) {
            picVer = m_dialogInfo->defUser->valueForKey("picVer")->intValue();
        }
        if (m_dialogInfo->defUser->objectForKey("uid")) {
            uid = m_dialogInfo->defUser->valueForKey("uid")->getCString();
        }
        if(picstr==""){
            picstr ="g044.png";
        }else{
            picstr+=".png";
        }
        auto pic1 = CCLoadSprite::createSprite(picstr.c_str());
        CCCommonUtils::setSpriteMaxSize(pic1, 70);
        m_picNode->removeAllChildren();
        ////////融合底图
        auto sizeLayer = CCSize(80, 80);
        m_selfModelLayer = CCRenderTexture::create(sizeLayer.width, sizeLayer.height);
        m_selfModelLayer->setAnchorPoint(ccp(0.5, 0.5));
        ccBlendFunc cbf = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA};
        auto spr = CCLoadSprite::createSprite("Mail_headBack.png");
        spr->setScale(1);
        spr->setPosition(ccp(sizeLayer.width / 2, sizeLayer.height / 2));
        auto bgCircle = CCLoadSprite::createSprite("Mail_head_backBattle.png");
        bgCircle->setBlendFunc(cbf);
        bgCircle->setPosition(ccp(sizeLayer.width / 2, sizeLayer.height / 2));
        pic1->setPosition(ccp(sizeLayer.width / 2, sizeLayer.height / 2 - 3));
        m_selfModelLayer->begin();
        spr->visit();
        pic1->visit();
        bgCircle->visit();
        m_selfModelLayer->end();
        m_picNode->addChild(m_selfModelLayer);
        if (CCCommonUtils::isUseCustomPic(picVer) && uid != "")
        {
            m_headImgNode = HFHeadImgNode::create();
            string backImg = "Mail_headBack.png";
            string renderImg = "Mail_head_backBattle.png";
//            m_headImgNode->initHeadImgUrl3(m_picNode, CCCommonUtils::getCustomPicUrl(uid, picVer), 1.0f, 74, true, ccp(sizeLayer.width / 2, sizeLayer.height / 2 - 2), sizeLayer, backImg, renderImg);  simon
        }
        if(m_dialogInfo->defReport){
            int count = m_dialogInfo->defReport->count();
            for (int i=0; i<count; i++) {
                CCDictionary* dic = _dict(m_dialogInfo->defReport->objectAtIndex(i));
                if(dic){
                    dead += dic->valueForKey("dead")->intValue();
                    num += dic->valueForKey("num")->intValue();
                    hurt += dic->valueForKey("hurt")->intValue();
                    kill += dic->valueForKey("kill")->intValue();
                }
            }
        }
        m_powerTitleText->setString(_lang("133101").c_str());
        m_woundedTitleText->setString(_lang("105545").c_str());
        m_numTitleText->setString(_lang("105544").c_str());
        m_pointTitleText->setString(_lang("133114").c_str());
        m_powerText->setString(CC_CMDITOA(-m_dialogInfo->defPowerLost));
        m_woundedText->setString(CC_CMDITOA(hurt));
        m_numText->setString(CC_CMDITOA(dead));
        m_pointText->setString(CC_CMDITOA(m_dialogInfo->userScore));
    }
    
    /////黑骑士信息
    if (m_dialogInfo->attUser) {
        string mid = "";
        if (m_dialogInfo->attUser->objectForKey("uid")) {
            mid = m_dialogInfo->attUser->valueForKey("uid")->getCString();
        }
        string temp = "";
        temp = CCCommonUtils::getNameById(mid);
        temp.append("  ");
        temp.append(_lang_1("102272", CC_ITOA(m_dialogInfo->monsterLevel)));
        m_rewardTitleText->setString(temp);
        string picstr = "tile_pop_icon21.png";
        int pointId2 = 0;
        if (m_dialogInfo->attUser->objectForKey("pointId")) {
            pointId2 = m_dialogInfo->attUser->valueForKey("pointId")->intValue();
        }
        CCPoint pt2 = WorldController::getPointByIndex(pointId2);
        m_posTxt1->setString(_lang_2("105521", CC_ITOA(pt2.x), CC_ITOA(pt2.y)));
        auto pic = CCLoadSprite::createSprite(picstr.c_str());
        CCCommonUtils::setSpriteMaxSize(pic, 70);
        int mType = 0;
        int color = 1;
        string monsterStr = CCCommonUtils::getPropById(mid, "monster");
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
        m_hqsPicNode->removeAllChildren();
        ////////融合底图
        auto sizeLayer = CCSize(80, 80);
        ccBlendFunc cbf = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA};
        m_selfModelLayer1 = CCRenderTexture::create(sizeLayer.width, sizeLayer.height);
        m_selfModelLayer1->setAnchorPoint(ccp(0.5, 0.5));
        auto spr = CCLoadSprite::createSprite("Mail_headBack.png");
        spr->setScale(1);
        spr->setPosition(ccp(sizeLayer.width / 2, sizeLayer.height / 2));
        auto bgCircle = CCLoadSprite::createSprite("Mail_head_backBattle.png");
        bgCircle->setBlendFunc(cbf);
        bgCircle->setPosition(ccp(sizeLayer.width / 2, sizeLayer.height / 2));
        pic->setPosition(ccp(sizeLayer.width / 2, sizeLayer.height / 2));
        m_selfModelLayer1->begin();
        spr->visit();
        auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(color).c_str());
        CCCommonUtils::setSpriteMaxSize(iconBg, 72,true);
        iconBg->setPosition(ccp(sizeLayer.width / 2, sizeLayer.height / 2 - 3));
        iconBg->visit();
        if(mType!=2){
            auto preBg = CCLoadSprite::createSprite(CCString::createWithFormat("Activty_icon_BG%d.png",mType)->getCString());
            CCCommonUtils::setSpriteMaxSize(preBg, 72,true);
            preBg->setPosition(ccp(sizeLayer.width / 2, sizeLayer.height / 2 - 3));
            preBg->visit();
        }
        pic->visit();
        bgCircle->visit();
        m_selfModelLayer1->end();
        m_hqsPicNode->addChild(m_selfModelLayer1);
    }
    
    dead = 0;
    num = 0;
    hurt = 0;
    kill = 0;
    total = 0;
    string percentStr = CC_ITOA(m_dialogInfo->winPercent);
    percentStr.append("%");
    string tipStr = _lang_1("133093", percentStr.c_str());
    if (m_dialogInfo->failTimes>0) {
        tipStr.append("\n");
        tipStr.append(_lang_1("133057",CC_ITOA(m_dialogInfo->failTimes)));
        m_tipText->setString(tipStr);
    }else{
        m_tipText->setString(tipStr);
    }
    
    if(m_dialogInfo->attReport){
        int count = m_dialogInfo->attReport->count();
        for (int i=0; i<count; i++) {
            CCDictionary* dic = _dict(m_dialogInfo->attReport->objectAtIndex(i));
            if(dic){
                dead += dic->valueForKey("dead")->intValue();
                num += dic->valueForKey("num")->intValue();
                hurt += dic->valueForKey("hurt")->intValue();
                kill += dic->valueForKey("kill")->intValue();
            }
        }
    }
    total = dead+num+hurt;
    m_totalTitleText->setString(_lang("133096").c_str());
    m_killTotalText->setString(_lang("133097").c_str());
    m_perTitleText->setString(_lang("133098").c_str());
    
    m_totalText->setString(CC_CMDITOA(total));
    m_killText->setString(CC_CMDITOA(dead));
    double percent = dead*1.0/total;
    m_perText->setString(CC_ITOA_Percent(percent));
    
    m_posBG->setContentSize(CCSize(m_posTxt->getContentSize().width*m_posTxt->getOriginScaleX(), m_posBG->getContentSize().height));
    m_underlineSpr->setContentSize(CCSize(m_posTxt->getContentSize().width*m_posTxt->getOriginScaleX(), m_underlineSpr->getContentSize().height));
    m_posBG1->setContentSize(CCSize(m_posTxt1->getContentSize().width*m_posTxt1->getOriginScaleX(), m_posBG1->getContentSize().height));
    m_underlineSpr1->setContentSize(CCSize(m_posTxt1->getContentSize().width*m_posTxt1->getOriginScaleX(), m_underlineSpr1->getContentSize().height));
}

void MailHeiqishiCell::onEnter(){
    CCNode::onEnter();
    
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, false);
}

void MailHeiqishiCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

SEL_CCControlHandler MailHeiqishiCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    
    return NULL;
}

bool MailHeiqishiCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalNode", CCNode*, this->m_totalNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, this->m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_underlineSpr", CCScale9Sprite*, this->m_underlineSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_posBG", CCNode*, this->m_posBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_underlineSpr1", CCScale9Sprite*, this->m_underlineSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_posBG1", CCNode*, this->m_posBG1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_posTxt", CCLabelIF*, this->m_posTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_posTxt1", CCLabelIF*, this->m_posTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeText", CCLabelIF*, this->m_timeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipText", CCLabelIF*, this->m_tipText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText", CCLabelIF*, this->m_nameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardTitleText", CCLabelIF*, this->m_rewardTitleText);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerTitleText", CCLabelIF*, this->m_powerTitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerText", CCLabelIF*, this->m_powerText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woundedTitleText", CCLabelIF*, this->m_woundedTitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woundedText", CCLabelIF*, this->m_woundedText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numTitleText", CCLabelIF*, this->m_numTitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numText", CCLabelIF*, this->m_numText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pointTitleText", CCLabelIF*, this->m_pointTitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pointText", CCLabelIF*, this->m_pointText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalTitleText", CCLabelIF*, this->m_totalTitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalText", CCLabelIF*, this->m_totalText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_killTotalText", CCLabelIF*, this->m_killTotalText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_killText", CCLabelIF*, this->m_killText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_perTitleText", CCLabelIF*, this->m_perTitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_perText", CCLabelIF*, this->m_perText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hqsPicNode", CCNode*, this->m_hqsPicNode);
    return false;
}


bool MailHeiqishiCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    m_touchPoint = pTouch->getLocation();
    if(isTouchInside(m_posBG, pTouch)){
        return true;
    }
    if(isTouchInside(m_posBG1, pTouch)){
        return true;
    }
    return false;
}

void MailHeiqishiCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if (fabs( pTouch->getLocation().y - m_touchPoint.y) > 10) {
        return;
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::stopReturnToChat();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ChatServiceCocos2dx::stopReturnToChat();
#endif
    if (isTouchInside(m_posBG, pTouch)) {
        int pos = m_dialogInfo->defUser->valueForKey("pointId")->intValue();
        WorldController::getInstance()->openTargetIndex = pos;
        CCPoint pt = WorldController::getPointByIndex(pos);
        if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
            WorldMapView::instance()->gotoTilePoint(pt);
        }else{
            int index = WorldController::getIndexByPoint(pt);
            SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, index);
        }
        //zym 2015.12.11 simon
//        PopupViewController::getInstance()->forceClearAll(true);
        PopupViewController::getInstance()->removeAllPopupView();
    }
    if (isTouchInside(m_posBG1, pTouch)) {
        int pos = m_dialogInfo->attUser->valueForKey("pointId")->intValue();
        WorldController::getInstance()->openTargetIndex = pos;
        CCPoint pt = WorldController::getPointByIndex(pos);
        if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
            WorldMapView::instance()->gotoTilePoint(pt);
        }else{
            int index = WorldController::getIndexByPoint(pt);
            SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, index);
        }
        //zym 2015.12.11
//        PopupViewController::getInstance()->forceClearAll(true); simon
        PopupViewController::getInstance()->removeAllPopupView();
    }
}

void MailHeiqishiCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}

HeiqishiResult MailHeiqishiCell::getReportResult(){
    if(m_dialogInfo->winner == ownerUid){
        return HWIN;
    }
    isatkHelp = false;
    if(m_dialogInfo->atkHelper!=NULL){
        cocos2d::CCObject* obj;
        CCARRAY_FOREACH(m_dialogInfo->atkHelper, obj){
            string helpUid = dynamic_cast<CCString*>(obj)->getCString();
            if(helpUid==ownerUid){
                isatkHelp = true;
                break;
            }
        }
    }
    attUid = "";
    if (m_dialogInfo->attUser && m_dialogInfo->attUser->objectForKey("uid")) {
        attUid = m_dialogInfo->attUser->valueForKey("uid")->getCString();
    }
    defUid = "";
    if (m_dialogInfo->defUser && m_dialogInfo->defUser->objectForKey("uid")) {
        defUid = m_dialogInfo->defUser->valueForKey("uid")->getCString();
    }
    
    if(attUid==ownerUid){
        isatkHelp = true;
    }
    if(m_dialogInfo->atkGen == NULL && m_dialogInfo->battleType!=6){
        isatkHelp = true;
    }
    if(isatkHelp){
        if(attUid==m_dialogInfo->winner)
            return HWIN;
        else
            return HLOOSE;
        
    }else{
        if(defUid==m_dialogInfo->winner)
            return HWIN;
        else
            return HLOOSE;
    }
    return HLOOSE;
}
