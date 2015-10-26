//
//  SetAccountView.cpp
//  IF
//
//  Created by wangzhenlei on 13-11-4.
//
//

#include "SetAccountView.h"
#include "PopupViewController.h"
#include "SceneController.h"
#include "LocalController.h"
#include "CCCommonUtils.h"
#include "LoginServiceView.h"
#include "NetController.h"
#include "FBUtilies.h"
#include "../../Ext/CCDevice.h"
#include "UserBindCommand.h"
#include "SuggestionView.h"
#include "PopupViewController.h"
#include "SettingPopUpView.h"
#include "RegisterView.h"
#include "ParticleController.h"
#include "YesNoDialog.h"
#include "WorldController.h"
#include "FunBuildController.h"
#include "PlatformUtil.h"
#include "AppLibHelper.h"
#include <spine/Json.h>
#include "AchievementController.h"
#include "LuaController.h"

#define GM_Bind 1

int SERCER_TYPE = 0;
int WAR_NUM = 0;
string WARDID = "105265";
SetAccountView* SetAccountView::create(){
    SetAccountView* ret = new SetAccountView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool SetAccountView::init(){
    if(!PopupBaseView::init()){
        return false;
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    WARDID = "113904";
#endif
    setIsHDPanel(true);
    auto bg = CCBLoadFile("SetAccount", this, this);
    auto size = CCDirector::sharedDirector()->getWinSize();
    CCCommonUtils::setButtonTitle(m_bindBtn, _lang("105258").c_str());
    CCCommonUtils::setButtonTitle(m_changeBtn, _lang("105282").c_str());
    CCCommonUtils::setButtonTitle(m_newBtn, _lang("105263").c_str());
    m_waitInterface = NULL;
    setTitleName(_lang("105253").c_str());
    float preh =m_viewBg->getContentSize().height;
    changeBGHeight(m_viewBg);
    float dh = m_viewBg->getContentSize().height-preh;
    this->setContentSize(bg->getContentSize());
    if(dh>0){
        this->m_Listnode->setPositionY(this->m_Listnode->getPositionY()-dh/2);
        m_topNode->setPositionY(m_topNode->getPositionY()+dh/2);
        m_top2Node->setPositionY(m_top2Node->getPositionY()+dh/2);
    }
    
    string playerName = GlobalData::shared()->playerInfo.name;
    string nameLabelStr=playerName+"  ("+_lang("108755")+CC_ITOA(GlobalData::shared()->playerInfo.selfServerId)+")";
    m_nameLabel->setString(nameLabelStr.c_str());
    m_castleLabel->setString(_lang("105249")+CC_ITOA(FunBuildController::getInstance()->getMainCityLv()));
    
    m_iconNode->removeAllChildren();
    CCSprite* head = CCLoadSprite::createSprite(GlobalData::shared()->playerInfo.getPic().c_str());
    m_iconNode->addChild(head);
    CCCommonUtils::setSpriteMaxSize(head, 77,true);
    
    if (GlobalData::shared()->nowGameCnt < GlobalData::shared()->maxGameCnt) {
        m_newActLabel->setString(_lang("105235"));
    } else {
        m_newActLabel->setString(_lang("137437"));
    }
    m_rdMsgLabel->setString(_lang("105234"));
    
    onUpdateState();
    return true;
}

void SetAccountView::onUpdateState()
{
    m_noAct1Label->setString("");
    m_noAct2Label->setString("");
    m_fbActLabel->setString("");
    m_gpActLabel->setString("");
    
    m_facebookUid = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID, "");
    m_facebookName = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERNAME, "");
    m_googleUid = CCUserDefault::sharedUserDefault()->getStringForKey(GP_USERID, "");
    m_googleName = CCUserDefault::sharedUserDefault()->getStringForKey(GP_USERNAME, "");
    
    string tmpFBT = _lang("105269") + ":";
    m_fbActLabel->setString(tmpFBT.c_str());
    m_fbActMsgLabel->setString(_lang("105230").c_str());
    if (m_facebookUid!="" || m_facebookName!="") {
        string tmpFB = "";
        if (m_facebookName!="") {
            tmpFB = m_facebookName + " " + _lang("105295");
        }else {
            tmpFB = m_facebookUid + " " + _lang("105295");
        }
        m_fbActMsgLabel->setString(tmpFB.c_str());
    }
    
    string tmpGPT = _lang("105270") + ":";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    tmpGPT = _lang("113900") + ":";
#endif
    if(GlobalData::shared()->analyticID == "cn1"){
        tmpGPT = _lang("113969");
    }
    
    m_gpActLabel->setString(tmpGPT.c_str());
    m_gpActMsgLabel->setString(_lang("105230").c_str());
    if (m_googleUid!="" || m_googleName!="") {
        string tmpGP = "";
        
        if (m_googleName!="") {
            tmpGP = m_googleName + " " + _lang("105295");
        }else {
            tmpGP = m_googleUid + " " + _lang("105295");
        }
        m_gpActMsgLabel->setString(tmpGP.c_str());
    }
    if(GlobalData::shared()->isWeiboEnabled()){
        string tmpGP = _lang("105230");
        auto &info = GlobalData::shared()->playerInfo.m_platFormBindInfo[WEIBO];
        if (info.getBindName() != "") {
            tmpGP = info.getBindName() + " " + _lang("105295");
        }else if(info.bindId != ""){
            tmpGP = info.bindId + " " + _lang("105295");
        }
        m_gpActMsgLabel->setString(tmpGP.c_str());
    }
    
    if (m_facebookUid=="" && m_facebookName=="" && m_googleUid=="" && m_googleName=="") {
        m_noAct1Label->setString(_lang("105255")+_lang("103748"));
        m_noAct2Label->setString(_lang("105257")+_lang("105258"));
    }
    else {
        m_noAct1Label->setString(_lang("105255")+_lang("105256"));
//        m_noAct1Label->setPositionY(m_noAct1Label->getPositionY()-20);
        m_noAct2Label->setString("");
    }
    
    
    if(GlobalData::shared()->playerInfo.bindFlag) {
        m_rdMsgLabel->setVisible(false);
    }else {
        m_rdMsgLabel->setVisible(true);
    }
}

void SetAccountView::onEnter(){
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SetAccountView::onRetWarOKNewAccount), MSG_RET_WARING_ACC, NULL);
    onUpdateState();
    setTitleName(_lang("105253").c_str());
}

void SetAccountView::onExit(){
    GlobalData::shared()->isBind = false;
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_RET_WARING_ACC);
    CCNode::onExit();
}

bool SetAccountView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg", CCScale9Sprite*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bindBtn", CCControlButton*, this->m_bindBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_changeBtn", CCControlButton*, this->m_changeBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_Listnode", CCNode*, this->m_Listnode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_googleFlagspr", CCSprite*, this->m_googleFlagspr);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newBtn", CCControlButton*, this->m_newBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_gNode", CCNode*, this->m_gNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_topNode", CCNode*, this->m_topNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_top2Node", CCNode*, this->m_top2Node);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", Label*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_castleLabel", CCLabelIF*, this->m_castleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fbActLabel", CCLabelIFTTF*, this->m_fbActLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_gpActLabel", CCLabelIFTTF*, this->m_gpActLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fbActMsgLabel", CCLabelIFTTF*, this->m_fbActMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_gpActMsgLabel", CCLabelIFTTF*, this->m_gpActMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noAct1Label", CCLabelIF*, this->m_noAct1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noAct2Label", CCLabelIF*, this->m_noAct2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newActLabel", CCLabelIF*, this->m_newActLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rdMsgLabel", CCLabelIF*, this->m_rdMsgLabel);
    return true;
}

SEL_CCControlHandler SetAccountView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBindClick", SetAccountView::onBindClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onChangeClick", SetAccountView::onChangeClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onNewAccountClick", SetAccountView::onNewAccountClick);
    return NULL;
}

CCNode* SetAccountView::getGuideNode(string _key)
{
    if (_key == "SetAccount_bangding") {
        return m_bindBtn;
    }
    return NULL;
}

void SetAccountView::onBindClick(cocos2d::CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->addPopupInView(SetAccountNextView::create(1));
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("SetAccount_bangding"));
}
void SetAccountView::onChangeClick(cocos2d::CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->addPopupInView(SetAccountNextView::create(2));
}

void SetAccountView::onNewAccountClick(cocos2d::CCObject *pSender, CCControlEvent event){
    if(GlobalData::shared()->isCrossService){
        YesNoDialog::show(_lang("138064").c_str(), NULL);
        return ;
    }
    int leftCnt = GlobalData::shared()->maxGameCnt - GlobalData::shared()->nowGameCnt;
    if (leftCnt == 2 || leftCnt == 1)
    {
        PopupViewController::getInstance()->addPopupView(WarningView::create(_lang("105261"), _lang_3("137436", CC_ITOA(GlobalData::shared()->playerInfo.selfServerId), CC_ITOA(GlobalData::shared()->nowGameCnt), CC_ITOA(leftCnt)), CCCallFunc::create(this, callfunc_selector(SetAccountView::onStartNewAccount))));
    }
    else if (leftCnt <= 0)
    {
        PopupViewController::getInstance()->addPopupView(WarningView::create(_lang("105261"), _lang_1("137438", CC_ITOA(GlobalData::shared()->playerInfo.selfServerId)), CCCallFunc::create(this, callfunc_selector(SetAccountView::onStartNewAccount))));
    }
    else
    {
        onStartNewAccount();
    }
}

void SetAccountView::onStartNewAccount()
{
    SERCER_TYPE = 0;
    WAR_NUM = 0;
    
    if (m_facebookUid != "" || m_googleUid != "") {//当前账号有绑定
        if (true) {//WorldController::getInstance()->isServerCrossOpen || GlobalData::shared()->playerInfo.gmFlag == 1
            SERCER_TYPE = 1;//当前服
            WAR_NUM = 2;
            //只提示一次警告
            int leftCnt = GlobalData::shared()->maxGameCnt - GlobalData::shared()->nowGameCnt;
            if (leftCnt <= 0) {
                PopupViewController::getInstance()->addPopupView(WarningView::create(_lang("105261"), _lang("137441"), CCCallFunc::create(this, callfunc_selector(SetAccountView::onSendNewAccount))));
            } else {
                PopupViewController::getInstance()->addPopupView(WarningView::create(_lang("105261"), _lang("105236"), CCCallFunc::create(this, callfunc_selector(SetAccountView::onSendNewAccount))));
            }
        }
        else {
            //直接选服
            PopupViewController::getInstance()->addPopupView(SelServerView::create(CCCallFunc::create(this, callfunc_selector(SetAccountView::onBidWarOKNewAccount))));
        }
    }
    else {
        if (true) {//WorldController::getInstance()->isServerCrossOpen || GlobalData::shared()->playerInfo.gmFlag == 1
            SERCER_TYPE = 1;//当前服
            WAR_NUM = 1;
            PopupViewController::getInstance()->addPopupView(WarningView::create(_lang("105261"), _lang(WARDID), CCCallFunc::create(this, callfunc_selector(SetAccountView::onSendNewAccount))));
        }
        else {
            //第一次警告
            PopupViewController::getInstance()->addPopupView(WarningView::create(_lang("105261"), _lang(WARDID), CCCallFunc::create(this, callfunc_selector(SetAccountView::onOKNewAccount))));
        }
    }
}

void SetAccountView::onOKNewAccount()
{
    //选择服务器
    PopupViewController::getInstance()->addPopupView(SelServerView::create(CCCallFunc::create(this, callfunc_selector(SetAccountView::onLastOKNewAccount))));
}

void SetAccountView::onLastOKNewAccount()
{
    //第二次警告
    WAR_NUM = 1;
    PopupViewController::getInstance()->addPopupView(WarningView::create(_lang("105262"), _lang(WARDID), CCCallFunc::create(this, callfunc_selector(SetAccountView::onSendNewAccount))));
}

void SetAccountView::onRetWarOKNewAccount(CCObject* p)
{
    //服务器返回 再次警告
    WAR_NUM = 2;
    PopupViewController::getInstance()->addPopupView(WarningView::create(_lang("105262"), _lang(WARDID), CCCallFunc::create(this, callfunc_selector(SetAccountView::onSendNewAccount))));
}

void SetAccountView::onBidWarOKNewAccount()
{
    WAR_NUM = 2;
    PopupViewController::getInstance()->addPopupView(WarningView::create(_lang("105262"), _lang("105237"), CCCallFunc::create(this, callfunc_selector(SetAccountView::onSendNewAccount))));
}

void SetAccountView::onSendNewAccount()
{
    //make a new account  发送
    bool isOld = 1;
    if (SERCER_TYPE == 1) {
        isOld = 1;
    }
    else {
        isOld = 0;
    }
    
    NewACCCommand* cmd = new NewACCCommand(isOld, WAR_NUM);
    cmd->sendAndRelease();
    if(WAR_NUM == 2) {
        PopupViewController::getInstance()->removePopupView(this);
    }
    LuaController::getInstance()->clearChatNotice();
}

/////
WarningView* WarningView::create(string title, string info, CCCallFunc* yesfunc) {
    auto ret = new WarningView();
    if (ret && ret->init(title, info, yesfunc)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool WarningView::init(string title, string info, CCCallFunc* yesfunc) {
    bool ret = false;
    if (PopupBaseView::init()) {
        setIsHDPanel(true);
        CCBLoadFile("WarningView",this,this);
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        
        m_titleLabel->setString(title);
        m_infoLabel->setString(info);
        setYesCallback(yesfunc);
        
        CCCommonUtils::setButtonTitle(m_okBtn, _lang("105260").c_str());
        CCCommonUtils::setButtonTitle(m_cancelBtn, _lang("108532").c_str());
        m_yesCnt = 0;
        ret = true;
    }
    return ret;
}

void WarningView::onEnter() {
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void WarningView::onExit() {
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

void WarningView::onOKClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_onYes) {
        m_onYes->execute();
    }
    PopupViewController::getInstance()->removePopupView(this);
    
    return;
    
    m_yesCnt++;
    if(m_yesCnt == 1){
        m_titleLabel->setString(_lang("105262"));
    }
    if (m_yesCnt>=2) {
        if (m_onYes) {
            m_onYes->execute();
            PopupViewController::getInstance()->removePopupView(this);
        }
    }
}

void WarningView::onCancelClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->removePopupView(this);
}

SEL_CCControlHandler WarningView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOKClick", WarningView::onOKClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCancelClick", WarningView::onCancelClick);
    return NULL;
}

bool WarningView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel", CCLabelIF*, m_infoLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_okBtn", CCControlButton*, m_okBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cancelBtn", CCControlButton*, m_cancelBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    return false;
}

bool WarningView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void WarningView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_touchNode, pTouch)) {
        return;
    }
    PopupViewController::getInstance()->removePopupView(this);
}


/////
SelServerView* SelServerView::create(CCCallFunc* yesfunc) {
    auto ret = new SelServerView();
    if (ret && ret->init(yesfunc)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool SelServerView::init(CCCallFunc* yesfunc) {
    bool ret = false;
    if (PopupBaseView::init()) {
        CCBLoadFile("SelServerView",this,this);
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        
        setYesCallback(yesfunc);
        m_infoLabel->setString(_lang("105213"));
        CCCommonUtils::setButtonTitle(m_oldBtn, _lang("105211").c_str());
        CCCommonUtils::setButtonTitle(m_newBtn, _lang("105215").c_str());
        ret = true;
    }
    return ret;
}

void SelServerView::onEnter() {
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void SelServerView::onExit() {
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

void SelServerView::onOldClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    SERCER_TYPE = 1;
    PopupViewController::getInstance()->removePopupView(this);
    if (m_onYes) {
        m_onYes->execute();
    }
}

void SelServerView::onNewClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    SERCER_TYPE = 2;
    PopupViewController::getInstance()->removePopupView(this);
    if (m_onYes) {
        m_onYes->execute();
    }
}

SEL_CCControlHandler SelServerView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOldClick", SelServerView::onOldClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onNewClick", SelServerView::onNewClick);
    return NULL;
}

bool SelServerView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel", CCLabelIF*, m_infoLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_oldBtn", CCControlButton*, m_oldBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newBtn", CCControlButton*, m_newBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    return false;
}

bool SelServerView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void SelServerView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_touchNode, pTouch)) {
        return;
    }
    PopupViewController::getInstance()->removePopupView(this);
}

///SetAccountNextView
SetAccountNextView* SetAccountNextView::create(int type){
    SetAccountNextView* ret = new SetAccountNextView();
    if(ret && ret->init(type)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool SetAccountNextView::init(int type){
    if(!PopupBaseView::init()){
        return false;
    }
    setIsHDPanel(true);
    //m_type 1 绑定账号   2 切换账号
    m_type = type;
    auto bg = CCBLoadFile("SetAccountNext", this, this);
    m_waitInterface = NULL;
    
    string gpIconPath = "google_icon.png";
    if (m_type==1) {
        setTitleName(_lang("105258"));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        m_description->setString(_lang("113902"));
        gpIconPath = "icon_game-center2.png";
#else
        m_description->setString(_lang("105289"));
#endif
    }
    else if (m_type==2) {
        setTitleName(_lang("105282"));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        m_description->setString(_lang("113903"));
        gpIconPath = "icon_game-center2.png";
#else
        m_description->setString(_lang("105252"));
#endif
    }
    
    auto gpIcon = CCLoadSprite::createSprite(gpIconPath.c_str());
    gpIcon->setScale(0.8);
    m_gpIconNode->addChild(gpIcon);
    
    float preh =m_viewBg->getContentSize().height;
    changeBGHeight(m_viewBg);
    float dh = m_viewBg->getContentSize().height-preh;
    this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    if (CCCommonUtils::isIosAndroidPad())
    {
        dh = CCDirector::sharedDirector()->getWinSize().height - 2048;
        dh = dh / 2.4;
    }
    if(dh>0){
        this->m_Listnode->setPositionY(this->m_Listnode->getPositionY()+dh/2);
    }
    
    if(GlobalData::shared()->analyticID != "market_global" && GlobalData::shared()->analyticID != "AppStore"){
        m_description->setVisible(false);
        m_gNode->setVisible(false);
    }else{
        m_description->setVisible(true);
        m_gNode->setVisible(true);
    }
    
    int gapY = 137;
    int startY = m_gNode->getPositionY();
    if(m_gNode->isVisible()){
        startY += gapY;
        if(m_facebookBtn->isVisible() || m_facebookLoginBtn->isVisible()){
            startY += gapY;
        }
    }else if(m_facebookBtn->isVisible() || m_facebookLoginBtn->isVisible()){
        startY += 2 * gapY;
    }

    m_otherNode = CCNode::create();
    m_Listnode->addChild(m_otherNode);
    m_otherNode->setPosition(m_gNode->getPositionX(), startY);
    startY = 0;
    //#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    //添加其他平台绑定
    for (auto it = GlobalData::shared()->playerInfo.m_platFormBindInfo.begin(); it != GlobalData::shared()->playerInfo.m_platFormBindInfo.end(); it++) {
        if(it->second.bindPf != ""){
            SettingAccountButton *btn = SettingAccountButton::create(it->first, m_type);
            if(btn){
                m_otherNode->addChild(btn);
                btn->setPositionY(startY);
                startY -= gapY;
            }
        }
    }
    //#endif
    this->updateButtonState(NULL);
    for (int i=1; i<=4; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
        m_fireNode1->addChild(particle);
        
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
        m_fireNode2->addChild(particle1);
    }
    
    return true;
}

void SetAccountNextView::onEnter(){
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SetAccountNextView::FacebookCallback), MSG_FBLoginSucess, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SetAccountNextView::UserBindCancel), MSG_USER_BIND_CANCEL, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SetAccountNextView::updateButtonState), MSG_USER_BIND_OK, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SetAccountNextView::onResponsed_3rdPlatform), MSG_RESPONSED_3RD_PLATFORM, NULL);
}

void SetAccountNextView::onExit(){
    GlobalData::shared()->isBind = false;
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FBLoginSucess);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_USER_BIND_CANCEL);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_USER_BIND_OK);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_RESPONSED_3RD_PLATFORM);
    CCNode::onExit();
}

bool SetAccountNextView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg", CCScale9Sprite*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_facebookBtn", CCControlButton*, this->m_facebookBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_googlePlayBtn", CCControlButton*, this->m_googlePlayBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_facebookLoginBtn", CCControlButton*, this->m_facebookLoginBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_googlePlayLoginBtn", CCControlButton*, this->m_googlePlayLoginBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_facebookLoginTitle1", CCLabelIFTTF*, this->m_facebookLoginTitle1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_googleLoginTitle1", CCLabelIFTTF*, this->m_googleLoginTitle1);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_gpIconNode", CCNode*, this->m_gpIconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode1", CCNode*, this->m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode2", CCNode*, this->m_fireNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_Listnode", CCNode*, this->m_Listnode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_description", CCLabelIF*, this->m_description);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_googleFlagspr", CCSprite*, this->m_googleFlagspr);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_gNode", CCNode*, this->m_gNode);
    return true;
}

SEL_CCControlHandler SetAccountNextView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFacebookClick", SetAccountNextView::onFacebookClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGooglePlayClick", SetAccountNextView::onGooglePlayClick);
    return NULL;
}

void SetAccountNextView::onFacebookClick(cocos2d::CCObject *pSender, CCControlEvent event)
{
    string localCus = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
    if (m_type==1)
    {//绑定账号
        if (localCus != "")
        {//已经绑定fb  提示是否解绑
            PopupViewController::getInstance()->addPopupView(WarningView::create(_lang("105261"), _lang("105250"), CCCallFunc::create(this, callfunc_selector(SetAccountNextView::DisconnectFacebook))));
        }else
        {
            OpenLoginFB();
        }
    }
    else if (m_type==2)
    {//切换账号
        string gpUid = CCUserDefault::sharedUserDefault()->getStringForKey(GP_USERID,"");
        if (localCus != "" || gpUid!="")
        {//已经绑定fb  直接显示登录fb
            OpenLoginFB();
        }else
        {//当前账号未绑定 提示警告
            PopupViewController::getInstance()->addPopupView(WarningView::create(_lang("105261"), _lang("105277"), CCCallFunc::create(this, callfunc_selector(SetAccountNextView::OpenLoginFB))));
        }
    }
    return;
}

void SetAccountNextView::DisconnectFacebook(){
    GameController::getInstance()->showWaitInterface();
    UserBindCancelCommand* cmd = new UserBindCancelCommand(CC_ITOA(FACEBOOK));
    cmd->sendAndRelease();
}
void SetAccountNextView::DisconnectGooglePlay(){
    GameController::getInstance()->showWaitInterface();
    UserBindCancelCommand* cmd = new UserBindCancelCommand(CC_ITOA(GOOGLEPLAY));
    cmd->sendAndRelease();
}
void SetAccountNextView::DisconnectIOS(){
    GameController::getInstance()->showWaitInterface();
    UserBindCancelCommand* cmd = new UserBindCancelCommand(CC_ITOA(APPSTORE));
    cmd->sendAndRelease();
}

void SetAccountNextView::OpenLoginFB()
{
    if (FBUtilies::fbIsLogin())
    {
        FBUtilies::fbLogout();
    }
    GlobalData::shared()->isBind = true;
    bool flag = FBUtilies::fbLogin();
    if(!flag){
        GlobalData::shared()->isBind = false;
        CCCommonUtils::flyHint("", "", _lang("107080"));
    }
}

void SetAccountNextView::FacebookCallback(CCObject* params)
{
    GlobalData::shared()->isBind = false;
    CCLOG("2-2");
    m_facebookUid =CCUserDefault::sharedUserDefault()->getStringForKey("tmpFaceBook_uid", "");
    if (m_facebookUid == "") {
        return;
    }
    CCLOG("2-3 = %s",m_facebookUid.c_str());
    loginFacebookAction();
    return;
}

void SetAccountNextView::loginFacebookAction(){
    if(m_facebookUid==""){
        return;
    }
    CCLOG("3");
    string preuid = CCUserDefault::sharedUserDefault()->getStringForKey("tmpFaceBook_uid","");
    CCLOG("4 = %s",preuid.c_str());
    CCLOG("6 = %s",m_facebookUid.c_str());
    
    m_optType=m_type;
//    GameController::getInstance()->showWaitInterface();
    CCLOG("7");
    m_facebookName = CCUserDefault::sharedUserDefault()->getStringForKey("tmpFaceBook_Name","");
    CCLOG("8 = %s",m_facebookName.c_str());
    UserBindCommand* cmd = new UserBindCommand("",m_facebookUid,"","","",m_optType,"","",m_facebookName,"");
    cmd->sendAndRelease();
}


void SetAccountNextView::loginFacebook(){
    YesNoDialog::show(_lang("105277").c_str(), CCCallFunc::create(this, callfunc_selector(SetAccountNextView::loginFacebookAction1)),0,false,CCCallFunc::create(this, callfunc_selector(SetAccountNextView::loginCancel)));
}
void SetAccountNextView::loginFacebookAction1(){
    m_optType = 2;
    string localCus = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
    if (FBUtilies::fbIsLogin())
    {
        FBUtilies::fbLogout();
    }
    GlobalData::shared()->isBind = true;
    bool flag = FBUtilies::fbLogin();
    if(!flag){
        GlobalData::shared()->isBind = false;
        CCCommonUtils::flyHint("", "", _lang("107080"));
    }
}

void SetAccountNextView::logingoogle(){
    YesNoDialog::show(_lang("105277").c_str(), CCCallFunc::create(this, callfunc_selector(SetAccountNextView::logingoogleAction1)),0,false,CCCallFunc::create(this, callfunc_selector(SetAccountNextView::loginCancel)));
}
void SetAccountNextView::logingoogleAction1(){
    m_optType = 2;
    string localCus = CCUserDefault::sharedUserDefault()->getStringForKey(GP_USERID,"");
    if(cocos2d::extension::CCDevice::GooglePlusIsLogin()){
        cocos2d::extension::CCDevice::GooglePlusLogout();
    }
}

void SetAccountNextView::loginCancel(){
    m_optType = 0;
}


//void SetAccountNextView::onFeedClick(cocos2d::CCObject *pSender, CCControlEvent event){
//    if (FBUtilies::fbIsLogin())
//    {
//        CCLOG("发送feed");
//        FBUtilies::fbPublishFeedDialog("龙族部落","天网恢恢，肥而不腻！", "这只是个测试，哦！~"
//                                       ,"https://play.google.com/store/apps/details?id=air.com.elextech.happyfarm","http://pocket.eleximg.com/latest/public/assets/feed_trade.png");
//    }
//    else
//    {
//        //先登录，登陆成功返回后 调用feed
//        //FBUtilies::fbLogin();
//    }
//}

void SetAccountNextView::OpenLoginGP()
{
    GlobalData::shared()->isBind = true;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(cocos2d::extension::CCDevice::isConnected_Platform(0)){
        cocos2d::extension::CCDevice::logout_Platform(0);
    }
    cocos2d::extension::CCDevice::login_Platform(0);
#else
    //调用登陆GameCenter
    GameController::getInstance()->showWaitInterface();
    PlatformUtilies::login_Platform(0);
#endif
}

void SetAccountNextView::onGooglePlayClick(cocos2d::CCObject *pSender, CCControlEvent event)
{
    string localCus = CCUserDefault::sharedUserDefault()->getStringForKey(GP_USERID,"");
    if (m_type==1)
    {//绑定账号
        if (localCus != "")
        {//已经绑定gp  提示是否解绑
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            PopupViewController::getInstance()->addPopupView(WarningView::create(_lang("105261"), _lang("105250"), CCCallFunc::create(this, callfunc_selector(SetAccountNextView::DisconnectIOS))));
#else
            PopupViewController::getInstance()->addPopupView(WarningView::create(_lang("105261"), _lang("105250"), CCCallFunc::create(this, callfunc_selector(SetAccountNextView::DisconnectGooglePlay))));
#endif
        }else
        {
            OpenLoginGP();
        }
    }
    else if (m_type==2)
    {//切换账号
        string fbUid = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
        if (localCus != "" || fbUid!="")
        {//已经绑定gp  直接显示登录gp
            OpenLoginGP();
        }else
        {//当前账号未绑定 提示警告
            PopupViewController::getInstance()->addPopupView(WarningView::create(_lang("105261"), _lang("105277"), CCCallFunc::create(this, callfunc_selector(SetAccountNextView::OpenLoginGP))));
        }
    }
    return;
}

void SetAccountNextView::GooglePlayCallback(CCObject* params)
{
    GlobalData::shared()->isBind = false;
    m_googleUid =dynamic_cast<CCString*>(params)->getCString();
    string localCus = CCUserDefault::sharedUserDefault()->getStringForKey(GP_USERID,"");
    logingoogleAction();
}

void SetAccountNextView::logingoogleAction(){
    if(m_googleUid==""){
        return;
    }
    m_optType = m_type;
    m_googleName = CCUserDefault::sharedUserDefault()->getStringForKey("tmpGooglePlayName", "");

    CCUserDefault::sharedUserDefault()->flush();
    GameController::getInstance()->showWaitInterface();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    UserBindCommand* cmd = new UserBindCommand(m_googleUid,"","","","",m_optType,"","","","");
    cmd->sendAndRelease();
#else
    UserBindCommand* cmd = new UserBindCommand(m_googleUid,"","","","",m_optType,"",m_googleName,"","");
    cmd->sendAndRelease();
#endif
}

void SetAccountNextView::loginOtherAction(float delta){
    if(!m_dict){
        return;
    }
    CCLog("in post command");
    std::string bindPf = m_dict->valueForKey("platform")->getCString();
    std::string bindId = m_dict->valueForKey("id")->getCString();
    std::string bindName = m_dict->valueForKey("name")->getCString();
    GameController::getInstance()->showWaitInterface();
    m_optType = m_type;
    
    UserBindCommand* cmd = new UserBindCommand("","","","","",m_optType,"","","","",bindPf, bindId, bindName);
    cmd->sendAndRelease();
    m_dict = NULL;
}

void SetAccountNextView::updateButtonState(CCObject* p)
{
    m_facebookLoginBtn->setVisible(true);
    m_facebookBtn->setVisible(false);
    m_googlePlayLoginBtn->setVisible(true);
    m_googlePlayBtn->setVisible(false);
    
    if (m_type==1)
    {
        //显示账号绑定
        string fbTitle = _lang("105269")+" "+_lang("105231");
        m_facebookLoginTitle1->setString(fbTitle.c_str());
        
        string _fbUid = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
        if (_fbUid != "")
        {//显示账号解绑
            m_facebookLoginBtn->setVisible(false);
            m_facebookBtn->setVisible(true);
            string fbTitle = _lang("105269")+" "+_lang("105232");
            m_facebookLoginTitle1->setString(fbTitle.c_str());
        }
        else {
            CCCommonUtils::setButtonSprite(m_facebookLoginBtn, "btn_green3.png");
        }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        string gpTitle = _lang("113900")+" "+_lang("105231");
        m_googleLoginTitle1->setString(gpTitle.c_str());
        
        string _gpUid = CCUserDefault::sharedUserDefault()->getStringForKey(GP_USERID,"");
        if (_gpUid != "")
        {//显示账号解绑
            m_googlePlayLoginBtn->setVisible(false);
            m_googlePlayBtn->setVisible(true);
            string gpTitle = _lang("113900")+" "+_lang("105232");
            m_googleLoginTitle1->setString(gpTitle.c_str());
        }
        else {
            CCCommonUtils::setButtonSprite(m_googlePlayLoginBtn, "btn_green3.png");
        }
#else
        string gpTitle = _lang("105270")+" "+_lang("105231");
        m_googleLoginTitle1->setString(gpTitle.c_str());
        
        string _gpUid = CCUserDefault::sharedUserDefault()->getStringForKey(GP_USERID,"");
        if (_gpUid != "")
        {//显示账号解绑
            m_googlePlayLoginBtn->setVisible(false);
            m_googlePlayBtn->setVisible(true);
            string gpTitle = _lang("105270")+" "+_lang("105232");
            m_googleLoginTitle1->setString(gpTitle.c_str());
        }
        else {
            CCCommonUtils::setButtonSprite(m_googlePlayLoginBtn, "btn_green3.png");
        }
#endif
    }
    else if (m_type==2)
    {//显示账号切换
        string fbTitle = _lang("105269")+" "+_lang("105253");
        m_facebookLoginTitle1->setString(fbTitle.c_str());
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        string gpTitle = _lang("113900")+" "+_lang("105253");
        m_googleLoginTitle1->setString(gpTitle.c_str());
#else
        string gpTitle = _lang("105270")+" "+_lang("105253");
        m_googleLoginTitle1->setString(gpTitle.c_str());
#endif
    }
    int index = 0;
    auto& children = m_otherNode->getChildren();
    for (auto child : children)
    {
        SettingAccountButton *btn = dynamic_cast<SettingAccountButton*>(child);
        if(btn){
            btn->refreshState(NULL);
        }
    }
}

void SetAccountNextView::UserBindCancel(cocos2d::CCObject *params)
{
    string type = dynamic_cast<CCString*>(params)->getCString();
    if (type == "fb") {
        if (FBUtilies::fbIsLogin())
        {
            FBUtilies::fbLogout();
        }
    }
    else if (type == "cu")
    {
        CCUserDefault::sharedUserDefault()->setStringForKey(CUSTOM_UID, "");
    }else if (type == "gp") {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(cocos2d::extension::CCDevice::isConnected_Platform(0)){
            cocos2d::extension::CCDevice::logout_Platform(0);
        }
#else
        //登出GameCenter
        PlatformUtilies::logout_Platform(APPSTORE);
#endif
    }else{
        
    }
    CCUserDefault::sharedUserDefault()->flush();
    this->updateButtonState(NULL);
}

/**
 *第三方平台登录消息响应
 */
void SetAccountNextView::onResponsed_3rdPlatform(CCObject *obj){
    CCString* jsonCCStr=dynamic_cast<CCString*>(obj);
    string jsonStr=jsonCCStr->getCString();
    
    CCLOG(CCString::createWithFormat("onResponsed3rdPlatform:%s",jsonStr.c_str())->getCString());
    
    //    Json::Value jsonObj;
    //    Json::Reader reader;
    //    reader.parse(jsonStr, jsonObj);
    
    Json *jsonObj = Json_create(jsonStr.c_str());
    string _msgId=Json_getString(jsonObj, "msgId", "");
    string _userName=Json_getString(jsonObj, "userName", "");
    string _userId=Json_getString(jsonObj, "userId", "");
    string _platform=Json_getString(jsonObj, "platform", "");
    CCLog("platform login %s, %s, %s, %s", _msgId.c_str(), _userName.c_str(), _userId.c_str(), _platform.c_str());
    Json_dispose(jsonObj);
    if (_msgId != ""){//有数据
        if (_msgId=="login_sucess_google") {
            //使用此用户名，绑定到游戏
            if (_userName != ""){//注意userName可以为空
                
                CCLOG(CCString::createWithFormat("login_sucess_google account:%s,%s m_operation_type:%d",_userName.c_str(),_userId.c_str(),1)->getCString());
                m_googleUid =_userId;
                
                //                string localCus = CCUserDefault::sharedUserDefault()->getStringForKey(GP_USERID,"");
                CCUserDefault::sharedUserDefault()->setStringForKey("tmpGooglePlayName",_userName);
                CCUserDefault::sharedUserDefault()->setStringForKey("tmpGooglePlay_uid", m_googleUid);
                logingoogleAction();
                
            }else{
                
                CCLOG("login_sucess_google: unkown");
                YesNoDialog::showConfirmCloseAllView("login failed google+","OK");
            }
//            AchievementController::getInstance()->postCompleteAchievementToGoogle();
        }else  if (_msgId=="login_failed_google") {
            
            CCLOG("login_failed_google..");
            //            AOEToast::show("login failed google+");
            
        }else  if (_msgId=="login_canceled_google") {
            
            CCLOG("login_failed_google..");
            //            AOEToast::show("Cancelled by the user");
            
        }else  if (_msgId=="logout_sucess_google") {
            
            CCLOG(CCString::createWithFormat("logout_sucess_google..")->getCString());
            
        }else if(_msgId=="login_sucess_gamecenter"){
            GameController::getInstance()->removeWaitInterface();
            //使用此用户名，绑定到游戏
            if (_userId != ""){ 
                CCLOG(CCString::createWithFormat("login_sucess_gamecenter account:%s m_operation_type:%d",_userName.c_str(),1)->getCString());
                m_googleUid =_userId;
                
                CCUserDefault::sharedUserDefault()->setStringForKey("tmpGooglePlayName",_userName);
                CCUserDefault::sharedUserDefault()->setStringForKey("tmpGooglePlay_uid", m_googleUid);
                logingoogleAction();
                AppLibHelper::updatePlatformScore(FunBuildController::getInstance()->getMainCityLv());
            }else{
                CCLOG("login_sucess_gamecenter: unkown");
            }
        }else if(_msgId=="login_failed_gamecenter"){
            GameController::getInstance()->removeWaitInterface();
            
//            YesNoDialog::showConfirmCloseAllView("login gamecenter error","OK");
            
        }else if(_msgId=="logout_sucess_gamecenter"){

        }else if(_msgId=="login_sucess_weibo" || _msgId=="login_sucess_other"){
            CCDictionary *dict = CCDictionary::create();
            dict->setObject(CCString::create(_platform), "platform");
            dict->setObject(CCString::create(_userId), "id");
            dict->setObject(CCString::create(_userName), "name");
            m_dict = dict;
            CCLog("post command");
            this->scheduleOnce(schedule_selector(SetAccountNextView::loginOtherAction), 1);
        }
    }
}

////

SettingAccountButton *SettingAccountButton::create(int bindType, int panelType){
    SettingAccountButton *ret = new SettingAccountButton(bindType, panelType);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void SettingAccountButton::refreshState(CCObject *obj){
    bool refreshFlag = true;
    if(obj){
        CCInteger *integer = dynamic_cast<CCInteger*>(obj);
        if(integer->getValue() == m_bindType){
            refreshFlag = true;
        }
    }
    if(refreshFlag){
        auto it = GlobalData::shared()->playerInfo.m_platFormBindInfo.find(m_bindType);
        if(it != GlobalData::shared()->playerInfo.m_platFormBindInfo.end()){
            auto &info = it->second;
            std::string bindBtnSprite = "btn_green3.png";
            std::string unBindBtnSprite = "mail_red_btn.png";
            std::string changeBtnSprite = "btn_Orange.png";
            std::string btnSpriteStr = bindBtnSprite;
            std::string btnStr = "";
            if(m_panelType == 2){
                btnSpriteStr = changeBtnSprite;
                if(m_bindType == WEIBO){
                    btnStr = _lang("113970");
                }else if(m_bindType == VK){
                    btnStr = _lang("114031");
                }
            }else{
                if(info.bindId != ""){
                    btnSpriteStr = unBindBtnSprite;
                    if(m_bindType == WEIBO){
                        btnStr = _lang("113972");
                    }else if(m_bindType == VK){
                        btnStr = _lang("114030");
                    }
                }else{
                    btnSpriteStr = bindBtnSprite;
                    if(m_bindType == WEIBO){
                        btnStr = _lang("113971");
                    }else if(m_bindType == VK){
                        btnStr = _lang("114029");
                    }
                }
            }
            CCCommonUtils::setButtonSprite(m_btn, btnSpriteStr.c_str());
            m_stateText->setString(btnStr);
        }
    }
}

void SettingAccountButton::onBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent){
    auto it = GlobalData::shared()->playerInfo.m_platFormBindInfo.find(m_bindType);
    if(it != GlobalData::shared()->playerInfo.m_platFormBindInfo.end()){
        auto &info = it->second;
        if(m_panelType == 2){
            onChangeBtnClick();
        }else{
            if(info.bindId != ""){
                onUnBindBtnClick();
            }else{
                onBindBtnClick();
            }
        }
    }
}

void SettingAccountButton::login(){
    GlobalData::shared()->isBind = true;
    auto &info = GlobalData::shared()->playerInfo.m_platFormBindInfo[m_bindType];
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "com/clash/of/jni/Jni", "loginPlatform", "(Ljava/lang/String;)V") ) {
        CCLOG("JNI: pay google method onConsumeCallback not found!");
        return;
    }
    jstring weibo = minfo.env->NewStringUTF(info.bindPf.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, weibo);
    minfo.env->DeleteLocalRef(weibo);
    minfo.env->DeleteLocalRef(minfo.classID);
#else
    PlatformUtilies::loginPlatform(info.bindPf);
#endif
}

void SettingAccountButton::logout(CCObject *obj){
    GlobalData::shared()->isBind = true;
    CCString *str = dynamic_cast<CCString*>(obj);
    if(!str){
        return;
    }
    auto &info = GlobalData::shared()->playerInfo.m_platFormBindInfo[m_bindType];
    if(info.bindPf != str->getCString()){
        return;
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "com/clash/of/jni/Jni", "logoutPlatform", "(Ljava/lang/String;)V") ) {
        CCLOG("JNI: pay google method onConsumeCallback not found!");
        return;
    }
    jstring weibo = minfo.env->NewStringUTF(info.bindPf.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, weibo);
    minfo.env->DeleteLocalRef(weibo);
    minfo.env->DeleteLocalRef(minfo.classID);
#else
    PlatformUtilies::logoutPlatform(info.bindPf);
#endif
}

void SettingAccountButton::onConfirmUnBind(){
    UserBindCancelCommand* cmd = new UserBindCancelCommand(CC_ITOA(m_bindType));
    cmd->sendAndRelease();
}

void SettingAccountButton::onBindBtnClick(){
    login();
}

void SettingAccountButton::onUnBindBtnClick(){
    PopupViewController::getInstance()->addPopupView(WarningView::create(_lang("105261"), _lang("105250"), CCCallFunc::create(this, callfunc_selector(SettingAccountButton::onConfirmUnBind))));
}

void SettingAccountButton::onChangeBtnClick(){
    string localCus = CCUserDefault::sharedUserDefault()->getStringForKey(GP_USERID,"");
    string fbUid = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
    bool isAccountBind = (localCus != "" || fbUid != "");
    for (auto it = GlobalData::shared()->playerInfo.m_platFormBindInfo.begin(); it != GlobalData::shared()->playerInfo.m_platFormBindInfo.end(); it++) {
        if(it->second.bindPf != "" && it->second.bindId != ""){
            isAccountBind = true;
        }
    }
    if(isAccountBind){
        login();
    }else{
        PopupViewController::getInstance()->addPopupView(WarningView::create(_lang("105261"), _lang("105277"), CCCallFunc::create(this, callfunc_selector(SettingAccountButton::login))));
    }
}

bool SettingAccountButton::init(){
    auto it = GlobalData::shared()->playerInfo.m_platFormBindInfo.find(m_bindType);
    if(it == GlobalData::shared()->playerInfo.m_platFormBindInfo.end()){
        return false;
    }
    if(it->second.bindPf == ""){
        return false;
    }
    std::string bindBtnSprite = "btn_green3.png";
    m_btn = CCControlButton::create(CCLoadSprite::createScale9Sprite(bindBtnSprite.c_str()));
    m_btn->setAnchorPoint(ccp(0.5, 0.5));
    m_btn->setPreferredSize(CCSize(390, 88));
    m_btn->addTargetWithActionForControlEvents(this, cccontrol_selector(SettingAccountButton::onBtnClick), CCControlEventTouchUpInside);
    this->addChild(m_btn);
    
    auto size = m_btn->getContentSize();
    
    std::string iconStr = getIconStr();
    m_icon = CCLoadSprite::createSprite(iconStr.c_str());
    this->addChild(m_icon);
    auto iconSize = m_icon->getContentSize();
    m_icon->setPositionX(-135);
    CCCommonUtils::setSpriteMaxSize(m_icon, 52);
    m_stateText = CCLabelIF::create();
    this->addChild(m_stateText);
    m_stateText->setPositionX(33.5);
    
    refreshState(NULL);
    return true;
}

void SettingAccountButton::onEnter(){
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SettingAccountButton::logout), MSG_USER_BIND_CANCEL, NULL);
}

void SettingAccountButton::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_USER_BIND_CANCEL);
    CCNode::onExit();
}

std::string SettingAccountButton::getIconStr(){
    if(m_bindType == WEIBO){
        return "weibo.png";
    }else if(m_bindType == VK){
        return "icon_vk_general.png";
    }
    return "";
}