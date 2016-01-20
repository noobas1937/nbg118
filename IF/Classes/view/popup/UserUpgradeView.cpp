//
//  UserUpgradeView.cpp
//  IF
//
//  Created by fubin on 14-4-8.
//
//

#include "UserUpgradeView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "FunBuildController.h"
#include "YesNoDialog.h"
#include "TalentController.h"
#include "GeneralManager.h"
#include "DataRecordCommand.h"
#include "RewardController.h"
#include "ParticleController.h"
#include "FlyRewardPic.h"
#include "UserLevelCommand.h"
#include "FBUtilies.h"
#include "GuideController.h"
#include "SoundController.h"

UserUpgradeView::UserUpgradeView(){
    CCLoadSprite::doResourceByCommonIndex(104, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(104, false);
    });
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UserUpgradeView::shareSuccess), MSG_FBFeedDialogResult, NULL);
};

UserUpgradeView::~UserUpgradeView(){
    CC_SAFE_RELEASE(m_cellArr);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FBFeedDialogResult);
};

void UserUpgradeView::shareSuccess(CCObject* param){
    this->scheduleOnce(schedule_selector(UserUpgradeView::delayClose), 0.12);
}

void UserUpgradeView::delayClose(float _time){
    if(m_waitInterFace){
        m_waitInterFace->remove();
        m_waitInterFace = NULL;
    }
   this->onRewardBtnClick(NULL, Control::EventType::TOUCH_DOWN);
}

UserUpgradeView* UserUpgradeView::create(){
    UserUpgradeView* ret = new UserUpgradeView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool UserUpgradeView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    m_cellArr = CCArray::create();
    m_cellArr->retain();
    TalentController::getInstance()->makeEndowment();
    
    auto tmpCCB = CCBLoadFile("UserUpgradeView",this,this);
    //this->setContentSize(tmpCCB->getContentSize());
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    setContentSize(size);
    this->setModelLayerTouchCallback([&](cocos2d::CCTouch *pTouch){
        // detect touch inside panel
        if (isTouchInside(this, pTouch)) {
            this->onRewardBtnClick(NULL, Control::EventType::TOUCH_DOWN);
        }
    });
    m_hatAnimNode->setPositionY(m_hatAnimNode->getPositionY()+40);
//    m_titleLabel->setString(_lang("103461").c_str());
    m_titleLabel->setString(_lang("113184"));
//    m_nameLabel->setString(GlobalData::shared()->playerInfo.name);
   // int userLv = GlobalData::shared()->playerInfo.level;
    int userLv = GlobalData::shared()->lordInfo.levelUp+1;
//    string lvInfo = _lang_1("103462",CC_ITOA(userLv));
    
    string lvInfo = _lang_1("113185", CC_ITOA(userLv));
    if(userLv>=GlobalData::shared()->MaxPlayerLv) {
        lvInfo += _lang("102183");
    }
    m_lvLabel->setString(lvInfo.c_str());
    
    int lastEndowment = 0;
    int lastPower = 0;
    
    int key = 100100 + userLv-1;
    auto dict = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(key));
    int curEndowment = dict->valueForKey("endowment")->intValue();
    int curPower = dict->valueForKey("power")->intValue();
    if (userLv>2) {
        int lastkey = 100100 + (userLv-2);
        auto lastdict = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(lastkey));
        lastEndowment = lastdict->valueForKey("endowment")->intValue();
        lastPower = lastdict->valueForKey("power")->intValue();
    }
    
    m_powLabel->setString(_lang("105034").c_str());
    
    std::string language = LocalController::shared()->getLanguageFileName();
    float sizeWidth = m_powLabel->getContentSize().width;
    if (!m_powLabel->canBeSupportedWithBmpFont(language))
    {
        if(language == "pt")
        {
            sizeWidth = 255;
        }
        else
        {
            sizeWidth = 165;
        }
    }
    
    string powerInfo = "+";
    powerInfo = powerInfo + CC_ITOA(curPower-lastPower);
    m_powNumLabel->setString(powerInfo.c_str());
    m_powNumLabel->setPositionX(m_powLabel->getPositionX() + sizeWidth*m_powLabel->getOriginScaleX()+20);
    
    m_skillLabel->setString(_lang("105035").c_str());
//    m_skillLabel->setMaxScaleXByWidth(200);
    sizeWidth = m_skillLabel->getContentSize().width;
    if (!m_skillLabel->canBeSupportedWithBmpFont(language))
    {
        if(language == "pt")
        {
            sizeWidth = 255;
        }
        else
        {
            sizeWidth = 165;
        }
    }
    string skillInfo = "+";
    skillInfo = skillInfo + CC_ITOA(curEndowment-lastEndowment);
    m_skillNumLabel->setString(skillInfo.c_str());
    float px=m_skillLabel->getPositionX() + m_skillLabel->getContentSize().width+20;
//    m_skillNumLabel->setPositionX(px>m_powNumLabel->getPositionX()?px:m_powNumLabel->getPositionX());//m_skillLabel->getPositionX() + sizeWidth*m_skillLabel->getOriginScaleX()+20
    if (CCCommonUtils::isIosAndroidPad()) {
        m_skillNumLabel->setPositionX(m_skillLabel->getPositionX() + sizeWidth*m_skillLabel->getOriginScaleX()+20);
    }
    else
        m_skillNumLabel->setPositionX(px>m_powNumLabel->getPositionX()?px:m_powNumLabel->getPositionX());
    //CCCommonUtils::setButtonTitle(m_okBtn, _lang("confirm").c_str());
    string skillBtnInfo = _lang("103451") + ": " + CC_ITOA(TalentController::getInstance()->curEndowment);
//    CCCommonUtils::setButtonTitle(m_skillBtn, skillBtnInfo.c_str());
//    std::string btnStr = "";
//    std::map<int, int>::iterator it = GlobalData::shared()->generalConfig.abilityOpenedLevel.find(GlobalData::shared()->playerInfo.level);
//    if(it != GlobalData::shared()->generalConfig.abilityOpenedLevel.end()){
//        btnStr = _lang("105033");
//    }else{
//        //btnStr = _lang_1("105032", CC_ITOA(GeneralManager::getInstance()->getTotalFreeSkillPoint(GlobalData::shared()->generals.begin()->second)));
//        btnStr = _lang_1("105032", CC_ITOA(TalentController::getInstance()->curEndowment));
//    }
//    CCCommonUtils::setButtonTitle(m_skillBtn, btnStr.c_str());
    
    //get reward item
    m_fbTxt->setString(_lang("107098").c_str());
    string item =  dict->valueForKey("level_item")->getCString();   CCLOG("///i///%s",item.c_str());
    int offY = 150;
    if(item==""){
        if (!CCCommonUtils::isIosAndroidPad()) {
            this->m_buildBG->setContentSize(CCSize(m_buildBG->getContentSize().width, m_buildBG->getContentSize().height-150));
            this->m_buildBG->setPositionY(this->m_buildBG->getPositionY()+150/2);
            m_kuangBG->setVisible(false);
            m_likeNode->setPositionY(m_likeNode->getPositionY() + offY);
        }
        return true;
    }
    CCCommonUtils::splitString(item, "|", vector_item);             CCLOG("///n///%s",vector_item[0].c_str());
    item =  dict->valueForKey("level_num")->getCString();          CCLOG("///n///%s",item.c_str());
    CCCommonUtils::splitString(item, "|", vector_number);
    
    int length = (vector_item.size()-1)/3+1;
    if (CCCommonUtils::isIosAndroidPad()) {
        length = (vector_item.size()-1)/4+1;
    }
    if(length>1){
        int dh = (length-1)*150;
        if (CCCommonUtils::isIosAndroidPad()) {
            dh = length*260;
            offY = dh - m_rewardlist->getContentSize().height;
            this->m_rewardlist->setContentSize(CCSize(m_rewardlist->getContentSize().width, this->m_rewardlist->getContentSize().height+offY));
            this->m_buildBG->setContentSize(CCSize(m_buildBG->getContentSize().width, m_buildBG->getContentSize().height+offY));
            this->m_buildBG->setPositionY(this->m_buildBG->getPositionY()-offY/2);
            this->m_kuangBG->setContentSize(CCSize(m_kuangBG->getContentSize().width, m_kuangBG->getContentSize().height+offY));
            this->m_kuangBG->setPositionY(this->m_kuangBG->getPositionY()-offY/2);
        }
        else {
            offY = dh;
            this->m_rewardlist->setContentSize(CCSize(m_rewardlist->getContentSize().width, this->m_rewardlist->getContentSize().height+dh));
            this->m_buildBG->setContentSize(CCSize(m_buildBG->getContentSize().width, m_buildBG->getContentSize().height+dh));
            this->m_buildBG->setPositionY(this->m_buildBG->getPositionY()-dh/2);
            this->m_kuangBG->setContentSize(CCSize(m_kuangBG->getContentSize().width, m_kuangBG->getContentSize().height+dh));
            this->m_kuangBG->setPositionY(this->m_kuangBG->getPositionY()-dh/2);
        }
    }else{
        offY = 0;
    }
    
    m_tabView = CCMultiColTableView::create(this, m_rewardlist->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setMultiColTableViewDelegate(this);
//    m_tabView->setTouchPriority(Touch_Default);
    m_rewardlist->addChild(m_tabView);
    m_tabView->setTouchEnabled(false);
    m_tabView->reloadData();
    
    std:: map<string, CCDictionary*>::iterator it = GlobalData::shared()->shareFbmap.find("level_up");
    bool flag = false;
    if (it!=GlobalData::shared()->shareFbmap.end()) {
        CCDictionary* dict = it->second;
        if (dict) {
            int on = dict->valueForKey("ON")->intValue();
            if (on==1) {
                flag = true;
            }
        }
    }
    if(GlobalData::shared()->analyticID == "cn1" || GlobalData::shared()->analyticID.find("cn_")<GlobalData::shared()->analyticID.length()){//平台是国内的，不显示facebook 的share btn 按钮
        flag = false;
    }
    if(GlobalData::shared()->playerInfo.level>3 && flag){
        m_likeNode->setPositionY(m_likeNode->getPositionY() - offY);
        m_likeNode->setVisible(true);
    }
    m_waitInterFace = NULL;
    if(GlobalData::shared()->playerInfo.level==6){
        CCPoint locPos = ccp(40, 70);
        CCPoint pos = m_buildBG->convertToWorldSpace(locPos);
        int len = (1-(pos.y/CCDirector::sharedDirector()->getWinSize().height))*100;
        GlobalData::shared()->isBind = true;
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//        //pos = CCDirector::sharedDirector()->convertToUI(pos);
//        pos.y = len;
//        FBUtilies::fbLike(pos);
//#else
//        string localCus = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
//        pos = ccp(pos.x, pos.y + 18);
//        FBUtilies::fbLike(pos);
//#endif
    }

    return true;
}

void UserUpgradeView::onEnter(){
    PopupBaseView::onEnter();
    playLvAnim();
}

void UserUpgradeView::onExit(){
    GlobalData::shared()->isBind = false;
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(UserUpgradeView::playGiftAnim), this);
    PopupBaseView::onExit();
}

void UserUpgradeView::onShareBtnClick(CCObject * pSender, Control::EventType pCCControlEvent){
    m_fbShareBtn->setEnabled(false);
    GlobalData::shared()->isBind = true;
    string link = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    link = "https://fb.me/789279541113368?from_feed=android_king";
#else
    link = "https://fb.me/789290781112244?from_feed=ios_king";
#endif
    std:: map<string, CCDictionary*>::iterator it = GlobalData::shared()->shareFbmap.find("level_up");
    bool flag = false;
    string name = "Clash Of Kings";
    string caption = _lang("107088");
    string linkDescription = _lang("107087");
    string pictureUrl = "http://cok.eleximg.com/cok/img/King_Feed_1.png";
    string ref = "";
    string shareDialog = "";
    if (it!=GlobalData::shared()->shareFbmap.end()) {
        CCDictionary* dict = it->second;
        if (dict) {
            name = _lang(dict->valueForKey("name")->getCString());
            caption = _lang(dict->valueForKey("caption")->getCString());
            shareDialog = dict->valueForKey("linkDescription")->getCString();
            linkDescription = _lang_1(shareDialog,CC_ITOA(GlobalData::shared()->playerInfo.level));
            pictureUrl = dict->valueForKey("pictureUrl")->getCString();
            link = dict->valueForKey("link")->getCString();
            ref = dict->valueForKey("ref")->getCString();
        }
    }
    FBUtilies::fbPublishFeedDialog(name,caption,linkDescription,link,pictureUrl,1,ref);
    FBUtilies::appEventShareLog(CC_ITOA(GlobalData::shared()->playerInfo.selfServerId), GlobalData::shared()->playerInfo.level, FunBuildController::getInstance()->getMainCityLv(), shareDialog);
    FBUtilies::appEventLevelUp(GlobalData::shared()->playerInfo.level);
    m_waitInterFace = GameController::getInstance()->showWaitInterface(m_fbShareBtn);
}

SEL_CCControlHandler UserUpgradeView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
   // CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSkillBtnClick", UserUpgradeView::onSkillBtnClick);
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOKBtnClick", UserUpgradeView::onOKBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onShareBtnClick", UserUpgradeView::onShareBtnClick);
    return NULL;
}

bool UserUpgradeView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
 //   CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_okBtn", CCControlButton*, this->m_okBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_skillBtn", CCControlButton*, this->m_skillBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, this->m_titleLabel);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvLabel", CCLabelIF*, this->m_lvLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powLabel", CCLabelIF*, this->m_powLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powNumLabel", CCLabelBMFont*, this->m_powNumLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_skillLabel", CCLabelIF*, this->m_skillLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_skillNumLabel", CCLabelBMFont*, this->m_skillNumLabel);    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardlist", CCNode*, this->m_rewardlist);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hatAnimNode", CCNode*, this->m_hatAnimNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_LvAnimNode", CCNode*, this->m_LvAnimNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_kuangBG", CCScale9Sprite*, this->m_kuangBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_likeNode", CCNode*, this->m_likeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fbTxt", CCLabelIFTTF*, this->m_fbTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fbShareBtn", CCControlButton*, this->m_fbShareBtn);
    return false;
}

void UserUpgradeView::playLvAnim(){
    CCActionInterval * delatTime = CCDelayTime::create(0.15);
    CCCallFunc * funcall= CCCallFunc::create(this, callfunc_selector(UserUpgradeView::playLvParticle));
    m_titleLabel->setScale(0.1);
    if (CCCommonUtils::isIosAndroidPad()) {
        CCScaleTo* iconRotate = CCScaleTo::create(0.125f, 2.6);
        CCScaleTo* iconRotate1 = CCScaleTo::create(0.125f, 1.8);
        CCScaleTo* iconRotate2 = CCScaleTo::create(0.125f, 0.2);
        CCScaleTo* iconRotate3 = CCScaleTo::create(0.25f, 2.0);
        CCScaleTo* iconRotate4 = CCScaleTo::create(0.25f, 1.0);
        CCSequence* action  =CCSequence::create(iconRotate, iconRotate1,iconRotate2,iconRotate3,NULL);
        CCSequence* action1  =CCSequence::create(iconRotate, iconRotate1,iconRotate2,iconRotate4,NULL);
        m_titleLabel->runAction(action1);
        m_powNumLabel->runAction((CCAction *)action->clone());
        m_skillNumLabel->runAction((CCAction *)action->clone());
        this->runAction(CCSequence::create(delatTime,  funcall,NULL));
    }
    else {
        CCScaleTo* iconRotate = CCScaleTo::create(0.125f, 1.3);
        CCScaleTo* iconRotate1 = CCScaleTo::create(0.125f, 0.9);
        CCScaleTo* iconRotate2 = CCScaleTo::create(0.125f, 1.1);
        CCScaleTo* iconRotate3 = CCScaleTo::create(0.25f, 1.0);
        CCSequence* action  =CCSequence::create(iconRotate, iconRotate1,iconRotate2,iconRotate3,NULL);
        m_titleLabel->runAction(action);
        m_powNumLabel->runAction((CCAction *)action->clone());
        m_skillNumLabel->runAction((CCAction *)action->clone());
        this->runAction(CCSequence::create(delatTime,  funcall,NULL));
    }
}
void UserUpgradeView::playLvParticle(){
    m_LvAnimNode->removeAllChildren();
    ParticleController::initParticle();
    
    auto newBatch = ParticleController::createParticleBatch();
    auto praticle1 = ParticleController::createParticle("Capacity_1");
    newBatch->addChild(praticle1);
    auto praticle2 = ParticleController::createParticle("Capacity_2");
    newBatch->addChild(praticle2);
    auto praticle3= ParticleController::createParticle("Capacity_3");
    newBatch->addChild(praticle3);
    auto praticle4 = ParticleController::createParticle("Capacity_4");
    newBatch->addChild(praticle4);
    auto praticle5 = ParticleController::createParticle("Capacity_5");
    newBatch->addChild(praticle5);
    auto praticle6 = ParticleController::createParticle("Capacity_6");
    newBatch->addChild(praticle6);
    m_LvAnimNode->addChild(newBatch);
    int dx = 0;
    praticle1->setPosition(ccp(dx, 0));
    praticle1->setAnchorPoint(ccp(0, 0));
    praticle2->setPosition(ccp(dx, 0));
    praticle2->setAnchorPoint(ccp(0, 0));
    praticle3->setPosition(ccp(dx, 0));
    praticle3->setAnchorPoint(ccp(0, 0));
    praticle4->setPosition(ccp(dx, 0));
    praticle4->setAnchorPoint(ccp(0, 0));
    praticle5->setPosition(ccp(dx, 0));
    praticle5->setAnchorPoint(ccp(0, 0));
    praticle6->setPosition(ccp(dx, 0));
    praticle6->setAnchorPoint(ccp(0, 0));
    playhatParticle();
    SoundController::sharedSound()->playEffects(Music_Sfx_UI_levelup);
//    for (int i=0; i<5; i++) {
//        playGiftAnim();
//    }
    m_falg = 0;
    m_endfalg = 0;
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(UserUpgradeView::playGiftAnim), this);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(UserUpgradeView::playGiftAnim), this, 0.2f,5, 0.0f, false);

}
void UserUpgradeView::playhatParticle(){
    this->m_hatAnimNode->removeAllChildren();
    auto newBatch = ParticleController::createParticleBatch();
    for (int i=1; i<8; i++) {
        auto praticle1 = ParticleController::createParticle(CCString::createWithFormat("LevelUp_%d",i)->getCString());
        newBatch->addChild(praticle1);
    }
    m_hatAnimNode->addChild(newBatch);
}
void UserUpgradeView::playGiftAnim(float t){
    m_falg++;
    auto newBatch = ParticleController::createParticleBatch();
    for (int i=1; i<5; i++) {
        
        auto praticle1 = ParticleController::createParticle(CCString::createWithFormat("Fireworks_%d",i)->getCString());
        newBatch->addChild(praticle1);
    }
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    int maxX = size.width;
    int maxY = size.height;
    int x = rand() % maxX;
    int y = rand() % maxY;
    this->addChild(newBatch,1000,m_falg+1000);
    newBatch->setPosition(x, y);
    //给烟花加尾动画
    CCActionInterval * delatTime = CCDelayTime::create(0.35);
    CCCallFunc * funcall= CCCallFunc::create(this, callfunc_selector(UserUpgradeView::playGiftEndAnim));
    this->runAction(CCSequence::create(delatTime,  funcall,NULL));
}
void UserUpgradeView::playGiftEndAnim(){
    m_endfalg++;
    auto node  = this->getChildByTag(m_endfalg+1000);
    if(node!=NULL){
        auto praticle1 = ParticleController::createParticle(CCString::createWithFormat("Fireworks_%d",5)->getCString());
        node->addChild(praticle1);
    }

}
void UserUpgradeView::playGetRewardAnim(){
    int count = m_cellArr->count();
    float delay[count-1];

    vector<float> v;
    back_insert_iterator<std::vector<float> > p = back_inserter(v);
    for(int i = 0; i < count; ++i){
        *p = 0.1*(i+1);
    }
    random_shuffle(v.begin(), v.end());

    for (int i = 0; i<count; i++) {
        CCTableViewCell* cell = dynamic_cast<CCTableViewCell*>(m_cellArr->objectAtIndex(i));
        auto node = cell->getChildByTag(1000);
        auto node1 = node->getChildByTag(999);
        if(node1){

            float delaytime = v[i];
            int type = R_GOODS;
            int value = atoi(vector_item[i].c_str());
            string picth = RewardController::getInstance()->getPicByType(type, value);
            CCPoint p = node1->getPosition();
            CCDirector::sharedDirector()->convertToGL(p);
            CCPoint p2 = node1->getParent()->convertToWorldSpace(p);
        //CCPoint localP = m_gView->convertToNodeSpaceAR(p2);
            FlyRewardPic::addFlyRewardAnim("goods",p2 , picth,delaytime);
            SoundController::sharedSound()->playEffects(Music_Sfx_UI_collect_item);
        }
    }
}

void UserUpgradeView::onSkillBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{

}

void UserUpgradeView::onOKBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (GlobalData::shared()->playerInfo.level == 2) {
        CCArray* p = CCArray::create();
        p->addObject(CCInteger::create(UPGRADE_OK));
        CCArray* d = CCArray::create();
        
        DataRecordCommand* cmd = new DataRecordCommand(OPEN_PANEL, p, d);
        cmd->sendAndRelease();
    }
    
    PopupViewController::getInstance()->removePopupView(this);
}
void UserUpgradeView::onRewardBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    
    
    if(vector_item.size()>0){
        playGetRewardAnim();
    }
    int level = GlobalData::shared()->lordInfo.levelUp+1;
    UserLevelRewardCommand* cmd = new UserLevelRewardCommand(level);
    cmd->sendAndRelease();
    this->closeSelf();
    
    if (level==3) {//引导使用新手礼包
        GuideController::share()->setGuide("3330100");
    }
    else if (level==5 && !GlobalData::shared()->isXiaoMiPlatForm() ) {//引导绑定
        GuideController::share()->setGuide("3370100");
    }
    
//    CCDictionary* tmp = CCDictionary::create();
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_PLAYER_LEVELUP,tmp);
}
void UserUpgradeView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
    
}

cocos2d::CCSize UserUpgradeView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    if (CCCommonUtils::isIosAndroidPad()) {
        return  CCSizeMake(250, 230);
    }
    return CCSize(140, 132);
}

CCTableViewCell* UserUpgradeView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx){
    int num = vector_item.size();
    int type = R_GOODS;
    int value = atoi(vector_item[0].c_str());
    std::string str = "";
    if(idx >= num){
        return NULL;
    }
    CCTableViewCell* pCell = table->dequeueGrid();
    if(idx < num){
        if(!pCell){
            pCell = new CCTableViewCell();
            pCell->autorelease();
            CCNode* node = CCNode::create();
            auto bg = CCLoadSprite::createScale9Sprite("icon_kuang.png");
            if (CCCommonUtils::isIosAndroidPad()) {
                bg->setPreferredSize(CCSizeMake(150, 150));
            }
            else
                bg->setPreferredSize(CCSize(98,98));
            node->addChild(bg);
            string name = CCCommonUtils::getNameById(vector_item[idx]);
            auto text = CCLabelIF::create(name.c_str());
            if (CCCommonUtils::isIosAndroidPad()) {
                text->setDimensions(CCSize(250, 0));
            }
            else
                text->setDimensions(CCSize(140, 0));
            text->setAnchorPoint(ccp(0.5,1.0));
            text->setAlignment(kCCTextAlignmentCenter);
            text->setVerticalAlignment(kCCVerticalTextAlignmentTop);
            if (CCCommonUtils::isIosAndroidPad()) {
                text->setFontSize(35);
            }
            else
                text->setFontSize(16);
            text->setColor({60,28,0});
            if (CCCommonUtils::isIosAndroidPad()) {
                text->setPosition(ccp(0, -80));
            }
            else
                text->setPosition(ccp(0, -50));
            node->addChild(text);
            value = atoi(vector_item[idx].c_str());
            auto icon = CCLoadSprite::createScale9Sprite(RewardController::getInstance()->getPicByType(type, value).c_str());
            float scale = 94/icon->getContentSize().width;
            if(scale>1){
                scale = 1.0;
            }
            if (CCCommonUtils::isIosAndroidPad()) {
                icon->setScale(scale * 1.5);
            }
            else
                icon->setScale(scale);
            node->addChild(icon,999,999);
            if (CCCommonUtils::isIosAndroidPad()) {
                node->setPosition(ccp(120, 130));
            }
            else
                node->setPosition(ccp(70,70));
            pCell->addChild(node,1000,1000);
            m_cellArr->addObject(pCell);
        }else{
            
        }
    }
    return pCell;

}

ssize_t UserUpgradeView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    int num = vector_item.size();
    if (CCCommonUtils::isIosAndroidPad()) {
        return ceil(num/4.0);
    }
    return ceil(num/3.0);
}

unsigned int UserUpgradeView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    if (CCCommonUtils::isIosAndroidPad()) {
        return 4;
    }
    return 3;
}


