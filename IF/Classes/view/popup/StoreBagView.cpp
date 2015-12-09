//
//  StoreBagView.cpp
//  IF
//
//  Created by lifangkai on 14-11-11.
//
//

#include "StoreBagView.h"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "DataRecordCommand.h"
#include "ParticleController.h"
#include "ChangeNickNameView.h"
#include "RollingText.h"
#include "ChangePicView.h"
#include "ShieldUnlockPopUpView.h"
#include "SettingPopUpView.h"
#include "ToolController.h"
#include "YesNoDialog.h"
#include "ChangePicCommand.h"
#include "WorldController.h"
#include "RoleInfoView.h"
#include "GoldExchangeMoreListView.h"
#include "PaymentCommand.h"
#include "PayController.h"
#include "WorldMapView.h"
#include "FunBuildController.h"
#include "BuildingAddOutPutCommand.h"
#include "QueueController.h"
#include "CCSafeNotificationCenter.h"
#include "SoundController.h"
#include "MailWritePopUpView.h"
#include "ChatController.h"
#include "ChatServiceCocos2dx.h"
#include "ChatView.h"
#include "ChestUseView.h"
#include "LuaController.h"
#include "AllianceMoveCityConfirmView.h"
#include "TreasureMapView.h"
#include "ActivityController.h"
#include "ComposeView.h"
#include "PayController.h"

static int advertiseCellW = 540;

StoreBagView* StoreBagView::create(){
    StoreBagView* ret = new StoreBagView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool StoreBagView::init()
{
   
    auto bg = CCBLoadFile("StoreBagView",this,this);
    setContentSize(bg->getContentSize());
    auto size = CCDirector::sharedDirector()->getWinSize();
    float dh = size.height-852-8;
    if (CCCommonUtils::isIosAndroidPad())
    {
        advertiseCellW = 1450.0;
        dh = size.height-2048+20;
    }
    
    m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height+dh));
    m_data = CCArray::create();
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    m_infoList->addChild(m_tabView);
    m_cityLv = FunBuildController::getInstance()->getMainCityLv();
    m_page = 2;

    return true;
}

void StoreBagView::onEnter(){
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);//
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(StoreBagView::refreashData), STORE_BAG_SHOW_DES, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(StoreBagView::refreashData), MSG_REFREASH_TOOL_DATA, NULL);
    refreshView();
}

void StoreBagView::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, STORE_BAG_SHOW_DES);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_REFREASH_TOOL_DATA);
    setTouchEnabled(false);
    ToolController::getInstance()->m_newGoodsList.clear();
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_HAS_NEW_GOOD);
    CCNode::onExit();
}

CCNode* StoreBagView::getGuideNode(string _key)
{
    if (_key == "GD_first") {
        StoreBagCell* cell = dynamic_cast<StoreBagCell*>(m_tabView->cellAtIndex(0));
        if (cell) {
            return cell->m_picBG1;
        }
        return NULL;
    }
    else if (_key == "GD_use") {
        StoreBagCell* cell = dynamic_cast<StoreBagCell*>(m_tabView->cellAtIndex(0));
        if (cell) {
            return cell->m_useBtn;
        }
        return NULL;
    }
    return NULL;
}

void StoreBagView::generateData(){
    m_data->removeAllObjects();
    m_dataList = NULL;


    m_dataList = &(ToolController::getInstance()->m_allTools);


    
    m_curList.clear();
    for (int i=0; i<m_dataList->size(); i++) {
        int tmpToolId = (*m_dataList)[i];
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(tmpToolId);
        if (info.getCNT()>0 && info.is_show != 1){
            m_curList.push_back(tmpToolId);
        }
    }
    sortDataByOrderNum();
    
    string itemId = "";
    bool ishas = false;
    int __num = 4;
    if (CCCommonUtils::isIosAndroidPad())
    {
        __num = 5;
    }
    int count = m_curList.size();
    for (int i=0; i<count; i++) {
        int tmpToolId = (m_curList)[i];
        if(tmpToolId==ToolController::getInstance()->m_currentUseItemId){
            ishas=true;
        }
        if (CCCommonUtils::isIosAndroidPad())
        {
            if(i%__num==0||i%__num==1||i%__num==2||i%__num==3){
                itemId.append(CC_ITOA(tmpToolId));
                itemId.append(":");
            }else{
                itemId += CC_ITOA(tmpToolId);
                m_data->addObject(CCString::create(itemId));
                itemId = "";
            }
        }
        else
        {
            if(i%__num==0||i%__num==1||i%__num==2){
                itemId.append(CC_ITOA(tmpToolId));
                itemId.append(":");
            }else{
                itemId += CC_ITOA(tmpToolId);
                m_data->addObject(CCString::create(itemId));
                itemId = "";
            }
        }
    }
    if(count%__num!=0){
        m_data->addObject(CCString::create(itemId));
    }
    int showCount =m_data->count();
    if(showCount<10){
        showCount = 10-showCount;
        for (int j=0; j<showCount; j++) {
            m_data->addObject(CCString::create("-1:-1:-1:-1"));
        }
    }
    if(!ishas){
        ToolController::getInstance()->m_currentUseItemId=0;
    }
}
bool StoreBagView::sortByUseOder(int itemId1, int itemId2){
    auto& info1 = ToolController::getInstance()->getToolInfoById(itemId1);
    auto& info2 = ToolController::getInstance()->getToolInfoById(itemId2);
    return (info1.orderNum<info2.orderNum);
    
}
void StoreBagView::sortDataByOrderNum(){
    if (m_curList.size()>0) {
        std::sort(m_curList.begin(),m_curList.end(),sortByUseOder);
    }
}
void StoreBagView::refreashData(CCObject* obj){
    if(obj && dynamic_cast<CCInteger*>(obj)){
        int opItemid = dynamic_cast<CCInteger*>(obj)->getValue();
        if(opItemid>0){
            CCObject* ccObj = NULL;
            int index = 0;
            std::string idString = CC_ITOA(opItemid);
            CCARRAY_FOREACH(m_data, ccObj){
                std::string items = dynamic_cast<CCString*>(ccObj)->getCString();
                std::vector<std::string> itemV ;
                CCCommonUtils::splitString(items, ":", itemV);
                std::vector<std::string>::iterator iter = std::find(itemV.begin(), itemV.end(), idString);
                if(iter!=itemV.end()){
                    StoreBagCell *cell = dynamic_cast<StoreBagCell*>(m_tabView->cellAtIndex(index));
                    if(cell){
                        auto& toolInfo = ToolController::getInstance()->getToolInfoById(opItemid);
                        CCNode *node = CCNode::create();
                        CCNode *nodePar = CCNode::create();
                        node->addChild(nodePar);
                        auto particle = ParticleController::createParticle("MallBagUse_1");
                        nodePar->addChild(particle);
                        particle->setPosition(CCPointZero);
                        auto particle1 = ParticleController::createParticle("MallBagUse_2");
                        nodePar->addChild(particle1);
                        particle1->setPosition(CCPointZero);
                        auto particle2 = ParticleController::createParticle("MallBagUse_3");
                        nodePar->addChild(particle2);
                        particle2->setPosition(CCPointZero);
                        nodePar->setScale(1.5);
                        auto pic = CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(toolInfo.itemId)).c_str(),true,CCLoadSpriteType_GOODS);
                        if (pic) {
                            CCCommonUtils::setSpriteMaxSize(pic, 120,true);
                            node->addChild(pic);
                            pic->setTag(10000);
                        }
                        CCPoint pos;
                        cell->playUseEffect(pos);
                        pos = m_infoList->convertToNodeSpace(pos);
                        node->setTag(10000);
                        m_infoList->addChild(node);
                        node->setPosition(pos);
                        if(pic){
                            if(toolInfo.getCNT()>0){
                                pic->runAction(CCSequence::create(CCScaleTo::create(0.5, 1.5),CCScaleTo::create(0.5, 1),NULL));
                                pic->runAction(CCSequence::create(CCDelayTime::create(1),CCCallFunc::create(this, callfunc_selector(StoreBagView::finishEffect)),NULL));
                            }else{
                                CCFiniteTimeAction *pAction1 = CCSequence::create(CCScaleTo::create(0.35, 1.5),CCScaleTo::create(0.15, 1.2),NULL);
                                pic->runAction(CCSequence::create(pAction1, CCCallFunc::create(this, callfunc_selector(StoreBagView::playUseAllEffect)), NULL));
                            }
                        }else{
                            finishEffect();
                        }
                        return;
                    }
                }
                ++index;
            }
        }
    }else if(obj && dynamic_cast<CCArray*>(obj)){
        CCArray* arr = dynamic_cast<CCArray*>(obj);
        if(arr && arr->count()>1){
            CCString* items = dynamic_cast<CCString*>(arr->objectAtIndex(0));
            CCInteger* index = dynamic_cast<CCInteger*>(arr->objectAtIndex(1));
            bool bChooseItem = (index->getValue()>-1)?true:false;
            if(bChooseItem==true){
                int offsetY = m_tabView->getContentOffset().y;
                int totalH = -offsetY;
                CCObject *ccObj =NULL;
                int nowIndex = -1;
                int aaTmp = 0;
                CCARRAY_FOREACH(m_data, ccObj){
                    std::string str1 = dynamic_cast<CCString*>(ccObj)->getCString() ;
                    std::string str2 = items->getCString();
                    if(str1.compare(str2) == 0){
                        nowIndex =aaTmp;
                        break;
                    }
                    ++aaTmp;
                }
                if(nowIndex>-1){
                    nowIndex = m_data->count() - (nowIndex+1);
                    int sizeH = nowIndex*160-200;
                    if (CCCommonUtils::isIosAndroidPad())
                    {
                        sizeH = nowIndex * (575 - 305) - 325;
                    }
                    if(sizeH<totalH){
                        CCPoint offset = m_tabView->getContentOffset();
                        offset.y += totalH - sizeH;
                        m_tabView->setContentOffset(offset);
                    }
                }
            }
            arr->release();
        }
    }
    refreashData();
}
void StoreBagView::playUseAllEffect(){
    CCNode *node = m_infoList->getChildByTag(10000);
    if(node){
        CCSprite *pic = dynamic_cast<CCSprite*>(node->getChildByTag(10000));
        if(pic){
            pic->runAction(CCSequence::create(CCScaleTo::create(0.15, 1.5),CCScaleTo::create(0.35, 0),NULL));
            pic->runAction(CCSequence::create(CCFadeOut::create(0.5),CCCallFunc::create(this, callfunc_selector(StoreBagView::finishEffect)),NULL));
        }
    }
}
void StoreBagView::finishEffect(){
    CCNode *node = m_infoList->getChildByTag(10000);
    if(node){
        CCSprite *pic = dynamic_cast<CCSprite*>(node->getChildByTag(10000));
        if(pic){
            pic->stopAllActions();
        }
        node->removeFromParent();
    }
    auto &info = ToolController::getInstance()->getToolInfoById(ToolController::getInstance()->m_currentUseItemId);
    if(info.getCNT()<=0){
        ToolController::getInstance()->m_currentUseItemId=0;
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(STORE_BAG_SHOW_DES);
    }
    refreashData();
}
void StoreBagView::refreashData(){
    float miny = m_tabView->minContainerOffset().y;
    CCPoint pos  = m_tabView->getContentOffset();
    generateData();
    m_tabView->reloadData();
    float mincurry = m_tabView->minContainerOffset().y;
    pos.y+=(-miny+mincurry);
    m_tabView->setContentOffset(pos);
}
void StoreBagView::refreshView(){
    generateData();
    m_tabView->reloadData();
}

SEL_CCControlHandler StoreBagView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool StoreBagView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    return false;
}



bool StoreBagView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_startPoint = pTouch->getStartLocation();
    if(isTouchInside(m_infoList, pTouch)){
        return false;
    }
    return true;
}

void StoreBagView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void StoreBagView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint endPoint = pTouch->getLocation();
    if(fabs(endPoint.x-m_startPoint.x)>10||fabs(endPoint.y-m_startPoint.y)>10){
        return;
    }
    if(ToolController::getInstance()->m_currentUseItemId!=0){
        ToolController::getInstance()->m_currentUseItemId=0;
        refreashData(NULL);
    }
}

CCSize StoreBagView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return CCSizeZero;
    }
    int selectitemId = ToolController::getInstance()->m_currentUseItemId;
    if(selectitemId==0){
        if (CCCommonUtils::isIosAndroidPad())
        {
            return CCSize(advertiseCellW, 575.0-305);
        }
        return CCSize(advertiseCellW, 160);
    }
    string itemId = dynamic_cast<CCString*>(m_data->objectAtIndex(idx))->getCString();
    if(itemId!=""){
        vector<string> vector1;
        CCCommonUtils::splitString(itemId, ":", vector1);
        int i= 0;
        while (i<vector1.size()) {
            if(vector1[i]!=""){
                string temp = vector1[i];
                if(atoi(temp.c_str())==selectitemId){
                    if (CCCommonUtils::isIosAndroidPad())
                    {
                        return CCSize(advertiseCellW, 575);
                    }
                    return CCSize(advertiseCellW, 366);
                }
            }
            i++;
        }
    }
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(advertiseCellW, 575.0-305);
    }
    return CCSize(advertiseCellW, 160);
}

CCSize StoreBagView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(advertiseCellW, 575.0);
    }
    return CCSize(advertiseCellW, 360);
}

CCTableViewCell* StoreBagView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    
    string itemId = dynamic_cast<CCString*>(m_data->objectAtIndex(idx))->getCString();

    StoreBagCell* cell = (StoreBagCell*)table->dequeueCell();
    if(cell){
        cell->setData(itemId);
    }else{
        cell = StoreBagCell::create(itemId,m_infoList);
    }
    return cell;
}

ssize_t StoreBagView::numberOfCellsInTableView(CCTableView *table)
{
    int num = ceil(m_data->count() / 1.0f);
    return num;
}

void StoreBagView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
    
}

void StoreBagView::scrollViewDidScroll(CCScrollView* view){
    CCLOG("%f", view->getContentOffset().x);
}

// class cell effect
StoreBagEffect* StoreBagEffect::create(){
    auto ret = new StoreBagEffect();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
bool StoreBagEffect::init()
{
     _ignoreAnchorPointForPosition = false;
    bool ret = true;
    auto node = CCBLoadFile("StoreBagEffect",this,this);
    setContentSize(node->getContentSize());
    if(m_sprNewIcon){
        std::string language = LocalController::shared()->getLanguageFileName();
        if(language.compare("zh_CN") == 0){
            cocos2d::CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("cn_newicon.png");
            if(frame){
                m_sprNewIcon->setDisplayFrame(frame);
                m_sprNewIcon1->setDisplayFrame(frame);
            }
        }else{
            cocos2d::CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("NewProp_2.png");
            if(frame){
                m_sprNewIcon->setDisplayFrame(frame);
                m_sprNewIcon1->setDisplayFrame(frame);
            }
        }
    }
    return ret;
}
bool StoreBagEffect::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprNewIcon", CCSprite*, this->m_sprNewIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprNewIcon1", CCSprite*, this->m_sprNewIcon1);
    return false;
}
//class cell

StoreBagCell* StoreBagCell::create(string itemIds, CCNode* touchNode)
{
    auto ret = new StoreBagCell();
    if (ret && ret->init(itemIds,touchNode)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool StoreBagCell::init(string itemIds, CCNode* touchNode)
{
    bool ret = true;
    CCBLoadFile("StoreBagCell",this,this);
    m_selectIndex1 = -99;
    m_waitInterface = NULL;
    m_touchNode = touchNode;
    m_useAllBtn->setVisible(false);
    setData(itemIds);
    return ret;
}

void StoreBagCell::setData(string itemIds)
{
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    m_selectIndex1=-99;
    m_a[0] = -1;
    m_a[1] = -1;
    m_a[2] = -1;
    m_a[3] = -1;
    m_a[4] = -1;
    m_items = itemIds;
    vector<string> vector1;
    CCCommonUtils::splitString(m_items, ":", vector1);
    int i= 0;
    while (i<vector1.size()) {
        if(vector1[i]!=""){
            string temp = vector1[i];
            m_a[i] = atoi(temp.c_str());
        }
        i++;
    }
    bool isDes = false;
    int __num = 4;
    if (CCCommonUtils::isIosAndroidPad())
    {
        __num = 5;
    }
    for (int j=0; j<__num; j++) {
        if(m_a[j]==ToolController::getInstance()->m_currentUseItemId){
            isDes = true;
            m_selectIndex1 = j;
            break;
        }
    }
    if(isDes){
        this->setContentSize(CCSize(606, 360));
    }else{
        this->setContentSize(CCSize(606, 160));
    }
    if (CCCommonUtils::isIosAndroidPad())
    {
        if(isDes){
            this->setContentSize(CCSize(1450, 575));
        }else{
            this->setContentSize(CCSize(1450, 575-305));
        }
    }
    m_picNode1->removeAllChildren();
    m_picNode1->setVisible(true);
    m_picNode2->removeAllChildren();
    m_picNode2->setVisible(true);
    m_picNode3->removeAllChildren();
    m_picNode3->setVisible(true);
    m_picNode4->removeAllChildren();
    m_picNode4->setVisible(true);
    
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_picNode5->removeAllChildren();
        m_picNode5->setVisible(true);
    }
    
    if(m_a[0]!=-1){
        this->m_itemNode1->setVisible(true);
        if (CCCommonUtils::isIosAndroidPad())
            CCCommonUtils::createGoodsIcon(m_a[0], m_picNode1, CCSize(110*1.6, 110*1.6),m_numLabel1);
        else
            CCCommonUtils::createGoodsIcon(m_a[0], m_picNode1, CCSize(110, 110),m_numLabel1);
//        ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_a[0]);
//        m_numLabel1->setString(CC_CMDITOA(info.getCNT()));
//        auto pic = CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(info.itemId)).c_str(),true,CCLoadSpriteType_GOODS);
//        if (pic) {
//            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(info.color).c_str());
//            CCCommonUtils::setSpriteMaxSize(iconBg, 127,true);
//            m_picNode1->addChild(iconBg);
//            CCCommonUtils::setSpriteMaxSize(pic, 120,true);
//            m_picNode1->addChild(pic);
//        }
    }else{
        this->m_itemNode1->setVisible(false);
    }
    if(m_a[1]!=-1){
        this->m_itemNode2->setVisible(true);
        if (CCCommonUtils::isIosAndroidPad())
            CCCommonUtils::createGoodsIcon(m_a[1], m_picNode2, CCSize(127*1.6, 127*1.6),m_numLabel2);
        else
            CCCommonUtils::createGoodsIcon(m_a[1], m_picNode2, CCSize(127, 127),m_numLabel2);
//        ToolInfo& info1 = ToolController::getInstance()->getToolInfoById(m_a[1]);
//        m_numLabel2->setString(CC_CMDITOA(info1.getCNT()));
//        auto pic = CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(info1.itemId)).c_str(),true,CCLoadSpriteType_GOODS);
//        if (pic) {
//            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(info1.color).c_str());
//            CCCommonUtils::setSpriteMaxSize(iconBg, 127,true);
//            m_picNode2->addChild(iconBg);
//            CCCommonUtils::setSpriteMaxSize(pic, 120,true);
//            m_picNode2->addChild(pic);
//        }
    }else{
        this->m_itemNode2->setVisible(false);
    }
    if(m_a[2]!=-1){
        this->m_itemNode3->setVisible(true);
        if (CCCommonUtils::isIosAndroidPad())
            CCCommonUtils::createGoodsIcon(m_a[2], m_picNode3, CCSize(127*1.6, 127*1.6),m_numLabel3);
        else
            CCCommonUtils::createGoodsIcon(m_a[2], m_picNode3, CCSize(127, 127),m_numLabel3);
//        ToolInfo& info2 = ToolController::getInstance()->getToolInfoById(m_a[2]);
//        m_numLabel3->setString(CC_CMDITOA(info2.getCNT()));
//        auto pic = CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(info2.itemId)).c_str(),true,CCLoadSpriteType_GOODS);
//        if (pic) {
//            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(info2.color).c_str());
//            CCCommonUtils::setSpriteMaxSize(iconBg, 127,true);
//            m_picNode3->addChild(iconBg);
//            CCCommonUtils::setSpriteMaxSize(pic, 120,true);
//            m_picNode3->addChild(pic);
//        }
    }else{
        this->m_itemNode3->setVisible(false);
    }
    if(m_a[3]!=-1){
        this->m_itemNode4->setVisible(true);
        if (CCCommonUtils::isIosAndroidPad())
            CCCommonUtils::createGoodsIcon(m_a[3], m_picNode4, CCSize(127*1.6, 127*1.6),m_numLabel4);
        else
            CCCommonUtils::createGoodsIcon(m_a[3], m_picNode4, CCSize(127, 127),m_numLabel4);
//        ToolInfo& info3 = ToolController::getInstance()->getToolInfoById(m_a[3]);
//        m_numLabel4->setString(CC_CMDITOA(info3.getCNT()));
//        auto pic = CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(info3.itemId)).c_str(),true,CCLoadSpriteType_GOODS);
//        if (pic) {
//            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(info3.color).c_str());
//            CCCommonUtils::setSpriteMaxSize(iconBg, 127,true);
//            m_picNode4->addChild(iconBg);
//            CCCommonUtils::setSpriteMaxSize(pic, 120,true);
//            m_picNode4->addChild(pic);
//        }
    }else{
        this->m_itemNode4->setVisible(false);
    }
    
    if(CCCommonUtils::isIosAndroidPad() && m_a[4]!=-1) {
        this->m_itemNode5->setVisible(true);
        CCCommonUtils::createGoodsIcon(m_a[4], m_picNode5, CCSize(127*1.6, 127*1.6),m_numLabel5);
    }
    else if(CCCommonUtils::isIosAndroidPad() && m_a[4]==-1) {
        this->m_itemNode5->setVisible(false);
    }
    
    
    
    m_desNode->setVisible(false);
    if(m_selectIndex1!=-99){
        m_desNode->setVisible(true);
        m_selectSpr->setVisible(true);
        m_itemsNode->setPositionY(206);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_itemsNode->setPositionY(360.1);
        }
        m_arrowSpr->setPositionX(93.2+m_selectIndex1*148);
        ToolController::getInstance()->m_currentUseItemId = m_a[m_selectIndex1];
        m_itemId = ToolController::getInstance()->m_currentUseItemId;
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_a[m_selectIndex1]);
        m_nameLabel->setString(info.getName().c_str());
        
        string tActId = info.getActId();
        if ("" != tActId && info.type == 0 && ActivityController::getInstance()->isHasAct(tActId)) {
            CCCommonUtils::setButtonTitle(m_useBtn, _lang("133076").c_str());
        }else if (info.type == 13 )
        {
              CCCommonUtils::setButtonTitle(m_useBtn, _lang("119028").c_str());
        }else {
            CCCommonUtils::setButtonTitle(m_useBtn, _lang("102137").c_str());
        }
        m_useBtn->setTouchPriority(Touch_Popup_Item);
        //CCCommonUtils::setButtonTitle(m_useAllBtn, _lang("104926").c_str());
        m_useAllBtn->setTouchPriority(Touch_Popup_Item);
        m_desNode->setVisible(true);
        m_itemsNode->setPositionY(206);
        m_arrowSpr->setPositionX(93.2+m_selectIndex1*148);
        m_selectSpr->setPositionX(85+145*m_selectIndex1);
        
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_arrowSpr->setPositionX(93.2+m_selectIndex1*285);
            m_selectSpr->setPositionX(85+283*m_selectIndex1);
            m_itemsNode->setPositionY(360.1);
        }
        bool hideAllBtn = true;
//        if(info.useall==1){
//            m_useAllBtn->setVisible(true);
//            hideAllBtn=false;
//        }else{
//            m_useAllBtn->setVisible(false);
//        }
        if(info.use==1 || info.useall==1){
            m_useBtn->setVisible(true);
            hideAllBtn=false;
        }else{
            m_useBtn->setVisible(false);
        }
        tActId = info.getActId();
        if (tActId != "")
        {
            if (info.type == 0)
            {
                if (ActivityController::getInstance()->isHasAct(tActId)) {
                    m_useBtn->setVisible(true);
                } else {
                    m_useBtn->setVisible(false);
                }
            } else {
                m_useBtn->setVisible(true);
            }
        }
        if(hideAllBtn==true){
            m_desLabel->setDimensions(CCSize(500, 120));
            if (CCCommonUtils::isIosAndroidPad())
            {
                m_desLabel->setDimensions(CCSize(1350, 160));
            }
        }else{
            m_desLabel->setDimensions(CCSize(400,120));
            if (CCCommonUtils::isIosAndroidPad()) {
                m_desLabel->setDimensions(CCSize(CCSize(1100, 160)));
            }
        }
        m_desLabel->setString(_lang(info.des).c_str());
    }else{
        m_itemsNode->setPositionY(0);
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_itemsNode->setPositionY(55);
        }
        m_desNode->setVisible(false);
         m_selectSpr->setVisible(false);
    }
    int iIdx = 0;
    while (iIdx<4) {
        std::vector<int> &newList= ToolController::getInstance()->m_newGoodsList;
        int gID =m_a[iIdx];
        if(gID!=-1){
            if(find(newList.begin(), newList.end(), gID)!=newList.end()){
                m_newNode[iIdx]->removeAllChildren();
                m_newNode[iIdx]->addChild(StoreBagEffect::create());
                ++iIdx;
                continue;
            }
        }
        m_newNode[iIdx]->removeAllChildren();
        ++iIdx;
    }
}

void StoreBagCell::onEnter() {
    CCNode::onEnter();
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup, false);//Touch_Popup_Item
//    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(StoreBagCell::refreashData), MSG_TOOL_CHANGE, NULL);
}

void StoreBagCell::onExit() {
    setTouchEnabled(false);
//    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TOOL_CHANGE);
    CCNode::onExit();
}

void StoreBagCell::refreashData(CCObject* obj)
{
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }

    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
    string numStr =  CC_CMDITOA(info.getCNT());
    if(m_selectIndex1==0){
        m_numLabel1->setString(numStr);
    }else if(m_selectIndex1==1){
        m_numLabel1->setString(numStr);
    }else if(m_selectIndex1==2){
        m_numLabel1->setString(numStr);
    }else if(m_selectIndex1==3){
        m_numLabel1->setString(numStr);
    }
    if(info.getCNT() <= 0){
        ToolController::getInstance()->m_currentUseItemId=0;
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(STORE_BAG_SHOW_DES);
    }
//    m_numLabel->setString(numStr);
//
//    if (info.cnt <= 0) {
//        m_priceNode->setVisible(true);
//        m_useBtn->setVisible(false);
//        
//        if (m_type == 1 && info.price<=0) {//背包内，使用完不可购买的道具
//        }
//        else if (m_type == 1) {
//            m_type = 2;
//        }
//        else if (m_type == 0 && info.use==1) {
//            m_unUseBtn->setVisible(true);
//            m_storeUseBtn->setVisible(false);
//        }
//    }
}

SEL_CCControlHandler StoreBagCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickUseBtnClick", StoreBagCell::onClickUseBtnClick);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickUseAllBtnClick", StoreBagCell::onClickUseAllBtnClick);
    return NULL;
}


bool StoreBagCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_clickInSide = false;
    m_startPoint = pTouch->getStartLocation();
    if (isTouchInside(m_touchNode,pTouch)){
        m_clickInSide = true;
        if(isTouchInside(this,pTouch)){
            return true;
        }
    }
//    if (isTouchInside(m_touchNode,pTouch)) {
//        m_clickInSide = true;
//        m_buyBtn->setEnabled(true);
//        m_storeUseBtn->setEnabled(true);
//        m_useBtn->setEnabled(true);
//        if (isTouchInside(m_useBtn,pTouch) || isTouchInside(m_storeUseBtn,pTouch)) {
//            return true;
//        }
//    }else{
//        m_buyBtn->setEnabled(false);
//        m_storeUseBtn->setEnabled(false);
//        m_useBtn->setEnabled(false);
//    }
    return false;
}

void StoreBagCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint endPoint = pTouch->getLocation();
    if(fabs(endPoint.x-m_startPoint.x)>10||fabs(endPoint.y-m_startPoint.y)>10){
        return;
    }
    
    if (isTouchInside(m_itemBg,pTouch)&&m_desNode->isVisible()) {
        return;
    }
    
    int curItemId = ToolController::getInstance()->m_currentUseItemId;
    if (isTouchInside(m_picBG1,pTouch)){
        if(m_a[0]!=-1){
            if (curItemId == m_a[0]) {
                showDesByIndex(-1);
            }else {
                showDesByIndex(0);
            }
        }
    }else if (isTouchInside(m_picBG2,pTouch)){
        if(m_a[1]!=-1){
            if (curItemId == m_a[1]) {
                showDesByIndex(-1);
            }else {
                showDesByIndex(1);
            }
        }
    }else if (isTouchInside(m_picBG3,pTouch)){
        if(m_a[2]!=-1){
            if (curItemId == m_a[2]) {
                showDesByIndex(-1);
            }else {
                showDesByIndex(2);
            }
        }
    }else if (isTouchInside(m_picBG4,pTouch)){
        if(m_a[3]!=-1){
            if (curItemId == m_a[3]) {
                showDesByIndex(-1);
            }else {
                showDesByIndex(3);
            }
        }
    }else if (CCCommonUtils::isIosAndroidPad() && isTouchInside(m_picBG5,pTouch)){
        if(m_a[4]!=-1){
            if (curItemId == m_a[4]) {
                showDesByIndex(-1);
            }else {
                showDesByIndex(4);
            }
        }
    }
    return;
    
//    if(m_selectIndex1!=-99&&(!isTouchInside(m_itemBg,pTouch))){
//        showDesByIndex(-1);
//        return;
//    }
    
    //    if (isTouchInside(m_useBtn,pTouch)) {
    //        if (m_useBtn->isVisible()) {
    //            onUseTool();
    //        }
    //        else {
    //            if (m_type==0) {
    //                ToolController::getInstance()->buyTool(m_itemId, 1, CCCallFunc::create(this, callfunc_selector(ToolCell::onRetBuyTool)));
    //            }
    //            else if (m_type == 2) {
    //                onYes();
    //            }
    //        }
    //    }
    //    else {
    //        if (m_storeUseBtn->isVisible()) {
    //            onUseTool();
    //        }
    //    }
}
void StoreBagCell::showDesByIndex(int index){
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    if(index==-1){
//        m_selectIndex1 = -99;
        ToolController::getInstance()->m_currentUseItemId=0;
//        m_itemsNode->setPositionY(0);
//        m_desNode->setVisible(false);
        m_desLabel->setString("");
        m_desLabel->setDimensions(CCSize(300,80));
        if (CCCommonUtils::isIosAndroidPad()) {
            m_desLabel->setDimensions(CCSize(600, 160));
        }
        CCArray *array = CCArray::create();
        array->addObject(CCString::create(m_items));
        array->addObject(CCInteger::create(-1));
        array->retain();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(STORE_BAG_SHOW_DES,array);
    }else{
        if(m_selectIndex1==index){
            return;
        }
        
        m_selectIndex1= index;
        ToolController::getInstance()->m_currentUseItemId = m_a[index];
        m_itemId = ToolController::getInstance()->m_currentUseItemId;
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_a[index]);
        m_nameLabel->setString(info.getName().c_str());

        CCCommonUtils::setButtonTitle(m_useBtn, _lang("102137").c_str());
        //CCCommonUtils::setButtonTitle(m_useAllBtn, _lang("104926").c_str());
        m_desNode->setVisible(true);
         m_selectSpr->setVisible(true);
        m_itemsNode->setPositionY(206);
        m_arrowSpr->setPositionX(93.2+m_selectIndex1*148);
        m_selectSpr->setPositionX(85+145*m_selectIndex1);
        this->setContentSize(CCSize(606, 360));
        if (CCCommonUtils::isIosAndroidPad())
        {
           m_selectSpr->setPositionX(85*1.7+283*m_selectIndex1);
            this->setContentSize(CCSize(1450, 575));
             m_itemsNode->setPositionY(360.1);
        }
        bool hideAllBtn = true;
//        if(info.useall==1){
//            m_useAllBtn->setVisible(true);
//            hideAllBtn=false;
//        }else{
//            m_useAllBtn->setVisible(false);
//        }
        if(info.use==1 || info.useall==1){
            m_useBtn->setVisible(true);
            hideAllBtn=false;
        }else{
            m_useBtn->setVisible(false);
        }
        if(hideAllBtn==true){
            m_desLabel->setDimensions(CCSize(500,120));
            if (CCCommonUtils::isIosAndroidPad()) {
                m_desLabel->setDimensions(CCSize(1350, 160));
            }
        }else{
            m_desLabel->setDimensions(CCSize(400,120));
            if (CCCommonUtils::isIosAndroidPad()) {
                m_desLabel->setDimensions(CCSize(1100, 160));
            }
        }
        m_desLabel->setString(_lang(info.des).c_str());
        
        CCArray *array = CCArray::create();
        array->addObject(CCString::create(m_items));
        array->addObject(CCInteger::create(m_selectIndex1));
        array->retain();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(STORE_BAG_SHOW_DES,array);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                               , CCString::createWithFormat("GD_first"));
    }
    
}
void StoreBagCell::onClickUseBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
//    PopupViewController::getInstance()->addPopupView(ChestUseView::create(NULL),false,false);return;
    if(!m_clickInSide) return ;
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
    if(ToolController::getInstance()->isNewVersionGoods(info.getVersion())){
        return;
    }
    bool flag = QueueController::getInstance()->checkMarchStatus();
    if (info.type==4 && info.type2==1 && flag) {
        CCCommonUtils::flyHint("", "", _lang("101439"));
        return ;
    }
    if(m_itemId == ITEM_CITY_DEF_FULL){
        unsigned int fireRate = 0;
        if (GlobalData::shared()->isCrossService) {
            fireRate = GlobalData::shared()->wb_fire_k1;
        }else if (GlobalData::shared()->cityTileCountry == NEUTRALLY) {
            fireRate = GlobalData::shared()->worldConfig.fire[2];
        }else{
            fireRate = GlobalData::shared()->worldConfig.fire[0];
        }
        auto now = WorldController::getInstance()->getTime();
        unsigned int fix = 0;
        if (GlobalData::shared()->cityStartFireStamp < GlobalData::shared()->cityFireStamp) {
            double start = MIN(now, GlobalData::shared()->cityFireStamp);
            double duration = start - GlobalData::shared()->cityStartFireStamp;
            fix = (unsigned int)(duration * (double)fireRate / 3600000.0);
        }
        unsigned int cityDefMax = GlobalData::shared()->imperialInfo[FUN_BUILD_WALL_ID].para2 + CCCommonUtils::getEffectValueByNum(125);
        unsigned int cityDefFix = GlobalData::shared()->cityDefenseVal - fix;
        if(now >= GlobalData::shared()->cityFireStamp){
            cityDefFix = GlobalData::shared()->cityDefenseVal;
        }
        if (cityDefFix >= cityDefMax) {
            YesNoDialog::showYesDialog(_lang("104964").c_str());
            return;
        }
        YesNoDialog::show(_lang_1("104955", info.getName().c_str()), CCCallFunc::create(this, callfunc_selector(StoreBagCell::sureToUseTool)));
    }else if(m_itemId == ITEM_SEND_NOTICE){
        if(ChatController::getInstance()->getIsNoticeOpen() == false){
            
            return;
        }
        int chatType = CHAT_COUNTRY;
        PopupViewController::getInstance()->removeLastPopupView();
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
        CCLOG("GlobalData::shared()->android_native_chat = %d", GlobalData::shared()->android_native_chat);
        if(GlobalData::shared()->android_native_chat == 1){
            if(!ChatServiceCocos2dx::isChatShowing){
                CCLOG("onChatBtnClick 1 showChatActivityFrom2dx");
                ChatServiceCocos2dx::m_channelType=CHANNEL_TYPE_COUNTRY;
//                ChatServiceCocos2dx::m_isInMailDialog=false;
                ChatServiceCocos2dx::m_isNoticItemUsed=true;
                ChatServiceCocos2dx::showChatActivityFrom2dx();
            }
        }else{
            PopupViewController::getInstance()->addPopupInView(ChatView::create(chatType,"",1),false);
        }
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
        if(ChatServiceCocos2dx::enableNativeChat){
            if(!ChatServiceCocos2dx::isChatShowing_fun()){
                CCLOG("onChatBtnClick 1 showChatActivityFrom2dx");
                ChatServiceCocos2dx::m_channelType=CHANNEL_TYPE_COUNTRY;
                //                ChatServiceCocos2dx::m_isInMailDialog=false;
                ChatServiceCocos2dx::m_isNoticItemUsed=true;
                ChatServiceCocos2dx::showChatIOSFrom2dx();
            }
        }else{
            PopupViewController::getInstance()->addPopupInView(ChatView::create(chatType,"",1),false);
        }
#endif
    }else if(m_itemId == ITEM_SEND_GIFT || m_itemId == ITEM_SEND_GIFTALLIANCE || m_itemId == ITEM_SEND_GIFTMORE){
        if(m_itemId == ITEM_SEND_GIFTALLIANCE && GlobalData::shared()->playerInfo.isInAlliance() == false){
            YesNoDialog::showYesDialog(_lang("101016").c_str());
            return;
        }
       PopupViewController::getInstance()->addPopupInViewWithAnim(MailWritePopUpView::createWithGift("",0,m_itemId));
    }else if(m_itemId >= ITEM_TREASURE1 && m_itemId <= ITEM_TREASURE5){
        if(GlobalData::shared()->isCrossService==true){
            YesNoDialog::showYesDialog(_lang("138064").c_str());
            return;
        }
        
        PopupViewController::getInstance()->addPopupView(TreasureMapView::create(info.color,m_itemId));
    }else if(info.useall==1 && info.getCNT()>1){
        PopupViewController::getInstance()->addPopupView(ToolNumSelectView::create(m_itemId));
    }else if (info.itemId == 200590) {//新手礼包
        sureToUseTool();
    }else if (info.itemId == ITEM_ALLIANCE_CITY_MOVE){
        sureToUseTool();
    }else if ("" != info.getActId()){
        sureToUseTool();
    }else{
        if(info.type == 13){
            sureToUseTool();
        }
        else{
        YesNoDialog::show(_lang_1("104955", info.getName().c_str()), CCCallFunc::create(this, callfunc_selector(StoreBagCell::sureToUseTool)));
        }
    }
}
void StoreBagCell::sureToUseTool(){
    if(!m_clickInSide) return ;
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
    if(info.useall == 1 && info.getCNT()>1){
        
    }else{
        onUseTool(false);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           , CCString::createWithFormat("GD_use"));
        if (m_itemId==ITEM_FIREWORK1||m_itemId==ITEM_FIREWORK2) {
            PopupViewController::getInstance()->removeAllPopupView();
        }
    }
}
void StoreBagCell::onClickUseAllBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if(!m_clickInSide) return ;
    onUseTool(true);
}

void StoreBagCell::onYes()
{
    ToolController::getInstance()->buyTool(m_itemId, 1, CCCallFunc::create(this, callfunc_selector(StoreBagCell::onUseToolEvent)),0,false,false,"StoreBagView");
}

void StoreBagCell::onUseToolEvent()
{
    onUseTool();
}

void StoreBagCell::onUseTool(bool useAll)
{
    ToolInfo& tInfo = ToolController::getInstance()->getToolInfoById(m_itemId);
     if(tInfo.type == 13 && tInfo.getCNT()>0){
       PopupViewController::getInstance()->addPopupView(ComposeView::create(m_itemId)) ;
        return ;
    }
    if((tInfo.type == 11 || tInfo.type == 12) && tInfo.getCNT()>0){
        if(GlobalData::shared()->isCrossService==true){
            YesNoDialog::showYesDialog(_lang("138064").c_str());
            return;
        }
        WorldMapView::instance()->gotoAddWorldItems(m_itemId);
        return;
    }
    if(m_itemId == ITEM_RENAME) {
//        ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
//        
//        if (info.getCNT() > 0) {  //获取数量
//            PopupViewController::getInstance()->addPopupView(ComposeView::create(m_itemId)) ;
//        }

        ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
        if (info.getCNT() > 0) {
            PopupViewController::getInstance()->addPopupView(ChangeNickNameView::create(info.uuid));
        }
    }
    else if(m_itemId == ITEM_INCREASE_GATHER_SPEED){
        auto sendUseCmd = [&](){
            bool ret = ToolController::getInstance()->useTool(m_itemId, 1 , true);
            if (ret) {
                m_waitInterface=GameController::getInstance()->showWaitInterface(m_useBtn);
            }
        };
        bool flag = CCCommonUtils::hasSpeedEffect(8);
        if(flag){
            YesNoDialog::show(_lang("101432").c_str(), sendUseCmd);
        }else{
            sendUseCmd();
        }
    }
    else if (m_itemId == ITEM_ALLIANCE_CITY_MOVE) {
        if (!GlobalData::shared()->playerInfo.isInAlliance()) {
            CCCommonUtils::flyText(_lang("115428"));
        } else {
            ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
            if (info.getCNT() > 0)
            {
                PopupViewController::getInstance()->addPopupView(AllianceMoveCityConfirmView::create());
            }
        }
    }
    else if(m_itemId == ITEM_RANDOM) {
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
        if (info.getCNT() > 0) {
            auto dict = CCDictionary::create();
            dict->setObject(CCInteger::create(0), "point");
            dict->setObject(CCString::create(info.uuid), "goodsId");
            
            WorldMapView::instance()->afterCityMove(dict);
            PopupViewController::getInstance()->removeAllPopupView();
        }
    }else if(m_itemId == ITEM_CHANGE_PIC) {
        PopupViewController::getInstance()->addPopupView(ChangePicView::create());
    }else if(m_itemId == ITEM_MOVE_BUILD) {//交换建筑
        auto world = WorldMapView::instance();
        if (world) {//在世界，不能使用改道具
            CCCommonUtils::flyHint("", "", _lang("108895"));
        }else {
            PopupViewController::getInstance()->removeAllPopupView();
            FunBuildController::getInstance()->OpenMoveBuild();
        }
    }
    else if(m_itemId == ITEM_WOOD_SPD || m_itemId == ITEM_FOOD_SPD || m_itemId == ITEM_IRON_SPD || m_itemId == ITEM_STONE_SPD) {
        int bType = FUN_BUILD_WOOD;
        if (m_itemId == ITEM_FOOD_SPD) {
            bType = FUN_BUILD_FOOD;
        }else if (m_itemId == ITEM_IRON_SPD) {
            bType = FUN_BUILD_IRON;
        }if (m_itemId == ITEM_STONE_SPD) {
            bType = FUN_BUILD_STONE;
        }
        int buildId = FunBuildController::getInstance()->getCanBoostBuild(bType);
        if (buildId>0) {
            m_boostBid = buildId;
            auto& bInfo = FunBuildController::getInstance()->getFunbuildById(m_boostBid);
            string tmpStr = _lang_2("104920", CC_ITOA(bInfo.level), _lang(bInfo.name).c_str());
            YesNoDialog::show( tmpStr.c_str() , CCCallFunc::create(this, callfunc_selector(StoreBagCell::onBoostBuild)));
        }
        else {//没有可以加速生产的资源建筑
            CCCommonUtils::flyHint("", "", _lang_1("104925",CCCommonUtils::getNameById(CC_ITOA(bType)).c_str() ));
        }
    }
    else if ("" != tInfo.getActId()) {
        if (tInfo.type == 0 && ActivityController::getInstance()->isHasAct(tInfo.getActId()))
        {
            ActivityController::getInstance()->createRwdViewByActId(tInfo.getActId());
        } else {
            if (tInfo.type != 0)
            {
                bool ret = ToolController::getInstance()->useTool(m_itemId, 1 , true);
                if (ret) {
                    m_waitInterface=GameController::getInstance()->showWaitInterface(m_useBtn);
                }
            }
        }
    }
    else {
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
        int num = 1;
        if(useAll){
//            num=info.cnt;
            PopupViewController::getInstance()->addPopupView(ToolNumSelectView::create(m_itemId));
            return;
        }
        bool ret = ToolController::getInstance()->useTool(m_itemId, num , true);
        if (ret) {

            m_waitInterface=GameController::getInstance()->showWaitInterface(m_useBtn);
  
            string numStr =  CC_CMDITOA(info.getCNT());
//            if(m_selectIndex1==0){
//                m_numLabel1->setString(numStr);
//            }else if(m_selectIndex1==1){
//                m_numLabel1->setString(numStr);
//            }else if(m_selectIndex1==2){
//                m_numLabel1->setString(numStr);
//            }else if(m_selectIndex1==3){
//                m_numLabel1->setString(numStr);
//            }
//            m_numLabel->setString(numStr);
        }
    }
}

void StoreBagCell::onBoostBuild()
{
    if (m_boostBid>0) {
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
        auto& bInfo = FunBuildController::getInstance()->getFunbuildById(m_boostBid);
        bInfo.effectTime = GlobalData::shared()->getWorldTime()+24*60*60-1;
        BuildingAddOutPutCommand* cmd = new BuildingAddOutPutCommand(bInfo.uuid,m_boostBid, info.uuid);
        cmd->sendAndRelease();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUILD_FUNCTION_EFFECT,CCInteger::create(m_boostBid));
        m_waitInterface=GameController::getInstance()->showWaitInterface(m_useBtn);
        ToolController::getInstance()->useTool(m_itemId,1,false);
        CCCommonUtils::flyHintWithDefault(CCCommonUtils::getIcon(CC_ITOA(info.itemId)), "", _lang_1("104909",info.getName().c_str()));
    }
}

bool StoreBagCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_useAllBtn", CCControlButton*, this->m_useAllBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_useBtn", CCControlButton*, this->m_useBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);

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
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrowSpr", CCSprite*, m_arrowSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectSpr", CCSprite*, m_selectSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode", CCNode*, m_desNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemsNode", CCNode*, m_itemsNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode1", CCNode*, m_itemNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode2", CCNode*, m_itemNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode3", CCNode*, m_itemNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode4", CCNode*, m_itemNode4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemBg", CCScale9Sprite*, m_itemBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newNode1", CCNode*, m_newNode[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newNode2", CCNode*, m_newNode[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newNode3", CCNode*, m_newNode[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newNode4", CCNode*, m_newNode[3]);
    
    if (CCCommonUtils::isIosAndroidPad())
    {
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode5", CCNode*, m_itemNode5);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode5", CCNode*, m_picNode5);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBG5", CCSprite*, m_picBG5);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newNode5", CCNode*, m_newNode[4]);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel5", CCLabelIF*, m_numLabel5);
    }
    
    return false;
}
CCNode* StoreBagCell::getCurPicNode(){
    CCNode* node = NULL;
    switch (m_selectIndex1) {
        case 0:{
            node = m_picNode1;
            break;
        }
        case 1:{
            node = m_picNode2;
            break;
        }
        case 2:{
            node = m_picNode3;
            break;
        }
        case 3:{
            node = m_picNode4;
            break;
        }
        case 4:{
            node = m_picNode5;
            break;
        }
        default:
            break;
    }
    return node;
}
void StoreBagCell::playUseEffect(CCPoint &pos){
    CCNode* node = getCurPicNode();
    if(node!=NULL){
        pos = node->convertToWorldSpace(CCPointZero);
        node->setVisible(false);
    }
}

ToolNumSelectView* ToolNumSelectView::create(int id,int opFrom) {
    auto ret = new ToolNumSelectView(id,opFrom);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ToolNumSelectView::init() {
    bool ret = false;
    if (PopupBaseView::init()) {
        setIsHDPanel(true);
        CCBLoadFile("ToolNumSelect", this, this);
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        if(m_opFrom==10){ //pay
            m_numAll = 10;
            string key = string("gold_buy_more_") + CC_ITOA(m_itemId);
            int number = CCUserDefault::sharedUserDefault()->getIntegerForKey(key.c_str());
            m_numCurrent = (number>0)?number:1; //m_numAll;
            std::string numMax = std::string("/") + CC_ITOA(m_numAll);
            m_numMaxText->setString(numMax);
            auto iter = GlobalData::shared()->goldExchangeList.find(CC_ITOA(m_itemId));
            if(iter!=GlobalData::shared()->goldExchangeList.end()){
                m_infoLabel->setString(_lang("101032"));
                CCCommonUtils::setButtonTitle(m_useBtn, _lang("confirm").c_str());
            }
        }else{
            ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
            m_numAll = info.getCNT();
            if(m_opFrom == 1 && m_numAll > 15){
                m_numAll = 15;
            }
            m_numCurrent = 1; //m_numAll;
            std::string numMax = std::string("/") + CC_ITOA(m_numAll);
            m_numMaxText->setString(numMax);
            if (m_opFrom == 1) {
                m_infoLabel->setString(_lang("104958"));
                CCCommonUtils::setButtonTitle(m_useBtn, _lang("confirm").c_str());
            }else{
                m_infoLabel->setString(_lang_1("104955",info.getName().c_str()));
                CCCommonUtils::setButtonTitle(m_useBtn, _lang("102137").c_str());
            }
        }
        
        CCSize barSize = m_barNode->getContentSize();
//        auto m_sliderBg = CCLoadSprite::createScale9Sprite("huadongtiao3.png");
//        m_sliderBg->setInsetBottom(5);
//        m_sliderBg->setInsetLeft(5);
//        m_sliderBg->setInsetRight(5);
//        m_sliderBg->setInsetTop(5);
//        m_sliderBg->setAnchorPoint(ccp(0.5,0.5));
//        m_sliderBg->setContentSize(CCSize(barSize.width,18));
//        m_sliderBg->setPosition(ccp(barSize.width*0.5,barSize.height*0.5+9));
//        
//        auto proSp = CCLoadSprite::createSprite("huadongtiao2.png");
//        auto thuSp = CCLoadSprite::createSprite("huadongtiao1.png");
//    
//        
//        m_slider = CCSliderBar::createSlider(m_sliderBg, proSp, thuSp);

//        m_slider->setValue(minVal);
//        m_slider->setProgressScaleX(barSize.width/proSp->getContentSize().width);
//        m_slider->setTag(1);
//        m_slider->setLimitMoveValue(20);
//        m_slider->setPosition(ccp(barSize.width*0.5,barSize.height*0.5));
//        m_slider->addTargetWithActionForControlEvents(this, cccontrol_selector(ToolNumSelectView::moveSlider), CCControlEventValueChanged);
//        m_barNode->addChild(m_slider);
        
        m_slider = NBSlider::create("nb_bar_bg.png", "nb_bar_pro.png", "nb_cursor_icon.png",NBSlider::TextureResType::PLIST);
        m_slider->setCapInsets(Rect(8, 1, 30, 13));
        m_slider->setContentSize(Size(barSize.width,15));
        
        float minVal = m_numCurrent*1.0/m_numAll;
        m_slider->setMinimumValue(minVal);
        m_slider->setMaximumValue(1.0f);
        m_slider->setValue(minVal);
        //    m_slider->setPosition(ccp(-60, -59));//fusheng d
        if (CCCommonUtils::isIosAndroidPad()) {
            //        m_slider->setPosition(ccp(-137, -56));//fusheng d
            m_slider->setScaleX(2.6);
            m_slider->setScaleY(2.0);
        }
        //    m_slider->addTargetWithActionForControlEvents(this, cccontrol_selector(ProductionSoldiersView::moveSlider), CCControlEventValueChanged);//fusheng d
        m_slider->addEventListener(CC_CALLBACK_2(ToolNumSelectView::moveSlider, this));
        m_barNode->addChild(m_slider, 1);
        
        
        m_invalidSlider = false;
        
        auto editSize = m_editNode->getContentSize();
        auto editpic =CCLoadSprite::createScale9Sprite("frame_text2.png");
        editpic->setInsetBottom(8);
        editpic->setInsetTop(8);
        editpic->setInsetRight(10);
        editpic->setInsetLeft(10);
        editpic->setPreferredSize(editSize);
        m_editBox = CCEditBox::create(editSize,editpic);
        m_editBox->setInputMode(kEditBoxInputModeNumeric);
        m_editBox->setText(CC_ITOA(m_numCurrent));
        m_editBox->setDelegate(this);
//        m_editBox->setTouchPriority(Touch_Popup);
        m_editBox->setMaxLength(12);
        m_editBox->setReturnType(kKeyboardReturnTypeDone);
        m_editBox->setPosition(ccp(editSize.width/2, editSize.height/2));
//        m_editBox->setTouchPriority(Touch_Default);
        m_editNode->addChild(m_editBox);
        
        ret = true;
    }
    return ret;
}

void ToolNumSelectView::onEnter() {
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void ToolNumSelectView::onExit() {
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

void ToolNumSelectView::onUseClick(CCObject * pSender, Control::EventType pCCControlEvent) {
    if(m_numCurrent > 0) {
        switch(m_opFrom){
            case 1:{
                CCArray *array = CCArray::create();
                array->retain();
                array->addObject(CCInteger::create(m_itemId));
                array->addObject(CCInteger::create(m_numCurrent));
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("msg.mail.gift.choose",array);
                break;
            }
            case 10:{
                string key = string("gold_buy_more_") + CC_ITOA(m_itemId);
                CCUserDefault::sharedUserDefault()->setIntegerForKey(key.c_str(),m_numCurrent);
                PayController::getInstance()->setPayNumber(m_itemId,m_numCurrent);
                break;
            }
            default:{
                ToolController::getInstance()->useTool(m_itemId, m_numCurrent, true);
                break;
            }
        }
    }
    this->closeSelf();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("addWaitInterface");
}

SEL_CCControlHandler ToolNumSelectView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onUseClick", ToolNumSelectView::onUseClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddClick", ToolNumSelectView::onAddClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSubClick", ToolNumSelectView::onSubClick);
    return NULL;
}

bool ToolNumSelectView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel", CCLabelIF*, m_infoLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numMaxText", CCLabelIF*, m_numMaxText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_useBtn", CCControlButton*, m_useBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoNode", CCNode*, m_infoNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editNode", CCNode*, m_editNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barNode", CCNode*, m_barNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subBtn", CCControlButton*, m_subBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addBtn", CCControlButton*, m_addBtn);
    return false;
}

bool ToolNumSelectView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}
void ToolNumSelectView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_touchNode, pTouch)) {
        return;
    }
    if(m_numCurrent > 0) {
        switch(m_opFrom){
            case 10:{
                PayController::getInstance()->resetPayState();
                break;
            }
            default:{
                break;
            }
        }
    }
    this->closeSelf();
}

void ToolNumSelectView::moveSlider(Ref *pSender, NBSlider::EventType type) {
    if (m_invalidSlider) {
        m_invalidSlider = false;
        return;
    }
    float percent = MAX(m_slider->getValue(),m_slider->getMinimumValue());
    percent = MIN(percent, m_slider->getMaximumValue());
    int num = round(percent * m_numAll);
    m_numCurrent = num;
    string tmp = CC_ITOA(m_numCurrent);
    m_editBox->setText(tmp.c_str());
}

void ToolNumSelectView::editBoxTextChanged(CCEditBox* editBox, const std::string& text){
    std::string temp  =text;
    size_t pos = temp.find_first_not_of("1234567890");
    while (pos!=std::string::npos) {
        temp.replace(pos, 1, "");
        pos = temp.find_first_not_of("1234567890");
    }
    int a = atoi(temp.c_str());
    editBox->setText(CC_ITOA(a));
}
void ToolNumSelectView::editBoxReturn(CCEditBox *editBox){
    string numStr = m_editBox->getText();
    int num = atoi(numStr.c_str());
    m_numCurrent = MAX(MIN(num, m_numAll), 1);
    float percent = m_numCurrent * 1.0f / m_numAll;
    m_slider->setValue(percent);
}
void ToolNumSelectView::onAddClick(CCObject * pSender, Control::EventType pCCControlEvent){
    m_numCurrent++;
    m_numCurrent = MAX(MIN(m_numCurrent, m_numAll), 1);
    setNumString();
}
void ToolNumSelectView::onSubClick(CCObject * pSender, Control::EventType pCCControlEvent){
    m_numCurrent--;
    m_numCurrent = MAX(MIN(m_numCurrent, m_numAll), 1);
    setNumString();
}
void ToolNumSelectView::setNumString(){
    float percent = m_numCurrent * 1.0f / m_numAll;
    m_slider->setValue(percent);
    m_editBox->setText(CC_ITOA(m_numCurrent));
}


