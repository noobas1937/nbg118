//
//  WildMonsterPopUp.cpp
//  IF
//
//  Created by 李锐奇 on 14-7-29.
//
//

#include "WildMonsterPopUp.h"
#include "BattleView.h"
#include "WorldMapView.h"

WildMonsterPopUp *WildMonsterPopUp::create(WorldCityInfo &info){
    WildMonsterPopUp *ret = new WildMonsterPopUp(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool WildMonsterPopUp::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        auto bg = CCBLoadFile("MonsterAttackCCB", this, this);
        this->setContentSize(bg->getContentSize());
        
        CCLoadSprite::doResourceByCommonIndex(206, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(206, false);
            CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_MONSTERLAYERBUST);
        });
        
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
        std::string showTip = _lang("103050");
//        this->m_descriptionText->setString(_lang("103749"));
        int level = atoi(CCCommonUtils::getPropById(m_info.fieldMonsterInfo.monsterId, "level").c_str());
        if(level > WorldController::getInstance()->currentMonsterLevel + 1){
            std::string tip = _lang_1("103760", CC_ITOA(WorldController::getInstance()->currentMonsterLevel + 1));
            showTip = tip + "\n" +  showTip;
//            this->m_descriptionText->setColor(ccRED);
//            this->m_descriptionText->setFontSize(24);
            this->m_btn->setVisible(false);
        }
        this->m_descriptionText->setString(showTip);
        if(m_btn->isVisible()){
            this->m_btn->setPositionY(-220 - this->m_descriptionText->getContentSize().height);
            if (CCCommonUtils::isIosAndroidPad())
            {
                m_btn->setPositionY(-480 - this->m_descriptionText->getContentSize().height);
            }
        }
        if(!WorldController::isInSelfServer(m_info.tileServerId)){
            this->m_btn->setEnabled(false);
        }
        std::string itemStr = CCCommonUtils::getPropById(m_info.fieldMonsterInfo.monsterId, "reward");
        vector.clear();
        CCCommonUtils::splitString(itemStr, ",", vector);
        int index = 0;
        while (index < vector.size() && index < maxnum) {
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

//        sign = CCLoadSprite::createSprite("icon_material.png");
//        CCCommonUtils::setSpriteMaxSize(sign, 108);
//        m_itemPicNode[0]->addChild(sign);
//        m_itemText[0]->setString(_lang("101451"));
        
        m_dropText->setString(_lang("101452"));
        std::string nameStr = CCCommonUtils::getNameById(m_info.fieldMonsterInfo.monsterId) + " LV." + CC_ITOA(level);
        m_nameAndLvText->setString(nameStr);
        
        ret = true;
    }
    return ret;
}

void WildMonsterPopUp::onExit(){
    WorldMapView::instance()->updateDirection();
    PopupBaseView::onExit();
}

void WildMonsterPopUp::onEnter(){
    WorldMapView::instance()->m_directionIcon->setVisible(false);
    WorldMapView::instance()->m_directionLabel->setVisible(false);
    PopupBaseView::onEnter();
}

//bool WildMonsterPopUp::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
//{
//    if(isTouchInside(this->m_hintBGNode, pTouch)){
//        m_startPoint =pTouch->getStartLocation();
//        return true;
//    }
//    return false;
//}
//
//void WildMonsterPopUp::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
//{
//    if(fabs(pTouch->getLocation().x-m_startPoint.x)>20||fabs(pTouch->getLocation().y-m_startPoint.y)>20){
//        return;
//    }
//    if (isTouchInside(m_hintBGNode,pTouch)) {
//        return;
//    }
//}

void WildMonsterPopUp::onAttackButton(CCObject * pSender, Control::EventType pCCControlEvent){
    if(!WorldController::getInstance()->canAttackMonster(m_info.fieldMonsterInfo.monsterId)){
        CCCommonUtils::flyHint("", "", _lang("E100047"));
        return;
    }
    unsigned int index = m_info.cityIndex;
    if(m_info.parentCityIndex != -1){
        index = m_info.parentCityIndex;
    }

    WorldController::getInstance()->openMarchDeploy(index,0);
    this->closeSelf();
}

bool WildMonsterPopUp::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
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

    return false;
}

SEL_CCControlHandler WildMonsterPopUp::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAttackButton", WildMonsterPopUp::onAttackButton);

    return NULL;
}