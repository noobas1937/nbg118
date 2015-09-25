//
//  FacebookMineView.cpp
//  IF
//
//  Created by ganxiaohua on 14-10-16.
//
//

#include "FacebookMineView.h"
#include "LoadWebImg.h"
#include "FBUtilies.h"
#include "PopupViewController.h"
#include "TipsView.h"
#include "FBSendWorkerCommand.h"
#include "SocialController.h"
#include "ParticleController.h"
#include "FacebookInviteView.h"
#include "FBMineDigCommand.h"
#include "FBMineFriendsCommand.h"
#include "FBMineHelpCommand.h"
#include "FBHelpRecordView.h"
#include <spine/Json.h>

static const char* lang1[3] = {"107021","107022","107023"};
static const char* lang2[2] = {"107024","107025"};
static const char* lang3[2] = {"107026","107027"};
static const char* parName[3] = {"D","L","R"};

FacebookMineView::~FacebookMineView()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FBFriendsList);
    //CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FBRequestResult);
}

FacebookMineView:: FacebookMineView(){
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FacebookMineView::afterGetFriendsInfo), MSG_FBFriendsList, NULL);
    //CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FacebookMineView::afterGetRequestResult), MSG_FBRequestResult, NULL);
}

void FacebookMineView::afterGetFriendsInfo(CCObject* param){
    CCArray* friends = dynamic_cast<CCArray*>(param);
    if (friends) {
        m_data->removeAllObjects();
        m_friendDatas->removeAllObjects();
        int num = friends->count();
        for (int i=0; i<num; i++) {
            auto dic = _dict(friends->objectAtIndex(i));
            if(dic){
                FBFriendInfo* info = FBFriendInfo::create();
                info->id = dic->valueForKey("id")->getCString();
                info->name = dic->valueForKey("name")->getCString();
                info->url = CCString::createWithFormat("https://graph.facebook.com/%s/picture?type=square",dic->valueForKey("id")->getCString())->getCString();
                m_data->addObject(info);
                m_friendDatas->addObject(info);
            }
        }
        int gapNum = 5 - m_data->count();
        if(gapNum<=0){
            gapNum = 1;
        }
        for(int i=0;i<gapNum;i++){
            FBFriendInfo* info = FBFriendInfo::create();
            info->id = CC_ITOA(12241111+i);
            info->name = "";
            info->url = "";
            info->add = true;
            m_data->addObject(info);
        }
        m_tableView->reloadData();
        CCLOG("friends count=%d",friends->count());
    }
}

void FacebookMineView::afterGetRequestResult(CCObject* param){
    
}

bool FacebookMineView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg", CCScale9Sprite*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvTxt", CCLabelIF*, this->m_lvTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_workerNum", CCLabelIF*, this->m_workerNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnTime", CCLabelIF*, this->m_btnTime);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnSendTxt", CCLabelIF*, this->m_btnSendTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnSend", CCControlButton*, this->m_btnSend);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_friendNode", CCNode*, this->m_friendNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moveNode", CCNode*, this->m_moveNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_workerNode", CCNode*, this->m_workerNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buttomNode", CCNode*, this->m_buttomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progress", CCScale9Sprite*, this->m_progress);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dialogTxt", CCLabelIF*, this->m_dialogTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_gouGlod", CCSprite*, this->m_gouGlod);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldGouLine", CCScale9Sprite*, this->m_goldGouLine);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mineNode", CCNode*, this->m_mineNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode", CCNode*, this->m_btnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dialogNode", CCNode*, this->m_dialogNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dialogArea", CCNode*, this->m_dialogArea);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_frieNode", CCNode*, this->m_frieNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldTxt", CCLabelIF*, this->m_goldTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_LVInfoBtn", CCControlButton*, this->m_LVInfoBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_FBInfoBtn", CCControlButton*, this->m_FBInfoBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_par1", CCNode*, this->m_par1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_par2", CCNode*, this->m_par2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_smokingNode", CCNode*, this->m_smokingNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnCallHelp", CCControlButton*, this->m_btnCallHelp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnRecord", CCControlButton*, this->m_btnRecord);
    return true;
}

SEL_CCControlHandler FacebookMineView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtnSendClick", FacebookMineView::onBtnSendClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onLVInfoBtnClick", FacebookMineView::onLVInfoBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFBInfoBtnClick", FacebookMineView::onFBInfoBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCallHelpClick", FacebookMineView::onCallHelpClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRecordClick", FacebookMineView::onRecordClick);
    return NULL;
}

CCSize FacebookMineView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    return CCSize(110,155.0);
}

CCSize FacebookMineView::cellSizeForTable(CCTableView *table)
{
    return CCSize(110,155.0);
}

CCTableViewCell* FacebookMineView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    FBFriendCell* cell = (FBFriendCell*)table->dequeueCell();
    if(cell==NULL){
        cell = FBFriendCell::create();
    }
    FBFriendInfo* info = dynamic_cast<FBFriendInfo*>(m_data->objectAtIndex(idx));
    cell->setData(info,m_friendNode);
    return cell;
}

ssize_t FacebookMineView::numberOfCellsInTableView(CCTableView *table)
{
    return m_data->count();
}

//void FacebookMineView::onHelpFriendClick(CCObject * pSender, Control::EventType pCCControlEvent){
//    FBUtilies::removeLike();
//}

void FacebookMineView::onBtnSendClick(CCObject * pSender, Control::EventType pCCControlEvent){
    //FBUtilies::shareFb();
    //FBUtilies::getFBFriendList();
    //FBUtilies::getFriendsData();
    //FBUtilies::fbLike(ccp(200,200));
    //return ;
    
    double mineTime = SocialController::getInstance()->mineTime - GlobalData::shared()->getWorldTime();
    double helpTime = SocialController::getInstance()->helpTime - GlobalData::shared()->getWorldTime();
    if(SocialController::getInstance()->miner!=0 && SocialController::getInstance()->miner == SocialController::getInstance()->xml_miner && (mineTime<=0 || SocialController::getInstance()->mineTime==0)){
        FBMineDigCommand* cmd = new FBMineDigCommand();
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(FacebookMineView::digMineSuccess), NULL));
        cmd->sendAndRelease();
        
    }else if(helpTime<=0){
        m_isSend = true;
        FBSendWorkerCommand* cmd = new FBSendWorkerCommand();
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(FacebookMineView::getWorkersFromServer), NULL));
        cmd->sendAndRelease();
    }
}

void FacebookMineView::digMineSuccess(CCObject* param){
    this->checkCall(NULL);
}

void FacebookMineView::onLVInfoBtnClick(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("107007"),kCCTextAlignmentLeft));
}

void FacebookMineView::onFBInfoBtnClick(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("107014"),kCCTextAlignmentLeft));
}

void FacebookMineView::onCallHelpClick(CCObject * pSender, Control::EventType pCCControlEvent){
    GlobalData::shared()->isBind = true;
    FBUtilies::fbPublishFeedDialog("name","caption","linkDescription","https://www.facebook.com/ClashOfKingsIF","http://i.imgur.com/g3Qc1HN.png");
}

void FacebookMineView::freshWorker(){
    m_scrollView->getContainer()->removeAllChildrenWithCleanup(true);
    m_workerNum->setString(_lang_2("107008", CC_ITOA(SocialController::getInstance()->miner), CC_ITOA(SocialController::getInstance()->xml_miner)));
    int count = 0;
    string _fbUid = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
    for (int i=0; i<m_dataWorkers->count(); i++) {
        FBFriendInfo* info = dynamic_cast<FBFriendInfo*>(m_dataWorkers->objectAtIndex(i));
        info->add = false;
        FBWorkderCell* cell = FBWorkderCell::create();
        cell->setData(info, m_workerNode);
        cell->setPosition(ccp(count*70, 0));
        m_scrollView->addChild(cell);
        count += 1;
    }
    if(SocialController::getInstance()->miner != SocialController::getInstance()->xml_miner){
        FBFriendInfo* info = FBFriendInfo::create();
        info->add = true;
        FBWorkderCell* cell = FBWorkderCell::create();
        cell->setData(info, m_workerNode);
        cell->setPosition(ccp(count*70, 0));
        m_scrollView->addChild(cell);
    }
    
    count = SocialController::getInstance()->miner;
    m_mineNode->removeAllChildrenWithCleanup(true);
    for(int i=0;i<count;i++){
        CCSprite* ren = CCLoadSprite::createSprite("UI_facebook_picture_03.png");
        ren->setPosition(ccp(i*37, 0));
        m_mineNode->addChild(ren);
        if(i!=(count-1)){
            CCSprite* line = CCLoadSprite::createSprite("UI_facebook_line_02.png");
            line->setPosition(ccp(i*35+20, 6));
            m_mineNode->addChild(line);
        }
    }
    if(count>0){
        CCSprite* line = CCLoadSprite::createSprite("UI_facebook_line_03.png");
        line->setPosition(ccp((count-1)*35+44+10, -11));
        m_mineNode->addChild(line);
    }
    this->updateLine();
}

void FacebookMineView::updateLine(){
    int count = SocialController::getInstance()->miner;
    int total = m_addH+180.0;
    int offY = 60;
    int useLen = total - offY;
    float len = count/(1.0*SocialController::getInstance()->xml_miner);
    len = MIN(len, 1);
    int lineLen = useLen*(1-len);
    float time = 0.8;
    float lastLen = m_goldGouLine->getContentSize().height;
    float scaleY = (lineLen+offY)/lastLen;
    if (scaleY>0 && scaleY<1) {
        m_goldGouLine->stopAllActions();
        m_gouGlod->stopAllActions();
        m_goldGouLine->runAction(CCScaleTo::create(time, 1.0,scaleY));
        m_gouGlod->runAction(CCMoveTo::create(time, ccp(m_gouGlod->getPositionX(), m_goldGouLine->getPositionY()-lineLen-42-offY)));
    }else{
        m_goldGouLine->setContentSize(CCSize(11.0,lineLen+offY));
        m_gouGlod->setPositionY(m_goldGouLine->getPositionY()-lineLen-42-offY);
    }
}

void FacebookMineView::onfriendInfoClick(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("107014"),kCCTextAlignmentLeft));
}

void FacebookMineView::onExpHelpClick(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("107007"),kCCTextAlignmentLeft));
}

void FacebookMineView::onRecordClick(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->addPopupInView(FBHelpRecordView::create());
}

bool FacebookMineView::init(){
    bool ret = false;
    if(PopupBaseView::init())
    {
        CCLoadSprite::doResourceByCommonIndex(207, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(207, false);
            
        });
        auto tmpCCB = CCBLoadFile("FacebookMineView", this, this);
        this->setContentSize(tmpCCB->getContentSize());
        int prev = m_viewBg->getContentSize().height;
        changeBGHeight(m_viewBg);
        int add = m_viewBg->getContentSize().height-prev;
        
        m_data = CCArray::create();
        m_dataWorkers = CCArray::create();
        m_friendDatas = CCArray::create();
        
        m_tableView = CCTableView::create(this, m_friendNode->getContentSize());
        m_tableView->setDirection(kCCScrollViewDirectionHorizontal);
        m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tableView->setDelegate(this);
        m_tableView->setTouchPriority(Touch_Default);
        m_friendNode->addChild(m_tableView);
        m_friendNode->setPositionY(m_friendNode->getPositionY()-20);
        m_tableView->reloadData();
        
        m_buttomNode->setPositionY(m_buttomNode->getPositionY() - add);
        m_dialogNode->setPositionY(m_dialogNode->getPositionY() - add);
        
        m_addH = MIN(230.0, add);
        m_bgNode->setPositionY(m_bgNode->getPositionY() - m_addH);
        m_btnTime->setString("");
        CCCommonUtils::setButtonTitle(m_btnSend, _lang("107009").c_str());
        int lv = MIN(GlobalData::shared()->glodMine_fb_k4,SocialController::getInstance()->xml_lv);
        m_lvTxt->setString(_lang_1("107005", CC_ITOA(lv)));
        float len = SocialController::getInstance()->exp*1.0/SocialController::getInstance()->xml_exp;
        len = MAX(len, 1);
        len = MIN(len, 0);
        m_progress->setContentSize(CCSize(250*len,22.0));
        
        m_workerNum->setString(_lang_2("107008", CC_ITOA(SocialController::getInstance()->miner), CC_ITOA(SocialController::getInstance()->xml_miner)));
        
        m_scrollView = CCScrollView::create(m_workerNode->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_scrollView->setTouchPriority(Touch_Popup);
        m_workerNode->addChild(m_scrollView);
        
        int gapNum = 5;
        for(int i=0;i<gapNum;i++){
            FBFriendInfo* info = FBFriendInfo::create();
            info->id = CC_ITOA(12241111+i);
            info->name = "";
            info->url = "";
            info->add = true;
            m_data->addObject(info);
        }
        m_tableView->reloadData();
        
        m_timeCount = 5;
        m_parCount = 0;
        m_parSmokeCount = 0;
        m_isSend = false;
        m_dialogNode->setVisible(false);
        m_goldTxt->setString(_lang_1("107010", CC_ITOA(SocialController::getInstance()->xml_gold)));
        this->getAnimationManager()->runAnimationsForSequenceNamed("Sleep");
        
        CCCommonUtils::setButtonTitle(m_btnCallHelp, _lang("m_btnCallHelp").c_str());
        
        FBMineFriendsCommand* fbCmd = new FBMineFriendsCommand("");
        fbCmd->sendAndRelease();
        
        this->schedule(schedule_selector(FacebookMineView::checkCall), 1);
        this->checkCall(0);
        this->updateLine();
        
        ret = true;
    }
    return ret;
    
}

void FacebookMineView::checkFriendsData(float _time){
    GlobalData::shared()->installFriendsInfo = CCUserDefault::sharedUserDefault()->getStringForKey("installFriendsInfo", "");
    if(m_friendDatas->count()>0){
        this->unschedule(schedule_selector(FacebookMineView::checkFriendsData));
    }
    if(GlobalData::shared()->installFriendsInfo!=""){
        CCLOG(" friends =%s",GlobalData::shared()->installFriendsInfo.c_str());
        Json* fjson = Json_create(GlobalData::shared()->installFriendsInfo.c_str());
        if(fjson){
            m_data->removeAllObjects();
            m_friendDatas->removeAllObjects();
            int size = Json_getSize(fjson);
            for (int i=0; i<size; i++) {
                Json *item = Json_getItemAt(fjson, i);
                string name = Json_getString(item,"name","");
                string id = Json_getString(item,"id","");
                CCLOG("id=%s name=%s",id.c_str(),name.c_str());
                
                FBFriendInfo* info = FBFriendInfo::create();
                info->id = id;
                info->name = name;
                info->url = CCString::createWithFormat("https://graph.facebook.com/%s/picture?type=square",id.c_str())->getCString();
                m_data->addObject(info);
                m_friendDatas->addObject(info);
            }
            Json_dispose(fjson);
            int gapNum = 5 - m_data->count();
            if(gapNum<=0){
                gapNum = 1;
            }
            for(int i=0;i<gapNum;i++){
                FBFriendInfo* info = FBFriendInfo::create();
                info->id = CC_ITOA(12241111+i);
                info->name = "";
                info->url = "";
                info->add = true;
                m_data->addObject(info);
            }
        }
    }
    if(m_friendDatas->count()>0){
        m_tableView->reloadData();
    }
}

void FacebookMineView::checkCall(float _time){
    double helpTime = SocialController::getInstance()->helpTime - GlobalData::shared()->getWorldTime();
    double mineTime = SocialController::getInstance()->mineTime - GlobalData::shared()->getWorldTime();
    m_workerNum->setString(_lang_2("107008", CC_ITOA(SocialController::getInstance()->miner), CC_ITOA(SocialController::getInstance()->xml_miner)));
    string sname = this->getAnimationManager()->getRunningSequenceName();
    m_btnNode->setVisible(mineTime<=0);
    m_workerNode->setVisible(m_btnNode->isVisible());
    m_mineNode->setVisible(m_btnNode->isVisible());
//    if(m_parCount%20==0){
//        m_par2->removeAllChildrenWithCleanup(true);
//        m_par1->removeAllChildrenWithCleanup(true);
//    }
//    if(m_parCount%4==0){
//        for (int i=1; i<3; i++) {
//            string tmpStart = parName[GlobalData::shared()->getRand(0,2)];
//            auto particle = ParticleController::createParticle(CCString::createWithFormat("CityRockfall_%s_%d",tmpStart.c_str(),i)->getCString());
//            particle->setPosition(ccp(GlobalData::shared()->getRand(-200,50),GlobalData::shared()->getRand(-20,20)));
//            int rand = GlobalData::shared()->getRand(0,100);
//            if(rand%2==0){
//                m_par2->addChild(particle);
//            }else{
//                m_par1->addChild(particle);
//            }
//        }
//    }
//    m_parCount += 1;
    
    if(m_timeCount<=0){
        m_dialogNode->setVisible(!m_dialogNode->isVisible());
        if(m_dialogNode->isVisible()){
            if (mineTime>0) {
                m_dialogTxt->setString(_lang(lang3[GlobalData::shared()->getRand(0,1)]));
            }else if(SocialController::getInstance()->miner==SocialController::getInstance()->xml_miner){
                m_dialogTxt->setString(_lang(lang2[GlobalData::shared()->getRand(0,1)]));
            }else{
                m_dialogTxt->setString(_lang(lang1[GlobalData::shared()->getRand(0,2)]));
            }
            m_timeCount = 5;
        }else{
            m_timeCount = 60;
        }
    }
    m_timeCount -= 1;
    if(mineTime>0){
        m_workerNum->setVisible(false);
        m_timeTxt->setString(_lang_1("107017",CC_SECTOA(mineTime)));
        if(sname!="WakeUp"){
            this->getAnimationManager()->runAnimationsForSequenceNamed("WakeUp");
            m_frieNode->removeAllChildrenWithCleanup(true);
            auto particle = ParticleController::createParticle("FlagFire_3");
            m_frieNode->addChild(particle);
        }
    }else{
        m_workerNum->setVisible(true);
        m_frieNode->removeAllChildrenWithCleanup(true);
        m_timeTxt->setString("");
        if(SocialController::getInstance()->miner!=0 && SocialController::getInstance()->miner == SocialController::getInstance()->xml_miner && (mineTime<=0 || SocialController::getInstance()->mineTime==0)){
            CCCommonUtils::setButtonTitle(m_btnSend, _lang("107016").c_str());
            m_btnSendTxt->setString("");
        }else if(helpTime<=0 || SocialController::getInstance()->helpTime==0){
            CCCommonUtils::setButtonTitle(m_btnSend, _lang("107009").c_str());
            m_btnSendTxt->setString("");
        }else{
            m_btnTime->setString(CC_SECTOA((int)helpTime));
            m_btnSendTxt->setString(_lang("107015"));
            CCCommonUtils::setButtonTitle(m_btnSend,"");
        }
        if(sname!="Sleep"){
            this->getAnimationManager()->runAnimationsForSequenceNamed("Sleep");
            FBSendWorkerCommand* cmd = new FBSendWorkerCommand();
            cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(FacebookMineView::getWorkersFromServer), NULL));
            cmd->sendAndRelease();
            m_parSmokeCount = 0;
        }
        m_parSmokeCount += 1;
        if(m_parSmokeCount%4==0){
            auto particle = ParticleController::createParticle("Breath");
            particle->setScale(180);
            m_smokingNode->addChild(particle);
        }else if(m_parSmokeCount%10==0){
            m_smokingNode->removeAllChildrenWithCleanup(true);
        }
    }
}

void FacebookMineView::getWorkersFromServer(CCObject* param){
    NetResult* result = dynamic_cast<NetResult*>(param);
    auto dic = _dict(result->getData());
    string helpInfo = "";
    if(dic){
        if(dic->objectForKey("helpTime")){
            auto helpTime = dic->valueForKey("helpTime")->doubleValue()/1000;
            if(helpTime!=0){
                helpTime = GlobalData::shared()->changeTime(helpTime);
            }
            SocialController::getInstance()->helpTime = helpTime;
        }
        if(dic->objectForKey("miner")){
           int miner = dic->valueForKey("miner")->intValue();
            SocialController::getInstance()->miner = miner;
        }
        if(dic->objectForKey("helpInfo")){
            helpInfo = dic->valueForKey("helpInfo")->getCString();
            parseWorker(helpInfo);
        }
        double helpTime = SocialController::getInstance()->helpTime - GlobalData::shared()->getWorldTime();
        if(helpTime<=0 || SocialController::getInstance()->helpTime==0){
            CCCommonUtils::setButtonTitle(m_btnSend, _lang("107009").c_str());
            m_btnSendTxt->setString("");
        }else{
            m_btnTime->setString(CC_SECTOA((int)helpTime));
            m_btnSendTxt->setString(_lang("107015"));
            CCCommonUtils::setButtonTitle(m_btnSend,"");
        }
    }
    
    this->freshWorker();
    if(m_isSend){
        int count = m_dataWorkers->count();
        if(SocialController::getInstance()->miner != SocialController::getInstance()->xml_miner){
            count = count -1;
        }
        for (int i=1; i<5; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("Appear_%d",i)->getCString());
            particle->setPosition(count*70, 0);
            m_mineNode->addChild(particle);
        }
    }
    m_isSend = false;
}

void FacebookMineView::parseWorker(string workers){
    m_dataWorkers->removeAllObjects();
    if(workers!=""){
        std::vector<std::string> itemArr;
        CCCommonUtils::splitString(workers,"|",itemArr);
        int num = itemArr.size();
        string _fbUid = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
        string _fbName = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERNAME,"");
        for (int i=0; i<num; i++) {
            if(itemArr[i]=="0"){
                FBFriendInfo* info = FBFriendInfo::create();
                info->id = _fbUid;
                info->name = _fbName;
                info->url = CCString::createWithFormat("https://graph.facebook.com/%s/picture?type=square",_fbUid.c_str())->getCString();
                info->add = false;
                m_dataWorkers->addObject(info);
            }else{
                //todo
            }
        }
    }
}

void FacebookMineView::onEnter(){
    CCNode::onEnter();
    this->setTitleName(_lang("107000"));
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FacebookMineView::updateWorkers), "sendSelfToWork", NULL);
    if(GlobalData::shared()->installFriendsInfo==""){
        FBUtilies::getFBFriendList();
    }
    this->unschedule(schedule_selector(FacebookMineView::checkFriendsData));
    this->schedule(schedule_selector(FacebookMineView::checkFriendsData),1);
    this->checkFriendsData(0);
}

void FacebookMineView::onExit(){
    GlobalData::shared()->isBind = false;
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "sendSelfToWork");
    CCNode::onExit();
}

void FacebookMineView::updateWorkers(CCObject* param){
    m_scrollView->getContainer()->removeAllChildrenWithCleanup(true);
    int count = 0;
    for (int i=0; i<m_data->count(); i++) {
        FBFriendInfo* info = dynamic_cast<FBFriendInfo*>(m_data->objectAtIndex(i));
        if(info->isCheck){
            FBWorkderCell* cell = FBWorkderCell::create();
            cell->setData(info, m_workerNode);
            cell->setPosition(ccp(count*70, 0));
            m_scrollView->addChild(cell);
        }
    }
    
    FBFriendInfo* info = FBFriendInfo::create();
    info->add = true;
    FBWorkderCell* cell = FBWorkderCell::create();
    cell->setData(info, m_workerNode);
    cell->setPosition(ccp(count*70, 0));
    m_scrollView->addChild(cell);
}

bool FacebookMineView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){

    return true;
}

void FacebookMineView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_dialogArea, pTouch)){
        this->showTip();
    }
}

void FacebookMineView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint pos = pTouch->getLocation();
}

void FacebookMineView::showTip(){
    string tip = "";
    double mineTime = SocialController::getInstance()->mineTime - GlobalData::shared()->getWorldTime();
//    if (mineTime>0) {
//        tip += _lang(lang3[0]);
//        tip += "\n";
//        tip += _lang(lang3[1]);
//    }else{
//        if(SocialController::getInstance()->miner==SocialController::getInstance()->xml_miner){
//            tip += _lang(lang2[0]);
//            tip += "\n";
//            tip += _lang(lang2[1]);
//        }else{
//            tip += _lang(lang1[0]);
//            tip += "\n";
//            tip += _lang(lang1[1]);
//            tip += "\n";
//            tip += _lang(lang1[2]);
//        }
//    }
    PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("107020"),kCCTextAlignmentLeft));
}


// workder
bool FBWorkderCell::init()
{
    bool ret = true;
    
    if (CCNode::init()) {
        auto tmpCCB = CCBLoadFile("FBWorkerCell", this, this);
        this->setContentSize(tmpCCB->getContentSize());
        
        return true;
    }
    
    return ret;
}

void FBWorkderCell::setData(FBFriendInfo* info,CCNode* clickArea)
{
    m_info = info;
    m_clickArea = clickArea;
    m_head->removeAllChildrenWithCleanup(true);
    m_nameTxt->setString("");
    if(m_info!=NULL && !m_info->add){
        LoadWebImg* img = LoadWebImg::create();
        img->setLoadURL(m_info->url.c_str());
        m_head->addChild(img);
        string name = m_info->name;
        if(name.length()>5){
            name = name.substr(0,5);
            name.append("...");
        }
        m_nameTxt->setString(name);
    }else if(m_info!=NULL && m_info->add){
        CCSprite* spr = CCLoadSprite::createSprite("icon_facebook_jiahao.png");
        m_head->addChild(spr);
    }
}

void FBWorkderCell::onEnter(){
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup_Item, false);
}

void FBWorkderCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

SEL_CCControlHandler FBWorkderCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    
    return NULL;
}

bool FBWorkderCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_head", CCNode*, this->m_head);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIF*, this->m_nameTxt);
    return false;
}

bool FBWorkderCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void FBWorkderCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (isTouchInside(m_clickNode, pTouch) && isTouchInside(m_clickArea, pTouch)) {
        //
        CCLog("FBFriendCell::onTouchEnded");
    }
}

void FBWorkderCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

