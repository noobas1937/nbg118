//
//  ActBossPopUp.cpp
//  IF
//
//  Created by 付彬 on 15/4/27.
//
//

#include "ActBossPopUp.h"
#include "BattleView.h"
#include "WorldMapView.h"
#include "AllianceManager.h"
#include "JoinAllianceView.h"
#include "ParticleController.h"

ActBossPopUp *ActBossPopUp::create(WorldCityInfo &info){
    ActBossPopUp *ret = new ActBossPopUp(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ActBossPopUp::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        CCLoadSprite::doResourceByCommonIndex(206, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(206, false);
            CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_MONSTERLAYERBUST);
        });
        setIsHDPanel(true);
        auto bg = CCBLoadFile("ActBossPopUp", this, this);
        this->setContentSize(bg->getContentSize());
        
        this->setModelLayerTouchCallback([&](cocos2d::CCTouch *pTouch){
            if(!isTouchInside(m_tileBg, pTouch) && (!isTouchInside(m_btn, pTouch) || !m_btn->isVisible())){
                if (!isTouchInside(m_itemTouchNode[0], pTouch) && !isTouchInside(m_itemTouchNode[1], pTouch) && !isTouchInside(m_itemTouchNode[2], pTouch) && !isTouchInside(m_itemTouchNode[3], pTouch)) {
                    this->closeSelf();
                }
            }
        });
        CCCommonUtils::setButtonTitle(m_btn, _lang("103701").c_str());
        std::string description = CCCommonUtils::getPropById(m_info.fieldMonsterInfo.monsterId, "description");
        std::vector<std::string> vector;
        CCCommonUtils::splitString(description, ",", vector);
        if(description != ""){
            int index = rand() % vector.size();
            if(index >= vector.size()){
                index = vector.size() - 1;
            }
            this->m_text->setString(_lang(vector[index]));
        }
        
        m_iconNode->removeAllChildren();
        int picAddX = 0;
        std::string icon = CCCommonUtils::getPropById(m_info.fieldMonsterInfo.monsterId, "monster") + "_bust.png";
        auto sprite  = CCLoadSprite::createSprite(icon.c_str(),true,CCLoadSpriteType_MONSTERLAYERBUST);
        m_iconNode->addChild(sprite);
        sprite->setPositionX(picAddX);
        
        
        int level = atoi(CCCommonUtils::getPropById(m_info.fieldMonsterInfo.monsterId, "level").c_str());
//        if(level > WorldController::getInstance()->currentMonsterLevel + 1){
//            this->m_descriptionText->setString(_lang_1("103760", CC_ITOA(WorldController::getInstance()->currentMonsterLevel + 1)));
//            this->m_btn->setVisible(false);
//        }
        
        this->m_descriptionText->setString(_lang("137468"));
        this->m_dropText->setString(_lang("137467"));

        if(!WorldController::isInSelfServer(m_info.tileServerId)){
            this->m_btn->setEnabled(false);
        }
        std::string itemStr = CCCommonUtils::getPropById(m_info.fieldMonsterInfo.monsterId, "reward");
        vector.clear();
        CCCommonUtils::splitString(itemStr, ",", vector);
        int index = 0;
        while (index < vector.size() && index < maxnum2) {
            std::string itemId = vector[index];
            std::string itemName = CCCommonUtils::getNameById(itemId);
            std::string iconPath = CCCommonUtils::getIcon(itemId);
            auto icon = CCLoadSprite::createSprite(iconPath.c_str(),true,CCLoadSpriteType_GOODS);
            CCCommonUtils::setSpriteMaxSize(icon, 108);
            m_itemPicNode[index]->addChild(icon);
            m_itemText[index]->setString(itemName);
            index++;
        }
        auto sign = CCLoadSprite::createSprite("MonsterAttack_4.png");
        CCCommonUtils::setSpriteMaxSize(sign, 108);
        m_itemPicNode[3]->addChild(sign);
        m_itemText[3]->setString(_lang("105849"));
        
//        m_dropText->setString(_lang("101452"));
        std::string nameStr = CCCommonUtils::getNameById(m_info.fieldMonsterInfo.monsterId) + " LV." + CC_ITOA(level);
        m_nameAndLvText->setString(nameStr);
        
        int curHp = m_info.fieldMonsterInfo.currentHp;
        int totalHp = m_info.fieldMonsterInfo.totalHp;
        double hpPro = curHp*1.0/totalHp;
        hpPro = MAX(0, hpPro);
        hpPro = MIN(hpPro, 1.0);
        m_hpLabel->setString(CC_ITOA_Percent(hpPro));
        m_bar->setScaleX(hpPro);
        
        if(!GlobalData::shared()->playerInfo.isInAlliance())
        {//没有联盟，按钮显示加入联盟
            CCCommonUtils::setButtonTitle(m_btn, _lang("137448").c_str());
        }
        else
        {//按钮显示集结
            CCCommonUtils::setButtonTitle(m_btn, _lang("137447").c_str());
        }
        
        for (int j=1; j<=4; j++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",j)->getCString());
            m_fireNode1->addChild(particle);
            
            auto particle1 = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",j)->getCString());
            m_fireNode2->addChild(particle1);
        }
        
        ret = true;
    }
    return ret;
}

void ActBossPopUp::onExit(){
    WorldMapView::instance()->updateDirection();
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_WORLD_DEFAIL_VIEW);
    PopupBaseView::onExit();
}

void ActBossPopUp::onEnter(){
    WorldMapView::instance()->m_directionIcon->setVisible(false);
    WorldMapView::instance()->m_directionLabel->setVisible(false);
    PopupBaseView::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActBossPopUp::onDetailCallback), MSG_WORLD_DEFAIL_VIEW, NULL);
    
    auto cmd = new WorldDetailCommand(m_info.parentCityIndex,m_info.tileServerId);
    cmd->setCallback(CCCallFuncO::create(this, nullptr, nullptr));
    cmd->sendAndRelease();
}

void ActBossPopUp::onDetailCallback(cocos2d::CCObject *obj)
{
    CCDictionary* dict = dynamic_cast<CCDictionary*>(obj);
    if (dict) {
        int pt = dict->valueForKey("point")->intValue();
        if (pt != m_info.cityIndex) {
            return;
        }
        int mmhp = dict->valueForKey("mmhp")->intValue();
        int mchp = dict->valueForKey("mchp")->intValue();
        m_info.fieldMonsterInfo.currentHp = mchp;
        m_info.fieldMonsterInfo.totalHp = mmhp;
        
        int curHp = m_info.fieldMonsterInfo.currentHp;
        int totalHp = m_info.fieldMonsterInfo.totalHp;
        double hpPro = curHp*1.0/totalHp;
        hpPro = MAX(0, hpPro);
        hpPro = MIN(hpPro, 1.0);
        m_hpLabel->setString(CC_ITOA_Percent(hpPro));
        m_bar->setScaleX(hpPro);
    }
}

void ActBossPopUp::onAttackButton(CCObject * pSender, Control::EventType pCCControlEvent){
    
    if(!GlobalData::shared()->playerInfo.isInAlliance())
    {//没有联盟，去加入联盟页
        PopupViewController::getInstance()->addPopupInView(JoinAllianceView::create(11));
        this->closeSelf();
        return;
    }
    
    bool canAttack = WorldController::getInstance()->currentStamine >= GlobalData::shared()->worldConfig.boss_decr;
    if(!canAttack){
        CCCommonUtils::flyHint("", "", _lang("E100047"));
        return;
    }
    unsigned int index = m_info.cityIndex;
    if(m_info.parentCityIndex != -1){
        index = m_info.parentCityIndex;
    }
    
//    WorldController::getInstance()->openMarchDeploy(index,0);
    AllianceManager::getInstance()->openAllianceMassView(index);
    this->closeSelf();
}

bool ActBossPopUp::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_text", CCLabelIF*, this->m_text);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn", CCControlButton*, this->m_btn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemText1", CCLabelIF*, this->m_itemText[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemText2", CCLabelIF*, this->m_itemText[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemText3", CCLabelIF*, this->m_itemText[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemText4", CCLabelIF*, this->m_itemText[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_descriptionText", CCLabelIF*, this->m_descriptionText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemPicNode1", CCNode*, this->m_itemPicNode[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemPicNode2", CCNode*, this->m_itemPicNode[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemPicNode3", CCNode*, this->m_itemPicNode[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemPicNode4", CCNode*, this->m_itemPicNode[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemTouchNode1", CCNode*, this->m_itemTouchNode[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemTouchNode2", CCNode*, this->m_itemTouchNode[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemTouchNode3", CCNode*, this->m_itemTouchNode[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemTouchNode4", CCNode*, this->m_itemTouchNode[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameAndLvText", CCLabelIF*, this->m_nameAndLvText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dropText", CCLabelIF*, this->m_dropText);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hpLabel", CCLabelIF*, this->m_hpLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bar", CCScale9Sprite*, this->m_bar);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode1", CCNode*, this->m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode2", CCNode*, this->m_fireNode2);
    return false;
}

SEL_CCControlHandler ActBossPopUp::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAttackButton", ActBossPopUp::onAttackButton);
    
    return NULL;
}