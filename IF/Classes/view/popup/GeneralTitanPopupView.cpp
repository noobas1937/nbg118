//
//  GeneralsPopupView.cpp
//  IF
//
//  Created by ganxiaohua on 14-6-11.
//
//

#include "GeneralTitanPopupView.h"
#include "TitanFeedCommand.h"

#include "TitanInfoCommand.h"
#include "BuildUpgradeView.h"
#include "TitanUpgradeView.h"
//#include "GeneralManager.h"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "DataRecordCommand.h"
#include "MainCityView.h"
#include "TitanInView.h"
//#include "ParticleController.h"
//#include "ChangeNickNameView.h"
//#include "RollingText.h"
//#include "ChangePicView.h"
//#include "ShieldUnlockPopUpView.h"
//#include "SettingPopUpView.h"
//#include "ToolController.h"
#include "YesNoDialog.h"
//#include "ChangePicCommand.h"
//#include "WorldController.h"
//#include "RoleInfoView.h"
//#include "AllRankListPopUpView.h"
#include "GeneralSkillListPopUpView.h"
//#include "FunBuildController.h"
//#include "SoundController.h"
#include "EquipmentController.h"
//#include "EquipmentInfo.h"
//#include "AchievementNewView.h"
#include "AchievementController.h"
#include "QueueController.h"
//
//#include "EquipNewUseView.h"
//#include "SceneController.h"
//#include "ImperialScene.h"
//#include "WorldMapView.h"
//
//#include "GetUserInfoCommand.h"
//#include "AllianceInfoView.h"
//#include "InvitesAllianceCommand.h"
//#include "CheckAllianceInfoView.h"
//#include "MailController.h"
//#include "ActivityController.h"
//
////const int cellW = 640;
//static int cellW = 520;

GeneralTitanPopupView* GeneralTitanPopupView::create(){
    GeneralTitanPopupView* ret = new GeneralTitanPopupView();
    ret->m_playerUid = "";
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

//void GeneralsPopupView::createInfoByUid(const string &uid) {
//    if (uid == GlobalData::shared()->playerInfo.uid) {
//        PopupViewController::getInstance()->addPopupInView(GeneralsPopupView::create());
//        return;
//    }
//    
//    auto ret = new GeneralsPopupView();
//    auto cmd = new GetUserInfoCommand(uid);
//    cmd->setCallback(CCCallFuncO::create(ret, callfuncO_selector(GeneralsPopupView::onGetPlayerInfoCallback), nullptr));
//    cmd->sendAndRelease();
//    
//}
//
void GeneralTitanPopupView::onGetPlayerInfoCallback(cocos2d::CCObject *obj) {
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
    
    this->m_playerUid = "";
    auto playerInfo = new PlayerInfo();
    playerInfo->updateInfo(info);
    this->m_playerUid = playerInfo->uid;
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


bool GeneralTitanPopupView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    
    
    
    CCLoadSprite::doResourceByCommonIndex(305, true);
    CCLoadSprite::doResourceByCommonIndex(3, true);
    
    auto bg = CCBLoadFile("GeneralTitanDetail",this,this);
   
    setContentSize(bg->getContentSize());
    
    setIsHDPanel(true);

    CCSize size = CCDirector::sharedDirector()->getWinSize();
    if (!CCCommonUtils::isIosAndroidPad()) {
        //changeBGHeight(m_buildBG);
        CCSize temp = m_buildBG->getContentSize();
        temp.height=size.height;
        m_buildBG->setContentSize(temp);
    }
    float extH = getExtendHeight();
    float add =  size.height - 852;
    if (CCCommonUtils::isIosAndroidPad())
    {
        add =  size.height - 2048;
        extH = size.height - 2048;
    }
    m_bottomNode->setPositionY(m_bottomNode->getPositionY()-extH);
    this->m_receiveGlow->setVisible(false);
    bool isOpenLongJing = false;

//    m_Titan = Titan::create(1);
//    m_Titan->setScale(0.7);
//    m_titanPos->addChild(m_Titan);
    m_titanPosInView->addChild( TitanInView::create());
//    auto sdsds = cocos2d::Label::createWithBMFont("Arial_Bold.fnt", "222221122");
//        this->m_titanPos->addChild(sdsds);
    

 
    m_ProTitanExt->setVisible(true);
    
    this->resetAttribute(nullptr);
    
    this->scheduleUpdate();
    
    CCCommonUtils::setButtonTitle(this->m_speedUpBtn,_lang("104903").c_str());
    

  
    return true;
}
void GeneralTitanPopupView::resetAttribute(CCObject* obj)
{
    CCDictionary *params = dynamic_cast<CCDictionary*>(obj);
//    if (params) {
//       
//        auto titanid = params->valueForKey("titanid");
//        if(titanid->length()!=0)
//        {
//            m_titanId = titanid->intValue();
//            ArmyInfo titanInfo = GlobalData::shared()->armyList[titanid->getCString()]; //fusheng map中通过id获得泰坦属性
//            m_titanAttackTxt->setString(CC_ITOA(titanInfo.attack));
//            m_titanMagicTxt->setString(CC_ITOA(titanInfo.attack));
//            m_Txt1->setString(CC_ITOA(titanInfo.attack));
//            m_Txt2->setString(CC_ITOA(titanInfo.attack));
//        }
//        auto status = params->valueForKey("status");
//        if(status->length()!=0)
//        {
//            
//        }
//        auto level = params->valueForKey("level");
//        if(level->length()!=0)
//        {
//
//
//            long tempTitanLevel = level->longValue();
//            
//            if(tempTitanLevel != this->titanLevel)
//            {
//                titanLevel = tempTitanLevel;
//                auto dict = _dict(LocalController::shared()->DBXMLManager()->getGroupByKey("titan")->objectForKey(CCString::createWithFormat("%ld",titanLevel)->getCString()));
//                this->needfood = dict->valueForKey("needfood")->intValue();
//                this->needexp = dict->valueForKey("needexp")->intValue();
//                this->maxManual= dict->valueForKey("maxmanual")->intValue();
//                this->m_needFood->setString(CCString::createWithFormat("%d",this->needfood)->getCString());
//                long lfood = GlobalData::shared()->resourceInfo.lFood;
//                if(this->needfood>lfood)
//                {
//                    this->m_needFood->setColor(ccColor3B::RED);
//                }
//                else
//                {
//                    this->m_needFood->setColor(ccColor3B::WHITE);
//                }
//                
//                std::vector<std::string> tmpVec;
//                CCCommonUtils::splitString(dict->valueForKey("feedcdtime")->getCString(), ",", tmpVec);
//                this->feedMaxNum = tmpVec.size()+1;
//            }
//
//        }
//        auto exp = params->valueForKey("exp");
//        if(exp->length()!=0)
//        {
//            this->currentExp = exp->intValue();
//            this->m_titanExtTxt->setString(CCString::createWithFormat("%d/%d",this->currentExp,this->needexp)->getCString());
//            
//            float ratio =((float)this->currentExp)/this->needexp;
//            
//            auto size = this->m_ProTitanExt->getContentSize();
//            size.width = 640.0*ratio;
//
//            this->m_ProTitanExt->setContentSize(size);
//            
//        }
//        auto feedcd = params->valueForKey("feedcd");
//        if(feedcd->length()!=0)
//        {
//            auto tmp = feedcd->doubleValue()/1000;
//            if (tmp > 0) {
//                tmp = GlobalData::shared()->changeTime(tmp);
//            }
//            this->feedCD =(float)(tmp - GlobalData::shared()->getWorldTime());
//            if (this->feedCD<0) {
//                this->feedCD=0;
//            }
//            string timeInfo = CC_SECTOA((int)this->feedCD);
//            this->m_feedCDTxt->setString(timeInfo);
//        }
//        auto feednum = params->valueForKey("feednum");
//        if(feednum->length()!=0)
//        {
//            this->feedNum = feednum->intValue();
//        }
//        auto currentmanual = params->valueForKey("maxmanual");
//        if(currentmanual->length()!=0)
//        {
//            this->currentManual = currentmanual->intValue();
//            
//            float ratio =((float)this->currentManual)/this->maxManual;
//            
//            auto size = this->m_ProTiTanAP->getContentSize();
//            size.width = 365.0*ratio;
//            this->m_ProTiTanAP->setContentSize(size);
//            this->m_titanAPTxt->setString(CCString::createWithFormat("AP:%d/%d",this->currentManual,this->maxManual)->getCString());
//        }
//        auto food = params->valueForKey("food");
//        if(food->length()!=0)
//        {
//            this->m_currentFoodNum->setString(CCString::createWithFormat("/%ld",food->longValue())->getCString());
//        }
//        else
//        {
//            long lfood = GlobalData::shared()->resourceInfo.lFood;
//
//            this->m_currentFoodNum->setString(CCString::createWithFormat("/%ld",lfood)->getCString());
//        }
//    }
    
    m_titanInfo = GlobalData::shared()->titanInfo;
    
    
    if(m_titanInfo.titanId=="")
    {
         CCCommonUtils::flyText("数据初始化失败");
        return;
    }
   
    if(m_titanInfo.titanId!="")
    {
        m_titanId = m_titanInfo.titanId;
        ArmyInfo armTitanInfo = GlobalData::shared()->armyList[m_titanId]; //fusheng map中通过id获得泰坦属性
        m_titanAttackTxt->setString(CC_ITOA(armTitanInfo.attack));
        m_titanMagicTxt->setString(CC_ITOA(armTitanInfo.attack));
        m_Txt1->setString(CC_ITOA(armTitanInfo.attack));
        m_Txt2->setString(CC_ITOA(armTitanInfo.attack));
    }
    
    if(m_titanInfo.status==0)
    {
        
    }
    else
    {
        
    }
    
   
//        if(titaiInfo.level!=0)
//        {
//            
//            
//            int tempTitanLevel = titaiInfo.level;
//            
//            if(tempTitanLevel != this->titanLevel)
//            {
//                titanLevel = tempTitanLevel;
//                auto dict = _dict(LocalController::shared()->DBXMLManager()->getGroupByKey("titan")->objectForKey(CCString::createWithFormat("%ld",titanLevel)->getCString()));
//                this->needfood = dict->valueForKey("needfood")->intValue();
//                this->needexp = dict->valueForKey("needexp")->intValue();
//                this->maxManual= dict->valueForKey("maxmanual")->intValue();
//                this->m_needFood->setString(CCString::createWithFormat("%d",this->needfood)->getCString());
//                
//                
//                std::vector<std::string> tmpVec;
//                CCCommonUtils::splitString(dict->valueForKey("feedcdtime")->getCString(), ",", tmpVec);
//                this->feedMaxNum = tmpVec.size()+1;
//            }
//            
//        }
    
    long lfood = GlobalData::shared()->resourceInfo.lFood;
    if(m_titanInfo.feedFoodNum>lfood)
    {
        this->m_needFood->setString(CCString::createWithFormat("%d",m_titanInfo.feedFoodNum)->getCString());
        this->m_needFood->setColor(ccColor3B::RED);
    }
    else
    {
        this->m_needFood->setString(CCString::createWithFormat("%d",m_titanInfo.feedFoodNum)->getCString());
        this->m_needFood->setColor(ccColor3B::WHITE);
    }
    
    
//        auto exp = params->valueForKey("exp");
    if(m_titanInfo.nextExp!=0)
    {
        this->m_titanExtTxt->setString(CCString::createWithFormat("%d/%d",m_titanInfo.exp,m_titanInfo.nextExp)->getCString());
        
        float ratio =((float)m_titanInfo.exp)/m_titanInfo.nextExp;
        
        auto size = this->m_ProTitanExt->getContentSize();
        size.width = 640.0*ratio;
        
        this->m_ProTitanExt->setContentSize(size);
    }
    else
    {
        this->m_titanExtTxt->setString("数据异常");
        

        
        auto size = this->m_ProTitanExt->getContentSize();
        size.width = 0;
        
        this->m_ProTitanExt->setContentSize(size);

    }
    


    if(m_titanInfo.maxManual!=0)
    {
        float ratio =((float)m_titanInfo.currentManual)/m_titanInfo.maxManual;
        
        auto size = this->m_ProTiTanAP->getContentSize();
        size.width = 365.0*ratio;
        this->m_ProTiTanAP->setContentSize(size);
        this->m_titanAPTxt->setString(CCString::createWithFormat("AP:%d/%d",m_titanInfo.currentManual,m_titanInfo.maxManual)->getCString());

    }
    else
    {
        
        auto size = this->m_ProTiTanAP->getContentSize();
        size.width = 0;
        this->m_ProTiTanAP->setContentSize(size);
        this->m_titanAPTxt->setString("数据异常");

    }
    
    if(params)
    {
        
        auto food = params->valueForKey("food");
        if(food->length()!=0)
        {
            this->m_currentFoodNum->setString(CCString::createWithFormat("/%ld",food->longValue())->getCString());
        }
        else
        {
            //long lfood = GlobalData::shared()->resourceInfo.lFood;
            
            this->m_currentFoodNum->setString(CCString::createWithFormat("/%ld",lfood)->getCString());
        }
    }
    else
    {
        long lfood = GlobalData::shared()->resourceInfo.lFood;
        
        this->m_currentFoodNum->setString(CCString::createWithFormat("/%ld",lfood)->getCString());
    }
    
    if(m_titanInfo.feedcd!=0)
    {
        
        this->feedCD =(float)(m_titanInfo.feedcd - GlobalData::shared()->getWorldTime());
        if (this->feedCD<0) {
            this->feedCD=0;
            isUpdating = false;
            this->m_speedUpBtn->setEnabled(false);
            if(m_titanInfo.exp<m_titanInfo.nextExp)
            {
                this->m_titanFeedBtn->setEnabled(true);
            }
            else
            {
                this->m_titanFeedBtn->setEnabled(false);
            }
            
        }
        else
        {
            isUpdating = true;
            this->m_speedUpBtn->setEnabled(true);
            this->m_titanFeedBtn->setEnabled(false);
        }
        string timeInfo = CC_SECTOA((int)this->feedCD);
        this->m_feedCDTxt->setString(timeInfo);
    }
    else
    {
        this->feedCD=0;
        isUpdating = false;
        this->m_speedUpBtn->setEnabled(false);
        if(m_titanInfo.exp<m_titanInfo.nextExp)
        {
            this->m_titanFeedBtn->setEnabled(true);
        }
        else
        {
            this->m_titanFeedBtn->setEnabled(false);
        }
        string timeInfo = CC_SECTOA((int)this->feedCD);
        this->m_feedCDTxt->setString(timeInfo);
    }
    
    
    
//    int qid = CCCommonUtils::getQueueSortByType(15) *1000 + (15 + 1)*100+1;
//    
//    auto iter = GlobalData::shared()->allQueuesInfo.find(qid);
//    
//    if (iter != GlobalData::shared()->allQueuesInfo.end())
//    {
//        
//        auto tmp = iter->second.finishTime/1000;
//        if (tmp > 0) {
//            tmp = GlobalData::shared()->changeTime(tmp);
//        }
//        this->feedCD =(tmp - GlobalData::shared()->getWorldTime());
//        if (this->feedCD<=0) {
//            this->feedCD=0;
//            isUpdating = false;
//            
//        }
//        else
//        {
//            isUpdating = true;
//            
//        }
//        string timeInfo = CC_SECTOA((int)this->feedCD);
//        this->m_feedCDTxt->setString(timeInfo);
//        
//        
//        
//    }
//    else
//    {
//        this->feedCD = 0;
//        string timeInfo = CC_SECTOA((int)this->feedCD);
//        this->m_feedCDTxt->setString(timeInfo);
//    }
}

//void GeneralsPopupView::asyDelayLoad(CCObject* p){
////    m_count += 1;
////    if(m_count==3){
////        m_scrollContainer->setVisible(true);
////        m_resetNode->setVisible(true);
////        refreshView();
////    }
//    m_scrollContainer->setVisible(true);
//    m_resetNode->setVisible(true);
//    refreshView();
//}
//
//void GeneralsPopupView::refreshView(){
//    update(0.0);
//    
//    m_bustPic->removeAllChildren();
//    string pBust = GlobalData::shared()->playerInfo.pic;
//    if (m_info) {
//        pBust = m_info->pic;
//    }
//    auto sprite = CCLoadSprite::createSprite((pBust + "_bust.png").c_str());
//    sprite->setTag(0);
//    sprite->setOpacity(0);
//    m_bustPic->addChild(sprite);
//    sprite->runAction(CCFadeIn::create(0.5));
//    
//    onRefreshEquip();
//}
//
//void GeneralsPopupView::onSetBagTipNode()
//{
//    m_bagTipNode->setVisible(false);
//    m_bagTipNode->stopAllActions();
//    if (!CCCommonUtils::isIosAndroidPad())
//    {
//        m_bagTipNode->setPosition(30, 35);
//    }
//    int rewardNum = AchievementController::getInstance()->getRewardAchieve();
//    if (rewardNum > 0)
//    {
//        m_bagTipNum->setString(CC_ITOA(rewardNum));
//        m_bagTipNode->setVisible(true);
//        CCPoint bagTipPos = m_bagTipNode->getPosition();
//        CCEaseExponentialIn* easeInAction = CCEaseExponentialIn::create(dynamic_cast<CCActionInterval*>(CCMoveTo::create(0.2, ccp(bagTipPos.x,bagTipPos.y+10))));
//        CCFiniteTimeAction* easeInActionRevers =CCMoveTo::create(0.2, ccp(bagTipPos.x,bagTipPos.y));
//
//        CCEaseExponentialIn* easeInAction1 = CCEaseExponentialIn::create(dynamic_cast<CCActionInterval*>(CCMoveTo::create(0.2, ccp(bagTipPos.x, bagTipPos.y-10))));
//        CCMoveTo* easeInAction1Revers = CCMoveTo::create(0.2, ccp(bagTipPos.x, bagTipPos.y));
//        CCRepeat* repeatAction = CCRepeat::create(CCSequence::create(
//                                                                     easeInAction
//                                                                     , easeInActionRevers
//                                                                     , easeInAction1
//                                                                     ,easeInAction1Revers
//                                                                     ,NULL)
//                                                  , 3);
//        CCSequence* ccseq = CCSequence::create(repeatAction,CCDelayTime::create(5),NULL);
//        this->m_bagTipNode->runAction(CCRepeatForever::create(ccseq));
//    }
//}
//
//void GeneralsPopupView::loadResource(){
//    CCLoadSprite::doResourceByCommonIndex(105, true);
//    CCLoadSprite::doResourceByCommonIndex(100, true);
//    CCLoadSprite::doResourceByCommonIndex(305, true);
//    CCLoadSprite::doResourceByCommonIndex(503, true);
//    CCLoadSprite::doResourceByCommonIndex(7, true);
//}
//
void GeneralTitanPopupView::onEnter(){
    PopupBaseView::onEnter();
    TitanInfoCommand* tiCommand = new TitanInfoCommand();
    tiCommand->sendAndRelease();
//    m_count = 0;
//    
//    string pBust = GlobalData::shared()->playerInfo.pic;
//    if (m_info) {
//        pBust = m_info->pic;
//    }
//    m_count = CCCommonUtils::getBustIdxByName(pBust);
//    CCLoadSprite::doLoadResourceAsync(GENERAL_PATH, CCCallFuncO::create(this, callfuncO_selector(GeneralsPopupView::asyDelayLoad), NULL), m_count);
//    
//    UIComponent::getInstance()->showPopupView(1);
//    loadResource();
//    if (m_playerUid=="" || m_playerUid==GlobalData::shared()->playerInfo.uid) {
//        auto &general = GlobalData::shared()->generals.begin()->second;
//        m_pointText->setString(_lang_1("105032", CC_ITOA(GeneralManager::getInstance()->getTotalFreeSkillPoint(general))));
//        m_pointText->setString(CC_ITOA(GeneralManager::getInstance()->getTotalFreeSkillPoint(general)));
//        m_skillNumNode->setVisible(GeneralManager::getInstance()->getTotalFreeSkillPoint(general)>0);
//    }
//    
//    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
   
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GeneralTitanPopupView::resetAttribute), MSG_TITAN_INFORMATION_RESET, NULL);
//    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
//    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GeneralsPopupView::onChangePic), MSG_CHANGE_PLAYER_PIC, NULL);
//    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GeneralsPopupView::updatePlayerName), MSG_CHANGE_PLAYER_NAME, NULL);
//    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GeneralsPopupView::updateEquipTips), "showEquipTips", NULL);
//    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
//    updatePlayerName(NULL);
//    
    onRefreshEquip();
//    onSetBagTipNode();
//    if (!CCCommonUtils::isIosAndroidPad())
//    {
//        m_scrollView->setContentSize(CCSize(m_infoNode->getContentSize().width,880));
//        m_scrollView->setContentOffset(ccp(0, m_infoNode->getContentSize().height - 880));
//        m_scrollView->setTouchEnabled(true);
//    }
//    
//    updateEquipTips(NULL);
}

void GeneralTitanPopupView::onExit(){
//    m_particle7->removeAllChildren();
//    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
//    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TITAN_INFORMATION_RESET);
//    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CHANGE_PLAYER_NAME);
//    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "showEquipTips");
//    resetRankPop();
//    CCLoadSprite::doResourceByCommonIndex(100, false);
////    CCLoadSprite::doResourceByCommonIndex(305, false);
//    CCLoadSprite::doResourceByGeneralIndex(1, false);
//    CCLoadSprite::doResourceByGeneralIndex(2, false);
//    CCLoadSprite::doResourceByGeneralIndex(3, false);
//    CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
    
    PopupBaseView::onExit();
}
//void GeneralsPopupView::onChangePic(CCObject * p){
//    refreshView();
//}
//
//void GeneralsPopupView::updatePlayerName(CCObject* p){
//    std::string name = GlobalData::shared()->playerInfo.name;
//    if (m_info) {
//        name = m_info->name;
//    }
//    m_nameTxt->setString(name.c_str());
//}
//
//void GeneralsPopupView::onChangeName(CCObject * pSender, Control::EventType pCCControlEvent){
//    PopupViewController::getInstance()->addPopupView(ChangeNickNameView::create("200021"));
//}
//
SEL_CCControlHandler GeneralTitanPopupView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSkillBtnClick", GeneralsPopupView::onSkillBtnClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAllianceClick", GeneralsPopupView::onAllianceClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTitanFeedClick", GeneralTitanPopupView::onTitanFeedClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSpeedUpClick", GeneralTitanPopupView::onSpeedUpClick);
    return NULL;
}
//
//void GeneralsPopupView::onAllianceClick(CCObject * pSender, CCControlEvent pCCControlEvent)
//{
//    if (m_info) {
//        PopupViewController::getInstance()->addPopupInView(CheckAllianceInfoView::create(&m_info->allianceInfo));
//    }
//}
//
//void GeneralsPopupView::onMessageClick(CCObject * pSender, CCControlEvent pCCControlEvent)
//{
//    if (m_info) {
//        std::string userName = m_info->name;
//        std::string uid = m_info->uid;
//        MailController::getInstance()->openMailDialogViewFirst(userName, uid);
//    }
//}
//
//void GeneralsPopupView::changePic(){
//    std::string icon = dynamic_cast<CCString*>(m_data->objectAtIndex(m_generalIndex))->getCString();
//    ChangePicCommand* cmd = new ChangePicCommand(icon);
//    cmd->sendAndRelease();
//}
//
//void GeneralsPopupView::onResetPicClick(CCObject * pSender, Control::EventType pCCControlEvent){
//    auto& info = ToolController::getInstance()->getToolInfoById(ITEM_CHANGE_PIC);
//    int gold = CCCommonUtils::getGoldBuyItem(CC_ITOA(ITEM_CHANGE_PIC));
//    if(info.getCNT() == 0 && !CCCommonUtils::isEnoughResourceByType(Gold, gold)){
////        CCCommonUtils::flyText(_lang("104912"));
//        YesNoDialog::gotoPayTips();
//        return;
//    }
//    changePic();
//    if(info.getCNT() > 0){
//        ToolController::getInstance()->useTool(ITEM_CHANGE_PIC, 1, false);
//    }
//}
//
//void GeneralsPopupView::onSkillBtnClick(CCObject * pSender, Control::EventType pCCControlEvent){
//    onSkillClick(NULL,Control::EventType::TOUCH_DOWN);
//}
//
bool GeneralTitanPopupView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanAttackTxt", CCLabelIF*, this->m_titanAttackTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanMagicTxt", CCLabelIF*, this->m_titanMagicTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_Txt1", CCLabelIF*, this->m_Txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_Txt2", CCLabelIF*, this->m_Txt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanExtTxt", CCLabelIF*, this->m_titanExtTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_feedCDTxt", CCLabelIF*, this->m_feedCDTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_needFood", CCLabelIF*, this->m_needFood);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_currentFoodNum", CCLabelIF*, this->m_currentFoodNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanFeedBtn", CCControlButton*, this->m_titanFeedBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_speedUpBtn", CCControlButton*, this->m_speedUpBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodStatus", CCSprite*, this->m_foodStatus);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanDetail", CCSprite*, this->m_titanDetail);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanSkill", CCSprite*, this->m_titanSkill);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanUngrade", CCSprite*, this->m_titanUngrade);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bustPic", CCNode*, this->m_bustPic);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon", CCNode*, this->m_icon);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchLayer", CCNode*, this->m_touchLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bNode", CCNode*, this->m_bNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", Label*, this->m_nameTxt);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pointText", CCLabelIF*, this->m_pointText);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_expTxt", CCLabelIF*, this->m_expTxt);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_expBar", CCScale9Sprite*, this->m_expBar);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftBg", CCSprite*, this->m_leftBg);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightBg", CCSprite*, this->m_rightBg);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_renameBtn", CCControlButton*, this->m_renameBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bTxt3", CCLabelIF*, this->m_bTxt3);
////    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bTxt4", CCLabelIF*, this->m_bTxt4);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bTxt5", CCLabelIF*, this->m_bTxt5);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bTxt6", CCLabelIF*, this->m_bTxt6);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode1", CCNode*, this->m_clickNode1);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode2", CCNode*, this->m_clickNode2);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode3", CCNode*, this->m_clickNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode4", CCNode*, this->m_clickNode4);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode5", CCNode*, this->m_clickNode5);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode6", CCNode*, this->m_clickNode6);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode7", CCNode*, this->m_clickNode7);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headIcon", CCNode*, this->m_headIcon);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTouch", CCNode*, this->m_nameTouch);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scrollContainer", CCNode*, this->m_scrollContainer);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnText", CCLabelIF*, this->m_btnText);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldNum", CCLabelIF*, this->m_goldNum);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resetPicBtn", CCControlButton*, this->m_resetPicBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldIcon", CCSprite*, this->m_goldIcon);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resetNode", CCNode*, this->m_resetNode);
////    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picCover", CCScale9Sprite*, this->m_picCover);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_staminePro", CCScale9Sprite*, this->m_staminePro);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stamineBG", CCScale9Sprite*, this->m_stamineBG);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stamineText", CCLabelIF*, this->m_stamineText);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stamineIcon", CCSprite*, this->m_stamineIcon);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
//    
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoNode", CCNode*, this->m_infoNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ProTitanExt", CCScale9Sprite*, this->m_ProTitanExt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ProTiTanAP", CCScale9Sprite*, this->m_ProTiTanAP);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanPosInView", CCNode*, this->m_titanPosInView);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanAPTxt", CCLabelIF*, this->m_titanAPTxt);
////    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_skillBtn", CCControlButton*, this->m_skillBtn);
//    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipNode1", CCNode*, this->m_equipNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipNode2", CCNode*, this->m_equipNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipNode3", CCNode*, this->m_equipNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipNode4", CCNode*, this->m_equipNode4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipNode5", CCNode*, this->m_equipNode5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipNode6", CCNode*, this->m_equipNode6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipNode7", CCNode*, this->m_equipNode7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipNode8", CCNode*, this->m_equipNode8);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eBgSpr1", CCSprite*, this->m_eBgSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eBgSpr2", CCSprite*, this->m_eBgSpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eBgSpr3", CCSprite*, this->m_eBgSpr3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eBgSpr4", CCSprite*, this->m_eBgSpr4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eBgSpr5", CCSprite*, this->m_eBgSpr5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eBgSpr6", CCSprite*, this->m_eBgSpr6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eBgSpr7", CCSprite*, this->m_eBgSpr7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eBgSpr8", CCSprite*, this->m_eBgSpr8);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particle7", CCNode*, this->m_particle7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particle8", CCNode*, this->m_particle8);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_red1Pt", CCSprite*, this->m_red1Pt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_red2Pt", CCSprite*, this->m_red2Pt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_red3Pt", CCSprite*, this->m_red3Pt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_red4Pt", CCSprite*, this->m_red4Pt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_red5Pt", CCSprite*, this->m_red5Pt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_red6Pt", CCSprite*, this->m_red6Pt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_red7Pt", CCSprite*, this->m_red7Pt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_red8Pt", CCSprite*, this->m_red8Pt);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipLabel1", CCLabelIF*, this->m_equipLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipLabel2", CCLabelIF*, this->m_equipLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipLabel3", CCLabelIF*, this->m_equipLabel3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipLabel4", CCLabelIF*, this->m_equipLabel4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipLabel5", CCLabelIF*, this->m_equipLabel5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipLabel6", CCLabelIF*, this->m_equipLabel6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipLabel7", CCLabelIF*, this->m_equipLabel7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipLabel8", CCLabelIF*, this->m_equipLabel8);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eLvBg1", CCScale9Sprite*, this->m_eLvBg1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eLvBg2", CCScale9Sprite*, this->m_eLvBg2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eLvBg3", CCScale9Sprite*, this->m_eLvBg3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eLvBg4", CCScale9Sprite*, this->m_eLvBg4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eLvBg5", CCScale9Sprite*, this->m_eLvBg5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eLvBg6", CCScale9Sprite*, this->m_eLvBg6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eLvBg7", CCScale9Sprite*, this->m_eLvBg7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eLvBg8", CCScale9Sprite*, this->m_eLvBg8);
//
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_skillNumNode", CCNode*, this->m_skillNumNode);
//    
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bagTipNode", CCNode*, this->m_bagTipNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bagTipNum", CCLabelIF*, this->m_bagTipNum);
//    
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selfInfoNode", CCNode*, this->m_selfInfoNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_otherNode", CCNode*, this->m_otherNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_medalNode", CCNode*, this->m_medalNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_expOthBar", CCScale9Sprite*, this->m_expOthBar);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_expOthTxt", CCLabelIF*, this->m_expOthTxt);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvOthTxt", CCLabelIF*, this->m_lvOthTxt);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_medalLable", CCLabelIF*, this->m_medalLable);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgBtn", CCControlButton*, this->m_msgBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnAlliance", CCControlButton*, this->m_btnAlliance);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noMedalLable", CCLabelIF*, this->m_noMedalLable);
//    
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_medalParticleNode", CCNode*, this->m_medalParticleNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_starNode", CCNode*, this->m_starNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_speStarNode", CCNode*, this->m_speStarNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_speStarTTF", CCLabelIF*, this->m_speStarTTF);
//    
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipTipNode", CCNode*, this->m_equipTipNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipTipLabel", CCLabelIF*, this->m_equipTipLabel);
//    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_receiveGlow", CCNode*, this->m_receiveGlow);
//
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lockL", CCSprite*, this->m_lockL);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lockR", CCSprite*, this->m_lockR);
//    
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_button", CCScale9Sprite*, this->m_button);
    
    return false;
}
//
//CCNode* GeneralsPopupView::getGuideNode(string _key)
//{
//    if (_key == "General_set") {
////        return m_clickNode3;
//        return m_button;
//    }
//    return NULL;
//}
//
void GeneralTitanPopupView::update(float time){
//    int currt = WorldController::getInstance()->currentStamine;
//    int total = GlobalData::shared()->worldConfig.stamineMax;
//    m_stamineText->setString(std::string("") + CC_ITOA(currt) + "/" + CC_ITOA(total));
//    currt = MIN(currt, total);
//    float s = 1.0 * currt / total;
//    m_staminePro->setScaleX(s);
//    m_tip->update(time);
    
    if(!isUpdating)
    {
        return;
    }
    
    if (this->feedCD>0) {
        this->feedCD -= time;
        string timeInfo = CC_SECTOA((int)this->feedCD);
        this->m_feedCDTxt->setString(timeInfo);
        
    }
    else
    {
        if(m_titanInfo.exp<m_titanInfo.nextExp)
        {
            this->m_titanFeedBtn->setEnabled(true);
        }
        else
        {
            this->m_titanFeedBtn->setEnabled(false);
        }
        
        this->m_feedCDTxt->setString("00:00:00");
        
       
        this->m_speedUpBtn->setEnabled(false);
    }
    
    return;
}
//
void GeneralTitanPopupView::onSkillClick(CCObject * pSender, Control::EventType pCCControlEvent){
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(GeneralTitanPopupView::animationFunc));
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
    
//    CCNode* pic = m_bustPic->getChildByTag(0);
//    if (pic) {
//        CCSprite* icon = dynamic_cast<CCSprite*>(pic);
//        if (icon) {
//            icon->runAction(CCFadeOut::create(0.2));
//        }
//    }
    
//    m_pic->runAction(CCFadeOut::create(0.2f));
}
void GeneralTitanPopupView::onTitanFeedClick(CCObject * pSender, Control::EventType pCCControlEvent){
    if(m_titanInfo.exp>=m_titanInfo.nextExp)
    {
        CCCommonUtils::flyText("please ungrade titan!");
        return;
    }
    if(m_titanInfo.feedNum<m_titanInfo.feedMaxNum)
    {
//        if (this->feedCD<=0) {
//            TitanFeedCommand *tfCommand = new TitanFeedCommand();
//            tfCommand->sendAndRelease();
//        }
        TitanFeedCommand *tfCommand = new TitanFeedCommand();
        tfCommand->sendAndRelease();
        
    }
    else
    {
        
    }

}

void GeneralTitanPopupView::onSpeedUpClick(CCObject * pSender, Control::EventType pCCControlEvent){


    
//    int tmpTime = GlobalData::shared()->allQueuesInfo[qid].finishTime - GlobalData::shared()->getWorldTime();
    YesNoDialog::showTime( _lang("102120").c_str() , CCCallFunc::create(this, callfunc_selector(GeneralTitanPopupView::speedUpCallBack)), (int)this->feedCD, _lang("104903").c_str());
    
    
}
void GeneralTitanPopupView::speedUpCallBack()
{
    
    int qid = CCCommonUtils::getQueueSortByType(15) *1000 + (15 + 1)*100+1;
    
    int gold = CCCommonUtils::getGoldByTime((int)this->feedCD);
    if( gold <= GlobalData::shared()->playerInfo.gold ){
        QueueController::getInstance()->startCCDQueue(qid, "", false, gold,"",1);
        //this->removeFromParent();
    }
    else {
        YesNoDialog::gotoPayTips();
    }
}

//
//void GeneralsPopupView::fadeOut(){
//    m_receiveGlow->removeFromParent();
//    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(GeneralsPopupView::animationFunc1));
//    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
//    
//    CCNode* pic = m_bustPic->getChildByTag(0);
//    if (pic) {
//        CCSprite* icon = dynamic_cast<CCSprite*>(pic);
//        if (icon) {
//            icon->runAction(CCFadeOut::create(0.2));
//        }
//    }
//    
////    m_pic->runAction(CCFadeOut::create(0.2f));
//}
//
void GeneralTitanPopupView::animationFunc(){
    this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
    PopupViewController::getInstance()->addPopupInView(GeneralSkillListPopUpView::create(&GlobalData::shared()->generals.begin()->second,"50000"));
}
//
//void GeneralsPopupView::animationFunc1(){
//    this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
//    PopupViewController::getInstance()->goBackPopupView();
//}
//
bool GeneralTitanPopupView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
//    m_TouchMoved = false;
//    m_TouchPoint = pTouch->getLocation();
//    if(isTouchInside(m_touchNode, pTouch) && isTouchInside(m_infoNode, pTouch)){
//        if ((m_playerUid=="" || m_playerUid==GlobalData::shared()->playerInfo.uid)) {
//            this->m_tip->setVisible(true);
//            if (CCCommonUtils::isIosAndroidPad()) {
//                this->m_tip->setPosition(ccp(-350, -250));
//            }
//            else
//                this->m_tip->setPosition(ccp(-150, -350));
//        }
//    }
//    if (m_info) {
//        if(isTouchInside(m_eBgSpr1, pTouch)){
//            onShowEquipTip(0);
//        }else if(isTouchInside(m_eBgSpr2, pTouch)){
//            onShowEquipTip(1);
//        }else if(isTouchInside(m_eBgSpr3, pTouch)){
//            onShowEquipTip(2);
//        }else if(isTouchInside(m_eBgSpr4, pTouch)){
//            onShowEquipTip(3);
//        }else if(isTouchInside(m_eBgSpr5, pTouch)){
//            onShowEquipTip(4);
//        }else if(isTouchInside(m_eBgSpr6, pTouch)){
//            onShowEquipTip(5);
//        }else if(isTouchInside(m_eBgSpr7, pTouch)){
//            onShowEquipTip(6);
//        }else if(isTouchInside(m_eBgSpr8, pTouch)){
//            onShowEquipTip(7);
//        }
//    }
//    m_x = 1000;
//    m_startY = pTouch->getLocation().y;
//    return true;
    
    
//    if(isTouchInside(m_titanFeed, pTouch))
//    {
//        return true;
//    }
    if(isTouchInside(this->m_titanUngrade, pTouch))
    {
        if (m_titanId == "") {
            return false;
        }
        PopupViewController::getInstance()->addPopupInView(TitanUpgradeView::create(400000000,CCString::create(m_titanId)->intValue()));
        return true;
    }
    
    if(isTouchInside(this->m_titanSkill, pTouch)){
        onSkillClick(NULL, CCControlEvent::TOUCH_DOWN);
        
        CCArray* p = CCArray::create();
        p->addObject(CCInteger::create(GENERAL_OPEN));
        CCArray* d = CCArray::create();
        
        DataRecordCommand* cmd = new DataRecordCommand(OPEN_PANEL, p, d);
        cmd->sendAndRelease();
        //        m_particleNode->removeAllChildren();
        return true;

        
    }
    if(isTouchInside(this->m_titanDetail, pTouch))
    {
        PopupViewController::getInstance()->addPopupInView(MainCityView::create(400000000));
        return true;

    }
    
    return true;
    
}

void GeneralTitanPopupView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
//    m_TouchMoved = true;
//    Vec2 oldPoint = m_TouchPoint;
//    m_TouchPoint = pTouch->getLocation();
//    float dtx = m_TouchPoint.x - oldPoint.x;
//    CCLOG("Titan touch dtx:%f",dtx);
//    m_Titan->onRotationY(dtx);
//    return;
}

void GeneralTitanPopupView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
//    if (m_TouchMoved) {
//        m_TouchMoved = false;
//        return;
//    }



    
//    if(isTouchInside(this->mg, pTouch))
//    {
//    }
    
    
//    if(this->m_tip->isVisible()){
//        this->m_tip->setVisible(false);
//        return;
//    }
//    
//    if(this->m_equipTipNode->isVisible()){
//        this->m_equipTipNode->setVisible(false);
//        return;
//    }
//    
//    int endY = pTouch->getLocation().y;
//    if (abs(endY - m_startY) >= 20) {
//        return;
//    }
//    
//    if (m_info) {
//        if(isTouchInside(m_clickNode4, pTouch)){
//            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
//            PopupViewController::getInstance()->addPopupInView(RoleInfoView::create(m_info,0,m_achievePro,m_medalCntStr,m_medalIconId));
//        }
//    }
//    else {
//        if(isTouchInside(m_clickNode7, pTouch)){
//            onSkillClick(NULL, CCControlEvent::TOUCH_DOWN);
//            
//            CCArray* p = CCArray::create();
//            p->addObject(CCInteger::create(GENERAL_OPEN));
//            CCArray* d = CCArray::create();
//            
//            DataRecordCommand* cmd = new DataRecordCommand(OPEN_PANEL, p, d);
//            cmd->sendAndRelease();
//            m_particleNode->removeAllChildren();
//
//    }else if(isTouchInside(m_nameTouch, pTouch)){
//        onChangeName(NULL,Control::EventType::TOUCH_DOWN);
//    }else if(isTouchInside(m_infoNode, pTouch)){
//        if(isTouchInside(m_eBgSpr1, pTouch)){
//            onClickEquip(0);
//        }else if(isTouchInside(m_eBgSpr2, pTouch)){
//            onClickEquip(1);
//        }else if(isTouchInside(m_eBgSpr3, pTouch)){
//            onClickEquip(2);
//        }else if(isTouchInside(m_eBgSpr4, pTouch)){
//            onClickEquip(3);
//        }else if(isTouchInside(m_eBgSpr5, pTouch)){
//            onClickEquip(4);
//        }else if(isTouchInside(m_eBgSpr6, pTouch)){
//            onClickEquip(5);
//        }else if(isTouchInside(m_eBgSpr7, pTouch)){
//            onClickEquip(6);
//        }else if(isTouchInside(m_eBgSpr8, pTouch)){
//            onClickEquip(7);
//        }else if(isTouchInside(m_clickNode4, pTouch)){
//                SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
//                PopupViewController::getInstance()->addPopupInView(RoleInfoView::create(&(GlobalData::shared()->playerInfo)));
//            }
//        }else if(isTouchInside(m_clickNode2, pTouch)){
//            PopupViewController::getInstance()->addPopupView(ChangePicView::create());
//        }else if(isTouchInside(m_clickNode3, pTouch)){
//            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
//            PopupViewController::getInstance()->addPopupInView(SettingPopUpView::create());
//            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("General_set"));
//        }else if(isTouchInside(m_clickNode5, pTouch)){
//            int configLv = GlobalData::shared()->userLvAboutInfo.ranking_k3;
//            int dabenLv = FunBuildController::getInstance()->getMainCityLv();
//            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
//            if(dabenLv>=configLv){
//                if(m_addPopup == true)
//                    return;
//                m_addPopup = true;
//                CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GeneralsPopupView::resetRankPop),MSG_POPUP_VIEW_IN, NULL);
//                PopupViewController::getInstance()->addPopupInView(AllRankListPopUpView::create());
//            }else{
//                CCCommonUtils::flyHint("quest_icon_1.png", "", _lang_1("108111",CC_ITOA(configLv)));
//            }
//        }else if(isTouchInside(m_clickNode6, pTouch)){
//            // tao.yu 第一版本不开放成就功能
//            CCCommonUtils::flyHint("", "", _lang("E100008"));
////            if(AchievementController::getInstance()->openFlag == FUNCTION_ON){
////                AchievementController::getInstance()->firstOpenPopup();
////                PopupViewController::getInstance()->addPopupInView(AchievementNewView::create());
////            }
//        }else{
//        }
//    }
}
//void GeneralsPopupView::resetRankPop(CCObject *obj){
//    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_POPUP_VIEW_IN);
//    m_addPopup = false;
//}
void GeneralTitanPopupView::onRefreshEquip()
{
    CCLoadSprite::doResourceByCommonIndex(100, true);
    m_equipNode1->removeAllChildren();
    m_equipNode2->removeAllChildren();
    m_equipNode3->removeAllChildren();
    m_equipNode4->removeAllChildren();
    m_equipNode5->removeAllChildren();
    m_equipNode6->removeAllChildren();
    m_equipNode7->removeAllChildren();
    m_equipNode8->removeAllChildren();
    m_EquipMap.clear();
    
//    m_red1Pt->setVisible(false);
//    m_red2Pt->setVisible(false);
//    m_red3Pt->setVisible(false);
//    m_red4Pt->setVisible(false);
//    m_red5Pt->setVisible(false);
//    m_red6Pt->setVisible(false);
//    m_red7Pt->setVisible(false);
//    m_red8Pt->setVisible(false);
    
    
    m_eLvBg1->setVisible(false);
    m_eLvBg2->setVisible(false);
    m_eLvBg3->setVisible(false);
    m_eLvBg4->setVisible(false);
    m_eLvBg5->setVisible(false);
    m_eLvBg6->setVisible(false);
    m_eLvBg7->setVisible(false);
    m_eLvBg8->setVisible(false);
    
    
    
    m_equipLabel1->setString("");
    m_equipLabel2->setString("");
    m_equipLabel3->setString("");
    m_equipLabel4->setString("");
    m_equipLabel5->setString("");
    m_equipLabel6->setString("");
    m_equipLabel7->setString("");
    m_equipLabel8->setString("");
    
    return;
    
    //打开屏蔽装备
    if(m_info&&(!m_info->showEquip)){
        for (int i=0; i<=7; i++) {
            string siteIcon = "";
            if (i==0) {
                siteIcon = "icon_weapons_close.png";
            }else if (i==1) {
                siteIcon = "icon_peeping_close.png";
            }else if (i==2) {
                siteIcon = "icon_clothes_close.png";
            }else if (i==3) {
                siteIcon = "icon_trousers_close.png";
            }else if (i==4) {
                siteIcon = "icon_shoes_close.png";
            }else if (i==5) {
                siteIcon = "icon_ring_close.png";
            }else if (i==6) {
                siteIcon = "icon_long_close.png";
            }
//            else if (i==7) {
//                siteIcon = "icon_long_close.png";
//            }
            auto icon = CCLoadSprite::createSprite(siteIcon.c_str(), true,CCLoadSpriteType_DEFAULT);
            
            if (i==0) {
                m_equipNode1->addChild(icon);
                m_red1Pt->setVisible(false);
                m_eLvBg1->setVisible(false);
            }else if (i==1) {
                m_equipNode2->addChild(icon);
                m_red2Pt->setVisible(false);
                m_eLvBg2->setVisible(false);
            }else if (i==2) {
                m_equipNode3->addChild(icon);
                m_red3Pt->setVisible(false);
                m_eLvBg3->setVisible(false);
            }else if (i==3) {
                m_equipNode4->addChild(icon);
                m_red4Pt->setVisible(false);
                m_eLvBg4->setVisible(false);
            }else if (i==4) {
                m_equipNode5->addChild(icon);
                m_red5Pt->setVisible(false);
                m_eLvBg5->setVisible(false);
            }else if (i==5) {
                m_equipNode6->addChild(icon);
                m_red6Pt->setVisible(false);
                m_eLvBg6->setVisible(false);
            }else if (i==6) {
                if (isOpenLongJing) {
                    m_equipNode7->addChild(icon);
                    m_red7Pt->setVisible(false);
                    m_eLvBg7->setVisible(false);
                }
            }
//            else if (i==7) {
//                m_equipNode8->addChild(icon);
//                m_red8Pt->setVisible(false);
//                m_eLvBg8->setVisible(false);
//            }
        }
        return;
    }//打开屏蔽装备
    
    vector<int> tmpEquip;
    if (m_info) {
        tmpEquip = m_info->m_equip;
    }else {
        map<string, int>::iterator it = EquipmentController::getInstance()->m_MyOnEquip.begin();
        for (; it != EquipmentController::getInstance()->m_MyOnEquip.end(); it++) {
            auto& info = EquipmentController::getInstance()->EquipmentInfoMap[it->second];
            tmpEquip.push_back(it->second);
            m_EquipMap[info.site] = it->first;
        }
    }
    
    map<int, int> tmpMap;
    for (int j=0; j<tmpEquip.size() ; j++) {
        auto& info = EquipmentController::getInstance()->EquipmentInfoMap[tmpEquip[j]];
        int site = info.site;
        m_EquipSiteMap[site] = tmpEquip[j];
        
        string bgPath = CCCommonUtils::getToolBgByColor(info.color);
        auto bg = CCLoadSprite::createSprite(bgPath.c_str());
        CCCommonUtils::setSpriteMaxSize(bg, 90, true);
        
        string bgKuangStr="";
        if (info.color==WHITE) {
            bgKuangStr="icon_kuang_bai.png";
        }else if (info.color==GREEN){
            bgKuangStr="icon_kuang_lv.png";
        }else if (info.color==BLUE){
            bgKuangStr="icon_kuang_lan.png";
        }else if (info.color==PURPLE){
            bgKuangStr="icon_kuang_zi.png";
        }else if (info.color==ORANGE){
            bgKuangStr="icon_kuang_cheng.png";
        }else if (info.color==GOLDEN){
            bgKuangStr="icon_kuang_jin.png";
        }
        CCSprite *bgKuang=NULL;
        if (bgKuangStr!="") {
            bgKuang = CCLoadSprite::createSprite(bgKuangStr.c_str());
        }
        
        string iconPath = CCCommonUtils::getIcon(CC_ITOA(tmpEquip[j]));
        auto icon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_EQUIP);
        CCCommonUtils::setSpriteMaxSize(icon, 90, true);
        
        string lvMsg = "Lv.";
        lvMsg = lvMsg + CC_ITOA(info.level);
        tmpMap[site] = 1;
        if (site==0) {
            m_equipNode1->addChild(bg);
            m_equipNode1->addChild(icon);
            m_equipNode1->addChild(bgKuang);
            m_equipLabel1->setString(lvMsg.c_str());
        }else if (site==1) {
            m_equipNode2->addChild(bg);
            m_equipNode2->addChild(icon);
            m_equipNode2->addChild(bgKuang);
            m_equipLabel2->setString(lvMsg.c_str());
        }else if (site==2) {
            m_equipNode3->addChild(bg);
            m_equipNode3->addChild(icon);
            m_equipNode3->addChild(bgKuang);
            m_equipLabel3->setString(lvMsg.c_str());
        }else if (site==3) {
            m_equipNode4->addChild(bg);
            m_equipNode4->addChild(icon);
            m_equipNode4->addChild(bgKuang);
            m_equipLabel4->setString(lvMsg.c_str());
        }else if (site==4) {
            m_equipNode5->addChild(bg);
            m_equipNode5->addChild(icon);
            m_equipNode5->addChild(bgKuang);
            m_equipLabel5->setString(lvMsg.c_str());
        }else if (site==5) {
            m_equipNode6->addChild(bg);
            m_equipNode6->addChild(icon);
            m_equipNode6->addChild(bgKuang);
            m_equipLabel6->setString(lvMsg.c_str());
        }else if (site==6) {            
            m_equipNode7->addChild(bg);
            m_equipNode7->addChild(icon);
            m_equipNode7->addChild(bgKuang);
            m_equipLabel7->setString(lvMsg.c_str());
            
            //特效 白绿蓝紫橙金
            m_particle7->removeAllChildren();
            if(1==info.color){//绿
                int part = 3;
                for (int par=1; par<=5; par++) {
                    auto particle7 = ParticleController::createParticle(CCString::createWithFormat("Cristal_%d_%d",part,par)->getCString());
                    particle7->setScale(2.1f);
                    if (CCCommonUtils::isIosAndroidPad())
                    {
                        particle7->setScale(4.0f);
                    }
                    m_particle7->addChild(particle7);
                }
            }else if (2==info.color){//蓝
                int part = 1;
                for (int par=1; par<=5; par++) {
                    auto particle7 = ParticleController::createParticle(CCString::createWithFormat("Cristal_%d_%d",part,par)->getCString());
                    particle7->setScale(2.1f);
                    if (CCCommonUtils::isIosAndroidPad())
                    {
                        particle7->setScale(4.0f);
                    }
                    m_particle7->addChild(particle7);
                }
            }else if (3==info.color){//紫
                int part = 4;
                for (int par=1; par<=5; par++) {
                    auto particle7 = ParticleController::createParticle(CCString::createWithFormat("Cristal_%d_%d",part,par)->getCString());
                    particle7->setScale(2.1f);
                    if (CCCommonUtils::isIosAndroidPad())
                    {
                        particle7->setScale(4.0f);
                    }
                    m_particle7->addChild(particle7);
                }
            }else if (4==info.color){//橙
                int part = 5;
                for (int par=1; par<=5; par++) {
                    auto particle7 = ParticleController::createParticle(CCString::createWithFormat("Cristal_%d_%d",part,par)->getCString());
                    particle7->setScale(2.1f);
                    if (CCCommonUtils::isIosAndroidPad())
                    {
                        particle7->setScale(4.0f);
                    }
                    m_particle7->addChild(particle7);
                }
            }else if (5==info.color){//金
                int part = 2;
                for (int par=1; par<=5; par++) {
                    auto particle7 = ParticleController::createParticle(CCString::createWithFormat("Cristal_%d_%d",part,par)->getCString());
                    particle7->setScale(2.1f);
                    if (CCCommonUtils::isIosAndroidPad())
                    {
                        particle7->setScale(4.0f);
                    }
                    m_particle7->addChild(particle7);
                }
            }
        }
//        else if (site==7) {
//            m_equipNode8->addChild(bg);
//            m_equipNode8->addChild(icon);
//            m_equipNode8->addChild(bgKuang);
//            m_equipLabel8->setString(lvMsg.c_str());
//        }
    }
    
    
    for (int i=0; i<=7; i++) {
        if (tmpMap.find(i) == tmpMap.end()) {
            string siteIcon = "";
            if (i==0) {
                siteIcon = "icon_weapons.png";
            }else if (i==1) {
                siteIcon = "icon_peeping.png";
            }else if (i==2) {
                siteIcon = "icon_clothes.png";
            }else if (i==3) {
                siteIcon = "icon_trousers.png";
            }else if (i==4) {
                siteIcon = "icon_shoes.png";
            }else if (i==5) {
                siteIcon = "icon_ring.png";
            }else if (i==6) {
                siteIcon = "icon_long_close.png";
            }
//            else if (i==7) {
//                siteIcon = "icon_long_close.png";
//            }
            auto icon = CCLoadSprite::createSprite(siteIcon.c_str(), true,CCLoadSpriteType_DEFAULT);
//            CCCommonUtils::setSpriteMaxSize(icon, 90, true);
//            CCCommonUtils::setSpriteGray(icon, true);
            
            bool isHave = EquipmentController::getInstance()->IsExsitRealEquipBySite(i);
            if (m_info) {
                isHave = false;
            }
            
            if (i==0) {
                m_equipNode1->addChild(icon);
                if (isHave) {
                    m_red1Pt->setVisible(true);
                }
            }else if (i==1) {
                m_equipNode2->addChild(icon);
                if (isHave) {
                    m_red2Pt->setVisible(true);
                }
            }else if (i==2) {
                m_equipNode3->addChild(icon);
                if (isHave) {
                    m_red3Pt->setVisible(true);
                }
            }else if (i==3) {
                m_equipNode4->addChild(icon);
                if (isHave) {
                    m_red4Pt->setVisible(true);
                }
            }else if (i==4) {
                m_equipNode5->addChild(icon);
                if (isHave) {
                    m_red5Pt->setVisible(true);
                }
            }else if (i==5) {
                m_equipNode6->addChild(icon);
                if (isHave) {
                    m_red6Pt->setVisible(true);
                }
            }else if (i==6) {
                if (isOpenLongJing) {
                    m_equipNode7->addChild(icon);
                    if (isHave) {
                        m_red7Pt->setVisible(true);
                    }
                }
            }
//            else if (i==7) {
//                m_equipNode8->addChild(icon);
//                if (isHave) {
//                    m_red8Pt->setVisible(true);
//                }
//            }
        }
    }
}
//
//void GeneralsPopupView::onClickEquip(int site)
//{
//    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
//    bool tmpShowSpeWillOpen = false;
//    if (FunBuildController::getInstance()->m_tileMap.find(53) == FunBuildController::getInstance()->m_tileMap.end()) {
//        tmpShowSpeWillOpen = true;
//    }else {
//        auto& tileInfo = FunBuildController::getInstance()->m_tileMap[53];
//        if (tileInfo.state == FUN_BUILD_LOCK || tileInfo.xmlOpen==1) {//53号地块不显示，则显示脚手架
//            tmpShowSpeWillOpen = true;
//        }
//    }
//    
//    if (tmpShowSpeWillOpen) {
//        CCCommonUtils::flyHint("", "", _lang("119000"));
//        return;
//    }
//    
//    if (site==7) {//site==6 ||
//        CCCommonUtils::flyHint("", "", _lang("119023"));
//        return;
//    }
//
//    if (site==6&&isOpenLongJing==false) {//site==6 ||
//        CCCommonUtils::flyHint("", "", _lang("119023"));
//        return;
//    }
//    if (site==6 && FunBuildController::getInstance()->getMainCityLv()<15){//15级以前不开放 龙晶商店开关1.1
//        CCCommonUtils::flyHint("", "", _lang("111659"));
//        return;
//    }
////    if(FunBuildController::getInstance()->getMainCityLv()<15){
////        if (site==6 || site==7) {
////            CCCommonUtils::flyHint("", "", _lang("119023"));
////            return;
////        }
////    }
//    
//    
//    bool isExsit = EquipmentController::getInstance()->IsExsitEquipBySite(site);
//    
//    string uuid = "";
//    if (m_EquipMap.find(site) != m_EquipMap.end()) {
//        uuid = m_EquipMap[site];
//    }
//    //修改流程，没有装备时也跳转到打造页面
////    if (!isExsit && uuid=="") {
////        string _dialog = "";
////        if (site == 0) {
////            _dialog = "119014";
////        }else if (site == 1) {
////            _dialog = "119015";
////        }else if (site == 2) {
////            _dialog = "119016";
////        }else if (site == 3) {
////            _dialog = "119017";
////        }else if (site == 4) {
////            _dialog = "119018";
////        }else if (site == 5) {
////            _dialog = "119019";
////        }
////        
////        int bid = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_FORGE);
////        if (bid>0) {
////            YesNoDialog::show(_lang_1("119065", _lang(_dialog).c_str()), CCCallFunc::create(this, callfunc_selector(GeneralsPopupView::onYesGoToBuild)));
////        }else {
////            YesNoDialog::show(_lang_1("119066", _lang(_dialog).c_str()), CCCallFunc::create(this, callfunc_selector(GeneralsPopupView::onYesGoToBuild)));
////        }
////        return;
////    }
//    
////    PopupViewController::getInstance()->addPopupInView(EquipUseView::create(site));
//    PopupViewController::getInstance()->addPopupInView(EquipNewUseView::create(site));
//}
//
//void GeneralsPopupView::onYesGoToBuild()
//{
//    int bid = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_FORGE);
//    auto world = WorldMapView::instance();
//    if (world) {
//        FunBuildController::getInstance()->willMoveToBuildType = FUN_BUILD_FORGE;
//        world->leaveWorld();
//    }else {
//        PopupViewController::getInstance()->removeAllPopupView();
//        auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
//        if(layer) {
//            if (bid>0) {
//                layer->onMoveToBuildAndPlay(bid);
//            }else {
//                int itemId =layer->findCanBuildTile(FUN_BUILD_FORGE);
//                if(itemId==-1){
//                    itemId =layer->findCanBuildTile(2);
//                    if (itemId==-1) {
//                        return;
//                    }
//                }
//                layer->onMoveToBuildAndPlay(itemId);
//            }
//        }
//    }
//}
//
//void GeneralsPopupView::addMedalParticle(string medalId)
//{
//    if (m_medalNode->getChildrenCount()>0)
//    {
//        m_medalParticleNode->removeAllChildren();
//        map<int, vector<float>> colorMap;
//        for (int i=1; i<=2; i++) {
//            colorMap.clear();
//            AchievementController::getInstance()->getMedalIconColor(medalId, colorMap);
//            auto particle = ParticleController::createParticle(CCString::createWithFormat("Achievement_%d",i)->getCString());
//            particle->setAnchorPoint(ccp(0.5, 0.5));
//            vector<float> v = colorMap[i];
//            ccColor4F color = {v.at(0), v.at(1), v.at(2), v.at(3)};
//            particle->setStartColor(color);
//            color = {v.at(4), v.at(5), v.at(6), v.at(7)};
//            particle->setEndColor(color);
//            particle->setScale(1.2f);
//            m_medalParticleNode->addChild(particle);
//        }
//    }
//}
//
//void GeneralsPopupView::onShowEquipTip(int site)
//{
//    if (site==7) {//site==6 ||
//        CCCommonUtils::flyHint("", "", _lang("119023"));
//        return;
//    }
//    if(site==6 && isOpenLongJing==false){
//        CCCommonUtils::flyHint("", "", _lang("119023"));
//        return;
//    }
//    if(!m_info->showEquip){
//        CCCommonUtils::flyHint("", "", _lang("113802"));
//    }else if (m_EquipSiteMap.find(site) != m_EquipSiteMap.end()) {
//        string name = CCCommonUtils::getNameById(CC_ITOA(m_EquipSiteMap[site]));
//        int xt = site%2;
//        int yt = site/2;
//        if (!CCCommonUtils::isIosAndroidPad())
//        {
//            m_equipTipNode->setPosition(ccp(-200+xt*210, 190-yt*108));
//        } else {
//            m_equipTipNode->setPosition(ccp(-359 + xt * 326, 653 - yt * 206.3));
//        }
//        
//        m_equipTipLabel->setString(name);
//        m_equipTipNode->setVisible(true);
//    }
//}
//
////提示可打造并穿戴装备
//void GeneralsPopupView::updateEquipTips(CCObject* params){
//    if (m_playerUid=="" || m_playerUid==GlobalData::shared()->playerInfo.uid){//只对自己提示
//        glowVect.clear();
//        for (int i=0; i<6; i++) {
//            if (EquipmentController::getInstance()->isShowEquipTips(i)) {
//                //播放动画
//                CCLog("ShowEquipTips(%d)",i);
//                glowVect.push_back(i);
//                break;
//            }
//        }
//        curGlow=0;
//        if (glowVect.size()<1) {
//            m_receiveGlow->removeFromParent();
//            return;
//        }
//        this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(GeneralsPopupView::equipLoopGlowCallBack));
//        equipLoopGlowCallBack();
//    }else{
//        m_receiveGlow->removeFromParent();
//    }
//}
//
//void GeneralsPopupView::equipLoopGlowCallBack(){
//    if(glowVect.size()==0){
//        return;
//    }
//    int curSite = glowVect[curGlow];
//    switch (curSite) {
//        case 0:
//        {
//            CCPoint main_p=m_mainNode->getPosition();
//            CCPoint equipNode_p=m_equipNode1->getParent()->getPosition();
////            equipNode_p.y-=108;
////            if(CCCommonUtils::isIosAndroidPad())
////            {
////                equipNode_p.y-=92;
////            }
//            m_receiveGlow->setPosition(equipNode_p+main_p);
//        }
//            break;
//        case 1:
//        {
//            CCPoint main_p=m_mainNode->getPosition();
//            CCPoint equipNode_p=m_equipNode2->getParent()->getPosition();
////            equipNode_p.y-=108;
////            if(CCCommonUtils::isIosAndroidPad())
////            {
////                equipNode_p.y-=92;
////            }
//            m_receiveGlow->setPosition(equipNode_p+main_p);
//        }
//            break;
//        case 2:
//        {
//            CCPoint main_p=m_mainNode->getPosition();
//            CCPoint equipNode_p=m_equipNode3->getParent()->getPosition();
////            equipNode_p.y-=108;
////            if(CCCommonUtils::isIosAndroidPad())
////            {
////                equipNode_p.y-=92;
////            }
//            m_receiveGlow->setPosition(equipNode_p+main_p);
//        }
//            break;
//        case 3:
//        {
//            CCPoint main_p=m_mainNode->getPosition();
//            CCPoint equipNode_p=m_equipNode4->getParent()->getPosition();
////            equipNode_p.y-=108;
////            if(CCCommonUtils::isIosAndroidPad())
////            {
////                equipNode_p.y-=92;
////            }
//            m_receiveGlow->setPosition(equipNode_p+main_p);
//        }
//            break;
//        case 4:
//        {
//            CCPoint main_p=m_mainNode->getPosition();
//            CCPoint equipNode_p=m_equipNode5->getParent()->getPosition();
////            equipNode_p.y-=108;
////            if(CCCommonUtils::isIosAndroidPad())
////            {
////                equipNode_p.y-=92;
////            }
//            m_receiveGlow->setPosition(equipNode_p+main_p);
//        }
//            break;
//        case 5:
//        {
//            CCPoint main_p=m_mainNode->getPosition();
//            CCPoint equipNode_p=m_equipNode6->getParent()->getPosition();
////            equipNode_p.y-=108;
////            if(CCCommonUtils::isIosAndroidPad())
////            {
////                equipNode_p.y-=92;
////            }
//            m_receiveGlow->setPosition(equipNode_p+main_p);
//        }
//            break;
//        case 6:
//        {
//            CCPoint main_p=m_mainNode->getPosition();
//            CCPoint equipNode_p=m_equipNode7->getParent()->getPosition();
////            equipNode_p.y-=108;
////            if(CCCommonUtils::isIosAndroidPad())
////            {
////                equipNode_p.y-=92;
////            }
//            m_receiveGlow->setPosition(equipNode_p+main_p);
//        }
//            break;
//        case 7:
//        {
//            CCPoint main_p=m_mainNode->getPosition();
//            CCPoint equipNode_p=m_equipNode8->getParent()->getPosition();
////            equipNode_p.y-=108;
////            if(CCCommonUtils::isIosAndroidPad())
////            {
////                equipNode_p.y-=92;
////            }
//            m_receiveGlow->setPosition(equipNode_p+main_p);
//        }
//            break;
//            
//        default:
//            break;
//    }
////    curGlow++;
////    curGlow%=glowVect.size();
//    this->getAnimationManager()->runAnimationsForSequenceNamed("Loop");
//}
//
//////////////////////////////////
////////////////////
///////////////////////
//#include "UploadImageController.h"
//ChangePicPopupView* ChangePicPopupView::create(){
//    ChangePicPopupView* ret = new ChangePicPopupView();
//    if(ret && ret->init()){
//        ret->autorelease();
//    }else{
//        CC_SAFE_DELETE(ret);
//    }
//    return ret;
//}
//
//bool ChangePicPopupView::init()
//{
//    if (!PopupBaseView::init()) {
//        return false;
//    }
//    setIsHDPanel(true);
//    if (CCCommonUtils::isIosAndroidPad())
//    {
//        cellW = 520 * 2;
//    }
//    
//    CCLoadSprite::doResourceByCommonIndex(305, true);
//    
//    CCBLoadFile("GeneralPicCCB",this,this);
//    CCSize size = CCDirector::sharedDirector()->getWinSize();
//    setContentSize(size);
//    
//    int extH = this->getExtendHeight();
//    m_bgColor->setContentSize(CCSizeMake(m_bgColor->getContentSize().width, m_bgColor->getContentSize().height+extH));
//    m_bgColor->setPositionY(m_bgColor->getPositionY()-extH/2.0f);
//    
//    m_setUserPicBtn->setVisible(UploadImageController::shared()->getUploadImageFlag()>0);
//    
//    m_count = 0;
//    generateData();
//    m_timeLb->setString("");
//    m_tabView = CCTableView::create(this, m_scrollContainer->getContentSize());
//    m_tabView->setDirection(kCCScrollViewDirectionHorizontal);
//    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
//    m_tabView->setTouchPriority(Touch_Popup);
//    m_tabView->setDelegate(this);
//    m_scrollContainer->addChild(m_tabView);
//    m_tabView->setTouchEnabled(false);
//    
//    m_scrollContainer->setVisible(false);
//    
//    CCLoadSprite::doLoadResourceAsync(GENERAL_PATH, CCCallFuncO::create(this, callfuncO_selector(ChangePicPopupView::asyDelayLoad), NULL), 1);
//    CCLoadSprite::doLoadResourceAsync(GENERAL_PATH, CCCallFuncO::create(this, callfuncO_selector(ChangePicPopupView::asyDelayLoad), NULL), 2);
//    CCLoadSprite::doLoadResourceAsync(GENERAL_PATH, CCCallFuncO::create(this, callfuncO_selector(ChangePicPopupView::asyDelayLoad), NULL), 3);
//    
//
//    setCleanFunction([](){
//        CCLoadSprite::doResourceByGeneralIndex(1, false);
//        CCLoadSprite::doResourceByGeneralIndex(2, false);
//        CCLoadSprite::doResourceByGeneralIndex(3, false);
//        CCLoadSprite::doResourceByCommonIndex(305, false);
//    });
//    
//    return true;
//}
//void ChangePicPopupView::onGameTick(float time){
//    double lastTime = 0;
//    if (GlobalData::shared()->playerInfo.pic_nextUpdateTime - GlobalData::shared()->playerInfo.pic_banTime > 0) {
//        lastTime = GlobalData::shared()->playerInfo.pic_nextUpdateTime;
//    }
//    else{
//        lastTime = GlobalData::shared()->playerInfo.pic_banTime;
//    }
//    lastTime = lastTime - WorldController::getInstance()->getTime();
//    if (lastTime > 1000) {
//        m_timeLb->setString(CC_SECTOA((lastTime / 1000))) ;
//    }
//    else{
//        CCCommonUtils::setButtonSprite(m_setUserPicBtn, "but_blue.png");
//        m_timeLb->setString("");
//        unschedule(schedule_selector(ChangePicPopupView::onGameTick));
//    }
//    
//}
//void ChangePicPopupView::generateData()
//{
//    m_data = CCArray::create();
//    const char* icons[11] = {"g044","g041","g024","g026","g015","g032","g008","g038", "g012", "g007","g045"};
//    for (int i=1; i<12; i++) {
//        m_data->addObject(CCString::create(icons[i-1]));
//    }
//}
//
//void ChangePicPopupView::setButtonState(int offX)
//{
//    m_resetPicBtn->setVisible(false);
//    m_btnText->setVisible(false);
//    m_goldNum->setVisible(false);
//    m_goldIcon->setVisible(false);
//    
//    int index = round(abs(offX - ((640 - cellW) / 2)) / cellW);
//    if (CCCommonUtils::isIosAndroidPad())
//    {
//        index = round(abs(offX - ((1536 - cellW) / 2)) / cellW);
//    }
//    bool flag = false;
//    if(index < 0 || index >= m_data->count()){
//        return;
//    }else{
//        m_generalIndex = index;
//        std::string icon = dynamic_cast<CCString*>(m_data->objectAtIndex(index))->getCString();
//        if(icon == GlobalData::shared()->playerInfo.pic && !GlobalData::shared()->playerInfo.isUseCustomPic()){
//            flag = false;
//        }else{
//            flag = true;
//        }
//    }
//    if(flag){
//        m_resetPicBtn->setVisible(true);
//        auto& info = ToolController::getInstance()->getToolInfoById(ITEM_CHANGE_PIC);
//        std::string icon = dynamic_cast<CCString*>(m_data->objectAtIndex(index))->getCString();
//        if(info.getCNT()>0 || icon==GlobalData::shared()->playerInfo.pic){
//            CCCommonUtils::setButtonTitle(m_resetPicBtn, _lang("104249").c_str());
//        }else{
//            CCCommonUtils::setButtonTitle(m_resetPicBtn, "");
//            m_btnText->setVisible(true);
//            m_goldNum->setVisible(true);
//            m_goldIcon->setVisible(true);
//            m_btnText->setString(_lang("104249"));
//            int gold = CCCommonUtils::getGoldBuyItem(CC_ITOA(ITEM_CHANGE_PIC));
//            m_goldNum->setString(CC_ITOA(gold));
//        }
//    }
//    m_generalIndex = index;
//    m_pageFlag->setPositionX(-100+m_generalIndex*29);
//    if (CCCommonUtils::isIosAndroidPad())
//    {
//        m_pageFlag->setPositionX(-237+m_generalIndex*64);
//    }
//}
//
//void ChangePicPopupView::asyDelayLoad(CCObject* p){
//    m_count += 1;
//    if(m_count==3){
//        m_scrollContainer->setVisible(true);
//        refreshView();
//    }
//}
//
//void ChangePicPopupView::refreshView(){
//    m_tabView->reloadData();
//    int index = 0;
//    while(index < m_data->count()){
//        std::string icon = dynamic_cast<CCString*>(m_data->objectAtIndex(index))->getCString();
//        if(icon == GlobalData::shared()->playerInfo.pic){
//            if (CCCommonUtils::isIosAndroidPad())
//            {
//                m_tabView->setContentOffset(ccp(-index * cellW + (1536 - cellW) / 2, 0));
//            }
//            else
//                m_tabView->setContentOffset(ccp(-index * cellW + (640 - cellW) / 2, 0));
//            break;
//        }
//        index ++;
//    }
//    setButtonState(m_tabView->getContentOffset().x);
//}
//
//void ChangePicPopupView::onGetPicBack(cocos2d::CCObject *pObj)
//{
//    setButtonState(m_tabView->getContentOffset().x);
//    if (GlobalData::shared()->picUploadFlag == 0) {
//        if (haveUpload())
//        {
//            CCCommonUtils::setButtonTitle(m_setUserPicBtn, _lang("4100006").c_str());
//        }
//        else
//        {
//            CCCommonUtils::setButtonTitle(m_setUserPicBtn, _lang("4100000").c_str());
//        }
//    }
//    else{
//        CCCommonUtils::setButtonTitle(m_setUserPicBtn, _lang("4100000").c_str());
//        if (GlobalData::shared()->playerInfo.pic_nextUpdateTime - WorldController::getInstance()->getTime() > 0) {
//            schedule(schedule_selector(ChangePicPopupView::onGameTick), 1.0);
//        }
//    }
//    
//}
//
//void ChangePicPopupView::onEnter(){
//    PopupBaseView::onEnter();
//    if (GlobalData::shared()->picUploadFlag == 0) {
//        if (haveUpload())
//        {
//            CCCommonUtils::setButtonTitle(m_setUserPicBtn, _lang("4100006").c_str());
//        }
//        else
//        {
//            CCCommonUtils::setButtonTitle(m_setUserPicBtn, _lang("4100000").c_str());
//        }
//    }
//    else{
//        CCCommonUtils::setButtonTitle(m_setUserPicBtn, _lang("4100000").c_str());
//    }
//    double timeNow = WorldController::getInstance()->getTime();
//    if (GlobalData::shared()->playerInfo.pic_nextUpdateTime - timeNow > 0 || GlobalData::shared()->playerInfo.pic_banTime - timeNow > 0) {
//        CCCommonUtils::setButtonSprite(m_setUserPicBtn, "Btn_grey.png");
//        schedule(schedule_selector(ChangePicPopupView::onGameTick), 1.0);
//    }
//    
//    
//    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ChangePicPopupView::onGetPicBack), MSG_UPLOADIMAGE_BACK, NULL);
//    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
//    setTouchEnabled(true);
//    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
//    
//}
//
//void ChangePicPopupView::onExit(){
//    unschedule(schedule_selector(ChangePicPopupView::onGameTick));
//    GlobalData::shared()->isUploadPic = false;
//    setTouchEnabled(false);
//    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_UPLOADIMAGE_BACK);
//    PopupBaseView::onExit();
//}
//
//SEL_CCControlHandler ChangePicPopupView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onResetPicClick", ChangePicPopupView::onResetPicClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSetUserPicClick", ChangePicPopupView::onSetUserPicClick);
//    return NULL;
//}
//bool ChangePicPopupView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
//{
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scrollContainer", CCNode*, this->m_scrollContainer);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resetPicBtn", CCControlButton*, this->m_resetPicBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_setUserPicBtn", CCControlButton*, this->m_setUserPicBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnText", CCLabelIF*, this->m_btnText);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLb", CCLabelIF*, this->m_timeLb);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldNum", CCLabelIF*, this->m_goldNum);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldIcon", CCSprite*, this->m_goldIcon);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pageFlag", CCSprite*, this->m_pageFlag);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgColor", CCLayerColor*, this->m_bgColor);
//    return false;
//}
//
//void ChangePicPopupView::onResetPicClick(CCObject * pSender, Control::EventType pCCControlEvent){
//    auto& info = ToolController::getInstance()->getToolInfoById(ITEM_CHANGE_PIC);
//    std::string icon = dynamic_cast<CCString*>(m_data->objectAtIndex(m_generalIndex))->getCString();
//    
//    if (GlobalData::shared()->playerInfo.isUseCustomPic() && icon==GlobalData::shared()->playerInfo.pic)
//    {
//        changePic();
//        return;
//    }
//    if (info.getCNT() <= 0) {
//        int gold = CCCommonUtils::getGoldBuyItem(CC_ITOA(ITEM_CHANGE_PIC));
//        YesNoDialog::show( _lang_1("105225",CC_ITOA(gold)) , CCCallFunc::create(this, callfunc_selector(ChangePicPopupView::onOKCostGold)));
//    }else {
//        changePic();
//        if(info.getCNT() > 0){
//            ToolController::getInstance()->useTool(ITEM_CHANGE_PIC, 1, false);
//        }
//        PopupViewController::getInstance()->removePopupView(this);
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(CHANGE_PIC_REMOVE);
//    }
////    int gold = CCCommonUtils::getGoldBuyItem(CC_ITOA(ITEM_CHANGE_PIC));
////    if(info.cnt == 0 && !CCCommonUtils::isEnoughResourceByType(Gold, gold)){
////        YesNoDialog::gotoPayTips();
////        return;
////    }
////    changePic();
////    if(info.cnt > 0){
////        ToolController::getInstance()->useTool(ITEM_CHANGE_PIC, 1, false);
////    }
////    PopupViewController::getInstance()->removePopupView(this);
////    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(CHANGE_PIC_REMOVE);
//}
//
//void ChangePicPopupView::onSetUserPicClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
//{
//    if (haveUpload())
//    {
//        //have uploaded one
//        YesNoDialog::showYesDialog(_lang("4100007").c_str(), false, NULL);
//    }
//    else
//    {
//        int level = FunBuildController::getInstance()->getMainCityLv();
//        if (level >= GlobalData::shared()->generalConfig.picUploadLv_limit) {
//
//            double timeNow = WorldController::getInstance()->getTime();
//            if (GlobalData::shared()->playerInfo.pic_nextUpdateTime - timeNow > 0) {
//                CCCommonUtils::flyHint("", "", _lang("105769"));
//            }else if (GlobalData::shared()->playerInfo.pic_banTime - timeNow > 0){
//                CCCommonUtils::flyHint("", "", _lang("105770"));
//            }
//            else{
//                PopupViewController::getInstance()->addPopupView(GeneralCustomPicView::create());
//            }
//        }
//        else{
//            CCCommonUtils::flyHint("", "", _lang_1("105776", CC_ITOA(GlobalData::shared()->generalConfig.picUploadLv_limit)));
//        }
//    }
//}
//
//bool ChangePicPopupView::haveUpload()
//{
//    int picVer = GlobalData::shared()->playerInfo.picVer;
//    if (picVer>2000000 && picVer<=3000000)
//    {
//        return true;
//    }
//    return false;
//}
//
//void ChangePicPopupView::onOKCostGold()
//{
//    auto& info = ToolController::getInstance()->getToolInfoById(ITEM_CHANGE_PIC);
//    int gold = CCCommonUtils::getGoldBuyItem(CC_ITOA(ITEM_CHANGE_PIC));
//    if(!CCCommonUtils::isEnoughResourceByType(Gold, gold)){
//        YesNoDialog::gotoPayTips();
//        return;
//    }
//    changePic();
//    PopupViewController::getInstance()->removePopupView(this);
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(CHANGE_PIC_REMOVE);
//}
//
//void ChangePicPopupView::changePic(){
//    std::string icon = dynamic_cast<CCString*>(m_data->objectAtIndex(m_generalIndex))->getCString();
//    ChangePicCommand* cmd = new ChangePicCommand(icon);
//    cmd->sendAndRelease();
//}
//
//bool ChangePicPopupView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
//    m_x = 1000;
//    return true;
//}
//
//void ChangePicPopupView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
//    if(isTouchInside(m_scrollContainer, pTouch)){
//        int dx = pTouch->getLocation().x - pTouch->getStartLocation().x;
//        if(dx > 10 || dx < -10){
//            if(m_x == 1000){
//                m_x = m_tabView->getContentOffset().x;
//            }
//            int offX = m_x + dx;
//            int min = -(m_data->count() - 1) * cellW - 300;
//            int max = 300;
//            if (CCCommonUtils::isIosAndroidPad())
//            {
//                max = 600;
//            }
//            if(offX > max){
//                offX = max;
//            }else if(offX < min){
//                offX = min;
//            }
//            if (CCCommonUtils::isIosAndroidPad())
//            {
//                m_tabView->setContentOffset(ccp(offX + (1536 - cellW) / 2, 0), false);
//            }
//            else
//                m_tabView->setContentOffset(ccp(offX + (640 - cellW) / 2, 0), false);
//        }
//    }
//}
//
//void ChangePicPopupView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
//{
//    if(!isTouchInside(m_scrollContainer, pTouch) && !isTouchInside(m_setUserPicBtn, pTouch)){
//        PopupViewController::getInstance()->removePopupView(this);
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(CHANGE_PIC_REMOVE);
//    }else{
//        int addX = -cellW / 2;
//        int dx = pTouch->getStartLocation().x - pTouch->getLocation().x;
//        if (CCCommonUtils::isIosAndroidPad()) {
//            if(dx > 40){
//                addX = -cellW;
//            }else if(dx < -40){
//                addX = 0;
//            }
//        } else {
//            if(dx > 20){
//                addX = -cellW;
//            }else if(dx < -20){
//                addX = 0;
//            }
//        }
//        int offSetX = int((m_tabView->getContentOffset().x + addX) / cellW) * cellW;
//        int minX = -(m_data->count() - 1) * cellW;
//        if(offSetX > 0){
//            offSetX = 0;
//        }else if(offSetX < minX){
//            offSetX = minX;
//        }
//        if (CCCommonUtils::isIosAndroidPad())
//        {
//            m_tabView->setContentOffset(ccp(offSetX + (1536 - cellW) / 2, 0), true);
//        }
//        else
//            m_tabView->setContentOffset(ccp(offSetX + (640 - cellW) / 2, 0), true);
//        setButtonState(offSetX);
//    }
//}
//
//CCSize ChangePicPopupView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
//{
//    if(idx >= m_data->count()){
//        return CCSizeZero;
//    }
//    if (CCCommonUtils::isIosAndroidPad())
//    {
//        return CCSize(cellW, 500*1.75);
//    }
//    return CCSize(cellW, 500);
//}
//
//CCSize ChangePicPopupView::cellSizeForTable(CCTableView *table)
//{
//    if (CCCommonUtils::isIosAndroidPad())
//    {
//        return CCSize(cellW, 500*1.75);
//    }
//    return CCSize(cellW, 500);
//}
//
//CCTableViewCell* ChangePicPopupView::tableCellAtIndex(CCTableView *table, ssize_t idx)
//{
//    if(idx >= m_data->count()){
//        return NULL;
//    }
//    
//    GeneralPicCell* cell = (GeneralPicCell*)table->dequeueCell();
//    std::string str = dynamic_cast<CCString*>(m_data->objectAtIndex(idx))->getCString();
//    if(cell){
//        cell->setData(str);
//    }else{
//        cell = GeneralPicCell::create(str);
//    }
//    return cell;
//}
//
//ssize_t ChangePicPopupView::numberOfCellsInTableView(CCTableView *table)
//{
//    int num = ceil(m_data->count() / 1.0f);
//    return num;
//}
//void ChangePicPopupView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
//{
//    
//}
//void ChangePicPopupView::scrollViewDidScroll(CCScrollView* view){
//    CCLOG("%f", view->getContentOffset().x);
//}
//
//GeneralPicCell *GeneralPicCell::create(std::string generalId){
//    GeneralPicCell *ret = new GeneralPicCell(generalId);
//    if(ret && ret->init()){
//        ret->autorelease();
//    }else{
//        CC_SAFE_DELETE(ret);
//    }
//    return ret;
//}
//
//void GeneralPicCell::setData(std::string generalId){
//    this->m_generalId = generalId;
//    refreshView();
//}
//
//void GeneralPicCell::refreshView()
//{
//    m_picNode->removeAllChildren();
//    auto sprite = CCLoadSprite::createSprite((m_generalId + "_bust.png").c_str());
////    sprite->setPosition(ccp(cellW / 2, 290));
////    float s = CCDirector::sharedDirector()->getWinSize().height / 852.0f;
////    if(s > 1.2){
////        s = 1.2;
////    }
////    sprite->setScale(s);
//    m_picNode->addChild(sprite);
//}
//
//bool GeneralPicCell::init(){
//    setCleanFunction([](){
//        CCLoadSprite::doResourceByGeneralIndex(1, false);
//        CCLoadSprite::doResourceByGeneralIndex(2, false);
//        CCLoadSprite::doResourceByGeneralIndex(3, false);
//    });
//    
//    CCBLoadFile("GeneralPicCell",this,this);
//
//    refreshView();
//    return true;
//}
//
//void GeneralPicCell::onEnter(){
//    CCTableViewCell::onEnter();
//}
//
//void GeneralPicCell::onExit(){
//    CCTableViewCell::onExit();
//}
//
//SEL_CCControlHandler GeneralPicCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
//    return NULL;
//}
//bool GeneralPicCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
//{
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, this->m_picNode);
//    return false;
//}
//
//
////----
//bool ClickTipView::init(){
//    auto bg = CCLoadSprite::createScale9Sprite("ui_balloons.png");
//    bg->setAnchorPoint(ccp(0, 1));
//    this->addChild(bg);
//    int stamineIntervalTime = WorldController::getInstance()->getStatmineIntervalTime();
//    
//    string showTip = _lang_2("103704", CC_ITOA(GlobalData::shared()->worldConfig.stamineIntervalNum), CC_ITOA(stamineIntervalTime));
//    showTip.append("\n");
//    showTip.append(_lang("103049"));
//    m_desText = CCLabelIF::create();
//    m_desText->setColor(ccBLACK);
//    m_desText->setDimensions(CCSize(360, 0));
//    m_desText->setAnchorPoint(ccp(0.5, 0.5));
//    m_desText->setString(showTip);
//    this->addChild(m_desText);
//    
//    bg->setPreferredSize(CCSize(m_desText->getContentSize().width * m_desText->getOriginScaleX() + 20, m_desText->getContentSize().height * m_desText->getOriginScaleY() + 50));
//
//    m_desText->setPosition(bg->getContentSize().width / 2, -bg->getContentSize().height / 2 + 15);
//
//    m_timeText = CCLabelIF::create();
//    m_timeText->setAnchorPoint(ccp(0.5, 0.5));
//    m_timeText->setColor(ccBLACK);
//    m_timeText->setPosition(bg->getContentSize().width / 2, -bg->getContentSize().height + 20);
//    this->addChild(m_timeText);
//    update(0.0f);
//    return true;
//}
//
////void ClickTipView::onEnter(){
////    CCNode::onEnter();
////    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
////}
////
////void ClickTipView::onExit(){
////    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
////    CCNode::onExit();
////}
//
//void ClickTipView::update(float time){
//    int currt = WorldController::getInstance()->currentStamine;
//    int total = GlobalData::shared()->worldConfig.stamineMax;
//    if(currt >= total){
//        m_timeText->setString("");
//    }else{
//        int stamineIntervalTime = WorldController::getInstance()->getStatmineIntervalTime();
//
//        double dTime = (WorldController::getInstance()->lastStamineTime + stamineIntervalTime * 1000) - WorldController::getInstance()->getTime();
//        if(dTime < 0){
//                dTime = 0;
//        }
//        m_timeText->setString(CCCommonUtils::timeLeftToCountDown(dTime / 1000));
//    }
//}
//
//
////CLASS GeneralCustomPicView
//GeneralCustomPicView* GeneralCustomPicView::create()
//{
//    GeneralCustomPicView* ret = new GeneralCustomPicView();
//    if (ret && ret->init())
//    {
//        ret->autorelease();
//    }
//    else
//    {
//        CC_SAFE_DELETE(ret);
//    }
//    return ret;
//}
//
//bool GeneralCustomPicView::init()
//{
//    bool ret = false;
//    do {
//        if (!PopupBaseView::init())
//        {
//            break;
//        }
//        CCBLoadFile("GeneralCustomPicView", this, this);
//        
//        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
//        this->setContentSize(winSize);
//        
//        int extH = this->getExtendHeight();
//        m_bgColor->setContentSize(CCSizeMake(m_bgColor->getContentSize().width, m_bgColor->getContentSize().height+extH));
//        m_bgColor->setPositionY(m_bgColor->getPositionY()-extH/2.0f);
////        m_resetNode->setPositionY(m_resetNode->getPositionY() - extH);
//        
//        CCCommonUtils::setButtonTitle(m_pictureBtn, _lang("4100002").c_str());
//        CCCommonUtils::setButtonTitle(m_cemaraBtn, _lang("4100001").c_str());
////        m_alertTTF->setString(_lang("105779"));
//        
//        double banTime  = ((1 + GlobalData::shared()->playerInfo.pic_banCount) *  GlobalData::shared()->generalConfig.picModelTime) / 86400;
//        string banStr("0");
//        if (banTime > 0) {
//            banStr = CC_ITOA(ceil( banTime)) ;
//        }
//        m_alertTTF->setString(_lang_1("105779", banStr.c_str()));//	这里的{0}={（1+该玩家已经被处理的次数）*被惩罚时间}/86400
//        m_selectIndex = 0;
//        
//        ret = true;
//    } while (0);
//    return ret;
//}
//
//void GeneralCustomPicView::onGetCustomPicClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
//{
//    m_selectIndex = ((CCControlButton*)pSender)->getTag();
//    int picVer = GlobalData::shared()->playerInfo.picVer;
//    if (picVer>2000000 && picVer<=3000000)
//    {
//        //have uploaded one
//        CCCallFunc* callBack = CCCallFunc::create(this, callfunc_selector(GeneralCustomPicView::doGetPicAction));
//        YesNoDialog::showYesDialog(_lang("105749").c_str(), false, callBack);
//        
//        return;
//    }
//    doGetPicAction();
//}
//
//void GeneralCustomPicView::doGetPicAction()
//{
//    string uid = GlobalData::shared()->playerInfo.uid;
//    UploadImageController::shared()->onUploadImage(m_selectIndex);
//    m_cemaraBtn->setEnabled(false);
//    m_pictureBtn->setEnabled(false);
//}
//
//void GeneralCustomPicView::onGetPicBack(cocos2d::CCObject *pObj)
//{
//    PopupViewController::getInstance()->removePopupView(this);
//}
//
//void GeneralCustomPicView::onEnter()
//{
//    PopupBaseView::onEnter();
//    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
//    setTouchEnabled(true);
//    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
//    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GeneralCustomPicView::onGetPicBack), MSG_GetHeadImgUrl, NULL);
//}
//
//void GeneralCustomPicView::onExit()
//{
//    setTouchEnabled(false);
//    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_GetHeadImgUrl);
//    PopupBaseView::onExit();
//}
//
//bool GeneralCustomPicView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
//{
//    return true;
//}
//
//void GeneralCustomPicView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
//{
//    if (!isTouchInside(m_touchNode, pTouch))
//    {
//        PopupViewController::getInstance()->removePopupView(this);
//    }
//}
//
//bool GeneralCustomPicView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
//{
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resetNode", CCNode*, this->m_resetNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pictureBtn", CCControlButton*, this->m_pictureBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cemaraBtn", CCControlButton*, this->m_cemaraBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_alertTTF", CCLabelIF*, this->m_alertTTF);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgColor", CCLayerColor*, this->m_bgColor);
//    return false;
//}
//
//SEL_CCControlHandler GeneralCustomPicView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
//{
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGetCustomPicClick", GeneralCustomPicView::onGetCustomPicClick);
//    return NULL;
//}