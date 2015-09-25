//
//  GeneralSkillListPopUpView.cpp
//  IF
//
//  Created by 李锐奇 on 14-5-20.
//
//

#include "GeneralSkillListPopUpView.h"
#include "PopupViewController.h"
#include "GeneralCommand.h"
#include "GeneralManager.h"
#include "YesNoDialog.h"
#include "UIComponent.h"
#include "ParticleController.h"
#include "ParticleFireAni.h"
#include "CCMathUtils.h"
#include "ToolController.h"
#include "SoundController.h"

GeneralSkillListPopUpView *GeneralSkillListPopUpView::create(GeneralInfo *info, std::string abilityId,std::string skillId){
    GeneralSkillListPopUpView *ret = new GeneralSkillListPopUpView(info, abilityId,skillId);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GeneralSkillListPopUpView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        m_bgNode = CCNode::create();
        this->addChild(m_bgNode);
        
        CCLoadSprite::doResourceByCommonIndex(100, true);
        CCLoadSprite::doResourceByCommonIndex(4, true);
        CCLoadSprite::doResourceByCommonIndex(7, true);
        auto bg = CCBLoadFile("Generaltianfu_02.ccbi", this, this);
        this->setContentSize(bg->getContentSize());
        CCLoadSprite::doResourceByCommonIndex(105, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(100, false);
            CCLoadSprite::doResourceByCommonIndex(105, false);
            CCLoadSprite::doResourceByCommonIndex(4, false);
            CCLoadSprite::doResourceByCommonIndex(7, false);
        });

        skillChangeFlag = false;
        m_totalH = 0;
        int prev = m_viewBg->getContentSize().height;
        changeBGHeight(m_viewBg);
        int add = m_viewBg->getContentSize().height - prev;
        m_addH = add;
        m_container->setContentSize(CCSize(m_container->getContentSize().width,m_container->getContentSize().height+add));
        m_aniContainer->setContentSize(m_container->getContentSize());
        m_container->setPositionY(m_container->getPositionY() - add);
        m_aniContainer->setPositionY(m_aniContainer->getPositionY() - add);
        m_parNode->setPositionY(m_parNode->getPositionY() - add/2);
        m_viewBg->setVisible(false);

        auto size2 = m_container->getContentSize();
        
        CCNode *node = CCNode::create();
        this->addChild(node);
        node->setPositionY(-add);
        
        auto size = CCDirector::sharedDirector()->getWinSize();
        m_scrollView = CCScrollView::create(m_container->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_scrollView->setAnchorPoint(ccp(0, 0));
        m_scrollView->setTouchPriority(Touch_Popup);
        m_container->addChild(m_scrollView);
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_scrollView->getContainer()->setScale(2.0);
            m_scrollView->setPositionX(100);
//            m_scrollView->setContentSize(m_container->getContentSize()*2.f);
        }
        
        m_nameTxt->setString(CCCommonUtils::getNameById(m_abilityId).c_str());
        
        m_tabTxt1->setString(_lang("128000"));
        m_tabTxt2->setString(_lang("128001"));
        m_tabTxt3->setString(_lang("128002"));
        float fw = m_tabTxt3->getContentSize().width;
        if (fw>160) {//防爆框
            m_tabTxt3->setScaleX(160.0/fw);
        }
        if(m_abilityId=="50000"){
            onTabClick1(NULL,Control::EventType::TOUCH_DOWN);
        }else if(m_abilityId=="50001"){
            onTabClick2(NULL,Control::EventType::TOUCH_DOWN);
        }else{
             onTabClick3(NULL,Control::EventType::TOUCH_DOWN);
        }
        
        m_btnResetText->setString(_lang("105447"));
        fw = m_btnResetText->getContentSize().width;
        if (fw>220) {//防爆框
            m_btnResetText->setScaleX(220.0/fw);
        }
        resetBtnIcon();
        ret = true;
    }
    return ret;
}

void GeneralSkillListPopUpView::onEnter(){
    PopupBaseView::onEnter();
    UIComponent::getInstance()->showPopupView(1);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GeneralSkillListPopUpView::generalSkillChange), GENERAL_SKILL_UPDATE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GeneralSkillListPopUpView::unLockSkill), GENERAL_UNLOCK_SKILL_UPDATE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GeneralSkillListPopUpView::refreshPoint), SAVE_SKILL_SUCCESS, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GeneralSkillListPopUpView::refreshPoint), SAVE_SKILL_FAIL, NULL);
    //this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
}

void GeneralSkillListPopUpView::onExit(){
    saveData();
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GENERAL_SKILL_UPDATE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GENERAL_UNLOCK_SKILL_UPDATE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, SAVE_SKILL_SUCCESS);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, SAVE_SKILL_FAIL);
    PopupBaseView::onExit();
}

void GeneralSkillListPopUpView::refreshPoint(CCObject* param){
    m_tabValue1->setString(CC_ITOA(m_info->getUsePoint("50000")));
    m_tabValue2->setString(CC_ITOA(m_info->getUsePoint("50001")));
    m_tabValue3->setString(CC_ITOA(m_info->getUsePoint("50002")));
    auto &general = GlobalData::shared()->generals.begin()->second;
    m_expTxt->setString(_lang_1("105032", CC_ITOA(GeneralManager::getInstance()->getTotalFreeSkillPoint(general))));
}

void GeneralSkillListPopUpView::resetBtnIcon(){
    m_resetNode->removeAllChildrenWithCleanup(true);
    int num = getResetItemNum();
    string picPath = "";
    if(num>0){
        picPath = CCCommonUtils::getIcon("200040");
        m_btnReseValue->setString(CC_ITOA(1));
    }else{
        picPath = "ui_gold.png";
        int cost = GlobalData::shared()->generalConfig.abilityResetGold;
        m_btnReseValue->setString(CC_ITOA(cost));
    }
    auto sprite = CCLoadSprite::createSprite(picPath.c_str());
    m_resetNode->addChild(sprite);
    CCCommonUtils::setSpriteMaxSize(sprite, 35);
}

int GeneralSkillListPopUpView::getResetItemNum(){
    int num = 0;
    auto &toolInfo = ToolController::getInstance()->getToolInfoById(200040);
    if (toolInfo.getCNT()>0) {
        num += toolInfo.getCNT();
    }
    return num;
}

void GeneralSkillListPopUpView::saveData(){
    if(!skillChangeFlag){
        return;
    }
    skillChangeFlag = false;
    CCDictionary *dict = CCDictionary::create();
    map<std::string, map<std::string, GeneralSkillInfo*>>::iterator it = m_info->generalSkillMap.find(m_abilityId);
    if(it == m_info->generalSkillMap.end()){
        return;
    }
    map<std::string, GeneralSkillInfo*>::iterator skillIt;
    for(skillIt = it->second.begin(); skillIt != it->second.end(); skillIt++){
        if(skillIt->second->level != 0){
            dict->setObject(CCInteger::create(skillIt->second->level), skillIt->first);
        }
    }
    GeneralSkillSaveCommand *cmd = new GeneralSkillSaveCommand(m_info->uuid, m_abilityId, dict);
    cmd->sendAndRelease();
    GeneralManager::getInstance()->resetGeneralSkillEffectValue();
}

void GeneralSkillListPopUpView::generalSkillChange(CCObject* obj){
    skillChangeFlag = true;
    refreshView(false);
}

void GeneralSkillListPopUpView::unLockSkill(CCObject* param){
    skillChangeFlag = true;

    CCString* skillObj = dynamic_cast<CCString*>(param);
    float useTime = 0.05;
    if (skillObj!=NULL) {
        string tempSkill = skillObj->getCString();
        int tempId = atoi(tempSkill.c_str());
        GeneralSkillCell* skillCell = dynamic_cast<GeneralSkillCell*>(this->m_scrollView->getContainer()->getChildByTag(tempId));
        if(skillCell){
            vector<std::string> next = skillCell->m_info->next;
            useTime = 0;//addLineAni(tempId);
            for (int i=0; i<next.size(); i++) {
                int id = atoi(next[i].c_str());
                GeneralSkillCell* cell = dynamic_cast<GeneralSkillCell*>(this->m_scrollView->getContainer()->getChildByTag(id));
                if(cell && cell->m_info->level<=0){
                    cell->showUnLockEffect(useTime);
                }
            }
        }
        this->unschedule(schedule_selector(GeneralSkillListPopUpView::playUnLockEnd));
        //this->schedule(schedule_selector(GeneralSkillListPopUpView::playUnLockEnd), 0.3+useTime);
        this->schedule(schedule_selector(GeneralSkillListPopUpView::playUnLockEnd), 0.7);
    }
}

void GeneralSkillListPopUpView::playUnLockEnd(float _time){
    this->unschedule(schedule_selector(GeneralSkillListPopUpView::playUnLockEnd));
    refreshView(false);
}

void GeneralSkillListPopUpView::refreshView(bool moveFlag){
    CCLoadSprite::doResourceByCommonIndex(105, true);
    m_tabValue1->setString(CC_ITOA(m_info->getUsePoint("50000")));
    m_tabValue2->setString(CC_ITOA(m_info->getUsePoint("50001")));
    m_tabValue3->setString(CC_ITOA(m_info->getUsePoint("50002")));
    m_totalH = 0;
    m_scrollView->getContainer()->stopAllActions();
    CCPoint scPos = this->m_scrollView->getContentOffset();
    this->m_scrollView->getContainer()->removeAllChildren();
    map<std::string, map<std::string, GeneralSkillInfo*>>::iterator it = m_info->generalSkillMap.find(m_abilityId);

    if(m_info != nullptr && it != m_info->generalSkillMap.end()){
        map<std::string, GeneralSkillInfo*>::iterator skillIt;
        for(skillIt = it->second.begin(); skillIt != it->second.end(); skillIt++){
            addCell(skillIt->second);
        }
    }else{
        std::string skills = CCCommonUtils::getPropById(m_abilityId, "skill");
        std::string posStr = CCCommonUtils::getPropById(m_abilityId, "position");
        std::string relationStr = CCCommonUtils::getPropById(m_abilityId, "relation");
        vector<std::string> posVector;
        vector<std::string> relationVector;
        vector<std::string> vector1;
        vector<std::string> vector2;
        vector<std::string> vector;
        CCCommonUtils::splitString(skills, "|", vector);
        CCCommonUtils::splitString(posStr, "|", vector1);
        CCCommonUtils::splitString(relationStr, "|", vector2);
        
        if(vector.size() == 0){
            return;
        }
        int i = 0;
        int j = 0;
        while(i < vector.size()){
            posVector.clear();
            relationVector.clear();
            std::string skillId = vector[i];
            CCCommonUtils::splitString(vector1[i], ";", posVector);
            if(vector2.size() > i && vector2[i] != ""){
                CCCommonUtils::splitString(vector2[i], ";", relationVector);
            }
            GeneralSkillInfo *info = GeneralSkillInfo::create(skillId, 0, "", m_abilityId);
            info->x = atoi(posVector[1].c_str());
            info->y = atoi(posVector[0].c_str());
            j = 0;
            while(j < relationVector.size()){
                info->next.push_back(relationVector[j]);
                j++;
            }
            addCell(info);
            i++;
        }
    }
    addLine();
    setBtnState();
//    m_totalH -= gapY;
    m_totalH = -m_totalH;
    auto& children = m_scrollView->getContainer()->getChildren();
    for (auto child : children)
    {
        CCNode *node = dynamic_cast<CCNode*>(child);
        if(node){
            node->setPositionY(node->getPositionY() + m_totalH);
        }
    }
//    CCArray *arr = m_scrollView->getContainer()->getChildren();
//    int i = 0;
//    while(i < arr->count()){
//        CCNode *node = dynamic_cast<CCNode*>(arr->objectAtIndex(i));
//        if(node){
//            node->setPositionY(node->getPositionY() + m_totalH);
//        }
//        i++;
//    }
    auto &general = GlobalData::shared()->generals.begin()->second;
    m_expTxt->setString(_lang_1("105032", CC_ITOA(GeneralManager::getInstance()->getTotalFreeSkillPoint(general))));
    
    m_scrollView->setContentSize(CCSize(640, m_totalH));
    
    
    int skillOffY = 0;
    if(m_skillId!=""){
        int sid = atoi(m_skillId.c_str());
        GeneralSkillCell *skillCell = dynamic_cast<GeneralSkillCell*>(this->m_scrollView->getContainer()->getChildByTag(sid));
        if(skillCell){
            skillOffY = m_totalH - skillCell->getPositionY() - this->m_scrollView->getViewSize().height/2;
        }
        m_skillId = "";
    }
    if(!moveFlag){
        m_scrollView->setContentOffset(scPos);
    }else{
        int currentY = m_container->getContentSize().height - m_totalH + 70+skillOffY;
        CCPoint maxOffset = m_scrollView->maxContainerOffset();
        
        if(currentY>=maxOffset.y){
            currentY = maxOffset.y;
        }
        if (CCCommonUtils::isIosAndroidPad())
        {
            currentY *= 2.9f;
        }
        m_scrollView->setContentOffset(ccp(0, currentY));
    }
}

bool GeneralSkillListPopUpView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_skillResetBtn", CCControlButton*, m_skillResetBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bNode", CCNode*, m_bNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIF*, m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_expTxt", CCLabelIF*, m_expTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_container", CCNode*, m_container);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg", CCScale9Sprite*, m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tab1", CCControlButton*, m_tab1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tab2", CCControlButton*, m_tab2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tab3", CCControlButton*, m_tab3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tabTxt1", CCLabelIF*, m_tabTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tabValue1", CCLabelIF*, m_tabValue1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tabTxt2", CCLabelIF*, m_tabTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tabValue2", CCLabelIF*, m_tabValue2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tabTxt3", CCLabelIF*, m_tabTxt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tabValue3", CCLabelIF*, m_tabValue3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnResetText", CCLabelIF*, m_btnResetText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnReseValue", CCLabelIF*, m_btnReseValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_parNode", CCNode*, m_parNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_aniContainer", CCNode*, m_aniContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resetNode", CCNode*, m_resetNode);
    return false;
}

SEL_CCControlHandler GeneralSkillListPopUpView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSkillReset", GeneralSkillListPopUpView::onSkillReset);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTabClick1", GeneralSkillListPopUpView::onTabClick1);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTabClick2", GeneralSkillListPopUpView::onTabClick2);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTabClick3", GeneralSkillListPopUpView::onTabClick3);
    return NULL;
}

void GeneralSkillListPopUpView::onTabClick1(CCObject * pSender, Control::EventType pCCControlEvent){
    saveData();
    CCLoadSprite::doResourceByCommonIndex(105, true);
    m_bgNode->removeAllChildrenWithCleanup(true);
    auto tbg = CCLoadSprite::loadResource("UI_UseSkill_picture_blackwhite.png");
    auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
    int maxHight = CCDirector::sharedDirector()->getWinSize().height;
    int curHight = -500;
    while (curHight<maxHight) {
//        auto bg = CCLoadSprite::createSprite("UI_UseSkill_picture_01.png");
        auto bg = CCCommonUtils::addFilterSprite("UI_UseSkill_picture_blackwhite.png", CCCommonUtils::covertHSBToRGB(20, 0.77, 1.0), 0, 1.04, 4.4);
        bg->setAnchorPoint(ccp(0, 1));
        if (CCCommonUtils::isIosAndroidPad())
        {
            bg->setScale(2.4);
        }

        bg->setPosition(ccp(0, curHight));
        curHight += bg->getContentSize().height;
        tBatchNode->addChild(bg);
    }
    m_bgNode->addChild(tBatchNode);
    m_abilityId = "50000";
    m_tab1->setEnabled(false);
    m_tab2->setEnabled(true);
    m_tab3->setEnabled(true);
    
    m_tabTxt1->setColor({225,52,52});
    m_tabValue1->setColor({225,52,52});
    
    m_tabTxt2->setColor({47,91,96});
    m_tabValue2->setColor({47,91,96});
    
    m_tabTxt3->setColor({74,106,25});
    m_tabValue3->setColor({74,106,25});
    
    int viewH = m_viewBg->getContentSize().height;
    m_viewBg->initWithSpriteFrame(CCLoadSprite::loadResource("UI_Skill_Red.png"));
    
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_viewBg->setPreferredSize(CCSize(1536,viewH));
    }
    else
        m_viewBg->setContentSize(CCSize(640,viewH));
    
    m_viewBg->setAnchorPoint(ccp(0.5, 1.0));
    m_parNode->removeAllChildrenWithCleanup(true);
    for (int i=1; i<3; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("SkillBG_R_%d",i)->getCString());
        particle->setPositionY(i==1?-m_addH:m_addH);
        m_parNode->addChild(particle);
    }
    refreshView();
}

void GeneralSkillListPopUpView::onTabClick2(CCObject * pSender, Control::EventType pCCControlEvent){
    saveData();
    CCLoadSprite::doResourceByCommonIndex(105, true);
    m_bgNode->removeAllChildrenWithCleanup(true);
    auto tbg = CCLoadSprite::loadResource("UI_UseSkill_picture_blackwhite.png");
    auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
    int maxHight = CCDirector::sharedDirector()->getWinSize().height;
    int curHight = -500;
    while (curHight<maxHight) {
//        auto bg = CCLoadSprite::createSprite("UI_UseSkill_picture_02.png");
        auto bg = CCCommonUtils::addFilterSprite("UI_UseSkill_picture_blackwhite.png", CCCommonUtils::covertHSBToRGB(190, 0.77, 1.0), 0, 1.04, 4.4);
        bg->setAnchorPoint(ccp(0, 1));
        bg->setPosition(ccp(0, curHight));
        curHight += bg->getContentSize().height;
        tBatchNode->addChild(bg);
        
    }
    if (CCCommonUtils::isIosAndroidPad())
    {
        tBatchNode->setScale(2.4f);
    }
    m_bgNode->addChild(tBatchNode);
    m_abilityId = "50001";
    m_tab1->setEnabled(true);
    m_tab2->setEnabled(false);
    m_tab3->setEnabled(true);
    
    m_tabTxt1->setColor({102,52,50});
    m_tabValue1->setColor({102,52,50});
    
    m_tabTxt2->setColor({0,182,227});
    m_tabValue2->setColor({0,182,227});
    
    m_tabTxt3->setColor({74,106,25});
    m_tabValue3->setColor({74,106,25});
    
    int viewH = m_viewBg->getContentSize().height;
    m_viewBg->initWithSpriteFrame(CCLoadSprite::loadResource("skill_bg_blue.png"));
    m_viewBg->setContentSize(CCSize(640,viewH));
    m_viewBg->setAnchorPoint(ccp(0.5, 1.0));
    m_parNode->removeAllChildrenWithCleanup(true);
    for (int i=1; i<3; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("SkillBG_B_%d",i)->getCString());
        particle->setPositionY(i==1?-m_addH:m_addH);
        m_parNode->addChild(particle);
    }
    refreshView();
}

void GeneralSkillListPopUpView::onTabClick3(CCObject * pSender, Control::EventType pCCControlEvent){
    saveData();
    CCLoadSprite::doResourceByCommonIndex(105, true);
    m_bgNode->removeAllChildrenWithCleanup(true);
    auto tbg = CCLoadSprite::loadResource("UI_UseSkill_picture_blackwhite.png");
    auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
    int maxHight = CCDirector::sharedDirector()->getWinSize().height;
    int curHight = -500;
    while (curHight<maxHight) {
//        auto bg = CCLoadSprite::createSprite("UI_UseSkill_picture_03.png");
        auto bg = CCCommonUtils::addFilterSprite("UI_UseSkill_picture_blackwhite.png", CCCommonUtils::covertHSBToRGB(108, 0.77, 1.0), 0, 1.04, 4.4);

        bg->setAnchorPoint(ccp(0, 1));
        bg->setPosition(ccp(0, curHight));
        curHight += bg->getContentSize().height;
        tBatchNode->addChild(bg);
    }
    if (CCCommonUtils::isIosAndroidPad())
    {
        tBatchNode->setScale(2.4f);
    }
    m_bgNode->addChild(tBatchNode);
    m_abilityId = "50002";
    m_tab1->setEnabled(true);
    m_tab2->setEnabled(true);
    m_tab3->setEnabled(false);
    
    m_tabTxt1->setColor({102,52,50});
    m_tabValue1->setColor({102,52,50});
    
    m_tabTxt2->setColor({47,91,96});
    m_tabValue2->setColor({47,91,96});
    
    m_tabTxt3->setColor({157,227,0});
    m_tabValue3->setColor({157,227,0});
    
    int viewH = m_viewBg->getContentSize().height;
    m_viewBg->initWithSpriteFrame(CCLoadSprite::loadResource("skill_bg_green.png"));
    m_viewBg->setContentSize(CCSize(640,viewH));
    m_viewBg->setAnchorPoint(ccp(0.5, 1.0));
    m_parNode->removeAllChildrenWithCleanup(true);
    for (int i=1; i<3; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("SkillBG_G_%d",i)->getCString());
        particle->setPositionY(i==1?-m_addH:m_addH);
        m_parNode->addChild(particle);
    }
    refreshView();
}

void GeneralSkillListPopUpView::onSkillReset(CCObject * pSender, Control::EventType pCCControlEvent){
    map<std::string, map<std::string, GeneralSkillInfo*>>::iterator skillIt = m_info->generalSkillMap.find(m_abilityId);
    if(skillIt == m_info->generalSkillMap.end()){//study skill
        int index = -1;
        int i = 0;
        while(i < m_info->abilityToChoose.size()){
            if(m_abilityId == m_info->abilityToChoose[i]){
                index = i;
                break;
            }
            i++;
        }
        if(index == -1){
            return;
        }
        GeneralAbilityChooseCommand *cmd = new GeneralAbilityChooseCommand(m_info->uuid, index, m_abilityId);
        cmd->sendAndRelease();
        GeneralManager::getInstance()->addGeneralAbility(*m_info, m_abilityId);
        m_info->abilityToChoose.clear();
        PopupViewController::getInstance()->goBackPopupView();
    }else{//resest skill point
        if(getResetItemNum()>0){
            YesNoDialog::show( _lang("105453").c_str() , CCCallFunc::create(this, callfunc_selector(GeneralSkillListPopUpView::confirmResetSkill)));
        }else{
            int cost = GlobalData::shared()->generalConfig.abilityResetGold;
            auto dialog = YesNoDialog::show(_lang("105453").c_str(), CCCallFunc::create(this, callfunc_selector(GeneralSkillListPopUpView::confirmResetSkill)), cost);
            dialog->setYesText(_lang("105447").c_str());
        }
    }
}

void GeneralSkillListPopUpView::confirmResetSkill(){
    int cost = GlobalData::shared()->generalConfig.abilityResetGold;
    string itemUUid = "";
    if(getResetItemNum()>0){
        auto &toolInfo = ToolController::getInstance()->getToolInfoById(200040);
        itemUUid = toolInfo.uuid;
    }
    if(!CCCommonUtils::isEnoughResourceByType(Gold, cost) && itemUUid==""){
        YesNoDialog::gotoPayTips();
        return;
    }
    m_lastPoint = GeneralManager::getInstance()->getTotalFreeSkillPoint(*m_info);
    m_skillResetBtn->setEnabled(false);
    GeneralSkillResetCommand *cmd = new GeneralSkillResetCommand(m_info->uuid, m_abilityId,itemUUid);
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(GeneralSkillListPopUpView::resetSuccess), NULL));
    cmd->sendAndRelease();

}

void GeneralSkillListPopUpView::resetSuccess(CCObject* param){
    GeneralManager::getInstance()->resetGeneralSkillEffectValue();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("Skill_Reset_Effect");
    playResetAni(m_info->getUsePoint("50000"),m_info->getUsePoint("50001"),m_info->getUsePoint("50002"));
    map<std::string, map<std::string, GeneralSkillInfo*>>::iterator skillIt = m_info->generalSkillMap.find("50000");
    map<std::string, GeneralSkillInfo*>::iterator it;
    for(it = skillIt->second.begin(); it != skillIt->second.end(); it++){
        it->second->level = 0;
    }
    skillIt = m_info->generalSkillMap.find("50001");
    for(it = skillIt->second.begin(); it != skillIt->second.end(); it++){
        it->second->level = 0;
    }
    skillIt = m_info->generalSkillMap.find("50002");
    for(it = skillIt->second.begin(); it != skillIt->second.end(); it++){
        it->second->level = 0;
    }
    this->scheduleOnce(schedule_selector(GeneralSkillListPopUpView::delayPlayAni), 0.9);
    this->resetBtnIcon();
}

void GeneralSkillListPopUpView::playResetAni(int v1,int v2,int v3){
    
    CCPoint pos = m_aniContainer->convertToNodeSpace(CCPoint(ccp(320, m_aniContainer->getContentSize().height+150)));
    CCPoint worldPos = m_tabValue1->getParent()->convertToWorldSpace(m_tabValue1->getPosition());
    CCPoint startPoint = m_aniContainer->convertToNodeSpace(worldPos);
    int m_flyX = startPoint.x;
    int m_flyY = startPoint.y;
    
    int toX = pos.x;
    int toY = pos.y;
    int num = MIN(35,v1);
    float delayTime = 1.2;
    for(int i=0;i<num;i++){
        auto pNode = CCNode::create();
        CCPoint endPoint = ccp(toX, toY);
        float angle=CCMathUtils::getAngle(startPoint, endPoint);
        float angle2 = fabsf(angle);
        pNode->setPosition(startPoint);
        auto ball = CCLoadSprite::createSprite("skill_fly_ball.png");
        ball->setOpacity(0);
        pNode->addChild(ball,1000);
        CCSequence* sc1 = CCSequence::create(CCDelayTime::create(0.2),CCFadeIn::create(0.1),CCScaleTo::create(0.5, 0.3),NULL);
        CCSequence* sc2 = CCSequence::create(CCDelayTime::create(0.2),CCFadeIn::create(0.1),CCFadeOut::create(0.5),NULL);
        CCSpawn* swp = CCSpawn::create(sc1,sc2,NULL);
        ball->runAction(swp);
        
        ccBezierConfig  beziercofig;
        int rand = GlobalData::shared()->getRand(1,100);
        if(rand%2==0){
            beziercofig.controlPoint_1 = ccp(m_flyX+GlobalData::shared()->getRand(30,60),m_flyY);
            beziercofig.controlPoint_2 = ccp(toX+GlobalData::shared()->getRand(30,60),toY);
        }else{
            beziercofig.controlPoint_1 = ccp(m_flyX-GlobalData::shared()->getRand(30,60),m_flyY);
            beziercofig.controlPoint_2 = ccp(toX-GlobalData::shared()->getRand(30,60),toY);
        }
        if(angle2<15){
            angle = 0;
        }else{
            angle = 90-angle;
        }
        float useFactor = (toY - m_flyY)/400.0;
        useFactor = MAX(useFactor, 1);
        float useTime = delayTime + i*0.08;
        if(num>30){
            useTime = delayTime + i*0.07;
        }
        beziercofig.endPosition = endPoint;
        CCActionInterval *forward = CCBezierTo::create(useTime, beziercofig);
        //auto func = CCCallFuncO::create(this, callfuncO_selector(ImperialScene::onFlyTroopEnd), pNode);
        CCSequence* s1 = CCSequence::create(forward ,CCDelayTime::create(0.05), NULL);
        CCSequence* s2 = CCSequence::create(CCRotateTo::create(useTime*0.7, angle),NULL);//
        CCSpawn* sp = CCSpawn::create(s1,s2,NULL);
        pNode->runAction(sp);
        m_aniContainer->addChild(pNode, 10000);
    }
    
    pos = m_aniContainer->convertToNodeSpace(CCPoint(ccp(320, m_aniContainer->getContentSize().height+150)));
    worldPos = m_tabValue2->getParent()->convertToWorldSpace(m_tabValue1->getPosition());
    startPoint = m_aniContainer->convertToNodeSpace(worldPos);
    m_flyX = startPoint.x;
    m_flyY = startPoint.y;
    
    toX = pos.x;
    toY = pos.y;
    num = MIN(35,v2);
    for(int i=0;i<num;i++){
        auto pNode = CCNode::create();
        CCPoint endPoint = ccp(toX, toY);
        float angle=CCMathUtils::getAngle(startPoint, endPoint);
        float angle2 = fabsf(angle);
        pNode->setPosition(startPoint);
        auto ball = CCLoadSprite::createSprite("skill_fly_ball.png");
        ball->setOpacity(0);
        pNode->addChild(ball,1000);
        CCSequence* sc1 = CCSequence::create(CCDelayTime::create(0.2),CCFadeIn::create(0.1),CCScaleTo::create(0.5, 0.3),NULL);
        CCSequence* sc2 = CCSequence::create(CCDelayTime::create(0.2),CCFadeIn::create(0.1),CCFadeOut::create(0.5),NULL);
        CCSpawn* swp = CCSpawn::create(sc1,sc2,NULL);
        ball->runAction(swp);
        
        ccBezierConfig  beziercofig;
        int rand = GlobalData::shared()->getRand(1,100);
        if(rand%2==0){
            beziercofig.controlPoint_1 = ccp(m_flyX+GlobalData::shared()->getRand(30,60),m_flyY);
            beziercofig.controlPoint_2 = ccp(toX+GlobalData::shared()->getRand(30,60),toY);
        }else{
            beziercofig.controlPoint_1 = ccp(m_flyX-GlobalData::shared()->getRand(30,60),m_flyY);
            beziercofig.controlPoint_2 = ccp(toX-GlobalData::shared()->getRand(30,60),toY);
        }
        if(angle2<15){
            angle = 0;
        }else{
            angle = 90-angle;
        }
        float useFactor = (toY - m_flyY)/400.0;
        useFactor = MAX(useFactor, 1);
        float useTime = delayTime + i*0.08;
        if(num>30){
            useTime = delayTime + i*0.07;
        }
        beziercofig.endPosition = endPoint;
        CCActionInterval *forward = CCBezierTo::create(useTime, beziercofig);
        CCSequence* s1 = CCSequence::create(forward ,CCDelayTime::create(0.05), NULL);
        CCSequence* s2 = CCSequence::create(CCRotateTo::create(useTime*0.7, angle),NULL);//
        CCSpawn* sp = CCSpawn::create(s1,s2,NULL);
        pNode->runAction(sp);
        m_aniContainer->addChild(pNode, 10000);
    }
    
    
    pos = m_aniContainer->convertToNodeSpace(CCPoint(ccp(320, m_aniContainer->getContentSize().height+150)));
    worldPos = m_tabValue3->getParent()->convertToWorldSpace(m_tabValue1->getPosition());
    startPoint = m_aniContainer->convertToNodeSpace(worldPos);
    m_flyX = startPoint.x;
    m_flyY = startPoint.y;
    
    toX = pos.x;
    toY = pos.y;
    num = MIN(35,v3);
    for(int i=0;i<num;i++){
        auto pNode = CCNode::create();
        CCPoint endPoint = ccp(toX, toY);
        float angle=CCMathUtils::getAngle(startPoint, endPoint);
        float angle2 = fabsf(angle);
        pNode->setPosition(startPoint);
        auto ball = CCLoadSprite::createSprite("skill_fly_ball.png");
        ball->setOpacity(0);
        pNode->addChild(ball,1000);
        CCSequence* sc1 = CCSequence::create(CCDelayTime::create(0.2),CCFadeIn::create(0.1),CCScaleTo::create(0.5, 0.3),NULL);
        CCSequence* sc2 = CCSequence::create(CCDelayTime::create(0.2),CCFadeIn::create(0.1),CCFadeOut::create(0.5),NULL);
        CCSpawn* swp = CCSpawn::create(sc1,sc2,NULL);
        ball->runAction(swp);
        
        ccBezierConfig  beziercofig;
        int rand = GlobalData::shared()->getRand(1,100);
        if(rand%2==0){
            beziercofig.controlPoint_1 = ccp(m_flyX+GlobalData::shared()->getRand(30,60),m_flyY);
            beziercofig.controlPoint_2 = ccp(toX+GlobalData::shared()->getRand(30,60),toY);
        }else{
            beziercofig.controlPoint_1 = ccp(m_flyX-GlobalData::shared()->getRand(30,60),m_flyY);
            beziercofig.controlPoint_2 = ccp(toX-GlobalData::shared()->getRand(30,60),toY);
        }
        if(angle2<15){
            angle = 0;
        }else{
            angle = 90-angle;
        }
        float useFactor = (toY - m_flyY)/400.0;
        useFactor = MAX(useFactor, 1);
        float useTime = delayTime + i*0.08;
        if(num>30){
            useTime = delayTime + i*0.07;
        }
        beziercofig.endPosition = endPoint;
        CCActionInterval *forward = CCBezierTo::create(useTime, beziercofig);
        CCSequence* s1 = CCSequence::create(forward ,CCDelayTime::create(0.05), NULL);
        CCSequence* s2 = CCSequence::create(CCRotateTo::create(useTime*0.7, angle),NULL);//
        CCSpawn* sp = CCSpawn::create(s1,s2,NULL);
        pNode->runAction(sp);
        m_aniContainer->addChild(pNode, 10000);
    }
}

void GeneralSkillListPopUpView::onAbilityResetClick(CCObject * pSender, Control::EventType pCCControlEvent){
    int cost = GlobalData::shared()->generalConfig.abilityResetGold;
    auto dialog = YesNoDialog::show(_lang_1("105454", CC_ITOA(cost)), CCCallFunc::create(this, callfunc_selector(GeneralSkillListPopUpView::confirmResetAbility)), cost);
    dialog->setYesText(_lang("105448").c_str());
}

void GeneralSkillListPopUpView::confirmResetAbility(){
    int cost = GlobalData::shared()->generalConfig.abilityResetGold;
    if(!CCCommonUtils::isEnoughResourceByType(Gold, cost)){
//        CCCommonUtils::flyText(_lang("104912"));
        YesNoDialog::gotoPayTips();
        return;
    }
    int index = 0;
    while(index < m_info->ability.size()){
        if(m_info->ability[index] == m_abilityId){
            GeneralAbilityResetCommand *cmd = new GeneralAbilityResetCommand(m_info->uuid, m_abilityId, index, m_abilityId);
            cmd->sendAndRelease();
            break;
        }
        index++;
    }
    GeneralManager::getInstance()->removeGeneralAbility(*m_info, m_abilityId);
    PopupViewController::getInstance()->goBackPopupView();
    this->closeSelf();
}

void GeneralSkillListPopUpView::addCell(GeneralSkillInfo *info){
    GeneralSkillCell *cell = GeneralSkillCell::create(info, m_info,m_aniContainer);
    cell->setTag(atoi(info->skillId.c_str()));
   
    cell->setPosition(getPointByIndex(info->x, info->y));
//    if (CCCommonUtils::isIosAndroidPad())
//    {
//        cell->setScale(2.0f);
//        cell->setPosition(getPointByIndex(info->x*2, info->y*2));
//    }
    m_scrollView->addChild(cell, 2);
    if(m_totalH > cell->getPositionY()){
        m_totalH = cell->getPositionY();
    }
}

void GeneralSkillListPopUpView::addLine(){
    auto add = [](const CCPoint &startPt, LinePath *path, CCNode *parent,int skillId){
        auto startRealPt = ccp((startPt.x) * gapX, -(startPt.y) * gapY);
        bool flag = false;
        int pNum = 0;
        while(!path->m_pathWalkOver){
            path->next();
            CCObject *obj;
            int count = 0;
            CCARRAY_FOREACH(path->getCurrentPathLine(), obj){
                PathLine *line = dynamic_cast<PathLine*>(obj);
                int x = startRealPt.x + (line->startPt.x - startPt.x) * gapX;
                int y = startRealPt.y - line->startPt.y * gapX + 40;
                int tag = skillId*100+ pNum*1000 + count;
                auto bgSprite = CCLoadSprite::createSprite("UI_Skill_line_01.png");//cd_di_02
                bgSprite->setTag(tag);
                bgSprite->setScaleX((gapX * 1.0 + 20)/ bgSprite->getContentSize().width);
                int z = -1;
                bool flagRound = false;
                if(int(line->startPt.y) != int(line->endPt.y)){
                    bgSprite->setRotation(90);
                    int dy = 10;
                    if(line->startPt.y == 0){
                        dy = -10;
                    }
                    //dy = 0;
                    y -= gapX / 2 + dy;
                    
                }else{
                    flag = true;
                    if(line->endPt.x < startPt.x){
                        x -= gapX / 2;
                    }else{
                        x += gapX / 2;
                    }
                    bgSprite->setScaleX((gapX * 1.0 + 5) / bgSprite->getContentSize().width);
                    z = 0;
                }
                bgSprite->setPosition(ccp(x, y));
                parent->addChild(bgSprite, z);
                bgSprite->setAnchorPoint(ccp(0.5, 0.5));
                if(!line->isOpen){
                    //CCCommonUtils::setSpriteGray(bgSprite, true);
                }else{
                    auto line = CCLoadSprite::createSprite("UI_Skill_line_03.png");
                    line->setScaleX((gapX * 1.0 +20)/ line->getContentSize().width);
                    line->setRotation(bgSprite->getRotation());
                    line->setPosition(bgSprite->getPosition());
                    parent->addChild(line, 1);
                    line->setAnchorPoint(ccp(0.5, 0.5));
                }
                count += 1;
            }
                pNum += 1;
        }
        if (flag) {
            auto round = CCLoadSprite::createSprite("UI_Skill_line_02.png");
            round->setPosition(ccp(320-20 - gapX*2, startRealPt.y-40));
            parent->addChild(round, 10);
            round->setAnchorPoint(ccp(0, 0.5));
            
            auto round2 = CCLoadSprite::createSprite("UI_Skill_line_02.png");
            round2->setPosition(ccp(320-23, startRealPt.y-40));
            parent->addChild(round2, 10);
            round2->setAnchorPoint(ccp(0, 0.5));
            
            auto round3 = CCLoadSprite::createSprite("UI_Skill_line_02.png");
            round3->setPosition(ccp(320+gapX*2-20, startRealPt.y-40));
            parent->addChild(round3, 10);
            round3->setAnchorPoint(ccp(0, 0.5));
        }
    };
    
    int i = 0;
    
    auto& arr = this->m_scrollView->getContainer()->getChildren();
    if(arr.size()==0) return ;
    int total = arr.size();
    vector<CCPoint> startVector;
    vector<CCPoint> endVector;
    vector<bool> relationVector;
    while(i < total)
    {
        GeneralSkillCell *cell = dynamic_cast<GeneralSkillCell*>(arr.at(i));
        startVector.push_back(ccp(cell->m_info->x, cell->m_info->y));
        auto &next = cell->m_info->next;
        if(next.size() > 0){
            int j = 0;
            while(j < next.size()){
                int skillId = atoi(next[j].c_str());
                GeneralSkillCell *nextCell = dynamic_cast<GeneralSkillCell*>(this->m_scrollView->getContainer()->getChildByTag(skillId));
                if(nextCell){
                    endVector.push_back(ccp(nextCell->m_info->x, nextCell->m_info->y));
                    if(cell->m_info->level!=cell->m_info->maxLevel){
                        relationVector.push_back(false);
                    }else{
                        relationVector.push_back(!nextCell->m_info->isLocked());
                    }
                }
                j++;
            }
        }
        LinePath *path = LinePath::create(startVector, endVector, relationVector);
        int skillId = atoi(cell->m_info->skillId.c_str());
        add(ccp(cell->m_info->x, cell->m_info->y), path, m_scrollView,skillId);
        i++;
        startVector.clear();
        endVector.clear();
        relationVector.clear();
    }
}

float GeneralSkillListPopUpView::addLineAni(int skillId){
    float delayTime = 0;
    for(int i=0;i<6;i++){
        for (int j=0; j<10; j++) {
            int tag = skillId*100+i*1000+j;
            auto node = this->m_scrollView->getContainer()->getChildByTag(tag);
            if(node){
                auto line = CCLoadSprite::createSprite("UI_Skill_line_03.png");
                float lineScale = (gapX * 1.0 +20)/ line->getContentSize().width;
                line->setScaleX(0.1);
                line->setOpacity(0);
                line->setRotation(node->getRotation());
                if(node->getRotation()==90){
                    line->setAnchorPoint(ccp(0, 0.5));
                    line->setPosition(ccp(node->getPositionX(),node->getPositionY() + (gapX * 1.0 +20)/2));
                }else{
                    CCLOG("node->getPositionX()=%f",node->getPositionX());
                    line->setAnchorPoint(node->getAnchorPoint());
                    line->setPosition(node->getPosition());
                }
                node->getParent()->addChild(line, 1);
                line->runAction(CCSequence::create(CCDelayTime::create(i*0.65),CCFadeIn::create(0.01),CCScaleTo::create(0.5, lineScale,1),NULL));
                delayTime = i*0.65;
            }
        }
    }
    return delayTime;
}

CCPoint GeneralSkillListPopUpView::getPointByIndex(int x, int y){
    return ccp(startX + (x - 1) * gapX - 82, -40-(y) * gapY);
}

void GeneralSkillListPopUpView::setBtnState(){
    this->m_skillResetBtn->setEnabled(true);
    int total = m_info->getUsePoint("50000");
    total += m_info->getUsePoint("50001");
    total += m_info->getUsePoint("50002");
    if(total<=0){
        this->m_skillResetBtn->setEnabled(false);
    }
}

void GeneralSkillListPopUpView::delayPlayAni(float _time){
    //CCCallFuncO* fun2 = CCCallFuncO::create(this, callfuncO_selector(UIComponent::playPowerAniEnd2), NULL);

    CCActionInterval * delayM1 = CCDelayTime::create(0.05);
    CCActionInterval * scaleToM1 = CCScaleTo::create(0.1, 1.2,1.1);
    CCActionInterval * scaleToM2 = CCScaleTo::create(0.1, 1.0,1.0);
    CCActionInterval * seqM = CCSequence::create(scaleToM1, scaleToM2, NULL);
    CCActionInterval * seqM_RP = CCRepeat::create(seqM, 4);
    m_expTxt->runAction(CCSequence::create(delayM1, seqM_RP, CCDelayTime::create(0.3), NULL));
    m_countNum = 5;
    this->unschedule(schedule_selector(GeneralSkillListPopUpView::playCountPoint));
    this->schedule(schedule_selector(GeneralSkillListPopUpView::playCountPoint), 0.3);
    this->playCountPoint(0);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GENERAL_SKILL_UPDATE);
}

void GeneralSkillListPopUpView::playCountPoint(float _time){
    m_lastPoint += 1;
    int maxPoint = GeneralManager::getInstance()->getTotalFreeSkillPoint(*m_info);
    if(m_lastPoint>=maxPoint){
        m_lastPoint = maxPoint;
    }
    m_expTxt->setString(_lang_1("105032", CC_ITOA(m_lastPoint)));
    if(m_countNum<=0){
        this->unschedule(schedule_selector(GeneralSkillListPopUpView::playCountPoint));
        m_expTxt->setString(_lang_1("105032", CC_ITOA(maxPoint)));
        m_aniContainer->removeAllChildrenWithCleanup(true);
    }
    m_countNum -= 1;
}
//------------

GeneralSkillCell *GeneralSkillCell::create(GeneralSkillInfo *info, GeneralInfo *gInfo,CCNode* node,bool require){
    GeneralSkillCell *ret = new GeneralSkillCell(info, gInfo,node,require);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GeneralSkillCell::init(){
    auto bg = CCBLoadFile("Generaltianfu_01", this, this);
    this->setContentSize(bg->getContentSize());
    string bgFile = "";
    if(m_info->abilityId=="50000"){
        bgFile = "UI_Skill_icon_frame_red.png";
    }else if (m_info->abilityId=="50001"){
        bgFile = "UI_Skill_icon_frame_blue.png";
    }else{
        bgFile = "UI_Skill_icon_frame_green.png";
    }
    m_skillItemBg->initWithSpriteFrame(CCLoadSprite::loadResource(bgFile.c_str()));
    string icon = CCCommonUtils::getIcon(m_info->skillId);
    auto sprite = CCLoadSprite::createSprite(icon.c_str());
    this->m_picNode->addChild(sprite);
    //sprite->setScale(120.0f / sprite->getContentSize().width);
    isPlayingAni = false;
    isOpened = false;
    //this->m_nameLabel->setString(CCCommonUtils::getNameById(m_info->skillId).c_str());
    m_lvLabel->setString(std::string("") + CC_ITOA(m_info->level) + "/" + CC_ITOA(m_info->maxLevel));
    if(!m_info->isLocked() && !m_isRequire){
        m_lvLabel->setColor({255,255,0});
        if(m_info->level > 0){
            isOpened = true;
            //this->getAnimationManager()->runAnimationsForSequenceNamed("Default");
        }else{
            if(GeneralManager::getInstance()->isSkillClick(m_ginfo->uuid, m_info->skillId)){
                isOpened = true;
                //this->getAnimationManager()->runAnimationsForSequenceNamed("Default");
            }else{
                //this->getAnimationManager()->runAnimationsForSequenceNamed("Lock");

            }
        }
        m_particleNode->removeAllChildrenWithCleanup(true);
        
        if(m_info->type>=10){// && m_info->type<16
            string parFile = "SkillIconGlowR_";
            if (m_info->abilityId=="50001") {
                parFile = "SkillIconGlowB_";
            }else if(m_info->abilityId=="50002"){
                parFile = "SkillIconGlowG_";
            }
            for(int i=1;i<5;i++){
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",parFile.c_str(),i)->getCString());
                m_particleNode->addChild(particle);
            }
        }
    }else{
        m_nameLabel->setColor(ccGRAY);
        m_lvLabel->setColor(ccGRAY);
        CCCommonUtils::setSpriteGray(sprite, true);
        CCCommonUtils::setSpriteGray(m_skillItemBg, true);
        m_particleNode->removeAllChildrenWithCleanup(true);
        if(m_info->type>=10){// && m_info->type<16
            for(int i=1;i<5;i++){
                auto particle = ParticleController::createParticle(CCString::createWithFormat("SkillIconGlowW_%d",i)->getCString());
                m_particleNode->addChild(particle);
            }
        }
    }
    if(m_isRequire){
        m_lvNode->setVisible(false);
        this->m_nameLabel->setString(_lang_1("102272", CC_ITOA(m_info->maxLevel)));
    }
    return true;
}

void GeneralSkillCell::onEnter(){
    CCNode::onEnter();
    setSwallowsTouches(false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GeneralSkillCell::flyBallAni), "Skill_Reset_Effect", NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GeneralSkillCell::refresh), SAVE_SKILL_SUCCESS, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GeneralSkillCell::refresh), SAVE_SKILL_FAIL, NULL);
}

void GeneralSkillCell::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "Skill_Reset_Effect");
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, SAVE_SKILL_SUCCESS);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, SAVE_SKILL_FAIL);
    CCNode::onExit();
}

void GeneralSkillCell::refresh(CCObject* param){
    CCString* skillObj = dynamic_cast<CCString*>(param);
    if(skillObj){
        string temp = skillObj->getCString();
        if(m_info!=NULL && temp==m_info->skillId){
            m_lvLabel->setString(std::string("") + CC_ITOA(m_info->level) + "/" + CC_ITOA(m_info->maxLevel));
        }
    }
}

void GeneralSkillCell::flyBallAni(CCObject* obj){
    if(m_info->level<=0){
        return ;
    }
    m_resetLv = m_info->level;
    CCPoint pos = m_pNode->convertToNodeSpace(CCPoint(ccp(320, m_pNode->getContentSize().height+150)));
    CCPoint worldPos = this->getParent()->convertToWorldSpace(ccp(this->getPosition().x+90,this->getPosition().y+110));
    CCPoint startPoint = m_pNode->convertToNodeSpace(worldPos);
    auto pNode = CCNode::create();
    pNode->setPosition(startPoint);
    string tmpStart = "CrownUp_";
    int count = 8;
    for (int i=1; i<=count; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        particle->setPositionType(kCCPositionTypeRelative);
        pNode->addChild(particle);
    }
    m_pNode->addChild(pNode, 10000);
    this->scheduleOnce(schedule_selector(GeneralSkillCell::playAni), 0.4);
}

void GeneralSkillCell::playAni(float _time){
    CCPoint pos = m_pNode->convertToNodeSpace(CCPoint(ccp(320, m_pNode->getContentSize().height+150)));
    CCPoint worldPos = this->getParent()->convertToWorldSpace(ccp(this->getPosition().x+90,this->getPosition().y+110));
    CCPoint startPoint = m_pNode->convertToNodeSpace(worldPos);
    int m_flyX = startPoint.x;
    int m_flyY = startPoint.y;
    
    int toX = pos.x;
    int toY = pos.y;
    for(int i=0;i<m_resetLv;i++){
        auto pNode = CCNode::create();
        CCPoint endPoint = ccp(toX, toY);
        float angle=CCMathUtils::getAngle(startPoint, endPoint);
        float angle2 = fabsf(angle);
        pNode->setPosition(startPoint);
        auto ball = CCLoadSprite::createSprite("skill_fly_ball.png");
        pNode->addChild(ball,1000);
        CCSequence* sc1 = CCSequence::create(CCDelayTime::create(0.3),CCScaleTo::create(0.5, 0.3),NULL);
        CCSequence* sc2 = CCSequence::create(CCDelayTime::create(0.3),CCFadeOut::create(0.5),NULL);
        CCSpawn* swp = CCSpawn::create(sc1,sc2,NULL);
        ball->runAction(swp);
        
        ccBezierConfig  beziercofig;
        int rand = GlobalData::shared()->getRand(1,100);
        if(rand%2==0){
            beziercofig.controlPoint_1 = ccp(m_flyX+GlobalData::shared()->getRand(30,60),m_flyY);
            beziercofig.controlPoint_2 = ccp(toX+GlobalData::shared()->getRand(30,60),toY);
        }else{
            beziercofig.controlPoint_1 = ccp(m_flyX-GlobalData::shared()->getRand(30,60),m_flyY);
            beziercofig.controlPoint_2 = ccp(toX-GlobalData::shared()->getRand(30,60),toY);
        }
        if(angle2<15){
            angle = 0;
        }else{
            angle = 90-angle;
        }
        float useFactor = (toY - m_flyY)/400.0;
        useFactor = MAX(useFactor, 1);
        float useTime = 0.5 + i*0.04;
        if(m_resetLv>10){
            useTime = 0.35 + i*0.04;
        }
        beziercofig.endPosition = endPoint;
        CCActionInterval *forward = CCBezierTo::create(useTime, beziercofig);
        //auto func = CCCallFuncO::create(this, callfuncO_selector(ImperialScene::onFlyTroopEnd), pNode);
        CCSequence* s1 = CCSequence::create(forward ,CCDelayTime::create(0.05), NULL);
        CCSequence* s2 = CCSequence::create(CCRotateTo::create(useTime*0.7, angle),NULL);//
        CCSpawn* sp = CCSpawn::create(s1,s2,NULL);
        pNode->runAction(sp);
        m_pNode->addChild(pNode, 10000);
    }
}

bool GeneralSkillCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleNode", CCNode*, m_particleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchLayer", CCNode*, m_touchLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvLabel", CCLabelIF*, m_lvLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_skillItemBg", CCSprite*, m_skillItemBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvNode", CCNode*, m_lvNode);
    return false;
}

bool GeneralSkillCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(this->m_touchLayer, pTouch)){
        return true;
    }
    return false;
}

void GeneralSkillCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(isPlayingAni){
        return;
    }

    if(abs(int(pTouch->getStartLocation().y - pTouch->getLocation().y)) > 12){
        return;
    }
    if(!isTouchInside(this->getParent()->getParent()->getParent(), pTouch)){
        return;
    }
    map<std::string, GeneralInfo>::iterator it = GlobalData::shared()->generals.find(m_ginfo->uuid);
    if(it == GlobalData::shared()->generals.end()){
        return;
    }
    clickOnCell();
    return ;
//    std::string str = this->getAnimationManager()->getLastCompletedSequenceName();
//    if(!isOpened && !m_info->isLocked()){
//        GeneralManager::getInstance()->saveSkillClick(m_ginfo->uuid, m_info->skillId);
//        m_particleNode->removeAllChildren();
//        CCParticleBatchNode *batch = ParticleController::createParticleBatch();
//        string tmpStart = "Magic_Click_";
//        int count = 2;
//        for (int i=1; i<count; i++) {
//            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
//            batch->addChild(particle);
//        }
//        m_particleNode->addChild(batch);
//
//        isPlayingAni = true;
//        this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(GeneralSkillCell::animationFunc));
//        //this->getAnimationManager()->runAnimationsForSequenceNamed("LockOut");
//        //scheduleOnce(schedule_selector(GeneralSkillCell::addBoomb), 2.0f);
//
//    }else{
//        clickOnCell();
//    }

}

void GeneralSkillCell::addBoomb(float _time){
    string tmpStart = "ScienceOut_";
    int count = 9;
    for (int i=1; i<count; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        m_particleNode->addChild(particle);
    }
}

void GeneralSkillCell::animationFunc(){
    isOpened = true;
    isPlayingAni = false;
    m_particleNode->removeAllChildren();
    this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
}

void GeneralSkillCell::clickOnCell(){
    //this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    PopupViewController::getInstance()->addPopupView(GeneralSkillUpdatePopUpView::create(m_info, m_ginfo));

}

void GeneralSkillCell::showUnLockEffect(float delayTime){
    this->scheduleOnce(schedule_selector(GeneralSkillCell::addBoomb), delayTime);
//    string tmpStart = "ScienceOut_";
//    int count = 9;
//    for (int i=1; i<count; i++) {
//        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
//        m_particleNode->addChild(particle);
//    }
}

//--------------------

GeneralSkillUpdatePopUpView *GeneralSkillUpdatePopUpView::create(GeneralSkillInfo *info, GeneralInfo *gInfo){
    GeneralSkillUpdatePopUpView *ret = new GeneralSkillUpdatePopUpView(info, gInfo);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GeneralSkillUpdatePopUpView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(105, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(7, false);
        });
        
        auto bg = CCBLoadFile("TIPS_wujiang_02.ccbi", this, this);
        this->setContentSize(bg->getContentSize());
        string bgFile = "";
        if(m_info->abilityId=="50000"){
            bgFile = "UI_Skill_icon_frame_red.png";
        }else if (m_info->abilityId=="50001"){
            bgFile = "UI_Skill_icon_frame_blue.png";
        }else{
            bgFile = "UI_Skill_icon_frame_green.png";
        }
        m_skillItemBg->initWithSpriteFrame(CCLoadSprite::loadResource(bgFile.c_str()));
        
        this->m_titleText->setString(CCCommonUtils::getNameById(m_info->skillId).c_str());
        string dialog = CCCommonUtils::getPropById(m_info->skillId, "description");
        std::string type = CCCommonUtils::getPropById(m_info->skillId, "type");
        if(type=="12" || type=="13" || type=="15"){// || type=="17" || type=="20"
            std::string base = CCCommonUtils::getPropById(m_info->skillId, "base");
            m_descriptionText->setString(_lang_1(dialog.c_str(),base.c_str()));
        }else if (type=="14"){// || type=="18" || type=="19"
            std::string base = CCCommonUtils::getPropById(m_info->skillId, "base");
            vector<std::string> vector;
            CCCommonUtils::splitString(base, "|", vector);
            if(vector.size()==2){
                m_descriptionText->setString(_lang_2(dialog.c_str(),vector[0].c_str(),vector[1].c_str()));
            }
        }else{
            m_descriptionText->setString(_lang(dialog.c_str()));
        }

        CCCommonUtils::setButtonTitle(m_updateBtn, _lang("105445").c_str());
        this->m_picContainer->removeAllChildren();
        string icon = CCCommonUtils::getIcon(m_info->skillId);
        auto sprite = CCLoadSprite::createSprite(icon.c_str());
        this->m_picContainer->addChild(sprite);
        refreshView();
        m_initLv = m_info->level;
        ret = true;
    }
    return ret;
}

void GeneralSkillUpdatePopUpView::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GeneralSkillUpdatePopUpView::saveSuccess), SAVE_SKILL_SUCCESS, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GeneralSkillUpdatePopUpView::saveSuccess), SAVE_SKILL_FAIL, NULL);
}

void GeneralSkillUpdatePopUpView::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, SAVE_SKILL_SUCCESS);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, SAVE_SKILL_FAIL);
    if(m_initLv!=m_info->level && m_info->level==m_info->maxLevel){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GENERAL_UNLOCK_SKILL_UPDATE,CCString::create(m_info->skillId));
    }else if(m_initLv!=m_info->level){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GENERAL_SKILL_UPDATE);
    }
    PopupBaseView::onExit();
}

bool GeneralSkillUpdatePopUpView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    return true;
}

void GeneralSkillUpdatePopUpView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (!isTouchInside(m_clickArea, pTouch)) {
        this->closeSelf();
    }
}

bool GeneralSkillUpdatePopUpView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_updateBtn", CCControlButton*, m_updateBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleText", CCLabelIF*, m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_descriptionText", CCLabelIF*, m_descriptionText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_attAddText", CCLabelIF*, m_attAddText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvText", CCLabelIF*, m_lvText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintText", CCLabelIF*, m_hintText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picContainer", CCNode*, m_picContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextLVText", CCLabelIF*, m_nextLVText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_curProTxt", CCLabelIF*, m_curProTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addTxt", CCLabelIF*, m_addTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numTxt", CCLabelIF*, m_numTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickArea", CCNode*, m_clickArea);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_proNode", CCNode*, m_proNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progress", CCScale9Sprite*, m_progress);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextPro", CCScale9Sprite*, m_nextPro);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_skillItemBg", CCSprite*, m_skillItemBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_currentNode", CCNode*, m_currentNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextNode", CCNode*, m_nextNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_curRomanNode", CCNode*, m_curRomanNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextRomanNode", CCNode*, m_nextRomanNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lockNode", CCNode*, m_lockNode);
    return false;
}

SEL_CCControlHandler GeneralSkillUpdatePopUpView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onUpdateClick", GeneralSkillUpdatePopUpView::onUpdateClick);    
    return NULL;
}

void GeneralSkillUpdatePopUpView::onUpdateClick(CCObject * pSender, Control::EventType pCCControlEvent){
    if (pCCControlEvent==CCControlEventTouchDown) {//按下
        schedule(schedule_selector(GeneralSkillUpdatePopUpView::UpgradeSkill), 0.3, kCCRepeatForever, 0.8);
    }else if (pCCControlEvent==CCControlEventTouchUpInside){//抬起
        unschedule(schedule_selector(GeneralSkillUpdatePopUpView::UpgradeSkill));
        if (m_isLongPressUpBtn) {
            m_isLongPressUpBtn=false;
            if(GeneralManager::getInstance()->getTotalFreeSkillPoint(*m_ginfo) < m_info->getUpgradeSkillPointNeed()){
                m_updateBtn->setEnabled(false);
            }else{
                m_updateBtn->setEnabled(true);
            }
        }else{
            if(m_info->level >= m_info->maxLevel){
                return;
            }
            if(GeneralManager::getInstance()->getTotalFreeSkillPoint(*m_ginfo) < m_info->getUpgradeSkillPointNeed()){
                m_updateBtn->setEnabled(false);
                return;
            }
            m_info->level = m_info->level+1;
            m_updateBtn->setEnabled(false);
            GeneralManager::getInstance()->saveSkill(m_info->abilityId, m_info->skillId);
//            saveSuccess(NULL);
        }
    }
}

void GeneralSkillUpdatePopUpView::UpgradeSkill(float f){
    m_isLongPressUpBtn=true;
    if(m_info->level >= m_info->maxLevel){
        return;
    }
    if(GeneralManager::getInstance()->getTotalFreeSkillPoint(*m_ginfo) < m_info->getUpgradeSkillPointNeed()){
        m_updateBtn->setEnabled(false);
        return;
    }
    m_info->level = m_info->level+1;
//    m_updateBtn->setEnabled(false);
    GeneralManager::getInstance()->saveSkill(m_info->abilityId, m_info->skillId);
//    saveSuccess(NULL);
}

void GeneralSkillUpdatePopUpView::saveSuccess(CCObject* param){
    refreshView();
    //this->unschedule(schedule_selector(GeneralSkillUpdatePopUpView::cdClick));
    //this->schedule(schedule_selector(GeneralSkillUpdatePopUpView::cdClick),0.4);
    cdClick(0);
}

void GeneralSkillUpdatePopUpView::cdClick(float _time){
    this->unschedule(schedule_selector(GeneralSkillUpdatePopUpView::cdClick));
    if(m_info->level >= m_info->maxLevel){
        this->closeSelf();
        return;
    }
    if(GeneralManager::getInstance()->getTotalFreeSkillPoint(*m_ginfo) < m_info->getUpgradeSkillPointNeed()){
        m_updateBtn->setEnabled(false);
        return;
    }
    m_updateBtn->setEnabled(true);
}

void GeneralSkillUpdatePopUpView::refreshView(){
    setBtnState();
    setText();
}

void GeneralSkillUpdatePopUpView::setText(){
    std::string lvStr = CC_ITOA(m_info->level);
    this->m_lvText->setString(lvStr.c_str());
    if((m_info->level+1)>=m_info->maxLevel){
        this->m_nextLVText->setString(CC_ITOA(m_info->maxLevel));
    }else{
        this->m_nextLVText->setString(CC_ITOA(m_info->level+1));
    }
    
    
//    std::string str = "";
//    std::string str1 = "";
//    std::string str2 = "";
//    
//    auto getEffectStr = [](GeneralSkillInfo *info, int index, bool isNextLevel){
//        if(info->level == 0 && !isNextLevel){
//            return std::string("");
//        }
//        if(info->level >= info->maxLevel && isNextLevel){
//            return std::string("");
//        }
//        double baseValue = info->base[index];
//        double add = info->add[index];
//        double effectValue = baseValue + (info->level - 1) * add;
//        if(isNextLevel){
//            effectValue = baseValue + info->level * add;
//        }
//        std::string effectStr = "";
//        if(info->showType == 0){
//            effectValue = effectValue * 1.0f / 100.0f;
//            effectStr = CC_ITOA_1(effectValue);
//            effectStr = effectStr + "%";
//        }else{
//            effectStr = CC_ITOA(effectValue);
//        }
//        return effectStr;
//    };
//    
//    std::string dialogId = "";
//    if(m_info->des.size()>=1){
//         dialogId = m_info->des[0];
//    }
//    vector<std::string> currentLevel;
//    vector<std::string> nextLevel;
//    int index = 0;
//    while (index < m_info->base.size()) {
//        std::string currentStr = getEffectStr(m_info, index, false);
//        std::string nextStr = getEffectStr(m_info, index, true);
//        if(currentStr != ""){
//            currentLevel.push_back(currentStr);
//        }
//        if(nextStr != ""){
//            nextLevel.push_back(nextStr);
//        }
//        index++;
//    }
//    if(currentLevel.size() == 1){
//        str1 = _lang_1(dialogId, currentLevel[0].c_str());
//    }else if(currentLevel.size() == 2){
//        str1 = _lang_2(dialogId, currentLevel[0].c_str(), currentLevel[1].c_str());
//    }else if(currentLevel.size() == 3){
//        str1 = _lang_3(dialogId, currentLevel[0].c_str(), currentLevel[1].c_str(), currentLevel[2].c_str());
//    }
//    if(nextLevel.size() == 1){
//        str2 = _lang_1(dialogId, nextLevel[0].c_str());
//    }else if(nextLevel.size() == 2){
//        str2 = _lang_2(dialogId, nextLevel[0].c_str(), nextLevel[1].c_str());
//    }else if(nextLevel.size() == 3){
//        str2 = _lang_3(dialogId, nextLevel[0].c_str(), nextLevel[1].c_str(), nextLevel[2].c_str());
//    }
// 
//    
//    if(str1 != ""){
//        str += _lang("105455") + ": " + str1 + "\n";
//    }
//    if(str2 != ""){
//        str += _lang("105456") + ": " + str2;
//    }

    //this->m_attAddText->setString(str.c_str());
    string add = CCCommonUtils::getPropById(m_info->skillId, "add");
    double addValue = atof(add.c_str());
    double maxLv = m_info->maxLevel*addValue;
    double current = addValue*m_info->level;
    string currentStr = CC_ITOA(current);
    string addStr = "+";
    if(m_info->showType == 0){
        currentStr = CC_ITOA_1(current/100.0);
        currentStr.append("%");
        addStr.append(CC_ITOA_1(addValue/100.0));
        addStr.append("%");
    }else{
        addStr.append(add.c_str());
    }
    this->m_curProTxt->setString(currentStr);
    this->m_addTxt->setString(addStr);
    float len = current*1.0/maxLv;
    if(len>1) len = 1.0;
    m_progress->setContentSize(CCSize(338.0*len,23));
    
    double next = addValue*(m_info->level+1);
    float nextLen = next*1.0/maxLv;
    if(nextLen>1) nextLen = 1.0;
    m_nextPro->setContentSize(CCSize(338.0*nextLen,23));
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_progress->setContentSize(CCSize(338.0*len,46));
        m_nextPro->setContentSize(CCSize(338.0*nextLen,46));
    }
    if(m_info->type>=10){// && m_info->type<16
        m_proNode->setVisible(false);
    }else{
        m_proNode->setVisible(true);
    }
    
    m_currentNode->removeAllChildrenWithCleanup(true);
    m_nextNode->removeAllChildrenWithCleanup(true);
    m_nextRomanNode->setVisible(true);
    m_nextRomanNode->setVisible(true);
    m_curRomanNode->setPositionX(-65.0);
    m_nextRomanNode->setPositionX(56);
    int nextLv = m_info->level+1;
    if(m_info->level>=m_info->maxLevel){
        nextLv = m_info->maxLevel;
        m_curRomanNode->setVisible(false);
        m_nextRomanNode->setPositionX(4);
        this->m_addTxt->setString("");
    }
    if(m_info->isLocked()){
        m_proNode->setVisible(false);
        m_nextRomanNode->setVisible(false);
        m_curRomanNode->setVisible(false);
        m_hintText->setPositionY(20);
        
    }else{
//        m_proNode->setVisible(true);
        m_nextRomanNode->setVisible(true);
        m_curRomanNode->setVisible(true);
        m_hintText->setPositionY(-131.6);
        if(m_info->type>=10 ){//&& m_info->type<16
            m_proNode->setVisible(false);
        }else{
            m_proNode->setVisible(true);
        }
        if(m_info->level>=m_info->maxLevel){
            nextLv = m_info->maxLevel;
            m_curRomanNode->setVisible(false);
        }
    }
}

void GeneralSkillUpdatePopUpView::setBtnState(){
    m_hintText->setString("");
    m_lockNode->removeAllChildrenWithCleanup(true);
    if(m_info->level >= m_info->maxLevel){
        m_updateBtn->setVisible(false);
    }else if(m_info->isLocked()){
        m_updateBtn->setVisible(false);
        std::string hintStr = "";
        std::string conditionSkill = CCCommonUtils::getPropById(m_info->skillId, "condition_skill");
        if(conditionSkill != ""){
            vector<std::string> vector1;
            vector<std::string> vector;
            CCCommonUtils::splitString(conditionSkill, "|", vector);
            int i = 0;
            while(i < vector.size()){
                vector1.clear();
                CCCommonUtils::splitString(vector[i], ";", vector1);
                std::string requiredskillId = vector1[0];
                GeneralSkillInfo *skillInfo = m_ginfo->getSkill(m_info->abilityId, requiredskillId);
                if(skillInfo){
                    GeneralSkillCell *cell = GeneralSkillCell::create(skillInfo, &GlobalData::shared()->generals.begin()->second,m_lockNode,true);
                    cell->setPosition(i*150, 0);
                    m_lockNode->addChild(cell);
                }
                i++;
            }
            int num = vector.size();
            m_lockNode->setPositionX(m_lockNode->getPositionX() - num*150/2);
            vector.clear();
            vector1.clear();
        }
        m_hintText->setString(_lang("118007"));
    }else{
        if(GeneralManager::getInstance()->getTotalFreeSkillPoint(*m_ginfo) < m_info->getUpgradeSkillPointNeed()){
            m_updateBtn->setEnabled(false);
        }else{
            m_updateBtn->setEnabled(true);
        }
    }
}

//-----

LinePath *LinePath::create(vector<CCPoint> &starts, vector<CCPoint> &ends, vector<bool> &relation){
    LinePath *ret = new LinePath();
    if(ret && ret->init(starts, ends, relation)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

CCArray *LinePath::getCurrentPathLine(){
    return m_currentPathLines;
}

bool LinePath::init(vector<CCPoint> &starts, vector<CCPoint> &ends, vector<bool> &relation){
    m_currentPathLines = CCArray::create();
    m_allLines.clear();
    m_startLines = CCArray::create();
    m_pathWalkOver = false;
    generatePath(starts, ends, relation);
    return true;
}

void LinePath::generatePath(vector<CCPoint> &starts, vector<CCPoint> &ends, vector<bool> &relation){
    if(starts.size() > 1 && ends.size() > 1){
        CCLOG("path error");
        m_pathWalkOver = true;
        return;
    }
    if(starts.size() == 0 || ends.size() == 0){
        CCLOG("path empty error");
        m_pathWalkOver = true;
        return;
    }
    int startIndex = 0;
    int endIndex = 0;
    while(startIndex < starts.size()){
        endIndex = 0;
        auto startPt = starts[startIndex];
        while(endIndex < ends.size()){
            auto endPt = ends[endIndex];
            bool flag = relation[endIndex];
            PathLine *lineStart = PathLine::create();
            lineStart->startPt = ccp(startPt.x, 0);
            lineStart->endPt = ccp(startPt.x, 1);
            lineStart->endY = endPt.y;
            if(!lineStart->isOpen){
                lineStart->isOpen = flag;
            }
            addLineToMap(lineStart);
            if(endIndex == 0){
                m_startLines->addObject(lineStart);
            }
            int lineStartX = startPt.x;
            int lineEndX = endPt.x;
            while(lineStartX != lineEndX){
                PathLine *line = PathLine::create();
                line->startPt = ccp(lineStartX, 1);
                if(lineStartX > lineEndX){
                    lineStartX--;
                }else{
                    lineStartX++;
                }
                line->endPt = ccp(lineStartX, 1);
                line->endY = endPt.y;
                line->isOpen = flag;
                addLineToMap(line);
            }
            
            PathLine *lineEnd = PathLine::create();
            lineEnd->startPt = ccp(endPt.x, 1);
            lineEnd->endPt = ccp(endPt.x, 2);
            lineEnd->endY = endPt.y;
            lineEnd->isOpen = flag;

            addLineToMap(lineEnd);

            endIndex++;
        }
        startIndex++;
    }
}

void LinePath::addLineToMap(PathLine *line){
    int x = line->startPt.x;
    int y = line->startPt.y;
    int index = x + numPerRow * y;
    map<int, CCArray*>::iterator it = m_allLines.find(index);
    if(it != m_allLines.end()){
        CCObject *obj;
        CCARRAY_FOREACH(it->second, obj){
            PathLine *tmp = dynamic_cast<PathLine*>(obj);
            if(int(tmp->startPt.x) == int(line->startPt.x) &&
               int(tmp->startPt.y) == int(line->startPt.y) &&
               int(tmp->endPt.x) == int(line->endPt.x) &&
               int(tmp->endPt.y) == int(line->endPt.y)
               ){
                return;
            }
        }
        it->second->addObject(line);
    }else{
        CCArray *arr = CCArray::create();
        arr->retain();
        arr->addObject(line);
        m_allLines[index] = arr;
    }
}

void LinePath::next(){
    if(m_currentPathLines->count() == 0){
        m_currentPathLines->addObjectsFromArray(m_startLines);
    }else{
        CCArray *tmp = CCArray::create();
        CCObject *obj;
        CCARRAY_FOREACH(m_currentPathLines, obj){
            PathLine *line = dynamic_cast<PathLine*>(obj);
            int endX = line->endPt.x;
            int endY = line->endPt.y;
            int index = endX + numPerRow * endY;
            map<int, CCArray*>::iterator it = m_allLines.find(index);
            if(it != m_allLines.end()){
                tmp->addObjectsFromArray(it->second);
            }
        }
        m_currentPathLines->removeAllObjects();
        m_currentPathLines->addObjectsFromArray(tmp);
        if(m_currentPathLines->count() == 0){
            m_pathWalkOver = true;
        }
        tmp->removeAllObjects();
    }
}

LinePath::~LinePath(){
    map<int, CCArray*>::iterator it;
    for(it = m_allLines.begin(); it != m_allLines.end(); it++){
        it->second->removeAllObjects();
        it->second->release();
    }
    m_allLines.clear();
}