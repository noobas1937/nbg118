//
//  CreateAllianceView.cpp
//  IF
//
//  Created by chenliang on 14-3-25.
//
//

#include "CreateAllianceView.h"
#include "SoundController.h"
#include "PopupViewController.h"
#include "CreateAllianceCommand.h"
#include "CheckAllianceNameCommand.h"
#include "CheckAllianceTagCommand.h"
#include "YesNoDialog.h"
#include "ParticleController.h"
#include "AllianceInfoView.h"
#include "ChatController.h"
#include "FunBuildController.h"
#include "AllianceManager.h"

static int ALLIANCE_NAME_MAX_LENGTH = 20;
static int ALLIANCE_TAG_MAX_LENGTH = 3;

static const float BOTTOM_BUTTON_AREA_HEIGHT = 106;

CreateAllianceView* CreateAllianceView::create(float operateAreaHeight){
    CreateAllianceView* ret = new CreateAllianceView(operateAreaHeight);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void CreateAllianceView::editBoxEditingDidBegin(CCEditBox* editBox)
{
}

void CreateAllianceView::editBoxEditingDidEnd(CCEditBox* editBox)
{
}

static std::string allianceNameAvailableCharactersLeftPlaceHolder = "{1} characters left";
static int ALLIANCE_NAME_VALID_LENGTH = 10;
static int ALLIANCE_NAME_TAG_VALID_LENGTH = 3;
void CreateAllianceView::editBoxTextChanged(CCEditBox* editBox, const std::string& text)
{
    if(editBox == m_allianceNameTagEditBox)
    {
        int charactersLeft = ALLIANCE_NAME_TAG_VALID_LENGTH - text.length();
        
        m_allianceNameTagAvailableLettersLabel->setString(_lang_1("115008", CC_ITOA(charactersLeft)));
        
        CheckAllianceTagCommand* command = new CheckAllianceTagCommand();
        command->putParam("abbr", CCString::create(m_allianceNameTagEditBox->getText()));
        command->setCallback(CCCallFuncO::create(this, callfuncO_selector(CreateAllianceView::onCheckAllianceTag), NULL));
        command->sendAndRelease();
        
        m_allianceNameTagEditBox->setText(text.c_str());
    }
}

void CreateAllianceView::onCheckAllianceName(CCObject* data)
{
    m_nameOkNode->setVisible(true);
    NetResult* result = dynamic_cast<NetResult*>(data);
    
    CCString* stringData = dynamic_cast<CCString*>(result->getData());
    string current = m_allianceNameEditBox->getText();
    
    bool ok = stringData->intValue() != 1 && current.length() > 0;
    m_nameOkNode->getChildByTag(0)->setVisible(!ok);
    m_nameOkNode->getChildByTag(1)->setVisible(ok);
    m_btnCreate->setEnabled(ok);
}

void CreateAllianceView::onCheckAllianceTag(CCObject* data)
{
    m_tagOkNode->setVisible(true);
    NetResult* result = dynamic_cast<NetResult*>(data);
    
    CCString* stringData = dynamic_cast<CCString*>(result->getData());
    
    string currentTagContent = m_allianceNameTagEditBox->getText();
    bool ok = stringData->intValue() != 1 && currentTagContent.length() >= ALLIANCE_TAG_MAX_LENGTH;
    m_tagOkNode->getChildByTag(0)->setVisible(!ok);
    m_tagOkNode->getChildByTag(1)->setVisible(ok);
}

void CreateAllianceView::onCreateAlliance(CCObject* data)
{
    NetResult* result = dynamic_cast<NetResult*>(data);
    
    CCDictionary* alliance = dynamic_cast<CCDictionary*>(result->getData());
    if(alliance)
    {
        GlobalData::shared()->playerInfo.allianceInfo.updateAllianceInfo(alliance);
        GlobalData::shared()->playerInfo.setAllianceId(GlobalData::shared()->playerInfo.allianceInfo.uid);
        YesNoDialog::showVariableTitle(_lang_1("115012",GlobalData::shared()->playerInfo.allianceInfo.name.c_str()),CCCallFunc::create(this, callfunc_selector(CreateAllianceView::createSuccess)),_lang("confirm").c_str());
    }else{
        PopupViewController::getInstance()->removeAllPopupView();
    }
}

void CreateAllianceView::createSuccess(){
    PopupViewController::getInstance()->removeAllPopupView();
    AllianceManager::getInstance()->addAnnounceEvent();
    PopupViewController::getInstance()->addPopupInView(AllianceInfoView::create(&GlobalData::shared()->playerInfo.allianceInfo));
    //发聊天信息
    string str  = _lang_1("115217", GlobalData::shared()->playerInfo.allianceInfo.name.c_str());
    str.append("  (").append(_lang("115181")).append(")");
    ChatController::getInstance()->sendCountryChat(str.c_str(), CHAT_COUNTRY, 1);
}

void CreateAllianceView::editBoxReturn(CCEditBox* editBox)
{
}

bool CreateAllianceView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    //step 1
    //CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceNameEditBox", CCEditBox*, this->m_allianceNameEditBox);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_step1TitleLabel", CCLabelIF*, this->m_step1TitleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_step1ContentLabel", CCLabelIF*, this->m_step1ContentLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceNameAvailableLettersLabel", CCLabelIF*, this->m_allianceNameAvailableLettersLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_step1Node", CCNode*, this->m_step1Node);
    
    //step 5
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_step5TitleLabel", CCLabelIF*, this->m_step5TitleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_step5ContentLabel", CCLabelIF*, this->m_step5ContentLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameOkNode", CCNode*, this->m_nameOkNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg", CCScale9Sprite*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode1", CCNode*, this->m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode2", CCNode*, this->m_fireNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnCreate", CCControlButton*, this->m_btnCreate);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceBtnTitle", CCLabelIF*, this->m_allianceBtnTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_createBtnCost", CCLabelIF*, this->m_createBtnCost);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnCancel", CCControlButton*, this->m_btnCancel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, this->m_titleLabel);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceIntroEditBox", CCEditBox*, this->m_allianceIntroEditBox);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_enterNameTxt", CCLabelIF*, this->m_enterNameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_unionTitleTxt", CCLabelIF*, this->m_unionTitleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameNode", CCNode*, this->m_nameNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconGold", CCSprite*, this->m_iconGold);
    if (CCCommonUtils::isIosAndroidPad()) {
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cancleLabel", CCLabelIF*, this->m_cancleLabel);
    }
    return true;
}

SEL_CCControlHandler CreateAllianceView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "ontClickCreate", CreateAllianceView::ontClickCreate);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCancelClick", CreateAllianceView::onCancelClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onNameBgClick", CreateAllianceView::onNameBgClick);
    return NULL;
}

void CreateAllianceView::onChooseAllianceSymbolBtnClick(CCObject *pSender, CCControlEvent event)
{
//    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    
    CCCallFuncO* callback = CCCallFuncO::create(this, callfuncO_selector(CreateAllianceView::iconSelected), NULL);
}

void CreateAllianceView::ontClickCreate(CCObject *pSender, CCControlEvent event)
{
    m_allianceNameEditBox->setEnabled(false);
    m_allianceIntroEditBox->setEnabled(false);
    
    std::string allianceName = m_allianceNameEditBox->getText();
    if(allianceName=="" || allianceName.length()<=0) return ;
//    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    string language = setDefaultAllianceLanguge();
    
    CreateAllianceCommand* command = new CreateAllianceCommand();
    command->putParam("name", CCString::create(m_allianceNameEditBox->getText()));
    command->putParam("abbr", CCString::create(""));
    command->putParam("icon", CCString::create(""));
    command->putParam("intro", CCString::create(m_allianceIntroEditBox->getText()));
    command->putParam("language", CCString::create(language));
    
    command->setCallback(CCCallFuncO::create(this, callfuncO_selector(CreateAllianceView::onCreateAlliance), NULL));
    command->setFailCallback(CCCallFuncO::create(this, callfuncO_selector(CreateAllianceView::createFail), NULL));
    command->sendAndRelease();
    
    m_btnCreate->setEnabled(false);
    addLoadingAni();
}

void CreateAllianceView::createFail(CCObject* obj)
{
    m_allianceNameEditBox->setEnabled(true);
    m_allianceIntroEditBox->setEnabled(true);
    m_btnCreate->setEnabled(true);
    if (m_loadingIcon)
    {
        m_loadingIcon->removeFromParent();
    }
}

void CreateAllianceView::onCancelClick(CCObject *pSender, CCControlEvent event){
    this->closeSelf();
}

void CreateAllianceView::iconSelected(CCObject* object)
{
    CCString* iconName = dynamic_cast<CCString*>(object);
    m_symbolPlaceHolder->removeAllChildren();
    
    CCSprite* icon = CCSprite::create(iconName->getCString());
    m_symbolPlaceHolder->addChild(icon);
}

void CreateAllianceView::languageSelected(CCObject* data)
{
    string language = dynamic_cast<CCString*>(data)->getCString();
    
    setLanguage(language);
    m_step4CurrentLanguageLabel->setString(_lang(language.c_str()).c_str());
}

void CreateAllianceView::onChooseAllianceLanguageBtnClick(CCObject *pSender, CCControlEvent event)
{
//    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
}

bool CreateAllianceView::init()
{
    if(PopupBaseView::init())
    {
        CCLoadSprite::doResourceByCommonIndex(6, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(6, false);
        });
        
        setIsHDPanel(true);
        CCScrollView* scrollView = CCScrollView::create();
        scrollView->setDirection(kCCScrollViewDirectionVertical);
        
        scrollView->setAnchorPoint(CCPoint(0,0));
        scrollView->setPosition(0, BOTTOM_BUTTON_AREA_HEIGHT);
        
        CCNode* node = CCBLoadFile("AllianceCreateView", this, this);
        const CCSize& contentSize = node->getContentSize();
        this->setContentSize(contentSize);
        scrollView->setContentOffset(CCPoint(0, m_operateAreaHeight-contentSize.height));
        scrollView->setContentSize(contentSize);
        scrollView->setViewSize(CCSize(640, m_operateAreaHeight));
        
        addChild(scrollView);
        
        auto sprite9 = CCLoadSprite::createScale9Sprite("nb_vip_detail_list_bg.png");
        sprite9->setInsetBottom(15);
        sprite9->setInsetLeft(15);
        sprite9->setInsetRight(15);
        sprite9->setInsetTop(15);
        
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_allianceNameEditBox = CCEditBox::create(CCSizeMake(880,80),sprite9);
            m_allianceNameEditBox->setFontSize(44);
        } else {
            m_allianceNameEditBox = CCEditBox::create(CCSizeMake(440,40),sprite9);
            m_allianceNameEditBox->setFontSize(22);
        }
        m_allianceNameEditBox->setMaxLength(ALLIANCE_NAME_MAX_LENGTH);
        m_allianceNameEditBox->setFontColor(Color3B(196, 207, 255));
        m_allianceNameEditBox->setReturnType(kKeyboardReturnTypeDone);
        m_allianceNameEditBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
        m_nameNode->addChild(m_allianceNameEditBox);
        
        auto spriteText = CCLoadSprite::createScale9Sprite("nb_vip_detail_list_bg.png");
        spriteText->setInsetBottom(15);
        spriteText->setInsetLeft(15);
        spriteText->setInsetRight(15);
        spriteText->setInsetTop(15);
        
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_allianceIntroEditBox = CCEditBox::create(CCSizeMake(880,90),spriteText);
            m_allianceIntroEditBox->setFontSize(44);
        } else {
            m_allianceIntroEditBox = CCEditBox::create(CCSizeMake(440,45),spriteText);
            m_allianceIntroEditBox->setFontSize(22);
        }
        m_allianceIntroEditBox->setMaxLength(199);
        m_allianceIntroEditBox->setFontColor(Color3B(196, 207, 255));
        m_allianceIntroEditBox->setReturnType(kKeyboardReturnTypeDone);
        m_allianceIntroEditBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
        m_allianceIntroEditBox->setPosition(ccp(322.0, 690));
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_allianceIntroEditBox->setPosition(ccp(644, 818 * 2));
            auto node = CCNode::create();
            node->setScale(0.5);
            m_step1Node->addChild(node);
            node->addChild(m_allianceIntroEditBox);
        } else {
            m_step1Node->addChild(m_allianceIntroEditBox);
        }
        
        //step 1
        m_allianceNameEditBox->setPlaceHolder(_lang("115009").c_str());
        m_step1TitleLabel->setString(_lang("115007").c_str());
        m_step1ContentLabel->setString(_lang("115010"));
        //m_allianceNameEditBox->setText(_lang("115009").c_str());
        //m_enterNameTxt->setString(_lang("115009").c_str());
        m_nameOkNode->setVisible(false);
        m_titleLabel->setString(_lang("115007").c_str());
        m_unionTitleTxt->setString(_lang("115054").c_str());
//        CCCommonUtils::setButtonTitle(m_btnCreate, _lang("115007").c_str());
        string btnStr = "";
        if (FunBuildController::getInstance()->getMainCityLv()<GlobalData::shared()->alliance_cost_k8) {
            m_btnCreate->setEnabled(false);
            btnStr = _lang_1("115507", CC_ITOA(GlobalData::shared()->alliance_cost_k8));
        }
        else if(FunBuildController::getInstance()->getMainCityLv()>=GlobalData::shared()->alliance_cost_k8 && FunBuildController::getInstance()->getMainCityLv()<GlobalData::shared()->alliance_cost_k6){
            btnStr.append(CC_ITOA(GlobalData::shared()->alliance_cost_k5)).append("/").append(_lang_1("115241", CC_ITOA(GlobalData::shared()->alliance_cost_k6)));
        }else{
            btnStr = _lang("115062");
            m_iconGold->setVisible(false);
            m_createBtnCost->setPositionX(m_createBtnCost->getPositionX()+70);
        }
        m_createBtnCost->setString(btnStr.c_str());
        m_allianceBtnTitle->setString(_lang("115007").c_str());
        CCCommonUtils::setButtonTitle(m_btnCancel, _lang("cancel_btn_label").c_str());
        if (CCCommonUtils::isIosAndroidPad()) {
            CCCommonUtils::setButtonTitle(m_btnCancel,"");
            m_cancleLabel->setString(_lang("cancel_btn_label"));
        }
        
        m_btnCreate->setEnabled(false);
        
        for (int i=1; i<=4; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
            m_fireNode1->addChild(particle);
        }
        
        for (int i=1; i<=4; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
            m_fireNode2->addChild(particle);
        }
        lastName = "";
        this->schedule(schedule_selector(CreateAllianceView::checkName));
        return true;
    }
    
    return false;
}

void CreateAllianceView::addLoadingAni(){
    if(m_loadingIcon==NULL){
        m_loadingIcon = CCLoadSprite::createSprite("loading_1.png");
        m_loadingIcon->setAnchorPoint(ccp(0.5, 0.5));
        auto cSize =CCDirector::sharedDirector()->getWinSize();
        m_loadingIcon->setPosition(ccp(cSize.width/2,cSize.height/2));
    }
    
    CCActionInterval * rotateto1 = CCRotateTo::create(0.5, 720);
    CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
    CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
    m_loadingIcon->runAction(rotateForever);
    this->addChild(m_loadingIcon,1000000);
    
}

void CreateAllianceView::onNameBgClick(CCObject *pSender, CCControlEvent event){
        m_enterNameTxt->setString("");
}

void CreateAllianceView::checkName(float _time){
    std::string str = m_allianceNameEditBox->getText();
    if(str=="" || lastName==str || str.length()<3){
        return ;
    }
    m_enterNameTxt->setString("");
    lastName = str;
    m_step1ContentLabel->setString(_lang("115010"));
    CheckAllianceNameCommand* command = new CheckAllianceNameCommand();
    command->putParam("name", CCString::create(m_allianceNameEditBox->getText()));
    command->setCallback(CCCallFuncO::create(this, callfuncO_selector(CreateAllianceView::onCheckAllianceName), NULL));
    command->sendAndRelease();
}

void CreateAllianceView::onEnter(){
    PopupBaseView::onEnter();
}

void CreateAllianceView::onExit(){
    PopupBaseView::onExit();
}
string CreateAllianceView::setDefaultAllianceLanguge(){
    //联盟语言默认成设备语言。若设备语言不在xml所给列表，则设为all language。联盟语言的值保存为对应语言文件的ID。
    string langId = "";
    
    CCDictionary* dic_all = LocalController::shared()->shared()->DBXMLManager()->getGroupByKey("language");
    int idx = 9500;
    int num = dic_all->count();
    string allLanguageId = "";
    string curLang = GlobalData::shared()->lang;
    for (int i=0; i<num; ++i) {
        CCDictionary* dic_one = _dict(dic_all->objectForKey(CC_ITOA(idx+i)));
        CCLOG("%s %s %d",dic_one->valueForKey("lang_ma")->getCString(), dic_one->valueForKey("lang_id")->getCString(), dic_one->valueForKey("mark")->intValue());
        if(dic_one->valueForKey("mark") && strcmp(dic_one->valueForKey("lang_ma")->getCString(), "")!=0 && curLang.find(dic_one->valueForKey("lang_ma")->getCString())< curLang.length()){
            langId = dic_one->valueForKey("lang_id")->getCString();
        }
        if(strcmp(dic_one->valueForKey("lang_ma")->getCString(), "")==0){
            allLanguageId = dic_one->valueForKey("lang_id")->getCString();
        }
        CCLOG("langId %s", langId.c_str());
    }
    
    if(strcmp(langId.c_str(), "")==0){
        langId = allLanguageId;
    }
    
    return langId;
}