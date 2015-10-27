//
//  UseToolView.cpp
//  IF
//
//  Created by fubin on 14-3-14.
//
//

#include "UseToolView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "FunBuildController.h"
#include "YesNoDialog.h"
#include "ToolInfo.h"
#include "ToolController.h"
#include "ArmyController.h"
#include "WorldController.h"
#include "WorldMapView.h"
#include "VipUtil.h"
#include "StoreBagView.h"
#include "StoreBuyConfirmDialog.h"

const float numPerRow = 1.0;
UseToolView::~UseToolView(){
    if(m_type == USE_TOOL_VIP_ACTIVITY){
        int dtime = GlobalData::shared()->playerInfo.vipEndTime - GlobalData::shared()->getWorldTime();
        if(dtime>0){
            VipUtil::showVIPActiveView();
        }
    }
}
UseToolView* UseToolView::create(string type, CCDictionary* dict,string title){
    UseToolView* ret = new UseToolView();
    if(ret && ret->init(type, dict,title)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool UseToolView::init(string type, CCDictionary* dict,string title)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    setCleanFunction([](){
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
    });
    m_type = type;
    m_dict = dict;
    auto tmpCCB = CCBLoadFile("UseToolView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
    int oldBgHeight = m_buildBG->getContentSize().height;
    changeBGHeight(m_buildBG);
    int newBgHeight = m_buildBG->getContentSize().height;
    int addHeight = newBgHeight - oldBgHeight;
    int oldWidth = m_infoList->getContentSize().width;
    int oldHeight = m_infoList->getContentSize().height;
    m_numTTF->setVisible(false);
    if (m_type == USE_TOOL_LOTTERY1){
        m_numTTF->setVisible(true);
        string name = _lang("111101");
        name.append(":").append(CC_CMDITOAL(GlobalData::shared()->resourceInfo.lChip));
        m_numTTF->setString(name.c_str());
    }else if (m_type == USE_TOOL_LOTTERY2){
        m_numTTF->setVisible(true);
        string name = _lang("111102");
        name.append(":").append(CC_CMDITOAL(GlobalData::shared()->resourceInfo.lDiamond));
        m_numTTF->setString(name.c_str());
    }else if(m_type == USE_TOOL_VIP_ACTIVITY){
        auto &playerInfo = GlobalData::shared()->playerInfo;
        int dTime = playerInfo.vipEndTime - GlobalData::shared()->getWorldTime();
        if(dTime>0){
            m_numTTF->setVisible(true);
            m_numTTF->setString(_lang("108659") + CC_SECTOA(dTime));
            this->schedule(schedule_selector(UseToolView::onTimer),1.0);
        }else{
            m_numTTF->setVisible(false);
            oldHeight += 20;
        }
    }else if(m_type == USE_TOOL_VIP_PIC){
        m_numTTF->setVisible(true);
        auto &playerInfo = GlobalData::shared()->playerInfo;
        m_numTTF->setString(_lang("103010") + CC_CMDITOA(playerInfo.vipPoints));
    }else{
        oldHeight += 20;
    }
    m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
    m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
    
    if (title == "") {
        if (m_type == USE_TOOL_MARCH_CANCEL)
        {
            setTitleName(_lang("104039").c_str());
        }else if (m_type == USE_TOOL_MARCH_CANCEL_RALLY) {
            setTitleName(_lang("115191").c_str());//解散
        }else{
            setTitleName(_lang("104903").c_str());
        }
    }
    else
    {
        setTitleName(_lang(title).c_str());
    }
    
    m_tabView = CCMultiColTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setMultiColTableViewDelegate(this);
//    m_tabView->setTouchPriority(Touch_Popup);
    m_infoList->addChild(m_tabView);
    
    
    oldBgHeight = m_buildBG->getContentSize().height;//fusheng
    changeBGMaxHeight(m_buildBG);
    m_buildBG->setPositionY(m_buildBG->getPositionY()+m_buildBG->getContentSize().height-oldBgHeight);
    
    return true;
}
void UseToolView::onTimer(float dt){
    if(m_type == USE_TOOL_VIP_ACTIVITY){
        auto &playerInfo = GlobalData::shared()->playerInfo;
        int dTime = playerInfo.vipEndTime - GlobalData::shared()->getWorldTime();
        if(dTime>0){
            m_numTTF->setVisible(true);
            m_numTTF->setString(_lang("108659") + CC_SECTOA(dTime));
        }else{
            m_numTTF->setVisible(false);
            this->unschedule(schedule_selector(UseToolView::onTimer));
        }
    }
}
void UseToolView::updateInfo()
{
    m_curList.clear();
    if (m_type == USE_TOOL_CITY_MOVE) {
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(ITEM_FREE_MOVE_CITY);//获取新手迁城道具
        if (info.getCNT()>0) {
            m_curList.push_back(ITEM_FREE_MOVE_CITY);
        }
        m_curList.push_back(ITEM_MOVE_CITY);
    }
    else if (m_type == USE_TOOL_MARCH_CANCEL) {
        m_curList.push_back(200004);
//        m_curList.push_back(200006);//高级行军召回
    }
    else if (m_type == USE_TOOL_MARCH_CANCEL_RALLY) {
        m_curList.push_back(200006);//高级行军召回
    }
    else if (m_type == USE_TOOL_CHANGE_NAME) {
        m_curList.push_back(ITEM_RENAME);
    }
    else if (m_type == USE_TOOL_MATE_BOX) {
        m_curList.push_back(200603);
    }
    else if (m_type == USE_TOOL_CHANGE_PIC) {
        m_curList.push_back(ITEM_CHANGE_PIC);
    }else if (m_type == USE_TOOL_VIP_PIC) {
        std::vector<int>* m_resList= &(ToolController::getInstance()->m_typeTools[3]);
        for (int i = 0; i<m_resList->size(); i++) {
            auto& info = ToolController::getInstance()->getToolInfoById((*m_resList)[i]);
            if (info.type2 == 11) {
                if(info.getCNT()>0){
                    m_curList.push_back(info.itemId);
                }else if(info.price>0){
                    m_curList.push_back(info.itemId);
                }
            }
        }
//        m_curList.push_back(200350);
//        m_curList.push_back(200351);
//        m_curList.push_back(200352);
    }else if (m_type == USE_TOOL_VIP_ACTIVITY) {
        std::vector<int>* m_resList= &(ToolController::getInstance()->m_typeTools[3]);
        for (int i = 0; i<m_resList->size(); i++) {
            auto& info = ToolController::getInstance()->getToolInfoById((*m_resList)[i]);
            if (info.type2 == 12) {
                if(info.getCNT()>0){
                    m_curList.push_back(info.itemId);
                }else if(info.price>0){
                    m_curList.push_back(info.itemId);
                }
            }
        }
//        m_curList.push_back(200370);
//        m_curList.push_back(200371);
//        m_curList.push_back(200372);
    }else if (m_type == USE_TOOL_LOTTERY1){
        std::vector<int>* m_resList= &(ToolController::getInstance()->m_typeTools[3]);
        for (int i = 0; i<m_resList->size(); i++) {
            auto& info = ToolController::getInstance()->getToolInfoById((*m_resList)[i]);
            if (info.type2 == 15) {
                if(info.getCNT()>0){
                    m_curList.push_back(info.itemId);
                }else if(info.price>0){
                    m_curList.push_back(info.itemId);
                }
            }
        }
    }else if (m_type == USE_TOOL_LOTTERY2){
        std::vector<int>* m_resList= &(ToolController::getInstance()->m_typeTools[3]);
        for (int i = 0; i<m_resList->size(); i++) {
            auto& info = ToolController::getInstance()->getToolInfoById((*m_resList)[i]);
            if (info.type2 == 16) {
                if(info.getCNT()>0){
                    m_curList.push_back(info.itemId);
                }else if(info.price>0){
                    m_curList.push_back(info.itemId);
                }
            }
        }
    }
    
    m_tabView->reloadData();
}

void UseToolView::refreashData(CCObject* obj)
{
    CCPoint offset = m_tabView->getContentOffset();
    CCSize containerSize = m_tabView->getContainer()->getContentSize();
    if (m_type == USE_TOOL_VIP_ACTIVITY || m_type == USE_TOOL_VIP_PIC) {
        auto &playerInfo = GlobalData::shared()->playerInfo;
        if(m_type == USE_TOOL_VIP_PIC){
            m_numTTF->setString(_lang("103010") + CC_CMDITOA(playerInfo.vipPoints));
        }else{
            this->unschedule(schedule_selector(UseToolView::onTimer));
            int dTime = playerInfo.vipEndTime - GlobalData::shared()->getWorldTime();
            if(dTime>0){
                m_numTTF->setVisible(true);
                m_numTTF->setString(_lang("108659") + CC_SECTOA(dTime));
                this->schedule(schedule_selector(UseToolView::onTimer),1.0);
            }else{
                m_numTTF->setVisible(false);
            }
        }
        updateInfo();
    }else if (m_type == USE_TOOL_LOTTERY1 || m_type == USE_TOOL_LOTTERY2){
        updateInfo();
        long num = m_type==USE_TOOL_LOTTERY1?GlobalData::shared()->resourceInfo.lChip:GlobalData::shared()->resourceInfo.lDiamond;
        string name = m_type==USE_TOOL_LOTTERY1?_lang("111101"):_lang("111102");
        name.append(":").append(CC_CMDITOAL(num));
        m_numTTF->setString(name);
    }
    CCSize containerSize1 = m_tabView->getContainer()->getContentSize();
    offset.y = offset.y - (containerSize1.height - containerSize.height);
    m_tabView->setContentOffset(offset);
}

void UseToolView::onEnter()
{
    CCNode::onEnter();
    updateInfo();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UseToolView::refreashData), MSG_REFREASH_TOOL_DATA, NULL);
}

void UseToolView::onExit()
{
    this->unschedule(schedule_selector(UseToolView::onTimer));
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_REFREASH_TOOL_DATA);
    CCNode::onExit();
}

SEL_CCControlHandler UseToolView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool UseToolView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numTTF", CCLabelIF*, this->m_numTTF);
    
    return false;
}

void UseToolView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell)
{
}

cocos2d::CCSize UseToolView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(1476, 285);
    }
    return CCSize(600, 160);
}

CCTableViewCell* UseToolView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx)
{
    if(idx >= m_curList.size()){
        return NULL;
    }
    
    OneToolCell* cell = (OneToolCell*)table->dequeueGrid();
    if(idx < m_curList.size()){
        if(cell){
            cell->setData(m_curList[idx], m_type, m_dict);
        }else{
            cell = OneToolCell::create(m_curList[idx], m_type, m_dict, m_infoList);
        }
    }
    return cell;
}

ssize_t UseToolView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    int num = ceil(m_curList.size() / numPerRow);
    return num;
}

unsigned int UseToolView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return numPerRow;
}

////

OneToolCell* OneToolCell::create(int itemId, string type, CCDictionary* dict, CCNode* listNode)
{
    auto ret = new OneToolCell();
    if (ret && ret->init(itemId, type, dict, listNode)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool OneToolCell::init(int itemId, string type, CCDictionary* dict, CCNode* listNode)
{
    bool ret = true;
    CCBLoadFile("UseToolCell",this,this);
    m_waitInterface = NULL;
    m_listNode = listNode;
    setData(itemId, type, dict);
    return ret;
}

void OneToolCell::setData(int itemId, string type, CCDictionary* dict)
{
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    m_itemId = itemId;
    m_type = type;
    m_dict = dict;
    ToolInfo& toolInfo = ToolController::getInstance()->getToolInfoById(itemId);
    
    m_nameLabel->setString(toolInfo.getName().c_str());
    m_desLabel->setString(_lang(toolInfo.des).c_str());
    m_numLabel->setString(CC_CMDITOA(toolInfo.getCNT()));
    m_price = toolInfo.price;
    
    m_picNode->removeAllChildren();
    CCCommonUtils::createGoodsIcon(toolInfo.itemId, m_picNode, CCSize(80, 80));
    
//    auto pic = CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(toolInfo.itemId)).c_str(),true,CCLoadSpriteType_GOODS);
//    if (pic) {
//        auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(toolInfo.color).c_str());
//        CCCommonUtils::setSpriteMaxSize(iconBg, 80);
//        m_picNode->addChild(iconBg);
//        
//        CCCommonUtils::setSpriteMaxSize(pic, 80);
//        m_picNode->addChild(pic);
//    }

    m_inBtnGoldNum->setString(CC_CMDITOA(m_price));
    CCCommonUtils::setButtonTitle(m_useBtn, _lang("102137").c_str());
    m_btnMsgLabel->setString(_lang("104906").c_str());
    
    if (toolInfo.getCNT()>0) {
        m_buyNode->setVisible(false);
        m_buyBtn->setEnabled(false);
        m_useBtn->setVisible(true);
        m_useBtn->setEnabled(true);
    }
    else {
        m_buyNode->setVisible(true);
        m_buyBtn->setEnabled(true);
        m_useBtn->setVisible(false);
        m_useBtn->setEnabled(false);
    }
}

void OneToolCell::onEnter() {
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(OneToolCell::refreashData), MSG_TOOL_CHANGE, NULL);
}

void OneToolCell::onExit() {
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TOOL_CHANGE);
    CCNode::onExit();
}

void OneToolCell::refreashData(CCObject* obj)
{
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
}

SEL_CCControlHandler OneToolCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickUseBtn", OneToolCell::onClickUseBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBuyBtn", OneToolCell::onClickBuyBtn);
    return NULL;
}

void OneToolCell::onClickUseBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if(m_waitInterface!=NULL){
        return;
    }
    onUseTool();
}
void OneToolCell::onYes1(CCObject* ccObj){
    int num = 1;
    if(ccObj){
        CCInteger* cInt = dynamic_cast<CCInteger*>(ccObj);
        if(cInt){
            num = cInt->getValue();
        }
    }
    m_itemBuyCount = num;
    m_waitInterface=GameController::getInstance()->showWaitInterface(m_listNode);
    m_waitInterface->setPosition(ccp(m_listNode->getContentSize().width/2, m_listNode->getContentSize().height/2));
    ToolController::getInstance()->buyTool(m_itemId, m_itemBuyCount, CCCallFunc::create(this, callfunc_selector(OneToolCell::onUseTool1)),0,true,false,"UseToolView");
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUY_CONFIRM_OK_WITHOUT_TWEEN);
}
void OneToolCell::onUseTool1(){
    if(this->getParent() == NULL){
        return;
    }
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    if(m_itemBuyCount>0){
        if (m_type == USE_TOOL_VIP_PIC || m_type == USE_TOOL_LOTTERY1 || m_type == USE_TOOL_LOTTERY2){
            m_waitInterface=GameController::getInstance()->showWaitInterface(m_listNode);
            m_waitInterface->setPosition(ccp(m_listNode->getContentSize().width/2, m_listNode->getContentSize().height/2));
            ToolController::getInstance()->useTool(m_itemId,m_itemBuyCount,true);
        }
    }
}
void OneToolCell::onYes()
{
    m_waitInterface=GameController::getInstance()->showWaitInterface(m_listNode);
    m_waitInterface->setPosition(ccp(m_listNode->getContentSize().width/2, m_listNode->getContentSize().height/2));
    ToolController::getInstance()->buyTool(m_itemId, 1, CCCallFunc::create(this, callfunc_selector(OneToolCell::onUseTool)),0,true,false,"UseToolView");
}

void OneToolCell::onUseTool()
{
    if(this->getParent() == NULL){
        return;
    }
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    ToolInfo& toolInfo = ToolController::getInstance()->getToolInfoById(m_itemId);
    m_dict->setObject(CCString::create(toolInfo.uuid), "goodsId");
    if (m_type == USE_TOOL_CITY_MOVE) {
        
        WorldMapView::instance()->afterCityMove(m_dict);
        ToolController::getInstance()->useTool(m_itemId);
        PopupViewController::getInstance()->goBackPopupView();
        return;
    }
    else if (m_type == USE_TOOL_MARCH_CANCEL ||m_type == USE_TOOL_MARCH_CANCEL_RALLY) {
        WorldMapView::instance()->afterMarchCancel(m_dict);
        ToolController::getInstance()->useTool(m_itemId);
        PopupViewController::getInstance()->goBackPopupView();
        return;
    }
    else if (m_type == USE_TOOL_CHANGE_NAME) {
        std::string nickName = m_dict->valueForKey("nickName")->getCString();
        ToolController::getInstance()->useItemChnageName(m_itemId, nickName);
        return;
    }
    else if (m_type == USE_TOOL_CHANGE_PIC) {
        std::string pic = m_dict->valueForKey("pic")->getCString();
        ToolController::getInstance()->useItemChnagePic(m_itemId, pic);
        PopupViewController::getInstance()->goBackPopupView();
        return;
    }else if (m_type == USE_TOOL_VIP_ACTIVITY) {
        m_waitInterface=GameController::getInstance()->showWaitInterface(m_listNode);
        m_waitInterface->setPosition(ccp(m_listNode->getContentSize().width/2, m_listNode->getContentSize().height/2));
        ToolController::getInstance()->useTool(m_itemId,1,true);
        return;
    }else if (m_type == USE_TOOL_MATE_BOX) {
        m_waitInterface=GameController::getInstance()->showWaitInterface(m_listNode);
        m_waitInterface->setPosition(ccp(m_listNode->getContentSize().width/2, m_listNode->getContentSize().height/2));
        ToolController::getInstance()->useTool(m_itemId,1,true);
        return;
    }else if (m_type == USE_TOOL_VIP_PIC || m_type == USE_TOOL_LOTTERY1 || m_type == USE_TOOL_LOTTERY2) {
        
        if(toolInfo.getCNT()==1){
            m_waitInterface=GameController::getInstance()->showWaitInterface(m_listNode);
            m_waitInterface->setPosition(ccp(m_listNode->getContentSize().width/2, m_listNode->getContentSize().height/2));
            ToolController::getInstance()->useTool(m_itemId,1,true);
        }else{
            PopupViewController::getInstance()->addPopupView(ToolNumSelectView::create(m_itemId), false);
        }
        return;
    }/*else if (m_type == USE_TOOL_LOTTERY2) {
        m_waitInterface=GameController::getInstance()->showWaitInterface(m_listNode);
        m_waitInterface->setPosition(ccp(m_listNode->getContentSize().width/2, m_listNode->getContentSize().height/2));
        ToolController::getInstance()->useTool(m_itemId,1,true);
        return;
    }*/
    else {
        return ;
    }
}

void OneToolCell::onClickBuyBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if(m_waitInterface!=NULL){
        return;
    }
    if (m_type == USE_TOOL_MARCH_CANCEL||m_type == USE_TOOL_MARCH_CANCEL_RALLY) {
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);//200004
        if (info.getCNT()>0) {
            onYes();
        }else{
            YesNoDialog::showButtonAndGold( _lang("104919").c_str() , CCCallFunc::create(this, callfunc_selector(OneToolCell::onYes)), _lang("104906").c_str(), info.price);
        }

    }
//    else if (m_type == USE_TOOL_MARCH_CANCEL_RALLY)//高级行军召回
//    {
//        ToolInfo& info = ToolController::getInstance()->getToolInfoById(200006);
//        if (info.getCNT()>0) {
//            onYes();
//        }else{
//            YesNoDialog::showButtonAndGold( _lang("104919").c_str() , CCCallFunc::create(this, callfunc_selector(OneToolCell::onYes)), _lang("104906").c_str(), info.price);
//        }
//    }
    else if (m_type == USE_TOOL_VIP_PIC || m_type == USE_TOOL_LOTTERY1 || m_type == USE_TOOL_LOTTERY2)
    {
        ToolInfo& info1 = ToolController::getInstance()->getToolInfoById(m_itemId);
//        YesNoDialog::showButtonAndGold( _lang("104919").c_str() , CCCallFunc::create(this, callfunc_selector(OneToolCell::onYes)), _lang("104906").c_str(), info1.price);
        CCPoint p(0, 0);
        p = m_picNode->convertToWorldSpace(ccp(-55,-55));
        StoreBuyConfirmDialog* dialog = StoreBuyConfirmDialog::show(CCCommonUtils::getIcon(CC_ITOA(info1.itemId)).c_str(), info1.getName().c_str(), _lang(info1.des).c_str(), info1.price,info1.color, CCCallFuncO::create(this, callfuncO_selector(OneToolCell::onYes1), NULL), p, Gold);
        if (dialog){
            int maxNum = floor(GlobalData::shared()->playerInfo.gold /info1.price);
            maxNum = MAX(1, maxNum);
            dialog->showSliderBar(maxNum);
        }

    }
    else if(m_type == USE_TOOL_VIP_ACTIVITY)
    {
        ToolInfo& info1 = ToolController::getInstance()->getToolInfoById(m_itemId);
        YesNoDialog::showButtonAndGold( _lang("104919").c_str() , CCCallFunc::create(this, callfunc_selector(OneToolCell::onYes)), _lang("104906").c_str(), info1.price);
    }
    else{
        onYes();
    }
}

bool OneToolCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_useBtn", CCControlButton*, this->m_useBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, m_numLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyBtn", CCControlButton*, this->m_buyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyNode", CCNode*, m_buyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnMsgLabel", CCLabelIF*, m_btnMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inBtnGoldNum", CCLabelIF*, m_inBtnGoldNum);
    return false;
}