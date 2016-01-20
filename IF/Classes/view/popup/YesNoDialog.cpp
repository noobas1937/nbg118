//
//  YesNoDialog.cpp
//  IF
//
//  Created by zhaohf on 13-9-04.
//
//

#include "CommonInclude.h"
#include "YesNoDialog.h"
#include "PopupViewController.h"
#include "CCLabelIF.h"
#include "GoldExchangeView.h"
#include "AllianceInfoView.h"
#include "ChatServiceCocos2dx.h"
#include "ChatController.h"
#include "LuaController.h"
#include "GoldExchangeAdvertisingView.h"
#include "GoldExchangeItemView.h"
#include "CCClipNode.h"
#include "ButtonLightEffect.h"

#include "GuideController.h"

string YesNoDialog::m_dialogStr="";
string YesNoDialog::m_buttonName="";
CCCallFunc* YesNoDialog::m_callBackFunc=NULL;
bool YesNoDialog::m_isCloseCallback=false;
YesNoDialog* YesNoDialog::_yesNoDialogInstance=NULL;


YesNoDialog::~YesNoDialog(void)
{
    CCLOGTIME();
//    if (m_onClose)
//        m_onClose->execute();
}

YesNoDialog::YesNoDialog()
:isCloseOnNoButton(true)
,disableWhenTouched(false)
,m_function(NULL)
,mTimeDes("")
{
    CCLOGTIME();
}

YesNoDialog* YesNoDialog::getInstance()
{
    if (_yesNoDialogInstance==NULL) {
        _yesNoDialogInstance=new YesNoDialog();
    }
    return _yesNoDialogInstance;
}

void YesNoDialog::onEnter()
{
    CCNode::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(YesNoDialog::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
    if (getParent() && (getParent()->getChildByTag(BUILDINGVIEW_TAG) || getParent()->getChildByTag(YESNODLG_TAG))){
//        m_layerColorBG->setOpacity(0);
    }
    else{
        setTag(YESNODLG_TAG);
    }
}

void YesNoDialog::doEnter()
{
//    m_layerColorBG->setOpacity(m_opacity);
}

void YesNoDialog::onExit()
{
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(YesNoDialog::onEnterFrame), this);
    setTouchEnabled(false);
    CCNode::onExit();
}

bool YesNoDialog::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_muteTimeNode->isVisible() && (isTouchInside(m_box1,pTouch) || isTouchInside(m_box2,pTouch) || isTouchInside(m_box3,pTouch) || isTouchInside(m_box4,pTouch)))
    {
        return true;
    }
    if (isTouchInside(m_sprBG, pTouch)) {
        return false;
    }
    keyPressedBtnClose(NULL,Control::EventType::TOUCH_DOWN);
    return true;
}

void YesNoDialog::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(!isTouchInside(m_sprBG,pTouch)){
        PopupViewController::getInstance()->removePopupView(this);
        return ;
    }
    if(!m_muteTimeNode->isVisible())
    {
        return;
    }
    if (isTouchInside(m_box1,pTouch)) {
        m_selectSpr->setPosition(ccp(-207,164));
        m_index = 0;
    }else if(isTouchInside(m_box2,pTouch)){
        m_selectSpr->setPosition(ccp(60,161));
        m_index = 1;
    }else if(isTouchInside(m_box3,pTouch)){
        m_selectSpr->setPosition(ccp(-207,98));
        m_index = 2;
    }else if(isTouchInside(m_box4,pTouch)){
        m_selectSpr->setPosition(ccp(60,100));
        m_index = 3;
    }
    
    ChatController::getInstance()->setBanTime(getBannedTime(m_index));
}

int YesNoDialog::getBannedTime(int index)
{
    int bannedTime = 0;
    switch (index)
    {
        case 0:
            bannedTime = 3600;
            break;
        case 1:
            bannedTime = 3600*2;
            break;
        case 2:
            bannedTime = 3600*3;
            break;
        case 3:
            bannedTime = 3600*4;
            break;
        default:
            break;
    }
    return bannedTime;
}

void YesNoDialog::gotoPayTips() {
//    auto tip = new YesNoDialog();
//    if(GlobalData::shared()->analyticID == "common"){
//        tip->init(_lang("E100001").c_str(),_lang("confirm").c_str());
//    }else{
//        tip->init(_lang("104912").c_str(),_lang("101221").c_str());
//        tip->setYesCallback(CCCallFunc::create(tip, callfunc_selector(YesNoDialog::gotoPayCallback)));
//    }
//    PopupViewController::getInstance()->addPopupView(tip,false);
//    tip->release();
    auto tip = ConfirmCoinView::create();
    PopupViewController::getInstance()->addPopupView(tip,false);
}

void YesNoDialog::gotoPayCallback() {
//    PopupViewController::getInstance()->addPopupView(GoldExchangeView::create(),false);
}

YesNoDialog* YesNoDialog::showTime(const char * content,CCCallFunc* yesfunc,int time, const char * btnMsg, bool isBuild, int sumTime, int sumGold)
{
    CCLOG("--------Show YesNoDialog---------");
    YesNoDialog* dialog = new YesNoDialog();
    
    if(1){
        dialog->init(content,btnMsg);
        dialog->setYesCallback(yesfunc);
        dialog->m_goldIcon->setVisible(true);
        dialog->m_goldNum->setVisible(true);
        dialog->m_btnText->setVisible(true);
        dialog->m_time = time;
        dialog->m_isBuild = isBuild;
        dialog->m_sumGold = sumGold;
        dialog->m_sumTime = sumTime;
        dialog->onEnterFrame(0);
        CCCommonUtils::setButtonTitle(dialog->m_btnOk, "");
    }
    
    PopupViewController::getInstance()->addPopupView(dialog, false);
    CC_SAFE_RELEASE(dialog);
    return dialog;
}

YesNoDialog* YesNoDialog::showResCost(const char * content,CCCallFunc* yesfunc, const char * btnMsg, bool isBuild, int sumCost,int resType)
{
    CCLOG("--------Show YesNoDialog---------");
    YesNoDialog* dialog = new YesNoDialog();
    
    dialog->init(content,btnMsg);
    dialog->setYesCallback(yesfunc);
    dialog->m_goldIcon->setVisible(false);
    if (sumCost>0) {
        dialog->m_goldNum->setVisible(true);
        dialog->m_goldNum->setString(CC_ITOA(sumCost));
    }
    dialog->m_btnText->setVisible(true);
    dialog->m_isBuild = isBuild;
    dialog->onEnterFrame(0);
    CCCommonUtils::setButtonTitle(dialog->m_btnOk, "");
    std::string resPicStr = "ui_gold.png";
    switch (resType) {
        case Silver:
            resPicStr = "ui_steel.png";
            break;
        case Wood:
            resPicStr = "ui_wood.png";
            break;
        case Iron:
            resPicStr = "ui_iron.png";
            break;
        case Food:
            resPicStr = "ui_food.png";
            break;
        case Stone:
            resPicStr = "ui_stone.png";
            break;
        case Gold:
            resPicStr = "ui_gold.png";
            break;
        default:
            break;
    }
    auto resPic = CCLoadSprite::createSprite(resPicStr.c_str(),true,CCLoadSpriteType_DEFAULT);
    if (resPic && sumCost>0) {
        CCCommonUtils::setSpriteMaxSize(resPic,40);
        dialog->m_resNode->addChild(resPic);
    }
    
    PopupViewController::getInstance()->addPopupView(dialog, false);
    CC_SAFE_RELEASE(dialog);
    return dialog;
}

YesNoDialog* YesNoDialog::showTimeWithDes(const char * content,const char* timeDes,int time){
    YesNoDialog* dialog = new YesNoDialog();
    dialog->init(content,_lang("confirm").c_str());
    dialog->mTimeDes = timeDes;
    dialog->m_btnText->setVisible(true);
    dialog->m_time = time;
    dialog->onEnterFrame1(0);
    CCCommonUtils::setButtonTitle(dialog->m_btnOk, "");
    PopupViewController::getInstance()->addPopupView(dialog, false);
    CC_SAFE_RELEASE(dialog);
    return dialog;
}
YesNoDialog* YesNoDialog::showQueueConfirm(const char * content,CCCallFunc* yesfunc,const char* t1,const char* t2,int time1,int time2,const char * buttonName,int gold){
    YesNoDialog* dialog = new YesNoDialog();
    if(1){
        std::string showTip = content + std::string("\n") + t1 + " " + CC_SECTOA(time1) + "\n" + std::string(t2) +" " + CC_SECTOA(time2);
        dialog->init(showTip.c_str());
        dialog->setYesCallback(yesfunc);
        dialog->m_goldIcon->setVisible(true);
        dialog->m_goldNum->setVisible(true);
        dialog->m_time = time1;
        dialog->m_btnText->setVisible(true);
        dialog->m_btnText->setString(buttonName);
        dialog->m_goldNum->setString(CC_ITOA(gold));
        dialog->mContents.push_back(content);
        dialog->mContents.push_back(std::string(t1));
        dialog->mContents.push_back(CC_SECTOA(time1));
        dialog->mContents.push_back(std::string(t2) +" " + CC_SECTOA(time2));
        dialog->onEnterFrame(0);
        CCCommonUtils::setButtonTitle(dialog->m_btnOk, "");
    }
    
    PopupViewController::getInstance()->addPopupView(dialog, false);
    CC_SAFE_RELEASE(dialog);
    return dialog;
}
YesNoDialog* YesNoDialog::show(const char *content,CCCallFunc*yesfunc,int gold,bool canShowMulti, CCCallFunc* nofunc, CCCallFunc* selfunc, const char * buttonStr, const char * iconName)
{
	CCLOG("--------Show YesNoDialog---------");
    YesNoDialog* dialog = new YesNoDialog();


    if(gold>0){
//        std::string popFlag = CCUserDefault::sharedUserDefault()->getStringForKey(GOLD_POP);
//        if(popFlag == "1"){
//            if(yesfunc){
//                yesfunc->execute();
//            }
//            dialog->release();
//            return NULL;
//        }
        dialog->init(content,"");
        dialog->setYesCallback(yesfunc);
        dialog->m_goldIcon->setVisible(true);
        dialog->m_goldNum->setVisible(true);
        dialog->m_btnText->setVisible(true);
        dialog->m_goldNum->setString(CC_ITOA(gold));
    }else{
        if (strcmp(buttonStr, "") != 0)
        {
            dialog->init(content,"");
            dialog->m_goldNum->setVisible(true);
            dialog->m_goldNum->setString(buttonStr);
        }
        else
        {
            dialog->init(content);
            dialog->m_goldNum->setVisible(false);
        }
        if (strcmp(iconName, "") != 0)
        {
            dialog->m_goldIcon->setVisible(true);
            dialog->m_goldIcon->setDisplayFrame(CCLoadSprite::loadResource(iconName));
            dialog->m_goldIcon->setScale(48.0f/dialog->m_goldIcon->getContentSize().height);
        }
        else
        {
            dialog->m_goldIcon->setVisible(false);
            dialog->m_goldIcon->setDisplayFrame(CCLoadSprite::loadResource("ui_gold.png"));
        }
        dialog->setYesCallback(yesfunc);
    }
    if (nofunc) {
        dialog->setNoCallback(nofunc);
    }
    if (selfunc) {
        dialog->m_noticeNode->setVisible(true);
        dialog->setSelCallback(selfunc);
        dialog->m_noticeSpr->setVisible(false);
        dialog->onClickNoticeBtn(NULL, Control::EventType::TOUCH_DOWN);
    }
    PopupViewController::getInstance()->addPopupView(dialog, false);
    if (dialog) dialog->release();
    return dialog;
}

YesNoDialog* YesNoDialog::marchAlertShow(const char *content,CCCallFunc*yesfunc,CCCallFunc*nofunc)
{
    CCLOG("--------Show YesNoDialog---------");
    YesNoDialog* dialog = new YesNoDialog();
    
    dialog->init(content);
    dialog->m_goldNum->setVisible(false);
    dialog->m_goldIcon->setVisible(false);
    dialog->m_goldIcon->setDisplayFrame(CCLoadSprite::loadResource("ui_gold.png"));
    CCCommonUtils::setButtonTitle(dialog->m_btnOk, _lang("3000000").c_str());
    dialog->m_btnCancel->setVisible(true);
    dialog->m_btnOk->setPositionX(dialog->m_btnOk->getPositionX() - 140);
    if (yesfunc) {
        dialog->setYesCallback(yesfunc);
    }
    
    if (nofunc) {
        dialog->setNoCallback(nofunc);
    }
    else
        dialog->setNoCallback(CCCallFunc::create(dialog, callfunc_selector(YesNoDialog::closeSelf)));
    
    PopupViewController::getInstance()->addPopupView(dialog, false);
    CC_SAFE_RELEASE(dialog);
    return dialog;
}

YesNoDialog* YesNoDialog::lotteryShow(const char * content,CCCallFunc* yesfunc,int gold,bool alignCenter, CCCallFunc* nofunc, CCCallFunc* selfunc, const char * buttonStr, const char * iconName)
{
    YesNoDialog* dialog = new YesNoDialog();
    
    dialog->init(content,"");
    dialog->m_goldNum->setVisible(true);
    dialog->m_goldNum->setString(buttonStr);
    dialog->m_goldNum->setPositionY(0.0f);
    if (alignCenter)
    {
        dialog->m_goldNum->setPositionX(0.0f);
        dialog->m_goldNum->setAnchorPoint(ccp(0.5, 0.5));
        dialog->m_goldNum->setHorizontalAlignment(kCCTextAlignmentCenter);
    }
    
    dialog->m_goldIcon->setVisible(true);
    dialog->m_goldIcon->setDisplayFrame(CCLoadSprite::loadResource(iconName));
    dialog->m_goldIcon->setScale(48.0f/dialog->m_goldIcon->getContentSize().height);
    dialog->m_goldIcon->setPositionY(0.0f);
    
    if (alignCenter)
    {
        CCSize size = dialog->m_goldNum->getContentSize();
        float offsetX = size.width*dialog->m_goldNum->getOriginScaleX();
        dialog->m_goldIcon->setPositionX(-offsetX/2.0f-24);
    }
    else
    {
        dialog->m_goldIcon->setPositionX(dialog->m_goldIcon->getPositionX() - 10.0f);
    }
    
    dialog->setYesCallback(yesfunc);
    
    if (nofunc) {
        dialog->setNoCallback(nofunc);
    }
    if (selfunc) {
        dialog->m_noticeNode->setVisible(true);
        dialog->setSelCallback(selfunc);
        dialog->m_noticeSpr->setVisible(false);
        dialog->onClickNoticeBtn(NULL, Control::EventType::TOUCH_DOWN);
    }
    PopupViewController::getInstance()->addPopupView(dialog, false);
    CC_SAFE_RELEASE(dialog);
    return dialog;
}

YesNoDialog* YesNoDialog::show(const char *content,function<void()> function,int gold)
{
	CCLOG("--------Show YesNoDialog---------");
    YesNoDialog* dialog = new YesNoDialog();
    
    if(gold>0){
        dialog->init(content,"");
        dialog->m_function = function;
        dialog->m_goldIcon->setVisible(true);
        dialog->m_goldNum->setVisible(true);
        dialog->m_btnText->setVisible(true);
        dialog->m_goldNum->setString(CC_ITOA(gold));
    }else{
        dialog->init(content);
        dialog->m_function = function;
    }
    
    dialog->init(content);
    dialog->m_function = function;

    PopupViewController::getInstance()->addPopupView(dialog, false);
    CC_SAFE_RELEASE(dialog);
    return dialog;
}

void YesNoDialog::showYesDialogResume(float time)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(YesNoDialog::showYesDialogResume), YesNoDialog::getInstance());
    if (m_dialogStr!="" && !ChatServiceCocos2dx::isChatShowing) {
        showYesDialog(m_dialogStr.c_str(),false,m_callBackFunc);
    }
    m_dialogStr="";
    m_callBackFunc=NULL;
#endif
}

YesNoDialog* YesNoDialog::showYesDialog(const char *content,bool canShowMulti,CCCallFunc * YesFunc,bool originConfirm)
{
	CCLOG("--------Show YesDialog---------");
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(ChatServiceCocos2dx::isChatShowing){
        if(!ChatServiceCocos2dx::isForumShowing){
            ChatServiceCocos2dx::exitChatActivityFrom2dx();
            m_dialogStr=content;
            m_callBackFunc=YesFunc;
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(YesNoDialog::showYesDialogResume), YesNoDialog::getInstance(), 0.0f, 1, 0.01f, false);
        }
        return NULL;
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (ChatServiceCocos2dx::isChatShowing_fun()) {
        ChatServiceCocos2dx::hiddenChatIosFrom2dx();
        m_dialogStr=content;
        m_callBackFunc=YesFunc;
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(YesNoDialog::showYesDialogResume), YesNoDialog::getInstance(), 0.0f, 1, 0.01f, false);
        return NULL;
    }
#endif
    
    
    
    
    YesNoDialog* dialog = new YesNoDialog();
//    if (originConfirm) {
//        dialog->init(content,"Confirm");
//    } else {
        dialog->init(content);
//    }
    dialog->hideNoButton();
    if(YesFunc){
        dialog->setYesCallback(YesFunc);
    }
    PopupViewController::getInstance()->addPopupView(dialog, false);
    CC_SAFE_RELEASE(dialog);
    return dialog;
}


void YesNoDialog::showVariableTitleResume(float time)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(YesNoDialog::showVariableTitleResume), YesNoDialog::getInstance());
    if (m_dialogStr!="" && !ChatServiceCocos2dx::isChatShowing) {
        showVariableTitle(m_dialogStr.c_str(),m_callBackFunc,m_buttonName.c_str(),m_isCloseCallback);
    }
    m_isCloseCallback=false;
    m_dialogStr="";
    m_buttonName="";
    m_callBackFunc=NULL;
#endif
}


YesNoDialog* YesNoDialog::showVariableTitle(const char * content, CCCallFunc * func,const char * buttonName,bool isCloseCallback, int timeJson){
    CCLOG("--------Show YesNoDialog---------");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(ChatServiceCocos2dx::isChatShowing){
        if(!ChatServiceCocos2dx::isForumShowing){
            ChatServiceCocos2dx::exitChatActivityFrom2dx();
            m_dialogStr=content;
            m_callBackFunc=func;
            m_isCloseCallback=isCloseCallback;
            m_buttonName=buttonName;
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(YesNoDialog::showVariableTitleResume), YesNoDialog::getInstance(), 0.0f, 1, 0.01f, false);
        }
        return NULL;
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (ChatServiceCocos2dx::isChatShowing_fun()) {
        ChatServiceCocos2dx::hiddenChatIosFrom2dx();
        m_dialogStr=content;
        m_callBackFunc=func;
        m_isCloseCallback=isCloseCallback;
        m_buttonName=buttonName;
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(YesNoDialog::showVariableTitleResume), YesNoDialog::getInstance(), 0.0f, 1, 0.01f, false);
    }
#endif
    
    
    YesNoDialog* dialog = new YesNoDialog();
    dialog->init(content,buttonName, timeJson);
    
    dialog->setYesCallback(func);
    if (isCloseCallback) {
        dialog->setCloseCallback(func);
    }
    PopupViewController::getInstance()->addPopupView(dialog, false);
    CC_SAFE_RELEASE(dialog);
    return dialog;
}

YesNoDialog* YesNoDialog::showButtonAndGold(const char * content,CCCallFunc * func,const char * buttonName,int gold,const char* desDialog){
    YesNoDialog* dialog = new YesNoDialog();
    dialog->init(content,"");
    dialog->setYesCallback(func);
    dialog->m_goldIcon->setVisible(true);
    dialog->m_goldNum->setVisible(true);
    dialog->m_btnText->setVisible(true);
    dialog->m_btnText->setString(buttonName);
    dialog->m_goldNum->setString(CC_ITOA(gold));
    if(!string(desDialog).empty()){
        dialog->m_timeLabel->setString(desDialog);
        dialog->m_timeLabel->setVisible(true);
    }
    
    PopupViewController::getInstance()->addPopupView(dialog, false);
    CC_SAFE_RELEASE(dialog);
    return dialog;
}

YesNoDialog* YesNoDialog::showButtonAndPriceType(const char * content,CCCallFunc * func,const char * buttonName,int type,int value){
    YesNoDialog* dialog = new YesNoDialog();
    dialog->init(content,"");
    dialog->setYesCallback(func);
    std::string iconStr = "";
    switch (type) {
        case COST_WOOD:
            iconStr = "ui_wood.png";
            break;
        case COST_STONE:
            iconStr = "ui_stone.png";
            break;
        case COST_IRON:
            iconStr = "ui_iron.png";
            break;
        case COST_FOOD:
            iconStr = "ui_food.png";
            break;
        case COST_MONEY:
            iconStr = "ui_money.png";
            break;
        case COST_GOLD:
            iconStr = "ui_gold.png";
            break;
        case COST_ALLIANCEHONOR:
            iconStr = "Contribution_icon1.png";
            break;
        case COST_ALLIANCECONTRIBUTION:
            iconStr = "Contribution_icon.png";
            break;
    }
    if(iconStr!=""){
        CCSprite* spr = CCLoadSprite::createSprite(iconStr.c_str());
        spr->setScale(0.7);
        spr->setPosition(dialog->m_goldIcon->getPosition());
        dialog->m_goldIcon->getParent()->addChild(spr);
        if(type==COST_ALLIANCEHONOR||type==COST_ALLIANCECONTRIBUTION){
            spr->setPositionX(spr->getPositionX()-20);
        }
    }
    dialog->m_goldIcon->setVisible(false);
    dialog->m_goldNum->setVisible(true);
    dialog->m_btnText->setVisible(true);
    dialog->m_btnText->setString(buttonName);
    dialog->m_goldNum->setString(CC_CMDITOA(value));
    PopupViewController::getInstance()->addPopupView(dialog, false);
    CC_SAFE_RELEASE(dialog);
    return dialog;
}

YesNoDialog* YesNoDialog::showAllianceConfirm(const char * content,const char * buttonName){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(ChatServiceCocos2dx::isChatShowing){
        string str(content);
        ChatServiceCocos2dx::flyHint("", "", str, 0.0f, 0.0f, false);
        return NULL;
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (ChatServiceCocos2dx::isChatShowing_fun()) {
        string str(content);
        ChatServiceCocos2dx::flyHint("", "", str, 0.0f, 0.0f, false);
        return NULL;
    }
#endif
    YesNoDialog* dialog = new YesNoDialog();
    dialog->init(content,buttonName);
    dialog->m_isAlliance = true;
    PopupViewController::getInstance()->addPopupView(dialog, false);
    CC_SAFE_RELEASE(dialog);
    return dialog;
}


void YesNoDialog::showConfirmCloseAllViewResume(float time)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(YesNoDialog::showConfirmCloseAllViewResume), YesNoDialog::getInstance());
    if (m_dialogStr!="" && !ChatServiceCocos2dx::isChatShowing) {
        showConfirmCloseAllView(m_dialogStr.c_str(),m_buttonName.c_str());
    }
    m_dialogStr="";
    m_buttonName="";
#endif
}

YesNoDialog* YesNoDialog::showConfirmCloseAllView(const char * content,const char * buttonName){
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(ChatServiceCocos2dx::isChatShowing){
        if(!ChatServiceCocos2dx::isForumShowing){
            ChatServiceCocos2dx::exitChatActivityFrom2dx();
            m_dialogStr=content;
            m_buttonName=buttonName;
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(YesNoDialog::showConfirmCloseAllViewResume), YesNoDialog::getInstance(), 0.0f, 1, 0.01f, false);
        }
        return NULL;
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (ChatServiceCocos2dx::isChatShowing_fun()) {
        ChatServiceCocos2dx::hiddenChatIosFrom2dx();
        m_dialogStr=content;
        m_buttonName=buttonName;
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(YesNoDialog::showConfirmCloseAllViewResume), YesNoDialog::getInstance(), 0.0f, 1, 0.01f, false);
    }
#endif
    
    
    YesNoDialog* dialog = new YesNoDialog();
    dialog->init(content,buttonName);
    dialog->m_closeAll = true;
    PopupViewController::getInstance()->addPopupView(dialog, false);
    CC_SAFE_RELEASE(dialog);
    return dialog;
}

YesNoDialog* YesNoDialog::showQuitDialog(cocos2d::CCCallFunc *func,bool forLoading/*=false*/){
    CCLOG("--------Show YesNoDialog  showQuitDialog---------");
    YesNoDialog* dialog = new YesNoDialog();
    dialog->init(_lang("dialog_message_exit_confirm").c_str());
    dialog->setYesCallback(func);
    PopupViewController::getInstance()->addPopupView(dialog, false);
    CC_SAFE_RELEASE(dialog);
    return dialog;
}

bool YesNoDialog::init(const char *content,const char * buttonName, int timeJson)
{
    if (!PopupBaseView::init()) {
        CCLOG("!PopupBaseView::init()");
        return false;
    }
    this->setIsHDPanel(true);
    CCNode* _rootNode = _rootNode = CCBLoadFile("YesNoDialog.ccbi", this, this);
    
    bool bRet=false;
    if (_rootNode) {
        mContents.clear();
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        //label可滑动，放在scrollview上
        m_contentLabel = CCLabelTTF::create();
        m_contentLabel->setDimensions(CCSize(360, 0));
        m_contentLabel->setString(content);
        m_contentLabel->setColor(ccWHITE);
        m_contentLabel->setFontSize(24);
        m_contentLabel->setVerticalAlignment(kCCVerticalTextAlignmentTop);
        m_contentLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
        m_contentLabel->setPosition(ccp(0, -10));
        int totalHeight = m_contentLabel->getContentSize().height ;/** m_contentLabel->getOriginScaleY();*/
        if(totalHeight > m_subLayer->getContentSize().height + 5){
//            if (CCCommonUtils::isIosAndroidPad() && CCCommonUtils::getIsHDViewPort()) {
//                m_contentLabel->setDimensions(CCSize(720, 0));
//                m_contentLabel->setFontSize(48);
//                CCScrollView* scroll = CCScrollView::create(CCSize(720, 340));
//                scroll->setContentSize(CCSize(720, m_contentLabel->getContentSize().height/**m_contentLabel->getOriginScaleY()*/));
//                
//                scroll->addChild(m_contentLabel);
//                m_subLayer->addChild(scroll);
//                float gapWidth = 720-m_contentLabel->getContentSize().width/**m_contentLabel->getOriginScaleX()*/;
//                scroll->setPosition(ccp(gapWidth/2, 10));
//                scroll->setDirection(kCCScrollViewDirectionVertical);
//                scroll->setAnchorPoint(ccp(0, 0));
//                scroll->setContentOffset(ccp(0, 340-m_contentLabel->getContentSize().height/**m_contentLabel->getOriginScaleY()*/));
//            }
//            else {
                CCScrollView* scroll = CCScrollView::create(CCSize(360, 170));
                scroll->setContentSize(CCSize(360, m_contentLabel->getContentSize().height/**m_contentLabel->getOriginScaleY()*/));
                
                scroll->addChild(m_contentLabel);
                m_subLayer->addChild(scroll);
                float gapWidth = 360-m_contentLabel->getContentSize().width/**m_contentLabel->getOriginScaleX()*/;
                scroll->setPosition(ccp(gapWidth/2, 5));
                scroll->setDirection(kCCScrollViewDirectionVertical);
                scroll->setAnchorPoint(ccp(0, 0));
                scroll->setContentOffset(ccp(0, 170-m_contentLabel->getContentSize().height/**m_contentLabel->getOriginScaleY()*/));
//            }
        }else{
            m_subNode->addChild(m_contentLabel);
            m_contentLabel->setAnchorPoint(ccp(0, 1));
            float gapWidth = 360-m_contentLabel->getContentSize().width/**m_contentLabel->getOriginScaleX()*/;
            m_contentLabel->setPosition(ccp(gapWidth/2, 0));
        }
        CCCommonUtils::setButtonTitle(m_btnOk, buttonName);

        this->m_btnText->setString(buttonName);

        CCCommonUtils::setButtonTitle(m_btnCancel, _lang("cancel_btn_label").c_str());
        this->m_goldIcon->setVisible(false);
        this->m_goldNum->setVisible(false);
        this->m_btnText->setVisible(false);
        this->m_btnCancel->setVisible(false);
        this->m_muteTimeNode->setVisible(false);
        bRet=true;
        
        // change btn priority to prevent touch swallowed by others (CCControl class must change it's DefaultTouchPriority)
        m_btnOk->setTouchPriority(-1);
        m_btnCancel->setTouchPriority(-1);
        
        m_noticeNode->setVisible(false);
        m_noticeLabel->setString(_lang("102109"));
        m_timeLabel->setString("");
        
        m_time=-1;
        m_isBuild=true;
        m_isAlliance = false;
        m_closeAll = false;
        serverUpdateInfo(content, timeJson);
    }
    return bRet;
}


void YesNoDialog::serverUpdateInfo(const char * content , int timeJson)
{
    if (timeJson > 0)
    {
        m_Endtime = timeJson;
        if (m_Endtime < 0 )
        {
            m_Endtime = 0;
        }
        
        std::string timeStr = CC_SECTOA(m_Endtime);
        if (CCCommonUtils::isIosAndroidPad() && CCCommonUtils::getIsHDViewPort()) {
            m_timeLabel->setFontSize(60);
        }
        else
            m_timeLabel->setFontSize(30);
        m_timeLabel->setString(timeStr);
        if (m_Endtime > 0)
        {
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(YesNoDialog::onUpdateLabel), this, 1.0,kCCRepeatForever, 0.0f, false);
        }
        
    }
}

void YesNoDialog::onUpdateLabel(float dt)
{
    m_Endtime--;
    if (m_Endtime <= 0)
    {
   		m_Endtime = 0;
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(YesNoDialog::onUpdateLabel), this);
    }

     std::string timeStr = CC_SECTOA(m_Endtime);
     m_timeLabel->setString(timeStr);
    

}

void YesNoDialog::hideNoButton()
{
    m_btnCancel->setVisible(false);
//    m_btnOk->setPositionX(m_subNode->getContentSize().width/2);
}

void YesNoDialog::showCancelButton(){
    this->m_btnCancel->setVisible(true);
    this->m_btnOk->setPositionX(-m_btnCancel->getPositionX());
}

void YesNoDialog::setYesButtonTitle(const char *content)
{
    CCCommonUtils::setButtonTitle(m_btnOk, content);
}

void YesNoDialog::setYesText(const char *content){
    m_btnText->setString(content);
}

void YesNoDialog::setNoButtonTitle(const char *content)
{
    CCCommonUtils::setButtonTitle(m_btnCancel, content);
}

void YesNoDialog::showMuteTimeNode()
{
    m_muteTimeNode->setVisible(true);
    m_txt1->setString("1"+_lang("105591"));
    m_txt2->setString("2"+_lang("105591"));
    m_txt3->setString("3"+_lang("105591"));
    m_txt4->setString("4"+_lang("105591"));
    m_selectSpr->setPosition(ccp(-207,164));
    m_index = 0;
}
void YesNoDialog::setUpAutoClose(){
//    m_bg1->setVisible(false);
//    m_bg2->setVisible(true);
    m_btnOk->setVisible(false);
    m_btnCancel->setVisible(false);
    scheduleOnce(schedule_selector(YesNoDialog::doAutoClose), 1.5);
}

void YesNoDialog::doAutoClose(float t)
{
    keypressedBtnOk(NULL,CCControlEventTouchUpInside);
}

void YesNoDialog::addToSubLayer(CCNode* node)
{
    m_subLayer->addChild(node);
}

void YesNoDialog::clearSubLayer()
{
    m_subLayer->removeAllChildrenWithCleanup(true);
}
void YesNoDialog::onEnterFrame1(float dt){
    m_time--;
    m_timeLabel->setString(mTimeDes + CC_SECTOA(m_time));
    if (m_time<=0) {
        PopupViewController::getInstance()->removePopupView(this);
    }
}
void YesNoDialog::onEnterFrame(float dt)
{
    if(mContents.size()>0){
        if (mContents.size()>3 && m_time>0 && m_contentLabel) {
            std::string showTip = "";
            mContents[2] = CC_SECTOA(m_time);
            std::string tmp = mContents[0];
            tmp.append("\n" + mContents[1] + " " + mContents[2]);
            tmp.append("\n" + mContents[3]);
            m_contentLabel->setString(tmp.c_str());
        }
        m_time--;
        if (m_time<=0) {
            PopupViewController::getInstance()->removePopupView(this);
        }
        return;
    }
    if (m_time>0) {
        int freeSpdT = GlobalData::shared()->freeSpdT;
        if (m_isBuild && m_time <=freeSpdT) {
            if(m_goldIcon->isVisible()) {
                m_goldNum->setString("");
                m_goldIcon->setVisible(false);
                m_btnText->setString(_lang("103672"));
                m_btnText->setPositionY(m_btnText->getPositionY()-14);
            }
        }
        else {
            int gold = 0;
            if (m_sumTime>0 && m_sumGold>0) {
                gold = m_sumGold*(m_time*1.0/m_sumTime)+0.5;
                gold = MAX(1, gold);
            }
            else
            {
                gold = CCCommonUtils::getGoldByTime(m_time);
            }
            m_goldNum->setString(CC_ITOA(gold));
            
        }
        m_time--;
        m_timeLabel->setString(_lang("108659") + CC_SECTOA(m_time));
        if (m_time<=0) {
            PopupViewController::getInstance()->removePopupView(this);
        }
    }
}

#pragma mark -
#pragma mark CocosBuilder Part
SEL_MenuHandler YesNoDialog::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName) {
	return NULL;    
}

SEL_CCControlHandler YesNoDialog::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) {

	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "keyPressedBtnOk", YesNoDialog::keypressedBtnOk);
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "keyPressedBtnCancel", YesNoDialog::keypressedBtnCancel);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickNoticeBtn", YesNoDialog::onClickNoticeBtn);
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "keyPressedBtnClose", YesNoDialog::keyPressedBtnClose);

	return NULL;
}

bool YesNoDialog::onAssignCCBMemberVariable(CCObject * pTarget, const char* pMemberVariableName, CCNode * pNode) {
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_layerColorBG", CCModelLayerColor*, this->m_layerColorBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subNode", CCNode*, this->m_subNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subLayer", CCLayer*, this->m_subLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnOk", CCControlButton*, this->m_btnOk);
	CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnCancel", CCControlButton*, this->m_btnCancel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldNum", CCLabelIF*, this->m_goldNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnText", CCLabelIF*, this->m_btnText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldIcon", CCSprite*, this->m_goldIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resNode", CCNode*, this->m_resNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noticeNode", CCNode*, this->m_noticeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noticeBtn", CCControlButton*, this->m_noticeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noticeSpr", CCSprite*, this->m_noticeSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noticeLabel", CCLabelIF*, this->m_noticeLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, this->m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprBG", CCNode*, this->m_sprBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt1", CCLabelIF*, this->m_txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt2", CCLabelIF*, this->m_txt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt3", CCLabelIF*, this->m_txt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt4", CCLabelIF*, this->m_txt4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_box1", CCNode*, this->m_box1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_box2", CCNode*, this->m_box2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_box3", CCNode*, this->m_box3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_box4", CCNode*, this->m_box4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectSpr", CCNode*, this->m_selectSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_muteTimeNode", CCNode*, this->m_muteTimeNode);
    return false;
}

#pragma mark -
#pragma mark Control Events

void YesNoDialog::keypressedBtnOk(CCObject * pSender, Control::EventType pCCControlEvent) {
	CCLOG("---------keypressedBtnOk--------------");
    if (m_function) {
        m_function();
    }
    this->retain();
    if(m_isAlliance){
        PopupViewController::getInstance()->removeAllPopupView();
        PopupViewController::getInstance()->addPopupInView(AllianceInfoView::create(&GlobalData::shared()->playerInfo.allianceInfo));
        
        if(GlobalData::shared()->playerInfo.isfirstJoin )//fusheng 第一次加入联盟
        {
            GuideController::share()->setGuide("3380100");//fusheng 联盟功能引导
            
        }
    }else if(m_closeAll){
        PopupViewController::getInstance()->removeAllPopupView();
    }else{
        PopupViewController::getInstance()->removePopupView(this);
    }
    if (m_onYes) {
        m_onClose=NULL;    // 如果执行了onYes，关闭的时候就不再执行onClose
        m_onYes->execute();
    }
    this->release();
}

void YesNoDialog::keyPressedBtnClose(CCObject * pSender, Control::EventType pCCControlEvent){
    if (m_onClose)
        m_onClose->execute();
    if(m_isAlliance){
        PopupViewController::getInstance()->removeAllPopupView();
        PopupViewController::getInstance()->addPopupInView(AllianceInfoView::create(&GlobalData::shared()->playerInfo.allianceInfo));
    }else if(m_closeAll){
        PopupViewController::getInstance()->removeAllPopupView();
    }else{
         PopupViewController::getInstance()->removePopupView(this);
    }
}

void YesNoDialog::keypressedBtnCancel(CCObject * pSender, Control::EventType pCCControlEvent) {
	CCLOG("---------keypressedBtnCancel--------------");

    if (m_onNo) {
        m_onClose=NULL;    // 如果执行了onNo，关闭的时候就不再执行onClose
        m_onNo->execute();
    }
    if (isCloseOnNoButton) {
        PopupViewController::getInstance()->removePopupView(this);
    } else if (disableWhenTouched) {
        m_btnCancel->setEnabled(false);
    }
    
}

void YesNoDialog::onClickNoticeBtn(CCObject * pSender, Control::EventType pCCControlEvent) {
    if (m_onSel) {
        m_noticeSpr->setVisible(!m_noticeSpr->isVisible());
        m_onSel->execute();
    }
}


/////////////////////////////////////////////////////
//
// ConfirmCoinView
//
/////////////////////////////////////////////////////
ConfirmCoinView::ConfirmCoinView(){
    
}
ConfirmCoinView::~ConfirmCoinView(){
    
}
ConfirmCoinView* ConfirmCoinView::create(){
    ConfirmCoinView* ret = new ConfirmCoinView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool ConfirmCoinView::init(){
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCBLoadFile("ConfirmCoinView",this,this);
    CCLoadSprite::doResourceByCommonIndex(8, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(8, false);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
    });
    
    setContentSize(CCDirector::sharedDirector()->getWinSize());
    setTitleName(_lang(""));
    if(GlobalData::shared()->analyticID == "common"){
        m_descriptionText->setString(_lang("E100001").c_str());
        setButtonTitle(m_confirmBtn, _lang("confirm").c_str());
    }else{
        m_descriptionText->setString(_lang("104912").c_str());
        setButtonTitle(m_confirmBtn, _lang("101221").c_str());
    }
    string titleString = "";
    GoldExchangeItem* dataItem=NULL;
    GoldExchangeItem* dataItemNormal=NULL;
    for (map<string, GoldExchangeItem*>::iterator it = GlobalData::shared()->goldExchangeList.begin(); it != GlobalData::shared()->goldExchangeList.end(); it++) {
        if(!it->second)
            continue;
        if(it->second->type=="5"){
            continue;
        }else if(it->second->type == "3"){
            if(it->second->bought||it->second->popup_image=="close"){
                continue;
            }
            if(it->second->end>GlobalData::shared()->getWorldTime()){
                if(dataItem==NULL ||dataItem->popup<it->second->popup){
                    dataItem = it->second;
                }
            }
        }else if(it->second->type == "1"){
            if(it->second->bought||it->second->popup_image=="hide" || it->second->popup_image.empty()){
                continue;
            }
            if(it->second->end>GlobalData::shared()->getWorldTime()){
                if(dataItemNormal==NULL ||dataItemNormal->popup<it->second->popup){
                    dataItemNormal = it->second;
                }
            }
        }
    }
    CCRect costBtnRect = CCRectZero;
    if(dataItem){
        titleString = _lang(dataItem->name);
        m_nodeAdv->removeAllChildren();
        CCSize size = m_nodeAdv->getContentSize();
        CCClipNode *clipNode = CCClipNode::create(size.width, size.height);
        m_nodeAdv->addChild(clipNode);
        if( dataItem->popup_image == "throne"){
            auto cell = GoldExchangeHallweenCell::create(dataItem);
            clipNode->addChild(cell);
        }else{
            if(LuaController::getInstance()->checkLuaValid(dataItem->popup_image,dataItem->md5,dataItem->type)){
                std::string info = dataItem->toString();
                costBtnRect = LuaController::getInstance()->createExchangeAdvCell(clipNode,info);
                auto &array = clipNode->getChildren();
                for(auto child:array)
                {
                    if(child)
                    {
                        child->setPositionY(0.0f);
                    }
                }
            }else{
                auto cell = GoldExchangeAdvertisingCommCell::create(dataItem);
                clipNode->addChild(cell);
                costBtnRect = cell->getCostBtnRect();
            }
        }
    }else if(dataItemNormal){
        titleString = _lang(dataItemNormal->name);
        m_nodeAdv->removeAllChildren();
        CCSize size = m_nodeAdv->getContentSize();
        CCClipNode *clipNode = CCClipNode::create(size.width, size.height);
        m_nodeAdv->addChild(clipNode);

        string path = CCFileUtils::sharedFileUtils()->getWritablePath();
        bool bCCB2 = CCFileUtils::sharedFileUtils()->isFileExist(path + "/lua/ccbi/GoldExchangeAdvnormal_giftLuaCell.ccbi");
        bool bImage1 = CCFileUtils::sharedFileUtils()->isFileExist(path + "/lua/resources/normal_giftadv.pvr.ccz");
        bool bImagePlist1 = CCFileUtils::sharedFileUtils()->isFileExist(path + "/lua/resources/normal_giftadv.plist");
        if(bCCB2 && bImage1 && bImagePlist1){
            std::string info = dataItemNormal->toString();
            costBtnRect = LuaController::getInstance()->createExchangeAdvCell(clipNode,info);
            auto &array = clipNode->getChildren();
            for(auto child:array)
            {
                if(child)
                {
                    child->setPositionY(0.0f);
                }
            }
        }
    }
    m_titleLabel->setString(titleString.c_str());
    
//    if(!costBtnRect.equals(CCRectZero)){//fusheng  不显示按钮高光
//        CCNode *animNode = CCNode::create();
//        m_nodeAdv->addChild(animNode);
//        CCPoint pos = costBtnRect.origin;
//        animNode->setContentSize(costBtnRect.size);
//        animNode->setPosition(pos);
//        ButtonLightEffect* btnEffect = ButtonLightEffect::create(costBtnRect.size);
//        animNode->addChild(btnEffect);
////        btnEffect->setPosition(ccp(0, -10));
//    }
    
    return true;
}

void ConfirmCoinView::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
}

void ConfirmCoinView::onExit(){
    setTouchEnabled(false);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCNode::onExit();
}

bool ConfirmCoinView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (isTouchInside(m_touchNode, pTouch)) {
        return false;
    }
    this->closeSelf();
    return true;
}

void ConfirmCoinView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void ConfirmCoinView::keyPressedBtnOk(CCObject * pSender, CCControlEvent pCCControlEvent){
    this->closeSelf();
    PopupViewController::getInstance()->addPopupView(GoldExchangeView::create(),false);
}

SEL_CCControlHandler ConfirmCoinView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "keyPressedBtnOk", ConfirmCoinView::keyPressedBtnOk);
    return NULL;
}

bool ConfirmCoinView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_descriptionText", CCLabelIF*, this->m_descriptionText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeAdv", CCNode*, this->m_nodeAdv);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_confirmBtn", CCControlButton*, this->m_confirmBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    return false;
}
