//
//  NewBaseTileInfo.cpp
//  IF
//
//  Created by 李锐奇 on 14-8-5.
//
//

#include "BaseTileInfo.h"
#include "AddFavorite.h"
#include "WorldMapView.h"
#include "SceneController.h"
#include "SoundController.h"
#include "AllianceCallHelpCommand.h"

static const ccColor3B TileButtonFontColor = ccWHITE;

bool NewBaseTileInfo::init(bool selfInit, bool isMarch) {
    PopupBaseView::init();
    setIsHDPanel(true);
    this->setModelLayerOpacity(0);
    isClosing = false;
    m_isMarch = isMarch;
    m_isSpeFadeIn = false;
    m_ccb = CCBLoadFile("WorldTilePopUp",this,this);
    this->setContentSize(m_ccb->getContentSize());
//    this->m_resourceNode->setVisible(false);
    std::string str = std::string("X:") + CC_ITOA((int)m_cityInfo.cityTilePoint.x) + ", Y:" + CC_ITOA((int)m_cityInfo.cityTilePoint.y);
    if(!WorldController::isInSelfServer(m_cityInfo.tileServerId)){
        str = std::string("K:") + CC_ITOA(GlobalData::shared()->playerInfo.currentServerId)+ ", " + str;
    }
    m_tilePoint->setString(str);
    int w = m_tilePoint->getContentSize().width * m_tilePoint->getOriginScaleX();
    auto bgW = m_infoBG->getContentSize().width * m_infoBG->getScale();
    float dx = 0.0f;
    if(w > bgW - 100){
        dx = ((w + 100) - bgW) / m_infoBG->getScale();
    }
    m_infoBG->setContentSize(m_infoBG->getContentSize() + Size(dx, 0));
    m_faveIcon->setPositionX(m_faveIcon->getPositionX() - dx / 2 * m_infoBG->getScale());
    if(m_cityInfo.cityType != ResourceTile && m_cityInfo.cityType != Tile_allianceArea && m_cityInfo.cityType != tile_superMine && m_cityInfo.cityType != tile_tower && m_cityInfo.cityType != tile_wareHouse && m_cityInfo.cityType != tile_banner){
        this->m_title->setVisible(false);
        m_tilePoint->setFontSize(20);
        m_tilePoint->setPositionY(m_tilePoint->getPositionY() + 10);
    }
    int orgX = m_addNode->getPositionX();
    int finalX = m_tilePoint->getPositionX() + m_tilePoint->getContentSize().width * m_tilePoint->getOriginScaleX() / 2 + 25;
    if(finalX > orgX){
        m_addNode->setPositionX(finalX);
    }
    if(WorldController::getInstance()->isInFavorite(m_cityInfo, GlobalData::shared()->playerInfo.currentServerId)){
        m_faveIcon->setDisplayFrame(CCLoadSprite::loadResource("world_favo_2.png"));
        m_faveIcon->setScale(1.0f);
        m_faveIcon->setPositionY(m_faveIcon->getPositionY() - 3);
        m_faveIcon->setPositionX(m_faveIcon->getPositionX() + 16);
        m_addNode->setVisible(false);
        m_tilePoint->setPositionX(m_tilePoint->getPositionX() + 15);
    }
    m_addBtn->setSwallowsTouches(false);
    isInitEnd = false;
    m_openNum = 0;
    
//    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(NewBaseTileInfo::animationInFunc));
//    CCLOG("onenter end ******************");
//    string animaName = "FadeIn";
//    if (true) {
//        if (m_btnCnt == 4 || m_btnCnt == 5) {
//            animaName = "FadeIn5";
//        }else if (m_btnCnt == 1) {
//            animaName = "FadeIn2";
//        }else if (m_btnCnt == 3) {
//            animaName = "FadeIn4";
//        }
//    }
//    this->getAnimationManager()->runAnimationsForSequenceNamed(animaName.c_str());
    
    return true;
}

bool NewBaseTileInfo::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode1", CCNode*, this->m_btnNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode2", CCNode*, this->m_btnNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode3", CCNode*, this->m_btnNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode4", CCNode*, this->m_btnNode4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode5", CCNode*, this->m_btnNode5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode6", CCNode*, this->m_btnNode6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameNode", CCNode*, this->m_nameNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node1", CCNode*, this->m_node1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node2", CCNode*, this->m_node2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node3", CCNode*, this->m_node3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node4", CCNode*, this->m_node4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node5", CCNode*, this->m_node5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node6", CCNode*, this->m_node6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText1", CCLabelIF*, this->m_nameText1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText2", CCLabelIF*, this->m_nameText2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText3", CCLabelIF*, this->m_nameText3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText4", CCLabelIF*, this->m_nameText4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText5", CCLabelIF*, this->m_nameText5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText6", CCLabelIF*, this->m_nameText6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgCircleNode", CCNode*, this->m_bgCircleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_title", CCLabelIF*, this->m_title);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tilePoint", CCLabelIF*, this->m_tilePoint);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoBG", CCScale9Sprite*, this->m_infoBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameAndAlliance", CCLabelIF*, this->m_nameAndAlliance);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_faveIcon", CCSprite*, this->m_faveIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addNode", CCNode*, this->m_addNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addBtn", CCControlButton*, this->m_addBtn);

    return false;
}

SEL_CCControlHandler NewBaseTileInfo::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "keyPressedBtnFavorite", BaseTileInfo::keyPressedBtnFavorite)
    return NULL;
}

void NewBaseTileInfo::keyPressedBtnFavorite(CCObject * pSender, Control::EventType pCCControlEvent) {
    auto world = WorldMapView::instance();
    if (world) {
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        world->addPopupView(this->getFavoriteView());
    }
    closeThis();
}

void NewBaseTileInfo::addNameNode(){
    if(m_cityInfo.playerName == ""){
        return;
    }
    m_nameNode->removeAllChildren();
    auto bg = CCLoadSprite::createSprite("name_bg.png");
    m_nameNode->addChild(bg);
    
    string owner = "";
    if (m_cityInfo.leagueAsn == "")
    {
        owner = m_cityInfo.playerName.c_str();
    }
    else
    {
        owner += "(" + m_cityInfo.leagueAsn + ")" + m_cityInfo.playerName;
    }
    auto label = CCLabelIF::create(owner.c_str());
    if(GlobalData::shared()->isCrossService && m_cityInfo.srcServer>0 && GlobalData::shared()->playerInfo.crossFightSrcServerId!=m_cityInfo.srcServer){
        label->setColor({255,0,0});
    }
    m_nameNode->addChild(label);
}

BaseTileInfo* NewBaseTileInfo::getFavoriteView() {
    return AddFavorite::create(m_cityInfo.cityIndex);
}

void NewBaseTileInfo::setFavoButtonVisible(bool isVisible) {
//    if (m_btnFavorite->isVisible() == isVisible) {
//        return;
//    }
//    
//    m_btnFavorite->setVisible(isVisible);
}

void NewBaseTileInfo::setButtonCount(unsigned int count,bool needLargeButton) {
//    if(count == 1 || count == 2 || count == 3){
//        m_infoNode->setPositionY(m_infoNode->getPositionY() + 50);
//    }

    m_btnCnt = count;
    CCLOG("set btn count  end ***********set btn count  end*******");
    count = TOTAL_BUTTON_NUM;
    m_buttonStack.clear();
    m_buttonState.clear();
    int index = 1;
    while (index <= TOTAL_BUTTON_NUM) {
        auto node = getBtnNode(index);
        if(node){
            node->removeChildByTag(ICON_TAG);
            node->removeChildByTag(BUTTON_TAG);
            node->removeChildByTag(ICON_DEL_TAG);
            index++;
        }
    }
    for (int i = 1; i <= count; ++i) {
        auto button = CCControlButton::create();
        button->setTag(BUTTON_TAG);
        button->setEffectStr("");
        button->setTouchPriority(Touch_Popup);
        auto node = getNode(i);
        node->setVisible(false);
        
        auto btnNode = getBtnNode(i);
        btnNode->addChild(button);
        m_buttonStack.push_back(button);
        m_buttonState.push_back(ButtonNone);
    }
    onSetMarkBtn(m_btnCnt);
}

CCNode* NewBaseTileInfo::getNode(unsigned int index){
    if(index == 1){
        return m_node1;
    }else if(index == 2){
        return m_node2;
    }else if(index == 3){
        return m_node3;
    }else if(index == 4){
        return m_node4;
    }
    else if(index == 5){
        return m_node5;
    }
    return m_node6;
}

CCNode* NewBaseTileInfo::getBtnNode(unsigned int index){
    if(index == 1){
        return m_btnNode1;
    }else if(index == 2){
        return m_btnNode2;
    }else if(index == 3){
        return m_btnNode3;
    }else if(index == 4){
        return m_btnNode4;
    }else if(index == 5){
        return m_btnNode5;
    }
    return m_btnNode6;
}

CCLabelIF *NewBaseTileInfo::getNameText(unsigned int index){
    if(index == 1){
        return m_nameText1;
    }else if(index == 2){
        return m_nameText2;
    }else if(index == 3){
        return m_nameText3;
    }else if(index == 4){
        return m_nameText4;
    }
    else if(index == 5){
        return m_nameText5;
    }
    return m_nameText6;
}

unsigned int NewBaseTileInfo::getButtonCount() {
    return m_buttonStack.size();
}

void NewBaseTileInfo::setButtonName(unsigned int order,const string& name) {
    if (order == 0 || order > m_buttonStack.size()) {
        return;
    }
    //    CCCommonUtils::setButtonTitle(m_buttonStack[order - 1],name.c_str());
    getNameText(order)->setString(name.c_str());
}

void NewBaseTileInfo::setButtonCallback(unsigned int order,SEL_CCControlHandler handler) {
    if (order == 0 || order > m_buttonStack.size()) {
        return;
    }
    m_buttonStack[order - 1]->addTargetWithActionForControlEvents(this, handler, CCControlEventTouchUpInside);
}

CCControlButton* NewBaseTileInfo::getButton(unsigned int order) {
    if (order == 0 || order > m_buttonStack.size()) {
        return NULL;
    }
    return m_buttonStack[order - 1];
}

TileButtonState NewBaseTileInfo::getButtonState(unsigned int order) {
    if (order == 0 || order > m_buttonState.size()) {
        return ButtonNone;
    }
    return m_buttonState[order - 1];
}

TileButtonState NewBaseTileInfo::getButtonState(CCControlButton* button) {
    if (button) {
        auto it = std::find(m_buttonStack.begin(), m_buttonStack.end(), button);
        if (it != m_buttonStack.end()) {
            return getButtonState(it - m_buttonStack.begin() + 1);
        }
    }
    return ButtonNone;
}

void NewBaseTileInfo::setButtonState(unsigned int order, TileButtonState state) {
    if (order == 0 || order > m_buttonState.size()) {
        return;
    }
    m_buttonState[order - 1] = state;
    auto btn = getButton(order);
    auto node = getNode(order);
    node->setVisible(true);
    CCCommonUtils::setButtonSprite(btn, "bnt_02.png");

    CCScale9Sprite *sprite = CCLoadSprite::createScale9Sprite("bnt_02.png");
    btn->setPreferredSize(CCSize(122, 124));
    sprite->setColor(ccGRAY);
    btn->setBackgroundSpriteForState(sprite,CCControlStateDisabled);
    
    CCSprite *icon = CCLoadSprite::createSprite(getButtonPic(state).c_str());
    getBtnNode(order)->addChild(icon);
    icon->setTag(ICON_TAG);
    
    if (state == ButtonMarkLine) {
        getBtnNode(order)->removeChildByTag(ICON_DEL_TAG);
        int rallyPoint = GlobalData::shared()->playerInfo.allianceInfo.rallyPoint;
        if (rallyPoint == m_cityInfo.parentCityIndex || rallyPoint == m_cityInfo.cityIndex) {//撤销标记
            CCSprite *dicon = CCLoadSprite::createSprite("red_no.png");
            getBtnNode(order)->addChild(dicon);
            dicon->setTag(ICON_DEL_TAG);
        }
    }
}

void NewBaseTileInfo::setButtonState(CCControlButton *button, TileButtonState state) {
    if (button) {
        auto it = std::find(m_buttonStack.begin(), m_buttonStack.end(), button);
        if (it != m_buttonStack.end()) {
            setButtonState(it - m_buttonStack.begin() + 1,state);
        }
    }
}

std::string NewBaseTileInfo::getButtonPic(TileButtonState state){
    if(state==AppointOfficals){
        return "icon_appoint.png";
    }else if(state==ButtonOrderIcon){
        return "al_order_icon.png";
    }
    return std::string("tile_pop_icon") + CC_ITOA(int(state)) + ".png";
}

void NewBaseTileInfo::closeThis() {
    if(isClosing){
        return;
    }
    isClosing = true;
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(NewBaseTileInfo::animationFunc));
    string animaName = "FadeOut";
    if (m_isSpeFadeIn) {
        if (m_btnCnt == 4 || m_btnCnt == 5) {
            animaName = "FadeOut5";
        }else if (m_btnCnt == 1) {
            animaName = "FadeOut2";
        }else if (m_btnCnt == 3) {
            animaName = "FadeOut4";
        }
    }
    this->getAnimationManager()->runAnimationsForSequenceNamed(animaName.c_str());
}

void NewBaseTileInfo::closeImmediately(){
    animationFunc();
}

void NewBaseTileInfo::animationFunc(){
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
        WorldMapView::instance()->removeTouchEffect();
    }
    this->removeFromParent();
}

void NewBaseTileInfo::resetPos(CCPoint &pt){
    m_ccb->setPosition(pt);
}

void NewBaseTileInfo::animationInFunc(){
//    int y = m_infoNode->getPositionY();
//    if (this->getAnimationManager()->getLastCompletedSequenceName() == "FadeIn" ) {
//        AnimationCallback();
//    }
    
//    this->getAnimationManager()->runAnimationsForSequenceNamed("Loop");
//    m_infoNode->setPositionY(y);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           , CCString::createWithFormat("WL_wood"));
    isInitEnd = true;

}

bool NewBaseTileInfo::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_faveIcon, pTouch) || isTouchInside(m_addBtn, pTouch)){
        return true;
    }
    return false;
}

void NewBaseTileInfo::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_faveIcon, pTouch) || isTouchInside(m_addBtn, pTouch)){
        if(!WorldController::getInstance()->isInFavorite(m_cityInfo, GlobalData::shared()->playerInfo.currentServerId)){
            keyPressedBtnFavorite(NULL,Control::EventType::TOUCH_DOWN);
        }else{
            CCCommonUtils::flyHint("", "", _lang("108873"));
        }
    }
}

void NewBaseTileInfo::onEnter(){
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
}

void NewBaseTileInfo::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

void NewBaseTileInfo::addToParent(){
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
        WorldMapView::instance()->addPopupView3(this);
    }
}

void NewBaseTileInfo::setButtonEnable(int index, bool enable){
    auto btn = getButton(index);
    auto node = btn->getParent();
//    btn->setEnabled(enable);

    CCSprite *icon = dynamic_cast<CCSprite*>(node->getChildByTag(ICON_TAG));
    if(icon){
        CCCommonUtils::setSpriteGray(icon, !enable);
    }
}

void NewBaseTileInfo::AnimationCallback()
{
    if (m_cityInfo.playerName == GlobalData::shared()->playerInfo.name) {
        return;
    }
    
    float dt = 0.3;
    if (m_btnCnt==4 || m_btnCnt==5) {
        CCActionInterval * moveTo5 = CCMoveTo::create(dt,ccp(-103, 113));// -75 145
        m_node5->runAction(moveTo5);
        CCActionInterval * moveTo4 = CCMoveTo::create(dt,ccp(103, 113));// 75 145
        m_node4->runAction(moveTo4);
        
//        CCActionInterval * moveToA5 = CCMoveTo::create(dt,ccp(31.5, -32.3));// 19, -39
//        m_arrow5->runAction(moveToA5);
//        CCActionInterval * moveToA4 = CCMoveTo::create(dt,ccp(-31.5, -32.5));// -21 -39
//        m_arrow4->runAction(moveToA4);
//        
//        CCActionInterval * rotateTo5 = CCRotateTo::create(dt, 45);//62.5
//        m_arrow5->runAction(rotateTo5);
//        CCActionInterval * rotateTo4 = CCRotateTo::create(dt,135);//115
//        m_arrow4->runAction(rotateTo4);
    }
    else if (m_btnCnt==1) {
        CCActionInterval * moveTo1 = CCMoveTo::create(dt,m_node2->getPosition());
        m_node1->runAction(moveTo1);
        
//        CCActionInterval * moveToA1 = CCMoveTo::create(dt,m_arrow2->getPosition());
//        m_arrow1->runAction(moveToA1);
//        CCActionInterval * roteToA1 = CCRotateTo::create(dt,m_arrow2->getRotation());
//        m_arrow1->runAction(roteToA1);
    }
    else if (m_btnCnt==3) {
        CCActionInterval * moveTo1 = CCMoveTo::create(dt,m_node5->getPosition());
        m_node1->runAction(moveTo1);
        
//        CCActionInterval * moveToA1 = CCMoveTo::create(dt,m_arrow5->getPosition());
//        m_arrow1->runAction(moveToA1);
//        CCActionInterval * roteToA1 = CCRotateTo::create(dt,m_arrow5->getRotation());
//        m_arrow1->runAction(roteToA1);
    }
}

void NewBaseTileInfo::onSetMarkBtn(int count)
{
    string animaName = "FadeIn";
    
    bool st = true;
    if (m_cityInfo.playerName == GlobalData::shared()->playerInfo.name) {
        st = false;
    }
    
    if (GlobalData::shared()->playerInfo.isInAlliance())
    {
        if (GlobalData::shared()->playerInfo.allianceInfo.leaderUid == GlobalData::shared()->playerInfo.uid) {
        }else {
            st = false;
        }
    }
    else {
        st = false;
    }
    
    if (! WorldController::getInstance()->isInSelfServer(m_cityInfo.tileServerId)) {
        st = false;
    }
    
    if (m_isMarch) {//是行军
        st = false;
    }
    
    int idx = 1;
    if (count==2 || count==0 || count==4 || count==5) {
        idx = 1;
        if (count==4 || count==5) {
//            this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(NewBaseTileInfo::AnimationCallback));
            // 4 5 位置微调 播放完FadeIn后调整
        }
    }else if (count == 1) {
        idx = 3;
    }else if (count == 3) {
        idx = 4;
    }else {
        st = false;
    }
    
    if (st) {
        int rallyPoint = GlobalData::shared()->playerInfo.allianceInfo.rallyPoint;
        if (rallyPoint == m_cityInfo.parentCityIndex || rallyPoint == m_cityInfo.cityIndex) {//撤销标记
            setButtonName(idx, _lang("115415"));
        }
        else {
            setButtonName(idx, _lang("115414"));
        }
        setButtonState(idx, ButtonMarkLine);
        setButtonCallback(idx, cccontrol_selector(NewBaseTileInfo::onMarkLineClick));
    }
    
    if (m_openNum>0) {
        return;
    }
    
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(NewBaseTileInfo::animationInFunc));
    CCLOG("onenter end ******************");
    
    if (st) {
        if (m_btnCnt == 4 || m_btnCnt == 5) {
            animaName = "FadeIn5";
            m_isSpeFadeIn = true;
        }else if (m_btnCnt == 1) {
            animaName = "FadeIn2";
            m_isSpeFadeIn = true;
        }else if (m_btnCnt == 3) {
            animaName = "FadeIn4";
            m_isSpeFadeIn = true;
        }
    }
    this->getAnimationManager()->runAnimationsForSequenceNamed(animaName.c_str());
    m_openNum++;
}

void NewBaseTileInfo::onMarkLineClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    int cityIndex = m_cityInfo.cityIndex;
    if (m_cityInfo.parentCityIndex > 0) {
        cityIndex = m_cityInfo.parentCityIndex;
    }
    int rallyPoint = GlobalData::shared()->playerInfo.allianceInfo.rallyPoint;
    if (rallyPoint == m_cityInfo.parentCityIndex || rallyPoint == m_cityInfo.cityIndex) {//撤销标记
        cityIndex = 0;
    }
    AllianceMarkLineCommand* cmd = new AllianceMarkLineCommand(cityIndex);
    cmd->sendAndRelease();
    closeThis();
}