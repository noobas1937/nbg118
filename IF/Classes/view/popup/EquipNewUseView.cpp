//
//  EquipNewUseView.cpp
//  IF
//
//  Created by fubin on 15/3/5.
//
//

#include "EquipNewUseView.h"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "ToolController.h"
#include "EquipmentController.h"
#include "EquipmentInfo.h"
#include "CCClipNode.h"
#include "EquipmentListView.h"
#include "QueueController.h"
#include "YesNoDialog.h"
#include "FunBuildController.h"
#include "FunBuild.h"
#include "SoundController.h"
#include "LongJingStoreView.h"

#define rp_color {0,255,255}

static string curHaveOnUuid = "";
static bool CanClick = true;
static int WeapType = 0;

EquipNewUseView* EquipNewUseView::create(int site){
    EquipNewUseView* ret = new EquipNewUseView();
    if(ret && ret->init(site)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipNewUseView::init(int site)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    CCLoadSprite::doResourceByCommonIndex(305, true);
    CCLoadSprite::doResourceByCommonIndex(11, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(305, false);
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_EQUIP);
    });
    
    setIsHDPanel(true);
    m_weapType = site;
    WeapType = m_weapType;
    auto bg = CCBLoadFile("EquipNewUseView",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else {
        setContentSize(bg->getContentSize());
    }
    
    if (true) {
        auto bgSpr = CCLoadSprite::createSprite("bg0.png");//common11
        auto batchNode = CCSpriteBatchNode::createWithTexture(bgSpr->getTexture());
        m_bgNode->addChild(batchNode);
        
        int bgSprH = bgSpr->getContentSize().height;
        int sumHeight = (CCDirector::sharedDirector()->getWinSize().height / bgSprH)+1;
        for (int i=0; i<sumHeight; i++) {
            auto bgSpr1 = CCLoadSprite::createSprite("bg0.png");
            auto bgSpr2 = CCLoadSprite::createSprite("bg0.png");
            bgSpr2->setFlipX(true);
            if(i%2 == 1) {
                bgSpr1->setFlipY(true);
                bgSpr2->setFlipY(true);
            }
            if (CCCommonUtils::isIosAndroidPad()) {
                bgSpr1->setScaleX(2.4);
                bgSpr2->setScaleX(2.4);
                bgSpr1->setPosition(ccp(384, -i*bgSprH-60));
                bgSpr2->setPosition(ccp(1152, -i*bgSprH-60));
            }
            else {
                bgSpr1->setPosition(ccp(160, -i*bgSprH-60));
                bgSpr2->setPosition(ccp(480, -i*bgSprH-60));
            }
            batchNode->addChild(bgSpr1);
            batchNode->addChild(bgSpr2);
        }
    }
    
    if (!CCCommonUtils::isIosAndroidPad()) {
        int extH = getExtendHeight();
        m_infoList->setPositionY(m_infoList->getPositionY()-extH);
        m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height+extH));
        m_shadeNode->setPositionY(m_shadeNode->getPositionY()-extH/2);
        m_shadeNode->setContentSize(CCSize(m_shadeNode->getContentSize().width, m_shadeNode->getContentSize().height+extH));
        m_aniNode->setPositionY(m_aniNode->getPositionY()-extH);
        m_middle2Node->setPositionY(m_middle2Node->getPositionY()-extH);
    }
    
    m_tableViewSize = m_infoList->getContentSize();
    
    m_clipNode = CCClipNode::create(m_infoList->getContentSize());
    m_clipNode->setAnchorPoint(ccp(0,0));
    m_infoList->addChild(m_clipNode);
    
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Popup);
    m_tabView->setPosition(ccp(0, 200));
    m_clipNode->addChild(m_tabView);
    
    CCCommonUtils::setButtonTitle(m_takeOffBtn, _lang("119003").c_str());
    
    return true;
}

void EquipNewUseView::refresh(){
    if (m_weapType == 0) {
        setTitleName(_lang("119014"));
        m_noEquipLabel->setString(_lang_1("119042", _lang("119014").c_str()));
    }else if (m_weapType == 1) {
        setTitleName(_lang("119015"));
        m_noEquipLabel->setString(_lang_1("119042", _lang("119015").c_str()));
    }else if (m_weapType == 2) {
        setTitleName(_lang("119016"));
        m_noEquipLabel->setString(_lang_1("119042", _lang("119016").c_str()));
    }else if (m_weapType == 3) {
        setTitleName(_lang("119017"));
        m_noEquipLabel->setString(_lang_1("119042", _lang("119017").c_str()));
    }else if (m_weapType == 4) {
        setTitleName(_lang("119018"));
        m_noEquipLabel->setString(_lang_1("119042", _lang("119018").c_str()));
    }else if (m_weapType == 5) {
        setTitleName(_lang("119019"));
        m_noEquipLabel->setString(_lang_1("119042", _lang("119019").c_str()));
    }else if (m_weapType == 6) {
        setTitleName(_lang("111664"));
        m_noEquipLabel->setString(_lang_1("119042", _lang("111664").c_str()));
    }
}

void EquipNewUseView::onEnter(){
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipNewUseView::onCloseView), EQUIP_CLOSE_VIEW, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipNewUseView::onOpenUpdate), MSG_CLICK_VIEW, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipNewUseView::onShowWateInterface), MSG_POST_NET_PUT, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipNewUseView::onPlayPutOnEquip), MSG_PLAY_EQUIP_PUT, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipNewUseView::onGetShowPowerMsg), EQUIP_POWER_EFFECT_SHOW, NULL);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    
    refresh();
    generateData();
    m_tabView->reloadData();
    onPlayDropTable();
}

void EquipNewUseView::onExit(){
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, EQUIP_CLOSE_VIEW);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CLICK_VIEW);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_POST_NET_PUT);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PLAY_EQUIP_PUT);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, EQUIP_POWER_EFFECT_SHOW);
    setTouchEnabled(false);
    CCNode::onExit();
}

void EquipNewUseView::onGetShowPowerMsg(CCObject* obj)
{
    CCCallFuncO* fun = CCCallFuncO::create(this, callfuncO_selector(EquipNewUseView::showPowerEffect), obj);
    this->runAction(CCSequence::create(CCDelayTime::create(0.6f), fun, NULL));
}

void EquipNewUseView::showPowerEffect(CCObject* obj)
{
    CCDouble* d = nullptr;
    d = dynamic_cast<CCDouble*>(obj);
    if (!d) {
        return;
    }
    double exc = d->getValue();
    UIComponent::getInstance()->showFlyCBViewPower(exc);
}

void EquipNewUseView::onPlayDropTable()
{
    CanClick = false;
    CCActionInterval * moveTo0 = CCMoveTo::create(0,ccp(0, 500));
    CCActionInterval * moveTo1 = CCMoveTo::create(0.3,ccp(0, 0));
    CCCallFunc * funcall= CCCallFunc::create(this, callfunc_selector(EquipNewUseView::onDropEnd));
    m_tabView->runAction(CCSequence::create(moveTo0, moveTo1, funcall, NULL));
}

void EquipNewUseView::onDropEnd()
{
    CanClick = true;
    scheduleOnce(schedule_selector(EquipNewUseView::onDelayTfunc), 0.3f);
}

void EquipNewUseView::onDelayTfunc(float dt)
{
    if (m_openMap.size()>=2 && m_data.size()>=2) {//curHaveOnUuid=="" &&
        onOpenUpdate(CCString::create(m_data[0]));
    }
}

void EquipNewUseView::onCloseView(CCObject * obj)
{
    scheduleOnce(schedule_selector(EquipNewUseView::onDelayWearfunc), 1.5f);
    
//    float miny = m_tabView->minContainerOffset().y;
//    CCPoint pos  = m_tabView->getContentOffset();
//    generateData();
//    m_tabView->reloadData();
//    
//    onPlayDropTable();
////    if (curHaveOnUuid == "") {
////        onPlayDropTable();
////    }else {
////        float mincurry = m_tabView->minContainerOffset().y;
////        pos.y+=(-miny+mincurry);
////        m_tabView->setContentOffset(pos);
////    }
}

void EquipNewUseView::onDelayWearfunc(float dt)
{
    float miny = m_tabView->minContainerOffset().y;
    CCPoint pos  = m_tabView->getContentOffset();
    generateData();
    m_tabView->reloadData();
    
    onPlayDropTable();
}

void EquipNewUseView::onOpenUpdate(CCObject * obj)
{
    CCString* itemId = dynamic_cast<CCString*>(obj);
    if (!itemId) {
        return;
    }
    string key = itemId->getCString();
    if (m_openMap.find(key) == m_openMap.end()) {
        return;
    }
    m_openMap[key] = (m_openMap[key]+1)%2;
    
    float miny = m_tabView->minContainerOffset().y;
    CCPoint pos  = m_tabView->getContentOffset();
    m_tabView->reloadData();
    float mincurry = m_tabView->minContainerOffset().y;
    pos.y+=(-miny+mincurry);
    m_tabView->setContentOffset(pos);
}

void EquipNewUseView::generateData()
{
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    CCLoadSprite::doResourceByCommonIndex(305, true);
    m_playNode->removeAllChildren();
    
    m_iconNode->removeAllChildren();
    curHaveOnUuid = "";
    string curUuid = EquipmentController::getInstance()->FindOnEquipBySite(m_weapType);
    curHaveOnUuid = curUuid;
    m_data.clear();
    m_data = EquipmentController::getInstance()->FindEquipmentsByType(m_weapType);
    
    m_openMap.clear();
    for (int i=0; i<m_data.size(); i++) {
        m_openMap[m_data[i]] = 0;
    }
    m_data.push_back("");//显示按钮
    
    m_prar1Label->setString("");
    m_prar2Label->setString("");
    m_prar3Label->setString("");
    m_prar4Label->setString("");
    m_prar5Label->setString("");
    m_num1Label->setString("");
    m_num2Label->setString("");
    m_num3Label->setString("");
    m_num4Label->setString("");
    m_num5Label->setString("");
    
    if (curHaveOnUuid != "") {
        m_msgNode->setVisible(false);
        m_equipNode->setVisible(true);
        
        int curItemId = EquipmentController::getInstance()->m_MyOnEquip[curHaveOnUuid];
        auto& curInfo = EquipmentController::getInstance()->EquipmentInfoMap[curItemId];
        if(curInfo.site==6){
            curInfo = EquipmentController::getInstance()->LongEquipmentInfoMap[curHaveOnUuid];
        }
        string bgTT = CCCommonUtils::getToolBgByColor(curInfo.color);
        auto iconBgTT = CCLoadSprite::createSprite(bgTT.c_str());
        if (CCCommonUtils::isIosAndroidPad()) {
            CCCommonUtils::setSpriteMaxSize(iconBgTT, 180, true);
        }
        else
            CCCommonUtils::setSpriteMaxSize(iconBgTT, 90, true);
        m_iconNode->addChild(iconBgTT);
        
        string strEquipTmp = CCCommonUtils::getIcon(CC_ITOA(curInfo.itemId));
        auto iconEquipTmp = CCLoadSprite::createSprite(strEquipTmp.c_str(), true,CCLoadSpriteType_EQUIP);
        if (CCCommonUtils::isIosAndroidPad()) {
            CCCommonUtils::setSpriteMaxSize(iconEquipTmp, 180, true);
        }
        else
            CCCommonUtils::setSpriteMaxSize(iconEquipTmp, 90, true);
        m_iconNode->addChild(iconEquipTmp);
        
        //特效
        if(curInfo.site==6){
            m_particle->removeAllChildren();
            int part=1;
            if(1==curInfo.color){//绿
                part = 3;
            }else if (2==curInfo.color){//蓝
                part = 1;
            }else if (3==curInfo.color){//紫
                part = 4;
            }else if (4==curInfo.color){//橙
                part = 5;
            }else if (5==curInfo.color){//金
                part = 2;
            }
            for (int par=1; par<=5; par++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("Cristal_%d_%d",part,par)->getCString());
                particle->setScale(2.3f);
                if (CCCommonUtils::isIosAndroidPad()) {
                    particle->setScale(4.3f);
                }
                m_particle->addChild(particle);
            }
        }
        
        m_nameLabel->setString(_lang(curInfo.name));
        m_lvLabel->setString(_lang("102203") + " " + CC_ITOA(curInfo.level));
        
        for (int i=1; i<=5; i++)
        {
            string tmpInfo = "";
            if (curInfo.showDias.find(i) != curInfo.showDias.end()) {
                string dialog = curInfo.showDias[i];
                if (dialog!="" && curInfo.values.find(i)!=curInfo.values.end()) {
                    tmpInfo = tmpInfo+"+"+CC_ITOA_MONEY(curInfo.values[i])+"%";
                }
                
                if (i==1) {
                    m_prar1Label->setString(_lang(dialog));
                    m_num1Label->setString(tmpInfo);
                }else if (i==2) {
                    m_prar2Label->setString(_lang(dialog));
                    m_num2Label->setString(tmpInfo);
                    if(curInfo.site==6){
                        m_prar2Label->setColor(rp_color);
                        m_num2Label->setColor(rp_color);
                    }
                }else if (i==3) {
                    m_prar3Label->setString(_lang(dialog));
                    m_num3Label->setString(tmpInfo);
                }else if (i==4) {
                    m_prar4Label->setString(_lang(dialog));
                    m_num4Label->setString(tmpInfo);
                    if(curInfo.site==6){
                        m_prar4Label->setColor(rp_color);
                        m_num4Label->setColor(rp_color);
                    }
                }else if (i==5) {
                    m_prar5Label->setString(_lang(dialog));
                    m_num5Label->setString(tmpInfo);
                }
            }
        }
        
//        m_middleNode->setPositionY(120);
//        m_middle1Node->setPositionY(-224);
        m_infoList->setContentSize(m_tableViewSize);
        m_clipNode->setContentSize(m_infoList->getContentSize());
        m_tabView->setViewSize(m_infoList->getContentSize());
        
    }else {
        m_msgNode->setVisible(true);
        m_equipNode->setVisible(false);
        
//        m_middleNode->setPositionY(120+150);
//        m_middle1Node->setPositionY(-224+150);
        m_infoList->setContentSize(m_tableViewSize);//+CCSize(620, 150)
        m_clipNode->setContentSize(m_infoList->getContentSize());
        m_tabView->setViewSize(m_infoList->getContentSize());
    }
}

SEL_CCControlHandler EquipNewUseView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTakeOffClick", EquipNewUseView::onTakeOffClick);
    return NULL;
}

bool EquipNewUseView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_middleNode", CCNode*, this->m_middleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_middle1Node", CCNode*, this->m_middle1Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_middle2Node", CCNode*, this->m_middle2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipNode", CCNode*, this->m_equipNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particle", CCNode*, this->m_particle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_takeOffBtn", CCControlButton*, this->m_takeOffBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvLabel", CCLabelIF*, this->m_lvLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgLabel", CCLabelIF*, this->m_msgLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgNode", CCNode*, this->m_msgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noEquipLabel", CCLabelIF*, this->m_noEquipLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_prar1Label", CCLabelIF*, this->m_prar1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_prar2Label", CCLabelIF*, this->m_prar2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_prar3Label", CCLabelIF*, this->m_prar3Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_prar4Label", CCLabelIF*, this->m_prar4Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_prar5Label", CCLabelIF*, this->m_prar5Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num1Label", CCLabelIF*, this->m_num1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num2Label", CCLabelIF*, this->m_num2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num3Label", CCLabelIF*, this->m_num3Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num4Label", CCLabelIF*, this->m_num4Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num5Label", CCLabelIF*, this->m_num5Label);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_aniNode", CCNode*, this->m_aniNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_playNode", CCNode*, this->m_playNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_shadeNode", CCNode*, this->m_shadeNode);
    
    return false;
}

void EquipNewUseView::onTakeOffClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (!CanClick) {
        return;
    }
    if (curHaveOnUuid != "") {
        onShowWateInterface(NULL);
        EquipmentController::getInstance()->startTakeOffEquipment(curHaveOnUuid);
//        PopupViewController::getInstance()->addPopupView(EquipConfrimView::create(curHaveOnUuid));
    }
    return;
}

bool EquipNewUseView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_startPoint = pTouch->getStartLocation();
    if(isTouchInside(m_infoList, pTouch)){
        return false;
    }
    return true;
}

void EquipNewUseView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint endPoint = pTouch->getLocation();
    if(fabs(endPoint.x-m_startPoint.x)>10||fabs(endPoint.y-m_startPoint.y)>10){
        return;
    }
}

CCSize EquipNewUseView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data.size()){
        return CCSizeZero;
    }
    
    string itemId = m_data[idx];
    if(itemId!="" && m_openMap.find(itemId) != m_openMap.end()){
        if (m_openMap[itemId] == 1) {
            if (CCCommonUtils::isIosAndroidPad()) {
                return CCSize(1488, 672);
            }
            return CCSize(620, 280);
        }else {
            if (CCCommonUtils::isIosAndroidPad()) {
                return CCSize(1488, 432);
            }
            return CCSize(620, 180);
        }
    }
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1488, 432);
    }
    return CCSize(620, 180);
}

CCSize EquipNewUseView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1488, 432);
    }
    return CCSize(620, 180);
}

CCTableViewCell* EquipNewUseView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    string itemId = m_data[idx];
    EquipNewUseCell* cell = (EquipNewUseCell*)table->dequeueCell();
    if(cell){
        cell->setData(itemId, m_openMap[itemId]);
    }else{
        cell = EquipNewUseCell::create(itemId, m_infoList, m_openMap[itemId]);
    }
    return cell;
}

ssize_t EquipNewUseView::numberOfCellsInTableView(CCTableView *table)
{
    int num = ceil(m_data.size() / 1.0f);
    return num;
}

void EquipNewUseView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}

void EquipNewUseView::onShowWateInterface(CCObject * obj)
{
    m_waitInterface= GameController::getInstance()->showWaitInterface(m_shadeNode, 0);
}

void EquipNewUseView::onPlayPutOnEquip(CCObject * obj)
{
    if (obj) {
        CCDictionary* dict = dynamic_cast<CCDictionary*>(obj);
        if (dict) {
            CanClick = false;
            int startX = dict->valueForKey("ptx")->intValue();
            int startY = dict->valueForKey("pty")->intValue();
            m_willPutOnUuid = dict->valueForKey("uuid")->getCString();
            CCLOG("xx %d, yy %d, uuid: %s", startX, startY, m_willPutOnUuid.c_str());
            
            CCLOG("end");
            
            auto endPt = m_iconNode->getParent()->convertToWorldSpace(m_iconNode->getPosition());
            auto winSize = CCDirector::sharedDirector()->getWinSize();
            
            m_playNode->removeAllChildren();
            m_playNode->setPosition(startX, startY);
            
            CCLoadSprite::doResourceByCommonIndex(305, true);
            auto frameBg = CCLoadSprite::createSprite("Lord_icon_box.png");
            if (CCCommonUtils::isIosAndroidPad()) {
                frameBg->setScale(2.2);
            }
            else
                frameBg->setScale(1.1);
            m_playNode->addChild(frameBg);
            
            int tmpItemId = EquipmentController::getInstance()->m_MyEquipmentMap[m_willPutOnUuid];
            auto& info = EquipmentController::getInstance()->EquipmentInfoMap[tmpItemId];
            string bgStr = CCCommonUtils::getToolBgByColor(info.color);
            auto bg = CCLoadSprite::createSprite(bgStr.c_str());
            m_playNode->addChild(bg);
            string picStr = CCCommonUtils::getIcon(CC_ITOA(tmpItemId));
            auto icon = CCLoadSprite::createSprite(picStr.c_str(), true,CCLoadSpriteType_EQUIP);
            if (CCCommonUtils::isIosAndroidPad()) {
                CCCommonUtils::setSpriteMaxSize(icon, 200, true);
            }
            else
                CCCommonUtils::setSpriteMaxSize(icon, 100, true);
            m_playNode->addChild(icon);
            
            CCActionInterval * moveTo0 = CCMoveTo::create(0.5,ccp(winSize.width/2, winSize.height/2));
            CCActionInterval * easeExponentialOut1= CCEaseExponentialInOut::create(moveTo0);
            CCActionInterval * scaleTo0 = CCScaleTo::create(0.5,1.5);
            CCActionInterval * easeExponentialOut2= CCEaseExponentialInOut::create(scaleTo0);
            CCFiniteTimeAction * spawn0 =CCSpawn::create(easeExponentialOut1,easeExponentialOut2,NULL);
            
            CCActionInterval * delayT = CCDelayTime::create(0.3);
            
            CCActionInterval * moveTo1 = CCMoveTo::create(0.2,endPt);
            CCActionInterval * scaleTo1 = CCScaleTo::create(0.2,0.8);
            CCFiniteTimeAction * spawn1 =CCSpawn::create(moveTo1,scaleTo1,NULL);
            
            CCActionInterval * scaleTo2 = CCScaleTo::create(0.1,1.0);
            CCActionInterval * scaleTo3 = CCScaleTo::create(0.1,0.85);
            CCActionInterval * scaleTo4 = CCScaleTo::create(0.1,0.9);
            CCActionInterval * delayT2 = CCDelayTime::create(0.3);
            
            CCCallFunc * funcall0= CCCallFunc::create(this, callfunc_selector(EquipNewUseView::onPlayParticle));
            
            CCCallFunc * funcall= CCCallFunc::create(this, callfunc_selector(EquipNewUseView::onPostPutOn));
            m_playNode->runAction(CCSequence::create(funcall, spawn0, delayT, spawn1, funcall0, scaleTo2, scaleTo3, scaleTo4, delayT2, NULL));
        }
    }
}

void EquipNewUseView::onPlayParticle()
{
    auto node = CCNode::create();
    m_playNode->addChild(node, -1);
    string tmpStart = "ScienceOut_";
    int count = 8;
    for (int i=1; i<=count; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        if (CCCommonUtils::isIosAndroidPad()) {
            particle->setScale(2.4);
        }
        else
            particle->setScale(1.2);
        node->addChild(particle);
    }
    SoundController::sharedSound()->playEffects(Music_Sfx_UI_putonequipment);
}

void EquipNewUseView::onPostPutOn()
{
    CanClick = true;
    if (CCCommonUtils::isIosAndroidPad()) {
        m_playNode->setScale(1.8);
    }
    else
        m_playNode->setScale(0.9);
    
    if (m_willPutOnUuid!="") {
        onShowWateInterface(NULL);
        EquipmentController::getInstance()->startPutOnEquipment(m_willPutOnUuid, curHaveOnUuid);
    }
}

//class cell
EquipNewUseCell* EquipNewUseCell::create(string itemId, CCNode* touchNode, int openState)
{
    auto ret = new EquipNewUseCell();
    if (ret && ret->init(itemId, touchNode, openState)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipNewUseCell::init(string itemId, CCNode* touchNode, int openState)
{
    bool ret = true;
    CCLoadSprite::doResourceByCommonIndex(305, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(305, false);
    });
    
    m_parentNode = touchNode;
    CCBLoadFile("EquipNewUseCell",this,this);
    CCCommonUtils::setButtonTitle(m_putOnBtn, _lang("119002").c_str());
    if (CCCommonUtils::isIosAndroidPad()) {
        setContentSize(CCSize(1488, 432));
    }
    else
        setContentSize(CCSize(620, 180));
    
    string wName = "";
    switch (WeapType) {
        case 0:
            wName = _lang("119014");
            break;
        case 1:
            wName = _lang("119015");
            break;
        case 2:
            wName = _lang("119016");
            break;
        case 3:
            wName = _lang("119017");
            break;
        case 4:
            wName = _lang("119018");
            break;
        case 5:
            wName = _lang("119019");
            break;
        default:
            break;
    }
    CCCommonUtils::setButtonTitle(m_gotoBtn, _lang("119069").c_str());
    m_gotoLabel->setString(_lang_1("119058",wName.c_str()));
    if(WeapType==6){
        m_gotoLabel->setString(_lang("111653"));
    }
    
    setData(itemId, openState);
    return ret;
}

void EquipNewUseCell::setData(string itemId, int openState)
{
    m_uuid = itemId;
    
    m_prar1Label->setString("");
    m_prar2Label->setString("");
    m_prar3Label->setString("");
    m_prar4Label->setString("");
    m_prar5Label->setString("");
    m_num1Label->setString("");
    m_num2Label->setString("");
    m_num3Label->setString("");
    m_num4Label->setString("");
    m_num5Label->setString("");
    
    if (CCCommonUtils::isIosAndroidPad()) {
        m_touchNode->setContentSize(CCSize(1488, 432));
    }
    else
        m_touchNode->setContentSize(CCSize(620, 180));
    if (m_uuid != "")
    {
        m_mainNode->setVisible(true);
        m_equipNode->setVisible(true);
        m_bottomNode->setVisible(true);
        m_gotoNode->setVisible(false);
        m_gotoBtn->setVisible(false);
        
        m_picNode->removeAllChildren();
        
        int m_itemId = EquipmentController::getInstance()->m_MyEquipmentMap[m_uuid];
        auto& info = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
        if(info.site==6){
            info = EquipmentController::getInstance()->LongEquipmentInfoMap[m_uuid];
        }
        
        string bgStr = CCCommonUtils::getToolBgByColor(info.color);
        auto bg = CCLoadSprite::createSprite(bgStr.c_str());
        m_picNode->addChild(bg);
        string picStr = CCCommonUtils::getIcon(CC_ITOA(m_itemId));
        auto icon = CCLoadSprite::createSprite(picStr.c_str(), true,CCLoadSpriteType_EQUIP);
        CCCommonUtils::setSpriteMaxSize(icon, 100, true);
        m_picNode->addChild(icon);
        //特效
        if(info.site==6){
            m_particle->removeAllChildren();
            int part=1;
            if(1==info.color){//绿
                part = 3;
            }else if (2==info.color){//蓝
                part = 1;
            }else if (3==info.color){//紫
                part = 4;
            }else if (4==info.color){//橙
                part = 5;
            }else if (5==info.color){//金
                part = 2;
            }
            for (int par=1; par<=5; par++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("Cristal_%d_%d",part,par)->getCString());
                particle->setScale(2.3f);
                m_particle->addChild(particle);
            }
        }
        
        m_nameLabel->setString(_lang(info.name));
        m_lvLabel->setString(_lang("102203") + " " + CC_ITOA(info.level));
//        m_msgLabel->setString(_lang(info.des));
        
        if(info.showType == "no")
        {
            m_putOnBtn->setVisible(false);
            m_cannotEquipLabel->setVisible(true);
            m_cannotEquipLabel->setString(_lang("119075"));
        }
        else
        {
            m_putOnBtn->setVisible(true);
            m_cannotEquipLabel->setVisible(false);
        }
        
        if(openState == 1) {
            m_msgNode->setVisible(true);
            if (CCCommonUtils::isIosAndroidPad()) {
                m_mainNode->setPositionY(136);
                m_touchNode->setContentSize(CCSize(1488, 672));
            }
            else{
                m_mainNode->setPositionY(136);
                m_touchNode->setContentSize(CCSize(620, 280));
            }
            
            if(info.showType == "no")
            {
                m_prar1Label->setString(_lang("108730"));
                m_desLabel->setVisible(true);
                m_desLabel->setString(_lang(info.des));
            }
            else
            {
                m_desLabel->setVisible(false);
                for (int i=1; i<=5; i++)
                {
                    string tmpInfo = "";
                    if (info.showDias.find(i) != info.showDias.end()) {
                        string dialog = info.showDias[i];
                        if (dialog!="" && info.values.find(i)!=info.values.end()) {
                            tmpInfo = tmpInfo+"+"+CC_ITOA_MONEY(info.values[i])+"%";
                        }
                        if (i==1) {
                            m_prar1Label->setString(_lang(dialog));
                            m_prar1Label->setMaxScaleXByWidth(320);
                            m_num1Label->setString(tmpInfo);
                        }else if (i==2) {
                            m_prar2Label->setString(_lang(dialog));
                            m_prar2Label->setMaxScaleXByWidth(320);
                            m_num2Label->setString(tmpInfo);
                            if(info.site==6){
                                m_prar2Label->setColor(rp_color);
                                m_num2Label->setColor(rp_color);
                            }
                        }else if (i==3) {
                            m_prar3Label->setString(_lang(dialog));
                            m_prar3Label->setMaxScaleXByWidth(320);
                            m_num3Label->setString(tmpInfo);
                        }else if (i==4) {
                            m_prar4Label->setString(_lang(dialog));
                            m_prar4Label->setMaxScaleXByWidth(320);
                            m_num4Label->setString(tmpInfo);
                            if(info.site==6){
                                m_prar4Label->setColor(rp_color);
                                m_num4Label->setColor(rp_color);
                            }
                        }else if (i==5) {
                            m_prar5Label->setString(_lang(dialog));
                            m_prar5Label->setMaxScaleXByWidth(320);
                            m_num5Label->setString(tmpInfo);
                        }
                    }
                }
            }
            
        }else {
            m_msgNode->setVisible(false);
            m_mainNode->setPositionY(28);
        }
    }
    else {
        m_mainNode->setPositionY(28);
        m_mainNode->setVisible(true);
        m_msgNode->setVisible(false);
        m_bottomNode->setVisible(true);
        m_equipNode->setVisible(false);
        
        m_gotoNode->setVisible(true);
        m_gotoBtn->setVisible(true);
    }
}

void EquipNewUseCell::onEnter() {
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}

void EquipNewUseCell::onExit() {
    setTouchEnabled(false);
    CCNode::onExit();
}

void EquipNewUseCell::onPutOnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
//    PopupViewController::getInstance()->addPopupView(EquipConfrimView::create(m_uuid));
    if (!CanClick) {
        return;
    }
    
    int m_itemId = EquipmentController::getInstance()->m_MyEquipmentMap[m_uuid];
    auto& info = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
    if(info.level > GlobalData::shared()->playerInfo.level) {
        YesNoDialog::show(_lang("119040").c_str(), NULL);
        return;
    }
    
    auto dict = CCDictionary::create();
    auto pt3 = m_picNode->getParent()->convertToWorldSpace(m_picNode->getPosition());
    dict->setObject(CCString::create(CC_ITOA(pt3.x)), "ptx");
    dict->setObject(CCString::create(CC_ITOA(pt3.y)), "pty");
    dict->setObject(CCString::create(m_uuid), "uuid");
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_PLAY_EQUIP_PUT,dict);
    
    //设置该栏位是否装备过
    string udString = "user_had_equip_";
    udString+=CC_ITOA(info.site);
    CCUserDefault::sharedUserDefault()->setBoolForKey(udString.c_str(), true);
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_POST_NET_PUT);
//    EquipmentController::getInstance()->startPutOnEquipment(m_uuid, curHaveOnUuid);
    return;
}

SEL_CCControlHandler EquipNewUseCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPutOnClick", EquipNewUseCell::onPutOnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGoToClick", EquipNewUseCell::onGoToClick);
    return NULL;
}

bool EquipNewUseCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_putOnBtn", CCControlButton*, m_putOnBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particle", CCNode*, m_particle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvLabel", CCLabelIF*, m_lvLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgLabel", CCLabelIF*, m_msgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cannotEquipLabel", CCLabelIF*, m_cannotEquipLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgNode", CCNode*, m_msgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_prar1Label", CCLabelIF*, m_prar1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_prar2Label", CCLabelIF*, m_prar2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_prar3Label", CCLabelIF*, m_prar3Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_prar4Label", CCLabelIF*, m_prar4Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_prar5Label", CCLabelIF*, m_prar5Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num1Label", CCLabelIF*, this->m_num1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num2Label", CCLabelIF*, this->m_num2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num3Label", CCLabelIF*, this->m_num3Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num4Label", CCLabelIF*, this->m_num4Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num5Label", CCLabelIF*, this->m_num5Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, this->m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_gotoBtn", CCControlButton*, m_gotoBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipNode", CCNode*, m_equipNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_gotoNode", CCNode*, m_gotoNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_gotoLabel", CCLabelIF*, m_gotoLabel);
    return false;
}

bool EquipNewUseCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!CanClick) {
        return false;
    }
    m_startPoint = pTouch->getStartLocation();
    if (isTouchInside(m_parentNode,pTouch)){
        if(isTouchInside(m_touchNode,pTouch)){
            return true;
        }
    }
    return false;
}

void EquipNewUseCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint endPoint = pTouch->getLocation();
    if(fabs(endPoint.x-m_startPoint.x)>10||fabs(endPoint.y-m_startPoint.y)>10){
        return;
    }
    
    if (isTouchInside(m_putOnBtn,pTouch)){
    }else if (m_uuid != "" ){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CLICK_VIEW, CCString::create(m_uuid));
    }
}

void EquipNewUseCell::onGoToClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (!CanClick) {
        return;
    }
    
    m_qid = QueueController::getInstance()->getCanRecQidByType(TYPE_FORGE);
    if (m_qid != QID_MAX) {
        YesNoDialog::show(_lang("119060").c_str(), CCCallFunc::create(this, callfunc_selector(EquipNewUseCell::onHavestEquip)));
    }else {
        if(WeapType==6||WeapType==7){
            PopupViewController::getInstance()->addPopupInView(LongJingStoreView::create());
        }else{
            int buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_FORGE);
            if(buildId==0){
                return;
            }
            PopupViewController::getInstance()->addPopupInView(EquipmentListView::create(WeapType));
        }
    }
}

void EquipNewUseCell::onHavestEquip()
{
    if (m_qid != QID_MAX) {
        QueueController::getInstance()->startFinishQueue(m_qid, false);
        map<int, QueueInfo>::iterator it =  GlobalData::shared()->allQueuesInfo.find(m_qid);
        EquipmentController::getInstance()->startFinishCrtEquip(it->second.uuid);
        QueueController::getInstance()->startFinishQueue((it->second).qid, false);
        
        int m_buildingKey = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_FORGE);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REMOVE_STATE, CCInteger::create(m_buildingKey));
    }
}

//class EquipInfoView

EquipConfrimView* EquipConfrimView::create(string itemId)
{
    auto ret = new EquipConfrimView();
    if (ret && ret->init(itemId)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipConfrimView::init(string itemId)
{
    bool ret = true;
    CCLoadSprite::doResourceByCommonIndex(11, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
    });
    
    if (PopupBaseView::init()) {
        CCBLoadFile("EquipConfrimView",this,this);
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        setData(itemId);
    }
    return ret;
}

void EquipConfrimView::setData(string itemId)
{
    m_itemId = itemId;
    
    int tmpItemId = 0;
    if (m_itemId == curHaveOnUuid) {
        tmpItemId = EquipmentController::getInstance()->m_MyOnEquip[m_itemId];
    }else {
        tmpItemId = EquipmentController::getInstance()->m_MyEquipmentMap[m_itemId];
    }
    
    EquipmentInfo& info = EquipmentController::getInstance()->EquipmentInfoMap[tmpItemId];
    string strName = _lang(info.name) + "   "+_lang("102203")+" " + CC_ITOA(info.level);
    m_nameLabel->setString( _lang(info.name) );
    m_lvLabel->setString(_lang("102203")+" " + CC_ITOA(info.level));
    m_msgLabel->setString(_lang(info.des));
    
    string iconPath = CCCommonUtils::getIcon(CC_ITOA(tmpItemId));
    string bgPath = CCCommonUtils::getToolBgByColor(info.color);
    
    auto icon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_EQUIP);
    CCCommonUtils::setSpriteMaxSize(icon, 100, true);
    auto bg = CCLoadSprite::createSprite(bgPath.c_str());
    CCCommonUtils::setSpriteMaxSize(bg, 100, true);
    m_picNode->addChild(bg);
    m_picNode->addChild(icon);
    
    int curY = 0;
    if (m_itemId == curHaveOnUuid)
    {//显示脱下装备
        for (int i=1; i<=4; i++)
        {
            if (info.showDias.find(i) != info.showDias.end())
            {
                string dialog = info.showDias[i];
                float value = info.values[i];
                string msg2 = "";
                msg2 = msg2 + " -"+CC_ITOA_MONEY(value)+"%";
                auto msgCell = EquipMsgCell::create(_lang(dialog), msg2, false);
//                auto msgCell = EquipMsgCell::create(_lang_1(dialog, CC_ITOA_1(value)), "");
                msgCell->setPosition(ccp(0, curY));
                m_infoList->addChild(msgCell);
                curY -= 30;
            }else {
                break;
            }
        }
        CCCommonUtils::setButtonTitle(m_okBtn, _lang("119003").c_str());
    }
    else
    {//显示穿上装备
        CCCommonUtils::setButtonTitle(m_okBtn, _lang("119002").c_str());
        if (curHaveOnUuid == "")
        {
            for (int i=1; i<=4; i++)
            {
                if (info.showDias.find(i) != info.showDias.end())
                {
                    string dialog = info.showDias[i];
                    float value = info.values[i];
                    string msg2 = "";
                    msg2 = msg2 + " +"+CC_ITOA_MONEY(value)+"%";
                    auto msgCell = EquipMsgCell::create(_lang(dialog), msg2);
//                    auto msgCell = EquipMsgCell::create(_lang_1(dialog, CC_ITOA_1(value)), "");
                    msgCell->setPosition(ccp(0, curY));
                    m_infoList->addChild(msgCell);
                    curY -= 30;
                }else {
                    break;
                }
            }
            
        }else {
            
            map<string, float> willDialog;
            map<string, float> repeateDia;
            for (int i=1; i<=4; i++)
            {
                if (info.showDias.find(i) != info.showDias.end())
                {
                    string dialog = info.showDias[i];
                    float value = info.values[i];
                    willDialog[dialog] = value;
                }else {
                    break;
                }
            }
            
            int curOnItemId = EquipmentController::getInstance()->m_MyOnEquip[curHaveOnUuid];
            EquipmentInfo& curInfo = EquipmentController::getInstance()->EquipmentInfoMap[curOnItemId];
            
            for (int i=1; i<=4; i++)
            {
                if (curInfo.showDias.find(i) != curInfo.showDias.end())
                {
                    string dialog = curInfo.showDias[i];
                    float value = curInfo.values[i];
                    string msg2 = "";
                    bool isAdd = true;
                    if (willDialog.find(dialog) == willDialog.end()) { // 更换后 没有此项
                        msg2 = msg2+"-"+CC_ITOA_MONEY(value)+"%";
                        isAdd = false;
                    }else {
                        repeateDia[dialog] = value;
                        float addValue = willDialog[dialog]-value;
                        if (addValue>=0) {
                            msg2 = msg2+"+"+CC_ITOA_MONEY(addValue)+"%";
                        }else {
                            msg2 = msg2+CC_ITOA_MONEY(addValue)+"%";
                            isAdd = false;
                        }
                    }
                    
                    auto msgCell = EquipMsgCell::create(_lang(dialog)+" +"+CC_ITOA_MONEY(value)+"%", msg2, isAdd);
//                    auto msgCell = EquipMsgCell::create(_lang_1(dialog, CC_ITOA_1(value)), msg2, isAdd);
                    msgCell->setPosition(ccp(0, curY));
                    m_infoList->addChild(msgCell);
                    curY -= 30;
                }else {
                    break;
                }
            }
            
            map<string, float>::iterator it = willDialog.begin();
            for (; it!=willDialog.end(); it++) {
                if (repeateDia.find(it->first) == repeateDia.end()) {
                    string dialog = it->first;
                    float value = it->second;
                    string msg2 = "";
                    msg2 = msg2+"+"+CC_ITOA_MONEY(value)+"%";
                    auto msgCell = EquipMsgCell::create(_lang(dialog)+" +"+CC_ITOA_1(0)+"%", msg2);
//                    auto msgCell = EquipMsgCell::create(_lang_1(dialog, CC_ITOA_1(0)), msg2);
                    msgCell->setPosition(ccp(0, curY));
                    m_infoList->addChild(msgCell);
                    curY -= 30;
                }
            }
            
        }
    }
}

void EquipConfrimView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}

void EquipConfrimView::onExit()
{
    setTouchEnabled(false);
    CCNode::onExit();
}

bool EquipConfrimView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_touchNode,pTouch)){
        return false;
    }
    return true;
}

void EquipConfrimView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isTouchInside(m_touchNode,pTouch)){
        PopupViewController::getInstance()->removePopupView(this);
    }
}

void EquipConfrimView::onOKClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->removePopupView(this);
    if (curHaveOnUuid != "" && curHaveOnUuid == m_itemId) {
        EquipmentController::getInstance()->startTakeOffEquipment(curHaveOnUuid);
    } else {
        EquipmentController::getInstance()->startPutOnEquipment(m_itemId, curHaveOnUuid);
    }
    return;
}

SEL_CCControlHandler EquipConfrimView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOKClick", EquipConfrimView::onOKClick);
    return NULL;
}

bool EquipConfrimView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_okBtn", CCControlButton*, m_okBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, m_bg);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvLabel", CCLabelIF*, m_lvLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgLabel", CCLabelIF*, m_msgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    
    return false;
}

//class msg cell
EquipMsgCell* EquipMsgCell::create(string msg1, string msg2, bool isAdd)
{
    auto ret = new EquipMsgCell();
    if (ret && ret->init(msg1, msg2, isAdd)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
bool EquipMsgCell::init(string msg1, string msg2, bool isAdd)
{
    bool ret = true;
    CCBLoadFile("EquipMsgCell",this,this);
    m_msg1Label->setString(msg1);
    
    if (!isAdd) {
        m_msg2Label->setColor(ccRED);
    }
    
    m_msg2Label->setString(msg2);
    return ret;
}
void EquipMsgCell::onEnter() {
    CCNode::onEnter();
}
void EquipMsgCell::onExit() {
    CCNode::onExit();
}
SEL_CCControlHandler EquipMsgCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}
bool EquipMsgCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg1Label", CCLabelIF*, m_msg1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg2Label", CCLabelIF*, m_msg2Label);
    return false;
}