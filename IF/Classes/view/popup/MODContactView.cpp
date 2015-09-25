//
//  MODContactView.cpp
//  IF
//
//  Created by 李锐奇 on 14/11/19.
//
//

#include "MODContactView.h"
#include "PopupViewController.h"
#include "MODCommand.h"
#include "MailController.h"
#include "ParticleController.h"
#include "FunBuildController.h"
#include "HelpshiftCocos2dx.h"
#include "LanguageSettingView.h"

#define LANGUAGE_COUNT 17
const char* lang_name[LANGUAGE_COUNT] = {"English","Français","Deutsch","Pусский","한국어","ไทย","日本語","Português","Español","العربية","Türkçe","Indonesia","繁體中文","简体中文","Italiano","Polski","Nederlands"};
const char* lang_icon[LANGUAGE_COUNT] = {"GbUs","FR","DE","RU","KR","TH","JP","BrPr","ES","AE","TR","ID","TW","CN","IT","PL","NL"};
const char* lang_type[LANGUAGE_COUNT] = {"en","fr","de","ru","ko","th","ja","pt","es","ar","tr","id","zh_TW","zh_CN","it","pl","nl"};

const int lang_status[LANGUAGE_COUNT] = {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0};//

string getStringByType(string str,int isName){
    
    string name = "";
    string icon = "";
    if (str=="zh-Hans" || str=="zh_CN") {
        name = "简体中文";
        icon = "CN";
    }
    if (str=="zh-Hant" || str=="zh_TW") {
        name = "繁體中文";
        icon = "TW";
    }
    if (str=="ms"|| str=="id") {
        name = "Indonesia";
        icon = "ID";
    }
    if (str=="ja") {
        name = "日本語";
        icon = "JP";
    }
    if (str=="en") {
        name = "English";
        icon = "GbUs";
    }
    if (str=="fr") {
        name = "Français";
        icon = "FR";
    }
    if (str=="it") {
        name = "Italiano";
        icon = "IT";
    }
    //没上的语言
    if (str=="de") {
        name = "Deutsch";
        icon = "DE";
    }
    if (str=="ru") {
        name = "Pусский";
        icon = "RU";
    }
    if (str=="ko") {
        name = "한국어";
        icon = "KR";
    }
    if (str=="th") {
        name = "ไทย";
        icon = "TH";
    }
    if (str=="pt") {
        name = "Português";
        icon = "BrPr";
    }
    if (str=="es") {
        name = "Español";
        icon = "ES";
    }
    if (str=="ar") {
        name = "العربية";
        icon = "AE";
    }
    if (str=="tr") {
        name = "Türkçe";
        icon = "TR";
    }
    if (str=="pl") {
        name = "Polski";
        icon = "PL";
    }
    if (str=="nl") {
        name = "Nederlands";
        icon = "NL";
    }
    //    if (str=="") {
    //        name = "";
    //        icon = "";
    //    }
    if (icon!="") {
        if((icon=="TW" || icon=="HK")  && CCCommonUtils::checkTaiWanFlag()){
            icon = "CN";
        }
        icon = CCCommonUtils::changeChinaFlag(icon);
        icon+=".png";
    }
    
    if (isName==1) {
        return name;
    }else if (isName==0){
        return icon;
    }
    return "";
}

MODContactView *MODContactView::create(){
    MODContactView *ret = new MODContactView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void MODContactView::onEnter(){
    PopupBaseView::onEnter();
    setTitleName(_lang("132000"));
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(MODContactView::changeFlagIcon),"changeFlagIcon", NULL);
}

void MODContactView::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "changeFlagIcon");
    PopupBaseView::onExit();
}

bool MODContactView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        auto ccb = CCBLoadFile("MODContactView", this, this);
        this->setContentSize(ccb->getContentSize());
        int addH = CCDirector::sharedDirector()->getWinSize().height - 852;
        if (CCCommonUtils::isIosAndroidPad())
        {
            addH = CCDirector::sharedDirector()->getWinSize().height - 2048;
        }
        m_bottomNode->setPositionY(m_bottomNode->getPositionY() - addH);
        CCCommonUtils::setButtonTitle(m_contactBtn, _lang("132001").c_str());
        CCCommonUtils::setButtonTitle(m_howToBtn, _lang("132003").c_str());
        
        CCCommonUtils::setButtonTitle(m_mailBtn, _lang("132035").c_str());
        //语言国旗按钮
        std::string defLan = LocalController::shared()->getLanguageFileName();
        defLan="en";//默认英文
        std::string language= CCUserDefault::sharedUserDefault()->getStringForKey("usd_mod_language", defLan);
        language=getStringByType(language,0);
        auto pic  = CCLoadSprite::createSprite(language.c_str());
        if(pic && CCCommonUtils::isShowFlagForChinese()){
            m_lanNode->addChild(pic);
            pic->setTag(111);
            if (!CCCommonUtils::isIosAndroidPad())
            {
                if(pic->getContentSize().width>111){
                    pic->setScale(0.6776);
                }else{
                    pic->setScale(0.88);
                }
            }
            else
              pic->setScale(2);
            
        }
        
        this->m_introText->setString(_lang("132002"));
        m_bgContainer->setPositionY(m_bgContainer->getPositionY() - addH);
        addBG();
        
        for (int i=1; i<=4; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
            m_fireNode1->addChild(particle);
            
            auto particle1 = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
            m_fireNode2->addChild(particle1);
        }
        
//        屏蔽mod
//        m_lanNode->setVisible(false);
//        m_mailBtn->setVisible(false);
        getMODLanguageFromSever();

        ret = true;
    }
    return ret;
}

bool MODContactView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_contactBtn", CCControlButton*, m_contactBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_howToBtn", CCControlButton*, m_howToBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_introText", CCLabelIF*, m_introText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgContainer", CCNode*, m_bgContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode1", CCNode*, m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode2", CCNode*, m_fireNode2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lanNode", CCNode*, m_lanNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lanBtn", CCControlButton*, m_lanBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailBtn", CCControlButton*, m_mailBtn);

    return false;
}

SEL_CCControlHandler MODContactView::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickSendMail", MODContactView::onClickSendMail);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickIntro", MODContactView::onClickIntro);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickLanBtn", MODContactView::onClickLanBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickMailBtn", MODContactView::onClickMailBtn);
    
    return NULL;
}

void MODContactView::onClickSendMail(CCObject * pSender, Control::EventType pCCControlEvent){
    std::string defLan = LocalController::shared()->getLanguageFileName();
    defLan="en";//默认英文
    std::string lan= CCUserDefault::sharedUserDefault()->getStringForKey("usd_mod_language", defLan);
//    lan="cn";
    auto cmd = new MODCommand(lan);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(MODContactView::onGetModDataBack), NULL));
    cmd->sendAndRelease();
//    onGetModDataBack(NULL);
}

void MODContactView::onClickIntro(CCObject * pSender, Control::EventType pCCControlEvent){
    if(FunBuildController::getInstance()->getMainCityLv() < 10){
        CCCommonUtils::flyHint("", "", _lang("132034"));
        return;
    }
    PopupViewController::getInstance()->addPopupInView(IntroPopUpView::create());
}

void MODContactView::onClickLanBtn(CCObject * pSender, Control::EventType pCCControlEvent){
    if(modLanArr&&modLanArr->count()>0){
        PopupViewController::getInstance()->addPopupInView(MODLanguageView2::create(modLanArr));
    }
}
void MODContactView::onClickMailBtn(CCObject * pSender, Control::EventType pCCControlEvent){
    HelpshiftCocos2dx::showConversation();
}

void MODContactView::onGetModDataBack(CCObject *obj){
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : net error
        return;
    }
    
    auto params = _dict(ret->getData());
    std::string modUid = params->valueForKey("uid")->getCString();
    std::string modName = params->valueForKey("name")->getCString();
    std::string modLanguage = params->valueForKey("lang")->getCString();
//    std::string modUid = "123456";
//    std::string modName = "haodehouha";
//    std::string modLanguage = "zh_CN";
    MailController::getInstance()->openMailDialogViewFirst(modName, modUid, modLanguage, MAIL_MOD_PERSONAL);
}

void MODContactView::addBG(){
    std::string picName = "technology_09.png";
    auto totalH = CCDirector::sharedDirector()->getWinSize().height;
    int h = 0;
    auto batch = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource(picName.c_str())->getTexture());
    m_bgContainer->addChild(batch);
    while (h < totalH) {
        auto sprite = CCLoadSprite::createSprite(picName.c_str());
        sprite->setAnchorPoint(ccp(0, 0));
        batch->addChild(sprite);
        sprite->setPosition(ccp(0, h));
        h += sprite->getContentSize().height;
        if (CCCommonUtils::isIosAndroidPad())
        {
            sprite->setScale(2.4);
            
        }
    }
}

void MODContactView::changeFlagIcon(CCObject* lan){
    std::string lanStr=dynamic_cast<CCString*>(lan)->getCString();
    //语言国旗按钮
    string icon = getStringByType(lanStr,0);
    auto pic  = CCLoadSprite::createSprite(icon.c_str());
    if(pic){
        m_lanNode->removeChildByTag(111);
        m_lanNode->removeChildByTag(222);
        m_lanNode->addChild(pic);
        pic->setTag(111);
        if (!CCCommonUtils::isIosAndroidPad())
        {
            if(pic->getContentSize().width>111){
                pic->setScale(0.6776);
            }else{
                pic->setScale(0.88);
            }
        }
        else
            pic->setScale(2.f);
        
    }
}

void MODContactView::getMODLanguageFromSever(){
    auto cmd = new MODLanGetCommand();
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(MODContactView::getMODLanguageFromSeverBack), NULL));
    cmd->sendAndRelease();
}

void MODContactView::getMODLanguageFromSeverBack(CCObject *obj){
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : net error
        return;
    }
    
    auto params = _dict(ret->getData());
    modLanArr=dynamic_cast<CCArray*>(params->objectForKey("langlist"));
    modLanArr->retain();
}

//----
#pragma mark IntroPopUpView
bool IntroPopUpView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        auto ccb = CCBLoadFile("IntroPopUpView", this, this);
        auto size = CCDirector::sharedDirector()->getWinSize();
        this->setContentSize(ccb->getContentSize());
        int addH = size.height - 852;
        if (CCCommonUtils::isIosAndroidPad())
        {
            addH = size.height - 2048;
        }
        m_container->setContentSize(m_container->getContentSize() + CCSize(0, addH));
        m_bottomNode->setPositionY(m_bottomNode->getPositionY() - addH);
        m_container->setPositionY(m_container->getPositionY() - addH);
        m_scrollView = CCScrollView::create(m_container->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_container->addChild(m_scrollView);
        m_bgContainer->setPositionY(m_bgContainer->getPositionY() - addH);
        addText();
        addBG();
        for (int i=1; i<=4; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
            m_fireNode1->addChild(particle);
            
            auto particle1 = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
            m_fireNode2->addChild(particle1);
        }

        ret = true;
    }
    return ret;
}

void IntroPopUpView::addBG(){
    std::string picName = "technology_09.png";
    auto totalH = CCDirector::sharedDirector()->getWinSize().height;
    int h = 0;
    auto batch = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource(picName.c_str())->getTexture());
    m_bgContainer->addChild(batch);
    while (h < totalH) {
        auto sprite = CCLoadSprite::createSprite(picName.c_str());
        sprite->setAnchorPoint(ccp(0, 0));
        batch->addChild(sprite);
        sprite->setPosition(ccp(0, h));
        h += sprite->getContentSize().height;
        if (CCCommonUtils::isIosAndroidPad())
        {
            sprite->setScale(2.4);
        }
    }
}

void IntroPopUpView::addText(){
    std::vector<string> vector = {
        "132004"
        ,"132025"
        ,"132026"
        ,"132027"
        ,"132028"
        ,"132029"
        ,"132030"
        ,"132031"
        ,"132032"
    };
    int h = 0;
    int index = vector.size() - 1;
    while (index >= 0) {
        std::string did = vector[index];
        std::string str = _lang(did);
        auto label = CCLabelIF::create(str.c_str());
        label->setDimensions(CCSize(600, 0));
        
        if("132025" == did || "132029" == did || "132031" == did){
            label->setColor(ccc3(228, 138, 34));
        }else if("132027" == did){
            label->setColor(ccc3(0, 109, 172));
        }else{
            label->setColor(ccc3(143, 119, 85));
        }
        label->setFontSize(28);
        if (CCCommonUtils::isIosAndroidPad())
        {
            label->setDimensions(CCSize(1400, 0));
            label->setFontSize(40);
        }
        m_scrollView->addChild(label);
        label->setPosition(ccp(0, h));
        int textH = label->getOriginScaleY() * label->getContentSize().height;
        h += textH + 10;
        index--;
    }
    m_scrollView->setContentSize(CCSize(600, h));
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_scrollView->setContentSize(CCSize(1400, h));
    }
    m_scrollView->setContentOffset(ccp(0, m_container->getContentSize().height - h));
}

bool IntroPopUpView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_container", CCNode*, this->m_container);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgContainer", CCNode*, m_bgContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode1", CCNode*, m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode2", CCNode*, m_fireNode2);

    return false;
}

#pragma mark MODLanguageView2
MODLanguageView2 *MODLanguageView2::create(CCArray *modLanArr){
    MODLanguageView2 *ret = new MODLanguageView2(modLanArr);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MODLanguageView2::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        auto ccb = CCBLoadFile("MODLanguageView2", this, this);
        auto size = CCDirector::sharedDirector()->getWinSize();
        this->setContentSize(ccb->getContentSize());
        int addH = size.height - 852;
        int lanH=40;
        if (CCCommonUtils::isIosAndroidPad())
        {
            addH = size.height - 2048;
            lanH = 60;

        }
        
        m_bottomNode->setPositionY(m_bottomNode->getPositionY() - addH);
//        m_container->setContentSize(m_container->getContentSize() + CCSize(0, addH));
        m_container->setContentSize(CCSize(m_container->getContentSize().width, m_container->getContentSize().height + addH -lanH));
        m_container->setPositionY(m_container->getPositionY() - addH -lanH);
        if (CCCommonUtils::isIosAndroidPad())
        {
            
        }
        m_scrollView = CCScrollView::create(m_container->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_container->addChild(m_scrollView);
        m_subTitle->setPositionY(m_container->getContentSize().height+50);//language
        m_bgContainer->setPositionY(m_bgContainer->getPositionY() - addH);
        
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_libBg->setPositionY(m_container->getContentSize().height+122);
            m_upSpr->setPositionY(m_container->getContentSize().height);
            m_subTitle->setPositionY(m_container->getContentSize().height+100);//language
        }
        
        
        
        m_subTitle->setString(_lang("102910"));
        addText();
        addBG();
        for (int i=1; i<=4; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
            m_fireNode1->addChild(particle);
            
            auto particle1 = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
            m_fireNode2->addChild(particle1);
        }
        
        ret = true;
    }
    return ret;
}

void MODLanguageView2::addBG(){
    std::string picName = "technology_09.png";
    auto totalH = CCDirector::sharedDirector()->getWinSize().height;
    int h = 0;
    auto batch = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource(picName.c_str())->getTexture());
    m_bgContainer->addChild(batch);
    while (h < totalH) {
        auto sprite = CCLoadSprite::createSprite(picName.c_str());
        sprite->setAnchorPoint(ccp(0, 0));
        batch->addChild(sprite);
        sprite->setPosition(ccp(0, h));
        h += sprite->getContentSize().height;
        if (CCCommonUtils::isIosAndroidPad())
        {
            sprite->setScale(2.4);
        }
    }
    
//    h = 0;
//    picName = "Alliance diban.png";
//    batch = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource(picName.c_str())->getTexture());
//    m_container->addChild(batch);
//    while (h < m_container->getContentSize().height) {
//        auto sprite = CCLoadSprite::createSprite(picName.c_str());
//        sprite->setAnchorPoint(ccp(0, 0));
//        batch->addChild(sprite);
//        sprite->setPosition(ccp(0, h));
//        h += sprite->getContentSize().height;
//    }

}

void MODLanguageView2::addText(){
    
    m_listNode=CCNode::create();
    int indexY =0;
    int indexX = 0;

    m_listNode->setPosition(0, m_container->getContentSize().height-150);
    int count = m_listData->count();
    int j=0;
    for (int i=0; i<count; i++) {
        CCDictionary *dict = dynamic_cast<CCDictionary*>(m_listData->objectAtIndex(i));
        string str = dict->valueForKey("lang")->getCString();
        indexY = j/4;
        indexX = j%4;
        if (CCCommonUtils::isIosAndroidPad())
        {
            indexY = j/5;
            indexX = j%5;
        }
        MODLanguageViewCell* cell = MODLanguageViewCell::create(str);
        m_listNode->addChild(cell);
        cell->setPosition(indexX*153, -indexY*150);
        if (CCCommonUtils::isIosAndroidPad())
        {
            cell->setScale(2.f);
            cell->setPosition(indexX*153*2, -indexY*150*2-150);
        }
        j++;
    }
    
    int h = 600;
    m_scrollView->addChild(m_listNode);
    m_scrollView->setContentSize(m_container->getContentSize());
//    m_scrollView->setContentOffset(ccp(0, -m_container->getContentSize().height));
}

bool MODLanguageView2::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_container", CCNode*, this->m_container);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgContainer", CCNode*, m_bgContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode1", CCNode*, m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode2", CCNode*, m_fireNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upSpr", CCScale9Sprite*, m_upSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_libBg", CCScale9Sprite*, m_libBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subTitle", CCLabelIF*, this->m_subTitle);
    
    return false;
}


#pragma mark MODLanguageView
MODLanguageView* MODLanguageView::create(){
    MODLanguageView* ret = new MODLanguageView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void MODLanguageView::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
    
    
}

void MODLanguageView::onExit(){
    
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool MODLanguageView::init(){
    if(PopupBaseView::init()){
        auto bg = CCBLoadFile("MODLanguageView", this, this);
        this->setContentSize(bg->getContentSize());
        m_listData= CCArray::create();
        setData();
        return true;
    }
    return false;
}

void MODLanguageView::setData()
{
    this->m_listNode->removeAllChildren();
    m_totalNode->setPositionY(0);
    m_listData->removeAllObjects();
    int indexY =0;
    int indexX = 0;
    
    m_translationTitle->setString(_lang("102910"));
    m_translationDes->setVisible(false);
    m_listNode->setPosition(-291, 82);
    int count = LANGUAGE_COUNT;
    for (int i=0; i<count; i++) {
        if(lang_status[i]==0){
            m_listData->addObject(CCString::create(CC_ITOA(i)));
        }
    }
    CCObject* obj=NULL;
    int j=0;
    CCARRAY_FOREACH(m_listData, obj){
        string str = dynamic_cast<CCString*>(obj)->getCString();
        indexY = j/4;
        indexX = j%4;
        MODLanguageViewCell* cell = MODLanguageViewCell::create(str);
        m_listNode->addChild(cell);
        cell->setPosition(indexX*153, -indexY*150);
        j++;
    }
    if(indexY>1){
        m_listBG->setContentSize(CCSize(m_listBG->getContentSize().width, 332+(indexY-1)*150));
        CCSize size = m_listBG->getContentSize();
        m_totalNode->setPositionY(109.0+(indexY-1)*150);
    }
}

bool MODLanguageView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_translationTitle", CCLabelIF*, this->m_translationTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_translationDes", CCLabelIF*, this->m_translationDes);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalNode", CCNode*, this->m_totalNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listBG", CCScale9Sprite*, this->m_listBG);
    return true;
}

SEL_CCControlHandler MODLanguageView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    return NULL;
}

bool MODLanguageView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (isTouchInside(m_listBG, pTouch)) {
        m_touchPoint = pTouch->getLocation();
        return false;
    }
    PopupViewController::getInstance()->removePopupView(this);
    return true;
}
void MODLanguageView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (fabs(pTouch->getLocation().x-m_touchPoint.x) > 30 || fabs(pTouch->getLocation().y-m_touchPoint.y) > 30)
    {
        return;
    }
    touchEvent();
}
void MODLanguageView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
}

void MODLanguageView::touchEvent()
{
    
}

#pragma mark MODLanguageViewCell
MODLanguageViewCell* MODLanguageViewCell::create(string str){
    MODLanguageViewCell* ret = new MODLanguageViewCell(str);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void MODLanguageViewCell::onEnter(){
    CCTableViewCell::onEnter();
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
    
    
}

void MODLanguageViewCell::onExit(){
    
    setTouchEnabled(false);
    CCTableViewCell::onExit();
}

bool MODLanguageViewCell::init(){
    if(CCTableViewCell::init()){
        auto bg = CCBLoadFile("MODLanguageViewCell", this, this);
        this->setContentSize(bg->getContentSize());
        this->m_name->setSystemFontSize(20);
        setData(m_str);
        return true;
    }
    return false;
}

void MODLanguageViewCell::setData(string str)
{
    m_str = str;
    
    m_languageType = m_str;

    string name = getStringByType(m_languageType, 1);
    string icon = getStringByType(m_languageType, 0);
    
    auto pic  = CCLoadSprite::createSprite(icon.c_str());
    if(pic->getContentSize().width > 111){
        pic->setScale(0.77);
    }
    this->m_picNode->removeAllChildren();
    this->m_picNode->addChild(pic);
    this->m_name->setString(name.c_str());
    std::string language = LocalController::shared()->getLanguageFileName();

}

bool MODLanguageViewCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name", Label*, this->m_name);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, this->m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconBG", CCScale9Sprite*, this->m_iconBG);
    return true;
}

SEL_CCControlHandler MODLanguageViewCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    return NULL;
}

bool MODLanguageViewCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_touchPoint = pTouch->getLocation();
    if (isTouchInside(this->m_iconBG, pTouch)) {
        return true;
    }
    return false;
}
void MODLanguageViewCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (fabs(pTouch->getLocation().x-m_touchPoint.x) > 30 || fabs(pTouch->getLocation().y-m_touchPoint.y) > 30)
    {
        return;
    }
    touchEvent();
}
void MODLanguageViewCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
}

void MODLanguageViewCell::touchEvent()
{
    CCLOGFUNC();

    CCUserDefault::sharedUserDefault()->setStringForKey("usd_mod_language", m_languageType);
    CCUserDefault::sharedUserDefault()->flush();

    PopupViewController::getInstance()->goBackPopupView();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("changeFlagIcon",CCString::create(m_languageType));

}

//string MODLanguageViewCell::getStringByType(string str,int isName){
//
//    string name = "";
//    string icon = "";
//    if (str=="zh-Hans") {
//        name = "简体中文";
//        icon = "CN";
//    }
//    if (str=="zh-Hant") {
//        name = "繁體中文";
//        icon = "TW";
//    }
//    if (str=="ms") {
//        name = "Indonesia";
//        icon = "ID";
//    }
//    if (str=="ja") {
//        name = "日本語";
//        icon = "JP";
//    }
//    if (str=="en") {
//        name = "English";
//        icon = "GbUs";
//    }
//    if (str=="fr") {
//        name = "Français";
//        icon = "FR";
//    }
//    if (str=="it") {
//        name = "Italiano";
//        icon = "IT";
//    }
//    //没上的语言
//    if (str=="de") {
//        name = "Deutsch";
//        icon = "DE";
//    }
//    if (str=="ru") {
//        name = "Pусский";
//        icon = "RU";
//    }
//    if (str=="ko") {
//        name = "한국어";
//        icon = "KR";
//    }
//    if (str=="th") {
//        name = "ไทย";
//        icon = "TH";
//    }
//    if (str=="pt") {
//        name = "Português";
//        icon = "BrPr";
//    }
//    if (str=="es") {
//        name = "Español";
//        icon = "ES";
//    }
//    if (str=="ar") {
//        name = "العربية";
//        icon = "AE";
//    }
//    if (str=="tr") {
//        name = "Türkçe";
//        icon = "TR";
//    }
//    if (str=="pl") {
//        name = "Polski";
//        icon = "PL";
//    }
//    if (str=="nl") {
//        name = "Nederlands";
//        icon = "NL";
//    }
////    if (str=="") {
////        name = "";
////        icon = "";
////    }
//    if (icon!="") {
//        if(GlobalData::shared()->playerInfo.regCountry=="CN" && icon=="TW"){
//            icon = "CN";
//        }
//        icon+=".png";
//    }
//
//    if (isName==1) {
//        return name;
//    }else if (isName==0){
//        return icon;
//    }
//    return "";
//}
