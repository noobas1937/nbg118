//
//  MailPopUpView.cpp
//  IF
//
//  Created by 李锐奇 on 13-10-11.
//
//

#include "MailPopUpView.h"
#include "PopupViewController.h"
#include "CCSafeNotificationCenter.h"
#include "MailWritePopUpView.h"
#include "MailCell.h"
#include "MailWritePopUpView.h"
#include "MailController.h"
#include "UIComponent.h"
#include "SettingPopUpView.h"
#include "YesNoDialog.h"
#include "DataRecordCommand.h"
#include "MailMonsterCellInfo.h"
#include "MailResourceHelpCellInfo.h"
static int currentType = 0;

#define MAIL_VIEW_EDIT_BOX_HEIGHT 80
void MailPopUpView::onEnter(){
    PopupBaseView::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailPopUpView::refreshView), MAIL_LIST_CHANGE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailPopUpView::refreshAddList), MAIL_LIST_ADD, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailPopUpView::onCheckBoxClick), CHECK_BOX_SELECT, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailPopUpView::onCheckBoxClick), CHECK_BOX_UNSELECT, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailPopUpView::sendReloadMoreMail), MAIL_RELOAD_MORE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailPopUpView::deleteMailcellByIndex), MAIL_LIST_DELETE, NULL);
//    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailPopUpView::onDeleteAllMail), MAIL_DEL_ALL_MSG, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailPopUpView::onEditAllMail), MAIL_EDIT_MSG, NULL);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -3,false);
    setTitleName(_lang("105513"));
    CCLoadSprite::doResourceByCommonIndex(6, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(6, false);
    });
//    UIComponent::getInstance()->showPopupView(3);
    if(!isInit){
        refreshView(NULL);
    }
    //setUnreadEffect();
    
    CCArray* p = CCArray::create();
    p->addObject(CCInteger::create(MAIL_OPEN));
    DataRecordCommand *cmd = new DataRecordCommand(OPEN_PANEL, p, CCArray::create());
    cmd->sendAndRelease();
}

void MailPopUpView::onExit(){
    isInit = false;
    removeLoadingAni();
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_LIST_ADD);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_LIST_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, CHECK_BOX_SELECT);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, CHECK_BOX_UNSELECT);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_RELOAD_MORE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_LIST_DELETE);
//    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_DEL_ALL_MSG);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_EDIT_MSG);
    setTouchEnabled(false);
    //resetMailDeleteFlag();
    MailController::getInstance()->CanRemoveMail = false;
    if(m_nodeEdit->isVisible()){
        CCPoint offset = m_tabView->getContentOffset();
        m_tabView->setViewSize(m_listContainer->getContentSize());
        m_tabView->setPositionY(0);
        m_tabView->setContentOffset(CCPoint(offset.x, offset.y+MAIL_VIEW_EDIT_BOX_HEIGHT));
        m_nodeEdit->setVisible(false);
    }
    PopupBaseView::onExit();
}

void MailPopUpView::resetMailDeleteFlag(){
    map<string, MailInfo*>::iterator it;
    for(it = GlobalData::shared()->mailList.begin(); it != GlobalData::shared()->mailList.end(); it++){
        it->second->prepareToDelete = false;
    }
}

MailPopUpView* MailPopUpView::create(int panelType){
    MailPopUpView* ret = new MailPopUpView(panelType);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MailPopUpView::init(){
    if(!PopupBaseView::init()){
        return false;
    }
    setIsHDPanel(true);
    MailController::getInstance()->initTranslateMails();
    CCLoadSprite::doResourceByCommonIndex(6, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(6, false);
        
    });
    isInit = true;
    m_loadingIcon=NULL;
    m_addNum = 0;
    m_deleteY = 0;
    m_cellWith = 150;
    if (CCCommonUtils::isIosAndroidPad()) {
        m_cell_HD_Width = 309;
    }
    m_isChangeTab = false;
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
        this->m_downNode->setPositionY(m_downNode->getPositionY() - dh + 77/*bg sprite's height*/);
    }
    
    setTitleName(_lang("105513"));
    m_data = CCArray::create();
    m_data->retain();
    m_userdata = CCArray::create();
    m_nodeEdit->setVisible(false);
//    CCModelLayerColor* modelLayer = CCModelLayerColor::create();
//    modelLayer->setOpacity(125);
//    modelLayer->setColor(ccBLACK);
//    modelLayer->setContentSize(CCSize(94,150));
//    modelLayer->setAnchorPoint(ccp(0.5, 0.5));
//    m_checkBoxContainer->addChild(modelLayer);
    m_checkBox = CheckBox::create();
    //m_checkBox = NULL;
    //m_checkBoxContainer->addChild(m_checkBox);
    m_checkBox->setVisible(false);
    m_checkBox->setBtnTouchPriority(-1);
   // m_ccLayerMode->setTouchPriority(-1);
    //m_ccLayerMode1->setVisible(false);
    //m_ccLayerMode1->setTouchPriority(-1);
    
    m_tabView = CCTableView::create(this, m_listContainer->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setDelegate(this);
    m_tabView->setTouchPriority(1);
    m_tabView->setPositionX(0);
    
    m_listContainer->addChild(m_tabView);
    m_BGTouchLayer = BGTouchLayer::create(m_listContainer->getContentSize());
    m_BGTouchLayer->setAnchorPoint(ccp(0.5, 1.0));
    m_BGNode->addChild(m_BGTouchLayer);
    m_BGTouchLayer->setPosition(this->m_bg->getPosition());
//    CCCommonUtils::setButtonTitle(this->m_userMailBtn, _lang("105500").c_str());
//    CCCommonUtils::setButtonTitle(this->m_systemMailBtn, _lang("105568").c_str());
//    CCCommonUtils::setButtonTitle(this->m_saveMailBtn, _lang("105569").c_str());

    m_noMail->setString(_lang("105514").c_str());
    
    resetMailDeleteFlag();
    refreshView(NULL);
    
    MailController::getInstance()->showMailWarning();
    MailController::getInstance()->getAllianceMember();
//    m_lblAll->setString(_lang("102145").c_str());
//    m_lblDel->setString(_lang("108523").c_str());
//    m_lblReward->setString( _lang("101313").c_str());
//    m_lblWrite->setString(_lang("105048").c_str());
//    m_lblSave->setString(_lang("108501").c_str());
//    m_deleteBtnTitle->setString(_lang("108523").c_str());
//    m_deleteBtn->setVisible(false);
//    m_deleteBtnTitle->setVisible(false);
//    m_writeBtn->setVisible(false);

//    currentType = getOpenType();
//    switch (currentType) {
//        case USERMAIL:
//            onUserMailClick(NULL,Control::EventType::TOUCH_DOWN);
//            break;
//        case SYSTEMMAIL:
//            onSystemMailClick(NULL,Control::EventType::TOUCH_DOWN);
//            break;
//        case SAVEMAIL:
//            onSaveMailClick(NULL,Control::EventType::TOUCH_DOWN);
//            break;
//        default:
//            onUserMailClick(NULL,Control::EventType::TOUCH_DOWN);
//            break;
//    }
    
    return true;
}
bool MailPopUpView::getDataContent(){
    string allReadUids = "";
    string type = "";

    int total = m_data->count();
    int i = 0;
    while(i < total){
        auto &mail = GlobalData::shared()->mailList[dynamic_cast<CCString*>(m_data->objectAtIndex(i))->getCString()];
        if(mail->isReadContent==false){
            if(mail->uid!=""){

                if((mail->tabType==USERMAIL&&(mail->dialogs->count()>0))||mail->type==MAIL_RESOURCE||mail->type==MAIL_ATTACKMONSTER||mail->type==MAIL_RESOURCE_HELP||
                   mail->tabType==MAILTAB5){
                    i++;
                    mail->isReadContent = true;
                    continue;
                }
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
void MailPopUpView::refreshAddList(cocos2d::CCObject *obj){
    m_tabView->setTouchStop(true);
    removeLoadingAni();
    m_isGetAllContent = false;
    int add = dynamic_cast<CCInteger*>(obj)->getValue();
    m_addNum = add;
    generateDataArr();
    float dy = m_tabView->getContentOffset().y;
    if(m_addNum==0){
//        m_tabView->reloadData();
//        m_tabView->setContentOffset(ccp(0,0));
        return;
    }
    if(m_allSelected){
        m_allSelected=false;
        MailController::getInstance()->clearOpMails();
        onAllSelected(NULL,Control::EventType::TOUCH_DOWN);
    }
    m_tabView->reloadData();
    m_newMinOffsetY = m_tabView->minContainerOffset().y;
    if(dy>m_tabView->maxContainerOffset().y){
        dy = m_tabView->maxContainerOffset().y;
    }
    if(dy<m_newMinOffsetY){
        dy = m_newMinOffsetY;
    }
    if(m_currMinOffsetY<0&&m_newMinOffsetY<0){
        float dy1 = m_newMinOffsetY-m_currMinOffsetY+m_currOffsetY;
        if(dy1>0){
            dy1 = 0;
        }
        m_tabView->setContentOffset(ccp(0,dy1));
        float movedy = m_newMinOffsetY-m_currMinOffsetY + 200;
        if(movedy>0){
            movedy = 0;
        }
//        m_tabView->setContentOffsetInDuration(ccp(0,movedy), 0.7);
    }
    if(m_data->count()>0){
        m_noMail->setVisible(false);
    }else{
        m_noMail->setVisible(true);
    }
}
void MailPopUpView::deleteMailcellByIndex(cocos2d::CCObject *obj){
    if(obj==NULL)
        return;
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

    m_tabView->removeCellAtIndex(index);
    
    m_deleteY=m_cellWith;
   // m_tabView->updateInset();
    refreshView(NULL);
    if(m_isLoadMore){
        m_currOffsetY = m_tabView->getContentOffset().y;
        if(m_currOffsetY > 30){
            addLoadingAni();
            m_tabView->setBounceable(false);
            this->scheduleOnce(schedule_selector(MailPopUpView::sendReloadMoreMail),0.0);
        }
    }

}
void MailPopUpView::refreshView(CCObject* obj){
     //m_noMail->setString(_lang("105514").c_str());
    setUnreadEffect();
    generateDataArr();
    m_noMail->setVisible(false);
    m_isGetAllContent =getDataContent();
    if(m_isGetAllContent){
        m_data->removeAllObjects();
        addLoadingAni();
    }else{
        m_data->removeAllObjects();
        generateDataArr();
        m_isGetAllContent = false;
        refreshDataList();
        removeLoadingAni();
    }

}
void MailPopUpView::addLoadingAni(){
    if(m_loadingIcon){
        m_loadingIcon->stopAllActions();
        m_listContainer->removeChild(m_loadingIcon,true);
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
void MailPopUpView::removeLoadingAni(){
    if(m_loadingIcon){
        m_listContainer->removeChild(m_loadingIcon);
        m_loadingIcon = NULL;
    }
}
void MailPopUpView::sendReloadMoreMail(CCObject* obj)
{
    sendReloadMoreMail(0.0);
}
void MailPopUpView::sendReloadMoreMail(float _time){
    int count = m_data->count();
    int j = 0;
    int realcount = count;
    while(j < count){
        auto &mail1 = GlobalData::shared()->mailList[dynamic_cast<CCString*>(m_data->objectAtIndex(j))->getCString()];
        if(mail1->tabType==2||mail1->tabType==3||mail1->tabType==4 || mail1->type == CHAT_ROOM){
            realcount--;
        }
        j++;
    }
    addLoadingAni();
    MailController::getInstance()->reloadMailMore(1000,realcount,20);
}
void MailPopUpView::refreshDataList(){
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
        if (pos.y > 0) {
           // pos.y  = 0;
           // m_tabView->setContentOffset(pos);
            //return;
        }
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
    if(num>0){
        m_noMail->setVisible(false);
    }else{
        m_noMail->setVisible(true);
    }
//    if(currentType==USERMAIL){
//        if(GlobalData::shared()->mailCountObj.perT<=0&&num<=0){
//            m_noMail->setVisible(true);
//        }
//        
//    }else if(currentType==SYSTEMMAIL){
//        if(GlobalData::shared()->mailCountObj.sysT<=0&&num<=0){
//            m_noMail->setVisible(true);
//        }
//    }else if(currentType==SAVEMAIL){
//        if(GlobalData::shared()->mailCountObj.saveT<=0&&num<=0){
//            m_noMail->setVisible(true);
//        }
//    }else{
//        m_noMail->setVisible(false);
//    }
}
void MailPopUpView::setUnreadEffect(){
    map<std::string, MailInfo*>::iterator it;
    int userCount = GlobalData::shared()->mailCountObj.perR;
    int sysCount = GlobalData::shared()->mailCountObj.sysR;
    int saveCount = GlobalData::shared()->mailCountObj.saveR;

    if(userCount > 0){
        m_userUnreadNode->setVisible(true);
       // m_userUnreadText->setString(CC_ITOA(userCount));
    }else{
        m_userUnreadNode->setVisible(false);
    }
    
    if(sysCount > 0){
        m_sysUnreadNode->setVisible(true);
      //  m_sysUnreadText->setString(CC_ITOA(sysCount));
    }else{
        m_sysUnreadNode->setVisible(false);
    }
   // m_mailBGFlag2->setVisible(false);
    if(saveCount > 0){
        m_saveUnreadNode->setVisible(true);
   //     m_saveUnreadText->setString(CC_ITOA(saveCount));
        if(GlobalData::shared()->mailCountObj.upR>0){
   //         m_mailBGFlag2->setVisible(true);
        }
    }else{
        m_saveUnreadNode->setVisible(false);
    }
}

int MailPopUpView::getMaxMailNum(){
    if(m_panelType == USERMAIL){
        return GlobalData::shared()->mailConfig.userMailMax;
    }else if(m_panelType == SYSTEMMAIL){
        return GlobalData::shared()->mailConfig.sysMailMax;
    }else if(m_panelType == SAVEMAIL){
        return GlobalData::shared()->mailConfig.saveMailMax;
    }
    return 0;
}

void MailPopUpView::generateDataArr(){
    m_data->removeAllObjects();
    m_userdata->removeAllObjects();
    CCArray* reportData = CCArray::create();
    CCArray* reportData1 = CCArray::create();
    std::string saveData = "";
    std::string saveData1 = "";
    std::string tab3Data = "";
    std::string tab3Data1 = "";
    std::string tab4Data = "";
//    std::string tab4Data1 = "";
    std::string tab5Data = "";
    std::string tab5Data1 = "";
    
    int saveDataMax = 0;
    int saveData1Max = 0;
    int tab3DataMax = 0;
    int tab3Data1Max = 0;
    int tab4DataMax = 0;
//    int tab4Data1Max = 0;
    int tab5DataMax = 0;
    int tab5Data1Max = 0;
    
    
    map<std::string, MailInfo*>::iterator it;
//    CCLOGFUNCF("miallist count %d",GlobalData::shared()->mailList.size());
    for(it = GlobalData::shared()->mailList.begin(); it != GlobalData::shared()->mailList.end(); it++){
        bool addFlag = false;
//        CCLOGFUNCF("miallist type %d",it->second->type);
        if(it->second->type==ALL_SERVICE){  //--MAILTAB3
            if(it->second->status == READ){
                if(it->second->createTime > tab3Data1Max){
                    tab3Data1Max = it->second->createTime;
                    tab3Data1 = it->second->uid;
                }
            }else{
                if(it->second->createTime > tab3DataMax){
                    tab3DataMax =  it->second->createTime;
                    tab3Data = it->second->uid;
                }
            }
        }else if(it->second->type== MAIL_FRESHER||it->second->type==MAIL_SYSNOTICE||it->second->type==MAIL_SYSUPDATE){ // SAVEMAIL
            if(it->second->status == READ){
                if(it->second->createTime > saveData1Max){
                    saveData1Max = it->second->createTime;
                    saveData1 = it->second->uid;
                }
            }else{
                if(it->second->createTime > saveDataMax){
                    saveDataMax =  it->second->createTime;
                    saveData = it->second->uid;
                }
            }
        }else if(it->second->tabType== MAILTAB4){
//            if(it->second->status == READ){
//                if(it->second->createTime > tab4Data1Max){
//                    tab4Data1Max = it->second->createTime;
//                    tab4Data1 = it->second->uid;
//                }
//            }else{
                if(it->second->createTime > tab4DataMax){
                    tab4DataMax =  it->second->createTime;
                    tab4Data = it->second->uid;
                }
//            }
        }else if(it->second->tabType== MAILTAB5){
            if(it->second->status == READ){
                if(it->second->createTime > tab5Data1Max){
                    tab5Data1Max =  it->second->createTime;
                    tab5Data1 = it->second->uid;
                }
            }else{
                if(it->second->createTime > tab5DataMax){
                    tab5DataMax =  it->second->createTime;
                    tab5Data = it->second->uid;
                }
            }
        }else if(it->second->tabType ==USERMAIL){
            if(!it->second->dialogs || it->second->dialogs->count()==0){
                addFlag = false;
            }else{
                addFlag = true;
            }
        }else if(it->second->type==MAIL_RESOURCE){
            MailResourceCellInfo* info = dynamic_cast<MailResourceCellInfo*>(it->second);
            if(info->collect->count()==0){
                addFlag = false;
            }else{
                addFlag = true;
            }
            
        }else if(it->second->type==MAIL_RESOURCE_HELP){
            MailResourceHelpCellInfo* info = dynamic_cast<MailResourceHelpCellInfo*>(it->second);
            if(info->collect->count()==0){
                addFlag = false;
            }else{
                addFlag = true;
            }
            
        }else if(it->second->type==MAIL_ATTACKMONSTER){
            MailMonsterCellInfo* info = dynamic_cast<MailMonsterCellInfo*>(it->second);
            if(info->monster->count()==0){
                addFlag = false;
            }else{
                addFlag = true;
            }
            
        }else{
            if(it->second->status == READ){
                reportData1->addObject(CCString::create(it->second->uid));
            }else{
                reportData->addObject(CCString::create(it->second->uid));
            }
        }
        if(addFlag){
            if(it->second->status == READ){
                if(it->second->type==CHAT_ROOM)
                    reportData1->addObject(CCString::create(it->second->crGroupId));
                else
                    reportData1->addObject(CCString::create(it->second->uid));
            }else{
                if(it->second->type==CHAT_ROOM)
                    reportData->addObject(CCString::create(it->second->crGroupId));
                else
                    reportData->addObject(CCString::create(it->second->uid));
            }
        }
    }
    reportData = MailController::getInstance()->getSortMailByTime(reportData);
    reportData1 = MailController::getInstance()->getSortMailByTime(reportData1);
    if(!saveData.empty()){
        m_data->addObject(CCString::create(saveData));
        saveData += std::string(",") + CC_ITOA(SAVEMAIL);
        m_folders.push_back(saveData);
    }else if(!saveData1.empty()){
        m_data->addObject(CCString::create(saveData1));
        saveData1 += std::string(",") + CC_ITOA(SAVEMAIL);
        m_folders.push_back(saveData1);
    }
    if(!tab3Data.empty()){
        m_data->addObject(CCString::create(tab3Data));
        tab3Data += std::string(",") + CC_ITOA(MAILTAB3);
        m_folders.push_back(tab3Data);
    }else if(!tab3Data1.empty()){
        m_data->addObject(CCString::create(tab3Data1));
        tab3Data1 += std::string(",") + CC_ITOA(MAILTAB3);
        m_folders.push_back(tab3Data1);
    }
    if(!tab4Data.empty()){
        m_data->addObject(CCString::create(tab4Data));
        tab4Data += std::string(",") + CC_ITOA(MAILTAB4);
        m_folders.push_back(tab4Data);
    }/*else if(!tab4Data1.empty()){
        m_data->addObject(CCString::create(tab4Data1));
    }*/
    if(!tab5Data.empty()){
        m_data->addObject(CCString::create(tab5Data));
        tab5Data += std::string(",") + CC_ITOA(MAILTAB5);
        m_folders.push_back(tab5Data);
    }else if(!tab5Data1.empty()){
        m_data->addObject(CCString::create(tab5Data1));
        tab5Data1 += std::string(",") + CC_ITOA(MAILTAB5);
        m_folders.push_back(tab5Data1);
    }
    if(reportData->count()>0){
        m_data->addObjectsFromArray(reportData);
    }
    if(reportData1->count()>0){
        m_data->addObjectsFromArray(reportData1);
    }
    setIsLoadMoreFlag();
    //m_data->addObjectsFromArray(m_userdata);
}

int  MailPopUpView::getOpenType(){
    map<std::string, MailInfo*>::iterator it;
    int userCount = 0;
    int sysCount = 0;
    int saveCount = 0;
    for(it = GlobalData::shared()->mailList.begin(); it != GlobalData::shared()->mailList.end(); it++){
        if(it->second->status == UNREAD){
            if(it->second->type== MAIL_FRESHER||it->second->type==ALL_SERVICE||it->second->type==MAIL_SYSNOTICE||it->second->type==MAIL_SYSUPDATE){
                saveCount +=  1;
            }else if(it->second->tabType ==USERMAIL&&(it->second->dialogs->count()>0)){
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
            if(it->second->tabType ==USERMAIL){
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

void MailPopUpView::sort(){
    int total = m_userdata->count();
    int i = 0;
    int j = total - 1;
    
    while(j > 0){
        i = 0;
        while(i < j){
            auto &mail1 = GlobalData::shared()->mailList[dynamic_cast<CCString*>(m_userdata->objectAtIndex(i))->getCString()];
            auto &mail2 = GlobalData::shared()->mailList[dynamic_cast<CCString*>(m_userdata->objectAtIndex(i + 1))->getCString()];
            
            bool sweepFlag = false;
            if(mail1->createTime < mail2->createTime){
                sweepFlag = true;
            }
            if(sweepFlag){
                m_userdata->swap(i, i + 1);
            }
            i++;
        }
        j--;
    }
}

bool MailPopUpView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_writeBtn", CCControlButton*, this->m_writeBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_deleteBtn", CCControlButton*, this->m_deleteBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_userMailBtn", CCControlButton*, this->m_userMailBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_systemMailBtn", CCControlButton*, this->m_systemMailBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_saveMailBtn", CCControlButton*, this->m_saveMailBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listContainer", CCNode*, this->m_listContainer);
   // CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_checkBoxContainer", CCNode*, this->m_checkBoxContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downNode", CCNode*, this->m_downNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailNum", CCLabelIF*, this->m_mailNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noMail", CCLabelIF*, this->m_noMail);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sysUnreadNode", CCNode*, this->m_sysUnreadNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_userUnreadNode", CCNode*, this->m_userUnreadNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_saveUnreadNode", CCNode*, this->m_saveUnreadNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sysUnreadText", CCLabelIF*, this->m_sysUnreadText);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_userUnreadText", CCLabelIF*, this->m_userUnreadText);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_saveUnreadText", CCLabelIF*, this->m_saveUnreadText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg1", CCScale9Sprite*, this->m_bg1);
  //  CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_deleteBtnTitle", CCLabelIF*, this->m_deleteBtnTitle);
  //  CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ccLayerMode", CCModelLayerColor*, this->m_ccLayerMode);
   // CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ccLayerMode1", CCModelLayerColor*, this->m_ccLayerMode1);
 //   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailBGFlag2", CCSprite*, this->m_mailBGFlag2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_BGNode", CCNode*, this->m_BGNode);

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

SEL_CCControlHandler MailPopUpView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
   // CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDeleteClick", MailPopUpView::onDeleteClick);
   // CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onWriteClick", MailPopUpView::onWriteClick);
   // CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onUserMailClick", MailPopUpView::onUserMailClick);
   // CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSystemMailClick", MailPopUpView::onSystemMailClick);
   // CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSaveMailClick", MailPopUpView::onSaveMailClick);
  //  CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAllReadClick", MailPopUpView::onAllReadClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAllSelected", MailPopUpView::onAllSelected);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDelete", MailPopUpView::onDelete);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onReward", MailPopUpView::onReward);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onWriteMailBtnClick", MailPopUpView::onWriteMailBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddSaveClick", MailPopUpView::onAddSaveClick);
    return NULL;
}

void MailPopUpView::onCheckBoxClick(CCObject* obj){
    if(obj != m_checkBox){
        return;
    }
    for(auto info : GlobalData::shared()->mailList){
        info.second->prepareToDelete = m_checkBox->isSelected();
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("selectAll", CCBoolean::create(m_checkBox->isSelected()));
}

void MailPopUpView::onAllReadClick(CCObject *pSender, CCControlEvent event){
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

void MailPopUpView::onDeleteClick(CCObject *pSender, CCControlEvent event){
  YesNoDialog::showYesDialog(_lang("105570").c_str(),false,CCCallFunc::create(this, callfunc_selector(MailPopUpView::onOkDeleteMail)),true);
}
void MailPopUpView::onOkDeleteMail(){
    string deleteUids = "";
    string type = "";
    if(m_data==NULL || m_data->count()<=0) return ;
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
void MailPopUpView::onWriteClick(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->addPopupInView(MailWritePopUpView::create("", ""));
}
void MailPopUpView::tableCellTouched(  cocos2d::extension::TableView* table, CCTableViewCell* cell){
    
}
CCSize MailPopUpView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
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
CCSize MailPopUpView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1450, m_cell_HD_Width);
    }
    return CCSize(604, m_cellWith);
}

CCTableViewCell* MailPopUpView::tableCellAtIndex(CCTableView *table, ssize_t idx){
    int num = m_data->count();
    if(0==m_data->count()||m_isGetAllContent){
        CCLOGFUNC("");
        return NULL;
    }
    int saveCount = 0;
    int sysCount = 0;
    int userCount = 0;
    map<string, MailInfo*>::iterator it;
       for(it = GlobalData::shared()->mailList.begin(); it != GlobalData::shared()->mailList.end(); it++){
               if(it->second==NULL){
                   continue;
               }
               if(it->second->type== MAIL_FRESHER||it->second->type==ALL_SERVICE||it->second->type==MAIL_SYSNOTICE||it->second->type==MAIL_SYSUPDATE){
                   if(m_panelType==SAVEMAIL){
                       saveCount++;
                   }

              }else if((it->second->tabType==USERMAIL&&(it->second->dialogs->count()>0))||it->second->type==MAIL_RESOURCE||it->second->type==MAIL_ATTACKMONSTER||it->second->type==MAIL_RESOURCE_HELP){
                    userCount++;
              }else if(it->second->tabType==4){
//                  sysCount++;
              }else{
                    sysCount++;
               }
      }
    int total = GlobalData::shared()->mailCountObj.perT+GlobalData::shared()->mailCountObj.sysT;
     m_isLoadMore = false;
    if(total>(userCount+sysCount)){
        num += 1;
         m_isLoadMore = true;
    }
    if(m_panelType==USERMAIL&&userCount<GlobalData::shared()->mailCountObj.perT){
       // num += 1;
    }else if(m_panelType==SYSTEMMAIL&&sysCount<GlobalData::shared()->mailCountObj.sysT){
       // num += 1;
    }else if(m_panelType==SAVEMAIL&&saveCount<GlobalData::shared()->mailCountObj.saveT){
        //num += 1;
    }
    
    if(idx > m_data->count()){
        return NULL;
    }
    MailCell* cell = (MailCell*)table->dequeueCell();
    MailInfo *info = NULL;
    if(idx<m_data->count()){
        std::string str = dynamic_cast<CCString*>(m_data->objectAtIndex(idx))->getCString();
        info = GlobalData::shared()->mailList[str];
    }
    if(cell){
        cell->setData(info,m_tabView);
    }else{
        cell = MailCell::create(info,m_tabView);
    }

    return cell;
}

void MailPopUpView::onEditAllMail(CCObject* obj)
{
    if(!obj || !dynamic_cast<CCInteger*>(obj)){
        return;
    }
    m_allSelected=false;
    CCPoint offset = m_tabView->getContentOffset();
    int value = dynamic_cast<CCInteger*>(obj)->getValue();
    bool bEdit = value==1?true:false;
    m_nodeEdit->setVisible(bEdit);
    if(bEdit==false){
        m_tabView->setViewSize(m_listContainer->getContentSize());
        m_tabView->setPositionY(0);
        m_tabView->setContentOffset(CCPoint(offset.x, offset.y+MAIL_VIEW_EDIT_BOX_HEIGHT));
        return;
    }
    CCSize listSize = m_listContainer->getContentSize();
    m_tabView->setViewSize(CCSize(listSize.width,listSize.height - MAIL_VIEW_EDIT_BOX_HEIGHT));
    m_tabView->setPositionY(MAIL_VIEW_EDIT_BOX_HEIGHT);
    m_tabView->setContentOffset(CCPoint(offset.x, offset.y-MAIL_VIEW_EDIT_BOX_HEIGHT));
//    MailInfo *info = NULL;
//    bool canEdit = false;
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

void MailPopUpView::onDeleteAllMail(CCObject* obj)
{
//    int j = 0;
//    m_tmpUids = "";
//    MailInfo *info = NULL;
//    for (int i=0; i<m_data->count(); i++) {
//        std::string str = dynamic_cast<CCString*>(m_data->objectAtIndex(i))->getCString();
//        info = GlobalData::shared()->mailList[str];
//        if (info && info->tabType!=2 && info->tabType!=3&& info->tabType!=4) {
//            if (info->save!=1) {
//                if (info->rewardId=="" || (info->rewardId!=""&&info->rewardStatus==1)) {
//                    m_tmpUids += "," + info->uid;
//                    j++;
//                }
//            }
//        }
//    }
    if (MailController::getInstance()->canOp()) {
        std::string showDialog = _lang("105570");
        YesNoDialog::show(showDialog.c_str(), CCCallFunc::create(this, callfunc_selector(MailPopUpView::onOKDeleteAllMail)));
    }
    else {
        CCCommonUtils::flyHint("", "", _lang("105599"));
    }
}

void MailPopUpView::onOKDeleteAllMail()
{
    MailController::getInstance()->removeAllOpMails();
}

ssize_t MailPopUpView::numberOfCellsInTableView(CCTableView *table){
    if(m_data->count()==0){
        return 0;
    }else if(m_isLoadMore){
        return m_data->count()+1;
    }else{
        return m_data->count();
    }
    
}
void MailPopUpView::setIsLoadMoreFlag(){
//    int saveCount = 0;
//    int sysCount = 0;
//    int userCount = 0;
//    map<string, MailInfo*>::iterator it;
//    for(it = GlobalData::shared()->mailList.begin(); it != GlobalData::shared()->mailList.end(); it++){
//        if(it->second==NULL){
//            continue;
//        }
//        if(it->second->type== MAIL_FRESHER||it->second->type==ALL_SERVICE||it->second->type==MAIL_SYSNOTICE||it->second->type==MAIL_SYSUPDATE){
//            if(m_panelType==SAVEMAIL){
//                saveCount++;
//            }
//            
//        }else if(it->second->tabType ==USERMAIL&&(it->second->dialogs->count()>0)){
//            //if(m_panelType==USERMAIL){
//            userCount++;
//            // }
//        }else{
//            // if(m_panelType==SYSTEMMAIL){
//            sysCount++;
//            // }
//        }
//    }
//    int total = GlobalData::shared()->mailCountObj.perT+GlobalData::shared()->mailCountObj.sysT;
//    if(total>(userCount+sysCount)){
//        m_isLoadMore = true;
//        if(GlobalData::shared()->mailList.size()==0){
//            addLoadingAni();
//            m_tabView->setBounceable(false);
//            this->scheduleOnce(schedule_selector(MailPopUpView::sendReloadMoreMail),1.0);
//        }
//    }
    MailCountObj& mailCount = GlobalData::shared()->mailCountObj;
    int total = mailCount.perT + mailCount.saveT + mailCount.studioT + mailCount.sysT + mailCount.modT + mailCount.fightT;
    int nowTotal = GlobalData::shared()->mailList.size();
    if(total>nowTotal){
        m_isLoadMore = true;
        if(nowTotal==0){
            addLoadingAni();
            m_tabView->setBounceable(false);
            this->scheduleOnce(schedule_selector(MailPopUpView::sendReloadMoreMail),1.0);
        }
    }
    m_isLoadMore = false;
    
}
void MailPopUpView::onUserMailClick(CCObject *pSender, CCControlEvent event){
    if(USERMAIL!=m_panelType){
        m_isChangeTab =true;
    }else{
        m_isChangeTab = false;
    }
    m_checkBox->setSelect(false);
    m_panelType = USERMAIL;
    currentType = m_panelType;
//    m_userMailBtn->setEnabled(false);
//    m_systemMailBtn->setEnabled(true);
//    m_saveMailBtn->setEnabled(true);
    resetMailDeleteFlag();
    refreshView(NULL);
}

void MailPopUpView::onSystemMailClick(CCObject *pSender, CCControlEvent event){
    if(SYSTEMMAIL!=m_panelType){
        m_isChangeTab =true;
    }else{
        m_isChangeTab = false;
    }
    m_checkBox->setSelect(false);
    m_panelType = SYSTEMMAIL;
    currentType = m_panelType;
//    m_userMailBtn->setEnabled(true);
//    m_systemMailBtn->setEnabled(false);
//    m_saveMailBtn->setEnabled(true);
    resetMailDeleteFlag();
    refreshView(NULL);
}

void MailPopUpView::onSaveMailClick(CCObject *pSender, CCControlEvent event){
    if(SAVEMAIL!=m_panelType){
        m_isChangeTab =true;
    }else{
        m_isChangeTab = false;
    }
    m_checkBox->setSelect(false);
    m_panelType = SAVEMAIL;
    currentType = m_panelType;
//    m_userMailBtn->setEnabled(true);
//    m_systemMailBtn->setEnabled(true);
//    m_saveMailBtn->setEnabled(false);
    resetMailDeleteFlag();
    refreshView(NULL);
}
bool MailPopUpView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_tabView->setBounceable(true);
    return true;
//    if(m_isLoadMore&&(!m_isGetAllContent))
//        return true;
//    else
//        return false;
//    if(isTouchInside(m_bg, pTouch)){
//        return true;
//    }
//    return false;
}
void MailPopUpView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
 
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
        auto cell = dynamic_cast<MailCell*>(m_tabView->cellAtIndex(m_data->count()));
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
void MailPopUpView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(m_isLoadMore){
        float y = (pTouch->getLocation().y - pTouch->getStartLocation().y);
        if(y<10){
            return;
        }
        m_currMinOffsetY = m_tabView->minContainerOffset().y;
        m_currOffsetY = m_tabView->getContentOffset().y;
        if(m_currOffsetY > 30){
            //addLoadingAni();
            m_tabView->setContentOffset(ccp(0, m_currOffsetY));
            m_tabView->setTouchStop(false);
            m_tabView->setBounceable(false);
            this->scheduleOnce(schedule_selector(MailPopUpView::sendReloadMoreMail),0.0);
        }
    }
}
void MailPopUpView::onAllSelected(CCObject *pSender, CCControlEvent event){
    if(m_allSelected==false){
        CCObject *ccObj = NULL;
        CCARRAY_FOREACH(m_data, ccObj){
            CCString *ccstring = dynamic_cast<CCString*>(ccObj);
            if(ccstring && ccstring->getCString()){
                std::string tmp = ccstring->getCString();
                bool isFolder = false;
                auto iter = m_folders.begin();
                while (iter!=m_folders.end()) {
                    std::vector<std::string> tmpVec;
                    CCCommonUtils::splitString((*iter), ",",  tmpVec);
                    if(tmpVec.size()>1){
                        if(tmpVec.at(0).compare(tmp) == 0){
                            int type = atoi(tmpVec.at(1).c_str());
                            MailController::getInstance()->addToOpMails("",type);
                            isFolder=true;
                            break;
                        }
                    }
                    ++iter;
                }
                if(isFolder==false){
                    MailController::getInstance()->addToOpMails(tmp);
                }
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
void MailPopUpView::onDelete(CCObject *pSender, CCControlEvent event){
    onDeleteAllMail(NULL);
    m_allSelected = false;
}
//void MailPopUpView::onReward(CCObject *pSender, CCControlEvent event){
//    MailController::getInstance()->rewardAllOpMails();
//    m_allSelected = false;
//}
void MailPopUpView::onWriteMailBtnClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent){
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
void MailPopUpView::onAddSaveClick(CCObject * pSender, Control::EventType pCCControlEvent){
    CCCommonUtils::flyHint("", "", _lang("E100008"));
}
void MailPopUpView::scrollViewDidScroll(CCScrollView* view)
{
  //  m_currMinOffsetY = m_tabView->minContainerOffset().y;
    float offsetY = m_tabView->getContentOffset().y;
//    if(offsetY>0&&offsetY>m_currMinOffsetY){
//        float y = (pTouch->getLocation().y - pTouch->getStartLocation().y)/5;
//        if(y<m_currMinOffsetY){
//            y += m_currMinOffsetY;
//        }
//        m_tabView->setContentOffset(ccp(0,y));
//    }
//    if(m_isLoadMore && offsetY>-30){
//        float y = 40;
//        if (m_currMinOffsetY>0) {
//            y=m_currMinOffsetY+40;
//        }
//        if(offsetY>=y){
//            m_tabView->setContentOffset(ccp(0,y));
//        }
//        
//    }


}
BGTouchLayer* BGTouchLayer::create(CCSize size){
    BGTouchLayer* ret = new BGTouchLayer(size);
    if(ret&&ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool BGTouchLayer::init(){
    CCLayer::init();
    this->setAnchorPoint(CCPoint(0, 0));
    this->setContentSize(m_size);
    return true;
}
void BGTouchLayer::onEnter(){
    CCLayer::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 3, true);

}

void BGTouchLayer::onExit(){
    setTouchEnabled(false);
    CCLayer::onExit();
}
bool BGTouchLayer::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){

    if(isTouchInside(this, pTouch)){
        return true;
    }
    return false;
}
void BGTouchLayer::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){

    if(isTouchInside(this, pTouch)){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_DELETERECOVER);
    }

}