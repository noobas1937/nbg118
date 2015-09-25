//
//  KingChangePalaceView.cpp
//  IF
//
//  Created by ganxiaohua on 15-1-4.
//
//

#include "KingChangePalaceView.h"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "ToolController.h"
#include "YesNoDialog.h"
#include "WorldController.h"
#include "FunBuildController.h"
#include "KingdomExecCommand.h"
#include "ManagerKingdomView.h"
#include "DynamicResourceController.h"

const int cellW = 640;
const int cellW_HD = 1536;
KingChangePalaceView* KingChangePalaceView::create(string palace){
    KingChangePalaceView* ret = new KingChangePalaceView(palace);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool KingChangePalaceView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(100, true);
//    CCLoadSprite::doResourceByCommonIndex(301, true);
    CCLoadSprite::doResourceByCommonIndex(302, true);
//    CCLoadSprite::doResourceByCommonIndex(303, true);
//    CCLoadSprite::doResourceByCommonIndex(304, true);
//    CCLoadSprite::doResourceByCommonIndex(309, true);
    
    DynamicResourceController::getInstance()->loadNameTypeResource("Common_301", false);
    DynamicResourceController::getInstance()->loadNameTypeResource("Common_303", false);
    DynamicResourceController::getInstance()->loadNameTypeResource("Common_304", false);
    DynamicResourceController::getInstance()->loadNameTypeResource("Common_309", false);

    auto bg = CCBLoadFile("KingChangePalaceView",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else {
        setContentSize(bg->getContentSize());
        int prev = m_viewBg->getContentSize().height;
        changeBGHeight(m_viewBg);
        int add = m_viewBg->getContentSize().height - prev;
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        
        m_leftBg->setScaleY(size.height/852.0);
        m_rightBg->setScaleY(size.height/852.0);
        
        m_scrollContainer->setPositionY(m_scrollContainer->getPositionY() - add/2);
        m_bottomNode->setPositionY(m_bottomNode->getPositionY() - add);
        m_scrollContainer->setContentSize(CCSize(m_scrollContainer->getContentSize().width, m_scrollContainer->getContentSize().height + add));
    }
    generateData();
    m_tabView = CCTableView::create(this, m_scrollContainer->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionHorizontal);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Popup);
    m_tabView->setDelegate(this);
    m_scrollContainer->addChild(m_tabView);
    m_tabView->setTouchEnabled(false);
    m_count = 0;
    setCleanFunction([](){
        CCLoadSprite::doResourceByGeneralIndex(100, false);
//        CCLoadSprite::doResourceByGeneralIndex(301, false);
        CCLoadSprite::doResourceByGeneralIndex(302, false);
//        CCLoadSprite::doResourceByGeneralIndex(303, false);
//        CCLoadSprite::doResourceByGeneralIndex(304, false);
//        CCLoadSprite::doResourceByGeneralIndex(309, false);
        DynamicResourceController::getInstance()->loadNameTypeResource("Common_301", true);
        DynamicResourceController::getInstance()->loadNameTypeResource("Common_303", true);
        DynamicResourceController::getInstance()->loadNameTypeResource("Common_304", true);
        DynamicResourceController::getInstance()->loadNameTypeResource("Common_309", true);

    });
    CCCommonUtils::setButtonTitle(m_resetPicBtn, _lang("110113").c_str());
    refreshView();
    return true;
}

void KingChangePalaceView::refreshView(){
    m_tabView->reloadData();
    int index = 0;
    while(index < m_data->count()){
        std::string icon = dynamic_cast<CCString*>(m_data->objectAtIndex(index))->getCString();
        if(icon == m_palace){
            if (CCCommonUtils::isIosAndroidPad()) {
                m_tabView->setContentOffset(ccp(-index * cellW_HD, 0));
            }
            else
                m_tabView->setContentOffset(ccp(-index * cellW, 0));
            break;
        }
        index ++;
    }
    setButtonState(m_tabView->getContentOffset().x);
}

void KingChangePalaceView::onEnter(){
    PopupBaseView::onEnter();
    setTitleName(_lang("110112"));
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void KingChangePalaceView::onExit(){
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

void KingChangePalaceView::generateData(){
    m_data = CCArray::create();
    const char* icons[5] = {"301","302","303","304","309"};
    for (int i=0; i<5; i++) {
        m_data->addObject(CCString::create(icons[i]));
    }
}

SEL_CCControlHandler KingChangePalaceView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onResetPicClick", KingChangePalaceView::onResetPicClick);
    
    return NULL;
}

void KingChangePalaceView::onResetPicClick(CCObject * pSender, Control::EventType pCCControlEvent){
    if(GlobalData::shared()->isCrossService){
        YesNoDialog::show(_lang("138064").c_str(), NULL);
        return ;
    }
    std::string palace = dynamic_cast<CCString*>(m_data->objectAtIndex(m_resIndex))->getCString();
    KingdomExecCommand* cmd = new KingdomExecCommand();
    cmd->putParam("type", CCInteger::create(CHANGE_PALACE));
    cmd->putParam("palace", CCString::create(palace));
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(KingChangePalaceView::callBackSuccess), NULL));
    cmd->sendAndRelease();
    GameController::getInstance()->showWaitInterface();
}

void KingChangePalaceView::callBackSuccess(CCObject* param){
    m_resetPicBtn->setEnabled(false);
    m_palace = dynamic_cast<CCString*>(m_data->objectAtIndex(m_resIndex))->getCString();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(KINGDOM_PALACE_CHANGE,CCString::create(m_palace));
}

void KingChangePalaceView::setButtonState(int offX){
    m_resetPicBtn->setEnabled(false);
    int index = round(abs(offX) / cellW);
    if (CCCommonUtils::isIosAndroidPad()) {
        index = round(abs(offX) / cellW_HD);
    }
    bool flag = false;
    if(index < 0 || index >= m_data->count()){
        return;
    }else{
        std::string icon = dynamic_cast<CCString*>(m_data->objectAtIndex(index))->getCString();
        if(icon == m_palace){
            flag = false;
        }else{
            flag = true;
        }
    }
    if(flag){
        m_resetPicBtn->setEnabled(true);
    }
    m_resIndex = index;
}

bool KingChangePalaceView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftBg", CCSprite*, this->m_leftBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightBg", CCSprite*, this->m_rightBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scrollContainer", CCNode*, this->m_scrollContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resetPicBtn", CCControlButton*, this->m_resetPicBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picCover", CCScale9Sprite*, this->m_picCover);
    
    return false;
}

bool KingChangePalaceView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_x = 1000;
    return true;
}

void KingChangePalaceView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_scrollContainer, pTouch)){
        int dx = pTouch->getLocation().x - pTouch->getStartLocation().x;
        if(dx > 10 || dx < -10){
            if(m_x == 1000){
                m_x = m_tabView->getContentOffset().x;
            }
            int offX = m_x + dx;
            int min = -(m_data->count() - 1) * cellW - 300;
            if (CCCommonUtils::isIosAndroidPad()) {
                min = -(m_data->count() - 1) * cellW_HD - 300;
            }
            int max = 300;
            if(offX > max){
                offX = max;
            }else if(offX < min){
                offX = min;
            }
            m_tabView->setContentOffset(ccp(offX, 0), false);
        }
    }
}

void KingChangePalaceView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    int addX = -cellW / 2;
    if (CCCommonUtils::isIosAndroidPad()) {
        addX = -cellW_HD / 2;
    }
    int dx = pTouch->getStartLocation().x - pTouch->getLocation().x;
    if(dx > 20){
        addX = -cellW;
        if (CCCommonUtils::isIosAndroidPad()) {
            addX = -cellW_HD;
        }
    }else if(dx < -20){
        addX = 0;
    }
    int offSetX = int((m_tabView->getContentOffset().x + addX) / cellW) * cellW;
    if (CCCommonUtils::isIosAndroidPad()) {
        offSetX = int((m_tabView->getContentOffset().x + addX) / cellW_HD) * cellW_HD;
    }
    int minX = -(m_data->count() - 1) * cellW;
    if (CCCommonUtils::isIosAndroidPad()) {
        minX = -(m_data->count() - 1) * cellW_HD;
    }
    if(offSetX > 0){
        offSetX = 0;
    }else if(offSetX < minX){
        offSetX = minX;
    }
    m_tabView->setContentOffset(ccp(offSetX, 0), true);
    setButtonState(offSetX);
}

CCSize KingChangePalaceView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return CCSizeZero;
    }
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(cellW_HD, 1392);
    }
    return CCSize(cellW, 580);
}

CCSize KingChangePalaceView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(cellW_HD, 1392);
    }
    return CCSize(cellW, 580);
}

CCTableViewCell* KingChangePalaceView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    
    PalaceIconCell* cell = (PalaceIconCell*)table->dequeueCell();
    std::string str = dynamic_cast<CCString*>(m_data->objectAtIndex(idx))->getCString();
    if(cell){
        cell->setData(str);
    }else{
        cell = PalaceIconCell::create(str);
    }
    return cell;
}

ssize_t KingChangePalaceView::numberOfCellsInTableView(CCTableView *table)
{
    int num = ceil(m_data->count() / 1.0f);
    return num;
}

void KingChangePalaceView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
    
}

void KingChangePalaceView::scrollViewDidScroll(CCScrollView* view){
    CCLOG("%f", view->getContentOffset().x);
}

PalaceIconCell *PalaceIconCell::create(std::string palaceIndex){
    PalaceIconCell *ret = new PalaceIconCell(palaceIndex);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void PalaceIconCell::setData(std::string palaceIndex){
    this->m_palaceIndex = palaceIndex;
    refreshView();
}

void PalaceIconCell::refreshView(){
    this->removeAllChildren();
    
    if (CCCommonUtils::isIosAndroidPad()) {
        m_picNode = CCNode::create();
        CCPoint pos = ccp(0, 0);
        if(m_palaceIndex=="303"){
            pos = pos + ccp(3.0,128.6);
        }else if(m_palaceIndex=="304"){
            pos = pos + ccp(0,223);
        }else{
            pos = pos + ccp(- 3.5,+148.6);
        }
        auto sprite = CCLoadSprite::createSprite(CCString::createWithFormat("throne_tower_%s.png",m_palaceIndex.c_str())->getCString());
        m_picNode->addChild(sprite);
        sprite->setPosition(pos);
        
        pos = ccp(0, 0);
        auto sprite2 = CCLoadSprite::createSprite(CCString::createWithFormat("throne_%s.png",m_palaceIndex.c_str())->getCString());
        m_picNode->addChild(sprite2);
        sprite2->setPosition(pos);
        
        pos = ccp(cellW_HD / 2, 190 + 200);
        m_picNode->setPosition(pos);
        m_picNode->setScale(1.8);
        this->addChild(m_picNode);
    }
    else {
        CCPoint pos = ccp(cellW / 2, 190);
        if(m_palaceIndex=="303"){
            pos = pos + ccp(3.0,128.6);
        }else if(m_palaceIndex=="304"){
            pos = pos + ccp(0,223);
        }else{
            pos = pos + ccp(- 3.5,+148.6);
        }
        auto sprite = CCLoadSprite::createSprite(CCString::createWithFormat("throne_tower_%s.png",m_palaceIndex.c_str())->getCString());
        this->addChild(sprite);
        sprite->setPosition(pos);
        
        pos = ccp(cellW / 2, 190);
        auto sprite2 = CCLoadSprite::createSprite(CCString::createWithFormat("throne_%s.png",m_palaceIndex.c_str())->getCString());
        this->addChild(sprite2);
        sprite2->setPosition(pos);
    }
}

bool PalaceIconCell::init(){
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(100, false);
    });
    refreshView();
    return true;
}

void PalaceIconCell::onEnter(){
    CCTableViewCell::onEnter();
}

void PalaceIconCell::onExit(){
    CCTableViewCell::onExit();
}
