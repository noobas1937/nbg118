//
//  ItemStatusView.cpp
//  IF
//
//  Created by ganxiaohua on 14-4-18.
//
//

#include "ItemStatusView.h"
#include "PopupViewController.h"
#include "UseItemStatusView.h"
#include "ShowStatusItemCommand.h"
#include "ToolController.h"
#include "WorldController.h"
#include "FunBuildController.h"
#include "ParticleController.h"

#define STATUS_MAX_COUNT 16
const char* typeName[STATUS_MAX_COUNT] = {"101400","101401","101402","101403","101404","101405","101412","101406","101407","101408","101409","101410","101411","101413","101414","101415"};
const char* typedesc[STATUS_MAX_COUNT] = {"101440","101421","101422","101423","101424","101425","101434","101441","101427","101428","101429","101430","101431","101442","101436","101437"};
int goodType[STATUS_MAX_COUNT] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};//对应物品的状态和技能的状态,14、15为技能状态

//const char* descriptionStr[16] = {"101443","101444","101445","101446","","101447","101449","101448","","","","","","101450","",""};
const char* descriptionStr[16] = {"137435","137432","137433","101446","","101447","101449","137434","","","","","","101450","",""};

ItemStatusView* ItemStatusView::create(){
    ItemStatusView* ret = new ItemStatusView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool ItemStatusView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
        CCLoadSprite::doResourceByCommonIndex(7, false);

    });
    auto tmpCCB = CCBLoadFile("ItemStatusView02",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else
        this->setContentSize(tmpCCB->getContentSize());
    this->setTitleName(_lang("102282").c_str());
    
    if (!CCCommonUtils::isIosAndroidPad()) {
        int oldBgHeight = m_viewBg->getContentSize().height;
        changeBGMaxHeight(m_viewBg);
        int newBgHeight = m_viewBg->getContentSize().height;
        int addHeight = newBgHeight - oldBgHeight;
        int oldWidth = m_infoList->getContentSize().width;
        int oldHeight = m_infoList->getContentSize().height;
        m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
        m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
    }
    
    m_scrollView = CCScrollView::create(m_infoList->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_infoList->addChild(m_scrollView);
    
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Popup);
    m_tabView->setDelegate(this);
    m_infoList->addChild(m_tabView);
    
//    onEnterUpdate();
//    GlobalData::shared()->statusMap.find(tmpSid) != GlobalData::shared()->statusMap.end()
    
    
    
    
    return true;
}

CCArray* ItemStatusView::getTypeArray(int type){
    int city_lv = FunBuildController::getInstance()->getMainCityLv();
    auto goodDic = LocalController::shared()->DBXMLManager()->getGroupByKey("goods");
    CCArray* array = new CCArray();
    array->init();
    int num = 0;
    if(goodDic)
    {
        CCDictElement* element;
        CCDICT_FOREACH(goodDic, element)
        {
            CCDictionary* dictInfo = _dict(element->getObject());
            if(type==dictInfo->valueForKey("type2")->intValue() && dictInfo->valueForKey("type")->intValue()==4 && city_lv>=dictInfo->valueForKey("lv")->intValue()){
                array->addObject(dictInfo);
            }
        }
    }
    num = array->count();
    for (int i=0; i<num; i++) {
        for (int j=i; j<num; j++) {
            CCDictionary* dict1 = _dict(array->objectAtIndex(i));
            CCDictionary* dict2 = _dict(array->objectAtIndex(j));
            if(dict1->valueForKey("order_num")->intValue()<dict2->valueForKey("order_num")->intValue()){
                array->swap(i, j);
            }
        }
    }
    //自定义装扮，添加取消装扮
    if(type == 100 && !ToolController::getInstance()->getMaincitySkin().empty()){
        CCDictionary *dict = CCDictionary::create();
        dict->setObject(CCString::create("customskin"), "customskin");
        array->addObject(dict);
    }
    return array;
}
void ItemStatusView::onEnterUpdate(){
    m_goodTypes.clear();
    vector<int> tmpGTypes;
    tmpGTypes.push_back(99);
    tmpGTypes.push_back(0);
    tmpGTypes.push_back(1);
    tmpGTypes.push_back(2);
    tmpGTypes.push_back(3);
    //    tmpGTypes.push_back(4);
    tmpGTypes.push_back(5);
    tmpGTypes.push_back(6);
    tmpGTypes.push_back(7);
    tmpGTypes.push_back(13);
    //    tmpGTypes.push_back(8);
    //    tmpGTypes.push_back(11);
    
    int mlv = FunBuildController::getInstance()->getMainCityLv();
    if (mlv>=FunBuildController::getInstance()->building_base_k4) {
        //        tmpGTypes.push_back(9);
    }
    if (mlv>=FunBuildController::getInstance()->building_base_k3) {
        //        tmpGTypes.push_back(10);
    }
    
    ToolController::getInstance()->m_typeItems.clear();
    if(ToolController::getInstance()->m_typeItems.size()<=0){
        for(int i=0;i<tmpGTypes.size();i++){
            int type = tmpGTypes[i]+1;
            ToolController::getInstance()->m_typeItems[type] = getTypeArray(type);
            if (ToolController::getInstance()->m_typeItems[type]->count()>0) {
                m_goodTypes.push_back(type-1);
            }
        }
    }
    //从其他敌方取得状态值
    map<int, double> ::iterator it=GlobalData::shared()->statusMap.find(501051);
    double time = GlobalData::shared()->getWorldTime();
    if(it!=GlobalData::shared()->statusMap.end()){
        if(it->second>time){
            m_goodTypes.push_back(14);
        }
    }
    it=GlobalData::shared()->statusMap.find(501050);
    if(it!=GlobalData::shared()->statusMap.end()){
        if(it->second>time){
            m_goodTypes.push_back(15);
        }
    }
    ShowStatusItemCommand* cmd = new ShowStatusItemCommand();
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ItemStatusView::updateInfo), NULL));
    cmd->sendAndRelease();
}
void ItemStatusView::updateInfo(CCObject* p)
{
    m_tabView->reloadData();
}

CCSize ItemStatusView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_goodTypes.size()){
        return CCSizeZero;
    }
    int tmpInt = m_goodTypes[idx];
    if (tmpInt==0 || tmpInt==7) {
//        return CCSize(610,200);
        if (CCCommonUtils::isIosAndroidPad()) {
            return CCSize(1496, 300);
        }
        return CCSize(610,155);
    }
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1496, 300);
    }
    return CCSize(610, 155);
}
CCSize ItemStatusView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1496, 300);
    }
    return CCSize(610, 155);
}
CCTableViewCell* ItemStatusView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_goodTypes.size()){
        return NULL;
    }
    NewItemStatusTypeCell* cell = (NewItemStatusTypeCell*)table->dequeueCell();
    if(cell){
//        cell->setType(m_goodTypes[idx]);
        cell->setData(m_goodTypes[idx], m_infoList);
    }else{;
        cell = NewItemStatusTypeCell::create(m_goodTypes[idx],m_infoList);
    }
    return cell;
}
ssize_t ItemStatusView::numberOfCellsInTableView(CCTableView *table)
{
    return m_goodTypes.size();;
}
void ItemStatusView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}

void ItemStatusView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    auto newItemCell = dynamic_cast<NewItemStatusTypeCell*>(cell);
    if(newItemCell&&newItemCell->mCell){
        ItemStatusTypeCell* itemCell = newItemCell->mCell;
        if(itemCell->m_type==14||itemCell->m_type==15){
            return;
        }
        string typeNameS = "";
        if (itemCell->m_type == 99) {
            typeNameS = _lang("101455");
        }else if(itemCell->m_type < STATUS_MAX_COUNT){
            typeNameS = _lang(typeName[itemCell->m_type]);
        }
        string description = "";
        if(itemCell->m_type == 99){
            description = _lang("101457");
        }else if(itemCell->m_type < STATUS_MAX_COUNT){
            description = _lang(descriptionStr[itemCell->m_type]);
        }
        PopupViewController::getInstance()->addPopupInView(UseItemStatusView::create(itemCell->m_type+1,
                                                                                     typeNameS,
                                                                                     description
                                                                                     ));
    }
}

void ItemStatusView::onEnter()
{
    CCNode::onEnter();
    this->setTitleName(_lang("102282").c_str());
    onEnterUpdate();
}

void ItemStatusView::onExit()
{
    CCNode::onExit();
}

SEL_CCControlHandler ItemStatusView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool ItemStatusView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg", CCScale9Sprite*, this->m_viewBg);
    
    return false;
}


ItemStatusTypeCell* ItemStatusTypeCell::create(int type,CCNode* pNode)
{
    auto ret = new ItemStatusTypeCell(type,pNode);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ItemStatusTypeCell::onEnter() {
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ItemStatusTypeCell::resetTime), MSG_ITME_STATUS_TIME_CHANGE, NULL);
    freshData();
    this->schedule(schedule_selector(ItemStatusTypeCell::updateTime), 1);
}

void ItemStatusTypeCell::onExit() {
    this->unschedule(schedule_selector(ItemStatusTypeCell::updateTime));
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ITME_STATUS_TIME_CHANGE);
    CCNode::onExit();
}

void ItemStatusTypeCell::resetTime(CCObject* param){
    CCInteger* integer = (CCInteger*)param;
    if(integer->getValue()==(m_type+1)){
        freshData();
        this->unschedule(schedule_selector(ItemStatusTypeCell::updateTime));
        this->schedule(schedule_selector(ItemStatusTypeCell::updateTime), 1);
    }
}

bool ItemStatusTypeCell::init()
{
    CCLoadSprite::doResourceByCommonIndex(504, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(504, false);
    });

    CCBLoadFile("ItemStatusTypeCell02",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        setContentSize(CCSizeMake(1496, 300));
    }
    else
        setContentSize(CCSizeMake(605, 155));
    setType(m_type);
    ignoreAnchorPointForPosition(true);
//    m_nameTxt->setString(_lang(typeName[m_type]));
//    m_descTxt->setString(_lang(typedesc[m_type]));
//    CCArray* array = ToolController::getInstance()->m_typeItems[m_type+1];
//    if(array && array->count()>0){
//        CCDictionary* dic = _dict(array->objectAtIndex(0));
//        if(dic){
//            int itemId = dic->valueForKey("id")->intValue();
//            ToolInfo& toolInfo = ToolController::getInstance()->getToolInfoById(itemId);
//            if(&toolInfo){
//                auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(toolInfo.color).c_str());
//                CCCommonUtils::setSpriteMaxSize(iconBg, 92, true);
//                m_iconNode->addChild(iconBg);
//                auto pic = CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(toolInfo.itemId)).c_str());
//                if (pic) {
//                    CCCommonUtils::setSpriteMaxSize(pic, 92, true);
//                    m_iconNode->addChild(pic);
//                }
//            }
//        }
//    }
//    
//    if (m_type==0 || m_type==7) {
//        m_cellBG->setPreferredSize(CCSizeMake(604, 200));
//        m_mainNode->setPositionY(40);
//        m_barNode->setPositionY(-3);
//    }
//    else {
//        m_cellBG->setPreferredSize(CCSizeMake(604, 155));
//        m_mainNode->setPositionY(0);
//        m_barNode->setPositionY(40.2);
//    }
    return true;
}

void ItemStatusTypeCell::setType(int type){
    m_type = type;
    m_iconNode->removeAllChildrenWithCleanup(true);
    this->m_sprArrow->setVisible(true);
    string typeNameS = "";
    if (m_type == 99) {
        typeNameS = _lang("101455");
    }else if(m_type < STATUS_MAX_COUNT){
        typeNameS = _lang(typeName[m_type]);
    }
    string descS = "";
    if (m_type == 99) {
        descS = _lang("101456");
    }else if(m_type < STATUS_MAX_COUNT){
        descS = _lang(typedesc[m_type]);
    }
    m_nameTxt->setString(typeNameS);
    m_descTxt->setString(descS);
    if(m_type==14||m_type==15 || m_type == 99){
        if(m_type==14||m_type==15){
            this->m_sprArrow->setVisible(false);
        }
        auto iconBg = CCLoadSprite::createSprite("tool_2.png");
        if (CCCommonUtils::isIosAndroidPad()) {
            CCCommonUtils::setSpriteMaxSize(iconBg, 140);
        }
        else
            CCCommonUtils::setSpriteMaxSize(iconBg, 92);
        m_iconNode->addChild(iconBg);
        string path = "";
        if(m_type==14){
            path ="501051_status.png";
        }else if(m_type==15){
            path ="501050_status.png";
        }else if(m_type == 99){
            path ="cityskin_status.png";
        }
        auto pic = CCLoadSprite::createSprite(path.c_str());
        if (pic) {
            if (CCCommonUtils::isIosAndroidPad()) {
                CCCommonUtils::setSpriteMaxSize(pic, 140);
            }
            else
                CCCommonUtils::setSpriteMaxSize(pic, 92);
            m_iconNode->addChild(pic);
        }
        
    }else{
        CCArray* array = ToolController::getInstance()->m_typeItems[m_type+1];
        if(array && array->count()>0){
            CCDictionary* dic = _dict(array->objectAtIndex(0));
            if(dic){
                int itemId = dic->valueForKey("id")->intValue();
                if (CCCommonUtils::isIosAndroidPad()) {
                    CCCommonUtils::createGoodsIcon(itemId, m_iconNode, CCSize(140, 140));
                }
                else
                    CCCommonUtils::createGoodsIcon(itemId, m_iconNode, CCSize(92, 92));
//                ToolInfo& toolInfo = ToolController::getInstance()->getToolInfoById(itemId);
//                if(&toolInfo){
//                    auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(toolInfo.color).c_str());
//                    CCCommonUtils::setSpriteMaxSize(iconBg, 92);
//                    m_iconNode->addChild(iconBg);
//                    auto pic = CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(toolInfo.itemId)).c_str(),true,CCLoadSpriteType_GOODS);
//                    if (pic) {
//                        CCCommonUtils::setSpriteMaxSize(pic, 92);
//                        m_iconNode->addChild(pic);
//                    }
//                }
            }
        }
    }

    if (m_barNode->getChildByTag(100)) {
        m_barNode->removeChildByTag(100);
    }
    this->getAnimationManager()->runAnimationsForSequenceNamed("loop");
    
    //加进度条头上的光效
    m_headParticleNode = CCNode::create();
    for (int i=1; i<=3; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("Loading_%d",i)->getCString());
        m_headParticleNode->addChild(particle);
    }
    m_barNode->addChild(m_headParticleNode);
    m_headParticleNode->setTag(100);
    
    freshData();
    this->unschedule(schedule_selector(ItemStatusTypeCell::updateTime));
    this->schedule(schedule_selector(ItemStatusTypeCell::updateTime), 1);
    
    if (m_type==0 || m_type==7) {
//        m_cellBG->setPreferredSize(CCSizeMake(604, 200));
        if (CCCommonUtils::isIosAndroidPad()) {
            m_cellBG->setPreferredSize(CCSizeMake(1496, 300));
        }
        else
            m_cellBG->setPreferredSize(CCSizeMake(604, 155));
//        m_mainNode->setPositionY(40);
//        m_barNode->setPositionY(-3);
        
        if (!CCCommonUtils::isIosAndroidPad()) {
            m_mainNode->setPositionY(0);
            m_barNode->setPositionY(40.2);
        }
    }else if(m_type==15){
        if (!CCCommonUtils::isIosAndroidPad()) {
            m_barNode->setPositionY(30);
        }
    }
    else {
        if (CCCommonUtils::isIosAndroidPad()) {
            m_cellBG->setPreferredSize(CCSizeMake(1496, 300));
        }
        else
            m_cellBG->setPreferredSize(CCSizeMake(604, 155));
        if (!CCCommonUtils::isIosAndroidPad()) {
            m_mainNode->setPositionY(0);
            m_barNode->setPositionY(40.2);
        }
        
    }
}

void ItemStatusTypeCell::freshData(){
    m_time = 0;
    int max = 0;
//    if(m_type>100){//不是物品产生的状态
//        map<int, double> ::iterator it=GlobalData::shared()->statusMap.find(501051);
//        double time = GlobalData::shared()->getWorldTime();
//    }else{
        map<int, CCDictionary* >::iterator it = ToolController::getInstance()->m_statusItems.find(m_type+1);
        if(it!=ToolController::getInstance()->m_statusItems.end()){
            m_time =(it->second->valueForKey("endTime")->doubleValue() - WorldController::getInstance()->getTime())/1000;
            max = (it->second->valueForKey("endTime")->doubleValue() - it->second->valueForKey("startTime")->doubleValue())/1000;
        }
//    }

    if(m_time>0){
        m_barNode->setVisible(true);
        max = MAX(max, 1);
        float len = m_time*1.0/max;
        if(len>1) len = 1;
        m_progress->setScaleX(len);
        m_barRight->setVisible(len>0.99);//右箭头
        m_timeTxt->setString(_lang_1("105805", CC_SECTOA(m_time)));
        m_receiveGlow->setVisible(true);
        float px=0;//进度条头上的光效位置
        px=m_progress->getPositionX()+(m_progress->getContentSize().width*len);
        m_headParticleNode->setPositionX(px);
    }else{
        m_progress->setScaleX(0);
        m_timeTxt->setString(_lang_1("105805", CC_SECTOA(0)));
        m_barNode->setVisible(false);
        m_receiveGlow->setVisible(false);
        this->unschedule(schedule_selector(ItemStatusTypeCell::updateTime));
    }
}

void ItemStatusTypeCell::updateTime(float t){
    freshData();
}


SEL_CCControlHandler ItemStatusTypeCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}

bool ItemStatusTypeCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_receiveGlow", CCNode*, this->m_receiveGlow);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_descTxt", CCLabelIF*, this->m_descTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barNode", CCNode*, this->m_barNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprArrow", CCSprite*, this->m_sprArrow);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progress", CCScale9Sprite*, this->m_progress);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cellBG", CCScale9Sprite*, this->m_cellBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode", CCNode*, this->m_clickNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barRight", CCSprite*, this->m_barRight);
    return false;
}

//class NewItemStatusTypeCell
NewItemStatusTypeCell* NewItemStatusTypeCell::create(int type,CCNode* pNode)
{
    NewItemStatusTypeCell* ret = new NewItemStatusTypeCell();
    if (ret && ret->init(type, pNode))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool NewItemStatusTypeCell::init(int type,CCNode* pNode)
{
    this->setData(type, pNode);
    return true;
}

void NewItemStatusTypeCell::setData(int type,CCNode* pNode)
{
    if (mCell == NULL)
    {
        mCell = ItemStatusTypeCell::create(type, pNode);
        addChild(mCell);
    }
    else
    {
        mCell->setType(type);
    }
}

