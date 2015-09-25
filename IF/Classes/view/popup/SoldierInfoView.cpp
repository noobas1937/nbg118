//
//  SoldierInfoView.cpp
//  IF
//
//  Created by ganxiaohua on 14-5-29.
//
//

#include "SoldierInfoView.h"
#include "PopupViewController.h"
#include "ProductionSoldiersView.h"
#include "FunBuildController.h"
#include "ArmyController.h"
#include "UIComponent.h"
#include "ParticleController.h"
#include "fireandcomman.h"
#include "YesNoDialog.h"
#include "TipsView.h"

SoldierInfoView* SoldierInfoView::create(ArmyInfo* info,int buildingId){
    SoldierInfoView* ret = new SoldierInfoView(info,buildingId);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool SoldierInfoView::init()
{
    if (!ArcPopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    bool m_isFort = m_buildingId/1000 == FUN_BUILD_FORT;
    int index = m_buildingId/1000000 - 423 + 200;
//    CCLoadSprite::doResourceByCommonIndex(index, true);
    CCLoadSprite::doResourceByCommonIndex(502, true);
    setCleanFunction([index](){
//        CCLoadSprite::doResourceByCommonIndex(index, false);
        CCLoadSprite::doResourceByCommonIndex(502, false);
    });
    
    CCBLoadFile("SoldierInfoView",this,this);
    setContentSize(CCDirector::sharedDirector()->getWinSize());
    int extH = getExtendHeight();
    m_mainNode->setPositionY(m_mainNode->getPositionY()+extH/2);
    m_bgColorLayer->setContentSize(CCSizeMake(m_bgColorLayer->getContentSize().width, m_bgColorLayer->getContentSize().height+extH));
    m_bgColorLayer->setPositionY(m_bgColorLayer->getPositionY() - extH);
    
//    this->setModelLayerTouchCallback([&](cocos2d::CCTouch *pTouch){
//        // detect touch inside panel
//        if (!isTouchInside(m_touchNode, pTouch)) {
//            this->onCloseclick(NULL,Control::EventType::TOUCH_DOWN);
//        }
//    });
    
    auto addBatchNode = [&](CCNode* node, const char* name){
        auto tbg = CCLoadSprite::loadResource(name);
        auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
        
        for (int i = 0; i < node->getChildrenCount(); i++)
        {
            auto bg = (CCNode*)(node->getChildren().at(i));
            bg->removeFromParent();
            tBatchNode->addChild(bg);
        }
        node->addChild(tBatchNode);
    };
    
    addBatchNode(m_bgNode1, "Ranking_tiaomu_bg8.png");
    addBatchNode(m_bgNode2, "Ranking_tiaomu_bg8.png");
    addBatchNode(m_bgNode3, "Ranking_tiaomu_bg8.png");
    addBatchNode(m_bgNode4, "Ranking_tiaomu_bg8.png");
    
    addBatchNode(m_bgNode5, "soldier_01.png");
    addBatchNode(m_bgNode6, "soldier_01.png");
    addBatchNode(m_bgNode7, "soldier_01.png");
    addBatchNode(m_bgNode8, "soldier_01.png");
    addBatchNode(m_bgNode9, "soldier_01.png");
    addBatchNode(m_bgNode10, "soldier_01.png");
    addBatchNode(m_bgNode11, "soldier_01.png");
    addBatchNode(m_bgNode12, "soldier_01.png");
    addBatchNode(m_bgNode13, "soldier_01.png");
    addBatchNode(m_bgNode14, "soldier_01.png");
    addBatchNode(m_bgNode15, "soldier_02.png");
    
    auto setBtnSp = [&](CCControlButton* btn){
        int tag = btn->getTag();
        string iconName = m_info->getAttrIcon(tag);
        CCLOG("iconName:%s", iconName.c_str());
        btn->setVisible(true);
        if (iconName == "")
        {
            btn->setVisible(false);
            return;
        }
        iconName.append(".png");
        btn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite(iconName.c_str()), CCControlStateNormal);
        btn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite(iconName.c_str()), CCControlStateHighlighted);
    };
    setBtnSp(m_infoBtn0);
    setBtnSp(m_infoBtn1);
    setBtnSp(m_infoBtn2);
    
    CCPoint pos = ccp(0,0);
    if(m_buildingId/1000 == FUN_BUILD_FORT){
        pos = ccp(0,-80);
    }
//    CCSprite* pic = CCLoadSprite::createSprite(m_info->getBodyIcon().c_str());
    m_iconSp->setPosition(pos);
//    m_icon->addChild(pic);
    m_iconSp->setDisplayFrame(CCLoadSprite::loadResource(m_info->getBodyIcon().c_str()));
//    if(m_buildingId/1000==FUN_BUILD_BARRACK4){
//        pic->setScale(0.85);
//    }
    if(m_isFort){
        std::string particleName = "WeaponsFire_Wood";
        for (int i=2; i <= 6; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s_%d",particleName.c_str(),i)->getCString());
            particle->setPosition(0, -95);
            m_icon->addChild(particle);
        }
        m_upkeepNode->setVisible(false);
    }
//    m_soldierBg->setVisible(m_buildingId/1000!=FUN_BUILD_BARRACK4);
    
    string lang = LocalController::shared()->getLanguageFileName();
    if (lang == "en" || lang == "ru")
    {
        m_attackValue->setPositionY(m_attackValue->getPositionY()-1);
        m_defValue->setPositionY(m_defValue->getPositionY()-1);
        m_lifeValue->setPositionY(m_lifeValue->getPositionY()-1);
        m_rangeValue->setPositionY(m_rangeValue->getPositionY()-1);
        m_speedValue->setPositionY(m_speedValue->getPositionY()-1);
        m_weihtValue->setPositionY(m_weihtValue->getPositionY()-1);
        m_upkeepValue->setPositionY(m_upkeepValue->getPositionY()-1);
        m_fightValue->setPositionY(m_fightValue->getPositionY()-1);
    }
    m_nameTxt->setString(m_info->getName());
//    m_attrTxt->setString(_lang("102136").c_str());
    m_fightTxt->setString((_lang("105034")+": ").c_str());
    CCString *powerStr = CCString::createWithFormat("%.1f",m_info->power);
    m_fightValue->setString(powerStr->getCString());
    m_attackTxt->setString((_lang("103600")+": ").c_str());
    m_attackValue->setString(CC_CMDITOA(m_info->attack));
    m_defTxt->setString((_lang("103601")+": ").c_str());
    m_defValue->setString(CC_CMDITOA( m_info->defence));
    m_lifeTxt->setString((_lang("103605")+": ").c_str());
    m_lifeValue->setString(CC_CMDITOA( m_info->health));
    m_speedTxt->setString((_lang("103602")+": ").c_str());
    m_speedValue->setString(CC_CMDITOA( m_info->speed));
    m_weihtTxt->setString((_lang("103604")+": ").c_str());
    m_weihtValue->setString(CC_CMDITOA(ArmyController::getInstance()->getLoad(m_info)));
    m_rangeTxt->setString((_lang("103603")+": ").c_str());
    int range = m_info->range/10;
    range = MAX(range, 1);
    m_rangeValue->setString(CC_CMDITOA(range));
    m_numTxt->setString(_lang_1("104907", ""));
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_numTxt->setMaxScaleXByWidth(170);
    } else {
        m_numTxt->setMaxScaleXByWidth(75);
    }
    m_numValue->setString(CC_CMDITOA(m_info->free));
    m_upkeepTxt->setString((_lang("103606")+": "));
    m_upkeepValue->setString(CCString::createWithFormat("%.2f", m_info->upkeep)->getCString());
//    m_desc->setString(m_info->getDesc().c_str());
    m_starNode->removeAllChildrenWithCleanup(true);
    for (int i=0; i<10; i++) {
        CCSprite* spr = NULL;
        if(m_info->level>=(i+1)){
            spr = CCLoadSprite::createSprite("mail_save.png");
            spr->setScale(0.6);
        }else{
            spr = CCLoadSprite::createSprite("icon_gray.png");
        }
        spr->setVisible(false);
        spr->setPosition(ccp(-131.6+i*28,0));
        m_starNode->addChild(spr);
    }
    //this->closeSelf();
    if(m_info->isArmy){
        CCCommonUtils::setButtonTitle(m_okBtn, _lang("102135").c_str());
    }else{
        CCCommonUtils::setButtonTitle(m_okBtn, _lang("102121").c_str());
    }
    
    auto m_sliderBg = CCLoadSprite::createScale9Sprite("huadongtiao3.png");
    m_sliderBg->setInsetBottom(5);
    m_sliderBg->setInsetLeft(5);
    m_sliderBg->setInsetRight(5);
    m_sliderBg->setInsetTop(5);
    m_sliderBg->setAnchorPoint(ccp(0.5,0.5));
    m_sliderBg->setPosition(ccp(304/2, 25));
    m_sliderBg->setContentSize(CCSize(304,18));
    
    auto bgSp = CCLoadSprite::createSprite("huadongtiao4.png");
    bgSp->setVisible(false);
    auto proSp = CCLoadSprite::createSprite("huadongtiao4.png");
    auto thuSp = CCLoadSprite::createSprite("huadongtiao1.png");
    
    m_trainSlider = CCSliderBar::createSlider(m_sliderBg, proSp, thuSp);
    m_trainSlider->setMinimumValue(0.0f);
    m_trainSlider->setMaximumValue(1.0f);
    m_trainSlider->setProgressScaleX(300/proSp->getContentSize().width);
    m_trainSlider->setTag(1);
    m_trainSlider->setLimitMoveValue(25);
    m_trainSlider->setTouchPriority(Touch_Popup);
    m_trainSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(SoldierInfoView::sliderCallBack), CCControlEventValueChanged);
    m_sliderContainer->addChild(m_trainSlider, 1);
    
    auto editSize = m_editBoxNode->getContentSize();
    auto editpic =CCLoadSprite::createScale9Sprite("frame_3.png");
    editpic->setContentSize(editSize);
    editpic->setInsetBottom(1);
    editpic->setInsetTop(1);
    editpic->setInsetRight(1);
    editpic->setInsetLeft(1);
    m_editBox = CCEditBox::create(editSize,editpic );
    m_editBox->setInputMode(kEditBoxInputModeNumeric);
    m_editBox->setText("0");
    m_editBox->setDelegate(this);
 //   m_editBox->setTouchPriority(Touch_Popup);
    m_editBox->setMaxLength(12);
    m_editBox->setReturnType(kKeyboardReturnTypeDone);
    m_editBox->setPosition(ccp(editSize.width/2, editSize.height/2));
    m_editBoxNode->addChild(m_editBox);
//    m_okBtn->setTouchPriority(Touch_Popup_Item);
    
    refreshNum();
    m_jieguNode->setVisible(false);
    
    if(m_isFort){
        m_defenseNode->setVisible(false);
        m_lifeNode->setVisible(false);
        m_speedNode->setVisible(false);
        m_weightNode->setVisible(false);
        m_upkeepNode->setVisible(false);
        m_rangeNode->setPosition(m_defenseNode->getPosition());
        m_fightNode->setPosition(m_lifeNode->getPosition());
    }
    return true;
}
void SoldierInfoView::editBoxTextChanged(CCEditBox* editBox, const std::string& text){
    std::string temp  =text;
    size_t pos = temp.find_first_not_of("1234567890");
    while (pos!=std::string::npos) {
        temp.replace(pos, 1, "");
        pos = temp.find_first_not_of("1234567890");
    }
    int a = atoi(temp.c_str());
    std::string aaaa = CC_CMDITOA(a);
    editBox->setText(aaaa.c_str());
}

void SoldierInfoView::onEnter(){
    UIComponent::getInstance()->hideReturnBtn();
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SoldierInfoView::refreshNum), ARMY_NUM_CHANGE, NULL);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void SoldierInfoView::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, ARMY_NUM_CHANGE);
    setTouchEnabled(false);
    CCNode::onExit();
}
void SoldierInfoView::sliderCallBack(CCObject*sender,CCControlEvent even){
    m_hireNum = m_trainSlider->getValue() * m_info->free;
    refreshNum();
}
void SoldierInfoView::editBoxReturn(CCEditBox *editBox){
    string numStr = m_editBox->getText();
    if(!numStr.empty()){
        size_t pos = numStr.find(",");
        while(pos!=string::npos){
            numStr.replace(pos, 1, "");
            pos=numStr.find(",");
        }
    }
    int num = atoi(numStr.c_str());
    if(num>m_info->free){
        num = m_info->free;
        
    }else if(num<0){
        num=0;
    }
    m_hireNum = num;
    if(m_info->free<=0){
        m_trainSlider->setValue(0.0);
    }else
        m_trainSlider->setValue(1.0f * num / m_info->free);
    if(m_info->free<=0){
        m_trainSlider->setTouchEnabled(false);
    }else{
        m_trainSlider->setTouchEnabled(true);
    }
    //    refreshNum();
}
void SoldierInfoView::refreshNum(CCObject* p){
    if(m_hireNum>m_info->free){
        m_hireNum = m_info->free;
    }
    m_editBox->setText(CC_CMDITOA(m_hireNum).c_str());
    if(m_hireNum>0){
        m_okBtn->setEnabled(true);
    }else{
        m_okBtn->setEnabled(false);
    }
    m_numValue->setString(CC_CMDITOA(m_info->free));
    string maxStr = "/"+CC_CMDITOA(m_info->free);
//    m_numMaxText->setString(maxStr);
    if(m_info->free<=0){
        m_trainSlider->setTouchEnabled(false);
    }else{
        m_trainSlider->setTouchEnabled(true);
    }
}
void SoldierInfoView::onOkclick(CCObject * pSender, Control::EventType pCCControlEvent){
    if(m_hireNum>0){
        if(m_info->isArmy){
            YesNoDialog::show(_lang("102317").c_str() , CCCallFunc::create(this, callfunc_selector(SoldierInfoView::goToDismissAction)));
        }else{
            YesNoDialog::show(_lang("102318").c_str() , CCCallFunc::create(this, callfunc_selector(SoldierInfoView::goToDismissAction)));
        }
    }
    m_jieguNode->setVisible(false);
    m_infoBtn0->setEnabled(true);
    m_infoBtn1->setEnabled(true);
    m_infoBtn2->setEnabled(true);
}
void SoldierInfoView::goToDismissAction(){
    if(m_info->isArmy){
        fireandcomman *fire = new fireandcomman(m_info->armyId, m_hireNum);
        fire->sendAndRelease();
    }else{
        fireandcomman *fire = new fireandcomman(m_info->armyId, m_hireNum,FORT_FIRE);
        fire->sendAndRelease();
    }
    m_hireNum = 0;
    m_trainSlider->setValue(1.0f * m_hireNum / m_info->free);
    
}
void SoldierInfoView::onAddClick(CCObject * pSender, Control::EventType pCCControlEvent){
    float value  = m_trainSlider->getValue() * m_info->free+1;
    if(m_info->free<=0){
        m_trainSlider->setValue(0.0);
    }else
        m_trainSlider->setValue(1.0f * value / m_info->free);
}
void SoldierInfoView::onSubClick(CCObject * pSender, Control::EventType pCCControlEvent){
    float value = m_trainSlider->getValue() * m_info->free-1;
    if(m_info->free<=0){
        m_trainSlider->setValue(0.0);
    }else
        m_trainSlider->setValue(1.0f * value / m_info->free);
}
void SoldierInfoView::onCloseclick(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->goBackPopupView();
}

void SoldierInfoView::onInfoDetailClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
    CCControlButton* btn = (CCControlButton*)pSender;
    int tag = btn->getTag();
    vector<string> descVec;
    m_info->getAttrDesc(tag, descVec);
    string title = _lang(descVec.at(0));
    string info = _lang(descVec.at(1));
    PopupViewController::getInstance()->addPopupView(TipsNewView::create(title,info));
}

bool SoldierInfoView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_jieguNode->isVisible() && !isTouchInside(m_bgColorLayerS, pTouch))
    {
        if (m_editOpen)
        {
            m_editOpen = false;
            return false;
        }
        return true;
    }
    else if (isTouchInside(m_jieguSp, pTouch))
    {
        return true;
    }
    else if (!isTouchInside(m_touchNode, pTouch))
    {
        return true;
    }
    m_editOpen = false;
    return false;
}

void SoldierInfoView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_jieguNode->isVisible() && !isTouchInside(m_bgColorLayerS, pTouch))
    {
        m_jieguNode->setVisible(false);
        m_infoBtn0->setEnabled(true);
        m_infoBtn1->setEnabled(true);
        m_infoBtn2->setEnabled(true);
    }
    else if (isTouchInside(m_jieguSp, pTouch))
    {
        m_jieguNode->setVisible(true);
        m_infoBtn0->setEnabled(false);
        m_infoBtn1->setEnabled(false);
        m_infoBtn2->setEnabled(false);
    }
    else if (!isTouchInside(m_touchNode, pTouch))
    {
        onCloseclick(NULL, CCControlEventTouchUpInside);
    }
}

SEL_CCControlHandler SoldierInfoView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOkclick", SoldierInfoView::onOkclick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSubClick", SoldierInfoView::onSubClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddClick", SoldierInfoView::onAddClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onInfoDetailClick", SoldierInfoView::onInfoDetailClick);
    return NULL;
}

bool SoldierInfoView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_okBtn", CCControlButton*, this->m_okBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numMaxText", CCLabelIF*, this->m_numMaxText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIF*, this->m_nameTxt);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_attrTxt", CCLabelIF*, this->m_attrTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_attackTxt", CCLabelIF*, this->m_attackTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_attackValue", CCLabelIF*, this->m_attackValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defTxt", CCLabelIF*, this->m_defTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defValue", CCLabelIF*, this->m_defValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lifeTxt", CCLabelIF*, this->m_lifeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lifeValue", CCLabelIF*, this->m_lifeValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rangeTxt", CCLabelIF*, this->m_rangeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rangeValue", CCLabelIF*, this->m_rangeValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_speedTxt", CCLabelIF*, this->m_speedTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_speedValue", CCLabelIF*, this->m_speedValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_weihtTxt", CCLabelIF*, this->m_weihtTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_weihtValue", CCLabelIF*, this->m_weihtValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numTxt", CCLabelIF*, this->m_numTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numValue", CCLabelIF*, this->m_numValue);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desc", CCLabelIF*, this->m_desc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon", CCNode*, this->m_icon);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_soldierBg", CCScale9Sprite*, this->m_soldierBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_starNode", CCNode*, this->m_starNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upkeepTxt", CCLabelIF*, this->m_upkeepTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upkeepValue", CCLabelIF*, this->m_upkeepValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upkeepNode", CCNode*, this->m_upkeepNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defenseNode", CCNode*, this->m_defenseNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lifeNode", CCNode*, this->m_lifeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rangeNode", CCNode*, this->m_rangeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_speedNode", CCNode*, this->m_speedNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_weightNode", CCNode*, this->m_weightNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sliderContainer", CCNode*, this->m_sliderContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editBoxNode", CCNode*, this->m_editBoxNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fightNode", CCNode*, this->m_fightNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fightValue", CCLabelIF*, this->m_fightValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fightTxt", CCLabelIF*, this->m_fightTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoBtn0", CCControlButton*, this->m_infoBtn0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoBtn1", CCControlButton*, this->m_infoBtn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoBtn2", CCControlButton*, this->m_infoBtn2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downNode", CCNode*, this->m_downNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode1", CCNode*, this->m_bgNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode2", CCNode*, this->m_bgNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode3", CCNode*, this->m_bgNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode4", CCNode*, this->m_bgNode4);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode5", CCNode*, this->m_bgNode5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode6", CCNode*, this->m_bgNode6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode7", CCNode*, this->m_bgNode7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode8", CCNode*, this->m_bgNode8);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode9", CCNode*, this->m_bgNode9);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode10", CCNode*, this->m_bgNode10);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode11", CCNode*, this->m_bgNode11);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode12", CCNode*, this->m_bgNode12);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode13", CCNode*, this->m_bgNode13);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode14", CCNode*, this->m_bgNode14);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode15", CCNode*, this->m_bgNode15);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_jieguNode", CCNode*, this->m_jieguNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgColorLayer", CCLayerColor*, this->m_bgColorLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgColorLayerS", CCLayerColor*, this->m_bgColorLayerS);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconSp", CCSprite*, this->m_iconSp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_jieguSp", CCSprite*, this->m_jieguSp);
    return false;
}
