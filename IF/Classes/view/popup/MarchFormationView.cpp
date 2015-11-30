//
//  MarchFormationView.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/7/21.
//
//

#include "MarchFormationView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "YesNoDialog.h"
#include "ToolController.h"
#include "UseCDToolView.h"
#include "ArmyInfo.h"
#include "LocalController.h"
#include "FunBuildController.h"
#include "TroopsController.h"
#include "WorldMapView.h"
#include "UIComponent.h"
#include "CCGraySprite.h"
#include "TipsView.h"
#include "WorldCommand.h"
#include "SceneController.h"
#include "AllianceWarDetailView.h"
#include "AllianceTeamInfo.h"
#include "UseItemStatusView.h"
#include "ItemStatusView.h"
#include "ArmyController.h"
#include "UseResToolView.h"
#include "WorldMarchCheck.h"
#include "GameController.h"
#include "SoundController.h"
#include "fireandcomman.h"
const float numPerRow = 1.0;
#define MARCH_FORMATION_CHANGE "march_formation_change"
MarchFormationView* MarchFormationView::create(int index){
    MarchFormationView* ret = new MarchFormationView(index);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
MarchFormationView::~MarchFormationView()
{
    CCLoadSprite::doResourceByCommonIndex(8, false);
}
bool MarchFormationView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(8, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    CCLoadSprite::doResourceByCommonIndex(6, true);
    CCLoadSprite::doResourceByCommonIndex(504, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(8, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
        CCLoadSprite::doResourceByCommonIndex(6, false);
        CCLoadSprite::doResourceByCommonIndex(504, false);
    });
    
    if (m_index == 1) {
        setTitleName(_lang("103688"));
    }
    else if (m_index == 2) {
        setTitleName(_lang("103689"));
    }
    auto tmpCCB = CCBLoadFile("MarchFormationView",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else {
        this->setContentSize(tmpCCB->getContentSize());
        
        int dy = 852 - CCDirector::sharedDirector()->getWinSize().height;
        m_bg->setPreferredSize(m_bg->getContentSize() + CCSize(0, -dy));

        m_downNode->setPositionY(m_downNode->getPositionY() + dy);
        m_infoList->setPositionY(m_infoList->getPositionY() + dy);
        m_infoList->setContentSize(m_infoList->getContentSize() - CCSize(0, dy));
    }
    
    m_tabView = CCMultiColTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setMultiColTableViewDelegate(this);
    m_tabView->setTouchPriority(Touch_Popup);
    m_infoList->addChild(m_tabView);
    
    CCCommonUtils::setButtonTitle(m_quickBtn, _lang("108572").c_str());
    CCCommonUtils::setButtonTitle(m_marchBtn, _lang("115035").c_str());
//    CCCommonUtils::setButtonTitle(m_formationBtn1, _lang("103681").c_str());
//    CCCommonUtils::setButtonTitle(m_formationBtn2, _lang("103682").c_str());
    TroopsController::getInstance()->m_curLoadNum = 0;
    m_msg1Label->setString("0/");
    m_msg3Label->setString(CC_ITOA(TroopsController::getInstance()->getMaxSoilder()));
    m_msg2Label->setString(CC_ITOA(TroopsController::getInstance()->m_curLoadNum));
    m_tmpArray = CCArray::create();
    m_selSoldierMap.clear();
    generateData();
    getFormationDetail(m_index);
    
    m_forTipsTxt->setString(_lang("103690"));
    if(m_tmpArray->count() == 0){
        m_hintText->setString(_lang("102151").c_str());
    }else{
        m_hintText->setString("");
    }
    m_marchBtn->setEffectStr("");
    updateInfo();
    updateFormationChange(NULL);
    updateLoadInfo(NULL);
    m_msg3Label->setString(CC_ITOA(TroopsController::getInstance()->getMaxSoilder()));
    _modelLayer->setOpacity(200);
    this->scheduleOnce(schedule_selector(MarchFormationView::loadResource), 0.02);
    SoundController::sharedSound()->playEffects(Music_Sfx_world_click_attack);
    m_marchBtn->setEnabled(false);
    
    
    
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch* touch, Event* evt)
    {
        if (isTouchInside(m_2touchBtnBg, touch)) {
            return true;
        }
        return false;
    };
    
    listener->onTouchEnded = [this](Touch* touch, Event* evt)
    {
        //        CCCommonUtils::flyHint("", "", "hehe");
        auto armyInfo = GlobalData::shared()->armyList[GlobalData::shared()->titanInfo.titanId];
        if((armyInfo.free + armyInfo.march)== 0)
        {
            return ;
        }
        if (selectDragon) {
            
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            
            selectDragon = false;//
            
            this->refreshDragonNumStatus();
            
        }
        else
        {
            //fusheng 需要判断是否可以点击派出龙   出征部队人数已满
            
            
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            
            

            selectDragon = true;
            
            this->refreshDragonNumStatus();

            

            
        }
        
        return ;
    };
    
    listener->setSwallowTouches(true);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, m_bg);
    
    
    refreshDragonStatus(nullptr);
    
    
    return true;
}


void MarchFormationView::refreshDragonStatus(CCObject* obj)
{
    auto armyInfo = GlobalData::shared()->armyList[GlobalData::shared()->titanInfo.titanId];
    if((armyInfo.free + armyInfo.march) != 0)//士兵里有龙
    {


        
        string name = CCCommonUtils::getNameById(GlobalData::shared()->titanInfo.titanId);
       
        m_dragonName->setString(name);
        
        
        string id = CC_ITOA(GlobalData::shared()->titanInfo.tid);
        
        auto picName = CCCommonUtils::getPropById(id, "dragonMarchUI");
        
        auto spr = CCLoadSprite::createSprite(picName.c_str());
        m_dragonPicNode->removeAllChildren();
        
        m_dragonPicNode->addChild(spr);
        


        
    }
  
}


void MarchFormationView::refreshDragonNumStatus()
{
    auto armyInfo = GlobalData::shared()->armyList[GlobalData::shared()->titanInfo.titanId];
    if((armyInfo.free + armyInfo.march) != 0 )//士兵里有龙
    {

        
        m_2touchBtn->setPositionX(selectDragon?94:20);
        
        m_2touchBtn->setSpriteFrame(selectDragon?"nb_checkboxBlockON.png":"nb_checkboxBlockOFF.png");
        
        m_2touchBtnBg->setSpriteFrame(selectDragon?"nb_checkboxBGSend.png":"nb_checkboxBGStay.png");
        
        TroopsController::getInstance()->updateTmpBattleData(GlobalData::shared()->titanInfo.titanId, selectDragon?1:0, GlobalData::shared()->titanInfo.titanId);
    }
    
    m_selSoldierMap[GlobalData::shared()->titanInfo.titanId] = selectDragon?1:0;
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MARCH_FORMATION_CHANGE);
}


void MarchFormationView::getFormationDetail(int index){
    if (index == 1) {
        m_formationBtn1->setHighlighted(true);
        m_formationBtn2->setHighlighted(false);
    }
    else if (index == 2) {
        m_formationBtn2->setHighlighted(true);
        m_formationBtn1->setHighlighted(false);
    }
    
    unselectAll();
    std::string formationStr = GlobalData::shared()->playerInfo.uid + "formation";
    formationStr.append(CC_ITOA(index));
    bool flag = CCUserDefault::sharedUserDefault()->getBoolForKey(formationStr.c_str(),false);
    if (flag == true) {
        //先判断总兵总上限
        std::string forNum = GlobalData::shared()->playerInfo.uid + "forNum";
        forNum.append(CC_ITOA(index));
        int numTotal = CCUserDefault::sharedUserDefault()->getIntegerForKey(forNum.c_str(), 0);
        if (numTotal > TroopsController::getInstance()->getMaxSoilder()) {
            std::string forMethod = GlobalData::shared()->playerInfo.uid + "forMethod";
            forMethod.append(CC_ITOA(index));
            CCUserDefault::sharedUserDefault()->setStringForKey(forMethod.c_str(), "");
            CCUserDefault::sharedUserDefault()->setIntegerForKey(forNum.c_str(), 0);
            CCUserDefault::sharedUserDefault()->flush();
            CCCommonUtils::flyHint("", "", _lang("103684"), 3, 0, true);
            return;
        }
        std::string forMethod = GlobalData::shared()->playerInfo.uid + "forMethod";
        forMethod.append(CC_ITOA(index));
        std::string method = CCUserDefault::sharedUserDefault()->getStringForKey(forMethod.c_str(), "");
        vector<std::string> vector1;
        CCCommonUtils::splitString(method, "|", vector1);
        int num = vector1.size();
        for (int i = 0; i < num; i++) {
            vector<std::string> vector2;
            CCCommonUtils::splitString(vector1[i], ",", vector2);
            if (vector2.size() == 2) {
                int selNum = atoi(vector2[1].c_str());
                
                int sid = atoi(vector2[0].c_str());
                if(sid>=107401&&sid<=107430)//fusheng 存在泰坦
                {
                    auto armyInfo = GlobalData::shared()->armyList[GlobalData::shared()->titanInfo.titanId];
                    if((armyInfo.free + armyInfo.march) != 0 )//士兵里有龙
                    {
                        selectDragon = selNum == 1?true:false;
                        
                        this->refreshDragonNumStatus();
                        
                    }
                    
                }
                else
                {
                    TroopsController::getInstance()->updateTmpBattleData(vector2[0], selNum, vector2[0]);
                }

                
            }
        }
    }
}
void MarchFormationView::loadResource(float _time){
    CCLoadSprite::doResourceByCommonIndex(204, true);
//    updateInfo();
}

void MarchFormationView::setAddBtnState(){
    if(CCCommonUtils::getEffectValueByNum(COMMAND_EFFECT) > FLT_EPSILON){
        this->m_addBtn->setVisible(false);
        this->m_addIcon->setVisible(false);
        this->m_addIcon1->setVisible(false);
        this->m_msg3Label->setColor(ccc3(86, 180, 29));
    }else{
        this->m_addBtn->setVisible(true);
        this->m_addIcon->setVisible(true);
        this->m_addIcon1->setVisible(true);
        this->m_msg3Label->setColor(ccc3(255, 247, 255));
        this->getAnimationManager()->runAnimationsForSequenceNamed("Default Timeline");
    }
}

void MarchFormationView::updateFormationChange(cocos2d::CCObject *obj){
    auto & arr = m_tabView->getContainer()->getChildren();
    if (true) {
        int i = 0;
        while(i < arr.size()){
            auto & children = dynamic_cast<CCNode*>(arr.at(i++))->getChildren();
            MarchFormationCell *cell = dynamic_cast<MarchFormationCell*>(children.at(0));
            if(cell){
                m_selSoldierMap[cell->m_soldierId] = cell->m_num;
            }
        }
        m_marchBtn->setEnabled(true);
    }
    int total = 0;
    map<string, int>::iterator it;
    for (it = TroopsController::getInstance()->m_tmpBattleInfos.begin(); it != TroopsController::getInstance()->m_tmpBattleInfos.end(); it++) {
        if (TroopsController::getInstance()->m_tmpBattleInfos[it->first] > 0) {
            total += TroopsController::getInstance()->m_tmpBattleInfos[it->first];
        }
    }
    
    m_msg1Label->setString((std::string("") + CC_ITOA(total) + "/").c_str());
}

void MarchFormationView::generateData(){
    TroopsController::getInstance()->m_tmpFreeSoldiers.clear();
    TroopsController::getInstance()->m_tmpBattleInfos.clear();
    TroopsController::getInstance()->m_tmpConfSoldiers.clear();
    m_tmpArray->removeAllObjects();
    std::map<string, ArmyInfo>::iterator it;
    for (it = GlobalData::shared()->armyList.begin(); it!=GlobalData::shared()->armyList.end(); it++) {
        if((it->second.free <= 0  && it->second.march <= 0) || !it->second.isArmy){
            continue;
        }
        int sid = atoi(it->first.c_str());
        if(sid>=107401&&sid<=107430)//fusheng 加一个保障
        {
            if (it->first != GlobalData::shared()->titanInfo.titanId) {
                continue;
            }
            else
            {
                TroopsController::getInstance()->m_tmpFreeSoldiers[it->first] = (it->second).free + (it->second).march;
                TroopsController::getInstance()->m_tmpConfSoldiers[it->first] = (it->second).free + (it->second).march;
                continue;
            }
        }

        TroopsController::getInstance()->m_tmpFreeSoldiers[it->first] = (it->second).free + (it->second).march;
        TroopsController::getInstance()->m_tmpConfSoldiers[it->first] = (it->second).free + (it->second).march;
        int index = 0;
        bool addFlag = false;
        while (index < m_tmpArray->count()) {
            std::string armyId = dynamic_cast<CCString*>(m_tmpArray->objectAtIndex(index))->getCString();
            auto &army = GlobalData::shared()->armyList[armyId];
            if(army.armyLevel < it->second.armyLevel){
                m_tmpArray->insertObject(CCString::create(it->first), index);
                addFlag = true;
                break;
            }
            index++;
        }
        if(!addFlag){
            m_tmpArray->addObject(CCString::create(it->first));
        }
    }
}

void MarchFormationView::updateInfo()
{
    m_tabView->reloadData();
}

bool MarchFormationView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (isTouchInside(m_quickBtn, pTouch)) {
        return true;
    }
    if (isTouchInside(m_marchBtn, pTouch)) {
        return true;
    }
    if (isTouchInside(m_formationBtn1, pTouch)) {
        return true;
    }
    if (isTouchInside(m_formationBtn2, pTouch)) {
        return true;
    }
    if (isTouchInside(m_forbidenNode, pTouch)) {
        return true;
    }
    return false;
}

void MarchFormationView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){

}

void MarchFormationView::onEnter()
{
    CCLoadSprite::doResourceByCommonIndex(105, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(105, false);
        
    });
    setAddBtnState();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MarchFormationView::updateArmyNumber), MSG_TROOPS_BACK, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MarchFormationView::updateFormationChange), MARCH_FORMATION_CHANGE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MarchFormationView::updateLoadInfo), MSG_TROOPS_BATTLE_LOAD, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MarchFormationView::refreshDragonStatus), MSG_TITAN_UPGRADE_COMPLETE, NULL);
    
    
    updateLoadInfo(NULL);
    CCNode::onEnter();
}

void MarchFormationView::onExit()
{
    setTouchEnabled(false);
//    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_TROOPS_BACK);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this,MARCH_FORMATION_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_TROOPS_BATTLE_LOAD);
      CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_TITAN_UPGRADE_COMPLETE);
    CCNode::onExit();
}

void MarchFormationView::updateLoadInfo(CCObject* obj)
{
    int loadNum = TroopsController::getInstance()->m_curLoadNum;
    m_helpBtn->setVisible(true);
    string loadInfo = CC_CMDITOA(loadNum);
    m_msg2Label->setString(loadInfo.c_str());
    
    int total = 0;
    map<string, int>::iterator it;
    for (it = TroopsController::getInstance()->m_tmpBattleInfos.begin(); it != TroopsController::getInstance()->m_tmpBattleInfos.end(); it++) {
        if (TroopsController::getInstance()->m_tmpBattleInfos[it->first] > 0) {
            total += TroopsController::getInstance()->m_tmpBattleInfos[it->first];
        }
    }
    
    m_msg1Label->setString((std::string("") + CC_ITOA(total) + "/").c_str());
    int maxSoilder = TroopsController::getInstance()->getMaxSoilder();
    m_msg3Label->setString(CC_ITOA(maxSoilder));
}

SEL_CCControlHandler MarchFormationView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickQuickBtn", MarchFormationView::onClickQuickBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickMarchBtn", MarchFormationView::onClickMarchBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHelpClick", MarchFormationView::onHelpClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddClick", MarchFormationView::onAddClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFormation1Click", MarchFormationView::onFormation1Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFormation2Click", MarchFormationView::onFormation2Click);
    return NULL;
}

bool MarchFormationView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dragonPicNode", CCNode*, this->m_dragonPicNode);
  
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dragonName", CCLabelIF*, this->m_dragonName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_2touchBtnBg", CCSprite*, this->m_2touchBtnBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_2touchBtn", CCSprite*, this->m_2touchBtn);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_marchBtn", CCControlButton*, this->m_marchBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_quickBtn", CCControlButton*, this->m_quickBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg1Label", CCLabelIF*, this->m_msg1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg2Label", CCLabelIF*, this->m_msg2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg3Label", CCLabelIF*, this->m_msg3Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downNode", CCNode*, this->m_downNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_forbidenNode", CCNode*, this->m_forbidenNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconContainer", CCNode*, this->m_iconContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_alertNode", CCNode*, this->m_alertNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
 
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintText", CCLabelIF*, m_hintText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintText1", CCLabelIF*, m_hintText1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_forTipsTxt", CCLabelIF*, m_forTipsTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addBtn", CCControlButton*, m_addBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addIcon", CCSprite*, m_addIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addIcon1", CCSprite*, m_addIcon1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpBtn", CCControlButton*, m_helpBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_formationBtn1", CCControlButton*, m_formationBtn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_formationBtn2", CCControlButton*, m_formationBtn2);

    return false;
}

void MarchFormationView::onHelpClick(CCObject * pSender, CCControlEvent pCCControlEvent){
}

void MarchFormationView::onFormation1Click(CCObject *pSender, CCControlEvent event){
    if (m_index == 1) {
        m_formationBtn1->setHighlighted(true);
        m_formationBtn2->setHighlighted(false);
        return;
    }
    
    m_index = 1;
    getFormationDetail(m_index);
    updateInfo();
    m_marchBtn->setEnabled(false);
    m_selSoldierMap.clear();
}
void MarchFormationView::onFormation2Click(CCObject *pSender, CCControlEvent event){
    if (m_index == 2) {
        m_formationBtn2->setHighlighted(true);
        m_formationBtn1->setHighlighted(false);
        return;
    }
    
    m_index = 2;
    getFormationDetail(m_index);
    updateInfo();
    m_marchBtn->setEnabled(false);
    m_selSoldierMap.clear();
}

void MarchFormationView::onClickMarchBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    std::string formationStr = GlobalData::shared()->playerInfo.uid + "formation";
    formationStr.append(CC_ITOA(m_index));
    
    bool flag = CCUserDefault::sharedUserDefault()->getBoolForKey(formationStr.c_str(),false);
    
    if (flag == false) {
        CCUserDefault::sharedUserDefault()->setBoolForKey(formationStr.c_str(), true);
    }

    std::string method = "";
    map<std::string, int>::iterator m_itr = m_selSoldierMap.begin();
    int numTotal = 0;
    while(m_itr != m_selSoldierMap.end()){
        method.append(m_itr->first);
        method.append(",");
        method.append(CC_ITOA(m_itr->second));
        numTotal += m_itr->second;
        m_itr++;
        if (m_itr != m_selSoldierMap.end()) {
            method.append("|");
        }
    }
    std::string forMethod = GlobalData::shared()->playerInfo.uid + "forMethod";
    forMethod.append(CC_ITOA(m_index));
    CCUserDefault::sharedUserDefault()->setStringForKey(forMethod.c_str(), method.c_str());
    std::string forNum = GlobalData::shared()->playerInfo.uid + "forNum";
    forNum.append(CC_ITOA(m_index));
    CCUserDefault::sharedUserDefault()->setIntegerForKey(forNum.c_str(), numTotal);
    CCUserDefault::sharedUserDefault()->flush();
    if (m_index == 1) {
        CCCommonUtils::flyHint("", "", _lang_1("103683", _lang("103681").c_str()), 3, 0, true);
    }
    else if (m_index == 2) {
        CCCommonUtils::flyHint("", "", _lang_1("103683", _lang("103682").c_str()), 3, 0, true);
    }
    m_marchBtn->setEnabled(false);
}

void MarchFormationView::updateArmyNumber(CCObject* obj)
{
    std::map<string, ArmyInfo>::iterator it;
    m_tmpArray->removeAllObjects();
    for (it = GlobalData::shared()->armyList.begin(); it!=GlobalData::shared()->armyList.end(); it++) {
        if(!it->second.isArmy){
            continue;
        }
        int sid = atoi(it->first.c_str());
        if(sid>=107401&&sid<=107430)//fusheng 加一个保障
        {
            if (it->first != GlobalData::shared()->titanInfo.titanId) {
                continue;
            }
            else
            {
                TroopsController::getInstance()->m_tmpFreeSoldiers[it->first] = (it->second).free + (it->second).march;
                TroopsController::getInstance()->m_tmpConfSoldiers[it->first] = (it->second).free + (it->second).march;
                continue;
            }
        }
        if((it->second).free != TroopsController::getInstance()->m_tmpConfSoldiers[it->first])
        {
            int count = (it->second).free-TroopsController::getInstance()->m_tmpConfSoldiers[it->first];
            TroopsController::getInstance()->m_tmpConfSoldiers[it->first] = (it->second).free;
            TroopsController::getInstance()->m_tmpFreeSoldiers[it->first] += count;
        }
        if((it->second).free==0)
        {
            continue;
        }
        
       
        int index = 0;
        bool addFlag = false;
        while (index < m_tmpArray->count()) {
            std::string armyId = dynamic_cast<CCString*>(m_tmpArray->objectAtIndex(index))->getCString();
            auto &army = GlobalData::shared()->armyList[armyId];
            if(army.armyLevel < it->second.armyLevel){
                m_tmpArray->insertObject(CCString::create(it->first), index);
                addFlag = true;
                break;
            }
            index++;
        }
        if(!addFlag){
            m_tmpArray->addObject(CCString::create(it->first));
        }
    }
    updateInfo();
}

void MarchFormationView::selectAll(){
    int totalNum = 0;
    int maxForceNum = TroopsController::getInstance()->getMaxSoilder();
    for (int i = 0; i<m_tmpArray->count(); i++) {
        std::string m_soldierId = dynamic_cast<CCString*>(m_tmpArray->objectAtIndex(i))->getCString();
        TroopsController::getInstance()->updateTmpBattleData(m_soldierId, 0, m_soldierId);
    }
    
    for (int i = 0; i<m_tmpArray->count(); i++) {
        std::string m_soldierId = dynamic_cast<CCString*>(m_tmpArray->objectAtIndex(i))->getCString();
        int tmpCntNum = 0;
        tmpCntNum = TroopsController::getInstance()->m_tmpConfSoldiers[m_soldierId];
        tmpCntNum = totalNum + tmpCntNum >= maxForceNum ? maxForceNum - totalNum : tmpCntNum;
        TroopsController::getInstance()->updateTmpBattleData(m_soldierId, tmpCntNum, m_soldierId);
        if(totalNum + tmpCntNum >= maxForceNum){
            break;
        }

        totalNum += tmpCntNum;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           , CCString::createWithFormat("ED_quick"));
}

void MarchFormationView::unselectAll(){
    for (int i = 0; i<m_tmpArray->count(); i++) {
        std::string m_soldierId = dynamic_cast<CCString*>(m_tmpArray->objectAtIndex(i))->getCString();
        TroopsController::getInstance()->updateTmpBattleData(m_soldierId, 0, m_soldierId);
    }
}

void MarchFormationView::onAddClick(CCObject * pSender, CCControlEvent pCCControlEvent){
    auto getTypeArray = [](int type){
        int city_lv = FunBuildController::getInstance()->getMainCityLv();
        auto goodDic = LocalController::shared()->DBXMLManager()->getGroupByKey("goods");
        CCArray* array = new CCArray();
        array->init();
        int num = 0;
        if(goodDic)
        {
            CCDictElement* element;
            CCDICT_FOREACH(goodDic, element)
            {
                CCDictionary* dictInfo = _dict(element->getObject());
                if(type==dictInfo->valueForKey("type2")->intValue() && dictInfo->valueForKey("type")->intValue()==4 && city_lv>=dictInfo->valueForKey("lv")->intValue()){
                    array->addObject(dictInfo);
                }
            }
        }
        num = array->count();
        for (int i=0; i<num; i++) {
            for (int j=i; j<num; j++) {
                CCDictionary* dict1 = _dict(array->objectAtIndex(i));
                CCDictionary* dict2 = _dict(array->objectAtIndex(j));
                if(dict1->valueForKey("order_num")->intValue()<dict2->valueForKey("order_num")->intValue()){
                    array->swap(i, j);
                }
            }
        }
        return array;
    };
    ToolController::getInstance()->m_typeItems[14] = getTypeArray(14);
    PopupViewController::getInstance()->addPopupInView(UseItemStatusView::create(14,_lang("101413"),_lang("101450")));
}

void MarchFormationView::onClickQuickBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    PopupViewController::getInstance()->goBackPopupView();
}

void MarchFormationView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell)
{
}

cocos2d::CCSize MarchFormationView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1410, 250);
    }
    return CCSize(600, 150);
}

CCTableViewCell* MarchFormationView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx)
{
    if(idx >= m_tmpArray->count()){
        return NULL;
    }
    
    MarchFormationCell* cell = (MarchFormationCell*)table->dequeueGrid();
    if(idx < m_tmpArray->count()){
        std::string gid = dynamic_cast<CCString*>(m_tmpArray->objectAtIndex(idx))->getCString();
        int num = 0;
        if(TroopsController::getInstance()->m_tmpBattleInfos.find(gid) != TroopsController::getInstance()->m_tmpBattleInfos.end())
        {
            num = TroopsController::getInstance()->m_tmpBattleInfos[gid];
        }
        if(cell){
            cell->setData(gid, num, m_index);
            cell->setTag(idx);
        }else{
            cell = MarchFormationCell::create(m_infoList,gid, num, m_index);
            cell->setTag(idx);
        }
    }
    return cell;
}

ssize_t MarchFormationView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    int num = ceil(m_tmpArray->count() / numPerRow);
    return num;
}

unsigned int MarchFormationView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return numPerRow;
}

#pragma mark -
#pragma mark HeroCell Part

MarchFormationCell* MarchFormationCell::create(CCNode* clickNode, string itemId, int num, int index)
{
    MarchFormationCell *ret = new MarchFormationCell();
    if (ret && ret->init(clickNode,itemId, num, index)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MarchFormationCell::init(CCNode* clickNode,string itemId, int num, int index)
{
    bool ret = true;
    CCBLoadFile("March2",this,this);
    m_clickNode = clickNode;
    m_subBtn->setTouchPriority(Touch_Popup_Item);
    m_addBtn->setTouchPriority(Touch_Popup_Item);
//    auto m_sliderBg = CCLoadSprite::createScale9Sprite("huadongtiao3.png");
//    m_sliderBg->setInsetBottom(5);
//    m_sliderBg->setInsetLeft(5);
//    m_sliderBg->setInsetRight(5);
//    m_sliderBg->setInsetTop(5);
    m_slider = NBSlider::create("nb_bar_bg.png", "nb_bar_pro.png", "nb_cursor_icon.png",NBSlider::TextureResType::PLIST);
    m_slider->setCapInsets(Rect(8, 1, 30, 13));
    m_slider->setContentSize(Size(230, 15));
//    m_sliderBg->setAnchorPoint(ccp(0.5,0.5));
//    m_sliderBg->setPosition(ccp(252/2, 25));
//    m_sliderBg->setContentSize(CCSize(252,18));
//    
//    auto bgSp = CCLoadSprite::createSprite("huadongtiao4.png");
//    bgSp->setVisible(false);
//    auto proSp = CCLoadSprite::createSprite("huadongtiao4.png");
//    auto thuSp = CCLoadSprite::createSprite("huadongtiao1.png");
//    
//    m_slider = CCSliderBar::createSlider(m_sliderBg, proSp, thuSp);
    m_slider->setMinimumValue(0.0f);
    m_slider->setMaximumValue(1.0f);
//    m_slider->setProgressScaleX(248/proSp->getContentSize().width);
    m_slider->setTag(1);
//    m_slider->setLimitMoveValue(25);
//    m_slider->setTouchPriority(Touch_Popup);
     m_slider->addEventListener(CC_CALLBACK_2(MarchFormationCell::valueChange, this));
//    m_slider->addTargetWithActionForControlEvents(this, cccontrol_selector(MarchFormationCell::valueChange), CCControlEventValueChanged);
     m_slider->setPosition(-230 / 2, -15 / 2);
    m_sliderNode->addChild(m_slider, 1);
    
    auto editSize = m_editNode->getContentSize();
    m_editBox = CCEditBox::create(editSize, CCLoadSprite::createScale9Sprite("frame_3.png"));
    m_editBox->setInputMode(kEditBoxInputModeNumeric);
    m_editBox->setText("0");
    m_editBox->setDelegate(this);
   // m_editBox->setTouchPriority(Touch_Popup);
    m_editBox->setMaxLength(12);
    m_editBox->setReturnType(kKeyboardReturnTypeDone);
    m_editBox->setPosition(ccp(editSize.width/2, editSize.height/2));
    m_editNode->addChild(m_editBox);
    
    CCLoadSprite::doResourceByCommonIndex(204, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(204, false);
    });
    
    setData(itemId, num, index);
    return ret;
}

void MarchFormationCell::setData(string itemId, int num, int index)
{
    m_soldierId = itemId;
    m_num = num;
    m_index = index;
    string name = CCCommonUtils::getNameById(m_soldierId);
    string picName = GlobalData::shared()->armyList[m_soldierId].getHeadIcon();
    m_cntNum = TroopsController::getInstance()->m_tmpConfSoldiers[m_soldierId];
    m_slider->setEnabled(true);
    m_nameLabel->setString(name.c_str());
    
    m_picNode->removeAllChildren();
    
    int id = atoi(itemId.c_str());

    if (picName != "ico107401_small.png")
    {
        auto pic = CCLoadSprite::createSprite(picName.c_str());
        CCCommonUtils::setSpriteMaxSize(pic, 110);
        m_picNode->addChild(pic);
        pic->setPositionY(-10);
        
        m_picBg0->setVisible(id % 4 == 0);
        m_picBg1->setVisible(id % 4 == 1);
        m_picBg2->setVisible(id % 4 == 2);
        m_picBg3->setVisible(id % 4 == 3);
    }
    
    auto pic = CCLoadSprite::createSprite(picName.c_str());
    CCCommonUtils::setSpriteMaxSize(pic, 110);
    m_picNode->addChild(pic);
    pic->setPositionY(-10);
    this->m_levelNode->removeAllChildren();
    string num1 = m_soldierId.substr(m_soldierId.size()-2);
    auto pic1= CCCommonUtils::getRomanSprite(atoi(num1.c_str())+1);
    m_levelNode->addChild(pic1);
    
    float pro = 0;
    if (m_cntNum>0) {
        pro = m_num * 1.0 / m_cntNum;
    }
    m_slider->setValue(pro);
    m_editBox->setText(CC_ITOA(m_num));
    
    TroopsController::getInstance()->updateTmpBattleData(m_soldierId, m_num, m_soldierId);
}

void MarchFormationCell::refresh(){
    int num = 0;
    if(TroopsController::getInstance()->m_tmpBattleInfos.find(m_soldierId) != TroopsController::getInstance()->m_tmpBattleInfos.end())
    {
        num = TroopsController::getInstance()->m_tmpBattleInfos[m_soldierId];
    }
    
    setData(m_soldierId, num, m_index);
}

void MarchFormationCell::valueChange(Ref *pSender, NBSlider::EventType type)
{
    if (m_cntNum<=0) {
        return;
    }
    
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    float value = m_slider->getValue();
    int curNum = round(value * 1.0 * m_cntNum);
    int retNum = TroopsController::getInstance()->updateTmpBattleData(m_soldierId, curNum, m_soldierId);
    m_editBox->setText(CC_ITOA(retNum));
    TroopsController::getInstance()->changeArrTime();
    m_num = retNum;
    int maxForceNum = TroopsController::getInstance()->getMaxSoilder();
    
    int total = 0;
    map<string, int>::iterator it;
    for (it = TroopsController::getInstance()->m_tmpBattleInfos.begin(); it != TroopsController::getInstance()->m_tmpBattleInfos.end(); it++) {
        if (TroopsController::getInstance()->m_tmpBattleInfos[it->first] > 0) {
            total += TroopsController::getInstance()->m_tmpBattleInfos[it->first];
        }
    }
    
    int d = maxForceNum - total;
    if(d < 0){
        curNum += d;
        m_slider->setValue(curNum * 1.0 / m_cntNum);
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MARCH_FORMATION_CHANGE);
}

bool MarchFormationCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (isTouchInside(m_clickNode, pTouch)) {
        return true;
    }
    return false;
}

void MarchFormationCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    
}

void MarchFormationCell::onEnter() {
    CCNode::onEnter();
}

void MarchFormationCell::onExit() {
    CCNode::onExit();
}

SEL_CCControlHandler MarchFormationCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSubClick", MarchFormationCell::onSubClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddClick", MarchFormationCell::onAddClick);
    
    return NULL;
}

void MarchFormationCell::onSubClick(CCObject * pSender, CCControlEvent pCCControlEvent){
    int num = m_slider->getValue() * m_cntNum - 1;
    if(num < 0){
        num = 0;
    }
    m_slider->setValue(num * 1.0f / m_cntNum);
}

void MarchFormationCell::onAddClick(CCObject * pSender, CCControlEvent pCCControlEvent){
    int num = m_slider->getValue() * m_cntNum + 1;
    if(num > m_cntNum){
        num = m_cntNum;
    }
    m_slider->setValue(num * 1.0f / m_cntNum);
}

bool MarchFormationCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_levelNode", CCNode*, m_levelNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sliderNode", CCNode*, m_sliderNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editNode", CCNode*, m_editNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subBtn", CCControlButton*, m_subBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addBtn", CCControlButton*, m_addBtn);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBg0", Sprite*, m_picBg0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBg1", Sprite*, m_picBg1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBg2", Sprite*, m_picBg2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBg3", Sprite*, m_picBg3);
    
    return false;
}

void MarchFormationCell::editBoxReturn(CCEditBox *editBox) {
    int value = atoi(editBox->getText());
    
    if (value > m_cntNum) {
        value = m_cntNum;
    } else if (value < 0) {
        value = 0;
    }
    value = TroopsController::getInstance()->updateTmpBattleData(m_soldierId, value, m_soldierId);
    
    if (m_cntNum > 0) {
        m_slider->setValue(1.0f * value / m_cntNum);
    }else{
        m_slider->setValue(0.0f);
    }
}