//
//  LongJingStoreView.cpp
//  IF
//
//  Created by zhangjun-zj on 15-8-14.
//
//

#include "StoreView.h"
#include "LongJingStoreView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "YesNoDialog.h"
#include "ToolController.h"
#include "UseCDToolView.h"
#include "YesNoDialog.h"
#include "FunBuildController.h"
#include "ChangeNickNameView.h"
#include "WorldMapView.h"
#include "ChangePicView.h"
#include "BuildingAddOutPutCommand.h"
#include "SceneController.h"
#include "LongJingBuyView.h"
#include "StoreMallCellTip.h"
#include "ParticleController.h"

static int advertiseCellW = 540;
const float numPerRow = 1.0;
LongJingStoreView* LongJingStoreView::create(StoreType storeType, StoreResourceType storeResourceType){
    LongJingStoreView* ret = new LongJingStoreView(storeType, storeResourceType);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool LongJingStoreView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(11, true);
    CCLoadSprite::doResourceByCommonIndex(8, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::doResourceByCommonIndex(8, false);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
    });
    auto tmpCCB = CCBLoadFile("LongJingStoreView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
    setTitleName(_lang("102160").c_str());
    
    int oldBgHeight = m_buildBG->getContentSize().height;
    changeBGHeight(m_buildBG);
    int newBgHeight = m_buildBG->getContentSize().height;
    addHeight = newBgHeight - oldBgHeight;
    int oldWidth = m_infoList->getContentSize().width;
    int oldHeight = m_infoList->getContentSize().height;
    m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
    m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));

    auto size = CCDirector::sharedDirector()->getWinSize();
    int count = size.height/44+1;
    for (int i=0; i<count; i++) {
        auto spr = CCLoadSprite::createSprite("Items_bg_big.png");
        if (CCCommonUtils::isIosAndroidPad()){
            spr->setScaleX(1.2);
        }
        this->m_BGNode->addChild(spr);
        spr->setPositionY(-i*44);
    }
    
//    int bgcount = newBgHeight/100+1;
//    for (int i = 0; i<=bgcount; i++) {
//        auto pic = CCLoadSprite::createSprite("technology_09.png");
//        pic->setPositionY(-i*100);
//        pic->setScaleX(0.98);
//        if (CCCommonUtils::isIosAndroidPad())
//        {
//            pic->setPositionY((i-3)*100);
//            pic->setScaleX(2.4f);
//            pic->setAnchorPoint(ccp(0, 1));
//        }
//        m_BGNode->addChild(pic);
//    }
    
    m_data = CCArray::create();
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    m_tabView->setDelegate(this);
    m_infoList->addChild(m_tabView);
    
    m_cityLv = FunBuildController::getInstance()->getMainCityLv();
    
//    m_mallView = StoreMallView::create();
//    float dy = 852-170;
//    if(CCCommonUtils::isIosAndroidPad())
//        dy = 2048-340;
//    m_mallView->setPositionY(-dy);

    updateInfo();
    
    m_desNode->setVisible(false);
    
    return true;
}
void LongJingStoreView::updateInfo()
{
//    m_mallNode->addChild(m_mallView);
    generateData();
    m_tabView->reloadData();
}

CCSize LongJingStoreView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return CCSizeZero;
    }
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(advertiseCellW, 300);
    }
    return CCSize(advertiseCellW, 200);
}

CCSize LongJingStoreView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(advertiseCellW, 300);
    }
    return CCSize(advertiseCellW, 200);
}

CCTableViewCell* LongJingStoreView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    
    string itemId = dynamic_cast<CCString*>(m_data->objectAtIndex(idx))->getCString();
    vector<string> vector1;
    CCCommonUtils::splitString(itemId, ":", vector1);
    if(vector1.size()==2){
        string item1=vector1[0];
        string item2=vector1[1];
        LongJingCell* cell = (LongJingCell*)table->dequeueCell();
        if(cell){
            cell->setData(atoi(item1.c_str()),atoi(item2.c_str()));
        }else{
            cell = LongJingCell::create(atoi(item1.c_str()),atoi(item2.c_str()),m_infoList);
        }
        cell->delegate=this;
        return cell;
    }
    return NULL;
}

ssize_t LongJingStoreView::numberOfCellsInTableView(CCTableView *table)
{
    int num = ceil(m_data->count() / 1.0f);
    return num;
}

void LongJingStoreView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
    
}

void LongJingStoreView::scrollViewDidScroll(CCScrollView* view){
    //    CCLOG("%f", view->getContentOffset().x);
    float mindy = m_tabView->minContainerOffset().y-20;
    float dy = m_tabView->getContentOffset().y;
    if(dy<mindy){
        m_tabView->setContentOffset(ccp(0, mindy));
    }
}

void LongJingStoreView::onEnter()
{
    CCNode::onEnter();
    setTouchEnabled(true);
}

void LongJingStoreView::onExit()
{
    setTouchEnabled(false);
    CCNode::onExit();
}

LongJingStoreView::~LongJingStoreView()
{
}

SEL_CCControlHandler LongJingStoreView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickHelpBtn", LongJingStoreView::onClickHelpBtn);
    return NULL;
}

bool LongJingStoreView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mallNode", CCNode*, this->m_mallNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_BGNode", CCNode*, this->m_BGNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode", CCNode*, this->m_desNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desbg", CCScale9Sprite*, this->m_desbg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desName", CCLabelIF*, this->m_desName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLine1", CCSprite*, this->m_desLine1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_valueNode", CCNode*, this->m_valueNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNum1", CCLabelIF*, this->m_desNum1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNum2", CCLabelIF*, this->m_desNum2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNum3", CCLabelIF*, this->m_desNum3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNum4", CCLabelIF*, this->m_desNum4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNum5", CCLabelIF*, this->m_desNum5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desPara1", CCLabelIF*, this->m_desPara1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desPara2", CCLabelIF*, this->m_desPara2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desPara3", CCLabelIF*, this->m_desPara3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desPara4", CCLabelIF*, this->m_desPara4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desPara5", CCLabelIF*, this->m_desPara5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLine2", CCSprite*, this->m_desLine2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLv", CCLabelIF*, this->m_desLv);
    return false;
}

void LongJingStoreView::showTipNode(int itemId,CCTouch *pTouch){
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(itemId);
    
    string itemName = info.getName();
    m_desName->setString(itemName);
    string limitLv = "Lv:";
    limitLv+=CC_ITOA(info.limitLv);
    m_desLv->setString(limitLv);
    
    string show1 = CCCommonUtils::getPropById(info.equipId, "show1");
    string num1 = CCCommonUtils::getPropById(info.equipId, "num1");
    
    vector<string> sVec;
    CCCommonUtils::splitString(show1, "|", sVec);
    vector<string> nVec;
    CCCommonUtils::splitString(num1, "|", nVec);
    
    m_desPara1->setString("");
    m_desPara2->setString("");
    m_desPara3->setString("");
    m_desPara4->setString("");
    m_desPara5->setString("");
    m_desNum1->setString("");
    m_desNum2->setString("");
    m_desNum3->setString("");
    m_desNum4->setString("");
    m_desNum5->setString("");
    
    float nodeW = m_desNode->getContentSize().width;
    float nodeH = 230;
    float cNodeH = nodeH;
    m_desNode->setContentSize(CCSize(nodeW, nodeH));
    m_desbg->setContentSize(CCSize(nodeW, nodeH));
    m_desName->setPositionY(211);
    m_desLine1->setPositionY(195);
    m_valueNode->setPositionY(146);
    bool gotoflag=true;
    if (sVec.size()>0 && num1.size()>0) {
        m_desPara1->setString(_lang(sVec[0].c_str()));
        m_desNum1->setString( "+"+nVec[0]+"%");
    }
    if (info.color>=2) {
        m_desPara2->setString(_lang("111655").c_str());
        m_desNum2->setString( "+???");
    }else if(gotoflag){
        m_desName->setColor({83,151,33});
        cNodeH = 105;
        gotoflag=false;
    }
    
    if (info.color>=3 && sVec.size()>1 && num1.size()>1) {
        m_desPara3->setString(_lang(sVec[1].c_str()));
        m_desNum3->setString( "+"+nVec[1]+"%");
    }else if(gotoflag){
        m_desName->setColor({71,132,225});
        cNodeH = 135;
        gotoflag=false;
    }
    
    if (info.color>=4) {
        m_desPara4->setString(_lang("111655").c_str());
        m_desNum4->setString( "+???");
    }else if(gotoflag){
        m_desName->setColor({112,72,194});
        cNodeH = 165;
        gotoflag=false;
    }
    
    if (info.color>=5 && sVec.size()>2 && num1.size()>2) {
        m_desPara5->setString(_lang(sVec[2].c_str()));
        m_desNum5->setString( "+"+nVec[2]+"%");
        m_desName->setColor({205,183,63});
    }else if(gotoflag){
        m_desName->setColor({193,129,72});
        cNodeH = 195;
        gotoflag=false;
    }
    m_desNode->setContentSize(CCSize(nodeW, cNodeH));
    m_desName->setPositionY(m_desName->getPositionY()-(nodeH-cNodeH));
    m_desLine1->setPositionY(m_desLine1->getPositionY()-(nodeH-cNodeH));
    m_valueNode->setPositionY(m_valueNode->getPositionY()-(nodeH-cNodeH));
    m_desbg->setContentSize(m_desNode->getContentSize());
    
    CCPoint tp = pTouch->getStartLocation();
    int dH=addHeight;
    if (CCCommonUtils::isIosAndroidPad())
    {
        dH=0;
        nodeW*=2;
    }
    m_desNode->setPositionY(tp.y-dH);
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float px = tp.x<(size.width-nodeW)?tp.x:(size.width-nodeW);
    m_desNode->setPositionX(px);
    m_desNode->setVisible(true);
    return;
    
}

void LongJingStoreView::hideTipNode(){
    
}

bool LongJingStoreView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (!isTouchInside(m_desNode,pTouch)) {
        if (m_desNode->isVisible()) {
            m_desNode->setVisible(false);
        }
    }
    return false;
}
void LongJingStoreView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}

void LongJingStoreView::onClickHelpBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
//    GlobalData::shared()->isBind = true;
//#endif
    CCCommonUtils::showHelpShiftSingleFAQ("45214");
}

void LongJingStoreView::generateData(){
    m_data->removeAllObjects();

//    m_dataList = &ToolController::getInstance()->m_warList;
    m_dataList = &ToolController::getInstance()->m_typeTools[9];
    
    m_curList.clear();
    for (int i=0; i<m_dataList->size(); i++) {
        int tmpToolId = (*m_dataList)[i];
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(tmpToolId);
        if (info.LongJing_price>0 && info.limitLv<=m_cityLv){
             m_curList.push_back(tmpToolId);
         }
    }
    string itemId = "";
    int count = m_curList.size();
    for (int i=0; i<count; i++) {
        int tmpToolId = (m_curList)[i];
        if(i%2==0){
            itemId = CC_ITOA(tmpToolId);
        }else{
            itemId.append(":");
            itemId += CC_ITOA(tmpToolId);
            m_data->addObject(CCString::create(itemId));
            itemId = "";
        }
    }
    if(count%2==1){
        itemId.append(":");
        itemId.append("-1");
        m_data->addObject(CCString::create(itemId));
    }
}

# pragma mark LongJingCell
LongJingCell::LongJingCell():m_itemId(0),m_itemId1(0),m_price(0),m_boostBid(0),m_touchNode(NULL),m_isClick(false){
}
LongJingCell* LongJingCell::create(int itemId, int itemId1, CCNode* touchNode)
{
    auto ret = new LongJingCell();
    if (ret && ret->init(itemId, itemId1, touchNode)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool LongJingCell::init(int itemId, int itemId1, CCNode* touchNode)
{
    bool ret = true;
    CCBLoadFile("StoreMallCell",this,this);
    this->setContentSize(CCSize(606, 200));
    if(CCCommonUtils::isIosAndroidPad())
    {
        this->setContentSize(CCSize(1450, 300));
    }
    m_touchNode = touchNode;
    m_bLayer = CCLayer::create();
    this->addChild(m_bLayer, 91);
    setData(itemId, itemId1);
    return ret;
}

void LongJingCell::setData(int itemId, int itemId1)
{
    m_itemId = itemId;
    m_itemId1 = itemId1;
    m_desNode->setVisible(false);
    m_priceLabel1->setColor({255,255,0});
    m_priceLabel2->setColor({255,255,0});
//    m_goldSpr1->setColor({255,255,255});
//    m_goldSpr2->setColor({255,255,255});
    //龙晶图标
    auto coinSpr1 = CCLoadSprite::createSprite("shuijing.png");
    coinSpr1->setPosition(m_goldSpr1->getPosition());
//    CCCommonUtils::setSpriteMaxSize(coinSpr1, 40);
    if(CCCommonUtils::isIosAndroidPad())
    {
        CCCommonUtils::setSpriteMaxSize(coinSpr1, 40*2);
    }
    else{
        CCCommonUtils::setSpriteMaxSize(coinSpr1, 40);
    }
    m_goldSpr1->getParent()->addChild(coinSpr1);
    m_goldSpr1->setVisible(false);
    auto coinSpr2 = CCLoadSprite::createSprite("shuijing.png");
    coinSpr2->setPosition(m_goldSpr2->getPosition());
    if(CCCommonUtils::isIosAndroidPad())
    {
        CCCommonUtils::setSpriteMaxSize(coinSpr2, 40*2);
    }
    else{
        CCCommonUtils::setSpriteMaxSize(coinSpr2, 40);
    }
    m_goldSpr2->getParent()->addChild(coinSpr2);
    m_goldSpr2->setVisible(false);
    
    m_buyBtngraySpr2->setVisible(false);
    m_buyBtngraySpr1->setVisible(false);
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
    //    m_nameLabel1->setString(_lang(info.name).c_str());
    m_priceLabel1->setString(CC_ITOA_K(info.LongJing_price));
    m_picNode1->removeAllChildren();
    if(CCCommonUtils::isIosAndroidPad())
    {
        CCCommonUtils::createGoodsIcon(m_itemId, m_picNode1, CCSize(92*1.8, 92*1.8),NULL,m_nameLabel1);
    }
    else{
        CCCommonUtils::createGoodsIcon(m_itemId, m_picNode1, CCSize(92, 92),NULL,m_nameLabel1);
    }
    //特效
    m_bLayer->removeAllChildren();
    m_particleL->removeAllChildren();
    
    int part = getParticalNum(info.color);
    for (int par=1; par<=5; par++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("Cristal_%d_%d",part,par)->getCString());
        particle->setScale(2.3f);
        if(CCCommonUtils::isIosAndroidPad())
        {
            particle->setScale(4.8f);
        }
        m_particleL->addChild(particle);
        
//        particle->setPosition(m_particleL->getPosition());
//        addParticleToBatch(particle);
    }
//    m_nameLabel1->setColor({0,255,255});
    
    //is hot
    m_hotpriceLabel1->setString(CC_CMDITOA(info.hotprice));
    if(info.ishot&&info.isShowHot){
        setLeftHot(true,info.hotnum);
        m_priceLabel1->setString(CC_CMDITOA(info.hotprice));
        m_hotpriceLabel1->setString(CC_CMDITOA(info.price_hot));
    }
    else{
        setLeftHot(false,info.hotnum);
    }
    if(itemId1!=-1){
        this->m_itemNode2->setVisible(true);
        ToolInfo& info1 = ToolController::getInstance()->getToolInfoById(m_itemId1);
//        m_nameLabel2->setString(_lang(info1.getName().c_str()));
        m_priceLabel2->setString(CC_ITOA_K(info1.LongJing_price));
        m_picNode2->removeAllChildren();
        if(CCCommonUtils::isIosAndroidPad())
        {
            CCCommonUtils::createGoodsIcon(m_itemId1, m_picNode2, CCSize(92*1.8, 92*1.8),NULL,m_nameLabel2);
        }
        else{
            CCCommonUtils::createGoodsIcon(m_itemId1, m_picNode2, CCSize(92, 92),NULL,m_nameLabel2);
        }
        //特效
        m_particleR->removeAllChildren();
        int part = getParticalNum(info1.color);
        for (int par=1; par<=5; par++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("Cristal_%d_%d",part,par)->getCString());
            particle->setScale(2.3f);
            if(CCCommonUtils::isIosAndroidPad())
            {
                particle->setScale(4.8f);
            }
            m_particleR->addChild(particle);
        }
        // is hot
        m_hotpriceLabel2->setString(CC_CMDITOA(info1.hotprice));
        if(info1.ishot&&info1.isShowHot){
            setRightHot(true, info1.hotnum);
            m_priceLabel2->setString(CC_CMDITOA(info1.hotprice));
            m_hotpriceLabel2->setString(CC_CMDITOA(info1.price_hot));
        }
        else
            setRightHot(false, info1.hotnum);
        
    }else{
        this->m_itemNode2->setVisible(false);
    }
}

void LongJingCell::addParticleToBatch(cocos2d::CCParticleSystemQuad *particle) {
    auto batchCount = m_parVec.size();
    while (batchCount--) {
        auto &batch = m_parVec[batchCount];
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
    m_bLayer->addChild(newBatch);
    newBatch->addChild(particle);
    m_parVec.push_back(newBatch);
}

int LongJingCell::getParticalNum(int color){
    int part=1;
    if(1==color){//绿
        part = 3;
    }else if (2==color){//蓝
        part = 1;
    }else if (3==color){//紫
        part = 4;
    }else if (4==color){//橙
        part = 5;
    }else if (5==color){//金
        part = 2;
    }
    return part;
}

void LongJingCell::onEnter() {
    CCNode::onEnter();
    setTouchEnabled(true);
    ToolController::getInstance()->m_mallCurrItemId=0;
}

void LongJingCell::onExit() {
    setTouchEnabled(false);
    CCNode::onExit();
}

void LongJingCell::refreashData(CCObject* obj)
{
    ToolController::getInstance()->m_mallCurrItemId =0;
    this->m_desNode->setVisible(false);
}

SEL_CCControlHandler LongJingCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}


bool LongJingCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_clickInSide = false;
    m_num = 0;
    if (isTouchInside(m_touchNode,pTouch)) {
        m_clickInSide = true;
        if (isTouchInside(this,pTouch)) {
            m_startPoint=pTouch->getStartLocation();
            if(isTouchInside(m_picBG1,pTouch)){
//                this->schedule(schedule_selector(LongJingCell::setLeft),0.2);
            }else if(isTouchInside(m_picBG2,pTouch)){
//                if(m_itemId1==-1){
//                    return false;
//                }
//                this->scheduleOnce(schedule_selector(LongJingCell::setRight),0.2);
//                
            }else if(isTouchInside(m_buyBtnSpr1,pTouch)){
                ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
                if(info.ishot&&info.isShowHot&&info.hotnum<=0){
                    return false;
                }else{
                    m_buyBtnSpr1->setScale(1.2);
                    
                }
                
            }else if(isTouchInside(m_buyBtnSpr2,pTouch)){
                if(m_itemId1==-1){
                    return false;
                }
                ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId1);
                if(info.ishot&&info.isShowHot&&info.hotnum<=0){
                    return false;
                }else{
                    m_buyBtnSpr2->setScale(1.2);
                }
                
            }
            return true;
        }
    }
    return false;
}
void LongJingCell::setLeft(float dt){
    this->unschedule(schedule_selector(LongJingCell::setLeft));
    setDesNode(m_itemId,0);
}
void LongJingCell::setRight(float dt){
    this->unschedule(schedule_selector(LongJingCell::setRight));
    setDesNode(m_itemId1,1);
}
void LongJingCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_buyBtnSpr1->setScale(1.0);
    m_buyBtnSpr2->setScale(1.0);
    m_desNode->setVisible(false);
    this->unschedule(schedule_selector(LongJingCell::setLeft));
    this->unschedule(schedule_selector(LongJingCell::setRight));
    CCPoint endPoint = pTouch->getLocation();
    if(fabs(endPoint.x-m_startPoint.x)>10||fabs(endPoint.y-m_startPoint.y)>10){
        return;
    }
    if(isTouchInside(m_buyBtnSpr1,pTouch)){
        onClickBuyBtn1(NULL,CCControlEvent::TOUCH_DOWN);
    }else if(isTouchInside(m_buyBtnSpr2,pTouch)){
        onClickBuyBtn2(NULL,CCControlEvent::TOUCH_DOWN);
    }else if(isTouchInside(m_picBG1,pTouch)){
//        onClickBuyBtn1(NULL,NULL);
        delegate->showTipNode(m_itemId, pTouch);
    }else if(isTouchInside(m_picBG2,pTouch)){
        if(m_itemId1==-1){
            return;
        }
//        onClickBuyBtn2(NULL,NULL);
        delegate->showTipNode(m_itemId1, pTouch);
    }
    auto layer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_TIP);
    layer->removeAllChildren();
}


void LongJingCell::setDesNode(int itemId,int index){
    m_desNode->setVisible(true);
    ToolController::getInstance()->m_mallCurrItemId = 99;
    if(index==0){//left
        m_desNode->setPositionX(290);
    }else{
        m_desNode->setPositionX(172);
    }
    if(CCCommonUtils::isIosAndroidPad())
    {
        float _p = m_desNode->getPositionX();
        m_desNode->setPositionX(_p*2);
        if (index!=0)
            m_desNode->setPositionX(_p*4-40);
    }
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(itemId);
    if(info.type==5 && !info.getPara().empty()){
        m_desNode->setVisible(false);
        auto layer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_TIP);
        if(layer->getChildrenCount()<=0){
            StoreMallCellTip* tip = StoreMallCellTip::create(info.getPara(),info.getName());
            layer->addChild(tip);
            float x = m_startPoint.x;
            if(index==1){
                x =100;
            }
            tip->setPosition(x, m_startPoint.y);
        }
    }else{
        string name = info.getName()+" Lv:"+ CC_ITOA(info.limitLv);
        m_desName->setString(name.c_str());
        m_desLabel->setString(info.des);//龙晶道具的des已经处理成txt
    }
    CCSize size = m_desLabel->getContentSize();
    size.width *= m_desLabel->getOriginScaleX();
    size.height *= m_desLabel->getOriginScaleY();
    CCScale9Sprite* bgScale9 = dynamic_cast<CCScale9Sprite*>(m_desNode->getChildByTag(10));
    CCSize psize = bgScale9->getContentSize();
    if(size.height>80){
        if(bgScale9 && psize.height<=149){
            psize.height +=  size.height - 80;
            bgScale9->setPreferredSize(psize);
            bgScale9->setPositionY(bgScale9->getPositionY() - (size.height - 80)*0.5);
            m_desNode->setPositionY(m_desNode->getPositionY() +  (size.height - 80)*0.5);
        }
    }else{
        if(bgScale9){
            bgScale9->setPreferredSize(CCSize(psize.width, 149));
            bgScale9->setPositionY(bgScale9->getPositionY() + (psize.height - 149)*0.5);
            m_desNode->setPositionY(m_desNode->getPositionY()- (psize.height - 149)*0.5);
        }
    }
}
void LongJingCell::onClickBuyBtn1(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    if(!m_clickInSide){
        return;
    }
    if(m_isClick)
        return;
    m_isClick = true;
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
    int price = 0;
//    if(info.ishot&&info.isShowHot)
//        price = info.hotprice;
//    else
    price = info.LongJing_price;
    int cCount = 0;
    if(info.type==3 && !info.getPara().empty()){
        cCount = atoi(info.getPara().c_str());
    }
    LongJingBuyView* dialog = LongJingBuyView::show(CCCommonUtils::getIcon(CC_ITOA(info.itemId)).c_str(), info.getName().c_str(), info.des.c_str(), price,info.color, CCCallFuncO::create(this, callfuncO_selector(LongJingCell::onBuyTool1), NULL),m_picBG1->convertToWorldSpace(CCPointZero),LongJing,cCount);
    if(dialog){
        dialog->setCloseCallback(CCCallFunc::create(this, callfunc_selector(LongJingCell::onCloseConfirm)));
    }
}
void LongJingCell::onClickBuyBtn2(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    if(!m_clickInSide){
        return;
    }
    if(m_isClick)
        return;
    m_isClick = true;
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId1);
    int price = 0;
//    if(info.ishot&&info.isShowHot)
//        price = info.hotprice;
//    else
    price = info.LongJing_price;
    int cCount = 0;
    if(info.type==3 && !info.getPara().empty()){
        cCount = atoi(info.getPara().c_str());
    }
    LongJingBuyView* dialog = LongJingBuyView::show(CCCommonUtils::getIcon(CC_ITOA(info.itemId)).c_str(), info.getName().c_str(), info.des.c_str(), price,info.color, CCCallFuncO::create(this, callfuncO_selector(LongJingCell::onBuyTool2), NULL),m_picBG2->convertToWorldSpace(CCPointZero),LongJing,cCount);
    if(dialog){
        dialog->setCloseCallback(CCCallFunc::create(this, callfunc_selector(LongJingCell::onCloseConfirm)));
    }
}
void LongJingCell::onBuyTool1(CCObject *ccObj)
{
    int buyCount = 1;
    if(ccObj){
        CCInteger* cInt = dynamic_cast<CCInteger*>(ccObj);
        if(cInt){
            buyCount = cInt->getValue();
        }
    }
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
//    if(info.ishot&&info.isShowHot)
//        ToolController::getInstance()->buyHotTool(m_itemId, buyCount, CCCallFunc::create(this, callfunc_selector(LongJingCell::onRetBuyTool)),0,"LongJingStoreView");
//    else
    ToolController::getInstance()->buyLongJingEquip(m_itemId, buyCount, CCCallFunc::create(this, callfunc_selector(LongJingCell::onRetBuyTool)),0,false,true,"LongJingStoreView");
}
void LongJingCell::onBuyTool2(CCObject *ccObj)
{
    int buyCount = 1;
    if(ccObj){
        CCInteger* cInt = dynamic_cast<CCInteger*>(ccObj);
        if(cInt){
            buyCount = cInt->getValue();
        }
    }
    ToolInfo& info1 = ToolController::getInstance()->getToolInfoById(m_itemId1);
//    if(info1.ishot&&info1.isShowHot)
//        ToolController::getInstance()->buyHotTool(m_itemId1,buyCount, CCCallFunc::create(this, callfunc_selector(LongJingCell::onRetBuyTool)),0,"LongJingStoreView");
//    else
    ToolController::getInstance()->buyLongJingEquip(m_itemId1,buyCount, CCCallFunc::create(this, callfunc_selector(LongJingCell::onRetBuyTool)),0,false,true,"LongJingStoreView");
}
void LongJingCell::onRetBuyTool()
{
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
    if(info.ishot&&info.isShowHot)
        setLeftHot(true,info.hotnum);
    
    if(m_itemId1!=-1){
        ToolInfo& info1 = ToolController::getInstance()->getToolInfoById(m_itemId1);
        if(info1.ishot&&info1.isShowHot)
            setRightHot(true, info1.hotnum);
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUY_CONFIRM_OK);
    m_isClick = false;
}

void LongJingCell::onYes()
{
    //    ToolController::getInstance()->buyTool(m_itemId, 1, CCCallFunc::create(this, callfunc_selector(LongJingCell::onUseTool)));
}
void LongJingCell::onCloseConfirm(){
    m_isClick=false;
}


bool LongJingCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel1", CCLabelIF*, m_nameLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel2", CCLabelIF*, m_nameLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode1", CCNode*, m_picNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode2", CCNode*, m_picNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_priceLabel1", CCLabelIF*, m_priceLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_priceLabel2", CCLabelIF*, m_priceLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode1", CCNode*, m_itemNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode2", CCNode*, m_itemNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBG1", CCSprite*, m_picBG1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBG2", CCSprite*, m_picBG2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode", CCNode*, m_desNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desName", CCLabelIF*, m_desName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyBtnSpr1", CCScale9Sprite*, m_buyBtnSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyBtnSpr2", CCScale9Sprite*, m_buyBtnSpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_saleoutSpr1", CCScale9Sprite*, m_saleoutSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_saleoutSpr2", CCScale9Sprite*, m_saleoutSpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyBtngraySpr1", CCScale9Sprite*, m_buyBtngraySpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyBtngraySpr2", CCScale9Sprite*, m_buyBtngraySpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldSpr1", CCSprite*, m_goldSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldSpr2", CCSprite*, m_goldSpr2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemHotNode1", CCNode*, m_itemHotNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotSpr1", CCSprite*, m_hotSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotgraySpr1", CCSprite*, m_hotgraySpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotNumSpr1", CCSprite*, m_hotNumSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotNumgraySpr1", CCSprite*, m_hotNumgraySpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotdesText1", CCLabelIF*, m_hotdesText1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotNum1Text", CCLabelIF*, m_hotNum1Text);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotLineSpr1", CCScale9Sprite*, m_hotLineSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotpriceLabel1", CCLabelIF*, m_hotpriceLabel1);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemHotNode2", CCNode*, m_itemHotNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotSpr2", CCSprite*, m_hotSpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotgraySpr2", CCSprite*, m_hotgraySpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotNumSpr2", CCSprite*, m_hotNumSpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotNumgraySpr2", CCSprite*, m_hotNumgraySpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotdesText2", CCLabelIF*, m_hotdesText2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotNum2Text", CCLabelIF*, m_hotNum2Text);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotLineSpr2", CCScale9Sprite*, m_hotLineSpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotpriceLabel2", CCLabelIF*, m_hotpriceLabel2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleL", CCNode*, m_particleL);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleR", CCNode*, m_particleR);
    return false;
}
void LongJingCell::setLeftHot(bool ishot,int num){
    m_itemHotNode1->setVisible(ishot);
    m_hotpriceLabel1->setVisible(ishot);
    m_hotNum1Text->setString(CC_CMDITOA(num));
    m_hotdesText1->setString(_lang("101224"));
    
    m_hotLineSpr1->setVisible(ishot);
    m_priceLabel1->setPositionY(11);
    if(ishot){
        if(num>0){
            m_buyBtngraySpr1->setVisible(false);
            m_saleoutSpr1->setVisible(false);
            m_hotgraySpr1->setVisible(false);
            m_hotSpr1->setVisible(true);
            m_hotNumSpr1->setVisible(true);
            m_hotNumgraySpr1->setVisible(false);
            m_nameLabel1->setColor({194,158,116});
            m_priceLabel1->setColor({255,255,0});
            m_hotpriceLabel1->setColor({221,172,33});
        }else{
            m_buyBtngraySpr1->setVisible(true);
            m_saleoutSpr1->setVisible(true);
            m_hotNumSpr1->setVisible(false);
            m_hotNumgraySpr1->setVisible(true);
            m_hotgraySpr1->setVisible(true);
            m_hotSpr1->setVisible(false);
            m_nameLabel1->setColor({156,156,156});
            m_priceLabel1->setColor({156,156,156});
            m_hotpriceLabel1->setColor({156,156,156});
            auto pic = dynamic_cast<CCSprite*>(m_picNode1->getChildByTag(GOODS_ICON_TAG));
            if(pic){
                pic->setColor({90,85,81});
            }
            auto pic1 = dynamic_cast<CCSprite*>(m_picNode1->getChildByTag(GOODS_BG_TAG));
            if(pic1){
                pic1->setColor({90,85,81});
            }
            m_goldSpr1->setColor({90,85,81});
            
        }
    }else{
        m_priceLabel1->setPositionY(0);
    }
}
void LongJingCell::setRightHot(bool ishot,int num){
    m_itemHotNode2->setVisible(ishot);
    m_hotpriceLabel2->setVisible(ishot);
    m_hotNum2Text->setString(CC_CMDITOA(num));
    m_hotdesText2->setString(_lang("101224"));
    m_hotLineSpr2->setVisible(ishot);
    m_priceLabel2->setPositionY(11);
    if(ishot){
        if(num>0){
            m_saleoutSpr2->setVisible(false);
            m_buyBtngraySpr2->setVisible(false);
            m_hotNumSpr2->setVisible(true);
            m_hotNumgraySpr2->setVisible(false);
            m_hotgraySpr2->setVisible(false);
            m_hotSpr2->setVisible(true);
            m_nameLabel2->setColor({194,158,116});
            m_priceLabel2->setColor({255,255,0});
            m_hotpriceLabel2->setColor({221,172,33});
        }else{
            m_saleoutSpr2->setVisible(true);
            m_buyBtngraySpr2->setVisible(true);
            m_hotNumSpr2->setVisible(false);
            m_hotNumgraySpr2->setVisible(true);
            m_hotgraySpr2->setVisible(true);
            m_hotSpr2->setVisible(false);
            m_nameLabel2->setColor({156,156,156});
            m_priceLabel2->setColor({156,156,156});
            m_hotpriceLabel2->setColor({156,156,156});
            auto pic = dynamic_cast<CCSprite*>(m_picNode2->getChildByTag(GOODS_ICON_TAG));
            if(pic){
                pic->setColor({90,85,81});
            }
            auto pic1 = dynamic_cast<CCSprite*>(m_picNode2->getChildByTag(GOODS_BG_TAG));
            if(pic1){
                pic1->setColor({90,85,81});
            }
            m_goldSpr2->setColor({90,85,81});
        }
    }else{
        m_priceLabel2->setPositionY(0);
    }
}