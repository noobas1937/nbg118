//
//  UseSkillPopUpView.cpp
//  IF
//
//  Created by ganxiaohua on 14-11-3.
//
//

#include "UseSkillPopUpView.h"

#include "PopupViewController.h"
#include "RewardController.h"
#include "UIComponent.h"
#include "ChangePicCommand.h"
#include "ToolController.h"
#include "UseToolView.h"
#include "UseSkillCommand.h"
#include "HarvestSkillAni.h"
#include "UIComponent.h"
#include "SceneController.h"
#include "GeneralManager.h"
#include "FunBuildController.h"
#include "GeneralSkillListPopUpView.h"
#include "YesNoDialog.h"

#define cellW (CCCommonUtils::isIosAndroidPad()? 195 : 115)

UseSkillPopUpView *UseSkillPopUpView::create(){
    UseSkillPopUpView *ret = new UseSkillPopUpView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void UseSkillPopUpView::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UseSkillPopUpView::onPicClick), "on_skill_click", NULL);
    this->refreshView();
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
}

void UseSkillPopUpView::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "on_skill_click");
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool UseSkillPopUpView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    return true;
}

void UseSkillPopUpView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_viewBg, pTouch)){
    }else if(m_useSkillNode->isVisible() && isTouchInside(m_clickArea, pTouch)){
    }else{
        PopupViewController::getInstance()->removePopupView(this);
    }
}

bool UseSkillPopUpView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(209, true);
        CCLoadSprite::doResourceByCommonIndex(105, true);
        CCLoadSprite::doResourceByCommonIndex(500, true);
         CCLoadSprite::doResourceByCommonIndex(7, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(209, false);
            CCLoadSprite::doResourceByCommonIndex(105, false);
            CCLoadSprite::doResourceByCommonIndex(500, false);
            CCLoadSprite::doResourceByCommonIndex(7, true);
        });
        CCBLoadFile("UseSkillViewCCB", this, this);
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        m_data = CCArray::create();
        m_tabView = CCMultiColTableView::create(this, m_listContainer->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionHorizontal);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setMultiColTableViewDelegate(this);
        m_tabView->setTouchPriority(0);
         m_tabView->getContainer()->runAction(CCMoveBy::create(1.0, Vec2(0, 1)));
        m_listContainer->addChild(m_tabView);
        
        m_skillId = "";
        
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_moveNode->runAction(CCMoveTo::create(0.7, ccp(768, 550)));
        } else {
            m_moveNode->runAction(CCMoveTo::create(0.7, ccp(320, 330)));
        }
        
        CCDictElement *element;
        auto dictSK = LocalController::shared()->DBXMLManager()->getGroupByKey("sk");
        CCDICT_FOREACH(dictSK, element){
            std::string skillId = element->getStrKey();
            std::string type = CCCommonUtils::getPropById(skillId, "type");
            int st = atoi(type.c_str());
            if(st>=10){//&& st<=16
                m_data->addObject(CCString::create(skillId.c_str()));
            }
            //CCLOG("skillId %s type=%s",skillId.c_str(),type.c_str());
        }
        //技能排序
        int p=0;
        for (int i=0; i<m_data->count(); i++) {
            std::string skillId = dynamic_cast<CCString*>(m_data->objectAtIndex(i))->getCString();
            GeneralInfo* info = &GlobalData::shared()->generals.begin()->second;
            if (info->checkHaveStudy(skillId)){
                if (i>p) {
                    CCObject* saveObj = m_data->objectAtIndex(p);
                    m_data->replaceObjectAtIndex(p, m_data->objectAtIndex(i));
                    m_data->replaceObjectAtIndex(i, saveObj);
                }
                p++;
            }
        }
        //城市增益状态分组
        auto statusDictInfo = LocalController::shared()->DBXMLManager()->getGroupByKey("status");
        if(statusDictInfo)
        {
            CCDictElement* element;
            CCDICT_FOREACH(statusDictInfo, element)
            {
                string strId = element->getStrKey();
                CCDictionary* dictInfo = _dict(element->getObject());
                int groupId = dictInfo->valueForKey("group")->intValue();
                groupId = groupId/100;
                m_statusGroup[groupId].push_back(atoi(strId.c_str()));
            }
        }
            
        m_useSkillNode->setVisible(false);
        m_useNode->setVisible(false);
        CCCommonUtils::setButtonTitle(m_useBtn, _lang("105460").c_str());
        m_skillTitle->setString(_lang("118000"));
        m_tabView->reloadData();
        m_tipsTxt->setVisible(true);
        this->schedule(schedule_selector(UseSkillPopUpView::updateCDTime), 1);
        ret = true;
    }
    return ret;
}

void UseSkillPopUpView::onPicClick(CCObject *data){
    CCString* skillId = dynamic_cast<CCString*>(data);
    if(skillId){
        m_skillId = skillId->getCString();
        m_skillName->setString(CCCommonUtils::getNameById(skillId->getCString()));
        m_useSkillNode->setVisible(true);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_useSkillNode->setPositionY(1135);
        } else {
            m_useSkillNode->setPositionY(640);
        }
        m_useNode->setVisible(false);
        string dialog = CCCommonUtils::getPropById(m_skillId, "description");
        std::string type = CCCommonUtils::getPropById(m_skillId, "type");
        if(type=="12" || type=="13" || type=="15"){
            std::string base = CCCommonUtils::getPropById(m_skillId, "base");
            m_descTxt->setString(_lang_1(dialog.c_str(),base.c_str()));
        }else if (type=="14"){
            std::string base = CCCommonUtils::getPropById(m_skillId, "base");
            vector<std::string> vector;
            CCCommonUtils::splitString(base, "|", vector);
            if(vector.size()==2){
                m_descTxt->setString(_lang_2(dialog.c_str(),vector[0].c_str(),vector[1].c_str()));
            }
        }else{
            m_descTxt->setString(_lang(dialog.c_str()));
        }
        m_descTxt->setColor({96,49,0});
        std::string head = CCCommonUtils::getIcon(m_skillId);
        m_head->removeAllChildrenWithCleanup(true);
        CCSprite* icon = CCLoadSprite::createSprite(head.c_str());
        //CCCommonUtils::setSpriteMaxSize(icon, 80,true);
        m_head->addChild(icon);
        m_skillItemBg->initWithSpriteFrame(CCLoadSprite::loadResource(GeneralManager::getInstance()->getBgBySkill(m_skillId).c_str()));
        map<std::string, SkillCDInfo*>::iterator it = GeneralManager::getInstance()->SkillCDMap.find(m_skillId);
        double gapTime = 0;
        GeneralInfo* info = &GlobalData::shared()->generals.begin()->second;
        if (info->checkHaveStudy(m_skillId)) {
            if(it != GeneralManager::getInstance()->SkillCDMap.end() && it->second->endTime!=0){
                gapTime = it->second->endTime - GlobalData::shared()->getWorldTime();
                m_useBtn->setEnabled(gapTime<=0);
            }else{
                m_useBtn->setEnabled(true);
            }
            CCCommonUtils::setButtonTitle(m_useBtn, _lang("105460").c_str());
            CCCommonUtils::setSpriteGray(icon, false);
            CCCommonUtils::setSpriteGray(m_skillItemBg, false);
        }else{
            CCCommonUtils::setButtonTitle(m_useBtn, _lang("118006").c_str());
            m_useBtn->setEnabled(true);
            
            CCCommonUtils::setSpriteGray(icon, true);
            CCCommonUtils::setSpriteGray(m_skillItemBg, true);
            CCSprite* lock = CCLoadSprite::createSprite("UI_UseSkill_lock.png");
            m_head->addChild(lock);
        }
        updateCDTime(0.0);
        m_skillItemBg->setVisible(false);
    }
}

void UseSkillPopUpView::updateCDTime(float t){
    if (m_skillId=="") {
        return ;
    }
    map<std::string, SkillCDInfo*>::iterator it = GeneralManager::getInstance()->SkillCDMap.find(m_skillId);
    GeneralInfo* info = &GlobalData::shared()->generals.begin()->second;
    double gapTime = 0;
    if (info->checkHaveStudy(m_skillId)) {
        if(it != GeneralManager::getInstance()->SkillCDMap.end() && it->second->endTime!=0){
            gapTime = it->second->endTime - GlobalData::shared()->getWorldTime();
        }
        if (gapTime>0) {
            m_tipsTxt->setString(_lang_1("105459",CC_SECTOA(gapTime)));
        }else{
            int cdTime =atoi(CCCommonUtils::getPropById(m_skillId, "cd_time").c_str());
            if (cdTime==0) {
                if (m_skillId=="601000" || m_skillId=="602700") {
                    cdTime = 480;
                }else if (m_skillId=="610700" || m_skillId=="622300"){
                    cdTime = 720;
                }else if (m_skillId=="612000" || m_skillId=="622000"){
                    cdTime = 1440;
                }else if (m_skillId=="622100" || m_skillId=="622200"){
                    cdTime = 1080;
                }else if (m_skillId=="621900"){
                    cdTime = 240;
                }
            }
            gapTime=cdTime*60;
            m_tipsTxt->setString(_lang_1("105459",CC_SECTOA(gapTime)));
        }
    }else{
        m_tipsTxt->setString(_lang("118005"));
    }
}

void UseSkillPopUpView::onChangeClick(CCObject *pSender, CCControlEvent event){
//    UseSkillCommand* cmd = new UseSkillCommand("");
//    cmd->sendAndRelease();

}

void UseSkillPopUpView::refreshView(){
    
}

bool UseSkillPopUpView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_moveNode", CCNode*, this->m_moveNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_skillTitle", CCLabelIF*, this->m_skillTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_descTxt", CCLabelIF*, this->m_descTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipsTxt", CCLabelIF*, this->m_tipsTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_skillName", CCLabelIF*, this->m_skillName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_head", CCNode*, this->m_head);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_useSkillNode", CCNode*, this->m_useSkillNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_useBtn", CCControlButton*, this->m_useBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_useTxt", CCLabelIF*, this->m_useTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_useNode", CCNode*, this->m_useNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickArea", CCNode*, this->m_clickArea);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_skillItemBg", CCSprite*, this->m_skillItemBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_useBgNode", CCNode*, this->m_useBgNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_sprL", CCSprite*, this->m_sprL);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_sprR", CCSprite*, this->m_sprR);
    return false;
}

SEL_CCControlHandler UseSkillPopUpView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onUseClick", UseSkillPopUpView::onUseClick);
    return NULL;
}

void UseSkillPopUpView::onUseClick(CCObject *pSender, CCControlEvent event){
    GeneralInfo* info = &GlobalData::shared()->generals.begin()->second;
    if (info->checkHaveStudy(m_skillId)) {
        if(m_skillId=="622300"&&WorldController::getInstance()->currentStamine >= GlobalData::shared()->worldConfig.stamineMax){
            CCCommonUtils::flyHint("", "", _lang("103746"));
            return ;
        }
        //判断城市增益中有没有同组增益正在使用
        std::string actNumber = CCCommonUtils::getPropById(m_skillId, "para1");
        auto statusDict = LocalController::shared()->DBXMLManager()->getObjectByKey(actNumber);
        int overlap = statusDict->valueForKey("overlap")->intValue();
        int group = statusDict->valueForKey("group")->intValue();
        int gID = group/100;
        if (statusDict->count()>0&&group>0) {
            int findSid = 0;
            if (overlap==0) {
                if (m_statusGroup.find(gID) != m_statusGroup.end()) {
                    int curTime = GlobalData::shared()->getWorldTime();
                    for (int i=0; i<m_statusGroup[gID].size(); i++) {
                        int tmpSid = m_statusGroup[gID][i];
                        if (GlobalData::shared()->statusMap.find(tmpSid) != GlobalData::shared()->statusMap.end()) {
                            if (GlobalData::shared()->statusMap[tmpSid] > curTime) {
                                findSid = tmpSid;
                                break;
                            }
                        }
                    }
                }
            }
            
            int curStatusGid = 0;
            if (findSid>0) {
                string findGroup = CCCommonUtils::getPropById(CC_ITOA(findSid), "group");
                curStatusGid = atoi(findGroup.c_str());
            }
            
            if (curStatusGid>0) {
                if (group>=curStatusGid) {
                    //可以覆盖
                    YesNoDialog::show(_lang("101454").c_str() , CCCallFunc::create(this, callfunc_selector(UseSkillPopUpView::sendUseSkillCommand)));
                }
                else {
                    YesNoDialog::showYesDialog(_lang("101433").c_str());
                }
            }else{
                sendUseSkillCommand();
            }
        }else{
            sendUseSkillCommand();
        }
        
//        UseSkillCommand* cmd = new UseSkillCommand(m_skillId,info->uuid);
//        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(UseSkillPopUpView::successCallBack), NULL));
//        cmd->setFailCallback(CCCallFuncO::create(this, callfuncO_selector(UseSkillPopUpView::failCallBack), NULL));
//        cmd->sendAndRelease();
//        m_useBtn->setEnabled(false);
    }else{
        this->retain();
        PopupViewController::getInstance()->removePopupView(this);
        PopupViewController::getInstance()->addPopupInView(GeneralSkillListPopUpView::create(&GlobalData::shared()->generals.begin()->second,info->getAbilityBySkillId(m_skillId),m_skillId));
        this->release();
    }
}

void UseSkillPopUpView::sendUseSkillCommand(){
    GeneralInfo* info = &GlobalData::shared()->generals.begin()->second;
    UseSkillCommand* cmd = new UseSkillCommand(m_skillId,info->uuid);
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(UseSkillPopUpView::successCallBack), NULL));
    cmd->setFailCallback(CCCallFuncO::create(this, callfuncO_selector(UseSkillPopUpView::failCallBack), NULL));
    cmd->sendAndRelease();
    m_useBtn->setEnabled(false);
}

void UseSkillPopUpView::failCallBack(CCObject *data){
    m_useBtn->setEnabled(true);
}

void UseSkillPopUpView::successCallBack(CCObject *data){
    NetResult* result = dynamic_cast<NetResult*>(data);
    auto dic = _dict(result->getData());
    std::string type = CCCommonUtils::getPropById(m_skillId, "type");
    m_tabView->reloadData();
    CCLOG("type=%s",type.c_str());
    int skillType = atoi(type.c_str());
    //10：立即返回；11：救援；12：丰收；13：资源保护；14：盟友行军；15：能工巧匠
    string tip = "";
    switch (skillType) {
        case 10:
            SceneController::getInstance()->gotoScene(SCENE_ID_WORLD);
            return ;
            break;
        case 11:
        {
            m_useNode->setVisible(true);
            m_useNode->setPositionY(m_useSkillNode->getPositionY());
            m_useSkillNode->setVisible(false);
            m_useTxt->setString(_lang("118002"));
            m_useBgNode->removeAllChildrenWithCleanup(true);
            auto useBg = CCLoadSprite::createSprite("skill_use_bg1.png");
            m_useBgNode->addChild(useBg);
        }
            break;
        case 12:
        {
            std::string para1 = CCCommonUtils::getPropById(m_skillId, "para1");
            double param1 = atof(para1.c_str());
            int outPutSec = param1*60*60;
            
            string harvestInfo = "";
            harvestInfo = CC_ITOA(outPutSec);// output time
            harvestInfo += "|";
            harvestInfo += CC_ITOA(2350);//x
            harvestInfo += "|";
            harvestInfo += CC_ITOA(700);//y
            GlobalData::shared()->m_harvestInfo = harvestInfo;
            
            if(SceneController::getInstance()->currentSceneId!=SCENE_ID_MAIN){
                SceneController::getInstance()->gotoScene(SCENE_ID_MAIN);
            }else{
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SHANGE_SCENCE_SHOW_HARVEST);
            }
            tip = _lang("118001");
        }
            break;
        case 13:
            break;
        case 14:
        {
            m_useNode->setVisible(true);
            m_useNode->setPositionY(m_useSkillNode->getPositionY());
            m_useSkillNode->setVisible(false);
            m_useTxt->setString(_lang("118003"));
            m_useBgNode->removeAllChildrenWithCleanup(true);
            auto useBg = CCLoadSprite::createSprite("skill_use_bg2.png");
            m_useBgNode->addChild(useBg);
        }

            break;
        case 15:
        {
            string fortInfo = "";
            if(dic){
                auto trap = _dict(dic->objectForKey("trap"));
                fortInfo = trap->valueForKey("id")->getCString();
                fortInfo += "|";
                fortInfo += trap->valueForKey("free")->getCString();
            }
            GlobalData::shared()->m_skillFortInfo = fortInfo;
            if(SceneController::getInstance()->currentSceneId!=SCENE_ID_MAIN){
                SceneController::getInstance()->gotoScene(SCENE_ID_MAIN);
            }else{
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SHOW_FORT_SKILL_EFFECT);
            }
        }
            break;
        case 18:{//622000 行军上限
            ToolController::getInstance()->retUseTool(dic);
        }
            break;
        case 19:{//622100 资源采集
            ToolController::getInstance()->retUseTool(dic);
        }
            break;
        case 20:
        {
            if(dic && dic->objectForKey("itemEffectObj"))
            {
                auto effectObj = _dict(dic->objectForKey("itemEffectObj"));
                if(effectObj->objectForKey("stamina")){
                    WorldController::getInstance()->currentStamine = effectObj->valueForKey("stamina")->intValue();
                    WorldController::getInstance()->lastStamineTime = effectObj->valueForKey("lastStaminaTime")->doubleValue();
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CURRENT_STAMINE);
                }
            }
        }
            break;
        default:
            break;
    }
    if(tip!=""){
        CCCommonUtils::flyHint("", "", _lang("118001").c_str());
    }
    if(skillType!=11 && skillType!=14){
      PopupViewController::getInstance()->removePopupView(this);
    }else{
        m_tabView->reloadData();
    }
}

void UseSkillPopUpView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
    
}

cocos2d::CCSize UseSkillPopUpView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(cellW, 1.9 * 120 + 3);
    }
    return CCSize(cellW, 120);
}

CCTableViewCell* UseSkillPopUpView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx){
    if(idx >= m_data->count()){
        return NULL;
    }
    UseSkillItemCell* cell = (UseSkillItemCell*)table->dequeueGrid();
    CCString* info = (CCString*)m_data->objectAtIndex(idx);
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info->getCString());
        }else{
            cell = UseSkillItemCell::create(info->getCString());
        }
    }
    return cell;
}

ssize_t UseSkillPopUpView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    return m_data->count();
}

unsigned int UseSkillPopUpView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 1;
}

void UseSkillPopUpView::scrollViewDidScroll(CCScrollView* view){
    float dx = view->getContentOffset().x;
//    CCLOG("%f",dx);
    if (dx>=0) {
        m_sprL->setVisible(false);
        m_sprR->setVisible(true);
    }else if (dx<=m_listContainer->getContentSize().width-(m_data->count()*cellW)) {
        m_sprL->setVisible(true);
        m_sprR->setVisible(false);
    }else{
        m_sprL->setVisible(true);
        m_sprR->setVisible(true);
    }
}

UseSkillItemCell *UseSkillItemCell::create(std::string skillId){
    UseSkillItemCell *ret = new UseSkillItemCell(skillId);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void UseSkillItemCell::setData(std::string skillId){
    m_skillId = skillId;
    std::string head = CCCommonUtils::getIcon(skillId);
    m_clickNode->initWithSpriteFrame(CCLoadSprite::loadResource("icon_kuang_keji.png"));
    m_clickNode->setScale(0.7);
    m_head->removeAllChildrenWithCleanup(true);
    CCSprite* icon = CCLoadSprite::createSprite(head.c_str());
    icon->setScale(0.7);
    //CCCommonUtils::setSpriteMaxSize(icon, 80,true);
    m_head->addChild(icon);
    bool flag = false;
    map<std::string, SkillCDInfo*>::iterator it = GeneralManager::getInstance()->SkillCDMap.find(m_skillId);
    double gapTime = 0;
    double effectTime = 0;
    double wTime = GlobalData::shared()->getWorldTime();
    if(it != GeneralManager::getInstance()->SkillCDMap.end()){
        if(it->second->endTime!=0){
            gapTime = it->second->endTime - wTime;
        }
        if(it->second->stat==0){
            flag = true;
        }
        if(it->second->effectEndTime!=0){
            effectTime = it->second->effectEndTime - wTime;
        }
    }
    GeneralInfo* info = &GlobalData::shared()->generals.begin()->second;
    if (info->checkHaveStudy(m_skillId)) {
        CCCommonUtils::setSpriteGray(icon, false);
        CCCommonUtils::setSpriteGray(m_clickNode, false);
    }else{
        CCCommonUtils::setSpriteGray(icon, true);
        CCCommonUtils::setSpriteGray(m_clickNode, true);
        CCSprite* lock = CCLoadSprite::createSprite("UI_UseSkill_lock.png");
        m_head->addChild(lock);
    }
    m_cdNode->setVisible(gapTime>0);
    m_timeTxt->setString(CC_SECTOA(gapTime));
    this->unschedule(schedule_selector(UseSkillItemCell::updateTime));
    if (gapTime>0) {
        this->schedule(schedule_selector(UseSkillItemCell::updateTime), 1);
        //CD特效******************
//        double useTime = it->second->endTime-it->second->startTime;
//        CCCommonUtils::setSpriteGray(icon, true);//把原来的底图置灰
//        CCSprite* iconCD = CCLoadSprite::createSprite(head.c_str());//上层CD图
//        CCProgressTimer *progressCD = CCProgressTimer::create(iconCD);
//        progressCD->setType(CCProgressTimerType::kCCProgressTimerTypeRadial);
//        progressCD->setPercentage(((useTime-gapTime)/useTime)*100.0f);//(gapTime/useTime)*100.0f
//        progressCD->setScale(0.7);
//        m_head->addChild(progressCD, 1);
//        CCProgressTo *progressToAction = CCProgressTo::create(useTime, 100.0);
//        progressCD->runAction(progressToAction);
        //******************
    }

    //持续技能特效
    std::string skillType="";
    GeneralInfo* gInfo = &GlobalData::shared()->generals.begin()->second;
    std::map<std::string, std::map<std::string, GeneralSkillInfo*>>::iterator abilityIt;
    for(abilityIt = gInfo->generalSkillMap.begin(); abilityIt != gInfo->generalSkillMap.end(); abilityIt++){
        std::map<std::string, GeneralSkillInfo*>::iterator skillIt;
        for(skillIt = abilityIt->second.begin(); skillIt != abilityIt->second.end(); skillIt++){
            if (skillIt->first==m_skillId) {
                skillType=abilityIt->first;
                break;
            }
        }
        if (skillType!="") {
            break;
        }
    }
    
    string parStr = "SkillGlowR_";
    if (skillType=="50000") {
        parStr = "SkillGlowR_";
    }else if (skillType=="50001"){
        parStr = "SkillGlowB_";
    }else if (skillType=="50002"){
        parStr = "SkillGlowG_";
    }
    m_pNode->setVisible(false);
    if (effectTime>0) {
        m_pNode->setVisible(true);
        m_pNode->removeAllChildren();
        for (int i=1; i<=4; i++) {//SkillGlowB_ //SkillGlowG_ //SkillGlowR_
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",parStr.c_str(),i)->getCString());
            m_pNode->addChild(particle);
        }
    }
}

void UseSkillItemCell::updateTime(float t){
    map<std::string, SkillCDInfo*>::iterator it = GeneralManager::getInstance()->SkillCDMap.find(m_skillId);
    int gapTime = 0;
    double effectTime = 0;
    double wTime = GlobalData::shared()->getWorldTime();
    if(it != GeneralManager::getInstance()->SkillCDMap.end() && it->second->endTime!=0){
        gapTime = it->second->endTime - wTime;
        if(it->second->effectEndTime!=0){
            effectTime = it->second->effectEndTime - wTime;
        }
    }
    m_cdNode->setVisible(gapTime>0);
    m_timeTxt->setString(CC_SECTOA(gapTime));
    
    if (effectTime<=0) {
        m_pNode->setVisible(false);
    }
}

void UseSkillItemCell::onEnter(){
    CCNode::onEnter();

    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UseSkillItemCell::clickHead), "on_skill_click", NULL);
}

void UseSkillItemCell::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "on_skill_click");
    CCNode::onExit();
}

bool UseSkillItemCell::init(){
    auto bg = CCBLoadFile("UseSkillItemCell", this, this);
    if (CCCommonUtils::isIosAndroidPad()){
        this->setScale(1.9);
    }
    this->setContentSize(bg->getContentSize());
    setData(m_skillId);
    return true;
}

bool UseSkillItemCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_isDraging = false;
    if(isTouchInside(m_clickNode, pTouch)){
        touchX=pTouch->getLocation().x;
        m_touchMove=false;
        return true;
    }
    return false;
}

void UseSkillItemCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_clickNode, pTouch)&&!m_touchMove){
        map<std::string, SkillCDInfo*>::iterator it = GeneralManager::getInstance()->SkillCDMap.find(m_skillId);
        double gapTime = 0;
        if(it != GeneralManager::getInstance()->SkillCDMap.end() && it->second->endTime!=0){
            gapTime = it->second->endTime - GlobalData::shared()->getWorldTime();
            if(it->second->stat==0 && it->second->skillId=="602700"){//救助
                //todo
            }
        }
        m_ccNode->stopAllActions();
        m_ccNode->runAction(CCSequence::create(CCScaleTo::create(0.15, 1.1),CCScaleTo::create(0.1, 1.0),NULL));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("on_skill_click",CCString::create(m_skillId));
    }
}

void UseSkillItemCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    float dx = touchX>pTouch->getLocation().x?(touchX-pTouch->getLocation().x):(pTouch->getLocation().x-touchX);
    if (dx>25) {
        m_touchMove=true;
    }
}

void UseSkillItemCell::changePic(){
    
}

void UseSkillItemCell::clickHead(CCObject* data){
    CCString* str = dynamic_cast<CCString*>(data);
    if(str){
        std::string clickPic = str->getCString();
    }
}

void UseSkillItemCell::changeSuccess(){
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHANGE_PLAYER_PIC);
}

bool UseSkillItemCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_head", CCNode*, this->m_head);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_cdNode", CCNode*, this->m_cdNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pNode", CCNode*, this->m_pNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickNode", CCSprite*, this->m_clickNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_ccNode", CCNode*, this->m_ccNode);
    return false;
}

SEL_CCControlHandler UseSkillItemCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}