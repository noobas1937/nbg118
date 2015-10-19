//
//  GeneralsPopupView.cpp
//  IF
//
//  Created by ganxiaohua on 14-6-11.
//
//

#include "GeneralTitanPopupView.h"
#include "TitanFeedCommand.h"
#include "PropSpeedupView.h"
#include "TitanInfoCommand.h"
#include "BuildUpgradeView.h"
#include "TitanUpgradeView.h"
#include "UseResToolView.h"
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
#include "ToolController.h"
#include "YesNoDialog.h"
//#include "ChangePicCommand.h"
//#include "WorldController.h"
//#include "RoleInfoView.h"
//#include "AllRankListPopUpView.h"
#include "GeneralSkillListPopUpView.h"
#include "FunBuildController.h"
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

void setGrayNode(cocos2d::CCNode *node , bool gray)
{
    auto trySpr = dynamic_cast<CCSprite*>(node);
    
    auto trySpr9 = dynamic_cast<CCScale9Sprite*>(node);
    
    if (trySpr) {
        CCCommonUtils::setSpriteGray(trySpr, gray);
    }
    
    if (trySpr9) {
        if(gray)
            trySpr9->setState(cocos2d::ui::Scale9Sprite::State::GRAY);
        else
            trySpr9->setState(cocos2d::ui::Scale9Sprite::State::NORMAL);
    }
    
    
    
    for(auto child:node->getChildren())
    {
        setGrayNode(child,gray);
    }
    
    
}
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
    
    //m_mainNode->setPosition();
    if(add>108)
    {
        float tempX = (add - 108)/3;
        
        m_mainNode->setPositionY(m_mainNode->getPositionY()-108-tempX);
        
        m_titanExtNode->setPositionY(m_titanExtNode->getPositionY()-108-tempX*2);
        
    }
    
    
    this->m_receiveGlow->setVisible(false);
    bool isOpenLongJing = false;

//    m_Titan = Titan::create(1);
//    m_Titan->setScale(0.7);
//    m_titanPos->addChild(m_Titan);
    m_titanPosInView->addChild( TitanInView::create());
//    auto sdsds = cocos2d::Label::createWithBMFont("Arial_Bold.fnt", "222221122");
//        this->m_titanPos->addChild(sdsds);
    
    m_ProTitanExtMaxWidth = m_ProTitanExt->getContentSize().width;
    
    m_ProTiTanAPMaxWidth = m_ProTiTanAP->getContentSize().width;
 
    m_ProTitanExt->setVisible(true);
    
    this->resetAttribute(nullptr);
    
    this->scheduleUpdate();
    
    m_feedCDBtnTxt->setString(_lang("104903").c_str());
    
//    CCCommonUtils::setButtonTitle(this->m_CleanFeedCDBtn,_lang("104903").c_str());
    
//    CCCommonUtils::setButtonTitle(this->m_titanFeedBtn,"喂食");
    
    this->m_titanStatusDesc->setString("Titan Upgrage CD......");//fusheng 需要文本
    

    
    m_titanFeedBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::DISABLED)->setState(cocos2d::ui::Scale9Sprite::State::GRAY);
    
    m_toolSpeedUpBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::DISABLED)->setState(cocos2d::ui::Scale9Sprite::State::GRAY);
    
    m_CleanFeedCDBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::DISABLED)->setState(cocos2d::ui::Scale9Sprite::State::GRAY);
    

    m_titanFeedBtn->setZoomOnTouchDown(false);
    
    m_CleanFeedCDBtn->setZoomOnTouchDown(false);//fusheng 不做放缩

    
    m_titanAPTxtPre->setString("AP");//fusheng 需要文本
    
    m_titanExtTxtPre->setString("Exp");//fusheng 需要文本
    
  
    return true;
}

CCNode* GeneralTitanPopupView::getGuideNode(string _key)
{
    if (_key == "Titan_Feed") {
        if (m_titanFeedBtn->isVisible() || !m_titanFeedBtn->isEnabled()) {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("Titan_Feed"));
            return NULL;
        }else {
            return m_titanFeedBtn;
        }
    }else if (_key == "Titan_Up") {
        return m_titanUngrade;
    }
    return NULL;
}

void GeneralTitanPopupView::resetAttribute(CCObject* obj)
{
    CCDictionary *params = dynamic_cast<CCDictionary*>(obj);

    
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
    
    string title = _lang_1("102272", CC_ITOA(m_titanInfo.level));
    if(m_titanInfo.level>=GlobalData::shared()->MaxBuildLv) {
        title += _lang("102183");
    }
    m_lvLabel->setString(title);
    
    bool isCanFeed = true;
    
    isFoodEnough = true;//fusheng 标记粮食充足
    
    FunBuildInfo& fbiInfo = FunBuildController::getInstance()->getFunbuildById(400000000);
    
    if(fbiInfo.state == FUN_BUILD_UPING)
    {
        CCCommonUtils::setSpriteGray(m_titanUngrade, true);
        this->m_titanFeedBtn->setEnabled(false);//fusheng 泰坦升级时
        
        titanFeedNode->setVisible(false);
        titanUpingNode->setVisible(true);
        this->upgradeCD =(float)(fbiInfo.updateTime - GlobalData::shared()->getWorldTime());
        
        isUpgrading = true;
        
        isUpdating = true;
        
       
        
        if (GlobalData::shared()->allQueuesInfo.find(1102) != GlobalData::shared()->allQueuesInfo.end()) {
            
            string key = GlobalData::shared()->allQueuesInfo[1102].key;
            

            if (GlobalData::shared()->allQueuesInfo[1102].itemId == 400000) {
                queue_id = 1102;
            }
            
            
        }
        
        if (GlobalData::shared()->allQueuesInfo.find(1101) != GlobalData::shared()->allQueuesInfo.end()) {
            
            string key = GlobalData::shared()->allQueuesInfo[1101].key;
            
            
            if (GlobalData::shared()->allQueuesInfo[1101].itemId == 400000) {
                queue_id = 1101;
            }
            
            
        }
        
        
        int toolID = ToolController::getInstance()->getSPDItem(ItemSpdMenu_City);
        
        auto& toolInfo = ToolController::getInstance()->getToolInfoById(toolID);
        
        
//        auto sp = m_toolSpeedUpBtn->getBackgroundSpriteForState(Control::State::DISABLED);
        
        
        
        if(toolInfo.getCNT()>0)//fusheng 判断加速道具个数
        {

            
            
            m_toolSpeedUpBtn->setEnabled(true);
        }
        else
        {
          
            
            m_toolSpeedUpBtn->setEnabled(false);
        }
        
        

    }
    else
    {
        CCCommonUtils::setSpriteGray(m_titanUngrade, false);
        titanFeedNode->setVisible(true);
        titanUpingNode->setVisible(false);

        
        isUpgrading = false;

    }
    

    
    if(m_titanInfo.status==0)
    {
        
    }
    else
    {
        
    }
    
   

    if(m_titanInfo.nextExp!=0)
    {
//        this->m_titanExtTxt->setString(CCString::createWithFormat("%d/%d",m_titanInfo.exp,m_titanInfo.nextExp)->getCString());
        
        this->m_titanExtTxt->setString(CCString::createWithFormat("%s/%s",CC_ITOA_K((long)m_titanInfo.exp),CC_ITOA_K((long)m_titanInfo.nextExp))->getCString());
        
        float ratio =((float)m_titanInfo.exp)/m_titanInfo.nextExp;
        
        auto size = this->m_ProTitanExt->getContentSize();
        
        size.width = m_ProTitanExtMaxWidth*ratio;
        
        m_ProTitanExt->setVisible(ratio != 0);
        
        auto oriSize = m_ProTitanExt->getOriginalSize();
        
        if (size.width<oriSize.width) {
            
            this->m_ProTitanExt->setContentSize(oriSize);

        }
        else
        {
            this->m_ProTitanExt->setContentSize(size);

        }
        
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
        
        size.width = m_ProTiTanAPMaxWidth * ratio;
        
        
        m_ProTiTanAP->setVisible(ratio != 0);
       
        auto oriSize = m_ProTiTanAP->getOriginalSize();
        
        if (size.width<oriSize.width) {
            
            this->m_ProTiTanAP->setContentSize(oriSize);
            
        }
        else
        {
            this->m_ProTiTanAP->setContentSize(size);
            
        }
        
        
        
        
        this->m_titanAPTxt->setString(CCString::createWithFormat("(%d/%d)",m_titanInfo.currentManual,m_titanInfo.maxManual)->getCString());

    }
    else
    {
        
        auto size = this->m_ProTiTanAP->getContentSize();
        size.width = 0;
        this->m_ProTiTanAP->setContentSize(size);
        this->m_titanAPTxt->setString("数据异常");

    }
   
    this->m_needFood->setString(CC_CMDITOAL(m_titanInfo.feedFoodNum));
    
    if(m_titanInfo.feedNum<m_titanInfo.feedMaxNum)//fusheng 只有在小于免费喂食的情况下  才判断粮食的
    {
        if(params)
        {
            
            auto food = params->valueForKey("food");
            if(food->length()!=0)
            {
                this->m_currentFoodNum->setString(CCString::createWithFormat("/%s",CC_CMDITOAL(food->longValue()).c_str())->getCString());
                
                if(m_titanInfo.feedFoodNum>food->longValue())
                {
                    
                    this->m_needFood->setColor(ccColor3B::RED);
                    
                    isFoodEnough = false;
                    
                }
                else
                {
                    this->m_needFood->setColor(ccColor3B::WHITE);
                }
            }
            else
            {
                //long lfood = GlobalData::shared()->resourceInfo.lFood;
                
                
                
                long lfood = GlobalData::shared()->resourceInfo.lFood;
                if(m_titanInfo.feedFoodNum>lfood)
                {
                    
                    this->m_needFood->setColor(ccColor3B::RED);
                    
                    isFoodEnough = false;
                }
                else
                {
                    this->m_needFood->setColor(ccColor3B::WHITE);
                }
                
                //            this->m_currentFoodNum->setString(CCString::createWithFormat("/%ld",lfood)->getCString());
                this->m_currentFoodNum->setString(CCString::createWithFormat("/%s",CC_CMDITOAL(lfood).c_str())->getCString());
            }
        }
        else
        {
            long lfood = GlobalData::shared()->resourceInfo.lFood;
            
            
            this->m_currentFoodNum->setString(CCString::createWithFormat("/%s",CC_CMDITOAL(lfood).c_str())->getCString());
            
            if(m_titanInfo.feedFoodNum>lfood)
            {
                
                this->m_needFood->setColor(ccColor3B::RED);
                
                isFoodEnough = false;
            }
            else
            {
                this->m_needFood->setColor(ccColor3B::WHITE);
            }
        }
    }

    
    
    
    
    if(m_titanInfo.exp>=m_titanInfo.nextExp)
    {
        isCanFeed = false;
        this->m_CleanFeedCDBtn->setEnabled(false);
        this->m_CleanFeedCDBtnNode->setVisible(false);
        
        this->m_titanFeedBtnNode->setVisible(true);
    }
    else
    {
        if(m_titanInfo.feedcd!=0)
        {
            
            this->feedCD =(float)(m_titanInfo.feedcd - GlobalData::shared()->getWorldTime());
            if (this->feedCD<0) {
                this->feedCD=0;
                isUpdating |= false;
                

                this->m_CleanFeedCDBtn->setEnabled(false);
                this->m_CleanFeedCDBtnNode->setVisible(false);
                
                this->m_titanFeedBtn->setEnabled(true);
                this->m_titanFeedBtnNode->setVisible(true);
                //            titanFeedNode->setVisible(true);
                //            titanUpingNode->setVisible(false);
                
                
            }
            else
            {
                isUpdating = true;
                this->m_CleanFeedCDBtn->setEnabled(true);
                m_CleanFeedCDBtnNode->setVisible(true);
                
                this->m_titanFeedBtn->setEnabled(false);
                m_titanFeedBtnNode->setVisible(false);
                
                
            }
            string timeInfo = CC_SECTOA((int)this->feedCD);
            this->m_feedCDTxt->setString(timeInfo);
            
            
        }
        else
        {
            this->feedCD=0;
            isUpdating |= false;
            this->m_CleanFeedCDBtn->setEnabled(false);
            m_CleanFeedCDBtnNode->setVisible(false);
            
            m_titanFeedBtnNode->setVisible(true);

            string timeInfo = CC_SECTOA((int)this->feedCD);
            this->m_feedCDTxt->setString(timeInfo);
            
            
        }
    }
    
 
    
    if(m_titanInfo.feedNum>=m_titanInfo.feedMaxNum)
    {
        m_titanFeedStatus->setString(CCString::createWithFormat("Count : %d/%d",m_titanInfo.feedMaxNum,m_titanInfo.feedMaxNum)->getCString());//fusheng 需要文本
        m_titanFeedTxt->setString("Accelerated growth");//fusheng 需要文本
        
        int gold = (m_titanInfo.feedNum - m_titanInfo.feedMaxNum)*50+CCCommonUtils::getGoldByTime(3600);
        gold = gold>1000? 1000:gold;
        this->m_needGlod->setString(CC_CMDITOAL(gold));
        this->m_needGlod->setColor(ccWHITE);
        m_needGoldNode->setVisible(true);
        m_needFoodNode->setVisible(false);

    }
    else
    {
        m_needGoldNode->setVisible(false);
        m_needFoodNode->setVisible(true);
        m_titanFeedStatus->setString(CCString::createWithFormat("Count : %d/%d",m_titanInfo.feedNum,m_titanInfo.feedMaxNum)->getCString());//fusheng 需要文本  还有中文
        m_titanFeedTxt->setString("Feed");//fusheng 需要文本
    }

    if(m_titanInfo.exp>=m_titanInfo.nextExp)
    {
        m_titanFeedStatus->setString(CCString::createWithFormat("Experience full, you can upgrade!!!")->getCString());
        m_titanFeedTxt->setString("Feed");//fusheng 需要文本
    }

    this->m_titanFeedBtn->setEnabled(isCanFeed);
    
    setGrayNode(m_titanFeedBtnNode,!isCanFeed);
//    CCCommonUtils::setSpriteGray(m_titanFeedBtnSprite, !isCanFeed);

}


//
void GeneralTitanPopupView::loadResource(){
//    CCLoadSprite::doResourceByCommonIndex(105, true);
//    CCLoadSprite::doResourceByCommonIndex(100, true);
    CCLoadSprite::doResourceByCommonIndex(305, true);
    CCLoadSprite::doResourceByCommonIndex(3, true);
//    CCLoadSprite::doResourceByCommonIndex(503, true);
//    CCLoadSprite::doResourceByCommonIndex(7, true);
}
//
void GeneralTitanPopupView::onEnter(){
    PopupBaseView::onEnter();

    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
   
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GeneralTitanPopupView::resetAttribute), MSG_TITAN_INFORMATION_RESET, NULL);

//    
    onRefreshEquip();
    

    resetAttribute(nullptr);//fusheng onEnter时刷新数据
    
    

}

void GeneralTitanPopupView::onExit(){

    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TITAN_INFORMATION_RESET);

    CCLoadSprite::doResourceByGeneralIndex(3, false);

    
    PopupBaseView::onExit();
}
SEL_CCControlHandler GeneralTitanPopupView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSkillBtnClick", GeneralsPopupView::onSkillBtnClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAllianceClick", GeneralsPopupView::onAllianceClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTitanFeedClick", GeneralTitanPopupView::onTitanFeedClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCleanFeedCDClick", GeneralTitanPopupView::onCleanFeedCDClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSpeedUpClick", GeneralTitanPopupView::onSpeedUpClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onToolSpeedUpClick", GeneralTitanPopupView::onToolSpeedUpClick);

    return NULL;
}
bool GeneralTitanPopupView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanAttackTxt", CCLabelIF*, this->m_titanAttackTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanMagicTxt", CCLabelIF*, this->m_titanMagicTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_Txt1", CCLabelIF*, this->m_Txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_Txt2", CCLabelIF*, this->m_Txt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanExtTxt", CCLabelIF*, this->m_titanExtTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_feedCDTxt", CCLabelIF*, this->m_feedCDTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upgradeCDTxt", CCLabelIF*, this->m_upgradeCDTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanFeedStatus", CCLabelIF*, this->m_titanFeedStatus);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanFeedTxt", CCLabelIF*, this->m_titanFeedTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_feedCDBtnTxt", CCLabelIF*, this->m_feedCDBtnTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvLabel", CCLabelIF*, this->m_lvLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_needFood", CCLabelIF*, this->m_needFood);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_currentFoodNum", CCLabelIF*, this->m_currentFoodNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanFeedBtn", CCControlButton*, this->m_titanFeedBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_speedUpBtn", CCControlButton*, this->m_speedUpBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_CleanFeedCDBtn", CCControlButton*, this->m_CleanFeedCDBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_feedCDGoldTxt", CCLabelIF*, this->m_feedCDGoldTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanExtTxtPre", CCLabelIF*, this->m_titanExtTxtPre);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_toolSpeedUpBtn", CCControlButton*, this->m_toolSpeedUpBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodStatus", CCSprite*, this->m_foodStatus);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanDetail", CCSprite*, this->m_titanDetail);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanSkill", CCSprite*, this->m_titanSkill);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanUngrade", CCSprite*, this->m_titanUngrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanFeedBtnSprite", CCSprite*, this->m_titanFeedBtnSprite);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bustPic", CCNode*, this->m_bustPic);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon", CCNode*, this->m_icon);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchLayer", CCNode*, this->m_touchLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanFeedBtnNode", CCNode*, this->m_titanFeedBtnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_CleanFeedCDBtnNode", CCNode*, this->m_CleanFeedCDBtnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "titanFeedNode", CCNode*, this->titanFeedNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "titanUpingNode", CCNode*, this->titanUpingNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanFeedCDNode", CCNode*, this->m_titanFeedCDNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanStatusDesc", CCLabelIF*, this->m_titanStatusDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_needGlod", CCLabelIF*, this->m_needGlod);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_needFoodNode", CCNode*, this->m_needFoodNode);
    
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
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_needGoldNode", CCNode*, this->m_needGoldNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanExtNode", CCNode*, this->m_titanExtNode);
    
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoNode", CCNode*, this->m_infoNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ProTitanExt", CCScale9Sprite*, this->m_ProTitanExt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ProTiTanAP", CCScale9Sprite*, this->m_ProTiTanAP);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanPosInView", CCNode*, this->m_titanPosInView);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanAPTxt", CCLabelIF*, this->m_titanAPTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanAPTxtPre", CCLabelIF*, this->m_titanAPTxtPre);
    
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
    
    if (isUpgrading)
    {
        if (this->upgradeCD>0) {
            this->upgradeCD -= time;
            string timeInfo = CC_SECTOA((int)this->upgradeCD);
            this->m_upgradeCDTxt->setString(timeInfo);
            
        }
        else
        {
            
            resetAttribute(nullptr);
        }
    }
    else
    {
        if (this->feedCD>0) {
            this->feedCD -= time;
            string timeInfo = CC_SECTOA((int)this->feedCD);
            this->m_feedCDTxt->setString(timeInfo);
            m_feedCDGoldTxt->setString(CC_CMDITOAL(CCCommonUtils::getGoldByTime((int)this->feedCD)));
            
        }
        else
        {
            
            resetAttribute(nullptr);
        }
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
    if(this->feedCD<=0)
    {
        if(m_titanInfo.exp>=m_titanInfo.nextExp)
        {
            CCCommonUtils::flyHint("", "", "please ungrade titan!");
            return;
        }
        
        if(!isFoodEnough)
        {
            
            PopupViewController::getInstance()->addPopupInView(UseResToolView::create(WorldResourceType::Food));
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
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("Titan_Feed"));
            m_titanFeedBtn->setEnabled(false);
            
        }
        else
        {
            
            int gold = (m_titanInfo.feedNum - m_titanInfo.feedMaxNum)*50+CCCommonUtils::getGoldByTime(3600);
            gold = gold>1000? 1000:gold;
            YesNoDialog::showButtonAndGold(_lang("102120").c_str() , CCCallFunc::create(this, callfunc_selector(GeneralTitanPopupView::AccGrowthCallBack)), "OK", gold);
        }

    }
//    else
//    {
//        
//        YesNoDialog::showTime( _lang("102120").c_str() , CCCallFunc::create(this, callfunc_selector(GeneralTitanPopupView::speedUpCallBack)), (int)this->feedCD, _lang("104903").c_str());
//    }
    
    
}
void GeneralTitanPopupView::AccGrowthCallBack()
{
    TitanFeedCommand *tfCommand = new TitanFeedCommand();
    tfCommand->sendAndRelease();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("Titan_Feed"));
    m_titanFeedBtn->setEnabled(false);
}

void GeneralTitanPopupView::onSpeedUpClick(CCObject * pSender, Control::EventType pCCControlEvent){

    
    int tmpTime = GlobalData::shared()->allQueuesInfo[queue_id].finishTime - GlobalData::shared()->getWorldTime();
    
    YesNoDialog::showTime( _lang("102120").c_str() , CCCallFunc::create(this, callfunc_selector(GeneralTitanPopupView::spdCallBack)), tmpTime, _lang("104903").c_str());
    
   


//
////    int tmpTime = GlobalData::shared()->allQueuesInfo[qid].finishTime - GlobalData::shared()->getWorldTime();
//    YesNoDialog::showTime( _lang("102120").c_str() , CCCallFunc::create(this, callfunc_selector(GeneralTitanPopupView::speedUpCallBack)), (int)this->feedCD, _lang("104903").c_str());
//    
    
}

void GeneralTitanPopupView::spdCallBack()
{
    int tmpTime = GlobalData::shared()->allQueuesInfo[queue_id].finishTime - GlobalData::shared()->getWorldTime();
    int gold = CCCommonUtils::getGoldByTime(tmpTime);
    if (tmpTime<=GlobalData::shared()->freeSpdT) {
        gold = 0;
    }
    FunBuildController::getInstance()->costCD(400000000, "", gold);
    
};

void GeneralTitanPopupView::onCleanFeedCDClick(CCObject * pSender, Control::EventType pCCControlEvent){
    
    
    YesNoDialog::showTime( _lang("102120").c_str() , CCCallFunc::create(this, callfunc_selector(GeneralTitanPopupView::speedUpCallBack)), (int)this->feedCD, _lang("104903").c_str(),false);
    
    
}

void GeneralTitanPopupView::onToolSpeedUpClick(CCObject * pSender, Control::EventType pCCControlEvent){
    //
            FunBuildInfo& fbiInfo = FunBuildController::getInstance()->getFunbuildById(400000000); //fusheng 泰坦道具加速
    
            if(fbiInfo.state == FUN_BUILD_UPING)
            {
               int toolID = ToolController::getInstance()->getSPDItem(ItemSpdMenu_City);
    
                auto& toolInfo = ToolController::getInstance()->getToolInfoById(toolID);
    
                if(toolInfo.getCNT()>0)
                {
                    if (queue_id != -1) {
                        PropSpeedupView::show(ItemSpdMenu_City, 400000000, queue_id);//fusheng 注意 不是只有1101  还有1102
                    }
                    
                }
            }
    

//    YesNoDialog::showTime( _lang("102120").c_str() , CCCallFunc::create(this, callfunc_selector(GeneralTitanPopupView::speedUpCallBack)), (int)this->feedCD, _lang("104903").c_str());
    
    
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
            if(isTouchInside(m_eBgSpr1, pTouch)){
                CCCommonUtils::flyHint("", "", _lang("E100008"));
            }else if(isTouchInside(m_eBgSpr2, pTouch)){
                CCCommonUtils::flyHint("", "", _lang("E100008"));
            }else if(isTouchInside(m_eBgSpr3, pTouch)){
                CCCommonUtils::flyHint("", "", _lang("E100008"));
            }else if(isTouchInside(m_eBgSpr4, pTouch)){
                CCCommonUtils::flyHint("", "", _lang("E100008"));
            }else if(isTouchInside(m_eBgSpr5, pTouch)){
                CCCommonUtils::flyHint("", "", _lang("E100008"));
            }else if(isTouchInside(m_eBgSpr6, pTouch)){
                CCCommonUtils::flyHint("", "", _lang("E100008"));
            }else if(isTouchInside(m_eBgSpr7, pTouch)){
                CCCommonUtils::flyHint("", "", _lang("E100008"));
            }else if(isTouchInside(m_eBgSpr8, pTouch)){
                CCCommonUtils::flyHint("", "", _lang("E100008"));
            }
    
//    if(isTouchInside(m_titanFeed, pTouch))
//    {
//        return true;
//    }
    if(isTouchInside(this->m_titanUngrade, pTouch))
    {
        if (m_titanId == "") {
            return false;
        }
        

        
        FunBuildInfo& fbiInfo = FunBuildController::getInstance()->getFunbuildById(400000000);//fusheng 使用建筑物状态
        
        if(fbiInfo.state == FUN_BUILD_UPING)
        {
        
            CCCommonUtils::flyHint("", "", "Titan is upgrading");
            return true;
        }
        
        PopupViewController::getInstance()->addPopupInView(TitanUpgradeView::create(400000000,CCString::create(m_titanId)->intValue()));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("Titan_Up"));
        return true;
    }
    if(isTouchInside(this->m_clickNode4, pTouch))
    {
        if (m_titanId == "") {
            return false;
        }

        
        
//        QueueInfo &qinfo =  GlobalData::shared()->allQueuesInfo[1101];
//        
//        if(qinfo.key == "400000000" && qinfo.finishTime > GlobalData::shared()->getWorldTime())
//        {
//            CCCommonUtils::flyHint("", "", "Titan is upgrading");
//            return true;
//        }
//        
//        PopupViewController::getInstance()->addPopupInView(TitanUpgradeView::create(400000000,CCString::create(m_titanId)->intValue()));
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("Titan_Up"));
        return true;
    }
    
    
    if(isTouchInside(this->m_titanSkill, pTouch)){

        
        
        CCCommonUtils::flyHint("", "", _lang("E100008"));//fusheng 技能界面先关闭

        
        
        //        onSkillClick(NULL, CCControlEvent::TOUCH_DOWN);
//        
//        CCArray* p = CCArray::create();
//        p->addObject(CCInteger::create(GENERAL_OPEN));
//        CCArray* d = CCArray::create();
//        
//        DataRecordCommand* cmd = new DataRecordCommand(OPEN_PANEL, p, d);
//        cmd->sendAndRelease();

        return true;

        
    }
    if(isTouchInside(this->m_titanDetail, pTouch))
    {
//        CCCommonUtils::flyHint("", "", _lang("E100008"));
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
    
    m_red1Pt->setVisible(false);
    m_red2Pt->setVisible(false);
    m_red3Pt->setVisible(false);
    m_red4Pt->setVisible(false);
    m_red5Pt->setVisible(false);
    m_red6Pt->setVisible(false);
    m_red7Pt->setVisible(false);
    m_red8Pt->setVisible(false);
    
    
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
