//
//  MailSystemListPopUp.cpp
//  IF
//
//  Created by lifangkai on 14-7-28.
//
//

#include "MailSystemListPopUp.h"
#include "MailPopUpView.h"
#include "PopupViewController.h"
#include "CCSafeNotificationCenter.h"
#include "MailWritePopUpView.h"
#include "MailSystemCell.h"
#include "MailWritePopUpView.h"
#include "MailController.h"
#include "UIComponent.h"
#include "SettingPopUpView.h"
#include "YesNoDialog.h"
#include "DataRecordCommand.h"
//static int currentType = 0;

#define MAIL_SYS_VIEW_EDIT_BOX_HEIGHT 80
void MailSystemListPopUp::onEnter(){
    //CCLoadSprite::doResourceByCommonIndex(6, true);
    PopupBaseView::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailSystemListPopUp::refreshView), MAIL_SAVE_LIST_CHANGE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailSystemListPopUp::refreshAddList), MAIL_LIST_ADD, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailSystemListPopUp::onCheckBoxClick), CHECK_BOX_SELECT, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailSystemListPopUp::onCheckBoxClick), CHECK_BOX_UNSELECT, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailSystemListPopUp::sendReloadMoreMail), MAIL_RELOAD_MORE, NULL);
  //  CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(MailSystemListPopUp::onEnterFrame), this, 1.0f,kCCRepeatForever, 0.0f, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailSystemListPopUp::deleteMailcellByIndex), MAIL_LIST_DELETE, NULL);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -3,false);
//    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailSystemListPopUp::onDeleteAllMail), MAIL_DEL_ALL_MSG, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailSystemListPopUp::onEditAllMail), MAIL_EDIT_MSG, NULL);
    if(m_panelType==2){
        setTitleName(_lang("105569"));
    }else if(m_panelType==3){
        setTitleName(_lang("103731"));
    }else if(m_panelType==4){
        setTitleName(_lang("105519"));
    }
    
    UIComponent::getInstance()->showPopupView(UIPopupViewType_Mail,true);
    if(!isInit){
        refreshView(NULL);
    }
    CCArray* p = CCArray::create();
    p->addObject(CCInteger::create(MAIL_OPEN));
    DataRecordCommand *cmd = new DataRecordCommand(OPEN_PANEL, p, CCArray::create());
    cmd->sendAndRelease();
    //setUnreadEffect();
}

void MailSystemListPopUp::onExit(){
    isInit = false;
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_LIST_ADD);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_SAVE_LIST_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, CHECK_BOX_SELECT);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, CHECK_BOX_UNSELECT);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_RELOAD_MORE);
   // CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(MailSystemListPopUp::onEnterFrame), this);
        CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_LIST_DELETE);
//    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_DEL_ALL_MSG);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_EDIT_MSG);
    setTouchEnabled(false);
    removeLoadingAni();
    if(m_nodeEdit->isVisible()){
        CCPoint offset = m_tabView->getContentOffset();
        m_tabView->setViewSize(m_listContainer->getContentSize());
        m_tabView->setPositionY(0);
        m_tabView->setContentOffset(CCPoint(offset.x, offset.y+MAIL_SYS_VIEW_EDIT_BOX_HEIGHT));
        m_nodeEdit->setVisible(false);
    }
    //resetMailDeleteFlag();
    PopupBaseView::onExit();
}
void MailSystemListPopUp::onEnterFrame(float dt){
    if(m_tabView){
        CCPoint pos;
        pos = m_tabView->getContentOffset();
 
        CCPoint minPt = m_tabView->minContainerOffset();
        CCPoint maxPt = m_tabView->maxContainerOffset();
        if (pos.y > 0) {
            //return;
        }
        if (pos.y > maxPt.y) {

        }

        if (pos.y >0) {//超过底部
            //pos.y = minPt.y;
            int count = m_data->count();
            MailSystemCell* wCell = dynamic_cast<MailSystemCell*>(m_tabView->cellAtIndex(count));
            if(wCell!=NULL){
                wCell->showLoadingMoreAnim();
            }
        }

    }
}
void MailSystemListPopUp::resetMailDeleteFlag(){
    map<string, MailInfo*>::iterator it;
    for(it = GlobalData::shared()->mailList.begin(); it != GlobalData::shared()->mailList.end(); it++){
        it->second->prepareToDelete = false;
    }
}

MailSystemListPopUp* MailSystemListPopUp::create(int panelType){
    MailSystemListPopUp* ret = new MailSystemListPopUp(panelType);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MailSystemListPopUp::init(){
    if(!PopupBaseView::init()){
        return false;
    }
    setIsHDPanel(true);
    isInit = true;
    m_addNum = 0;
    m_deleteY = 0;
     m_cellWith = 159;
    if (CCCommonUtils::isIosAndroidPad()) {
        m_cell_HD_Width = 309;
    }
    m_isChangeTab = false;
    CCLoadSprite::doResourceByCommonIndex(6, true); //simon
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(6, false); //simon
    });
    auto bg = CCBLoadFile("MailView", this, this);
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else {
        this->setContentSize(bg->getContentSize());
        
        int preHeight = this->m_buildBG->getContentSize().height;
        changeBGHeight(m_buildBG);
        int dh = m_buildBG->getContentSize().height - preHeight;
        this->m_bg->setContentSize(CCSize(m_bg->getContentSize().width, m_bg->getContentSize().height +dh));
        this->m_bg1->setContentSize(CCSize(m_bg1->getContentSize().width, m_bg1->getContentSize().height +dh));
        this->m_listContainer->setContentSize(CCSize(m_listContainer->getContentSize().width, m_listContainer->getContentSize().height + dh));
        this->m_downNode->setPositionY(m_downNode->getPositionY() - dh);

    }
    m_data = CCArray::create();
    m_data->retain();
    m_checkBox = CheckBox::create();
    m_checkBox->setVisible(false);
    m_checkBox->setBtnTouchPriority(-1);

    m_tabView = CCTableView::create(this, m_listContainer->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setDelegate(this);
    m_tabView->setTouchPriority(1);

    m_listContainer->addChild(m_tabView);
    m_BGTouchLayer = BGTouchLayer::create(m_listContainer->getContentSize());
    m_BGTouchLayer->setAnchorPoint(ccp(0.5, 1.0));
    m_BGNode->addChild(m_BGTouchLayer);
    m_BGTouchLayer->setPosition(this->m_bg->getPosition());
    MailController::getInstance()->CanRemoveMail = false;
    m_noMail->setString(_lang("105514").c_str());
    m_nodeEdit->setVisible(false);
//    m_lblAll->setString(_lang("102145").c_str());
//    m_lblDel->setString(_lang("108523").c_str());
//    m_lblReward->setString( _lang("101313").c_str());
//    m_lblWrite->setString(_lang("105048").c_str());
//    m_lblSave->setString(_lang("108501").c_str());
    resetMailDeleteFlag();
    refreshView(NULL);

    return true;
}
bool MailSystemListPopUp::getDataContent(){
    string allReadUids = "";
    string type = "";
    
    int total = m_data->count();
    int i = 0;
    while(i < total){
        auto &mail = GlobalData::shared()->mailList[dynamic_cast<CCString*>(m_data->objectAtIndex(i))->getCString()];
        if(mail->isReadContent==false){
            if(mail->uid!=""){
                allReadUids.append(mail->uid).append("|");
                type.append(CC_ITOA(mail->type)).append("|");
            }
        }
        i++;
    }
    bool isGetData = false;
    if(allReadUids != ""){
        isGetData = true;
        MailController::getInstance()->readMailFromServer(allReadUids,type);
    }
    return isGetData;
}
void MailSystemListPopUp::refreshAddList(cocos2d::CCObject *obj){
    m_tabView->setTouchStop(true);
    removeLoadingAni();
    int add = dynamic_cast<CCInteger*>(obj)->getValue();
    m_addNum = add;
    generateDataArr();
    if(m_addNum==0){
        m_tabView->reloadData();
        m_tabView->setContentOffset(ccp(0,0));
        return;
    }
    if(m_allSelected){
        m_allSelected=false;
        MailController::getInstance()->clearOpMails();
        onAllSelected(NULL,Control::EventType::TOUCH_DOWN);
    }
    m_tabView->reloadData();
    m_newMinOffsetY = m_tabView->minContainerOffset().y;
    if(m_newMinOffsetY<0&&m_currMinOffsetY<0){
        float dy = m_newMinOffsetY-m_currMinOffsetY+m_currOffsetY;
        if(dy>0){
            dy = 0;
        }
        m_tabView->setContentOffset(ccp(0,dy));
//        m_tabView->setContentOffset(ccp(0,m_newMinOffsetY-m_currMinOffsetY));
//        float movedy = m_newMinOffsetY-m_currMinOffsetY + 200;
//        if(movedy>0){
//            movedy = 0;
//        }
//        m_tabView->setContentOffsetInDuration(ccp(0,movedy), 0.7);
    }
    //refreshView();
    
}
void MailSystemListPopUp::deleteMailcellByIndex(cocos2d::CCObject *obj){
    int index = dynamic_cast<CCInteger*>(obj)->getValue();
    if (index<0) {
        return;
    }
    if (m_data->count() <= index) {
        return;
    }
    
    // CCTableViewCell * grid = m_pMultiTableViewDataSource->gridAtIndex(this,gridIdx);
    // MailCell* wCell = dynamic_cast<MailCell*>(this->gridAtIndex(m_tabView,index));
    // wCell->setDeleteStatusFlag();

    //  wCell->setDeleteStatusFlag(false);
    m_data->removeObjectAtIndex(index);
    int count = m_data->count();
    m_tabView->removeCellAtIndex(index);
    if(count<=0){
        PopupViewController::getInstance()->goBackPopupView();
        return;
    }
    m_deleteY=m_cellWith;
    // m_tabView->updateInset();
    refreshView(NULL);
    if(m_isLoadMore){
        m_currOffsetY = m_tabView->getContentOffset().y;
        if(m_currOffsetY > 30){
            addLoadingAni();
            m_tabView->setBounceable(false);
            this->scheduleOnce(schedule_selector(MailSystemListPopUp::sendReloadMoreMail),0.0);
        }
    }
    
}
void MailSystemListPopUp::refreshView(CCObject* obj){
    generateDataArr();
    m_noMail->setVisible(false);
    if(getDataContent()){
        addLoadingAni();
    }else{
        refreshDataList();
        removeLoadingAni();
    }
    
}
void MailSystemListPopUp::addLoadingAni(){
    if(m_loadingIcon){
        m_listContainer->removeChild(m_loadingIcon);
        m_loadingIcon = NULL;
    }
    m_loadingIcon = CCLoadSprite::createSprite("loading_1.png");
    m_loadingIcon->setAnchorPoint(ccp(0.5, 0.5));
    auto cSize = m_listContainer->getContentSize();
    m_loadingIcon->setPosition(ccp(cSize.width/2,cSize.height/2));
    
    CCActionInterval * rotateto1 = CCRotateTo::create(0.5, 720);
    CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
    CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
    m_loadingIcon->runAction(rotateForever);
    m_listContainer->addChild(m_loadingIcon,1000000);
}
void MailSystemListPopUp::removeLoadingAni(){
    if(m_loadingIcon){
        m_listContainer->removeChild(m_loadingIcon);
        m_loadingIcon = NULL;
    }
}
void MailSystemListPopUp::sendReloadMoreMail(cocos2d::CCObject *obj)
{
    sendReloadMoreMail(0.0);
}
void MailSystemListPopUp::sendReloadMoreMail(float _time){
    int count = m_data->count();
    int realcount = count;
    int type =m_panelType;
    if(m_panelType==4){
        type = 6;
    }
    addLoadingAni();
    if(m_panelType==MAILTAB5){
        type = 10;
    }
    MailController::getInstance()->reloadMailMore(type,realcount,20);
}
void MailSystemListPopUp::refreshDataList(){
    bool ishasChild = false;
    CCPoint pos;
    if(m_tabView->getContainer()->getChildrenCount()==0){
        ishasChild = true;
    }else{
        pos = m_tabView->getContentOffset();
    }
    //    m_data->addObjectsFromArray(arr);
    m_tabView->reloadData();
    if(ishasChild||m_isChangeTab){
        m_isChangeTab = false;
    }else{
        if (m_addNum>0) {
            pos.y = -m_addNum*m_cellWith;
            m_addNum = 0;
        }else if(m_deleteY>0){
            pos.y += m_cellWith;
            m_deleteY = 0;
        }
        CCPoint minPt = m_tabView->minContainerOffset();
        CCPoint maxPt = m_tabView->maxContainerOffset();
        if (pos.y > maxPt.y) {
            pos.y = maxPt.y;
        }
        
        if (pos.y < minPt.y) {
            pos.y = minPt.y;
        }
        m_tabView->setContentOffset(pos);
    }
    int num = m_data->count();
    int total = getMaxMailNum();
    m_mailNum->setString(_lang_2("105515", CC_ITOA(num), CC_ITOA(total)));
    m_mailNum->setString("");
    m_noMail->setVisible(false);
    if(m_panelType==USERMAIL){
        if(GlobalData::shared()->mailCountObj.perT<=0&&num<=0){
            m_noMail->setVisible(true);
        }
        
    }else if(m_panelType==SYSTEMMAIL){
        if(GlobalData::shared()->mailCountObj.sysT<=0&&num<=0){
            m_noMail->setVisible(true);
        }
    }else if(m_panelType==SAVEMAIL){
        if(GlobalData::shared()->mailCountObj.saveT<=0&&num<=0){
            m_noMail->setVisible(true);
        }
    }else{
        m_noMail->setVisible(false);
    }
}
void MailSystemListPopUp::setUnreadEffect(){
    map<std::string, MailInfo*>::iterator it;
    int userCount = GlobalData::shared()->mailCountObj.perR;
    int sysCount = GlobalData::shared()->mailCountObj.sysR;
    int saveCount = GlobalData::shared()->mailCountObj.saveR;
    
    if(userCount > 0){
      //  m_userUnreadNode->setVisible(true);
      //  m_userUnreadText->setString(CC_ITOA(userCount));
    }else{
      //  m_userUnreadNode->setVisible(false);
    }
    
    if(sysCount > 0){
     //   m_sysUnreadNode->setVisible(true);
     //   m_sysUnreadText->setString(CC_ITOA(sysCount));
    }else{
     //   m_sysUnreadNode->setVisible(false);
    }
    //m_mailBGFlag2->setVisible(false);
    if(saveCount > 0){
       // m_saveUnreadNode->setVisible(true);
      //  m_saveUnreadText->setString(CC_ITOA(saveCount));
        if(GlobalData::shared()->mailCountObj.upR>0){
         //   m_mailBGFlag2->setVisible(true);
        }
    }else{
       // m_saveUnreadNode->setVisible(false);
    }
}

int MailSystemListPopUp::getMaxMailNum(){
    if(m_panelType == USERMAIL){
        return GlobalData::shared()->mailConfig.userMailMax;
    }else if(m_panelType == SYSTEMMAIL){
        return GlobalData::shared()->mailConfig.sysMailMax;
    }else if(m_panelType == SAVEMAIL){
        return GlobalData::shared()->mailConfig.saveMailMax;
    }else if(m_panelType == MAILTAB3){
        return GlobalData::shared()->mailConfig.saveMailMax;
    }
    return 0;
}

void MailSystemListPopUp::generateDataArr(){
    m_data->removeAllObjects();
    CCArray *readData = CCArray::create();
    CCArray *unUpdateData = CCArray::create();
    map<std::string, MailInfo*>::iterator it;
    for(it = GlobalData::shared()->mailList.begin(); it != GlobalData::shared()->mailList.end(); it++){
        bool addFlag = false;
        if(it->second->tabType==4){
            if(m_panelType == MAILTAB4){
                addFlag = true;
            }
        }else if(it->second->tabType == MAILTAB5){
            if(m_panelType == MAILTAB5){
                addFlag = true;
            }
        }else if(it->second->type==ALL_SERVICE){
            if(m_panelType == MAILTAB3){
                addFlag = true;
            }
        }else if(it->second->type== MAIL_FRESHER||it->second->type==MAIL_SYSNOTICE||it->second->type==MAIL_SYSUPDATE){
            if(m_panelType == SAVEMAIL){
                addFlag = true;
            }
        }else if(it->second->tabType==USERMAIL){
            if(m_panelType == USERMAIL){
                addFlag = true;
            }
        }else{
            if(m_panelType == SYSTEMMAIL){
                addFlag = true;
            }
        }
        if(addFlag){
            if (it->second->type == MAIL_SYSUPDATE) {
                if(it->second->rewardId != "" && it->second->rewardStatus==0){
                    m_data->addObject(CCString::create(it->second->uid));
                }else{
                    readData->addObject(CCString::create(it->second->uid));
                }
            }
            else {
                if(m_panelType != MAILTAB4 && it->second->status == READ){
                    readData->addObject(CCString::create(it->second->uid));
                }else{
                    unUpdateData->addObject(CCString::create(it->second->uid));
                }
            }
        }
    }
    m_data = MailController::getInstance()->getSortMailByTime(m_data);
    if (unUpdateData->count()>0) {
        unUpdateData = MailController::getInstance()->getSortMailByTime(unUpdateData);
        m_data->addObjectsFromArray(unUpdateData);
    }
    if(readData->count()>0){
        readData = MailController::getInstance()->getSortMailByTime(readData);
        m_data->addObjectsFromArray(readData);
    }
//    sort();
    setIsLoadMoreFlag();
}

int  MailSystemListPopUp::getOpenType(){
    map<std::string, MailInfo*>::iterator it;
    int userCount = 0;
    int sysCount = 0;
    int saveCount = 0;
    for(it = GlobalData::shared()->mailList.begin(); it != GlobalData::shared()->mailList.end(); it++){
        if(it->second->status == UNREAD){
            if(it->second->type== MAIL_FRESHER||it->second->type==ALL_SERVICE||it->second->type==MAIL_SYSNOTICE||it->second->type==MAIL_SYSUPDATE){
                saveCount +=  1;
            }else if(it->second->tabType==USERMAIL){
                userCount++;
            }else{
                
                sysCount++;
            }
        }
    }
    if(userCount>0){
        return USERMAIL;
    }else if(sysCount>0){
        return SYSTEMMAIL;
    }else if(saveCount>0){
        return SAVEMAIL;
    }else{
        for(it = GlobalData::shared()->mailList.begin(); it != GlobalData::shared()->mailList.end(); it++){
            if(it->second->type== MAIL_FRESHER||it->second->type==ALL_SERVICE||it->second->type==MAIL_SYSNOTICE||it->second->type==MAIL_SYSUPDATE){
                saveCount +=  1;
            }
            if(it->second->tabType==USERMAIL){
                userCount++;
                
            }else{
                sysCount++;
            }
        }
        if(userCount>0){
            return USERMAIL;
        }else if(sysCount>0){
            return SYSTEMMAIL;
        }else if(saveCount>0){
            return SAVEMAIL;
        }
    }
    return m_panelType;
}

void MailSystemListPopUp::sort(){
    int total = m_data->count();
    int i = 0;
    int j = total - 1;
    
    while(j > 0){
        i = 0;
        while(i < j){
            auto &mail1 = GlobalData::shared()->mailList[dynamic_cast<CCString*>(m_data->objectAtIndex(i))->getCString()];
            auto &mail2 = GlobalData::shared()->mailList[dynamic_cast<CCString*>(m_data->objectAtIndex(i + 1))->getCString()];
            
            bool sweepFlag = false;
            if(mail1->createTime < mail2->createTime){
                sweepFlag = true;
            }
            if(sweepFlag){
                m_data->swap(i, i + 1);
            }
            i++;
        }
        j--;
    }
}

bool MailSystemListPopUp::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downNode", CCNode*, this->m_downNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailNum", CCLabelIF*, this->m_mailNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noMail", CCLabelIF*, this->m_noMail);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_BGNode", CCNode*, this->m_BGNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg1", CCScale9Sprite*, this->m_bg1);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeEdit", CCNode*, this->m_nodeEdit);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnAll", CCControlButton*, this->m_btnAll);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnDel", CCControlButton*, this->m_btnDel);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnReward", CCControlButton*, this->m_btnReward);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wrireMailBtn", CCControlButton*, this->m_wrireMailBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_saveBtn", CCControlButton*, this->m_saveBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblAll", CCLabelIF*, this->m_lblAll);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblReward", CCLabelIF*, this->m_lblReward);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblDel", CCLabelIF*, this->m_lblDel);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblWrite", CCLabelIF*, this->m_lblWrite);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblSave", CCLabelIF*, this->m_lblSave);
    return true;
}

SEL_CCControlHandler MailSystemListPopUp::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDeleteClick", MailSystemListPopUp::onDeleteClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onWriteClick", MailSystemListPopUp::onWriteClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onUserMailClick", MailSystemListPopUp::onUserMailClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSystemMailClick", MailSystemListPopUp::onSystemMailClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSaveMailClick", MailSystemListPopUp::onSaveMailClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAllReadClick", MailSystemListPopUp::onAllReadClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAllSelected", MailSystemListPopUp::onAllSelected);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDelete", MailSystemListPopUp::onDelete);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onReward", MailSystemListPopUp::onReward);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onWriteMailBtnClick", MailSystemListPopUp::onWriteMailBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddSaveClick", MailSystemListPopUp::onAddSaveClick);
    return NULL;
}

void MailSystemListPopUp::onCheckBoxClick(CCObject* obj){
    if(obj != m_checkBox){
        return;
    }
    for(auto info : GlobalData::shared()->mailList){
        info.second->prepareToDelete = m_checkBox->isSelected();
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("selectAll", CCBoolean::create(m_checkBox->isSelected()));
}

void MailSystemListPopUp::onAllReadClick(CCObject *pSender, CCControlEvent event){
    string allReadUids = "";
    string type = "";
    for(auto info : GlobalData::shared()->mailList){
        if(info.second->prepareToDelete){
            if(info.second->status == UNREAD){
                allReadUids.append(info.second->uid).append(",");
                type.append(CC_ITOA(info.second->type)).append(",");
                info.second->status = READ;
                MailController::getInstance()->readMailFromServer(info.second->uid,CC_ITOA(info.second->type));
            }
        }
    }
    if(allReadUids != ""){
        refreshView(NULL);
    }
}

void MailSystemListPopUp::onDeleteClick(CCObject *pSender, CCControlEvent event){
    YesNoDialog::showYesDialog(_lang("105570").c_str(),false,CCCallFunc::create(this, callfunc_selector(MailSystemListPopUp::onOkDeleteMail)),true);
}
void MailSystemListPopUp::onOkDeleteMail(){
    string deleteUids = "";
    string type = "";
    if(m_data==nullptr || m_data->count()<=0) return ;
    int num = m_data->count();
    for(int i=0;i<num;i++){
        std::string id = dynamic_cast<CCString*>(m_data->objectAtIndex(i))->getCString();
        MailInfo* info = GlobalData::shared()->mailList[id];
        if(info!=NULL && info->prepareToDelete){
            if(info->rewardId != "" && info->rewardStatus == 0){
                CCCommonUtils::flyText(_lang("105512"));
                return;
            }
            if(info->save==0){
                if(info->type==CHAT_ROOM)
                    deleteUids.append(info->crGroupId).append(",");
                else
                    deleteUids.append(info->uid).append(",");
                type.append(CC_ITOA(info->type)).append(",");
            }
        }
    }
    
    if(deleteUids != ""){
        deleteUids = deleteUids.substr(0, deleteUids.length() - 1);
        type = type.substr(0, type.length() - 1);
        MailController::getInstance()->removeMail(deleteUids, type);
        m_checkBox->setSelect(false);
    }
}
void MailSystemListPopUp::onWriteClick(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->addPopupInView(MailWritePopUpView::create("", ""));
}

//void MailSystemListPopUp::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell cell){
//    return;
//    MailSystemCell* wCell = dynamic_cast<MailSystemCell*>(cell);
//    if(wCell == NULL){
//        return;
//    }
//    wCell->showReadPopUp();
//}
void MailSystemListPopUp::tableCellTouched(  cocos2d::extension::TableView* table, CCTableViewCell* cell){
    
}
CCSize MailSystemListPopUp::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    //    QuestInfo* info = (QuestInfo*)m_data->objectAtIndex(idx);
    if(m_data->count()==0){
        return CCSize(0, 0);
    }
    if(m_isLoadMore&&idx == m_data->count()){
        if (CCCommonUtils::isIosAndroidPad()) {
            return CCSize(1450, 120);
        }
        return CCSize(604, 60);
    }
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1450, m_cell_HD_Width);
    }
    return CCSize(604, m_cellWith);
}
CCSize MailSystemListPopUp::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1450, m_cell_HD_Width);
    }
    return CCSize(604, m_cellWith);
}

CCTableViewCell* MailSystemListPopUp::tableCellAtIndex(CCTableView *table, ssize_t idx){
    int num = m_data->count();
    int saveCount = 0;
    int sysCount = 0;
    int userCount = 0;
    int mailTab3 = 0;
    int mailTab4 = 0;
    int mailTab5 = 0;

    map<string, MailInfo*>::iterator it;
    for(it = GlobalData::shared()->mailList.begin(); it != GlobalData::shared()->mailList.end(); it++){
        if(it->second==NULL){
            continue;
        }
        if(it->second->tabType==4){
            if(m_panelType==MAILTAB4){
                mailTab4++;
            }
        }else if(it->second->tabType==MAILTAB5){
            if(m_panelType==MAILTAB5){
                mailTab5++;
            }
        }
        else if(it->second->type==ALL_SERVICE){
            if(m_panelType==MAILTAB3){
                mailTab3++;
            }
        }else if(it->second->type== MAIL_FRESHER||it->second->type==MAIL_SYSNOTICE||it->second->type==MAIL_SYSUPDATE){
            if(m_panelType==SAVEMAIL){
                saveCount++;
            }
            
        }else if(it->second->tabType==USERMAIL){
            if(m_panelType==USERMAIL){
                userCount++;
            }
        }else{
            if(m_panelType==SYSTEMMAIL){
                sysCount++;
            }
        }
    }
    m_isLoadMore = false;
    if(m_panelType==USERMAIL&&userCount<GlobalData::shared()->mailCountObj.perT){
        num += 1;
        m_isLoadMore = true;
    }else if(m_panelType==SYSTEMMAIL&&sysCount<GlobalData::shared()->mailCountObj.sysT){
        num += 1;
        m_isLoadMore = true;
    }else if(m_panelType==SAVEMAIL&&saveCount<GlobalData::shared()->mailCountObj.saveT){
        num += 1;
        m_isLoadMore = true;
    }else if(m_panelType==MAILTAB3&&mailTab3<GlobalData::shared()->mailCountObj.studioT){
        num += 1;
        m_isLoadMore = true;
    }else if(m_panelType==MAILTAB4&&mailTab4<GlobalData::shared()->mailCountObj.fightT){
        num += 1;
        m_isLoadMore = true;
    }else if(m_panelType==MAILTAB5 && mailTab5 < GlobalData::shared()->mailCountObj.modT){
        num += 1;
        m_isLoadMore = true;
    }
    
    if(idx >= num){
        return NULL;
    }
    MailSystemCell* cell = (MailSystemCell*)table->dequeueCell();
    MailInfo *info = NULL;
    if(idx<m_data->count()){
        std::string str = dynamic_cast<CCString*>(m_data->objectAtIndex(idx))->getCString();
        info = GlobalData::shared()->mailList[str];
    }
    if(cell){
        cell->setData(info,m_tabView);
    }else{
        cell = MailSystemCell::create(info,m_tabView);
    }
    return cell;
}

ssize_t MailSystemListPopUp::numberOfCellsInTableView(CCTableView *table){
    if(m_data->count()==0){
        return 0;
    }else if(m_isLoadMore){
        return m_data->count()+1;
    }else{
        return m_data->count();
    }
    
}
void MailSystemListPopUp::setIsLoadMoreFlag(){

    int saveCount = 0;
    int sysCount = 0;
    int userCount = 0;
    int mailTab3 = 0;
    int mailTab4 = 0;
    int mailTab5 = 0;

    map<string, MailInfo*>::iterator it;
    for(it = GlobalData::shared()->mailList.begin(); it != GlobalData::shared()->mailList.end(); it++){
        if(it->second==NULL){
            continue;
        }
        if(it->second->tabType==4){
            if(m_panelType==MAILTAB4){
                mailTab4++;
            }
        }else if(it->second->tabType==MAILTAB5){
            if(m_panelType==MAILTAB5){
                mailTab5++;
            }
        }else if(it->second->type==ALL_SERVICE){
            if(m_panelType==MAILTAB3){
                mailTab3++;
            }
        }else
            if(it->second->type== MAIL_FRESHER||it->second->type==MAIL_SYSNOTICE||it->second->type==MAIL_SYSUPDATE){
                if(m_panelType==SAVEMAIL){
                    saveCount++;
                }
                
            }else if(it->second->tabType==USERMAIL){
                if(m_panelType==USERMAIL){
                    userCount++;
                }
            }else{
                if(m_panelType==SYSTEMMAIL){
                    sysCount++;
                }
            }
    }
    m_isLoadMore = false;
    if(m_panelType==USERMAIL&&userCount<GlobalData::shared()->mailCountObj.perT){

        m_isLoadMore = true;
    }else if(m_panelType==SYSTEMMAIL&&sysCount<GlobalData::shared()->mailCountObj.sysT){

        m_isLoadMore = true;
    }else if(m_panelType==SAVEMAIL&&saveCount<GlobalData::shared()->mailCountObj.saveT){

        m_isLoadMore = true;
    }else if(m_panelType==MAILTAB3&&mailTab3<GlobalData::shared()->mailCountObj.studioT){

        m_isLoadMore = true;
    }else if(m_panelType==MAILTAB4&&mailTab4<GlobalData::shared()->mailCountObj.fightT){
        
        m_isLoadMore = true;
    }else if(m_panelType==MAILTAB5&&mailTab5<GlobalData::shared()->mailCountObj.modT){
        
        m_isLoadMore = true;
    }
}
//
//unsigned int MailSystemListPopUp::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
//    return 1;
//}

void MailSystemListPopUp::onUserMailClick(CCObject *pSender, CCControlEvent event){
    if(USERMAIL!=m_panelType){
        m_isChangeTab =true;
    }else{
        m_isChangeTab = false;
    }
    m_checkBox->setSelect(false);
    m_panelType = USERMAIL;
    //currentType = m_panelType;
//    m_userMailBtn->setEnabled(false);
//    m_systemMailBtn->setEnabled(true);
//    m_saveMailBtn->setEnabled(true);
    resetMailDeleteFlag();
    refreshView(NULL);
}

void MailSystemListPopUp::onSystemMailClick(CCObject *pSender, CCControlEvent event){
    if(SYSTEMMAIL!=m_panelType){
        m_isChangeTab =true;
    }else{
        m_isChangeTab = false;
    }
    m_checkBox->setSelect(false);
    m_panelType = SYSTEMMAIL;
  //  currentType = m_panelType;
//    m_userMailBtn->setEnabled(true);
//    m_systemMailBtn->setEnabled(false);
//    m_saveMailBtn->setEnabled(true);
    resetMailDeleteFlag();
    refreshView(NULL);
}

void MailSystemListPopUp::onSaveMailClick(CCObject *pSender, CCControlEvent event){
    if(SAVEMAIL!=m_panelType){
        m_isChangeTab =true;
    }else{
        m_isChangeTab = false;
    }
    m_checkBox->setSelect(false);
    m_panelType = SAVEMAIL;
   // currentType = m_panelType;
//    m_userMailBtn->setEnabled(true);
//    m_systemMailBtn->setEnabled(true);
//    m_saveMailBtn->setEnabled(false);
    resetMailDeleteFlag();
    refreshView(NULL);
}
bool MailSystemListPopUp::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_tabView->setBounceable(true);
    m_startPoint = pTouch->getStartLocation();
    return true;
//    if(m_isLoadMore)
//        return true;
//    else
//        return false;
//    if(isTouchInside(m_bg, pTouch)){
//        return true;
//    }
//    return false;
}
void MailSystemListPopUp::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    float offsetY = m_tabView->getContentOffset().y;
    m_currMinOffsetY = m_tabView->minContainerOffset().y;
    if(offsetY>0&&offsetY>m_currMinOffsetY){
        float y = (pTouch->getLocation().y - pTouch->getStartLocation().y)/5;
        if(y<m_currMinOffsetY){
            y += m_currMinOffsetY;
        }
        m_tabView->setContentOffset(ccp(0,y));
    }
    if(m_isLoadMore && offsetY>-30){
        auto cell = dynamic_cast<MailSystemCell*>(m_tabView->cellAtIndex(m_data->count()));
        if(cell==NULL){
            return;
        }
        CCLabelIF* txt = (CCLabelIF*)cell->getChildByTag(10001);
        txt->setVisible(true);
        if(offsetY>30)
            txt->setString(_lang("105318").c_str());
        else
            txt->setString(_lang("115189").c_str());
    }
    return;
    
}
void MailSystemListPopUp::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(isTouchInside(m_bg, pTouch)){
       // CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_DELETERECOVER);
    }
    CCPoint endPoint = pTouch->getLocation();
    if((endPoint.y-m_startPoint.y>0)&&(fabs(endPoint.y-m_startPoint.y)>fabs(endPoint.x-m_startPoint.x))){
    
    }else{
        return;
    }
    if(m_isLoadMore){
        m_currMinOffsetY = m_tabView->minContainerOffset().y;
        m_currOffsetY = m_tabView->getContentOffset().y;
        float dy = 30;
//        if(m_currMinOffsetY>0){
//            dy = m_currMinOffsetY+30;
//        }
        if(m_currOffsetY > dy){
            addLoadingAni();
            m_tabView->setBounceable(false);
            m_tabView->setTouchStop(false);
            this->scheduleOnce(schedule_selector(MailSystemListPopUp::sendReloadMoreMail),0.0);
        }
    }
}

void MailSystemListPopUp::onEditAllMail(CCObject* obj)
{
    if(!obj || !dynamic_cast<CCInteger*>(obj)){
        return;
    }
    m_allSelected=false;
    int value = dynamic_cast<CCInteger*>(obj)->getValue();
    bool bEdit = value == 1? true:false;
    m_nodeEdit->setVisible(bEdit);
    CCPoint offset = m_tabView->getContentOffset();
    if(bEdit==false){
        m_tabView->setViewSize(m_listContainer->getContentSize());
        m_tabView->setPositionY(0);
        m_tabView->setContentOffset(CCPoint(offset.x, offset.y+MAIL_SYS_VIEW_EDIT_BOX_HEIGHT));
        return;
    }
    CCSize listSize = m_listContainer->getContentSize();
    m_tabView->setViewSize(CCSize(listSize.width,listSize.height - MAIL_SYS_VIEW_EDIT_BOX_HEIGHT));
    m_tabView->setPositionY(MAIL_SYS_VIEW_EDIT_BOX_HEIGHT);
    m_tabView->setContentOffset(CCPoint(offset.x, offset.y-MAIL_SYS_VIEW_EDIT_BOX_HEIGHT));
//    bool canEdit = false;
//    MailInfo *info = NULL;
//    for (int i=0; i<m_data->count(); i++) {
//        std::string str = dynamic_cast<CCString*>(m_data->objectAtIndex(i))->getCString();
//        info = GlobalData::shared()->mailList[str];
//        if (info && info->save!=1) {
//            canEdit=true;
//            break;
//        }
//    }
//    if (canEdit) {
        UIComponent::getInstance()->changeMailDelState();
//    }
//    else {
//        CCCommonUtils::flyHint("", "", _lang("105599"));
//    }
}

void MailSystemListPopUp::onDeleteAllMail(CCObject* obj)
{
//    int j = 0;
//    m_tmpUids = "";
//    MailInfo *info = NULL;
//    for (int i=0; i<m_data->count(); i++) {
//        std::string str = dynamic_cast<CCString*>(m_data->objectAtIndex(i))->getCString();
//        info = GlobalData::shared()->mailList[str];
//        if (info && info->save!=1) {
//            if (info->rewardId=="" || (info->rewardId!=""&&info->rewardStatus==1)) {
//                m_tmpUids += "," + info->uid;
//                j++;
//            }
//        }
//    }
//    if (m_tmpUids.length()>1) {
//        m_tmpUids = m_tmpUids.substr(1,m_tmpUids.length()-1);
//        YesNoDialog::show(_lang_1("105598",CC_ITOA(j)), CCCallFunc::create(this, callfunc_selector(MailSystemListPopUp::onOKDeleteAllMail)));
//    }
//    else {
//        CCCommonUtils::flyHint("", "", _lang("105599"));
//    }
    if (MailController::getInstance()->canOp()) {
        std::string showDialog = _lang("105570");
        YesNoDialog::show(showDialog.c_str(), CCCallFunc::create(this, callfunc_selector(MailSystemListPopUp::onOKDeleteAllMail)));
    }
    else {
        CCCommonUtils::flyHint("", "", _lang("105599"));
    }
}

void MailSystemListPopUp::onOKDeleteAllMail()
{
    MailController::getInstance()->removeAllOpMails();
    if(m_isLoadMore){
        addLoadingAni();
        m_tabView->setBounceable(false);
        this->scheduleOnce(schedule_selector(MailSystemListPopUp::sendReloadMoreMail),1.0);
    }
}

void MailSystemListPopUp::onAllSelected(CCObject *pSender, CCControlEvent event){
    if(m_allSelected==false){
        CCObject *ccObj = NULL;
        CCARRAY_FOREACH(m_data, ccObj){
            CCString *ccstring = dynamic_cast<CCString*>(ccObj);
            if(ccstring && ccstring->getCString()){
                MailController::getInstance()->addToOpMails(ccstring->getCString());
            }
        }
        m_allSelected=true;
    }else{
        m_allSelected=false;
        MailController::getInstance()->clearOpMails();
    }
    if(m_tabView){
        m_tabView->reloadData();
    }
}
void MailSystemListPopUp::onDelete(CCObject *pSender, CCControlEvent event){
    onDeleteAllMail(NULL);
    m_allSelected = false;
}
//void MailSystemListPopUp::onReward(CCObject *pSender, CCControlEvent event){
//    MailController::getInstance()->rewardAllOpMails();
//    m_allSelected = false;
//}
void MailSystemListPopUp::onWriteMailBtnClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent){
    MailController::getInstance()->clearOpMails();
    m_allSelected = false;
    if(m_tabView){
        m_tabView->reloadData();
    }
    PopupViewController::getInstance()->addPopupInView(MailWritePopUpView::create("", ""));
    CCArray* p = CCArray::create();
    p->addObject(CCInteger::create(MAIL_OPEN));
    CCArray* d = CCArray::create();
    d->addObject(CCInteger::create(1));
    DataRecordCommand *cmd = new DataRecordCommand(OPEN_PANEL, p, d);
    cmd->sendAndRelease();
}
void MailSystemListPopUp::onAddSaveClick(CCObject * pSender, Control::EventType pCCControlEvent){
    CCCommonUtils::flyHint("", "", _lang("E100008"));
}