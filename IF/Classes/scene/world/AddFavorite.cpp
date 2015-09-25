//
//  AddFavorite.cpp
//  IF
//
//  Created by 邹 程 on 14-2-25.
//
//

#include "AddFavorite.h"
#include "WorldController.h"
#include "WorldMapView.h"

bool AddFavorite::init() {
    
    CCLoadSprite::doResourceByCommonIndex(502, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(502, false);
    });
    BaseTileInfo::init();
    setIsHDPanel(true);
    auto ccb = CCBLoadFile("addFavorite",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else
        this->setContentSize(ccb->getContentSize());
    
    m_titleNew->setString(_lang("108501"));
    CCCommonUtils::setButtonTitle(m_btnOk, _lang("confirm").c_str());
    m_subNode->setVisible(false);
    //setFavoButtonVisible(false);
//    m_title->setString(_lang("108501"));
    
    auto point = WorldController::getPointByIndex(m_index);
    m_location->setString(_lang_2("108502", CC_ITOA((long)point.x), CC_ITOA((long)point.y)));
    m_tapLabel->setString(_lang("108503"));
    m_choLabel->setString(_lang("108504"));
    
    m_iconLabel[0]->setString(_lang("108505"));
    m_iconLabel[1]->setString(_lang("108506"));
    m_iconLabel[2]->setString(_lang("108520"));

//    setButtonCount(1);
//    setButtonName(1,_lang("confirm"));
//    setButtonCallback(1,cccontrol_selector(AddFavorite::keyPressedBtnOk));
    
    auto editSize = m_editTitleNode->getContentSize();
    m_editBox = InputFieldMultiLine::create(editSize, "01_24.png", 28);//UI_Alliance_TextBox.png
    m_editBox->setAddH(0);
    m_editBox->setAnchorPoint(ccp(0,0));
    m_editBox->setMaxChars(15);
    m_editBox->setLineNumber(1);
    m_editBox->setFontColor(ccc3(0, 0, 0));
    m_editBox->setPosition(ccp(0,0));
    m_editBox->setSwallowsTouches(true);
    m_editBox->setMoveFlag(true);
    m_editBox->setTouchPriority(0);
    m_editBox->setPlaceHolder(m_name.c_str());
    m_editBox->setDefaultText(m_name);
    m_editBox->setBgVisible(false);
    m_editTitleNode->addChild(m_editBox);
    
    for (int i=0; i<3; i++) {
        auto buttonNode = CCCommonUtils::ButtonNode::create(CCSizeMake(90, 90));
        buttonNode->setTouchDoBegin([&,i](bool isTouchInside){
            this->clickIcon((FavoriteType)i);
        });
        buttonNode->setSwallow(true);
        buttonNode->setPosition(ccp(45, 45));
        m_icon[i]->addChild(buttonNode);
    }
    
    clickIcon(m_type);
    
    return true;
}

bool AddFavorite::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_location", CCLabelIF*, this->m_location)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tapLabel", CCLabelIF*, this->m_tapLabel)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_choLabel", CCLabelIF*, this->m_choLabel)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconLabel1", CCLabelIF*, this->m_iconLabel[0])
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconLabel2", CCLabelIF*, this->m_iconLabel[1])
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconLabel3", CCLabelIF*, this->m_iconLabel[2])
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editTitleNode", CCNode*, this->m_editTitleNode)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon1", CCNode*, this->m_icon[0])
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon2", CCNode*, this->m_icon[1])
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon3", CCNode*, this->m_icon[2])
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectSprite", CCSprite*, this->m_selectSprite)
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleNew", CCLabelIF*, this->m_titleNew)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnOk", CCControlButton*, this->m_btnOk)
    
    return BaseTileInfo::onAssignCCBMemberVariable(pTarget, pMemberVariableName, pNode);
}

SEL_CCControlHandler AddFavorite::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "keyPressedBtnOk", AddFavorite::keyPressedBtnOk);
    
    return BaseTileInfo::onResolveCCBCCControlSelector(pTarget, pSelectorName);
}

void AddFavorite::clickIcon(FavoriteType type) {
    m_type = type;
    switch (m_type) {
        case Favorite: {
            m_selectSprite->setPositionX(-155.0);
        }
            break;
        case Friend: {
            m_selectSprite->setPositionX(0);
        }
            break;
        case Enemy: {
            m_selectSprite->setPositionX(155.0);
        }
            break;
        default:
            break;
    }
}

void AddFavorite::keyPressedBtnOk(CCObject * pSender, Control::EventType pCCControlEvent) {
    auto button = dynamic_cast<CCControlButton*>(pSender);
    if (button) {
        button->setEnabled(false);
    }
    std::string name = m_editBox->getText();
    if (!name.empty()) {
        m_name = name;
    }
    WorldMapView::instance()->addFavorite(m_name, m_index, m_type);
    closeThis();
}
