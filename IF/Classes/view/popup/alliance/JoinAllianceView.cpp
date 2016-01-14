//
//  JoinAllianceView.cpp
//  IF
//
//  Created by ganxiaohua on 14-3-24.
//
//

#include "JoinAllianceView.h"
#include "PopupViewController.h"
#include "GetAllianceListCommand.h"
#include "JoinAllianceCommand.h"
#include "ApplyAllianceCommand.h"
#include "AllianceManager.h"
#include "CancelAllianceApplyCommand.h"
#include "CreateAllianceView.h"
#include "AlertAddAllianceView.h"
#include "ParticleController.h"
#include "AllianceFlagPar.h"
#include "CheckAllianceInfoView.h"
#include "FunBuildController.h"
#include "SoundController.h"
#include "ShakeController.h"
#include "WorldController.h"
#include "GuideController.h"
#include "IFAllianceShakeLayer.h"
JoinAllianceView::~JoinAllianceView(){
    //CCLOG("~JoinAllianceView~JoinAllianceView~JoinAllianceView~JoinAllianceView~JoinAllianceView");
};

JoinAllianceView* JoinAllianceView::create(float operateAreaHeight){
    JoinAllianceView* ret = new JoinAllianceView(operateAreaHeight);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool JoinAllianceView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    this->setIsHDPanel(true);
    AllianceManager::getInstance()->allianceList.clear();
    CCLoadSprite::doResourceByCommonIndex(7, true);
    CCLoadSprite::doResourceByCommonIndex(205, true);
    CCLoadSprite::doResourceByCommonIndex(307, true);

    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(7, false);
        CCLoadSprite::doResourceByCommonIndex(205, false);
        CCLoadSprite::doResourceByCommonIndex(307, false);
    });
//    auto tbg = CCLoadSprite::loadResource("technology_09.png");
//    auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
//    int maxHight = CCDirector::sharedDirector()->getWinSize().height;
//    int curHight = -500;
//    while (curHight<maxHight) {
//        auto bg = CCLoadSprite::createSprite("technology_09.png");
//        bg->setAnchorPoint(ccp(0, 1));
//        bg->setPosition(ccp(0, curHight));
//        curHight += bg->getContentSize().height;
//        tBatchNode->addChild(bg);
//    }
//    if (CCCommonUtils::isIosAndroidPad()) {
//        tBatchNode->setScaleX(2.4);
//        tBatchNode->setScaleY(2048.0 / 852.0);
//    }
//    this->addChild(tBatchNode);
    auto tmpCCB = CCBLoadFile("JoinAllianceCCB",this,this);
    addLoadingAni();
    this->setContentSize(tmpCCB->getContentSize());
    int oldBgHeight = m_viewBg->getContentSize().height;
   
    changeBGHeight(m_viewBg);
    int newBgHeight = m_viewBg->getContentSize().height;
    m_viewBg->setVisible(false);
    int addHeight = newBgHeight - oldBgHeight;
    int oldWidth = m_allianceList->getContentSize().width;
    int oldHeight = m_allianceList->getContentSize().height;
    
    if(GlobalData::shared()->playerInfo.isInAlliance()){
        m_bottomNode->setVisible(false);
        addHeight += 100;
    }
    if (CCCommonUtils::isIosAndroidPad())
    {
        addHeight = ((CCDirector::sharedDirector()->getWinSize())).height - 2048;
    }
    m_allianceList->setPositionY(m_allianceList->getPositionY()-addHeight);//
    m_allianceList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
    
    
    
    m_allianceListBG->setPositionY(m_allianceListBG->getPositionY()-addHeight);//
    m_allianceListBG->setContentSize(CCSizeMake(m_allianceListBG->getContentSize().width, m_allianceListBG->getContentSize().height+addHeight));
    
    allianceArray = CCArray::create();
    m_tabView = CCTableView::create(this, m_allianceList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setDelegate(this);
    m_tabView->setTouchPriority(-1);
//    m_tabView->setBounceable(false);//fusheng 去掉弹性
    m_allianceList->addChild(m_tabView);
    
    auto sprite9 = CCLoadSprite::createScale9Sprite("nb_alliance_search_bg.png");
    CCSize s = CCSize(451, 52);
    if (CCCommonUtils::isIosAndroidPad()) {
        s = m_searchNode->getContentSize();
    }
    m_editBox = CCEditBox::create(s,sprite9);
    m_editBox->setAnchorPoint(ccp(0,0.5));
    m_editBox->setFontSize(20);
    if (CCCommonUtils::isIosAndroidPad()) {
        m_editBox->setAnchorPoint(CCPointZero);
        m_editBox->setFontSize(40);
    }
    m_editBox->setPlaceHolder("");
    m_editBox->setMaxLength(512);
    if (CCCommonUtils::isIosAndroidPad()) {
        m_editBox->setMaxLength(1024);
    }
    m_editBox->setFontColor(ccc3(0, 0, 0));
    m_editBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
    m_editBox->setReturnType(kKeyboardReturnTypeDone);
    m_editBox->setPosition(ccp(0,0));
    m_searchNode->addChild(m_editBox);
   
    this->setPosition(ccp(this->getPositionX(),this->getPositionY()+addHeight));
    
    CCCommonUtils::setButtonTitle(m_btnSearch, _lang("115013").c_str());
//    CCCommonUtils::setButtonTitle(m_btnAlliance, _lang("115007").c_str());
    string btnStr = "";
    if(FunBuildController::getInstance()->getMainCityLv()<GlobalData::shared()->alliance_cost_k8){
        m_btnAlliance->setEnabled(false);
//        btnStr = _lang_1("115507", CC_ITOA(GlobalData::shared()->alliance_cost_k8));
        btnStr.append(CC_ITOA(GlobalData::shared()->alliance_cost_k5)).append("&").append(_lang_1("115515", CC_ITOA(GlobalData::shared()->alliance_cost_k8)));
        m_iconGold->setVisible(true);
    }
    else if(FunBuildController::getInstance()->getMainCityLv()>=GlobalData::shared()->alliance_cost_k8 && FunBuildController::getInstance()->getMainCityLv()<GlobalData::shared()->alliance_cost_k6){
        m_btnAlliance->setEnabled(true);
        m_iconGold->setVisible(true);
        btnStr.append(CC_ITOA(GlobalData::shared()->alliance_cost_k5)).append("/").append(_lang_1("115241", CC_ITOA(GlobalData::shared()->alliance_cost_k6)));
    }else{
        m_btnAlliance->setEnabled(true);
        btnStr = _lang("115062");
        m_iconGold->setVisible(false);
        m_createBtnCost->setPositionX(m_createBtnCost->getPositionX()+70);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_createBtnCost->setPositionX(m_createBtnCost->getPositionX()+40);
        }
    }
    m_createBtnCost->setString(btnStr.c_str());
    m_allianceBtnTitle->setString(_lang("115007").c_str());
    
    if(GlobalData::shared()->playerInfo.isInAlliance()){
        m_btnAlliance->setVisible(false);
    }
    
    m_buttonNode->setPositionY(m_buttonNode->getPositionY()-addHeight);//
    m_bottomNode->setPositionY(m_bottomNode->getPositionY()-addHeight);
    
    for (int i=1; i<=4; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
        m_fireNode1->addChild(particle);
        
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
        m_fireNode2->addChild(particle1);
    }
    
    AllianceManager::getInstance()->needRefresh = false;
    GetAllianceListCommand* cmd = new GetAllianceListCommand("",1,0);
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(JoinAllianceView::updateAlliances), NULL));
    cmd->sendAndRelease();
    
    currentPage = 0;
    m_first = true;
    return true;
}

void JoinAllianceView::pageChange(){
    currentPage = 1;
    addLoadingAni();
    GetAllianceListCommand* cmd = new GetAllianceListCommand("",currentPage);
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(JoinAllianceView::updateAlliances), NULL));
    cmd->sendAndRelease();
}

void JoinAllianceView::updateAlliances(CCObject* data){
    NetResult* result = dynamic_cast<NetResult*>(data);
    auto params = _dict(result->getData());
    CCArray* arr =  (CCArray*)params->objectForKey("list");
    m_total_receive = params->valueForKey("total")->intValue();
//    m_total_receive = m_total_receive*3;//fusheng test
    allianceArray->removeAllObjects();
    AllianceManager::getInstance()->maxAlliancePage = m_total_receive/20+(m_total_receive%20==0?0:1);
    int num = arr->count();
//    for (int j = 0 ;j<3;j++)//fusheng test
    for (int i=0; i<num; i++) {
        CCDictionary* dicAlliance = (CCDictionary*)arr->objectAtIndex(i);
        AllianceInfo* alliance = new AllianceInfo();
        alliance->updateAllianceInfo(dicAlliance);
        if(alliance->recruit == 0)
        {
            CCLOG("OKKKK");
        }
        allianceArray->addObject(alliance);
        CC_SAFE_RELEASE(alliance);
    }
    //搜索无结果处理
    if(m_total_receive == 0){
        CCLabelIFTTF* noResult = CCLabelIFTTF::create();
        noResult->setFontSize(30);
        noResult->setDimensions(CCSizeMake(400,0));
        noResult->setPosition(m_allianceList->getContentSize()/2);
        noResult->setString(_lang("115204").c_str());
        m_allianceList->addChild(noResult, 10, 404);
    }else{
        m_allianceList->removeChildByTag(404);
    }
    m_tabView->reloadData();
    removeLoadingAni();
}


void JoinAllianceView::clickBtn1(CCObject * pSender, Control::EventType pCCControlEvent){
    addLoadingAni();
    GetAllianceListCommand* cmd = new GetAllianceListCommand("",1,0);
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(JoinAllianceView::updateAlliances), NULL));
    cmd->sendAndRelease();
}

void JoinAllianceView::clickBtn2(CCObject * pSender, Control::EventType pCCControlEvent){
    addLoadingAni();
    GetAllianceListCommand* cmd = new GetAllianceListCommand("",1,1);
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(JoinAllianceView::updateAlliances), NULL));
    cmd->sendAndRelease();
}

void JoinAllianceView::clickBtn3(CCObject * pSender, Control::EventType pCCControlEvent){
    addLoadingAni();
    GetAllianceListCommand* cmd = new GetAllianceListCommand("",1,2);
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(JoinAllianceView::updateAlliances), NULL));
    cmd->sendAndRelease();
}

void JoinAllianceView::createAlliacne(CCObject * pSender, Control::EventType pCCControlEvent){
    if(GlobalData::shared()->isCrossService){
        YesNoDialog::show(_lang("138064").c_str(), NULL);
        return ;
    }
    PopupViewController::getInstance()->addPopupView(CreateAllianceView::create(11));
}

void JoinAllianceView::onSelectClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
}

void JoinAllianceView::onSearchClick(CCObject * pSender, Control::EventType pCCControlEvent){
    m_allianceList->removeChildByTag(404);
    addLoadingAni();
    isRecommendView = false;
    const char* key = m_editBox->getText();
    GetAllianceListCommand* cmd = new GetAllianceListCommand(key,1,1);
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(JoinAllianceView::updateAlliances), NULL));
    cmd->sendAndRelease();
//    onAllianceShake(NULL);//testshake
}

void JoinAllianceView::onGameTick(float time){
    auto shakeLb =    m_shakeGuideNode->getChildByTag(2);
    if (shakeLb) {
        float lbWidth = shakeLb->getContentSize().width;
        float nextPosX = m_shakeGuideNode->getPositionX() - 0.1 * 300;
        if (nextPosX < -431 - lbWidth) {
            nextPosX = 302;
        }
        if (CCCommonUtils::isIosAndroidPad()) {
            nextPosX = m_shakeGuideNode->getPositionX() - 0.1 * 300;
            if (nextPosX < -1048 - lbWidth) {
                nextPosX = 695;
            }
        }
        m_shakeGuideNode->setPositionX(nextPosX);
    }
}
void JoinAllianceView::onShakeAni(){
    CCRotateBy * roaction = CCRotateBy::create(0.1f,60);
    CCRepeat * r1 = CCRepeat::create(CCSequence::create(roaction,roaction->reverse(),nullptr),4);
    CCDelayTime * d = CCDelayTime::create(1.0f);
    auto shakeSp =    m_shakeGuideNode->getChildByTag(1);
    if (shakeSp) {
        shakeSp->runAction(CCRepeatForever::create(CCSequence::create(r1,d,nullptr)));
    }
}
void JoinAllianceView::delayFun(float time){
    unschedule(schedule_selector(JoinAllianceView::onGameTick));
    auto shakeSp =    m_shakeGuideNode->getChildByTag(1);
    if (shakeSp) {
        shakeSp->stopAllActions();
    }
    m_shakeGuideNode->setVisible(false);
}
void JoinAllianceView::onEnter()
{
    PopupBaseView::onEnter();
    ShakeController::getInstance()->addAccelerometer();
    m_tabView->setTouchEnabled(true);
    CCLoadSprite::doResourceByCommonIndex(205, true);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    bool havePopup = false;
    if(GlobalData::shared()->playerInfo.isfirstJoin==1 && !GlobalData::shared()->playerInfo.isChatView && !GlobalData::shared()->playerInfo.isInAlliance()){
        PopupViewController::getInstance()->addPopupView(AlertAddAllianceView::create());
        havePopup = true;
    }
    if (!havePopup && !GlobalData::shared()->playerInfo.isInAlliance() && canShakeAlliance()) {
        m_shakeGuideNode->setVisible(true);
        onShakeAni();
        auto shakeLb =    m_shakeGuideNode->getChildByTag(2);
        if (shakeLb ) {
            dynamic_cast<CCLabelIF*>(shakeLb)->setString(_lang("115550"));
//            dynamic_cast<CCLabelIF*>(shakeLb)->setString("testlog-------gdfgdfniogndfogiof---------");
        }
        schedule(schedule_selector(JoinAllianceView::onGameTick), 0.1);
        scheduleOnce(schedule_selector(JoinAllianceView::delayFun), 15);
    }
    else{
        m_shakeGuideNode->setVisible(false);
    }
    if(AllianceManager::getInstance()->needRefresh){
        addLoadingAni();
        GetAllianceListCommand* cmd = new GetAllianceListCommand("",1,0);
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(JoinAllianceView::updateAlliances), NULL));
        cmd->sendAndRelease();
    }
    if (GlobalData::shared()->playerInfo.isInAlliance()) {
        this->setTitleName(_lang("138062"));
    }else{
        this->setTitleName(_lang("115008"));
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(JoinAllianceView::updateAlliances), MSG_INVITES_DATA, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(JoinAllianceView::showAlliance), MSG_ALERT_ALLIANCE_VIEW_CLOSE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(JoinAllianceView::onAllianceShake), MSG_ON_SHAKE, NULL);
}

void JoinAllianceView::onExit()
{
    unschedule(schedule_selector(JoinAllianceView::onGameTick));
    if (!WorldController::getInstance()->isInWorld) {
        ShakeController::getInstance()->removeAccelerometer();
    }
    PopupBaseView::onExit();
    m_tabView->setTouchEnabled(false);
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_INVITES_DATA);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ALERT_ALLIANCE_VIEW_CLOSE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ON_SHAKE);
    GlobalData::shared()->playerInfo.isChatView = false;
}
bool JoinAllianceView::canShakeAlliance(){
    return false;//todowdz 1.0.8
    if (GuideController::share()->isInTutorial()) {
        return false;
    }
    if(GlobalData::shared()->playerInfo.isInAlliance()){
        return false;
    }
    if (GlobalData::shared()->shakeAllianceFlag == 0) {
        return false;
    }
    return true;
}
void JoinAllianceView::onAllianceShake(CCObject* p){
    if(!canShakeAlliance()){
        return;
    }
    CCLOGFUNC("shakeAllianceLog----");
    
    if (ShakeController::getInstance()->m_shakeAllianceVec.size() == 0) {
        int page = CCUserDefault::sharedUserDefault()->getIntegerForKey("shakeAlliancePage", 0);
        GetAllianceListCommand* cmd = new GetAllianceListCommand("",++page,2);
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(JoinAllianceView::shakeDataBack), NULL));
        cmd->sendAndRelease();
        CCLOG("shakeAlliance cmd Log page= %d",page);
    }
    
    if (ShakeController::getInstance()->m_shakeAllianceVec.size() > 0) {
        CCLOGFUNC("showAlliance log shakeview");
        auto curentView =  dynamic_cast<IFAllianceShakeLayer*>(PopupViewController::getInstance()->getCurrentPopupView());
        if (curentView==NULL) {
            PopupViewController::getInstance()->addPopupView(IFAllianceShakeLayer::create());
        }
        else{
            curentView->refesh();
        }
    }
}
void JoinAllianceView::shakeDataBack(CCObject* pObj){
    NetResult* result = dynamic_cast<NetResult*>(pObj);
    auto params = _dict(result->getData());
    CCArray* arr =  (CCArray*)params->objectForKey("list");
    int m_total_receive = params->valueForKey("total")->intValue();
    ShakeController::getInstance()->m_shakeAllianceVec.clear();
    int num = arr->count();
    for (int i=0; i<num; i++) {
        CCDictionary* dicAlliance = (CCDictionary*)arr->objectAtIndex(i);
        AllianceInfo* alliance = new AllianceInfo();
        alliance->updateAllianceInfo(dicAlliance);
        ShakeController::getInstance()->m_shakeAllianceVec.pushBack(alliance);
    }
}
void JoinAllianceView::showAlliance(CCObject* p){
    m_allianceList->setVisible(true);
}

bool JoinAllianceView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {

    return true;
}
void JoinAllianceView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {

    if(isRecommendView && m_tabView->getContentOffset().y > 30 && m_total_receive!=0){
        addLoadingAni();
        m_tabView->setBounceable(false);
        this->scheduleOnce(schedule_selector(JoinAllianceView::callMore),0.0);
    }
    if(cellTouchIdx!=-1){
        ((JoinAlliaceCell*)m_tabView->cellAtIndex(cellTouchIdx))->unHighlight();
        cellTouchIdx = -1;
    }
    if (isTouchInside(m_btnAlliance, pTouch) && !m_btnAlliance->isEnabled()) {
        CCCommonUtils::flyHint("", "", _lang_1("115506", CC_ITOA(GlobalData::shared()->alliance_cost_k8)), 3, 0, true);
    }
}
void JoinAllianceView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(allianceArray->count() == 0){
        return;
    }
    
    float offsetY = m_tabView->getContentOffset().y;
    if (offsetY>0) {
        float a = m_tabView->getContentSize().height;
        float b = m_tabView->getParent()->getContentSize().height;
        offsetY = b-a>0?(b-a):0;
        offsetY += (pTouch->getLocation().y - pTouch->getStartLocation().y)/5;
        m_tabView->setContentOffset(ccp(0, offsetY));
    }
    
    if(offsetY>0 && isRecommendView && m_tabView->cellAtIndex(allianceArray->count())){
        CCLabelIF* txt = (CCLabelIF*)m_tabView->cellAtIndex(allianceArray->count())->getChildByTag(222);
        if(offsetY>30)
            txt->setString(_lang("105318").c_str());
        else
            txt->setString(_lang("115189").c_str());
    }
}
void JoinAllianceView::callMore(float t){
    m_tabView->setBounceable(true);
    int nextPage = ceil(allianceArray->count()/10.0) + 1;
    GetAllianceListCommand* cmd = new GetAllianceListCommand("",nextPage,0);
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(JoinAllianceView::getMoreInfo), NULL));
    cmd->sendAndRelease();
}
void JoinAllianceView::getMoreInfo(CCObject* data){
    NetResult* result = dynamic_cast<NetResult*>(data);
    auto params = _dict(result->getData());
    m_total_receive = params->valueForKey("total")->intValue();
    if(m_total_receive == 0){
        m_tabView->reloadData();
        m_tabView->setContentOffset(ccp(0,0));
        removeLoadingAni();
        return;
    }
    
    CCArray* arr =  (CCArray*)params->objectForKey("list");
    int num = arr->count();
    for (int i=0; i<num; i++) {
        CCDictionary* dicAlliance = (CCDictionary*)arr->objectAtIndex(i);
        AllianceInfo* alliance = new AllianceInfo();
        alliance->updateAllianceInfo(dicAlliance);
        allianceArray->addObject(alliance);
    }
    float currMinOffsetY = m_tabView->minContainerOffset().y;
    m_tabView->reloadData();
    float newMinOffsetY = m_tabView->minContainerOffset().y;
    removeLoadingAni();
    m_tabView->setContentOffset(ccp(0,newMinOffsetY-currMinOffsetY));
    m_tabView->setContentOffsetInDuration(ccp(0,newMinOffsetY-currMinOffsetY + 200), 0.5);
}

void JoinAllianceView::scrollViewDidScroll(CCScrollView* view){
 
    //fusheng add 保留滑动手感
    auto layout = view -> getContainer();
    
    float currentY = layout -> getPositionY();
   
   
//fusheng 需要上拉更新数据
//    if (currentY > 0 && layout -> getContentSize().height - view -> getViewSize().height >= 0) {
//        view -> setContentOffset(Vec2(0, 0));
//    }
    
   
    if (-currentY > layout -> getContentSize().height - view -> getViewSize().height)      {
        view -> setContentOffset(Vec2(0, -layout -> getContentSize().height + view -> getViewSize().height));
    }
}
void JoinAllianceView::getDataToServer(int page){
    std::string key  = m_editBox->getText();;
    if(key!="") return ;
    int num = allianceArray->count();
    int current = num/20+(num%20==0?0:1);
    if(current>=AllianceManager::getInstance()->maxAlliancePage){
        return ;
    }
    current = current+1;
    currentPage = current;
    addLoadingAni();
    GetAllianceListCommand* cmd = new GetAllianceListCommand("",current,0);
    cmd->sendAndRelease();
}

SEL_CCControlHandler JoinAllianceView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSearchClick", JoinAllianceView::onSearchClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "createAlliacne", JoinAllianceView::createAlliacne);
    return NULL;
}

bool JoinAllianceView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{

    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceListBG", CCScale9Sprite*, this->m_allianceListBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnSearch", CCControlButton*, this->m_btnSearch);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnAlliance", CCControlButton*, this->m_btnAlliance);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceBtnTitle", CCLabelIF*, this->m_allianceBtnTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_createBtnCost", CCLabelIF*, this->m_createBtnCost);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceList", CCNode*, this->m_allianceList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg", CCScale9Sprite*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_searchNode", CCNode*, this->m_searchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buttonNode", CCNode*, this->m_buttonNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode1", CCNode*, this->m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode2", CCNode*, this->m_fireNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconGold", CCSprite*, this->m_iconGold);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_shakeGuideNode", CCNode*, this->m_shakeGuideNode);
    return false;
}

void JoinAllianceView::tableCellTouched(  cocos2d::extension::TableView* table, CCTableViewCell* cell){
    
}

CCSize JoinAllianceView::tableCellSizeForIndex(CCTableView *table, ssize_t idx){
    if(allianceArray->count()==0){
        return CCSize(0, 0);
    }
    if(idx == allianceArray->count()){
        if (CCCommonUtils::isIosAndroidPad()) {
            return CCSize(1500, 60);
        }
        return CCSize(610, 30);
    }
    return cellSizeForTable(table);
}
CCSize JoinAllianceView::cellSizeForTable(CCTableView *table){
    if(allianceArray->count()==0){
        return CCSize(0, 0);
    }
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1500, 450);
    }
    return CCSize(610, 220);
}
CCTableViewCell* JoinAllianceView::tableCellAtIndex(  cocos2d::extension::TableView *table, ssize_t idx){
    if(allianceArray->count()==0){
        return NULL;
    }
    if(idx > allianceArray->count()){
        return NULL;
    }
    JoinAlliaceCell* cell = (JoinAlliaceCell*)table->dequeueCell();
    if(idx<allianceArray->count()){//
        if(cell){
            cell->updateAllianceInfo((AllianceInfo*)allianceArray->objectAtIndex(idx));
            
            cell->setIsFirst(idx == 0);
        }else{
           
            cell = JoinAlliaceCell::create((AllianceInfo*)allianceArray->objectAtIndex(idx),m_allianceList);
            
            cell->setIsFirst(idx == 0);
        }
        cell->getChildByTag(111)->setVisible(true);//ccb
        cell->getChildByTag(222)->setVisible(false);//get more Label
    }
    if(idx == allianceArray->count() && isRecommendView){
        if(cell==NULL){
            cell = JoinAlliaceCell::create(NULL,m_allianceList);
        }
        cell->updateAllianceInfo(NULL);
        cell->getChildByTag(111)->setVisible(false);//ccb
        cell->getChildByTag(222)->setVisible(true);//get more Label
        if(m_total_receive == 0){
            cell->getChildByTag(222)->setVisible(false);
        }
        
        cell->setIsFirst(idx == 0);
    }

    return cell;
}

ssize_t JoinAllianceView::numberOfCellsInTableView(  cocos2d::extension::TableView *table){
    if(allianceArray->count()==0){
        return 0;
    }
    if(isRecommendView)
        return allianceArray->count()+1;
    else
        return allianceArray->count();
}

//unsigned int JoinAllianceView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
//    return 1;
//}

void JoinAllianceView::addLoadingAni(){
    if(m_loadingIcon){
        m_allianceList->removeChild(m_loadingIcon);
        m_loadingIcon = NULL;
    }
    m_loadingIcon = CCLoadSprite::createSprite("loading_1.png");
    m_loadingIcon->setAnchorPoint(ccp(0.5, 0.5));
    auto cSize = m_allianceList->getContentSize();
    m_loadingIcon->setPosition(ccp(cSize.width/2,cSize.height/2));
    
    CCActionInterval * rotateto1 = CCRotateTo::create(0.5, 720);
    CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
    CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
    m_loadingIcon->runAction(rotateForever);
    m_allianceList->addChild(m_loadingIcon,1000000);
}
void JoinAllianceView::removeLoadingAni(){
    if(m_loadingIcon){
        m_allianceList->removeChild(m_loadingIcon);
        m_loadingIcon = NULL;
    }
}
void JoinAllianceView::tableCellHighlight(  cocos2d::extension::TableView *table, CCTableViewCell* cell){
//    ((JoinAlliaceCell*)cell)->setHighlight();//fusheng 点击不变暗
    cellTouchIdx = cell->getIdx();
}
void JoinAllianceView::tableCellUnhighlight(  cocos2d::extension::TableView *table, CCTableViewCell* cell){
//    ((JoinAlliaceCell*)cell)->m_bg->setColor(ccc3(255,255,255));
}
//=======================
void JoinAlliaceCell::updateAllianceInfo(AllianceInfo* info){
    m_info = info;
    if(!m_info) return;

    std::string str = "(";
    str.append(m_info->shortName.c_str());
    str.append(") ");
    str.append(m_info->name.c_str());
    if(m_allianceTxt){
        m_allianceTxt->setString(str.c_str());
    }
    str = _lang_1("115015",m_info->leader.c_str());
    if(m_leaderTxt){
        m_leaderTxt->setString(str.c_str());
    }
    if(m_allianceNumTxt){
        m_allianceNumTxt->setString(CCString::createWithFormat("%d/%d",m_info->currentNum,m_info->maxNum)->getCString());
    }
    str = CC_CMDITOA(m_info->totalForce);
    if(m_forceTxt){
        m_forceTxt->setString(str.c_str());
    }
    m_allianceIcon->removeAllChildrenWithCleanup(true);
    //    CCSprite* spr = CCLoadSprite::createSprite(m_info->getAllianceIcon().c_str());
    //    CCLOG("name=%s, icon=%s",m_info->name.c_str(),m_info->getAllianceIcon().c_str());
    //    CCSprite* spr = CCLoadSprite::createSprite("Allance_flay.png");
    //    m_allianceIcon->addChild(spr);
    CCLoadSprite::doResourceByCommonIndex(205, true);
    AllianceFlagPar* flag = AllianceFlagPar::create(m_info->getAllianceIcon().c_str());
    //flag->setScale(90.0/flag->getContentSize().width);
    m_allianceIcon->addChild(flag,0,444);
    //    CCCommonUtils::setSpriteMaxSize(spr, 80);
    string lang = ( strcmp(m_info->language.c_str(), "")==0?"115600":m_info->language );
    if(m_language){
        m_language->setString(_lang(lang).c_str());   
    }
    
    m_appNode->setVisible(false);
    bntRevoke->setVisible(false);
    m_btnJoin->setVisible(false);
    m_applyBtn->setVisible(false);
    bntRevoke->setEnabled(false);
    m_applyBtn->setEnabled(false);
    m_btnJoin->setEnabled(false);
    m_language->setVisible(true);
    if(m_info->isApplied){
        m_appNode->setVisible(true);
        m_language->setVisible(false);
    }
    //    if(m_info->isApplied){
    //        bntRevoke->setVisible(true);
    //        bntRevoke->setEnabled(true);
    //        m_appNode->setVisible(true);
    //    }else if(m_info->recruit==1){
    //        m_btnJoin->setVisible(true);
    //        m_btnJoin->setEnabled(true);
    //    }else{
    //        m_applyBtn->setVisible(true);
    //        m_applyBtn->setEnabled(true);
    //    }
    //todo
    m_RestrictIcon->removeAllChildrenWithCleanup(true);
    if (info->powerRestriction != 0 || info->castleRestriction != 0) {  //---
        if(info->recruit == 0 ){
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCSprite* spr = CCLoadSprite::createSprite("Alliance_Restriction.png");
        if (CCCommonUtils::isIosAndroidPad()) {
            spr->setScale(2);
        }
        m_RestrictIcon->addChild(spr);
        }
    }
}

JoinAlliaceCell* JoinAlliaceCell::create(AllianceInfo* info,CCNode* touchNode)
{
    auto ret = new JoinAlliaceCell(info,touchNode);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool JoinAlliaceCell::init()
{
    auto ccb = CCBLoadFile("JoinAllianceCell",this,this);
    ccb->setTag(111);
    
    m_allianceTxt = CCLabelIFTTF::create("", 24);
    m_allianceTxt->setColor(m_nameTxt->getColor());
    m_allianceTxt->setFontSize(m_nameTxt->getFontSize());
    m_allianceTxt->setAnchorPoint(m_nameTxt->getAnchorPoint());
    m_allianceTxt->setPosition(m_nameTxt->getPosition());
    m_nameTxt->getParent()->addChild(m_allianceTxt);
    
    CCCommonUtils::setButtonTitle(m_applyBtn, _lang("115019").c_str());
    //    CCCommonUtils::setButtonTitle(m_btnView, _lang("115018").c_str());
    CCCommonUtils::setButtonTitle(m_btnJoin, _lang("115020").c_str());
    CCCommonUtils::setButtonTitle(bntRevoke, _lang("115021").c_str());
    m_appTxt->setString(_lang("115056").c_str());
    //CCSprite* spr = CCLoadSprite::createSprite(m_info->getAllianceIcon().c_str());

    //    CCCommonUtils::setSpriteMaxSize(spr, 80);
    //this->schedule(schedule_selector(JoinAlliaceCell::scrollText), 0.3);
    //get more info
    CCLabelIF* m_moreInfo = CCLabelIF::create(_lang("115189").c_str());
    m_moreInfo->setPosition(ccp(305,20));
    m_moreInfo->setColor(ccc3(0,203,36));
    m_moreInfo->setFontSize(26);
    m_moreInfo->setTag(222);
    if (CCCommonUtils::isIosAndroidPad()) {
        m_moreInfo->setFontSize(52);
        m_moreInfo->setPosition(760, 40);
    }
    this->addChild(m_moreInfo);
    return true;
}

void JoinAlliaceCell::scrollText(){
    //    const char* str = m_instructionTxt->getString();
    //    std::string content = str;
    //    content = content.substr(1) + content.substr(0,1);
    //    m_instructionTxt->setString(content.c_str());
}

bool JoinAlliaceCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_touchPos = pTouch->getLocation();
    
    if (m_touchNode && isTouchInside(m_touchNode,pTouch)) {
        return true;
    }
    return false;
}
void JoinAlliaceCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
    CCPoint end = pTouch->getLocation();
    
    if (m_touchNode && isTouchInside(m_touchNode,pTouch) && isTouchInside(m_clickNode, pTouch) && fabsf(end.y - m_touchPos.y)<30 && m_info!=NULL) {
        //PopupViewController::getInstance()->addPopupInView(AllianceInfoView::create(m_info));
        this->unHighlight();//fusheng 取消按下状态
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        PopupViewController::getInstance()->addPopupInView(CheckAllianceInfoView::create(m_info));
    }
    //    if (m_touchNode && isTouchInside(m_touchNode,pTouch)) {
    //        if(m_btnJoin->isVisible() && isTouchInside(m_btnJoin,pTouch)){
    //            this->onJoinClick(NULL,Control::EventType::TOUCH_DOWN);
    //        }else if(m_btnView->isVisible() && isTouchInside(m_btnView,pTouch)){
    //            this->onViewClick(NULL,Control::EventType::TOUCH_DOWN);
    //        }
    //        else if(m_applyBtn->isVisible() && isTouchInside(m_applyBtn,pTouch)){
    //            this->onApplyClick(NULL,Control::EventType::TOUCH_DOWN);
    //        }else{
    //
    //        }
    //    }
}

void JoinAlliaceCell::onEnter() {
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,Touch_Popup, false);
    this->updateAllianceInfo(m_info);
}

void JoinAlliaceCell::onExit() {
    CCNode::onExit();
    setTouchEnabled(false);
}

void JoinAlliaceCell::onViewClick(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->addPopupInView(AllianceInfoView::create(m_info));
}

void JoinAlliaceCell::onJoinClick(CCObject * pSender, Control::EventType pCCControlEvent){
    ApplyAllianceCommand* cmd = new ApplyAllianceCommand(m_info->uid);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(JoinAlliaceCell::joinSuccess), NULL));
    cmd->sendAndRelease();
}

void JoinAlliaceCell::onApplyClick(CCObject * pSender, Control::EventType pCCControlEvent){
    ApplyAllianceCommand* cmd = new ApplyAllianceCommand(m_info->uid);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(JoinAlliaceCell::success), NULL));
    cmd->sendAndRelease();
}

void JoinAlliaceCell::onRevokeClick(CCObject * pSender, Control::EventType pCCControlEvent){
    CancelAllianceApplyCommand* cmd = new CancelAllianceApplyCommand(m_info->uid);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(JoinAlliaceCell::cancelApply), NULL));
    cmd->sendAndRelease();
}


void JoinAlliaceCell::success(CCObject* p){
    this->updateAllianceInfo(m_info);
    YesNoDialog::showVariableTitle(_lang_1("115023",GlobalData::shared()->playerInfo.allianceInfo.name.c_str()),NULL,_lang("115022").c_str());
}

void JoinAlliaceCell::cancelApply(CCObject* p){
    m_info->isApplied = false;
    this->updateAllianceInfo(m_info);
}

void JoinAlliaceCell::joinSuccess(CCObject* obj){
    YesNoDialog::showVariableTitle(_lang_1("115024",GlobalData::shared()->playerInfo.allianceInfo.name.c_str()),CCCallFunc::create(this, callfunc_selector(JoinAlliaceCell::joinLater)),_lang("confirm").c_str());
}

void JoinAlliaceCell::joinLater(){
    PopupViewController::getInstance()->removeAllPopupView();
    PopupViewController::getInstance()->addPopupInView(AllianceInfoView::create(&GlobalData::shared()->playerInfo.allianceInfo));
}

SEL_CCControlHandler JoinAlliaceCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onViewClick", JoinAlliaceCell::onViewClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onJoinClick", JoinAlliaceCell::onJoinClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onApplyClick", JoinAlliaceCell::onApplyClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRevokeClick", JoinAlliaceCell::onRevokeClick);
    return NULL;
}


bool JoinAlliaceCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_liantiao1", CCNode*, this->m_liantiao1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_liantiao2", CCNode*, this->m_liantiao2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIFTTF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leaderTxt", CCLabelIFTTF*, this->m_leaderTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceNumTxt", CCLabelIFTTF*, this->m_allianceNumTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_appTxt", CCLabelIFTTF*, this->m_appTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_forceTxt", CCLabelIFTTF*, this->m_forceTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnJoin", CCControlButton*, this->m_btnJoin);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnView", CCNode*, this->m_btnView);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_applyBtn", CCControlButton*, this->m_applyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "bntRevoke", CCControlButton*, this->bntRevoke);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_appNode", CCNode*, this->m_appNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceIcon", CCNode*, this->m_allianceIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode", CCNode*, this->m_clickNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_language", CCLabelIFTTF*, this->m_language);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_RestrictIcon", CCNode*, this->m_RestrictIcon);
    
    return false;
}

void JoinAlliaceCell::setHighlight(){
    if(!this->getChildByTag(111)->isVisible()) return;
//    this->getChildByTag(111)->setScale(0.97);
    m_bg->setColor(ccc3(128,128,128));
    ((CCSprite*)m_allianceIcon->getChildByTag(444)->getChildByTag(444))->setColor(ccc3(128,128,128));
}
void JoinAlliaceCell::unHighlight(){
    if(this==NULL || !this->getChildByTag(111)->isVisible()) return;
//    this->getChildByTag(111)->setScale(1.0);
    m_bg->setColor(ccc3(255,255,255));
    ((CCSprite*)m_allianceIcon->getChildByTag(444)->getChildByTag(444))->setColor(ccc3(255,255,255));
}

void JoinAlliaceCell::setIsFirst(bool isFirst)
{
    m_liantiao1->setVisible(isFirst);
    m_liantiao2->setVisible(!isFirst);
}
