//
//  MailResourcePopUpView.cpp
//  IF
//
//  Created by lifangkai on 14-10-9.
//
//

#include "MailResourcePopUpView.h"
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
#include "ChatServiceCocos2dx.h"

MailResourcePopUpView* MailResourcePopUpView::create(MailResourceCellInfo* info ){
    MailResourcePopUpView* ret = new MailResourcePopUpView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
MailResourcePopUpView::~MailResourcePopUpView(){
//    if(m_mailInfo&&m_mailInfo->collect->count()==0){
//        map<string, MailInfo*>::iterator it;
//        it = GlobalData::shared()->mailList.find(m_mailInfo->uid);
//        if (GlobalData::shared()->mailList.end() != it) {
//            MailInfo* mail = it->second;
//            mail->release();
//            GlobalData::shared()->mailList.erase(it);
//        }
//    }
}
bool MailResourcePopUpView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
//    CCLoadSprite::doResourceByCommonIndex(6, true);
    setCleanFunction([](){
//        CCLoadSprite::doResourceByCommonIndex(6, false);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
    });
    auto tmpCCB = CCBLoadFile("MailResourceCCB",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else {
        this->setContentSize(tmpCCB->getContentSize());
        int preHeight = this->m_buildBG->getContentSize().height;
        changeBGHeight(m_buildBG);
        int dh = m_buildBG->getContentSize().height - preHeight;
        this->m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height + dh));
        m_bg->setContentSize(CCSize(m_bg->getContentSize().width, m_bg->getContentSize().height + dh));
        m_downNode->setPositionY(m_downNode->getPositionY() - dh);

    }
    setTitleName(_lang("105513"));
    m_titleText->setString(_lang("105516").c_str());

    auto battlePic = CCLoadSprite::createSprite("Mail_caiji.png");
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
void MailResourcePopUpView::refresh(CCObject* obj){
    if(m_mailInfo->save ==0){
        m_unSaveBtn->setVisible(true);
        m_addSaveBtn->setVisible(false);
    }else{
        m_unSaveBtn->setVisible(false);
        m_addSaveBtn->setVisible(true);
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
void MailResourcePopUpView::readDialogContent(){
    if(m_mailInfo->unread>0){
        MailDialogReadCommand* cmd = new MailDialogReadCommand("",MAIL_RESOURCE);
        cmd->sendAndRelease();
        GlobalData::shared()->mailCountObj.sysR -=m_mailInfo->unread;
        m_mailInfo->unread = 0;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
    }
}
void MailResourcePopUpView::onEnter()
{
    CCNode::onEnter();
    setTitleName(_lang("105513"));
//    setTitleName(m_mailInfo->fromName);

    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailResourcePopUpView::refresh), MAIL_RESOURCE_LIST_CHANGE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailResourcePopUpView::refresh), MAIL_LIST_DELETE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailResourcePopUpView::refreshAddList), MAIL_LIST_ADD, NULL);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -2, false);
}

void MailResourcePopUpView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_RESOURCE_LIST_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_LIST_DELETE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_LIST_ADD);
    
    setTouchEnabled(false);
    readDialogContent();
    
    PopupBaseView::onExit();
}

bool MailResourcePopUpView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_tabView->setBounceable(true);
    if(m_isLoadMore)
        return true;
    else
        return false;
}
void MailResourcePopUpView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
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
        auto cell = dynamic_cast<MailResourceCell*>(m_tabView->cellAtIndex(m_data->count()-1));
        if(cell==NULL){
            return;
        }
        CCLabelIF* txt = (CCLabelIF*)cell->getChildByTag(10001);
        txt->setVisible(true);
        if(offsetY>30&&offsetY>m_currMinOffsetY)
            txt->setString(_lang("105318").c_str());
        else
            txt->setString(_lang("115189").c_str());
    }
    return;
    
}
void MailResourcePopUpView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(m_isLoadMore){
        m_currMinOffsetY = m_tabView->minContainerOffset().y;
        m_currOffsetY = m_tabView->getContentOffset().y;
        int dy = m_currMinOffsetY+30;
        if(m_currOffsetY >30&&m_currOffsetY>dy){
            addLoadingAni();
            m_tabView->setBounceable(false);
            this->scheduleOnce(schedule_selector(MailResourcePopUpView::sendReloadMoreMail),0.0);
        }
    }
}
void MailResourcePopUpView::sendReloadMoreMail(float _time){
    int realcount = m_mailInfo->collect->count();
    addLoadingAni();
    MailController::getInstance()->reloadMailMore(5,realcount,20);
}
void MailResourcePopUpView::removeLoadingAni(){
    if(m_loadingIcon){
        m_infoList->removeChild(m_loadingIcon);
        m_loadingIcon = NULL;
    }
}
void MailResourcePopUpView::addLoadingAni(){
    if(m_loadingIcon){
        m_infoList->removeChild(m_loadingIcon);
        m_loadingIcon = NULL;
    }
    m_loadingIcon = CCLoadSprite::createSprite("loading_1.png");
    if (CCCommonUtils::isIosAndroidPad()) {
        m_loadingIcon->setScale(2.4);
    }
    m_loadingIcon->setAnchorPoint(ccp(0.5, 0.5));
    auto cSize = m_infoList->getContentSize();
    m_loadingIcon->setPosition(ccp(cSize.width/2,cSize.height/2));
    
    CCActionInterval * rotateto1 = CCRotateTo::create(0.5, 720);
    CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
    CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
    m_loadingIcon->runAction(rotateForever);
    m_infoList->addChild(m_loadingIcon,1000000);
}
void MailResourcePopUpView::closeInputRefeshList(){
    
}
void MailResourcePopUpView::refreshAddList(cocos2d::CCObject *obj){
    
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
SEL_CCControlHandler MailResourcePopUpView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddSaveClick", MailResourcePopUpView::onAddSaveClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDeleteClick", MailResourcePopUpView::onDeleteClick);
    return NULL;
}

bool MailResourcePopUpView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downNode", CCNode*, this->m_downNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_unSaveBtn", CCControlButton*, this->m_unSaveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addSaveBtn", CCControlButton*, this->m_addSaveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_battlePicNode", CCNode*, m_battlePicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_deleteBtn", CCControlButton*, this->m_deleteBtn);
    return false;
}
void MailResourcePopUpView::onDeleteClick(cocos2d::CCObject *pSender, CCControlEvent event){
    map<string, MailInfo*>::iterator it;
    it = GlobalData::shared()->mailList.find(m_mailInfo->uid);
    if (GlobalData::shared()->mailList.end() == it) {
        CCLOGFUNC("");
        return;
    }
    MailResourceCellInfo* tempInfo = dynamic_cast<MailResourceCellInfo*>(it->second);
    MailDialogDeleteCommand* command = new MailDialogDeleteCommand("",MAIL_RESOURCE);
    command->sendAndRelease();

    GlobalData::shared()->mailCountObj.sysT -=1;
    GlobalData::shared()->mailCountObj.sysR -=tempInfo->unread;
    GlobalData::shared()->mailList.erase(it);
    tempInfo->unread = 0;
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
    PopupViewController::getInstance()->goBackPopupView();
    CC_SAFE_RELEASE(tempInfo);
    
}
void MailResourcePopUpView::onAddSaveClick(cocos2d::CCObject *pSender, CCControlEvent event){
    if(m_mailInfo->collect->count()==0)
        return;
    if(m_mailInfo->save ==1){
        MailDialogSaveCommand *cmd = new MailDialogSaveCommand("", 0,MAIL_RESOURCE);
        cmd->sendAndRelease();
        m_mailInfo->save = 0;
        CCCommonUtils::flyHint("quest_icon_1.png", "", _lang("105576"));
        m_unSaveBtn->setVisible(true);
        m_addSaveBtn->setVisible(false);
    }else{
        
        MailDialogSaveCommand *cmd = new MailDialogSaveCommand("", 1,MAIL_RESOURCE);
        cmd->sendAndRelease();
        m_mailInfo->save = 1;
        CCCommonUtils::flyHint("quest_icon_1.png", "", _lang("105575"));
        m_unSaveBtn->setVisible(false);
        m_addSaveBtn->setVisible(true);
    }
    
}
void MailResourcePopUpView::onPostBtnClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent){


}
void MailResourcePopUpView::setData(){
    m_data->removeAllObjects();
    if(m_mailInfo->collect->count()<m_mailInfo->totalNum){
        m_isLoadMore = true;
    }else{
        m_isLoadMore = false;
    }

    CCObject* obj;
    CCARRAY_FOREACH(m_mailInfo->collect, obj){
        m_data->addObject(obj);
    }
    if(m_isLoadMore){
        MailResourceInfo* info = MailResourceInfo::create();
        info->type = 3;
        m_data->addObject(info);
    }
}

void MailResourcePopUpView::scrollViewDidScroll(CCScrollView* view)
{
    
}

void MailResourcePopUpView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    ChatCell* tmpCell = dynamic_cast<ChatCell*>(cell);
    if (tmpCell) {
        //        tmpCell->cellTouchEnded(m_tabView->m_pTouchedCell);
    }
}
CCSize MailResourcePopUpView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    int height = 145;
    if(m_isLoadMore && idx==m_data->count()-1){
        height = 50;
    }
    if(idx < m_data->count()){
        MailResourceInfo* dialogInfo = dynamic_cast<MailResourceInfo*>(m_data->objectAtIndex(idx));
        if(dialogInfo){
            int add = (dialogInfo->drop && dialogInfo->drop->count()>0)?  dialogInfo->drop->count(): 0;
            height = 145 + add * 55;
            if (dialogInfo->reward == NULL) {
                height += 55;
            }
        }
    }
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1450, height*2.4);
    }
    return CCSize(604,height);
}

//cocos2d::CCSize MailResourcePopUpView::cellSizeForTable(CCTableView *table)
//{
//    return CCSize(580, 980);
//}

CCTableViewCell* MailResourcePopUpView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    
    //    if (type == CHAT_COUNTRY && idx >= ChatController::getInstance()->m_chatCountryPool.size()) {
    //        return NULL;
    //    }
    //    else if ( type == CHAT_ALLIANCE && idx >= ChatController::getInstance()->m_chatAlliancePool.size()) {
    //        return NULL;
    //    }
    if(idx>=m_data->count()){
        return NULL;
    }
    MailResourceInfo* dialogInfo = dynamic_cast<MailResourceInfo*>(m_data->objectAtIndex(idx));
    MailResourceCell* cell = (MailResourceCell*)table->dequeueCell();
    if(cell){
        cell->setData(dialogInfo,m_mailInfo,idx);
    }else{
        cell = MailResourceCell::create(dialogInfo,m_mailInfo,idx);
    }
    return cell;
}

ssize_t MailResourcePopUpView::numberOfCellsInTableView(CCTableView *table){
    int cellNum = 0;
    cellNum = m_data->count();
    return cellNum;
}


MailResourceCell* MailResourceCell::create(MailResourceInfo* dialogInfo,MailResourceCellInfo* mailInfo,int index){
    MailResourceCell* ret = new MailResourceCell(dialogInfo,mailInfo,index);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MailResourceCell::init()
{
    bool ret = true;
    m_ccbNode = CCBLoadFile("MailResourceCell",this,this);
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
void MailResourceCell::setCellStatus(bool isHint){

    
}
void MailResourceCell::setData(MailResourceInfo* dialogInfo,MailResourceCellInfo* mailInfo,int index)
{
    m_dialogInfo=dialogInfo;
    m_mailInfo = mailInfo;
    m_idx = index;
    m_deleteBtn->setVisible(false);
    if(m_dialogInfo->type==3){
        m_tipLoadingText->setVisible(true);
        m_ccbNode->setVisible(false);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_rewardBG->setContentSize(CCSize(1450,324));
        }
        else
            m_rewardBG->setContentSize(CCSize(604,135));
        m_node1->setPositionY(0);
        m_node2->setPositionY(0);
        if (CCCommonUtils::isIosAndroidPad()) {
            this->setContentSize(CCSizeMake(1450, 324));
        }
        else
            this->setContentSize(CCSizeMake(604, 135));
        return;
    }
    m_ccbNode->setVisible(true);
    m_tipLoadingText->setVisible(false);
    m_PicNode->removeAllChildren();
    int offsetY = 0;
    if (dialogInfo->reward==NULL) {
        m_nameText->setVisible(false);
        std::string path = "mail_pic_flag_5.png";
        auto icon = CCLoadSprite::createSprite(path.c_str());
        float scale = 1.0;
        scale =94/icon->getContentSize().width;
        scale = scale>1.0?1.0:scale;
        icon->setScale(scale);
        this->m_PicNode->addChild(icon);
        std::string tipStr = _lang("108896");
        CCLabelIF *label = CCLabelIF::create("", 24);
        label->setColor(ccc3(125, 98, 63));
        label->setDimensions(CCSize(450,70));
        label->setAlignment(kCCTextAlignmentLeft);
        label->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        label->setString(tipStr.c_str());
        label->setAnchorPoint(ccp(0, 0.5));
        label->setPosition(50, icon->getPositionY());
        m_PicNode->addChild(label);
        m_rewardBG->setContentSize(CCSize(604,200));
        m_node1->setPositionY(200-135);
        m_node2->setPositionY(200-135);
        this->setContentSize(CCSize(604,200));
        m_PicNode->setPositionY(m_PicNode->getPositionY() - 27.5);
        //offsetY -= (icon->getContentSize().height + 5 );
    }
    else{
        int count =dialogInfo->reward->count();
        if(count> 0){
            auto dict = _dict(dialogInfo->reward->objectAtIndex(0));
            int type = dict->valueForKey("t")->intValue();
            if (dialogInfo->level == -1) {//超级矿
                std::string nameStr = "";
                switch (type) {
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
                m_nameText->setString(nameStr.c_str());
            }
            else {
                std::string nameStr = string("Lv.") + CC_ITOA(dialogInfo->level) + " " + CCCommonUtils::getResourceProductByType(type);
                m_nameText->setString(nameStr.c_str());
            }
            
            for (int i = 0;i<count; i++) {
                CCDictionary *dict = dynamic_cast<CCDictionary*>(dialogInfo->reward->objectAtIndex(i));
                int type = dict->valueForKey("t")->intValue();
                int value = dict->valueForKey("v")->intValue();
                auto icon = CCLoadSprite::createSprite(RewardController::getInstance()->getPicByType(type, value).c_str());
                this->m_PicNode->addChild(icon);
                std::string  pName= RewardController::getInstance()->getNameByType(type);
                std:: string numstr = " " + pName + " + ";
                numstr.append(CC_CMDITOA(value));
                CCLabelIF *label = CCLabelIF::create("", 24);
                label->setColor(ccc3(125, 98, 63));
                label->setString(numstr.c_str());
                label->setAnchorPoint(ccp(0, 0.5));
                label->setPosition(50, icon->getPositionY());
                m_PicNode->addChild(label);
                offsetY -= (icon->getContentSize().height + 5 );
            }
        }
        if(dialogInfo->drop){
            int dropCount = dialogInfo->drop->count();
            if(dropCount>0){
                float tmpH = 135;
                int i = 0;
                while (i < dropCount) {
                    auto dic = dynamic_cast<CCDictionary*>(dialogInfo->drop->objectAtIndex(i));
                    int type = dic->valueForKey("type")->intValue();
                    if(type == R_GOODS){
                        auto itemDict = dynamic_cast<CCDictionary*>(dic->objectForKey("value"));
                        int itemid = itemDict->valueForKey("itemId")->intValue();
                        int value = itemDict->valueForKey("rewardAdd")->intValue();
                        auto &tool = ToolController::getInstance()->getToolInfoById(itemid);
                        auto pic = CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(tool.itemId)).c_str(),true,CCLoadSpriteType_GOODS);
                        int addH = 0;
                        if (pic) {
                            auto iconBg1 = CCLoadSprite::createSprite("icon_kuang.png");
                            CCCommonUtils::setSpriteMaxSize(iconBg1, 50,true);
                            this->m_PicNode->addChild(iconBg1);
                            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(tool.color).c_str());
                            CCCommonUtils::setSpriteMaxSize(iconBg, 45,true);
                            this->m_PicNode->addChild(iconBg);
                            CCCommonUtils::setSpriteMaxSize(pic, 45,true);
                            this->m_PicNode->addChild(pic);
                            pic->setPositionY(offsetY);
                            iconBg->setPositionY(offsetY);
                            iconBg1->setPositionY(offsetY);
                            addH = 50 + 5;
                            offsetY -= addH;
                            std::string str =" "  + tool.getName() + std::string(" + ") + CC_ITOA(value);
                            CCLabelIF *label = CCLabelIF::create("", 24);
                            label->setColor(ccc3(125, 98, 63));
                            label->setString(str.c_str());
                            label->setAnchorPoint(ccp(0, 0.5));
                            label->setPosition(50, pic->getPositionY());
                            m_PicNode->addChild(label);
                            tmpH += addH;
                        }
                    }
                    i++;
                }
                m_rewardBG->setContentSize(CCSize(604,tmpH));
                m_node1->setPositionY(tmpH-135);
                m_node2->setPositionY(tmpH-135);
                this->setContentSize(CCSize(604,tmpH));
            }else{
                m_rewardBG->setContentSize(CCSize(604,135));
                m_node1->setPositionY(0);
                m_node2->setPositionY(0);
                this->setContentSize(CCSizeMake(604, 135));
            }
        }else{
            m_rewardBG->setContentSize(CCSize(604,135));
            m_node1->setPositionY(0);
            m_node2->setPositionY(0);
            this->setContentSize(CCSizeMake(604, 135));
        }
    }
    CCPoint pos = WorldController::getPointByIndex(dialogInfo->pointId);
    std::string strPos = _lang_2("105521", CC_ITOA(pos.x), CC_ITOA(pos.y));
//    strPos.append(CC_ITOA(pos.x));
//    strPos.append(",");
//    strPos.append(CC_ITOA(pos.y));
    m_posTxt->setString(strPos.c_str());
    time_t timeT = dialogInfo->createTime;
    string timestr = CCCommonUtils::timeStampToDate(timeT);
    m_timeText->setString(timestr);
    m_posBG->setContentSize(CCSize(m_posTxt->getContentSize().width*m_posTxt->getOriginScaleX(), m_posBG->getContentSize().height));
    m_underlineSpr->setContentSize(CCSize(m_posTxt->getContentSize().width*m_posTxt->getOriginScaleX(), m_underlineSpr->getContentSize().height));
    //m_posBG->setPositionX(m_nameText->getPositionX()+m_nameText->getContentSize().width+10);
}

void MailResourceCell::onEnter(){
    CCNode::onEnter();
    
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, false);
}

void MailResourceCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

void MailResourceCell::onShieldClick(cocos2d::CCObject *pTarget, CCControlEvent touchEvent){
    if(GlobalData::shared()->chatShieldInfo.isReachLimit()){
        CCCommonUtils::flyText(_lang("105314"));
        return;
    }
    YesNoDialog::show(_lang("105313").c_str(), CCCallFunc::create(this, callfunc_selector(MailResourceCell::onYesClick)));
}

void MailResourceCell::onYesClick(){
    //    ChatLockCommand* cmd = new ChatLockCommand(_uid);
    //    cmd->sendAndRelease();
}

SEL_CCControlHandler MailResourceCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDeleteBtnClick", MailResourceCell::onDeleteBtnClick);
    return NULL;
}

bool MailResourceCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardBG", CCScale9Sprite*, this->m_rewardBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_PicNode", CCNode*, this->m_PicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_underlineSpr", CCScale9Sprite*, this->m_underlineSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_posBG", CCNode*, this->m_posBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText", CCLabelIF*, this->m_nameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_posTxt", CCLabelIF*, this->m_posTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeText", CCLabelIF*, this->m_timeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_deleteBtn", CCControlButton*, this->m_deleteBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node1", CCNode*, this->m_node1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node2", CCNode*, this->m_node2);
    
    return false;
}

void MailResourceCell::onDeleteBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if(m_mailInfo->save!=0){
        CCCommonUtils::flyHint("","",_lang("105575"));
        return;
    }
    MailController::getInstance()->removeOneMailResource(m_mailInfo->uid,m_dialogInfo->uid, CC_ITOA(m_dialogInfo->type));

}

void MailResourceCell::setTranslation(CCObject* msg){

}

bool MailResourceCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    m_touchPoint = pTouch->getLocation();
    if(m_dialogInfo->type==MAIL_RESOURCE&&isTouchInside(m_posBG, pTouch)){
        return true;
    }
    
    return false;
}

void MailResourceCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if (fabs( pTouch->getLocation().y - m_touchPoint.y) > 10) {
        return;
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::stopReturnToChat();
#endif
    int pos = m_dialogInfo->pointId;
    WorldController::getInstance()->openTargetIndex = pos;
    CCPoint pt = WorldController::getPointByIndex(pos);
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
        WorldMapView::instance()->gotoTilePoint(pt);
    }else{
        int index = WorldController::getIndexByPoint(pt);
        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, index);
    }
    PopupViewController::getInstance()->removeAllPopupView();
}
void MailResourceCell::openChatFun(){
    //  PopupViewController::getInstance()->addPopupView(ChatFunView::create(ChatController::getInstance()->idx, CHAT_COUNTRY), false);
}
void MailResourceCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}