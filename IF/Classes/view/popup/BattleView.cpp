//
//  BattleView.cpp
//  IF
//
//  Created by fubin on 14-3-11.
//
//

#include "BattleView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "YesNoDialog.h"
#include "ToolController.h"
#include "UseCDToolView.h"
#include "ArmyInfo.h"
#include "LocalController.h"
#include "FunBuildController.h"
#include "TroopsController.h"
#include "GeneralManager.h"
#include "GeneralInfo.h"
#include "WorldMapView.h"
#include "GeneralManager.h"
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
#include "BuildUpgradeView.h"
#include "TitanController.h"

#include "NBCommonUtils.h"

const float numPerRow = 1.0;
BattleView* BattleView::create(unsigned int startIndex ,unsigned int targetIndex,unsigned int haveOwner,float slow,int rally,int bType,int wtIndex,std::string other, int targetType){
    BattleView* ret = new BattleView();
    if(ret && ret->init(startIndex,targetIndex,haveOwner,slow,rally,bType,wtIndex,other,targetType)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

BattleView::BattleView():m_startIndex(0),m_targetIndex(0),m_haveOwner(0),m_rally(0),m_bType(-1),m_wtIndex(0),m_other(string()),m_targetType(0),m_slow(1.0),isBegin(true),selectDragon(false){};
BattleView::~BattleView()
{
    WorldController::getInstance()->alertProectFlag = false;
}

void BattleView::loadResource(){
    CCLoadSprite::doResourceByCommonIndex(8, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    CCLoadSprite::doResourceByCommonIndex(6, true);
    CCLoadSprite::doResourceByCommonIndex(504, true);
    CCLoadSprite::doResourceByCommonIndex(105, true);
}

bool BattleView::init(unsigned int startIndex,unsigned int targetIndex,unsigned int haveOwner,float slow,int rally,int bType,int wtIndex,std::string other, int targetType)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    loadResource();
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(8, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
        CCLoadSprite::doResourceByCommonIndex(6, false);
        CCLoadSprite::doResourceByCommonIndex(504, false);
        CCLoadSprite::doResourceByCommonIndex(105, false);
    });
    setIsHDPanel(true);
    m_slow=slow;
    TroopsController::getInstance()->resetGeneralToSend();
    m_startIndex = startIndex;
    m_targetIndex = targetIndex;
    m_haveOwner = haveOwner;
    m_rally = rally;
    m_bType = bType;
    m_wtIndex = wtIndex;
    m_other = other;
    m_targetType = targetType;
    auto tmpCCB = CCBLoadFile("March02",this,this);
    this->setContentSize(tmpCCB->getContentSize());

    int dy = 852 - CCDirector::sharedDirector()->getWinSize().height;
    if (CCCommonUtils::isIosAndroidPad())
    {
        dy = 2048 - CCDirector::sharedDirector()->getWinSize().height;
    }
    m_bg->setPreferredSize(m_bg->getContentSize() + CCSize(0, -dy));
    m_renderBG->setPreferredSize(m_renderBG->getContentSize() + CCSize(0, -dy));
    m_renderBG->setPositionY(m_renderBG->getPositionY() + dy);
    m_downNode->setPositionY(m_downNode->getPositionY() + dy);
    m_infoList->setPositionY(m_infoList->getPositionY() + dy);
    m_infoList->setContentSize(m_infoList->getContentSize() - CCSize(0, dy));
    m_tabView = CCMultiColTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setMultiColTableViewDelegate(this);
//    m_tabView->setTouchPriority(Touch_Popup);
    m_infoList->addChild(m_tabView);
//    changeBGMaxHeight(m_bg);//fusheng
//    m_box = CheckBox::create();
//    this->m_checkBoxContainer->addChild(m_box);
//    auto &generalInfo = GlobalData::shared()->generals.begin()->second;
//    if(generalInfo.state == COUNTRY_BATTLE){
//        m_box->setVisible(false);
//    }
    TroopsController::getInstance()->m_curSelHeroNum = 0;
    TroopsController::getInstance()->m_curLoadNum = 0;
    CCCommonUtils::setButtonTitle(m_quickBtn, _lang("105149").c_str());
//    CCCommonUtils::setButtonTitle(m_formationBtn1, _lang("103681").c_str());
//    CCCommonUtils::setButtonTitle(m_formationBtn2, _lang("103682").c_str());
    if (FunBuildController::getInstance()->getMainCityLv() >=15 && GlobalData::shared()->march_formation_switch == 2) {
        m_formationBtn1->setVisible(true);
        m_formationBtn2->setVisible(true);
    }
    else {
        m_formationBtn2->setVisible(false);
        m_formationBtn1->setVisible(false);
    }
    m_timeText->setString(_lang("108536"));
    m_msg1Label->setString("0/");
    m_msg3Label->setString(CC_ITOA(TroopsController::getInstance()->getMaxSoilder()));
    m_msg2Label->setString(CC_ITOA(TroopsController::getInstance()->m_curLoadNum));
    m_timeLabel->setString(CC_SECTOA(0));
    _modelLayer->setOpacity(200);
//    m_hintText1->setString(_lang("105147"));
    m_tmpArray = CCArray::create();
    generateData();//fusheng 把可用士兵放入m_tmpArray中
    if(m_tmpArray->count() == 0){
        m_hintText->setString(_lang("102151").c_str());
    }else{
        m_hintText->setString("");
    }
    switch (m_bType) {
        case MethodRally:
            CCCommonUtils::setButtonTitle(m_marchBtn, "");
            setTitleName(_lang("108582"));
            m_timeText->setString(_lang("108582"));
            break;
        case MethodUnion:
            CCCommonUtils::setButtonTitle(m_marchBtn, "");
            setTitleName(_lang("108582"));
            m_timeText->setString(_lang("108582"));
            break;
        case MethodYuanSolider:
            CCCommonUtils::setButtonTitle(m_marchBtn, "");
            setTitleName(_lang("115176"));
            m_timeText->setString(_lang("115176"));
            break;
    }
    m_marchBtn->setEffectStr("");
    updateInfo();
    this->scheduleOnce(schedule_selector(BattleView::loadResource), 0.02);
    updateLoadInfo(NULL);
    auto info = WorldController::getInstance()->getCityInfos().find(m_targetIndex);
    if(m_targetType==ActBossTile || (info != WorldController::getInstance()->getCityInfos().end() && (info->second.cityType == FieldMonster || info->second.cityType == MonsterTile || info->second.cityType == MonsterRange || info->second.cityType == ActBossTile))){
        m_iconContainer->removeAllChildren();
        auto sprite = CCLoadSprite::createSprite("Ui_tili.png");
        sprite->setScale(0.65);
        m_iconContainer->addChild(sprite);
//        int currt = WorldController::getInstance()->currentStamine;
//        int total = GlobalData::shared()->worldConfig.stamineMax;
        string str = _lang("3100112");
        
        if (info->second.cityType == ActBossTile || m_targetType==ActBossTile) {
            str += CC_ITOA(GlobalData::shared()->worldConfig.boss_decr);
        }else {
            int usePower = WorldController::getInstance()->getMonsterUsePower(m_targetIndex);
            if (usePower == 0) {
                
                usePower = GlobalData::shared()->worldConfig.stamineCostPerTime;
            }
            str += CC_ITOA(usePower);
        }
//        str += "   ";
//        str += std::string("") + CC_ITOA(currt) + "/" + CC_ITOA(total);
        m_msg2Label->setString(str);
    }else if(info != WorldController::getInstance()->getCityInfos().end() && info->second.cityType == ResourceTile){
        m_iconContainer->removeAllChildren();
        auto sprite = CCLoadSprite::createSprite(CCCommonUtils::getResourceIconByType(info->second.resource.type).c_str());
        m_iconContainer->addChild(sprite);
    }
    onClickQuickBtn(NULL, Control::EventType::TOUCH_DOWN);
    SoundController::sharedSound()->playEffects(Music_Sfx_world_click_attack);
    
    
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
        if(armyInfo.free == 0)
        {
            return false;
        }
        if (selectDragon) {
            
            selectDragon = false;//
            
            this->refreshDragonNumStatus();
            
            return true;
            
        }
        else
        {
            //fusheng 需要判断是否可以点击派出龙   出征部队人数已满

            
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);

            
            TroopsController::getInstance()->changeArrTime();
            
            int maxForceNum = TroopsController::getInstance()->getMaxSoilder();
            if (m_bType == MethodUnion || m_bType == MethodYuanSolider)
            {
                maxForceNum = MIN(m_rally, maxForceNum);
            }
            
            int total = 0;
            map<string, int>::iterator it;
            for (it = TroopsController::getInstance()->m_tmpBattleInfos.begin(); it != TroopsController::getInstance()->m_tmpBattleInfos.end(); it++) {
                if (TroopsController::getInstance()->m_tmpBattleInfos[it->first] > 0) {
                    total += TroopsController::getInstance()->m_tmpBattleInfos[it->first];
                }
            }
            
            int d = maxForceNum - total - 1;
            if(d >= 0){
                selectDragon = true;
                this->refreshDragonNumStatus();
            }
            
             return true;
    
        }
        
         return false;
    };
    
    listener->setSwallowTouches(true);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, m_bg);
    
    m_ProTiTanAPMaxWidth = m_ProTiTanAP->getContentSize().width;
    m_dragonMarchingLable->setString(_lang("500020"));
    refreshDragonStatus(nullptr);
  
    return true;
}

void BattleView::loadResource(float _time){
    CCLoadSprite::doResourceByCommonIndex(204, true);
    updateInfo();
}

void BattleView::setAddBtnState(){
    if(CCCommonUtils::getEffectValueByNum(COMMAND_EFFECT,false) > FLT_EPSILON){
        this->m_addBtn->setVisible(false);
        this->m_addIcon->setVisible(false);

        this->m_msg3Label->setColor(ccc3(86, 180, 29));
    }else{
        this->m_addBtn->setVisible(true);
        this->m_addIcon->setVisible(true);

        this->m_msg3Label->setColor(ccc3(255, 247, 255));
        this->getAnimationManager()->runAnimationsForSequenceNamed("Default Timeline");
    }
}

void BattleView::generateData(){
    TroopsController::getInstance()->m_tmpFreeSoldiers.clear();
    TroopsController::getInstance()->m_tmpBattleInfos.clear();
    TroopsController::getInstance()->m_tmpConfSoldiers.clear();
    m_tmpArray->removeAllObjects();
    std::map<string, ArmyInfo>::iterator it;
    for (it = GlobalData::shared()->armyList.begin(); it!=GlobalData::shared()->armyList.end(); it++) {
        if(it->second.free <= 0 || !it->second.isArmy){
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
                TroopsController::getInstance()->m_tmpFreeSoldiers[it->first] = (it->second).free;
                TroopsController::getInstance()->m_tmpConfSoldiers[it->first] = (it->second).free;
                continue;
            }
        }
        TroopsController::getInstance()->m_tmpFreeSoldiers[it->first] = (it->second).free;
        TroopsController::getInstance()->m_tmpConfSoldiers[it->first] = (it->second).free;
        
       
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

void BattleView::updateInfo()
{
    m_tabView->reloadData();
}

void BattleView::onEnter()
{
    CCNode::onEnter();
    loadResource();
    setAddBtnState();
    updateLoadInfo(NULL);
    switch (m_bType) {
        case MethodRally:
            setTitleName(_lang("108582"));
            break;
        case MethodUnion:
            setTitleName(_lang("108582"));
            break;
        case MethodYuanSolider:
            setTitleName(_lang("115176"));
            break;
            default:
            setTitleName(_lang("105150"));
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BattleView::updateArmyNumber), MSG_TROOPS_BACK, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BattleView::updateLoadInfo), MSG_TROOPS_BATTLE_LOAD, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BattleView::makeArrTime), MSG_TROOPS_TIME, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BattleView::titanNumChange), MSG_TITAN_COUNT_CHANGE, NULL);//fusheng 泰坦数量变化
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BattleView::titanInfoChange), MSG_TITAN_INFORMATION_RESET, NULL);
}

void BattleView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TROOPS_BATTLE_LOAD);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TROOPS_TIME);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_TROOPS_BACK);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_TITAN_COUNT_CHANGE);//fusheng 泰坦数量变化
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_TITAN_INFORMATION_RESET);
    CCNode::onExit();
}
void BattleView::titanInfoChange(CCObject* obj)
{
//    auto& children = m_tabView->getContainer()->getChildren();
//    for (auto child : children)
//    {
//        auto& childrenArr = child->getChildren();
//        SoldierCell *cell = dynamic_cast<SoldierCell*>(childrenArr.at(0));
//        if(cell){
//           
//            int sid = atoi(cell->m_soldierId.c_str());
//            if(sid>=107401&&sid<=107430)//fusheng 存在泰坦
//            {
//                string oldID = cell->m_soldierId;
//                cell->m_soldierId = GlobalData::shared()->titanInfo.titanId;
//                
//                if (oldID == GlobalData::shared()->titanInfo.titanId) {
//                    CCLOG("march titan ti li hui fu");
//                }
//                else
//                {
//                    if (TroopsController::getInstance()->m_tmpBattleInfos.find(oldID)!=TroopsController::getInstance()->m_tmpBattleInfos.end()) {
//                        
//                        TroopsController::getInstance()->m_tmpBattleInfos[cell->m_soldierId] = TroopsController::getInstance()->m_tmpBattleInfos[oldID];
//                        
//                        TroopsController::getInstance()->m_tmpBattleInfos[oldID] = 0;
//                        
//                        TroopsController::getInstance()->m_tmpFreeSoldiers[cell->m_soldierId] = TroopsController::getInstance()->m_tmpFreeSoldiers[oldID];
//                        
//                        TroopsController::getInstance()->m_tmpFreeSoldiers[oldID] = 0;
//                        
//                        TroopsController::getInstance()->m_tmpConfSoldiers[cell->m_soldierId] = TroopsController::getInstance()->m_tmpConfSoldiers[oldID];
//                        
//                        TroopsController::getInstance()->m_tmpConfSoldiers[oldID] = 0;
//                        
//                        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TITAN_COUNT_CHANGE
//                                                                                               , CCInteger::create(TroopsController::getInstance()->m_tmpBattleInfos[cell->m_soldierId])); //计算体力是否充足
//                        
//                        CCLOG("march update titan");
//                    }
//                }
//                
//                
//                
//                cell->refresh();
//            }
//            
//        }
//    }
    
    auto armyInfo = GlobalData::shared()->armyList[GlobalData::shared()->titanInfo.titanId];
    if(armyInfo.free != 0)//士兵里有龙
    {
        if (GlobalData::shared()->titanInfo.level == 1)
        {
            
        }
        else
        {
            string oldID = CCString::createWithFormat("%d", GlobalData::shared()->titanInfo.level + 107400 - 1)->getCString();
            
            if (TroopsController::getInstance()->m_tmpBattleInfos.find(oldID)!=TroopsController::getInstance()->m_tmpBattleInfos.end()) {
                
                TroopsController::getInstance()->m_tmpBattleInfos[GlobalData::shared()->titanInfo.titanId] = TroopsController::getInstance()->m_tmpBattleInfos[oldID];
                
                TroopsController::getInstance()->m_tmpBattleInfos[oldID] = 0;
                
                TroopsController::getInstance()->m_tmpFreeSoldiers[GlobalData::shared()->titanInfo.titanId] = TroopsController::getInstance()->m_tmpFreeSoldiers[oldID];
                
                TroopsController::getInstance()->m_tmpFreeSoldiers[oldID] = 0;
                
                TroopsController::getInstance()->m_tmpConfSoldiers[GlobalData::shared()->titanInfo.titanId] = TroopsController::getInstance()->m_tmpConfSoldiers[oldID];
                
                TroopsController::getInstance()->m_tmpConfSoldiers[oldID] = 0;
                
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TITAN_COUNT_CHANGE
                                                                                       , CCInteger::create(TroopsController::getInstance()->m_tmpBattleInfos[GlobalData::shared()->titanInfo.titanId])); //计算体力是否充足
                
                CCLOG("march update titan");
                
                //fusheng 需要刷新龙相关的节点
            }
           
        }
       
    }
    

    refreshDragonStatus(nullptr);
    TroopsController::getInstance()->changeArrTime();
}

void BattleView::generalSelect(){
//    std::string generalUid = GlobalData::shared()->generals.begin()->first;
//    TroopsController::getInstance()->m_generalToSend[0] = (m_box->isSelected() ? generalUid : "");
}

void BattleView::updateLoadInfo(CCObject* obj)
{
    int loadNum = TroopsController::getInstance()->m_curLoadNum;
    auto info = WorldController::getInstance()->getCityInfos().find(m_targetIndex);
    if(info != WorldController::getInstance()->getCityInfos().end() && info->second.cityType == ResourceTile){
        loadNum = loadNum / CCCommonUtils::getResourceLoadByType(info->second.resource.type);
        if(loadNum == 0){
            m_msg2Label->setColor(ccRED);
            auto fadeOut = CCFadeTo::create(0.6f, 127);
            auto fadeIn = CCFadeTo::create(0.6f, 255);
            auto delay = CCDelayTime::create(2.0f);
            CCSequence *action = CCSequence::create(fadeOut, fadeIn, delay, NULL);
            m_msg2Label->runAction(CCRepeatForever::create(action));
        }else{
            m_msg2Label->stopAllActions();
            m_msg2Label->setOpacity(255);
            m_msg2Label->setColor(ccc3(255, 255, 255));
        }
    }
    m_helpBtn->setVisible(true);
    m_stamineNode->setVisible(false);
    if(m_targetType==ActBossTile || (info != WorldController::getInstance()->getCityInfos().end() && (info->second.cityType == FieldMonster || info->second.cityType == MonsterTile || info->second.cityType == MonsterRange || info->second.cityType == ActBossTile))){
        m_iconContainer->removeAllChildren();
        auto sprite = CCLoadSprite::createSprite("Ui_tili.png");
        sprite->setScale(0.65);
        m_iconContainer->addChild(sprite);
//        int currt = WorldController::getInstance()->currentStamine;
//        int total = GlobalData::shared()->worldConfig.stamineMax;
        string str = _lang("3100112");
        if (info->second.cityType == ActBossTile || m_targetType==ActBossTile) {
            str += CC_ITOA(GlobalData::shared()->worldConfig.boss_decr);
        }else {
            int usePower = WorldController::getInstance()->getMonsterUsePower(m_targetIndex);
            if (usePower == 0) {
                usePower = GlobalData::shared()->worldConfig.stamineCostPerTime;
            }
            str += CC_ITOA(usePower);
        }
//        str += "   ";
//        str += std::string("") + CC_ITOA(currt) + "/" + CC_ITOA(total);
        m_msg2Label->setString(str);
        m_helpBtn->setVisible(false);
        m_stamineNode->setVisible(true);
    }else{
        string loadInfo = CC_CMDITOA(loadNum);
        m_msg2Label->setString(loadInfo.c_str());
    }
    
    int total = 0;
    map<string, int>::iterator it;
    for (it = TroopsController::getInstance()->m_tmpBattleInfos.begin(); it != TroopsController::getInstance()->m_tmpBattleInfos.end(); it++) {
        if (TroopsController::getInstance()->m_tmpBattleInfos[it->first] > 0) {
            total += TroopsController::getInstance()->m_tmpBattleInfos[it->first];
        }
    }

    m_msg1Label->setString((std::string("") + CC_ITOA(total) + "/").c_str());
    int maxSoilder = TroopsController::getInstance()->getMaxSoilder();
    if (m_bType == MethodUnion || m_bType == MethodYuanSolider)
    {
        maxSoilder = MIN(m_rally, maxSoilder);
    }
    m_msg3Label->setString(CC_ITOA(maxSoilder));
}
void BattleView::titanNumChange(CCObject* obj)
{
    auto info = WorldController::getInstance()->getCityInfos().find(m_targetIndex);
    
    CCLOG("titanNumChange" );
    if(m_targetType==ActBossTile || (info != WorldController::getInstance()->getCityInfos().end() && (info->second.cityType == FieldMonster || info->second.cityType == MonsterTile || info->second.cityType == MonsterRange || info->second.cityType == ActBossTile))){
        CCInteger *num = dynamic_cast<CCInteger*>(obj);
        if (num) {
            CCLOG("titanNumChange %d %d %d", num->getValue(),GlobalData::shared()->titanInfo.currentManual, GlobalData::shared()->titanInfo.costmanual);
            if (num->getValue() == 1)
            {
                if (GlobalData::shared()->titanInfo.currentManual<GlobalData::shared()->titanInfo.costmanual)
                {
                    this->m_marchBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::NORMAL)->setState(cocos2d::ui::Scale9Sprite::State::GRAY);
                    this->m_marchBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::HIGH_LIGHTED)->setState(cocos2d::ui::Scale9Sprite::State::GRAY);
                    this->m_marchBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::SELECTED)->setState(cocos2d::ui::Scale9Sprite::State::GRAY);
                    
                }
                else
                {
                    this->m_marchBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::NORMAL)->setState(cocos2d::ui::Scale9Sprite::State::NORMAL);
                    this->m_marchBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::HIGH_LIGHTED)->setState(cocos2d::ui::Scale9Sprite::State::NORMAL);
                    this->m_marchBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::SELECTED)->setState(cocos2d::ui::Scale9Sprite::State::NORMAL);
                }
            }
            else
            {
                this->m_marchBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::NORMAL)->setState(cocos2d::ui::Scale9Sprite::State::NORMAL);
                this->m_marchBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::HIGH_LIGHTED)->setState(cocos2d::ui::Scale9Sprite::State::NORMAL);
                this->m_marchBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::SELECTED)->setState(cocos2d::ui::Scale9Sprite::State::NORMAL);
            }
            //        m_marchBtn->setEnabled(true);
        }

    } //fusheng 花费体力的出征
    
}



SEL_CCControlHandler BattleView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickQuickBtn", BattleView::onClickQuickBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickMarchBtn", BattleView::onClickMarchBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHelpClick", BattleView::onHelpClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddClick", BattleView::onAddClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddStamineClick", BattleView::onAddStamineClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFormation1Click", BattleView::onFormation1Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFormation2Click", BattleView::onFormation2Click);
    return NULL;
}

bool BattleView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dragonPicNode", CCNode*, this->m_dragonPicNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dragonPicNode2", CCNode*, this->m_dragonPicNode2);
    
      CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dragonMarchingNode", CCNode*, this->m_dragonMarchingNode);
      CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dragonMarchingLable", CCLabelIF*, this->m_dragonMarchingLable);
      CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dragonNode", CCNode*, this->m_dragonNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dragonName", CCLabelIF*, this->m_dragonName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanAPTxt_0", CCLabelIF*, this->m_titanAPTxt_0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanAPTxt_1", CCLabelIF*, this->m_titanAPTxt_1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titanAPTxt_2", CCLabelIF*, this->m_titanAPTxt_2);
    
      CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ProTiTanAP", CCScale9Sprite*, this->m_ProTiTanAP);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_marchBtn", CCControlButton*, this->m_marchBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_quickBtn", CCControlButton*, this->m_quickBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg1Label", CCLabelIF*, this->m_msg1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg3Label", CCLabelIF*, this->m_msg3Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg2Label", CCLabelIF*, this->m_msg2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, this->m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeText", CCLabelIF*, this->m_timeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downNode", CCNode*, this->m_downNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconContainer", CCNode*, this->m_iconContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_alertNode", CCNode*, this->m_alertNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_checkBoxContainer", CCNode*, this->m_checkBoxContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_renderBG", CCScale9Sprite*, this->m_renderBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintText", CCLabelIF*, m_hintText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintText1", CCLabelIF*, m_hintText1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addBtn", CCControlButton*, m_addBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addIcon", CCSprite*, m_addIcon);
  
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpBtn", CCControlButton*, m_helpBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_formationBtn1", CCControlButton*, m_formationBtn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_formationBtn2", CCControlButton*, m_formationBtn2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stamineNode", CCNode*, this->m_stamineNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_2touchBtnBg", CCSprite*, this->m_2touchBtnBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_2touchBtn", CCSprite*, this->m_2touchBtn);
    return false;
}

void BattleView::onFormation1Click(CCObject *pSender, CCControlEvent event){
    m_index = 1;
    std::string formationStr = GlobalData::shared()->playerInfo.uid + "formation";
    formationStr.append(CC_ITOA(m_index));
    
    bool flag = CCUserDefault::sharedUserDefault()->getBoolForKey(formationStr.c_str(),false);
    bool hintFlag = false;
    if (flag == true) {
        //先判断总上限是否超出
        std::string forNum = GlobalData::shared()->playerInfo.uid + "forNum";
        forNum.append(CC_ITOA(m_index));
        int numTotal = CCUserDefault::sharedUserDefault()->getIntegerForKey(forNum.c_str(), 0);
        int maxForceNum = TroopsController::getInstance()->getMaxSoilder();
        if (m_bType == MethodUnion || m_bType == MethodYuanSolider)
        {
            maxForceNum = MIN(m_rally, maxForceNum);
        }
        if (numTotal > maxForceNum) {
            if (m_bType == MethodUnion || m_bType == MethodYuanSolider) {
                CCCommonUtils::flyHint("", "", _lang("103693"), 3, 0, true);
                return;
            }
            else {
                unselectAll();
                std::string forMethod = GlobalData::shared()->playerInfo.uid + "forMethod";
                forMethod.append(CC_ITOA(m_index));
                CCUserDefault::sharedUserDefault()->setStringForKey(forMethod.c_str(), "");
                CCUserDefault::sharedUserDefault()->setIntegerForKey(forNum.c_str(), 0);
                CCUserDefault::sharedUserDefault()->flush();
                CCCommonUtils::flyHint("", "", _lang("103684"), 3, 0, true);
                updateInfo();
                return;
            }
        }
        //判断单支兵种数量是否超出
        std::string forMethod = GlobalData::shared()->playerInfo.uid + "forMethod";
        forMethod.append(CC_ITOA(m_index));
        std::string method = CCUserDefault::sharedUserDefault()->getStringForKey(forMethod.c_str(), "");
        vector<std::string> vector1;
        CCCommonUtils::splitString(method, "|", vector1);
        int num = vector1.size();
        for (int i = 0; i < num; i++) {
            vector<std::string> vector2;
            CCCommonUtils::splitString(vector1[i], ",", vector2);
            if (vector2.size() == 2) {
                int selNum = atoi(vector2[1].c_str());
                int totalNum = TroopsController::getInstance()->m_tmpFreeSoldiers[vector2[0]] + TroopsController::getInstance()->m_tmpBattleInfos[vector2[0]];
                
                
                int sid = atoi(vector2[0].c_str());
                
                if(sid>=107401&&sid<=107430)//fusheng 存在泰坦
                {
                    auto armyInfo = GlobalData::shared()->armyList[GlobalData::shared()->titanInfo.titanId];
                    if(armyInfo.free == 0 && selNum != 0)//士兵里没有龙
                    {
                        hintFlag = true;
                        break;
                    }
                    
                }
                else
                {
                    if (selNum > totalNum) {
                        hintFlag = true;
                        break;
                    }
                }

                
                
            }
            else {
                CCCommonUtils::flyText(_lang("103686"));
                return;
            }
        }
        if (num == 0) {
            CCCommonUtils::flyText(_lang("103685"));
        }
        if (hintFlag) {
            CCCommonUtils::flyText(_lang("103686"));
            return;
        }
        else {
            unselectAll();
            for (int i = 0; i < num; i++) {
                vector<std::string> vector2;
                CCCommonUtils::splitString(vector1[i], ",", vector2);
                if (vector2.size() == 2) {
                    int sid = atoi(vector2[0].c_str());
                    
                    if(sid>=107401&&sid<=107430)//fusheng 存在泰坦
                    {
                        auto armyInfo = GlobalData::shared()->armyList[GlobalData::shared()->titanInfo.titanId];
                        if(armyInfo.free != 0 )//士兵里有龙
                        {
                            
                            selectDragon = atoi(vector2[1].c_str()) == 1?true:false;
                            
                            this->refreshDragonNumStatus();

                        }
                        
                    }
                    else
                    {
                        TroopsController::getInstance()->updateTmpBattleData(vector2[0], atoi(vector2[1].c_str()), vector2[0]);
                    }
                }
                else {
                    CCCommonUtils::flyText(_lang("103686"));
                    return;
                }
            }
            updateInfo();
        }
    }
    else {
        CCCommonUtils::flyText(_lang("103685"));
    }
}

void BattleView::onFormation2Click(CCObject *pSender, CCControlEvent event){
    m_index = 2;
    std::string formationStr = GlobalData::shared()->playerInfo.uid + "formation";
    formationStr.append(CC_ITOA(m_index));
    
    bool flag = CCUserDefault::sharedUserDefault()->getBoolForKey(formationStr.c_str(),false);
    bool hintFlag = false;
    if (flag == true) {
        //先判断总上限是否超出
        std::string forNum = GlobalData::shared()->playerInfo.uid + "forNum";
        forNum.append(CC_ITOA(m_index));
        int numTotal = CCUserDefault::sharedUserDefault()->getIntegerForKey(forNum.c_str(), 0);
        int maxForceNum = TroopsController::getInstance()->getMaxSoilder();
        if (m_bType == MethodUnion || m_bType == MethodYuanSolider)
        {
            maxForceNum = MIN(m_rally, maxForceNum);
        }
        if (numTotal > maxForceNum) {
            if (m_bType == MethodUnion || m_bType == MethodYuanSolider) {
                CCCommonUtils::flyHint("", "", _lang("103693"), 3, 0, true);
                return;
            }
            else {
                unselectAll();
                std::string forMethod = GlobalData::shared()->playerInfo.uid + "forMethod";
                forMethod.append(CC_ITOA(m_index));
                CCUserDefault::sharedUserDefault()->setStringForKey(forMethod.c_str(), "");
                CCUserDefault::sharedUserDefault()->setIntegerForKey(forNum.c_str(), 0);
                CCUserDefault::sharedUserDefault()->flush();
                CCCommonUtils::flyHint("", "", _lang("103684"), 3, 0, true);
                updateInfo();
                return;
            }
        }
        //判断单支兵种数量是否超出
        std::string forMethod = GlobalData::shared()->playerInfo.uid + "forMethod";
        forMethod.append(CC_ITOA(m_index));
        std::string method = CCUserDefault::sharedUserDefault()->getStringForKey(forMethod.c_str(), "");
        vector<std::string> vector1;
        CCCommonUtils::splitString(method, "|", vector1);
        int num = vector1.size();
        for (int i = 0; i < num; i++) {
            vector<std::string> vector2;
            CCCommonUtils::splitString(vector1[i], ",", vector2);
            if (vector2.size() == 2) {
                int selNum = atoi(vector2[1].c_str());
                int totalNum = TroopsController::getInstance()->m_tmpFreeSoldiers[vector2[0]] + TroopsController::getInstance()->m_tmpBattleInfos[vector2[0]];
                
                int sid = atoi(vector2[0].c_str());
                
                if(sid>=107401&&sid<=107430)//fusheng 存在泰坦
                {
                    auto armyInfo = GlobalData::shared()->armyList[GlobalData::shared()->titanInfo.titanId];
                    if(armyInfo.free == 0 && selNum != 0)//士兵里没有龙
                    {
                        hintFlag = true;
                        break;
                    }
                    
                }
                else
                {
                    if (selNum > totalNum) {
                        hintFlag = true;
                        break;
                    }
                }
                
               
            }
            else {
                CCCommonUtils::flyText(_lang("103686"));
                return;
            }
        }
        if (num == 0) {
            CCCommonUtils::flyText(_lang("103685"));
        }
        if (hintFlag) {
            CCCommonUtils::flyText(_lang("103686"));
            return;
        }
        else {
            unselectAll();
            for (int i = 0; i < num; i++) {
                vector<std::string> vector2;
                CCCommonUtils::splitString(vector1[i], ",", vector2);
                if (vector2.size() == 2) {
                    
                    
                    int sid = atoi(vector2[0].c_str());
                    
                    if(sid>=107401&&sid<=107430)//fusheng 存在泰坦
                    {
                        auto armyInfo = GlobalData::shared()->armyList[GlobalData::shared()->titanInfo.titanId];
                        if(armyInfo.free != 0 )//士兵里有龙
                        {
                            
                            selectDragon = atoi(vector2[1].c_str()) == 1?true:false;
                            
                            this->refreshDragonNumStatus();
                            
                        }
                        
                    }
                    else
                    {
                         TroopsController::getInstance()->updateTmpBattleData(vector2[0], atoi(vector2[1].c_str()), vector2[0]);
                    }

                    
                    
                    
                   
                }
                else {
                    CCCommonUtils::flyText(_lang("103686"));
                    return;
                }
            }
            updateInfo();
        }
    }
    else {
        CCCommonUtils::flyText(_lang("103685"));
    }
}

void BattleView::onHelpClick(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("102194")));
}

void BattleView::onClickMarchBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    
    if( this->m_marchBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::NORMAL)->getState() == cocos2d::ui::Scale9Sprite::State::GRAY)
    {
        
        CCCommonUtils::flyHint("", "", _lang("500010"));//fusheng 需要文本
        return ;
    }
    
    bool haveSoldier = false;
    for (auto &troop:TroopsController::getInstance()->m_tmpBattleInfos) {
        if (troop.second > 0) {
            haveSoldier = true;
        }
    }
    

    auto info = WorldController::getInstance()->getCityInfos().find(m_targetIndex);
    
    if (TroopsController::getInstance()->isHaveDefHeroInBattle() && TroopsController::getInstance()->m_isNotice) {
        YesNoDialog::show( _lang("102193").c_str() ,
                          CCCallFunc::create(this, callfunc_selector(BattleView::march)),
                          0,
                          false,
                          NULL,
                          CCCallFunc::create(this, callfunc_selector(BattleView::notice)));
    }
    else
    {
        if(info->second.cityType == ResourceTile || info->second.cityType == OriginTile || info->second.cityType == CampTile)
        {
            WorldMarchCheck* cmd = new WorldMarchCheck(m_targetIndex,info->second.cityType,WorldController::getInstance()->m_alertStateFlag);
            cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(BattleView::checkCanMarch), NULL));
            cmd->sendAndRelease();
            GameController::getInstance()->showWaitInterface();
        }
        else
        {
            march();
        }
    }
}

void BattleView::checkCanMarch(CCObject* param){
    GameController::getInstance()->removeWaitInterface();
    NetResult* result = dynamic_cast<NetResult*>(param);
    
    if(result && result->getErrorCode() == Error_OP_FAILURE)
    {
        CCLOGFUNCF("出现错误,关闭此面板");
        PopupViewController::getInstance()->removeAllPopupView();
        
    }else if(result && (int)result->getErrorCode() == TITAN_NO_ENOUGH_TILI)
    {
        TitanController::getInstance()->handleErrorCode((int)TITAN_NO_ENOUGH_TILI);
        PopupViewController::getInstance()->removeAllPopupView();
    }
    else
    {
        auto dic = _dict(result->getData());
        if(dic){
            bool ok = dic->valueForKey("needBreakShield")->boolValue();
            if(ok){
                YesNoDialog::show(_lang("101438").c_str(),CCCallFunc::create(this, callfunc_selector(BattleView::march)));
            }else{
                
                auto info = WorldController::getInstance()->getCityInfos().find(m_targetIndex);
                
                if (info->second.cityType == ResourceTile && info->second.playerName.empty()) {
                    bool marchingAlertFlag = false;
                    for(auto it :WorldController::getInstance()->m_marchInfo)
                    {
                        if(it.second.stateType == StateMarch && it.second.endPointIndex == m_targetIndex)
                        {
                            marchingAlertFlag = true;
                            break;
                        }
                    }
                    if (marchingAlertFlag) {
                        YesNoDialog::marchAlertShow(_lang("108897").c_str(),CCCallFunc::create(this, callfunc_selector(BattleView::march)),CCCallFunc::create(this, callfunc_selector(BattleView::closeSelf)));
                    }
                    else
                        march();
                }
                else
                    march();
            }
        }else{
             march();
        }
    }
}
void BattleView::updateArmyNumber(CCObject* obj)
{
    std::map<string, ArmyInfo>::iterator it;
    m_tmpArray->removeAllObjects();
    for (it = GlobalData::shared()->armyList.begin(); it!=GlobalData::shared()->armyList.end(); it++) {
        if(!it->second.isArmy){
            continue;
        }
        int sid = atoi(it->second.armyId.c_str());
        if(sid>=107401&&sid<=107430)//fusheng 加一个保障
        {
            if (it->first != GlobalData::shared()->titanInfo.titanId) {
                continue;
            }
            else
            {
                TroopsController::getInstance()->m_tmpFreeSoldiers[it->first] = (it->second).free;
                TroopsController::getInstance()->m_tmpConfSoldiers[it->first] = (it->second).free;
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
    refreshDragonStatus(nullptr);
}

void BattleView::march()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           , CCString::createWithFormat("ED_attack"));
    auto dict = CCDictionary::create();
    auto soldierDict = TroopsController::getInstance()->saveBattle();
    if(soldierDict->count() == 0){
        YesNoDialog::showYesDialog(_lang("105112").c_str());
        return;
    }

    dict->setObject(soldierDict, "soldier");
    dict->setObject(CCInteger::create(m_targetIndex), "targetIndex");
    dict->setObject(CCInteger::create(m_haveOwner), "haveOwner");
    CCArray *arr = CCArray::create();
    dict->setObject(arr, "generals");
    std::string other = "";
    switch (m_bType) {
        case MethodRally:
            other = CC_ITOA(m_wtIndex);
            break;
        case MethodUnion:
            other = m_other;
            if(TroopsController::getInstance()->m_saveSoldierNum > m_rally){
                YesNoDialog::showYesDialog(_lang_1("115194",CC_ITOA(m_rally)));
                return;
            }
            break;
        case MethodYuanSolider:
            if(TroopsController::getInstance()->m_saveSoldierNum > m_rally){
                YesNoDialog::showYesDialog(_lang_1("115166",CC_ITOA(m_rally)));
                return;
            }
            break;
    }
    SoundController::sharedSound()->playEffects(Music_Sfx_world_click_march);
    m_marchBtn->setEnabled(false);
    if(SceneController::getInstance()->currentSceneId==SCENE_ID_WORLD && m_bType!=MethodRally){
        if (GlobalData::shared()->playerInfo.regCountry == "JP")
        {
            ///日本攻击忍者打点
            auto info = WorldController::getInstance()->getCityInfos().find(m_targetIndex);
            if (info != WorldController::getInstance()->getCityInfos().end())
            {
                if (info->second.cityType == FieldMonster)
                {
                    string mId = info->second.fieldMonsterInfo.monsterId;
                    int imId = atoi(mId.c_str());
                    if (imId >= 900125 && imId <= 900151) //忍者 ID
                    {
                        GameController::getInstance()->callXCApi("action=riben_renzhe");
                        string killer = CCUserDefault::sharedUserDefault()->getStringForKey("riben.renzhe.killer");
                        if (killer.empty())
                        {
                            CCUserDefault::sharedUserDefault()->setStringForKey("riben.renzhe.killer", "1");
                            CCUserDefault::sharedUserDefault()->flush();
                            GameController::getInstance()->callXCApi("action=riben_renzhe_killer");
                        }
                    }
                }
            }
        }
        WorldMapView::instance()->afterMarchDeploy(dict,m_bType,other,m_targetType);
        //PopupViewController::getInstance()->removeAllPopupView();  //fusheng 先不关闭弹窗
    }else{
        if(m_bType == -1){
            return;
        }
        auto cmd = new WorldMarchCommand(m_targetIndex, (MarchMethodType)m_bType,1,soldierDict,NULL,-1, arr,m_wtIndex,m_other);
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(BattleView::callSuccess), NULL));
        cmd->sendAndRelease();
    }
}

void BattleView::callSuccess(CCObject* param){
    int type = m_bType;
    PopupViewController::getInstance()->removeAllPopupView();
    if(type==MethodRally)
    {
        auto info = WorldController::getInstance()->getCityInfos().find(m_targetIndex);
        if (info->second.cityType == ActBossTile || m_targetType==ActBossTile) {
            WorldController::getInstance()->currentStamine -= GlobalData::shared()->worldConfig.boss_decr;
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CURRENT_STAMINE);
        }
        
        NetResult* result = dynamic_cast<NetResult*>(param);
        auto dic = _dict(result->getData());
        if(dic){
            auto teamInfo = _dict(dic->objectForKey("allianceBattleTeam"));
            if(teamInfo){
                AllianceTeamInfo*  team = new AllianceTeamInfo();
                team->parseInfo(teamInfo);
                PopupViewController::getInstance()->addPopupInView(AllianceWarDetailView::create(team));
                team->autorelease();
            }
        }

    }else{
        if (m_targetType==ActBossTile) {
            WorldController::getInstance()->currentStamine -= GlobalData::shared()->worldConfig.boss_decr;
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CURRENT_STAMINE);
        }
        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD);
    }
}

void BattleView::notice()
{
    TroopsController::getInstance()->m_isNotice = !TroopsController::getInstance()->m_isNotice;
}

void BattleView::selectAll(){
    int totalNum = 0;
    int maxForceNum = TroopsController::getInstance()->getMaxSoilder();
    if (m_bType == MethodUnion || m_bType == MethodYuanSolider)
    {
        maxForceNum = MIN(m_rally, maxForceNum);
    }
//    if(m_box->isVisible()){
//        m_box->setSelect(true);
//        TroopsController::getInstance()->m_generalToSend[0] = GlobalData::shared()->generals.begin()->first;
//    }
    for (int i = 0; i<m_tmpArray->count(); i++) {
        std::string m_soldierId = dynamic_cast<CCString*>(m_tmpArray->objectAtIndex(i))->getCString();
        TroopsController::getInstance()->updateTmpBattleData(m_soldierId, 0, m_soldierId);
    }

    bool isResourceLimit = false;
    int totalResource = 0;
    float loadPerUnit = 0;
    auto info = WorldController::getInstance()->getCityInfos().find(m_targetIndex);
    if(info != WorldController::getInstance()->getCityInfos().end() && info->second.cityType == ResourceTile){
        bool isInMarch = false;
        loadPerUnit = CCCommonUtils::getResourceLoadByType(info->second.resource.type);
        for (auto march = WorldController::getInstance()->m_marchInfo.begin(); march != WorldController::getInstance()->m_marchInfo.end(); march++) {
            if (march->second.endPointIndex == m_targetIndex) {
                isInMarch = true;
                break;
            }
        }
        if(!isInMarch){
            isResourceLimit = true;
            totalResource = info->second.resource.sum+3;//防止采集剩余容错3个资源量
        }
    }

    if(isBegin)
    {
        isBegin = false;
      
    }
    else
    {
        auto armyInfo = GlobalData::shared()->armyList[GlobalData::shared()->titanInfo.titanId];
        if(armyInfo.free != 0 && maxForceNum>0)//士兵里有龙
        {
            totalNum = 1;
            
            selectDragon = true;
            
            refreshDragonNumStatus();
            
            
            
            
            
            if(m_targetType==ActBossTile || (info != WorldController::getInstance()->getCityInfos().end() && (info->second.cityType == FieldMonster || info->second.cityType == MonsterTile || info->second.cityType == MonsterRange || info->second.cityType == ActBossTile))){
                
                if(GlobalData::shared()->titanInfo.currentManual<GlobalData::shared()->titanInfo.costmanual)
                {
                    this->m_marchBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::NORMAL)->setState(cocos2d::ui::Scale9Sprite::State::GRAY); //fusheng onEnter之后注册通知函数  第一次在这里还没注册
                    this->m_marchBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::HIGH_LIGHTED)->setState(cocos2d::ui::Scale9Sprite::State::GRAY);
                    this->m_marchBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::SELECTED)->setState(cocos2d::ui::Scale9Sprite::State::GRAY);
                }
                
            } //fusheng 花费体力的出征
            
            
        }
    }
    
    
    for (int i = 0; i<m_tmpArray->count(); i++) {//fusheng 只判断m_tmpArray里的
        std::string m_soldierId = dynamic_cast<CCString*>(m_tmpArray->objectAtIndex(i))->getCString();
        int sid = CCString::create(m_soldierId)->intValue();
        
        
        int tmpCntNum = 0;
        tmpCntNum = TroopsController::getInstance()->m_tmpConfSoldiers[m_soldierId];
        tmpCntNum = totalNum + tmpCntNum >= maxForceNum ? maxForceNum - totalNum : tmpCntNum;
        if(isResourceLimit){
            float loadPerSoldier = ArmyController::getInstance()->getLoad(&GlobalData::shared()->armyList[m_soldierId], TroopsController::getInstance()->m_generalToSend.size());
            if(totalResource < int(int(TroopsController::getInstance()->m_curLoadNum + tmpCntNum * loadPerSoldier) / loadPerUnit)){
                int num = ceil((totalResource * loadPerUnit - TroopsController::getInstance()->m_curLoadNum) / loadPerSoldier);
                tmpCntNum = (num > tmpCntNum ? tmpCntNum : num);
            }
        }
        TroopsController::getInstance()->updateTmpBattleData(m_soldierId, tmpCntNum, m_soldierId);
        auto info = WorldController::getInstance()->getCityInfos().find(m_targetIndex);
        
        
        
        if(totalNum + tmpCntNum >= maxForceNum){
            break;
        }
        if(isResourceLimit && (totalResource*loadPerUnit < int(int(TroopsController::getInstance()->m_curLoadNum) * 1.0))){
            break;
        }
        totalNum += tmpCntNum;
    }
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
//                                                                           , CCString::createWithFormat("ED_quick"));//fushegn del
}

void BattleView::unselectAll(){

    
    this->m_marchBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::NORMAL)->setState(cocos2d::ui::Scale9Sprite::State::NORMAL);//fusheng onEnter之后注册通知函数  第一次在这里还没注册
    this->m_marchBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::HIGH_LIGHTED)->setState(cocos2d::ui::Scale9Sprite::State::NORMAL);
    this->m_marchBtn->getBackgroundSpriteForState(cocos2d::extension::Control::State::SELECTED)->setState(cocos2d::ui::Scale9Sprite::State::NORMAL);

    for (int i = 0; i<m_tmpArray->count(); i++) {
        std::string m_soldierId = dynamic_cast<CCString*>(m_tmpArray->objectAtIndex(i))->getCString();
        TroopsController::getInstance()->updateTmpBattleData(m_soldierId, 0, m_soldierId);
    }
    
    selectDragon = false;
    refreshDragonNumStatus();
}

void BattleView::onAddClick(CCObject * pSender, Control::EventType pCCControlEvent){
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

void BattleView::onAddStamineClick(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->addPopupInView(UseResToolView::create(13));
}

void BattleView::onClickQuickBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if(m_tmpArray->count() == 0){
        return;
    }
    if(int(TroopsController::getInstance()->m_curLoadNum) == 0){
//        TroopsController::getInstance()->m_curLoadNum = 0;
        selectAll();
    }else{
        unselectAll();
//        TroopsController::getInstance()->m_curLoadNum = 0;
    }
    
    TroopsController::getInstance()->changeArrTime();
    auto& children = m_tabView->getContainer()->getChildren();
    for (auto child : children)
    {
        auto& childrenArr = child->getChildren();
        SoldierCell *cell = dynamic_cast<SoldierCell*>(childrenArr.at(0));
        if(cell){
            cell->refresh();
        }
    }
//    
//    CCArray *arr = m_tabView->getContainer()->getChildren();
//    int i = 0;
//    while(i < arr->count()){
//        CCArray *children = dynamic_cast<CCNode*>(arr->objectAtIndex(i++))->getChildren();
//        SoldierCell *cell = dynamic_cast<SoldierCell*>(children->objectAtIndex(0));
//        if(cell){
//            cell->refresh();
//        }
//    }
}

void BattleView::makeArrTime(CCObject* obj)
{
    int spd = TroopsController::getInstance()->m_minSpeed;
    if (!spd) {
        m_timeLabel->setString(CC_SECTOA(0));
        return;
    }
    auto info = WorldController::getInstance()->getCityInfos().find(m_targetIndex);
    MarchMethodType mType = MethodBattle;
    if(m_bType >= 0){
        mType = MarchMethodType(m_bType);
    }

    int time = WorldController::getInstance()->getMarchTime(m_startIndex, m_targetIndex, info->second.cityType, mType) / 1000;
    time*=m_slow;
    m_timeLabel->setString(CC_SECTOA(time));
}

void BattleView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell)
{
    SoldierCell* soldierCell = dynamic_cast<SoldierCell*>(cell);
    if (soldierCell && isTouchInside(m_infoList, m_tabView->m_pCurTouch)) {
        soldierCell->cellTouchEnded(m_tabView->m_pCurTouch);
    }
}

cocos2d::CCSize BattleView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1410, 250);
    }
    return CCSize(600, 150);
}

CCTableViewCell* BattleView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx)
{
    if(idx >= m_tmpArray->count()){
        return NULL;
    }
    
    SoldierCell* cell = (SoldierCell*)table->dequeueGrid();
    if(idx < m_tmpArray->count()){
        std::string gid = dynamic_cast<CCString*>(m_tmpArray->objectAtIndex(idx))->getCString();
        int num = 0;
        if(TroopsController::getInstance()->m_tmpBattleInfos.find(gid) != TroopsController::getInstance()->m_tmpBattleInfos.end())
        {
            num = TroopsController::getInstance()->m_tmpBattleInfos[gid];
        }
        if(cell){
            cell->setData(gid, num, m_bType, m_rally);
            cell->setTag(idx);
        }else{
            cell = SoldierCell::create(gid, num, m_bType, m_rally);
            cell->setTag(idx);
        }
    }
    return cell;
}

ssize_t BattleView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    int num = ceil(m_tmpArray->count() / numPerRow);
    return num;
}

unsigned int BattleView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return numPerRow;
}

CCNode* BattleView::getGuideNode(string _key)
{
    if (_key=="attack") {
        return m_marchBtn;
    }
    else if (_key=="quick") {
        return m_quickBtn;
    }
    return NULL;
}

void BattleView::refreshDragonNumStatus()
{
    auto armyInfo = GlobalData::shared()->armyList[GlobalData::shared()->titanInfo.titanId];
    if(armyInfo.free != 0 )//士兵里有龙
    {
   
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TITAN_COUNT_CHANGE
                                                                               , CCInteger::create(selectDragon?1:0));
    
        m_2touchBtn->setPositionX(selectDragon?84:25);
        
        
        m_2touchBtn->setSpriteFrame(selectDragon?"nb_checkboxBlockON.png":"nb_checkboxBlockOFF.png");
    
        TroopsController::getInstance()->updateTmpBattleData(GlobalData::shared()->titanInfo.titanId, selectDragon?1:0, GlobalData::shared()->titanInfo.titanId);
    }
}

void BattleView::refreshDragonStatus(CCObject* obj)
{
    auto armyInfo = GlobalData::shared()->armyList[GlobalData::shared()->titanInfo.titanId];
    if(armyInfo.free != 0)//士兵里有龙
    {
        m_dragonNode->setVisible(true);
        m_dragonMarchingNode->setVisible(false);
       
        string name = CCCommonUtils::getNameById(GlobalData::shared()->titanInfo.titanId);
        m_dragonName->setString(name);
   
        
        string id = CC_ITOA(GlobalData::shared()->titanInfo.tid);
        
        auto picName = CCCommonUtils::getPropById(id, "dragonMarchUI");
       
        auto spr = CCLoadSprite::createSprite(picName.c_str());
       
        m_dragonPicNode->removeAllChildren();
        m_dragonPicNode->addChild(spr);
        
        if(GlobalData::shared()->titanInfo.maxManual!=0)
        {
            float ratio =((float)GlobalData::shared()->titanInfo.currentManual)/GlobalData::shared()->titanInfo.maxManual;
            
            auto size = this->m_ProTiTanAP->getContentSize();
            
            size.width = m_ProTiTanAPMaxWidth * ratio;
            
            
            m_ProTiTanAP->setVisible(ratio != 0);
            
            auto oriSize = m_ProTiTanAP->getOriginalSize();
            
            if (size.width<oriSize.width) {
                
                this->m_ProTiTanAP->setContentSize(oriSize);
                
            }
            else
            {
                this->m_ProTiTanAP->setContentSize(size);
                
            }
            
            
            
           
            this->m_titanAPTxt_0->setString("(");
            this->m_titanAPTxt_1->setString(CCString::createWithFormat("%d",GlobalData::shared()->titanInfo.currentManual<0?0:GlobalData::shared()->titanInfo.currentManual)->getCString());
            this->m_titanAPTxt_2->setString(CCString::createWithFormat("/%d)",GlobalData::shared()->titanInfo.maxManual)->getCString());
            
            vector<cocos2d::CCLabelIF *> labels;
            labels.push_back(m_titanAPTxt_0);
            labels.push_back(m_titanAPTxt_1);
            labels.push_back(m_titanAPTxt_2);
            
            NBCommonUtils::arrangeLabel(labels);

            
        }
        else
        {
            
            auto size = this->m_ProTiTanAP->getContentSize();
            size.width = 0;
            this->m_ProTiTanAP->setContentSize(size);

            
        }
        
        
        

    }
    else
    {
        
        string id = CC_ITOA(GlobalData::shared()->titanInfo.tid);
        
        auto picName = CCCommonUtils::getPropById(id, "dragonMarchUI");
        
        auto spr = CCLoadSprite::createSprite(picName.c_str());
        
        m_dragonPicNode2->removeAllChildren();
        m_dragonPicNode2->addChild(spr);
        
        
        m_dragonNode->setVisible(false);
        m_dragonMarchingNode->setVisible(true);
    }
}

#pragma mark -
#pragma mark HeroCell Part

SoldierCell* SoldierCell::create(string itemId, int num, int type, int rally)
{
    SoldierCell *ret = new SoldierCell();
    if (ret && ret->init(itemId, num, type, rally)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool SoldierCell::init(string itemId, int num, int type, int rally)
{
    bool ret = true;
    CCBLoadFile("March2",this,this);
    setContentSize(CCSize(604, 134));
    
//    m_subBtn->setTouchPriority(Touch_Popup_Item);
//    m_addBtn->setTouchPriority(Touch_Popup_Item);
    
//    auto m_sliderBg = CCLoadSprite::createScale9Sprite("huadongtiao3.png");
//    m_sliderBg->setInsetBottom(5);
//    m_sliderBg->setInsetLeft(5);
//    m_sliderBg->setInsetRight(5);
//    m_sliderBg->setInsetTop(5);
//    m_sliderBg->setAnchorPoint(ccp(0.5,0.5));
//    m_sliderBg->setPosition(ccp(252/2, 20));
//    m_sliderBg->setContentSize(CCSize(252,18));
//    
//    auto bgSp = CCLoadSprite::createSprite("huadongtiao4.png");
//    bgSp->setVisible(false);
//    auto proSp = CCLoadSprite::createSprite("huadongtiao4.png");
//    auto thuSp = CCLoadSprite::createSprite("huadongtiao1.png");
//    
//    m_slider = CCSliderBar::createSlider(m_sliderBg, proSp, thuSp);
//    m_slider->setMinimumValue(0.0f);
//    m_slider->setMaximumValue(1.0f);
//    m_slider->setProgressScaleX(248/proSp->getContentSize().width);
//    m_slider->setTag(1);
//    m_slider->setLimitMoveValue(25);
////    m_slider->setTouchPriority(Touch_Popup);
//    m_slider->addTargetWithActionForControlEvents(this, cccontrol_selector(SoldierCell::valueChange), CCControlEventValueChanged);
//    m_sliderNode->addChild(m_slider, 1);
    
    m_slider = NBSlider::create("nb_bar_bg.png", "nb_bar_pro.png", "nb_cursor_icon.png",NBSlider::TextureResType::PLIST);
    m_slider->setCapInsets(Rect(8, 1, 30, 13));
    m_slider->setContentSize(Size(230, 15));
    //    m_slider->setPosition(ccp(-60, -59));//fusheng d
    if (CCCommonUtils::isIosAndroidPad()) {
        //        m_slider->setPosition(ccp(-137, -56));//fusheng d
        m_slider->setScaleX(2.6);
        m_slider->setScaleY(2.0);
    }
    //    m_slider->addTargetWithActionForControlEvents(this, cccontrol_selector(ProductionSoldiersView::moveSlider), CCControlEventValueChanged);//fusheng d
    m_slider->addEventListener(CC_CALLBACK_2(SoldierCell::valueChange, this));
    m_slider->setPosition(-230 / 2, -15 / 2);
    m_sliderNode->addChild(m_slider, 1);
    
    auto editSize = m_editNode->getContentSize();
    m_editBox = CCEditBox::create(editSize, CCLoadSprite::createScale9Sprite("cz_bt_1.png"));
    m_editBox->setInputMode(kEditBoxInputModeNumeric);
    m_editBox->setText("0");
    m_editBox->setDelegate(this);
//    m_editBox->setTouchPriority(Touch_Popup);
    m_editBox->setMaxLength(12);
    m_editBox->setReturnType(kKeyboardReturnTypeDone);
    m_editBox->setPosition(ccp(editSize.width/2, editSize.height/2));
    m_editNode->addChild(m_editBox);

    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(204, false);
    });

    setData(itemId, num, type, rally);
    return ret;
}

void SoldierCell::setData(string itemId, int num, int type, int rally)
{
    m_soldierId = itemId;
    m_num = num;
    m_type = type;
    m_rally = rally;
    string name = CCCommonUtils::getNameById(m_soldierId);
    string picName = GlobalData::shared()->armyList[m_soldierId].getHeadIcon();
    m_cntNum = TroopsController::getInstance()->m_tmpConfSoldiers[m_soldierId];
//    m_slider->setEnabled(true);
    m_nameLabel->setString(name.c_str());
    
    m_picNode->removeAllChildren();
    
    int id = atoi(itemId.c_str());
    m_iconPath = picName;
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

    
    this->m_levelNode->removeAllChildren();
    string num1 = m_soldierId.substr(m_soldierId.size()-2);
    
//    if (id>= 107401 && id<= 107430) {//fusheng 泰坦 计算罗马数字时不加一
//        auto pic1= CCCommonUtils::getRomanSprite(atoi(num1.c_str()));
//        m_levelNode->addChild(pic1);
//    }
//    else
//    {
//            }
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

void SoldierCell::refresh(){
    int num = 0;
    if(TroopsController::getInstance()->m_tmpBattleInfos.find(m_soldierId) != TroopsController::getInstance()->m_tmpBattleInfos.end())
    {
        num = TroopsController::getInstance()->m_tmpBattleInfos[m_soldierId];
    }

    setData(m_soldierId, num, m_type, m_rally);
}

void SoldierCell::valueChange(Ref *pSender, NBSlider::EventType type)
{
    if (m_cntNum<=0) {
        return;
    }
    
    bool numChange = false;
    
    int oldNum = CCString::create(m_editBox->getText())->intValue();
    
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    float value = m_slider->getValue();
    int curNum = round(value * 1.0 * m_cntNum);
    int retNum = TroopsController::getInstance()->updateTmpBattleData(m_soldierId, curNum, m_soldierId);
    m_editBox->setText(CC_ITOA(retNum));
    
    if (oldNum != retNum) { //fusheng 数量改变
        numChange = true;
    }
    
    TroopsController::getInstance()->changeArrTime();
    
    int maxForceNum = TroopsController::getInstance()->getMaxSoilder();
    if (m_type == MethodUnion || m_type == MethodYuanSolider)
    {
        maxForceNum = MIN(m_rally, maxForceNum);
    }
    
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
        if (curNum >= 0)
            m_slider->setValue(curNum * 1.0 / m_cntNum);
        else
            m_slider->setValue(0);
    }
    
    
    int soldierId = CCString::create(m_soldierId)->intValue();
    
//    if (soldierId<=107430&&soldierId>=107401) {
//        //fusheng 改变泰坦个数
//        if (numChange) {
//            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TITAN_COUNT_CHANGE
//                                                                                   , CCInteger::create(retNum));
//        }
//    }
}

void SoldierCell::onEnter() {
    CCNode::onEnter();
}

void SoldierCell::onExit() {
    CCNode::onExit();
}

SEL_CCControlHandler SoldierCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSubClick", SoldierCell::onSubClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddClick", SoldierCell::onAddClick);

    return NULL;
}

void SoldierCell::onSubClick(CCObject * pSender, Control::EventType pCCControlEvent){
    int num = m_slider->getValue() * m_cntNum - 1;
    if(num < 0){
        num = 0;
    }
    m_slider->setValue(num * 1.0f / m_cntNum);
    
    int soldierId = CCString::create(m_soldierId)->intValue();
    
//    if (soldierId<=107430&&soldierId>=107401) {
//        //fusheng 改变泰坦个数
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TITAN_COUNT_CHANGE
//                                                                               , CCInteger::create(num));
//    }
}

void SoldierCell::onAddClick(CCObject * pSender, Control::EventType pCCControlEvent){
    int num = m_slider->getValue() * m_cntNum + 1;
    if(num > m_cntNum){
        num = m_cntNum;
    }
    m_slider->setValue(num * 1.0f / m_cntNum);
    
    int soldierId = CCString::create(m_soldierId)->intValue();
    
//    if (soldierId<=107430&&soldierId>=107401) {
//        //fusheng 改变泰坦个数
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TITAN_COUNT_CHANGE
//                                                                               , CCInteger::create(num));
//    }
}

bool SoldierCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_levelNode", CCNode*, m_levelNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sliderNode", CCNode*, m_sliderNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editNode", CCNode*, m_editNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subBtn", CCControlButton*, m_subBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addBtn", CCControlButton*, m_addBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headTouchNode", CCNode*, m_headTouchNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBg0", Sprite*, m_picBg0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBg1", Sprite*, m_picBg1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBg2", Sprite*, m_picBg2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBg3", Sprite*, m_picBg3);

    return false;
}

void SoldierCell::editBoxReturn(CCEditBox *editBox) {
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

void SoldierCell::cellTouchEnded(CCTouch* pTouch)
{
    if (isTouchInside(m_headTouchNode, pTouch)) {
        string name = CCCommonUtils::getNameById(m_soldierId);
        string des = _lang( CCCommonUtils::getPropById(m_soldierId, "description") );
        PopupViewController::getInstance()->addPopupView(UnlockItemInfoView::create(name, des, m_iconPath));
    }
}