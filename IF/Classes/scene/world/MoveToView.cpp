//
//  MoveToView.cpp
//  IF
//
//  Created by 邹 程 on 14-3-20.
//
//

#include "MoveToView.h"
#include "WorldMapView.h"
#include "ServerListPopUpView.h"
#include "UIComponent.h"

MoveToView* MoveToView::create() {
    auto ret = new MoveToView();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
    
bool MoveToView::init() {
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(502, true);
        CCLoadSprite::doResourceByCommonIndex(306, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(502, false);
            CCLoadSprite::doResourceByCommonIndex(306, false);
        });
        
        auto ccb = CCBLoadFile("MoveToView",this,this);
        this->setContentSize(ccb->getContentSize());
        
        m_input->setString(_lang("108528"));//输入坐标
        m_xLabel->setString(_lang("108529"));//X:
        m_yLabel->setString(_lang("108530"));//Y:
        
        //auto tempSprite = CCLoadSprite::createScale9Sprite("ProductionBook_5.png");
        //m_GotoBtn->addChild(tempSprite);
        
        CCCommonUtils::setButtonTitle(m_GotoBtn, _lang("119069").c_str());
        
        auto xSize = m_xNode->getContentSize();
        auto spriteText = CCLoadSprite::createScale9Sprite("01_24.png");//world_title_3.png
        m_xEdit = CCEditBox::create(xSize, spriteText);
        m_xEdit->setFontColor(ccc3(96, 93, 91));
        m_xEdit->setMaxLength(4);
        m_xEdit->setDelegate(this);
        m_xEdit->setInputMode(kEditBoxInputModeNumeric);
        m_xEdit->setReturnType(kKeyboardReturnTypeDone);
        m_xEdit->setPosition(ccp(xSize.width/2, xSize.height/2));
        if(WorldController::getInstance()->lastInputX != -1){
            m_xEdit->setText(CC_ITOA(WorldController::getInstance()->lastInputX));
        }else{
            m_xEdit->setText(UIComponent::getInstance()->m_xCoordText->getString().c_str());
        }
        m_xNode->addChild(m_xEdit);
        
        spriteText = CCLoadSprite::createScale9Sprite("01_24.png");
        auto ySize = m_yNode->getContentSize();
        m_yEdit = CCEditBox::create(ySize, spriteText);
        m_yEdit->setFontColor(ccc3(96, 93, 91));
        m_yEdit->setMaxLength(4);
        m_yEdit->setDelegate(this);
        m_yEdit->setInputMode(kEditBoxInputModeNumeric);
        m_yEdit->setReturnType(kKeyboardReturnTypeDone);
        m_yEdit->setPosition(ccp(ySize.width/2, xSize.height/2));
        if(WorldController::getInstance()->lastInputY != -1){
            m_yEdit->setText(CC_ITOA(WorldController::getInstance()->lastInputY));
        }else{
            m_yEdit->setText(UIComponent::getInstance()->m_yCoordText->getString().c_str());
        }
        m_yNode->addChild(m_yEdit);
        
        spriteText = CCLoadSprite::createScale9Sprite("01_24.png");
        auto zSize = m_zNode->getContentSize();
        m_zEdit = CCEditBox::create(zSize, spriteText);
        m_zEdit->setFontColor(ccc3(96, 93, 91));
        m_zEdit->setMaxLength(4);
        m_zEdit->setDelegate(this);
        m_zEdit->setInputMode(kEditBoxInputModeNumeric);
        m_zEdit->setReturnType(kKeyboardReturnTypeDone);
        m_zEdit->setPosition(ccp(zSize.width/2, zSize.height/2));
        std::string zStr = "";
        if(WorldController::getInstance()->lastInputZ != -1 && !GlobalData::shared()->isCrossService){
            zStr = zStr + CC_ITOA(WorldController::getInstance()->lastInputZ);
        }else{
            zStr = zStr + CC_ITOA(GlobalData::shared()->playerInfo.currentServerId);
        }
        m_zEdit->setText(zStr.c_str());
//        m_yEdit->setPosition(ccp(zSize.width/2 - 7, zSize.height/2));
        m_zNode->addChild(m_zEdit);
        if(!CCCommonUtils::isServerCrossOpen()){
            m_zEdit->setVisible(false);
            m_input->setPositionY(m_input->getPositionY() - 50);
        }else{
            m_kingdom1->setString(_lang("108755"));//王国：
            if (CCCommonUtils::isIosAndroidPad()) {
                float fw = m_kingdom1->getContentSize().width;
                float s = m_kingdom1->getScaleX();
                if (fw>220) {//防爆框
                    m_kingdom1->setScaleX(220/fw*s);
                }
            }
            else
                m_kingdom1->setMaxScaleXByWidth(110);
        }
        if(GlobalData::shared()->isCrossService && GlobalData::shared()->playerInfo.currentServerId > CROSS_SERVER_START_INDEX){
            m_kingdom1->setAnchorPoint(ccp(0.5,0.5));
            m_kingdom1->setPositionX(0);
            m_kingdom1->setString(_lang("138038"));
            m_zEdit->setVisible(false);
            m_zEdit->setEnabled(false);
        }
        this->setModelLayerTouchCallback([&](cocos2d::CCTouch *pTouch){
            // detect touch inside panel
            if (!isTouchInside(m_touchNode, pTouch)) {
                closeSelf();
            }
        });
        
        return true;

    }
    return false;
}

bool MoveToView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_input", CCLabelIF*, this->m_input)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_xLabel", CCLabelIF*, this->m_xLabel)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_yLabel", CCLabelIF*, this->m_yLabel)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_kingdom1", CCLabelIF*, this->m_kingdom1)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintText", CCLabelIF*, this->m_hintText)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_xNode", CCNode*, this->m_xNode)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_yNode", CCNode*, this->m_yNode)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_zNode", CCNode*, this->m_zNode)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_GotoBtn", CCControlButton*, this->m_GotoBtn)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode)
    return false;
}

SEL_CCControlHandler MoveToView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "keyPressedBtnGoto", MoveToView::keyPressedBtnGoto);
    return NULL;}

void MoveToView::editBoxEditingDidBegin(CCEditBox* editBox){
    editBox->setText("");
}

void MoveToView::editBoxReturn(CCEditBox *editBox) {
    int value = atoi(editBox->getText());
    bool reEdit = false;
    if (editBox == m_xEdit) {
        if (value >= _tile_count_x) {
            value = _tile_count_x - 1;
            reEdit = true;
        } else if (value < 0) {
            value = 0;
            reEdit = true;
        }
    } else if (editBox == m_yEdit){
        if (value >= _tile_count_y) {
            value = _tile_count_y - 1;
            reEdit = true;
        } else if (value < 0) {
            value = 0;
            reEdit = true;
        }
    }
    if (reEdit) {
        editBox->setText(CC_ITOA(value));
    }
}

void MoveToView::editBoxTextChanged(CCEditBox* editBox, const std::string& text){

}

void MoveToView::keyPressedBtnGoto(CCObject *pSender, CCControlEvent event) {
    auto button = dynamic_cast<CCControlButton*>(pSender);
    if (button) {
        button->setEnabled(false);
    }
    unsigned int x = atoi(m_xEdit->getText());
    unsigned int y = atoi(m_yEdit->getText());
    unsigned int z = atoi(m_zEdit->getText());
    
    int maxX = WorldController::getInstance()->getMapXByType(WorldController::getInstance()->currentMapType);
    int maxY = WorldController::getInstance()->getMapYByType(WorldController::getInstance()->currentMapType);
    if (x >= maxX || y >= maxY) {
        return;
    }
    bool zFlag = false;
    if(z != GlobalData::shared()->playerInfo.selfServerId){
        for (auto it = WorldController::getInstance()->m_serverList.begin(); it != WorldController::getInstance()->m_serverList.end(); it++) {
            if(it->serverId == z){
                zFlag = true;
            }
        }
    }else{
        zFlag = true;
    }
//    if(z == 0){
//        zFlag = false;
//    }
    if(!zFlag){
        m_hintText->setString(_lang("108757"));
        m_GotoBtn->setEnabled(true);
        return;
    }
    WorldController::getInstance()->lastInputX = x;
    WorldController::getInstance()->lastInputY = y;
    WorldController::getInstance()->lastInputZ = z;

    if (WorldMapView::instance()) {
        WorldMapView::instance()->gotoTilePoint(ccp(x,y), z, false);
    }
    
    closeSelf();
}
void MoveToView::onEnter() {
    PopupBaseView::onEnter();
//    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
//    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, true);//
}

void MoveToView::onExit() {
//    setTouchEnabled(false);
    PopupBaseView::onExit();
}