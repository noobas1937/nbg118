//
//  EquipmentBagView.cpp
//  IF
//
//  Created by fubin on 14-12-9.
//
//

#include "EquipmentBagView.h"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "RollingText.h"
#include "YesNoDialog.h"
#include "FunBuildController.h"
#include "QueueController.h"
//#include "ToolController.h"
//#include "EquipmentInfo.h"
#include "EquipmentController.h"
#include "HelpshiftCocos2dx.h"
#include "SoundController.h"
#include "ToolCreateView.h"

#include "SceneContainer.h"
#include "ImperialScene.h"
#include "StoreView.h"

const int advertiseCellW = 540;
const int advertiseCellW_HD = 1480;
static string BagViewItemId = "";
static bool ForceRefresh = false;

#define BROKEN_EQUIP_SPLIT "_"
#define rp_color {0,255,255}

#pragma mark -
#pragma mark - 材料、装备储藏箱
EquipmentBagView* EquipmentBagView::create(bool isFromWorkshop){
    EquipmentBagView* ret = new EquipmentBagView();
    if(ret && ret->init(isFromWorkshop)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipmentBagView::init(bool isFromWorkshop)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    m_isFromWorkshop=isFromWorkshop;
    CCLoadSprite::doResourceByCommonIndex(11, true);
    CCLoadSprite::doResourceByCommonIndex(306, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::doResourceByCommonIndex(306, false);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_EQUIP);
    });
    setIsHDPanel(true);
    auto tmpCCB = CCBLoadFile("EquipmentBagView",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else
        this->setContentSize(tmpCCB->getContentSize());
    
    setTitleName(_lang("102160").c_str());
    
    if (CCCommonUtils::isIosAndroidPad()) {
        auto pic = CCLoadSprite::createSprite("Hutch_wall.png");
        auto tBatchNode = CCSpriteBatchNode::createWithTexture(pic->getTexture());
        m_BGNode->addChild(tBatchNode);
        
        int bgcount = (m_infoList->getContentSize().height)/81+2;
        for (int i = 0; i<=bgcount; i++) {
            auto pic = CCLoadSprite::createSprite("Hutch_wall.png");
            pic->setScaleX(2.4);
            pic->setAnchorPoint(CCPoint(0, 0));
            pic->setPosition(ccp(0, -i*81-11));
            tBatchNode->addChild(pic);
            
            auto pic2 = CCLoadSprite::createSprite("Hutch_wall.png");
            pic2->setScaleX(2.4);
            pic2->setAnchorPoint(CCPoint(0, 0));
            pic2->setPosition(ccp(1356, -i*81-11));
            tBatchNode->addChild(pic2);
        }
    }
    else {
        changeBGHeight(m_buildBG);
        int extH = getExtendHeight();
        m_infoList->setContentSize(CCSizeMake(m_infoList->getContentSize().width, m_infoList->getContentSize().height+extH));
        m_bg2->setContentSize(CCSizeMake(m_bg2->getContentSize().width, m_bg2->getContentSize().height+extH));
        auto pic = CCLoadSprite::createSprite("Hutch_wall.png");
        auto tBatchNode = CCSpriteBatchNode::createWithTexture(pic->getTexture());
        m_BGNode->addChild(tBatchNode);
        
        int bgcount = (m_infoList->getContentSize().height+extH)/81+2;
        for (int i = 0; i<=bgcount; i++) {
            auto pic = CCLoadSprite::createSprite("Hutch_wall.png");
            pic->setPosition(ccp(0, -i*81-11));
            tBatchNode->addChild(pic);
            
            auto pic2 = CCLoadSprite::createSprite("Hutch_wall.png");
            pic2->setPosition(ccp(619, -i*81-11));
            tBatchNode->addChild(pic2);
        }
    }
    
    if (CCCommonUtils::isIosAndroidPad()) {
        m_equiBtnTxt->setVisible(true);
        m_mateBtnTxt->setVisible(true);
        m_mateBtnTxt->setString(_lang("119026"));
        m_equiBtnTxt->setString(_lang("119027"));
    }
    else {
        m_equiBtnTxt->setVisible(false);
        m_mateBtnTxt->setVisible(false);
        CCCommonUtils::setButtonTitle(m_mateBtn, _lang("119026").c_str());
        CCCommonUtils::setButtonTitle(m_equiBtn, _lang("119027").c_str());
    }
    
    m_data = CCArray::create();
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    m_infoList->addChild(m_tabView);
    BagViewItemId = "";
    m_page = 1;
    
    m_equiBtn->setEnabled(true);
    m_mateBtn->setEnabled(false);
    
    for (int j=1; j<=4; j++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",j)->getCString());
        m_fireNode1->addChild(particle);
        
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",j)->getCString());
        m_fireNode2->addChild(particle1);
    }
    m_openNum = 0;
    
    return true;
}

void EquipmentBagView::onEnter(){
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    UIComponent::getInstance()->showEquipOrBagBtn(2);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);//
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipmentBagView::changeMateCnt), MSG_MATE_CRT_END, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipmentBagView::refreashData), EQUIP_BAG_SHOW_DES, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipmentBagView::setFromWorkshop), "setFromWorkshop", NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipmentBagView::setRefreshView), EQUIP_CLOSE_VIEW, NULL);
    if (m_openNum==0) {
        refreshView();
    }else {
        refreashData(NULL);
    }
    
    setTitleName(_lang("119007"));
    m_openNum++;
}

void EquipmentBagView::onExit(){
    UIComponent::getInstance()->showEquipOrBagBtn(0);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MATE_CRT_END);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, EQUIP_BAG_SHOW_DES);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "setFromWorkshop");
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, EQUIP_CLOSE_VIEW);
    setTouchEnabled(false);
    if(m_isFromWorkshop){//如果从材料工坊进入，退出时返回材料工坊
//        PopupViewController::getInstance()->addPopupView(ToolCreateView::create());
    }
    CCNode::onExit();
}

void EquipmentBagView::setRefreshView(CCObject *obj){
    refreshView();
}

void EquipmentBagView::setFromWorkshop(CCObject *obj){
    int s=dynamic_cast<CCInteger*>(obj)->getValue();
    if (s>0) {
        m_isFromWorkshop=true;
    }else{
        m_isFromWorkshop=false;
    }
}

bool sortTmpList(string str1, string str2)
{
    int num1 = atoi(str1.c_str());
    int num2 = atoi(str2.c_str());
    if (num1 > num2) {
        return true;
    }else {
        return false;
    }
}

bool sortEquipList1(int itemId1, int itemId2)
{
    auto& eInfo1 = EquipmentController::getInstance()->EquipmentInfoMap[itemId1];
    auto& eInfo2 = EquipmentController::getInstance()->EquipmentInfoMap[itemId2];
    if (eInfo1.level>eInfo2.level) {
        return true;
    }else if (eInfo1.level<eInfo2.level) {
        return false;
    }else {
        if (eInfo1.color>eInfo2.color) {
            return true;
        }else {
            return false;
        }
    }
}

bool sortEquipList(string str1, string str2)
{
    int itemId1 = EquipmentController::getInstance()->m_MyEquipmentMap[str1];
    int itemId2 = EquipmentController::getInstance()->m_MyEquipmentMap[str2];
    return sortEquipList1(itemId1,itemId2);
}
bool sortEquipList2(string str1, string str2)
{
    vector<string> vect1;
    CCCommonUtils::splitString(str1, BROKEN_EQUIP_SPLIT, vect1);
    vector<string> vect2;
    CCCommonUtils::splitString(str2, BROKEN_EQUIP_SPLIT, vect2);
    return sortEquipList1(atoi(vect1.at(0).c_str()),atoi(vect2.at(0).c_str()));
}

void EquipmentBagView::generateData()
{
    m_data->removeAllObjects();
    vector<string> m_curList;
    
    if (m_page == 0) {
        map<int, vector<string>> tmpEquipMap1;
        map<int, vector<string>> tmpEquipMap2;
        map<int,int> tmpBrokenMap;
        map<string, int>::iterator it = EquipmentController::getInstance()->m_MyEquipmentMap.begin();
        for (; it!=EquipmentController::getInstance()->m_MyEquipmentMap.end(); it++)
        {
            auto& tmpInfo = EquipmentController::getInstance()->EquipmentInfoMap[it->second];
            if(tmpInfo.showType == "no")
            {
                tmpEquipMap1[tmpInfo.site].push_back(it->first);
                int newCount = tmpBrokenMap[it->second] + 1;
                tmpBrokenMap[it->second] = newCount;
            }
            else
            {
                tmpEquipMap2[tmpInfo.site].push_back(it->first);
            }
        }
        
        auto tmpItr = tmpBrokenMap.begin();
        while (tmpItr!=tmpBrokenMap.end()) {
            std::string tmpStr = CC_ITOA((*tmpItr).first) + std::string(BROKEN_EQUIP_SPLIT) + CC_ITOA((*tmpItr).second);
            auto tmpItr1 = tmpBrokenMap.find((*tmpItr).first+1);
            if(tmpItr1!=tmpBrokenMap.end()){
                tmpStr = tmpStr + BROKEN_EQUIP_SPLIT +(CC_ITOA((*tmpItr1).second));
            }else{
                tmpStr = tmpStr + BROKEN_EQUIP_SPLIT + "0";
            }
            m_curList.push_back(tmpStr);
            ++tmpItr;
        }
        sort(m_curList.begin(), m_curList.end(),sortEquipList2);
        
//        // 胚子
//        for (int i=0; i<=7; i++)
//        {
//            if (tmpEquipMap1.find(i) != tmpEquipMap1.end()) {
//                sort(tmpEquipMap1[i].begin(), tmpEquipMap1[i].end(), sortEquipList);
//                m_curList.insert(m_curList.end(), tmpEquipMap1[i].begin(), tmpEquipMap1[i].end());
//            }
//        }
        
        
        // 装备
        for (int i=0; i<=7; i++)
        {
            string OnEquipKey = EquipmentController::getInstance()->FindOnEquipBySite(i);
            if (OnEquipKey!="") {
                m_curList.push_back(OnEquipKey);
            }
            
            if (tmpEquipMap2.find(i) != tmpEquipMap2.end()) {
                sort(tmpEquipMap2[i].begin(), tmpEquipMap2[i].end(), sortEquipList);
                m_curList.insert(m_curList.end(), tmpEquipMap2[i].begin(), tmpEquipMap2[i].end());
            }
        }
        
    }
    else {
        for (int i=0; i<ToolController::getInstance()->m_mateTools.size(); i++) {
            int tmpToolId = ToolController::getInstance()->m_mateTools[i];
            ToolInfo& info = ToolController::getInstance()->getToolInfoById(tmpToolId);
            if (info.getCNT()>0){
                m_curList.push_back(CC_ITOA(tmpToolId));
            }
        }
        sort(m_curList.begin(), m_curList.end(), sortTmpList);
    }
    
    string itemIds = "";
    int count = m_curList.size();
    if (CCCommonUtils::isIosAndroidPad()) {
        for (int i=0; i<count; i++)
        {//4个道具 分为一组
            if(i%5==0||i%5==1||i%5==2||i%5==3){
                itemIds.append(m_curList[i]);
                if (i!=(count-1)) {
                    itemIds.append(":");
                }
                else {
                    m_data->addObject(CCString::create(itemIds));
                    itemIds = "";
                }
            }else{
                itemIds += m_curList[i];
                m_data->addObject(CCString::create(itemIds));
                itemIds = "";
            }
        }
        
        int showCount =m_data->count();
        if(showCount<10)
        {//显示空格子 补齐数据
            showCount = 10-showCount;
            for (int j=0; j<showCount; j++) {
                m_data->addObject(CCString::create("-1:-1:-1:-1"));
            }
        }
    }
    else {
        for (int i=0; i<count; i++)
        {//4个道具 分为一组
            if(i%4==0||i%4==1||i%4==2){
                itemIds.append(m_curList[i]);
                if (i!=(count-1)) {
                    itemIds.append(":");
                }
                else {
                    m_data->addObject(CCString::create(itemIds));
                    itemIds = "";
                }
            }else{
                itemIds += m_curList[i];
                m_data->addObject(CCString::create(itemIds));
                itemIds = "";
            }
        }
        
        int showCount =m_data->count();
        if(showCount<10)
        {//显示空格子 补齐数据
            showCount = 10-showCount;
            for (int j=0; j<showCount; j++) {
                m_data->addObject(CCString::create("-1:-1:-1:-1"));
            }
        }
    }
}

void EquipmentBagView::changeMateCnt(CCObject* obj)
{
    ForceRefresh = true;
    refreashData(obj);
    ForceRefresh = false;
}

void EquipmentBagView::refreashData(CCObject* obj){
    float miny = m_tabView->minContainerOffset().y;
    CCPoint pos  = m_tabView->getContentOffset();
    generateData();
    m_tabView->reloadData();
    float mincurry = m_tabView->minContainerOffset().y;
    pos.y+=(-miny+mincurry);
    m_tabView->setContentOffset(pos);
}

void EquipmentBagView::refreshView(){
    generateData();
    m_tabView->reloadData();
}

SEL_CCControlHandler EquipmentBagView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickMateBtn", EquipmentBagView::onClickMateBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickEquiBtn", EquipmentBagView::onClickEquiBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickHelpBtn", EquipmentBagView::onClickHelpBtn);
    return NULL;
}

bool EquipmentBagView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_BGNode", CCNode*, this->m_BGNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg2", CCScale9Sprite*, this->m_bg2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateBtn", CCControlButton*, this->m_mateBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equiBtn", CCControlButton*, this->m_equiBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpBtn", CCControlButton*, this->m_helpBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateBtnTxt", CCLabelIF*, this->m_mateBtnTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equiBtnTxt", CCLabelIF*, this->m_equiBtnTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode1", CCNode*, this->m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode2", CCNode*, this->m_fireNode2);
    return false;
}

void EquipmentBagView::onClickMateBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    m_equiBtn->setEnabled(true);
    m_mateBtn->setEnabled(false);
    m_page = 1;
    refreshView();
}

void EquipmentBagView::onClickEquiBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    m_equiBtn->setEnabled(false);
    m_mateBtn->setEnabled(true);
    m_page = 0;
    refreshView();
}

void EquipmentBagView::onClickHelpBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    GlobalData::shared()->isBind = true;
#endif
    CCCommonUtils::showHelpShiftSingleFAQ("45213");
//    CCCommonUtils::showHelpShiftFAQ("45202");
//    HelpshiftCocos2dx::showFAQSection("467");
}

bool EquipmentBagView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_startPoint = pTouch->getStartLocation();
    if(isTouchInside(m_infoList, pTouch)){
        return false;
    }
    return true;
}

void EquipmentBagView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint endPoint = pTouch->getLocation();
    if(fabs(endPoint.x-m_startPoint.x)>10||fabs(endPoint.y-m_startPoint.y)>10){
        return;
    }
    if(BagViewItemId!=""){
        BagViewItemId="";
        refreashData(NULL);
    }
}

CCSize EquipmentBagView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return CCSizeZero;
    }
    
    if (CCCommonUtils::isIosAndroidPad()) {
        if(BagViewItemId==""){
            return CCSize(advertiseCellW_HD, 310);
        }
        string itemId = dynamic_cast<CCString*>(m_data->objectAtIndex(idx))->getCString();
        if(itemId!=""){
            vector<string> vector1;
            CCCommonUtils::splitString(itemId, ":", vector1);
            int i= 0;
            while (i<vector1.size()) {
                if(vector1[i]!=""){
                    if(vector1[i]==BagViewItemId){
                        //                    return CCSize(advertiseCellW, 366);
                        return CCSize(advertiseCellW_HD, 310);
                    }
                }
                i++;
            }
        }
        return CCSize(advertiseCellW_HD, 310);
    }
    else {
        if(BagViewItemId==""){
            return CCSize(advertiseCellW, 180);
        }
        string itemId = dynamic_cast<CCString*>(m_data->objectAtIndex(idx))->getCString();
        if(itemId!=""){
            vector<string> vector1;
            CCCommonUtils::splitString(itemId, ":", vector1);
            int i= 0;
            while (i<vector1.size()) {
                if(vector1[i]!=""){
                    if(vector1[i]==BagViewItemId){
                        //                    return CCSize(advertiseCellW, 366);
                        return CCSize(advertiseCellW, 180);
                    }
                }
                i++;
            }
        }
        return CCSize(advertiseCellW, 180);
    }
}

CCSize EquipmentBagView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        CCSize(advertiseCellW_HD, 310);
    }
    return CCSize(advertiseCellW, 360);
}

CCTableViewCell* EquipmentBagView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    
    string itemId = dynamic_cast<CCString*>(m_data->objectAtIndex(idx))->getCString();
    EquipmentBagCell* cell = (EquipmentBagCell*)table->dequeueCell();
    if(cell){
        cell->setData(itemId, m_page);
    }else{
        cell = EquipmentBagCell::create(itemId, m_page, m_infoList);
    }
    return cell;
}

ssize_t EquipmentBagView::numberOfCellsInTableView(CCTableView *table)
{
    int num = ceil(m_data->count() / 1.0f);
    return num;
}

//class cell
#pragma mark -
#pragma mark - 储藏箱Cell（材料和装备用的同一个）

EquipmentBagCell* EquipmentBagCell::create(string itemIds, int page, CCNode* touchNode)
{
    auto ret = new EquipmentBagCell();
    if (ret && ret->init(itemIds, page, touchNode)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipmentBagCell::init(string itemIds, int page, CCNode* touchNode)
{
    bool ret = true;
    CCLoadSprite::doResourceByCommonIndex(11, true);
    CCLoadSprite::doResourceByCommonIndex(306, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::doResourceByCommonIndex(306, false);
    });
    
    CCBLoadFile("EquipmentBagCell",this,this);
    CCCommonUtils::setButtonTitle(m_crtBtn, _lang("102137").c_str());
    m_selIndex = -99;
    m_waitInterface = NULL;
    m_touchNode = touchNode;
    m_page = page;//0 装备， 1 材料
    setData(itemIds, page);
    return ret;
}

void EquipmentBagCell::setData(string itemIds, int page)
{
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    m_selIndex=-99;
    m_items = itemIds;
    m_page = page;
    vector<string> tmpVec;
    CCCommonUtils::splitString(m_items, ":", tmpVec);
    m_idsVec.clear();
    if (CCCommonUtils::isIosAndroidPad()) {
        for (int i=0; i<5; i++) {
            if (i<tmpVec.size()) {
                m_idsVec.push_back(tmpVec[i]);
            }
            else {
                m_idsVec.push_back("-1");
            }
        }
        
        bool isDes = false;
        for (int j=0; j<5; j++) {
            if(m_idsVec[j]==BagViewItemId){
                //            isDes = true;
                m_selIndex = j;
                break;
            }
        }
        if(isDes){
            this->setContentSize(CCSize(1480, 620));
        }else{
            this->setContentSize(CCSize(1480, 310));
        }
    }
    else {
        for (int i=0; i<4; i++) {
            if (i<tmpVec.size()) {
                m_idsVec.push_back(tmpVec[i]);
            }
            else {
                m_idsVec.push_back("-1");
            }
        }
        
        bool isDes = false;
        for (int j=0; j<4; j++) {
            if(m_idsVec[j]==BagViewItemId){
                //            isDes = true;
                m_selIndex = j;
                break;
            }
        }
        if(isDes){
            this->setContentSize(CCSize(606, 360));
        }else{
            this->setContentSize(CCSize(606, 180));
        }
    }
    
    m_picNode1->removeAllChildren();
    m_picNode2->removeAllChildren();
    m_picNode3->removeAllChildren();
    m_picNode4->removeAllChildren();
    m_numLabel1->setString("");
    m_numLabel2->setString("");
    m_numLabel3->setString("");
    m_numLabel4->setString("");
    m_itemNode1->setVisible(true);
    m_itemNode2->setVisible(true);
    m_itemNode3->setVisible(true);
    m_itemNode4->setVisible(true);
    m_wor1->setVisible(false);
    m_wor2->setVisible(false);
    m_wor3->setVisible(false);
    m_wor4->setVisible(false);
    m_barBG1->setVisible(false);
    m_barBG2->setVisible(false);
    m_barBG3->setVisible(false);
    m_barBG4->setVisible(false);
    m_yes1Spr->setVisible(false);
    m_yes2Spr->setVisible(false);
    m_yes3Spr->setVisible(false);
    m_yes4Spr->setVisible(false);
    if (CCCommonUtils::isIosAndroidPad()) {
        m_picNode5->removeAllChildren();
        m_numLabel5->setString("");
        m_itemNode5->setVisible(true);
        m_wor5->setVisible(false);
        m_barBG5->setVisible(false);
        m_yes5Spr->setVisible(false);
    }
    if (m_page==1) {
        m_barBG1->setVisible(true);
        m_barBG2->setVisible(true);
        m_barBG3->setVisible(true);
        m_barBG4->setVisible(true);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_barBG5->setVisible(true);
        }
    }
    
    //显示4个单元
    for (int m=0; m<m_idsVec.size(); m++)
    {
        string picPath = "";
        string bgPath = "";
        string num = "";
        int cnt = 0;
        if (m_idsVec[m] != "-1")
        {
            int color = WHITE;
            CCLoadSpriteType equip = CCLoadSpriteType_GOODS;
            if (m_page==1) {
                ToolInfo& info = ToolController::getInstance()->getToolInfoById(atoi(m_idsVec[m].c_str()));
                cnt = info.getCNT();
                num = CC_CMDITOA(cnt);
                picPath = CCCommonUtils::getIcon(CC_ITOA(info.itemId));
                bgPath = CCCommonUtils::getToolBgByColor(info.color);
                color = info.color;
            }
            else {
                int equipId = 0;
                
                if(m_idsVec[m].find(BROKEN_EQUIP_SPLIT)!=string::npos){
                    vector<string> mVec;
                    CCCommonUtils::splitString(m_idsVec[m], BROKEN_EQUIP_SPLIT, mVec);
                    if(mVec.size()>1){
                        equipId = atoi(mVec.at(0).c_str());
                        cnt = atoi(mVec.at(1).c_str());
                        num = CC_CMDITOA(cnt);
                    }
                }else{
                    if (EquipmentController::getInstance()->m_MyOnEquip.find(m_idsVec[m]) != EquipmentController::getInstance()->m_MyOnEquip.end()) {
                        equipId = EquipmentController::getInstance()->m_MyOnEquip[m_idsVec[m]];
                        if (m==0) {
                            m_yes1Spr->setVisible(true);
                        } else if (m==1) {
                            m_yes2Spr->setVisible(true);
                        } else if (m==2) {
                            m_yes3Spr->setVisible(true);
                        } else if (m==3) {
                            m_yes4Spr->setVisible(true);
                        }
                        if (CCCommonUtils::isIosAndroidPad() && m==4) {
                            m_yes5Spr->setVisible(true);
                        }
                    }else {
                        equipId = EquipmentController::getInstance()->m_MyEquipmentMap[m_idsVec[m]];
                    }
                }
                equip=CCLoadSpriteType_EQUIP;
                EquipmentInfo& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[equipId];
                picPath = CCCommonUtils::getIcon(CC_ITOA(eInfo.itemId));
                bgPath = CCCommonUtils::getToolBgByColor(eInfo.color);
                color = eInfo.color;
            }
            
            auto iconBg = CCLoadSprite::createSprite(bgPath.c_str());
            auto pic = CCLoadSprite::createSprite(picPath.c_str(),true,equip);
            if (CCCommonUtils::isIosAndroidPad()) {
                CCCommonUtils::setSpriteMaxSize(iconBg, 190,true);
                CCCommonUtils::setSpriteMaxSize(pic, 180,true);
            }
            else {
                CCCommonUtils::setSpriteMaxSize(iconBg, 127,true);
                CCCommonUtils::setSpriteMaxSize(pic, 120,true);
            }
            
            
            if (m==0) {
                m_numLabel1->setString(num);
                m_picNode1->addChild(iconBg);
                m_picNode1->addChild(pic);
                if (/*m_page==1 &&*/ cnt>=4 && color!=GOLDEN){
                    m_wor1->setVisible(true);
                }
                if(cnt>0){
                    m_barBG1->setVisible(true);
                }
            } else if (m==1) {
                m_numLabel2->setString(num);
                m_picNode2->addChild(iconBg);
                m_picNode2->addChild(pic);
                if (/*m_page==1 &&*/ cnt>=4 && color!=GOLDEN) {
                    m_wor2->setVisible(true);
                }
                if(cnt>0){
                    m_barBG2->setVisible(true);
                }
            } else if (m==2) {
                m_numLabel3->setString(num);
                m_picNode3->addChild(iconBg);
                m_picNode3->addChild(pic);
                if (/*m_page==1 &&*/ cnt>=4 && color!=GOLDEN) {
                    m_wor3->setVisible(true);
                }
                if(cnt>0){
                    m_barBG3->setVisible(true);
                }
            } else if (m==3) {
                m_numLabel4->setString(num);
                m_picNode4->addChild(iconBg);
                m_picNode4->addChild(pic);
                if (/*m_page==1 &&*/ cnt>=4 && color!=GOLDEN) {
                    m_wor4->setVisible(true);
                }
                if(cnt>0){
                    m_barBG4->setVisible(true);
                }
            } else if (CCCommonUtils::isIosAndroidPad() && m==4) {
                m_numLabel5->setString(num);
                m_picNode5->addChild(iconBg);
                m_picNode5->addChild(pic);
                if (/*m_page==1 &&*/ cnt>=4 && color!=GOLDEN) {
                    m_wor5->setVisible(true);
                }
                if(cnt>0){
                    m_barBG5->setVisible(true);
                }
            }
        }
        else
        {
            if (m==0) {
                this->m_itemNode1->setVisible(false);
            } else if (m==1) {
                this->m_itemNode2->setVisible(false);
            } else if (m==2) {
                this->m_itemNode3->setVisible(false);
            } else if (m==3) {
                this->m_itemNode4->setVisible(false);
            } else if (CCCommonUtils::isIosAndroidPad() && m==4){
                this->m_itemNode5->setVisible(false);
            }
        }
    }
    
    m_desNode->setVisible(false);
    if(m_selIndex!=-99){
//        m_desNode->setVisible(true);
//        m_selectSpr->setVisible(true);
//        m_itemsNode->setPositionY(206);
//        m_arrowSpr->setPositionX(93.2+m_selIndex*148);
//        m_selectSpr->setPositionX(85+145*m_selIndex);
        
        if (m_page==1) {
            ToolInfo& info = ToolController::getInstance()->getToolInfoById(atoi(m_idsVec[m_selIndex].c_str()));
            m_nameMateLabel->setString(info.getName().c_str());
//            picPath = CCCommonUtils::getIcon(CC_ITOA(info.itemId));
//            bgPath = CCCommonUtils::getToolBgByColor(info.color);
        }
        else {
            int equipId = 0;
            if(m_idsVec[m_selIndex].find(BROKEN_EQUIP_SPLIT)!=string::npos){
                vector<string> mVec;
                CCCommonUtils::splitString(m_idsVec[m_selIndex], BROKEN_EQUIP_SPLIT, mVec);
                if(mVec.size()>1){
                    equipId = atoi(mVec.at(0).c_str());
                }
            }else{
                if (EquipmentController::getInstance()->m_MyOnEquip.find(m_idsVec[m_selIndex]) != EquipmentController::getInstance()->m_MyOnEquip.end()) {
                    equipId = EquipmentController::getInstance()->m_MyOnEquip[m_idsVec[m_selIndex]];
                }else {
                    equipId = EquipmentController::getInstance()->m_MyEquipmentMap[m_idsVec[m_selIndex]];
                }
            }
            EquipmentInfo& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[equipId];
            m_nameLabel->setString(_lang(eInfo.name).c_str());
            m_desLabel->setString(_lang(eInfo.des).c_str());
        }
        
//        refreshItemData(m_selIndex, ForceRefresh);
        
    }else{
        if (!CCCommonUtils::isIosAndroidPad()) {
            m_itemsNode->setPositionY(20);
        }
        m_desNode->setVisible(false);
        m_selectSpr->setVisible(false);
    }
    
}

void EquipmentBagCell::onEnter()
{
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup_Item, false);
}

void EquipmentBagCell::onExit()
{
    setTouchEnabled(false);
    CCNode::onExit();
}

void EquipmentBagCell::refreashData(CCObject* obj)
{
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
    string numStr =  CC_CMDITOA(info.getCNT());
    if(m_selIndex==0){
        m_numLabel1->setString(numStr);
    }else if(m_selIndex==1){
        m_numLabel1->setString(numStr);
    }else if(m_selIndex==2){
        m_numLabel1->setString(numStr);
    }else if(m_selIndex==3){
        m_numLabel1->setString(numStr);
    }else if (CCCommonUtils::isIosAndroidPad() && m_selIndex==4){
        m_numLabel1->setString(numStr);
    }
    if(info.getCNT() <= 0){
        ToolController::getInstance()->m_currentUseItemId=0;
    }
}

bool EquipmentBagCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_clickInSide = false;
    m_startPoint = pTouch->getStartLocation();
    if (isTouchInside(m_touchNode,pTouch)){
        m_clickInSide = true;
        if(isTouchInside(this,pTouch)){
            return true;
        }
    }
    return false;
}

void EquipmentBagCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint endPoint = pTouch->getLocation();
    if(fabs(endPoint.x-m_startPoint.x)>10||fabs(endPoint.y-m_startPoint.y)>10){
        return;
    }
    
    if (isTouchInside(m_picBG1,pTouch)){
        if(m_idsVec[0]!="-1"){
            showDesByIndex(0);
        }
    }else if (isTouchInside(m_picBG2,pTouch)){
        if(m_idsVec[1]!="-1"){
            showDesByIndex(1);
        }
    }else if (isTouchInside(m_picBG3,pTouch)){
        if(m_idsVec[2]!="-1"){
            showDesByIndex(2);
        }
    }else if (isTouchInside(m_picBG4,pTouch)){
        if(m_idsVec[3]!="-1"){
            showDesByIndex(3);
        }
    }else if (CCCommonUtils::isIosAndroidPad() && isTouchInside(m_picBG5, pTouch)){
        if (m_idsVec[4]!="-1") {
            showDesByIndex(4);
        }
    }
}

void EquipmentBagCell::refreshItemData(int index, bool st)
{
    if(index==-1){
        BagViewItemId="";
    }else{
//        if(m_selIndex==index && !st){
//            return;
//        }
        m_selIndex= index;
        BagViewItemId = m_idsVec[m_selIndex];
        
        m_mateNode1->removeAllChildren();
        m_mateNode2->removeAllChildren();
        m_mateNode3->removeAllChildren();
        m_mateNode4->removeAllChildren();
        m_mateNode5->removeAllChildren();
        
        m_itemId = 0;
        if (m_page==1) {
            // 点击材料
//            PopupViewController::getInstance()->addPopupView(MaterialInfoView::create(atoi(m_idsVec[m_selIndex].c_str())));
            ToolInfo& info = ToolController::getInstance()->getToolInfoById(atoi(m_idsVec[m_selIndex].c_str()));
            if (info.color == GOLDEN) {
                CCCommonUtils::flyHint("", "", _lang("121987"));
                PopupViewController::getInstance()->addPopupView(MaterialInfoView::create(atoi(m_idsVec[m_selIndex].c_str())));
            } else {
                PopupViewController::getInstance()->addPopupInView(MateCreateView::create(info.itemId,0,0,0));
            }
        }
        else {
            // 点击装备
            if(m_idsVec[m_selIndex].find(BROKEN_EQUIP_SPLIT)!=string::npos){
                vector<string> vect;
                CCCommonUtils::splitString(m_idsVec[m_selIndex], BROKEN_EQUIP_SPLIT, vect);
                if(vect.size()>2){
                    PopupViewController::getInstance()->addPopupView(EquipInfoView::createWithBrokenEquip(atoi(vect.at(0).c_str()), atoi(vect.at(1).c_str()), atoi(vect.at(2).c_str())));
                }
            }else{
                int equipId = 0;
                if (EquipmentController::getInstance()->m_MyOnEquip.find(m_idsVec[m_selIndex]) != EquipmentController::getInstance()->m_MyOnEquip.end()) {
                    equipId = EquipmentController::getInstance()->m_MyOnEquip[m_idsVec[m_selIndex]];
                }else {
                    equipId = EquipmentController::getInstance()->m_MyEquipmentMap[m_idsVec[m_selIndex]];
                }
                
                PopupViewController::getInstance()->addPopupView(EquipInfoView::create(m_idsVec[m_selIndex]));
            }
        }
    }
}

void EquipmentBagCell::showDesByIndex(int index)
{
    refreshItemData(index);
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(EQUIP_BAG_SHOW_DES);
}

void EquipmentBagCell::onClickCrtBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if(!m_clickInSide) {
        return ;
    }
    EquipmentController::getInstance()->startMaterialCreate(m_itemId);
}

SEL_CCControlHandler EquipmentBagCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCrtBtn", EquipmentBagCell::onClickCrtBtn);
    return NULL;
}

bool EquipmentBagCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemsNode", CCNode*, m_itemsNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode1", CCNode*, m_itemNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode2", CCNode*, m_itemNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode3", CCNode*, m_itemNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode4", CCNode*, m_itemNode4);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode1", CCNode*, m_picNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode2", CCNode*, m_picNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode3", CCNode*, m_picNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode4", CCNode*, m_picNode4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel1", CCLabelIF*, m_numLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel2", CCLabelIF*, m_numLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel3", CCLabelIF*, m_numLabel3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel4", CCLabelIF*, m_numLabel4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBG1", CCSprite*, m_picBG1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBG2", CCSprite*, m_picBG2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBG3", CCSprite*, m_picBG3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBG4", CCSprite*, m_picBG4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barBG1", CCSprite*, m_barBG1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barBG2", CCSprite*, m_barBG2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barBG3", CCSprite*, m_barBG3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barBG4", CCSprite*, m_barBG4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectSpr", CCSprite*, m_selectSpr);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wor1", CCSprite*, m_wor1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wor2", CCSprite*, m_wor2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wor3", CCSprite*, m_wor3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wor4", CCSprite*, m_wor4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_yes1Spr", CCSprite*, m_yes1Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_yes2Spr", CCSprite*, m_yes2Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_yes3Spr", CCSprite*, m_yes3Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_yes4Spr", CCSprite*, m_yes4Spr);
    
    if (CCCommonUtils::isIosAndroidPad()) {
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode5", CCNode*, m_itemNode5);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode5", CCNode*, m_picNode5);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel5", CCLabelIF*, m_numLabel5);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBG5", CCSprite*, m_picBG5);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barBG5", CCSprite*, m_barBG5);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wor5", CCSprite*, m_wor5);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_yes5Spr", CCSprite*, m_yes5Spr);
    }
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode", CCNode*, m_desNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemBg", CCScale9Sprite*, m_itemBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrowSpr", CCSprite*, m_arrowSpr);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipNode", CCNode*, m_equipNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode", CCNode*, m_mateNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameMateLabel", CCLabelIF*, m_nameMateLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_crtBtn", CCControlButton*, m_crtBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode1", CCNode*, m_mateNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode2", CCNode*, m_mateNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode3", CCNode*, m_mateNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode4", CCNode*, m_mateNode4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode5", CCNode*, m_mateNode5);
    return false;
}


//class cell
#pragma mark -
#pragma mark - 材料合成面板
MateCreateView* MateCreateView::create(int itemId,int type,int count,int nextCount)
{
    auto ret = new MateCreateView();
    if (ret && ret->init(itemId,type,count,nextCount)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MateCreateView::init(int itemId,int type,int count,int nextCount)
{
    bool ret = true;
    if (!PopupBaseView::init()) {
        return false;
    }
    m_type = type;
    
    CCLoadSprite::doResourceByCommonIndex(207, true);
    CCLoadSprite::doResourceByCommonIndex(306, true);
    CCLoadSprite::doResourceByCommonIndex(305, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(207, false);
        CCLoadSprite::doResourceByCommonIndex(306, false);
        CCLoadSprite::doResourceByCommonIndex(305, false);
    });
    
    setIsHDPanel(true);
    auto tmpCCB = CCBLoadFile("MateCreateView",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else
        this->setContentSize(tmpCCB->getContentSize());
    CCCommonUtils::setButtonTitle(m_crtBtn, _lang("119028").c_str());
    
    if (!CCCommonUtils::isIosAndroidPad()) {
        int extH = getExtendHeight();
        if(extH>=284){
            m_midNode->setScale(1.05f);
        }
        if(extH<=10){
            m_midNode->setScale(0.9f);
        }
        m_midNode->setPositionY(m_midNode->getPositionY()-extH/2);
        m_bottomNode->setPositionY(m_bottomNode->getPositionY()-extH);
        m_mateNode->setPositionY(m_mateNode->getPositionY()-extH+5);
        m_leftBG->setContentSize(CCSizeMake(m_leftBG->getContentSize().width, m_leftBG->getContentSize().height+extH));
        m_rightBG->setContentSize(CCSizeMake(m_rightBG->getContentSize().width, m_rightBG->getContentSize().height+extH));

    }
    m_popLayer = CCLayer::create();
    setTitleName(_lang("119028"));
    setPrcNodeTag();
    m_waitInterface = NULL;
    m_needLabel->setString(_lang("119020"));
    m_needLabel->setVisible(false);

    setData(itemId,count,nextCount);
    setParticle();
    
    return ret;
}

void MateCreateView::setPrcNodeTag()
{
    m_mateNode1->setTag(1);
    m_mateNode2->setTag(2);
    m_mateNode3->setTag(3);
    m_mateNode4->setTag(4);
    m_mateNode5->setTag(5);
}

void MateCreateView::setParticle(){
    playCanCrtParticle();
    switch (m_type) {
        case 1:{
            auto &eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
            m_colorT = eInfo.color+1;
            break;
        }
        default:{
            ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
            m_colorT = info.color+1;
            break;
        }
    }
    if (m_colorT > GOLDEN) {
        m_colorT = GOLDEN;
    }
    
    if (m_colorT == GREEN) {
        tmpStartColorVec.push_back(ccc4f(0, 0.588235294, 0, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.588235294, 1, 0.392156863, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.647058824, 0.784313725, 0, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0, 0.196078431, 0, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.392156863, 0.784313725, 0, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.588235294, 1, 0.392156863, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.588235294, 1, 0.392156863, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.588235294, 1, 0.392156863, 0.392156863));
        
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0.196078431, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0.196078431, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0.196078431, 0, 0));
        tmpEndColorVec.push_back(ccc4f(1, 0.784313725, 0.392156863, 0.392156863));
        tmpEndColorVec.push_back(ccc4f(0, 0.196078431, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0.196078431, 0, 0));
    }else if (m_colorT == BLUE) {
        tmpStartColorVec.push_back(ccc4f(0.011764706, 0.011764706, 1, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.392156863, 0.784313725, 1, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.392156863, 0.784313725, 1, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0, 0, 0.294117647, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.392156863, 0.784313725, 1, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.098039216, 0.392156863, 1, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.392156863, 0.784313725, 1, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.392156863, 0.784313725, 1, 0.392156863));
        
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0.196078431, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0.196078431, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0.196078431, 0));
        tmpEndColorVec.push_back(ccc4f(1, 1, 1, 0.392156863));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0.196078431, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0.196078431, 0));
    }else if (m_colorT == PURPLE) {
        tmpStartColorVec.push_back(ccc4f(0.196078431, 0.156862745, 1, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.784313725, 0.588235294, 1, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.784313725, 0.588235294, 1, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.058823529, 0.039215686, 0.294117647, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.784313725, 0.588235294, 1, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.058823529, 0.137254902, 0.784313725, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.784313725, 0.588235294, 1, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.784313725, 0.588235294, 1, 0.392156863));
        
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0.196078431, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0.196078431, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0.117647059, 0.019607843, 0.254901961, 0));
        tmpEndColorVec.push_back(ccc4f(1, 1, 1, 0.392156863));
        tmpEndColorVec.push_back(ccc4f(0, 0.019607843, 0.196078431, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0.019607843, 0.196078431, 0));
    }else if (m_colorT == ORANGE) {
        tmpStartColorVec.push_back(ccc4f(1, 0.098039216, 0, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.784313725, 0.098039216, 0, 0.196078431));
        tmpStartColorVec.push_back(ccc4f(1, 0.588235294, 0.098039216, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.196078431, 0.019607843, 0, 0.294117647));
        tmpStartColorVec.push_back(ccc4f(1, 0.392156863, 0.117647059, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(1, 0.294117647, 0, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(1, 0.784313725, 0.392156863, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(1, 0.784313725, 0.392156863, 0.392156863));
        
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0.058823529, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0.058823529, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(1, 0.784313725, 0.392156863, 0.392156863));
        tmpEndColorVec.push_back(ccc4f(0.392156863, 0.019607843, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0.392156863, 0.019607843, 0, 0));
    }else if (m_colorT == GOLDEN) {
        tmpStartColorVec.push_back(ccc4f(1, 0.784313725, 0, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(1, 0.784313725, 0, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(1, 0.784313725, 0, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.196078431, 0.156862745, 0, 0.294117647));
        tmpStartColorVec.push_back(ccc4f(0.588235294, 0.392156863, 0, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.588235294, 0.392156863, 0, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(1, 0.784313725, 0, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(1, 0.784313725, 0, 0.392156863));
        
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(1, 0.882352941, 0.588235294, 0.392156863));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
    }
}

void MateCreateView::setData(int itemId,int count,int nextCount)
{
//    if(m_waitInterface!=NULL){
//        m_waitInterface->remove();
//        m_waitInterface = NULL;
//    }
    m_itemId = itemId;
    
    
    m_mateNode1->removeAllChildren();
    m_mateNode2->removeAllChildren();
    m_mateNode3->removeAllChildren();
    m_mateNode4->removeAllChildren();
    m_mateNode5->removeAllChildren();
    m_mateBgNode5->removeAllChildren();
    
    for (int i=0; i<6; i++) {
        m_selectSpr[i]->setVisible(false);
    }
    
    
    int color;
    int id;
    switch (m_type) {
        case 1:{
            equipCountMap.clear();
            map<string, int>::iterator eit = EquipmentController::getInstance()->m_MyEquipmentMap.begin();
            for (; eit!=EquipmentController::getInstance()->m_MyEquipmentMap.end(); eit++)
            {
                auto& tmpInfo = EquipmentController::getInstance()->EquipmentInfoMap[eit->second];
                if(tmpInfo.showType == "no")
                {
                    int newCount = equipCountMap[eit->second] + 1;
                    equipCountMap[eit->second] = newCount;
                }
                
            }
            
            auto eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
            color = eInfo.color;
            id = eInfo.itemId;
            m_itemCount = count;
            m_itemCount = equipCountMap[eInfo.itemId];
            m_itemNextCount = nextCount;
            m_itemNextCount = equipCountMap[eInfo.itemId+1];

            equipVec.clear();
            map<int, EquipmentInfo>::iterator it;
            for(it=EquipmentController::getInstance()->EquipmentInfoMap.begin();it!=EquipmentController::getInstance()->EquipmentInfoMap.end();++it){
                if (it->second.des==eInfo.des) {
                    equipVec.push_back(it->second);
                }
            }
            
            if (equipVec.size()>0 && equipVec.size()<=6) {
                for(int i=0;i<equipVec.size();i++){
                    EquipmentInfo& temInfo = equipVec[i];
                    string iconPath = CCCommonUtils::getIcon(CC_ITOA(temInfo.itemId));
                    string bgPath = CCCommonUtils::getToolBgByColor(temInfo.color);
                    auto icon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_GOODS);
                    CCCommonUtils::setSpriteMaxSize(icon, 80);
                    auto bg = CCLoadSprite::createSprite(bgPath.c_str());
                    CCCommonUtils::setSpriteMaxSize(bg, 80);
                    m_sprNode[i]->removeAllChildren();
                    m_sprNode[i]->addChild(bg);
                    m_sprNode[i]->addChild(icon);
                    m_num[i]->setString(CC_ITOA(equipCountMap[temInfo.itemId]));
                    if (temInfo.itemId==eInfo.itemId+1) {
                        m_selectSpr[i]->setVisible(true);
                    }
                }
            }
            
            break;
        }
        default:{
            ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
            color = info.color;
            id = info.itemId;
            m_itemCount = info.getCNT();
            ToolInfo& nextInfo = ToolController::getInstance()->getToolInfoById(m_itemId+1);
            m_itemNextCount =nextInfo.getCNT();
            
            toolVec.clear();
            map<int, ToolInfo>::iterator it;
            for(it=ToolController::getInstance()->m_toolInfos.begin();it!=ToolController::getInstance()->m_toolInfos.end();++it){
                if (it->second.des==info.des) {
                    toolVec.push_back(it->second);
                }
            }
            
            if (toolVec.size()>0 && toolVec.size()<=6) {
                for(int i=0;i<toolVec.size();i++){
                    ToolInfo& temInfo = toolVec[i];
                    string iconPath = CCCommonUtils::getIcon(CC_ITOA(temInfo.itemId));
                    string bgPath = CCCommonUtils::getToolBgByColor(temInfo.color);
                    auto icon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_GOODS);
                    CCCommonUtils::setSpriteMaxSize(icon, 80);
                    auto bg = CCLoadSprite::createSprite(bgPath.c_str());
                    CCCommonUtils::setSpriteMaxSize(bg, 80);
                    m_sprNode[i]->removeAllChildren();
                    m_sprNode[i]->addChild(bg);
                    m_sprNode[i]->addChild(icon);
                    m_num[i]->setString(CC_ITOA(temInfo.getCNT()));
                    if (temInfo.itemId==info.itemId+1) {
                        m_selectSpr[i]->setVisible(true);
                    }
                }
            }
            break;
        }
    }
    
//    m_infoLabel->setString(_lang(info.des));
    
    if (color < GOLDEN) {
        string items = "";
//        if(BagViewItemId.find(BROKEN_EQUIP_SPLIT)!=string::npos){
//            vector<string> vect;
//            CCCommonUtils::splitString(BagViewItemId, BROKEN_EQUIP_SPLIT, vect);
//            if(vect.size()>0){
//                items = vect.at(0);
//            }
//        }else{
//            items = BagViewItemId;
//        }
        items = CC_ITOA(id);
        
        auto addSpr1 = CCLoadSprite::createSprite("Forging_jiahao.png");
        m_mateNode1->addChild(addSpr1);
        place1=false;
        auto addSpr2 = CCLoadSprite::createSprite("Forging_jiahao.png");
        m_mateNode2->addChild(addSpr2);
        place2=false;
        auto addSpr3 = CCLoadSprite::createSprite("Forging_jiahao.png");
        m_mateNode3->addChild(addSpr3);
        place3=false;
        auto addSpr4 = CCLoadSprite::createSprite("Forging_jiahao.png");
        m_mateNode4->addChild(addSpr4);
        place4=false;
        
        string iconPath = CCCommonUtils::getIcon(items);
        string bgPath = CCCommonUtils::getToolBgByColor(color);
        string mateName = CCCommonUtils::getNameById(items);
        for (int i=0; i<m_itemCount; i++) {
            auto icon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_GOODS);
            CCCommonUtils::setSpriteMaxSize(icon, 100);
            auto bg = CCLoadSprite::createSprite(bgPath.c_str());
            CCCommonUtils::setSpriteMaxSize(bg, 100);
            
            if (i==0) {
                m_mateNode1->removeAllChildren();
                m_mateNode1->addChild(bg);
                m_mateNode1->addChild(icon);
                m_mateNode1->addChild(m_mateName1);
                m_mateName1->setString(mateName);
                m_mateName1->setColor(getColor(color));
                place1=true;
            }else if (i==1) {
                m_mateNode2->removeAllChildren();
                m_mateNode2->addChild(bg);
                m_mateNode2->addChild(icon);
                m_mateNode2->addChild(m_mateName2);
                m_mateName2->setString(mateName);
                m_mateName2->setColor(getColor(color));
                place2=true;
            }else if (i==2) {
                m_mateNode3->removeAllChildren();
                m_mateNode3->addChild(bg);
                m_mateNode3->addChild(icon);
                m_mateNode3->addChild(m_mateName3);
                m_mateName3->setString(mateName);
                m_mateName3->setColor(getColor(color));
                place3=true;
            }else if (i==3) {
                m_mateNode4->removeAllChildren();
                m_mateNode4->addChild(bg);
                m_mateNode4->addChild(icon);
                m_mateNode4->addChild(m_mateName4);
                m_mateName4->setString(mateName);
                m_mateName4->setColor(getColor(color));
                place4=true;
                break;
            }
        }
        
        string nextIcon = CCCommonUtils::getIcon(CC_ITOA(m_itemId+1));
        string nextBG = CCCommonUtils::getToolBgByColor(color+1);
        string mateName5 = CCCommonUtils::getNameById(CC_ITOA(m_itemId+1));
        auto nicon = CCLoadSprite::createSprite(nextIcon.c_str(), true,CCLoadSpriteType_GOODS);
        CCCommonUtils::setSpriteMaxSize(nicon, 130, true);
        auto nbg = CCLoadSprite::createSprite(nextBG.c_str());
        CCCommonUtils::setSpriteMaxSize(nbg, 130, true);
        m_mateNode5->addChild(nicon);
        m_mateBgNode5->addChild(nbg);
        m_mateNode5->addChild(m_mateName5);
        m_mateName5->setString(mateName5);
        m_mateName5->setColor(getColor(color+1));
        
        if (m_iconNode1->getChildrenCount()==0) {
            auto icon1 = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_GOODS);
            CCCommonUtils::setSpriteMaxSize(icon1, 50);
            auto bg1 = CCLoadSprite::createSprite(bgPath.c_str());
            CCCommonUtils::setSpriteMaxSize(bg1, 50);
            m_iconNode1->addChild(bg1);
            m_iconNode1->addChild(icon1);
        }
        
        if (m_iconNode2->getChildrenCount()==0) {
            auto icon1 = CCLoadSprite::createSprite(nextIcon.c_str(), true,CCLoadSpriteType_GOODS);
            CCCommonUtils::setSpriteMaxSize(icon1, 50);
            auto bg1 = CCLoadSprite::createSprite(nextBG.c_str());
            CCCommonUtils::setSpriteMaxSize(bg1, 50);
            m_iconNode2->addChild(bg1);
            m_iconNode2->addChild(icon1);
        }
        
        m_num1Label->setString(CC_ITOA(m_itemCount));
        
        m_num2Label->setString(CC_ITOA(m_itemNextCount));
        
    }
    else {
        place1=true;
        place2=true;
        place3=true;
        place4=true;
    }
    
    m_crtBtn->setEnabled(true);
    if (m_itemCount<4 && color < GOLDEN) {
        m_crtBtn->setEnabled(false);
        m_crtBtn->setVisible(false);
        m_needLabel->setVisible(true);
    } else if (color >= GOLDEN) {
        m_crtBtn->setEnabled(false);
        m_crtBtn->setVisible(false);
        m_needLabel->setVisible(false);
    }else if(m_itemCount>=4 && color < GOLDEN){
        m_crtBtn->setEnabled(true);
        m_crtBtn->setVisible(true);
        m_needLabel->setVisible(false);
    }
}

ccColor3B MateCreateView::getColor(int color){
    switch (color) {
        case 0:
            return TEXT_COLOR_WHITE;
        case 1:
            return TEXT_COLOR_GREEN;
        case 2:
            return TEXT_COLOR_BLUE;
        case 3:
            return TEXT_COLOR_PURPLE;
        case 4:
            return TEXT_COLOR_ORANGE;
        case 5:
            return TEXT_COLOR_YELLOW;
        default:
            break;
    }
    return {0,0,0};
}

void MateCreateView::refreashData(CCObject* obj)
{
    if(m_type==1){
        int count = m_itemCount - 4;
        int nextCount = m_itemNextCount + 1;
        setData(m_itemId,count,nextCount);
    }else{
        setData(m_itemId,0,0);
    }
    
    CCCallFunc* fun1 = CCCallFunc::create(this, callfunc_selector(MateCreateView::playEndCrtParticle));
    CCCallFunc* fun2 = CCCallFunc::create(this, callfunc_selector(MateCreateView::flyCrtDia));
    CCDelayTime* delay = CCDelayTime::create(1.25f);
    CCCallFunc* fun3 = CCCallFunc::create(this, callfunc_selector(MateCreateView::removeWaitInter));
    CCCallFunc* fun4 = CCCallFunc::create(this, callfunc_selector(MateCreateView::playCanCrtParticle));
    CCSequence* sq = CCSequence::create( fun1, fun2, delay ,fun3, fun4, NULL);
    this->runAction(sq);
    //合成完成飘字特效
    string mateName = CCCommonUtils::getNameById(CC_ITOA(m_itemId+1));
    string flyStr = _lang_1("111663", mateName.c_str());
    string color = CCCommonUtils::getPropById(CC_ITOA(m_itemId+1), "color");
    ccColor3B textColor = getColor(atoi(color.c_str()));
    CCCommonUtils::flyUiResText(flyStr, m_textFlyNode,ccp(0, 0) ,textColor, floating_type_ui1, 23);//ccGREEN
}

void MateCreateView::flyCrtDia()
{
//    CCCommonUtils::flyHint("", "", _lang("119032"));
}

void MateCreateView::removeWaitInter()
{
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
}

void MateCreateView::onEnter()
{
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MateCreateView::refreashData), MSG_MATE_CRT_END, NULL);
    setTouchEnabled(true);
}

void MateCreateView::onExit()
{
    setTouchEnabled(false);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    GlobalData::shared()->isBind = false;
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MATE_CRT_END);
    CCNode::onExit();
}

bool MateCreateView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    return true;
}
void MateCreateView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(m_type==1) {
        if (equipVec.size()>0 && equipVec.size()<=6) {
            for(int i=0;i<equipVec.size();i++){
                if (isTouchInside(m_mateListNode[i],pTouch)){
                    CCLOG("m_mateListNode[%d]",i);
                    if(i==0){
                        CCCommonUtils::flyHint("", "", _lang("111662"));
                        setData(equipVec[0].itemId,0,0);
                    }else{
                        setData(equipVec[i-1].itemId,0,0);
                    }
                    setParticle();
                    return;
                }
            }
        }
    }else{
        if (toolVec.size()>0 && toolVec.size()<=6) {
            for(int i=0;i<toolVec.size();i++){
                if (isTouchInside(m_mateListNode[i],pTouch)){
                    CCLOG("m_mateListNode[%d]",i);
                    if(i==0){
                        CCCommonUtils::flyHint("", "", _lang("111662"));
                        setData(toolVec[0].itemId,0,0);
                    }else{
                        setData(toolVec[i-1].itemId,0,0);
                    }
                    setParticle();
                    return;
                }
            }
        }
    }
    
    if (isTouchInside(m_mplace1,pTouch)){
        if (place1==false) {
            if(m_type==1) {
                PopupViewController::getInstance()->addPopupInView(StoreView::create(STORE_TYPE_GOTO_STORE));
            }else{
                gotoWorkShop(0);
            }
        }
        
    }else if (isTouchInside(m_mplace2,pTouch)){
        if (place2==false) {
            if(m_type==1) {
                PopupViewController::getInstance()->addPopupInView(StoreView::create(STORE_TYPE_GOTO_STORE));
            }else{
                gotoWorkShop(0);
            }
        }
    }else if (isTouchInside(m_mplace3,pTouch)){
        if (place3==false) {
            if(m_type==1) {
                PopupViewController::getInstance()->addPopupInView(StoreView::create(STORE_TYPE_GOTO_STORE));
            }else{
                gotoWorkShop(0);
            }
        }
    }else if (isTouchInside(m_mplace4,pTouch)){
        if (place4==false) {
            if(m_type==1) {
                PopupViewController::getInstance()->addPopupInView(StoreView::create(STORE_TYPE_GOTO_STORE));
            }else{
                gotoWorkShop(0);
            }
        }
    }
}

void MateCreateView::gotoWorkShop(float _time){
    PopupViewController::getInstance()->removeAllPopupView();
    map<int, FunBuildInfo>::iterator it;
    map<int, FunBuildInfo> *curBuildsInfo = FunBuildController::getInstance()->curBuildsInfo;
    for(it=curBuildsInfo->begin(); it != curBuildsInfo->end(); ++it){
        if((it->second).type==FUN_BUILD_WORKSHOP){
            if(SceneController::getInstance()->currentSceneId == SCENE_ID_MAIN){
                auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
                layer->clickWorkshop((it->second).itemId);
                if (CCCommonUtils::isIosAndroidPad()) {
                    layer->setPxPy(-2385, -25);//材料工坊中间位置
                }else{
                    layer->setPxPy(-2568, -400);//材料工坊中间位置
                }
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("setViewByMate",CCInteger::create(toolVec[0].itemId));
            }
        }
    }
}

void MateCreateView::onClickCrtBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
//    m_waitInterface= GameController::getInstance()->showWaitInterface(m_crtBtn);
    this->getAnimationManager()->runAnimationsForSequenceNamed("Create");
    Vector<FiniteTimeAction*> arrayOfActions;
    CCCallFunc* fun1 = CCCallFunc::create(this, callfunc_selector(MateCreateView::playStartCrtParticle));
    CCDelayTime* delay = CCDelayTime::create(0.5f);
    CCCallFunc* fun2 = CCCallFunc::create(this, callfunc_selector(MateCreateView::startMateCrt));
    arrayOfActions.pushBack(fun1);
    arrayOfActions.pushBack(delay);
    arrayOfActions.pushBack(fun2);
    CCSequence* sequenceAction = CCSequence::create(arrayOfActions);
    this->runAction(sequenceAction);
}

void MateCreateView::addPopParticleToBatch(cocos2d::CCParticleSystemQuad *particle)
{
    auto batchCount = m_parPopVec.size();
    while (batchCount--) {
        auto &batch = m_parPopVec[batchCount];
        if (batch) {
            auto batchBlend = batch->getBlendFunc();
            auto particleBlend = particle->getBlendFunc();
            if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
                batch->addChild(particle);
                return;
            }
        }
    }
    auto newBatch = ParticleController::createParticleBatch();
    m_popLayer->addChild(newBatch);
    newBatch->addChild(particle);
    m_parPopVec.push_back(newBatch);
}

void MateCreateView::playStartCrtParticle()
{
    removeParticle();
    m_popNode->addChild(m_popLayer);
    
    for (int j = 1; j <= 5; ++j)
    {
        if (j <= 4) {
            string tmpStart = "Crown_";
            int count = 4;
            for (int i=1; i<=count; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
                particle->setPosition(((CCNode*)(m_midNode->getChildByTag(j)))->getPosition());
                addPopParticleToBatch(particle);
            }
        } else {
            string tmpStart = "CollectionIn_";
            int count = 8;
            for (int i=1; i<=count; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
                particle->setPosition(((CCNode*)(m_midNode->getChildByTag(j)))->getPosition());
                addPopParticleToBatch(particle);
            }
        }
    }
}

void MateCreateView::removeParticle()
{
    auto batchCount = m_parPopVec.size();
    while (batchCount--) {
        m_popLayer->removeChild(m_parPopVec[batchCount]);
    }
    m_parPopVec.clear();
    m_popNode->removeAllChildren();
    m_endCrtNode->removeAllChildren();
}

void MateCreateView::startMateCrt()
{
    removeParticle();
    EquipmentController::getInstance()->startMaterialCreate(m_itemId,m_type);
}

void MateCreateView::playEndCrtParticle()
{
    removeParticle();
    m_popNode->addChild(m_popLayer);
    
//    string tmpStart = "CardOut_";
//    int count = 8;
//    for (int i=1; i<=count; i++) {
//        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
//        particle->setPosition(((CCNode*)(m_midNode->getChildByTag(5)))->getPosition());
//        addPopParticleToBatch(particle);
//    }
    
    if (m_colorT == GOLDEN || m_colorT == ORANGE) {
        auto particle1 = ParticleController::createParticle("LevelUp_1");
        particle1->setPosition(((CCNode*)(m_midNode->getChildByTag(5)))->getPosition());
        addPopParticleToBatch(particle1);
        
        if (m_colorT == GOLDEN) {
            auto particle2 = ParticleController::createParticle("LevelUp_6");
            particle2->setPosition(((CCNode*)(m_midNode->getChildByTag(5)))->getPosition());
            addPopParticleToBatch(particle2);
        }
        
        auto particle3 = ParticleController::createParticle("LevelUp_7");
        particle3->setPosition(((CCNode*)(m_midNode->getChildByTag(5)))->getPosition());
        addPopParticleToBatch(particle3);
    }
    
    auto tmpNode = CCNode::create();
    m_popNode->addChild(tmpNode);
    tmpNode->setScale(1.5);
    string tmpStart = "Synthesis_";
    int count = 8;
    for (int i=1; i<=count; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        particle->setPosition(((CCNode*)(m_midNode->getChildByTag(5)))->getPosition());
        
        particle->setStartColor(tmpStartColorVec[i-1]);
        particle->setEndColor(tmpEndColorVec[i-1]);
        
        tmpNode->addChild(particle);
//        addPopParticleToBatch(particle);
    }
    
//    auto particle1 = ParticleController::createParticle("MallBag_1");
//    auto particle2 = ParticleController::createParticle("MallBag_2");
//    auto particle3 = ParticleController::createParticle("MallBag_3");
//    m_endCrtNode->addChild(particle1);
//    m_endCrtNode->addChild(particle2);
//    m_endCrtNode->addChild(particle3);
    
    CCScaleTo* sc1 = CCScaleTo::create(0.25, 1.4, 1.4);
    CCDelayTime* delay = CCDelayTime::create(0.1);
    CCScaleTo* sc2 = CCScaleTo::create(0.25, 1, 1);
    CCSequence* sq = CCSequence::create(sc1, delay, sc2, NULL);
    m_mateNode5->runAction(sq);
    
    CCScaleTo* sc1Bg = CCScaleTo::create(0.25, 1.4, 1.4);
    CCDelayTime* delayBg = CCDelayTime::create(0.1);
    CCScaleTo* sc2Bg = CCScaleTo::create(0.25, 1, 1);
    CCSequence* sqBg = CCSequence::create(sc1Bg, delayBg, sc2Bg, NULL);
    m_mateBgNode5->runAction(sqBg);
    SoundController::sharedSound()->playEffects(Music_Sfx_UI_loteryrwd);
}

void MateCreateView::playCanCrtParticle()
{
    removeParticle();
    if(m_type==1){
        auto &eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
        if(m_itemCount < 4 || eInfo.color >= GOLDEN){
            return;
        }
    }else{
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
        if (info.getCNT() < 4 || info.color >= GOLDEN) {
            return;
        }
    }
    m_popNode->addChild(m_popLayer);
    string tmpStart = "UIGlow_LR_";
    int count = 3;
    for (int i=1; i <= count; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        particle->setPosition(ccp(0, 149));
        addPopParticleToBatch(particle);
    }
    for (int i=1; i <= count; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        particle->setPosition(ccp(0, -144));
        addPopParticleToBatch(particle);
    }
    tmpStart = "UIGlow_UD_";
    for (int i=1; i <= count; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        particle->setPosition(ccp(146, 5));
        addPopParticleToBatch(particle);
    }
    for (int i=1; i <= count; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        particle->setPosition(ccp(-146, 5));
        addPopParticleToBatch(particle);
    }
}

SEL_CCControlHandler MateCreateView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCrtBtn", MateCreateView::onClickCrtBtn);
    return NULL;
}

bool MateCreateView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_midNode", CCNode*, m_midNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_textFlyNode", CCNode*, m_textFlyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, m_bottomNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode1", CCNode*, m_iconNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode2", CCNode*, m_iconNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num1Label", CCLabelIF*, m_num1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num2Label", CCLabelIF*, m_num2Label);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftBG", CCScale9Sprite*, m_leftBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightBG", CCScale9Sprite*, m_rightBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateListNode1", CCNode*, m_mateListNode[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateListNode2", CCNode*, m_mateListNode[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateListNode3", CCNode*, m_mateListNode[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateListNode4", CCNode*, m_mateListNode[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateListNode5", CCNode*, m_mateListNode[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateListNode6", CCNode*, m_mateListNode[5]);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprNode1", CCNode*, m_sprNode[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprNode2", CCNode*, m_sprNode[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprNode3", CCNode*, m_sprNode[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprNode4", CCNode*, m_sprNode[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprNode5", CCNode*, m_sprNode[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprNode6", CCNode*, m_sprNode[5]);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num1", CCLabelIF*, m_num[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num2", CCLabelIF*, m_num[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num3", CCLabelIF*, m_num[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num4", CCLabelIF*, m_num[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num5", CCLabelIF*, m_num[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num6", CCLabelIF*, m_num[5]);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectSpr1", CCSprite*, m_selectSpr[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectSpr2", CCSprite*, m_selectSpr[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectSpr3", CCSprite*, m_selectSpr[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectSpr4", CCSprite*, m_selectSpr[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectSpr5", CCSprite*, m_selectSpr[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectSpr6", CCSprite*, m_selectSpr[5]);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode", CCNode*, m_mateNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateName1", CCLabelIF*, m_mateName1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateName2", CCLabelIF*, m_mateName2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateName3", CCLabelIF*, m_mateName3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateName4", CCLabelIF*, m_mateName4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateName5", CCLabelIF*, m_mateName5);
    
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameMateLabel", CCLabelIF*, m_nameMateLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_crtBtn", CCControlButton*, m_crtBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode1", CCNode*, m_mateNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode2", CCNode*, m_mateNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode3", CCNode*, m_mateNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode4", CCNode*, m_mateNode4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode5", CCNode*, m_mateNode5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_popNode", CCNode*, m_popNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_endCrtNode", CCNode*, m_endCrtNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_needLabel", CCLabelIF*, m_needLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel", CCLabelIF*, m_infoLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateBgNode5", CCNode*, m_mateBgNode5);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mplace1", CCScale9Sprite*, m_mplace1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mplace2", CCScale9Sprite*, m_mplace2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mplace3", CCScale9Sprite*, m_mplace3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mplace4", CCScale9Sprite*, m_mplace4);
    return false;
}

#pragma mark -
#pragma mark - 材料信息面板

MaterialInfoView* MaterialInfoView::create(int itemId)
{
    auto ret = new MaterialInfoView();
    if (ret && ret->init(itemId)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MaterialInfoView::init(int itemId)
{
    bool ret = true;
    CCLoadSprite::doResourceByCommonIndex(11, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
    
    if (PopupBaseView::init()) {
        setIsHDPanel(true);
        CCBLoadFile("MaterialInfoView",this,this);
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        setData(itemId);
    }
    return ret;
}

void MaterialInfoView::setData(int itemId)
{
    m_waitInterface = NULL;

    m_itemId = itemId;
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
    
    m_nameLabel->setString(_lang("102271"));
    m_name1Label->setString(info.getName());
    m_numLabel->setString(_lang_1("104907", CC_ITOA(info.getCNT())));
    m_msg2Label->setString(_lang("108730"));
    m_info1Label->setString(_lang(info.des));
    m_info2Label->setString(_lang("119078"));
    
    if (info.color == GOLDEN)// 已经达到最大级
    {
        m_mateBtn->setVisible(false);
        m_msg1Label->setVisible(true);
        m_msg1Label->setString(_lang("121987"));
    }
    else if(info.getCNT() < 4) // 数量不足
    {
        m_mateBtn->setVisible(false);
        m_msg1Label->setVisible(true);
        m_msg1Label->setString(_lang("119078"));
    }
    else
    {
        m_mateBtn->setVisible(true);
        m_msg1Label->setVisible(false);
    }
    
    string iconPath = CCCommonUtils::getIcon(CC_ITOA(m_itemId));
    string bgPath = CCCommonUtils::getToolBgByColor(info.color);
    
    auto icon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_EQUIP);
    if (CCCommonUtils::isIosAndroidPad()) {
        CCCommonUtils::setSpriteMaxSize(icon, 200, true);
    }
    else
        CCCommonUtils::setSpriteMaxSize(icon, 100, true);
    auto bg = CCLoadSprite::createSprite(bgPath.c_str());
    if (CCCommonUtils::isIosAndroidPad()) {
        CCCommonUtils::setSpriteMaxSize(bg, 200, true);
    }
    else
        CCCommonUtils::setSpriteMaxSize(bg, 100, true);
    m_picNode->addChild(bg);
    m_picNode->addChild(icon);
    
    CCCommonUtils::setButtonTitle(m_mateBtn, _lang("119028").c_str());
}

void MaterialInfoView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}

void MaterialInfoView::onExit()
{
    setTouchEnabled(false);
    CCNode::onExit();
}

bool MaterialInfoView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_touchNode,pTouch)){
        return false;
    }
    return true;
}

void MaterialInfoView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isTouchInside(m_touchNode,pTouch)){
        PopupViewController::getInstance()->removePopupView(this);
    }
}

SEL_CCControlHandler MaterialInfoView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickMateBtn", MaterialInfoView::onClickMateBtn);
    return NULL;
}

bool MaterialInfoView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateBtn", CCControlButton*, m_mateBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name1Label", CCLabelIF*, m_name1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg1Label", CCLabelIF*, m_msg1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg2Label", CCLabelIF*, m_msg2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info1Label", CCLabelIF*, m_info1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info2Label", CCLabelIF*, m_info2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, m_numLabel);
    return false;
}

void MaterialInfoView::onClickMateBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("setFromWorkshop",CCInteger::create(0));
    PopupViewController::getInstance()->addPopupInView(MateCreateView::create(m_itemId,0,0,0));
    closeSelf();
}

#pragma mark -
#pragma mark - 装备信息面板
EquipInfoView* EquipInfoView::createWithEquipId(int equipId){
    auto ret = new EquipInfoView();
    if (ret && ret->initWithEquipId(equipId)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
bool EquipInfoView::initWithEquipId(int equipId){
    bool ret = true;
    CCLoadSprite::doResourceByCommonIndex(11, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
    
    if (PopupBaseView::init()) {
        setIsHDPanel(true);
        CCBLoadFile("EquipInfoView",this,this);
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        m_type = 3;
        setDataByEquipId(equipId);
    }
    return ret;
}

void EquipInfoView::setDataByEquipId(int equipId)
{
    //    m_mateBtn->setVisible(false);
    m_waitInterface = NULL;
    m_uuid = "";
    
    m_itemId = equipId;
    
    m_msg1Label->setVisible(false);
    m_destroyBtn->setVisible(false);
    CCCommonUtils::setButtonTitle(m_mateBtn, _lang("110031").c_str());//确定
    if (CCCommonUtils::isIosAndroidPad()) {
        m_mateBtn->setPreferredSize(CCSize(260, 80));
        m_mateBtn->setPosition(ccp(0,m_destroyBtn->getPositionY()));
    }
    else {
        m_mateBtn->setPreferredSize(CCSize(244, 80));
        m_mateBtn->setPosition(ccp(0,-270));
    }
    
    m_nameLabel->setString(_lang("119051"));
    if (CCCommonUtils::isIosAndroidPad()) {
        m_msg1Label->setFontSize(48);
    }
    else
        m_msg1Label->setFontSize(24);
    EquipmentInfo& info = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
    if (info.site==6) {
        info=EquipmentController::getInstance()->LongEquipmentInfoMap[m_uuid];
    }
    m_curHaveOnUuid="";
    
    string strName = _lang(info.name) + "   "+_lang("102203")+" " + CC_ITOA(info.level);
    m_name1Label->setString(strName);
    m_msg1Label->setString(_lang("119057"));
    if (!CCCommonUtils::isIosAndroidPad()) {
        m_msg1Label->setPositionY(-230);
        m_nodeEndLine->setPositionY(-209);
    }
    if(info.showType == "no")
    {
        m_desLabel->setVisible(true);
        m_desLabel->setString(_lang(info.des));
    }
    else
    {
        m_msg2Label->setString(_lang("119056"));// 属性提升
        m_desLabel->setVisible(false);
        
        for (int i=1; i<=5; i++)
        {
            string tmpInfo = "";
            if (info.showDias.find(i) != info.showDias.end()) {
                string dialog = info.showDias[i];
                if (dialog!="" && info.values.find(i)!=info.values.end()) {
                    tmpInfo = tmpInfo +"+"+CC_ITOA_MONEY(info.values[i])+"%";
                }
                
                if (i==1) {
                    m_info1Label->setString(_lang(dialog));
                    m_num1Label->setString(tmpInfo);
                }else if (i==2) {
                    m_info2Label->setString(_lang(dialog));
                    m_num2Label->setString(tmpInfo);
                    if (info.site==6) {
                        m_info2Label->setColor(rp_color);
                        m_num2Label->setColor(rp_color);
                    }
                }else if (i==3) {
                    m_info3Label->setString(_lang(dialog));
                    m_num3Label->setString(tmpInfo);
                }else if (i==4) {
                    m_info4Label->setString(_lang(dialog));
                    m_num4Label->setString(tmpInfo);
                    if (info.site==6) {
                        m_info4Label->setColor(rp_color);
                        m_num4Label->setColor(rp_color);
                    }
                }else if (i==5) {
                    m_info5Label->setString(_lang(dialog));
                    m_num5Label->setString(tmpInfo);
                }
                
            }
        }
    }
    
    string iconPath = CCCommonUtils::getIcon(CC_ITOA(m_itemId));
    string bgPath = CCCommonUtils::getToolBgByColor(info.color);
    
    auto icon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_EQUIP);
    if (CCCommonUtils::isIosAndroidPad()) {
        CCCommonUtils::setSpriteMaxSize(icon, 200, true);
    }
    else
        CCCommonUtils::setSpriteMaxSize(icon, 100, true);
    auto bg = CCLoadSprite::createSprite(bgPath.c_str());
    if (CCCommonUtils::isIosAndroidPad()) {
        CCCommonUtils::setSpriteMaxSize(bg, 200, true);
    }
    else
        CCCommonUtils::setSpriteMaxSize(bg, 100, true);
    m_picNode->addChild(bg);
    m_picNode->addChild(icon);
}

EquipInfoView* EquipInfoView::createWithBrokenEquip(int itemId,int count,int nextCount){
    auto ret = new EquipInfoView();
    if (ret && ret->initWithBrokenEquip(itemId,count,nextCount)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

EquipInfoView* EquipInfoView::create(string uuid)
{
    auto ret = new EquipInfoView();
    if (ret && ret->init(uuid)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipInfoView::init(string uuid)
{
    bool ret = true;
    CCLoadSprite::doResourceByCommonIndex(11, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
    
    if (PopupBaseView::init()) {
        setIsHDPanel(true);
        CCBLoadFile("EquipInfoView",this,this);
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        m_type = 0;
        setData(uuid);
    }
    return ret;
}
bool EquipInfoView::initWithBrokenEquip(int itemId,int count,int nextCount){
    bool ret = true;
    CCLoadSprite::doResourceByCommonIndex(11, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
    
    if (PopupBaseView::init()) {
        setIsHDPanel(true);
        CCBLoadFile("EquipInfoView",this,this);
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        this->m_type = 1;
        this->m_itemId = itemId;
        this->m_itemCount = count;
        this->m_itemNextCount = nextCount;
        
        auto frame = CCLoadSprite::loadResource("btn_01.png");
        auto graySpr = CCLoadSprite::createSprite("btn_01.png");
        auto lbl = CCLabelIF::create();
        if (CCCommonUtils::isIosAndroidPad()) {
            graySpr->setScaleX(390.0/graySpr->getContentSize().width);
            graySpr->setScaleY(145.0/graySpr->getContentSize().height);
            CCCommonUtils::setSpriteGray(graySpr, true);
            m_mateBtn->getParent()->addChild(graySpr);
            lbl->setColor(ccGRAY);
            lbl->setFontSize(40);
            m_mateBtn->getParent()->addChild(lbl);
            lbl->setString(_lang("119028"));
        }
        else {
            graySpr->setScaleX(195.0/graySpr->getContentSize().width);
            graySpr->setScaleY(78.0/graySpr->getContentSize().height);
            CCCommonUtils::setSpriteGray(graySpr, true);
            m_mateBtn->getParent()->addChild(graySpr);
            lbl->setColor(ccGRAY);
            lbl->setFontSize(20);
            m_mateBtn->getParent()->addChild(lbl);
            lbl->setString(_lang("119028"));
        }

        auto &eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
        m_nameLabel->setString(_lang("119051"));
        std::string strName = _lang(eInfo.name);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_msg1Label->setFontSize(40);
        }
        else
            m_msg1Label->setFontSize(20);
        m_name1Label->setString(strName);
        m_msg2Label->setString(_lang("108730"));
        m_desLabel->setVisible(true);
        m_desLabel->setString(_lang(eInfo.des));
        m_nodeEndLine->setPositionY(-180);
        m_msg1Label->setPositionY(-220);
        if (eInfo.color == GOLDEN){
            m_mateBtn->setEnabled(false);
            graySpr->setVisible(true);
            lbl->setVisible(true);
            m_msg1Label->setVisible(true);
            m_msg1Label->setString(_lang("121987"));
        }
        else if(m_itemCount < 1) // 数量不足
        {
            m_mateBtn->setEnabled(false);
            graySpr->setVisible(true);
            lbl->setVisible(true);
            m_msg1Label->setVisible(true);
            m_msg1Label->setString(_lang("119080"));
        }
        else
        {
            graySpr->setVisible(false);
            lbl->setVisible(false);
            m_mateBtn->setEnabled(true);
            m_msg1Label->setVisible(false);
        }
        
        string iconPath = CCCommonUtils::getIcon(CC_ITOA(m_itemId));
        string bgPath = CCCommonUtils::getToolBgByColor(eInfo.color);
        
        auto icon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_EQUIP);
        CCCommonUtils::setSpriteMaxSize(icon, 100, true);
        auto bg = CCLoadSprite::createSprite(bgPath.c_str());
        if (CCCommonUtils::isIosAndroidPad()) {
            CCCommonUtils::setSpriteMaxSize(bg, 200, true);
        }
        else
            CCCommonUtils::setSpriteMaxSize(bg, 100, true);
        m_picNode->addChild(bg);
        m_picNode->addChild(icon);
        
        CCCommonUtils::setButtonTitle(m_mateBtn, _lang("119028").c_str());
        CCCommonUtils::setButtonTitle(m_destroyBtn, _lang("119052").c_str());
        
        m_destroyBtn->setPreferredSize(CCSize(200, 80));
        m_destroyBtn->setPosition(ccp(-120,-283));
        m_mateBtn->setPreferredSize(CCSize(200, 80));
        m_mateBtn->setPosition(ccp(120,-283));
        if (CCCommonUtils::isIosAndroidPad()) {
            m_mateBtn->setPosition(ccp(180,-480));
            m_destroyBtn->setPosition(ccp(-180,-480));
            m_desLabel->setPositionY(-200);
        }
        graySpr->setPosition(m_mateBtn->getPosition());
        lbl->setPosition(m_mateBtn->getPosition());
    }
    return ret;
}
void EquipInfoView::setData(string uuid)
{
//    m_mateBtn->setVisible(false);
    m_waitInterface = NULL;
    m_uuid = uuid;
    
    m_itemId = 0;
    if (EquipmentController::getInstance()->m_MyOnEquip.find(m_uuid) != EquipmentController::getInstance()->m_MyOnEquip.end()) {
        m_itemId = EquipmentController::getInstance()->m_MyOnEquip[m_uuid];
        m_msg1Label->setVisible(false);
        m_destroyBtn->setVisible(false);
        CCCommonUtils::setButtonTitle(m_mateBtn, _lang("119003").c_str());
        if (CCCommonUtils::isIosAndroidPad()) {
            m_mateBtn->setPreferredSize(CCSize(260, 80));
            m_mateBtn->setPosition(ccp(0,m_destroyBtn->getPositionY()));
//            m_msg1Label->setPosition(m_destroyBtn->getPosition() - ccp(0, 100));
        }
        else {
            m_mateBtn->setPreferredSize(CCSize(244, 80));
            m_mateBtn->setPosition(ccp(0,-270));
//            m_msg1Label->setPosition(m_destroyBtn->getPosition() - ccp(0, 5));
        }
    }else {
        m_itemId = EquipmentController::getInstance()->m_MyEquipmentMap[m_uuid];
        m_msg1Label->setVisible(false);
        m_destroyBtn->setVisible(true);
        CCCommonUtils::setButtonTitle(m_mateBtn, _lang("119002").c_str());
    }
    
    m_nameLabel->setString(_lang("119051"));
    if (CCCommonUtils::isIosAndroidPad()) {
        m_msg1Label->setFontSize(48);
    }
    else
        m_msg1Label->setFontSize(24);
    EquipmentInfo& info = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
    if (info.site==6) {
        info=EquipmentController::getInstance()->LongEquipmentInfoMap[m_uuid];
    }
    //查找该部位是否已经装备
    m_curHaveOnUuid="";
    int curSite = info.site;
    map<string, int>::iterator it = EquipmentController::getInstance()->m_MyOnEquip.begin();
    for (; it!=EquipmentController::getInstance()->m_MyOnEquip.end(); it++) {
        auto& tmpInfo = EquipmentController::getInstance()->EquipmentInfoMap[it->second];
        if (tmpInfo.site == curSite) {
            m_curHaveOnUuid = it->first;
            break;
        }
    }
    
    string strName = _lang(info.name) + "   "+_lang("102203")+" " + CC_ITOA(info.level);
    m_name1Label->setString(strName);
    m_msg1Label->setString(_lang("119057"));
    if (!CCCommonUtils::isIosAndroidPad()) {
        m_msg1Label->setPositionY(-230);
        m_nodeEndLine->setPositionY(-209);
    }
    if(info.showType == "no")
    {
//        m_msg2Label->setString(_lang("108730"));// 描述
//        m_msg1Label->setVisible(true);
//        m_msg1Label->setString(_lang("119076"));//无法分解
//        m_destroyBtn->setVisible(false);
        m_desLabel->setVisible(true);
        m_desLabel->setString(_lang(info.des));
    }
    else
    {
        m_msg2Label->setString(_lang("119056"));// 属性提升
        m_desLabel->setVisible(false);
        
        for (int i=1; i<=5; i++)
        {
            string tmpInfo = "";
            if (info.showDias.find(i) != info.showDias.end()) {
                string dialog = info.showDias[i];
                if (dialog!="" && info.values.find(i)!=info.values.end()) {
                    tmpInfo = tmpInfo +"+"+CC_ITOA_MONEY(info.values[i])+"%";
                }
                
                if (i==1) {
                    m_info1Label->setString(_lang(dialog));
                    m_num1Label->setString(tmpInfo);
                }else if (i==2) {
                    m_info2Label->setString(_lang(dialog));
                    m_num2Label->setString(tmpInfo);
                    if (info.site==6) {
                        m_info2Label->setColor(rp_color);
                        m_num2Label->setColor(rp_color);
                    }
                }else if (i==3) {
                    m_info3Label->setString(_lang(dialog));
                    m_num3Label->setString(tmpInfo);
                }else if (i==4) {
                    m_info4Label->setString(_lang(dialog));
                    m_num4Label->setString(tmpInfo);
                    if (info.site==6) {
                        m_info4Label->setColor(rp_color);
                        m_num4Label->setColor(rp_color);
                    }
                }else if (i==5) {
                    m_info5Label->setString(_lang(dialog));
                    m_num5Label->setString(tmpInfo);
                }
                
            }
        }
    }
    
    string iconPath = CCCommonUtils::getIcon(CC_ITOA(m_itemId));
    string bgPath = CCCommonUtils::getToolBgByColor(info.color);
    
    auto icon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_EQUIP);
    if (CCCommonUtils::isIosAndroidPad()) {
        CCCommonUtils::setSpriteMaxSize(icon, 200, true);
    }
    else
        CCCommonUtils::setSpriteMaxSize(icon, 100, true);
    auto bg = CCLoadSprite::createSprite(bgPath.c_str());
    if (CCCommonUtils::isIosAndroidPad()) {
        CCCommonUtils::setSpriteMaxSize(bg, 200, true);
    }
    else
        CCCommonUtils::setSpriteMaxSize(bg, 100, true);
    m_picNode->addChild(bg);
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
            if (CCCommonUtils::isIosAndroidPad())
            {
                particle->setScale(4.5f);
            }
            m_particle->addChild(particle);
        }
    }
    
    
    CCCommonUtils::setButtonTitle(m_destroyBtn, _lang("119052").c_str());
    if (info.site==6) {
        CCCommonUtils::setButtonTitle(m_destroyBtn, _lang("111658").c_str());//出售
    }
}

void EquipInfoView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipInfoView::refreashData), EQUIP_BAG_SHOW_DES, NULL);
}

void EquipInfoView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this,EQUIP_BAG_SHOW_DES);
    setTouchEnabled(false);
    CCNode::onExit();
}

bool EquipInfoView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_touchNode,pTouch)){
        return false;
    }
    return true;
}

void EquipInfoView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isTouchInside(m_touchNode,pTouch)){
        PopupViewController::getInstance()->removePopupView(this);
    }
}

SEL_CCControlHandler EquipInfoView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickDestroyBtn", EquipInfoView::onClickDestroyBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickMateBtn", EquipInfoView::onClickMateBtn);
    return NULL;
}

bool EquipInfoView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeEndLine", CCNode*, m_nodeEndLine);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateBtn", CCControlButton*, m_mateBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_destroyBtn", CCControlButton*, m_destroyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name1Label", CCLabelIF*, m_name1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg1Label", CCLabelIF*, m_msg1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg2Label", CCLabelIF*, m_msg2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particle", CCNode*, m_particle);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info1Label", CCLabelIF*, m_info1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num1Label", CCLabelIF*, m_num1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info2Label", CCLabelIF*, m_info2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num2Label", CCLabelIF*, m_num2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info3Label", CCLabelIF*, m_info3Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num3Label", CCLabelIF*, m_num3Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info4Label", CCLabelIF*, m_info4Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num4Label", CCLabelIF*, m_num4Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info5Label", CCLabelIF*, m_info5Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num5Label", CCLabelIF*, m_num5Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    return false;
}

void EquipInfoView::onClickDestroyBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    EquipmentInfo& info = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
    if (info.site==6) {
        info=EquipmentController::getInstance()->LongEquipmentInfoMap[m_uuid];
        YesNoDialog::show(_lang_2("111657",  _lang(info.name).c_str(), info.cost.c_str()), CCCallFunc::create(this, callfunc_selector(EquipInfoView::onYesDestroyLongEquip)));
    }else{
        string colorName = CCCommonUtils::getColorName(info.color);
        YesNoDialog::show(_lang_3("119053", CC_ITOA(info.break_down), CC_ITOA(info.break_up), colorName.c_str()), CCCallFunc::create(this, callfunc_selector(EquipInfoView::onYesDestroy)));
    }
}
void EquipInfoView::onClickMateBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_type==1) {
        //材料合成
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("setFromWorkshop",CCInteger::create(0));
        PopupViewController::getInstance()->addPopupInView(MateCreateView::create(m_itemId,1,m_itemCount,m_itemNextCount));
        closeSelf();
    }else if(m_type==0){
        if (m_uuid==m_curHaveOnUuid) {//卸下
            EquipmentController::getInstance()->startTakeOffEquipment(m_uuid);
        }else{//装备
            EquipmentController::getInstance()->startPutOnEquipment(m_uuid, m_curHaveOnUuid);
        }
        closeSelf();
    }else if (m_type==3){
        PopupViewController::getInstance()->removePopupView(this);
    }
}

void EquipInfoView::onYesDestroy()
{
    
    if(m_type==1){
        auto iter = EquipmentController::getInstance()->m_MyEquipmentMap.begin();
        while (iter!= EquipmentController::getInstance()->m_MyEquipmentMap.end()) {
            if((*iter).second == m_itemId){
                m_uuid = (*iter).first;
                break;
            }
            ++iter;
        }
    }
    if(!m_uuid.empty()){
        m_waitInterface = GameController::getInstance()->showWaitInterface(m_destroyBtn);
        EquipmentController::getInstance()->startDesEquipment(m_uuid);
    }
    
//    PopupViewController::getInstance()->removePopupView(this);
//    PopupViewController::getInstance()->addPopupView(EquipBreakItemView::create("201060|3;201082|2;201110|2;201120|4;201051|1"));
}
void EquipInfoView::onYesDestroyLongEquip(){
    if(!m_uuid.empty()){
        m_waitInterface = GameController::getInstance()->showWaitInterface(m_destroyBtn);
        EquipmentController::getInstance()->startSellLongEquipment(m_uuid);
    }
}

void EquipInfoView::refreashData(CCObject* obj)
{
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    if (obj) {
        CCString* mateData = dynamic_cast<CCString*>(obj);
        if (mateData) {
            string mateMsg = mateData->getCString();
            PopupViewController::getInstance()->removePopupView(this);
            PopupViewController::getInstance()->addPopupView(EquipBreakItemView::create(mateMsg));
        }
    }else{
        PopupViewController::getInstance()->removePopupView(this);
    }
}

//class EquipInfoView

#pragma mark -
#pragma mark 装备分解面板
EquipBreakItemView* EquipBreakItemView::create(string data)
{
    auto ret = new EquipBreakItemView();
    if (ret && ret->init(data)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipBreakItemView::init(string data)
{
    bool ret = true;
    CCLoadSprite::doResourceByCommonIndex(11, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
    
    if (PopupBaseView::init()) {
        setIsHDPanel(true);
        CCBLoadFile("EquipBreakItemView",this,this);
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        
        m_nameLabel->setString(_lang("119054"));
        
        m_tabView = CCTableView::create(this, m_infoList->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setTouchPriority(0);
        m_infoList->addChild(m_tabView);
        
        CCCommonUtils::setButtonTitle(m_okBtn, _lang("confirm").c_str());
        
        setData(data);
    }
    return ret;
}

void EquipBreakItemView::setData(string data)
{
    m_data.clear();
    m_numVec.clear();
    int sumNum = 0;
    vector<string> mateVec;
    CCCommonUtils::splitString(data, ";", mateVec);
    for (int i=0; i<mateVec.size(); i++)
    {
        vector<string> mateCell;
        CCCommonUtils::splitString(mateVec[i], "|", mateCell);
        if (mateCell.size()==2) {
            m_data.push_back(atoi(mateCell[0].c_str()));
            m_numVec.push_back(atoi(mateCell[1].c_str()));
            sumNum += atoi(mateCell[1].c_str());
        }
    }
    m_msg2Label->setString(_lang_1("119055", CC_ITOA(sumNum)));
    
    m_tabView->reloadData();
}

void EquipBreakItemView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void EquipBreakItemView::onExit()
{
    setTouchEnabled(false);
    CCNode::onExit();
}

bool EquipBreakItemView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}
void EquipBreakItemView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isTouchInside(m_touchNode,pTouch)){
        PopupViewController::getInstance()->removePopupView(this);
    }
}

SEL_CCControlHandler EquipBreakItemView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickOKBtn", EquipBreakItemView::onClickOKBtn);
    return NULL;
}

bool EquipBreakItemView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_okBtn", CCControlButton*, m_okBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg1Label", CCLabelIF*, m_msg1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg2Label", CCLabelIF*, m_msg2Label);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, m_infoList);
    return false;
}

void EquipBreakItemView::onClickOKBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->removePopupView(this);
}

CCSize EquipBreakItemView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(900, 150);
    }
    return CCSize(620, 90);
}
CCSize EquipBreakItemView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(900, 150);
    }
    return CCSize(620, 90);
}

CCTableViewCell* EquipBreakItemView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if (idx >= m_data.size() || idx >= m_numVec.size()) {
        return NULL;
    }
    
    int itemId = m_data[idx];
    EquipMateCell* cell = (EquipMateCell*)table->dequeueCell();
    if (cell) {
        cell->setData(itemId, m_numVec[idx]);
    } else {
        cell = EquipMateCell::create(itemId, m_numVec[idx]);
    }
    return cell;
}

ssize_t EquipBreakItemView::numberOfCellsInTableView(CCTableView *table)
{
    return m_data.size();
}

//class cell
#pragma mark -
#pragma mark - 草药合成Cell
EquipMateCell* EquipMateCell::create(int itemId, int num)
{
    auto ret = new EquipMateCell();
    if (ret && ret->init(itemId, num)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipMateCell::init(int itemId, int num)
{
    bool ret = true;
    CCLoadSprite::doResourceByCommonIndex(11, true);
    CCBLoadFile("EquipMateCell",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        setContentSize(CCSize(900, 150));
    }
    else
        setContentSize(CCSize(620, 90));
    setData(itemId, num);
    return ret;
}

void EquipMateCell::setData(int itemId, int num)
{
    m_itemId = itemId;
    m_num = num;
    
    auto& tool = ToolController::getInstance()->getToolInfoById(itemId);
    
    string iconPath = CCCommonUtils::getIcon(CC_ITOA(m_itemId));
    string bgPath = CCCommonUtils::getToolBgByColor(tool.color);
    
    auto icon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_GOODS);
    if (CCCommonUtils::isIosAndroidPad()) {
        CCCommonUtils::setSpriteMaxSize(icon, 120, true);
    }
    else
        CCCommonUtils::setSpriteMaxSize(icon, 60, true);
    auto bg = CCLoadSprite::createSprite(bgPath.c_str());
    if (CCCommonUtils::isIosAndroidPad()) {
        CCCommonUtils::setSpriteMaxSize(bg, 120, true);
    }
    else
        CCCommonUtils::setSpriteMaxSize(bg, 60, true);
    m_picNode->addChild(bg);
    m_picNode->addChild(icon);
    
    m_nameLabel->setString(tool.getName());
    
    string numMsg = "";
    numMsg = numMsg + "+" + CC_ITOA(m_num);
    m_levelLabel->setString(numMsg);
}

void EquipMateCell::onEnter() {
    CCNode::onEnter();
}
void EquipMateCell::onExit() {
    CCNode::onExit();
}
SEL_CCControlHandler EquipMateCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}
bool EquipMateCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_levelLabel", CCLabelIF*, m_levelLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    return false;
}