
#include "MailGiftView.h"
#include "ToolController.h"
#include "PopupViewController.h"
#include "YesNoDialog.h"
#include "MailController.h"
#include "MailSaveCommand.h"
#include "MailGetRewardCommand.h"
#include "MailWritePopUpView.h"
#include "CCRichLabelTTF.h"
#include "MailSendCommand.h"
#include "ParticleController.h"
#include "StoreBagView.h"
//
// MailGiftView
//
void MailGiftView::addToolIcon(int toolID, cocos2d::CCNode *mParent, CCLabelIF* mName, CCLabelIF* mDes){
    mParent->removeAllChildren();
    CCSize pSize = mParent->getContentSize();
    if(pSize.equals(CCSizeZero)){
        pSize = CCSize(100, 100);
    }
    CCCommonUtils::createGoodsIcon(toolID, mParent, pSize,NULL,mName,mDes);
//    if(ToolController::getInstance()->m_toolInfos.find(toolID)!=ToolController::getInstance()->m_toolInfos.end()){
//        ToolInfo& info = ToolController::getInstance()->getToolInfoById(toolID);
//        auto pic = CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(info.itemId)).c_str(),true,CCLoadSpriteType_GOODS);
//        if (pic) {
//            CCSize pSize = mParent->getContentSize();
//            if(pSize.equals(CCSizeZero)){
//                pSize = CCSize(100, 100);
//            }
//            auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(info.color).c_str());
//            CCCommonUtils::setSpriteMaxSize(iconBg, pSize.width-5,true);
//            mParent->addChild(iconBg);
//            float pos = (pSize.width-5)*0.5;
//            iconBg->setPosition(ccp(pos, pos));
//            CCCommonUtils::setSpriteMaxSize(pic, pSize.width - 10,true);
//            mParent->addChild(pic);
//            pos = (pSize.width - 10) *0.5;
//            pic->setPosition(ccp(pos, pos));
//        }
//        if(mName){
//            mName->setString(_lang(info.name));
//        }
//        if(mDes){
//            mDes->setString(_lang(info.des));
//        }
//    }else{
//        if(mName){
//            mName->setString("");
//        }
//        if(mDes){
//            mDes->setString("");
//        }
//    }
}


//
// MailGiftCell
//
MailGiftCell::MailGiftCell():mItemID(0),mItemCount(0){
};

MailGiftCell::~MailGiftCell(){
}

MailGiftCell* MailGiftCell::create(CCSize size,int propID,int itemid){
    MailGiftCell* ret = new MailGiftCell();
    if(ret && ret->init(size,itemid,propID)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MailGiftCell::init(CCSize size,int itemid,int propID){
    if (!CCNode::init()) {
        return false;
    }
    setCleanFunction([](){
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
    });
    this->mPropID = propID;
    auto tmpCCB = CCBLoadFile("MailGiftCell",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    m_lblTitle->setString(_lang("102160"));
    m_lblName->setString(_lang("101381"));
    refreshData(CCInteger::create(itemid));
    mContentSize = CCSize(size.width, size.height);
    
    m_effLayer = CCLayer::create();
    CCSize sizeHead = m_headNode->getContentSize();
    m_effLayer->setContentSize(sizeHead);
    m_headNode->getParent()->addChild(m_effLayer);
    m_effLayer->setPosition(m_headNode->getPosition());
    setItemCount(0);
    addEffect();
    return true;
}
void MailGiftCell::setItemCount(int var){
    mItemCount = var;
    if(mItemCount>0){
        m_nodeCount->setVisible(true);
        m_numLabel->setString(CC_CMDITOA(mItemCount));
    }else{
        m_nodeCount->setVisible(0);
    }
}
int MailGiftCell::getItemCount(){
    return mItemCount;
}
void MailGiftCell::removeEffect(){
//    auto batchCount = m_parPopVec.size();
//    while (batchCount--) {
//        m_effLayer->removeChild(m_parPopVec[batchCount]);
//    }
//    m_parPopVec.clear();
    m_effLayer->removeAllChildren();
}
void MailGiftCell::addEffect(){
    removeEffect();
    string tmpStart = "UIGlow_LR_";
    float offset = 70.0f;
    float scale = 0.9f;
    float scale1 = 0.5f;
    int count = 3;
    for (int i=1; i <= count; i++) {
        if(i==2){
            continue;
        }
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        particle->setPosition(ccp(offset, 0));
        particle->setScaleY(scale);
        particle->setScaleX(scale1);
        m_effLayer->addChild(particle);
//        addPopParticleToBatch(particle);
    }
    for (int i=1; i <= count; i++) {
        if(i==2){
            continue;
        }
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        particle->setPosition(ccp(-offset, 0));
        particle->setScaleY(scale);
        particle->setScaleX(scale1);
        m_effLayer->addChild(particle);
//        addPopParticleToBatch(particle);
    }
    tmpStart = "UIGlow_UD_";
    for (int i=1; i <= count; i++) {
        if(i==2){
            continue;
        }
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        particle->setPosition(ccp(0, offset));
        particle->setScaleX(scale);
        particle->setScaleY(scale1);
        m_effLayer->addChild(particle);
//        addPopParticleToBatch(particle);
    }
    for (int i=1; i <= count; i++) {
        if(i==2){
            continue;
        }
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        particle->setPosition(ccp(0, -offset));
        particle->setScaleX(scale);
        particle->setScaleY(scale1);
        m_effLayer->addChild(particle);
//        addPopParticleToBatch(particle);
    }
}
//void MailGiftCell::addPopParticleToBatch(cocos2d::CCParticleSystemQuad *particle){
//    auto batchCount = m_parPopVec.size();
//    while (batchCount--) {
//        auto &batch = m_parPopVec[batchCount];
//        if (batch) {
//            auto batchBlend = batch->getBlendFunc();
//            auto particleBlend = particle->getBlendFunc();
//            if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
//                batch->addChild(particle);
//                return;
//            }
//        }
//    }
//    auto newBatch = ParticleController::createParticleBatch();
//    m_effLayer->addChild(newBatch);
//    newBatch->addChild(particle);
//    m_parPopVec.push_back(newBatch);
//}
void MailGiftCell::onEnter() {
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailGiftCell::refreshData), MSG_MAIL_GIFT_CHOOSE, NULL);
    CCNode::onEnter();
}
void MailGiftCell::onExit() {
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MAIL_GIFT_CHOOSE);
    setTouchEnabled(false);
     CCNode::onExit();
}
SEL_CCControlHandler MailGiftCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onChooseItem", MailGiftCell::onChooseItem);
    return NULL;
}
bool MailGiftCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    //    m_startPoint = pTouch->getStartLocation();
    if(m_canChoose && isTouchInside(m_sprChoose, pTouch)){
        onChooseItem();
        return true;
    }
    return false;
}
void MailGiftCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    //    CCPoint endPoint = pTouch->getLocation();
    //    if(fabs(endPoint.x-m_startPoint.x)>10||fabs(endPoint.y-m_startPoint.y)>10){
    //        return;
    //    }
}
bool MailGiftCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headNode", CCNode*, this->m_headNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblTitle", CCLabelIF*, this->m_lblTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblDes", CCLabelIF*, this->m_lblDes);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblName", CCLabelIF*, this->m_lblName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprChoose", CCSprite*, this->m_sprChoose);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblInfo", CCLabelIF*, this->m_lblInfo);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeCount", CCNode*, this->m_nodeCount);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, this->m_numLabel);
    return false;
}
void MailGiftCell::onChooseItem(){
    MailGiftItemView *itemView = MailGiftItemView::create(mContentSize,mPropID);
    PopupViewController::getInstance()->addPopupView(itemView);
}
void MailGiftCell::refreshData(CCObject *ccObj){
    if(!ccObj){
        mItemID = -1;
    }
    CCInteger *ccInteger = dynamic_cast<CCInteger*>(ccObj);
    CCInteger *ccInteger1 = NULL;
    if(!ccInteger){
        CCArray *array = dynamic_cast<CCArray*>(ccObj);
        if(array && array->count()>1){
            ccInteger = dynamic_cast<CCInteger*>(array->objectAtIndex(0));
            ccInteger1 = dynamic_cast<CCInteger*>(array->objectAtIndex(1));
            
        }
        if(!ccInteger){
            mItemID = -1;
        }
    }
    mItemID = ccInteger->getValue();
    if(ccInteger1){
        setItemCount(ccInteger1->getValue());
        CC_SAFE_RELEASE_NULL(ccObj);
    }
    int propCount = 0;
    if(ToolController::getInstance()->m_toolInfos.find(mPropID) != ToolController::getInstance()->m_toolInfos.end()){
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(mPropID);
        propCount = MAX(0, info.getCNT());
    }
    switch (mPropID) {
        case ITEM_SEND_GIFTALLIANCE:{
            m_lblDes->setString(_lang_1("101013", CC_ITOA(propCount)));
            break;
        }
        case ITEM_SEND_GIFTMORE:{
            m_lblDes->setString(_lang_1("101012", CC_ITOA(propCount)));
            break;
        }
        default:{
            m_lblDes->setString(_lang_1("101382", CC_ITOA(propCount)));
            break;
        }
    }
    if(propCount>0){
        m_canChoose = true;
    }else{
        m_canChoose=false;
    }
    MailGiftView::addToolIcon(mItemID,m_headNode,m_lblInfo);
}

//
// MailGiftItemView
//
MailGiftItemView::MailGiftItemView(){
    
}
MailGiftItemView::~MailGiftItemView() {
    
}
MailGiftItemView* MailGiftItemView::create(CCSize &viewSize,int propID){
    auto ret = new MailGiftItemView();
    if (ret && ret->init(viewSize,propID)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
bool MailGiftItemView::init(CCSize &viewSize,int propID)
{
    if(!PopupBaseView::init())
        return false;
    auto ccb = CCBLoadFile("MailGiftItemView",this,this);
    CCSize contentSize = ccb->getContentSize();
    setContentSize(contentSize);
    mPropID = propID;
    m_bgSpr->setPreferredSize(viewSize);
    m_bgSpr->setPosition(ccp(-viewSize.width*0.5, -contentSize.height * 0.5 + 120));
    m_touchNode->setContentSize(viewSize);
    m_touchNode->setPosition(m_bgSpr->getPosition());
    CCPoint pos = m_bgSpr->getPosition() + ccp(20, 25);
    m_nodeList->setContentSize(CCSize(viewSize.width - 20, viewSize.height - 50));
    m_nodeList->setPosition(pos);
    
//    auto size = CCDirector::sharedDirector()->getWinSize();
//    float dh = size.height-852-8;
//    m_nodeList->setContentSize(CCSize(m_nodeList->getContentSize().width, m_nodeList->getContentSize().height+dh));
    m_data = CCArray::create();
    m_tabView = CCTableView::create(this, m_nodeList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    m_nodeList->addChild(m_tabView);
//    m_lblTitle->setString(_lang("102160"));
    refreshData();
    return true;
}
bool MailGiftItemView::sortByUseOder(int itemId1, int itemId2){
    auto& info1 = ToolController::getInstance()->getToolInfoById(itemId1);
    auto& info2 = ToolController::getInstance()->getToolInfoById(itemId2);
    return (info1.orderNum<info2.orderNum);
}
void MailGiftItemView::refreshData(CCObject *ccObj){
    generateData();
    m_tabView->reloadData();
}
void MailGiftItemView::generateData(){
    m_data->removeAllObjects();
    vector<int> curList;
    auto iter = ToolController::getInstance()->m_allTools.begin();
    while (iter != ToolController::getInstance()->m_allTools.end()) {
        if(ToolController::getInstance()->m_toolInfos.find((*iter))!=ToolController::getInstance()->m_toolInfos.end()){
            ToolInfo& info = ToolController::getInstance()->getToolInfoById((*iter));
            if (info.getCNT()>0 && info.is_gift && (*iter)!=ITEM_SEND_GIFT){
                curList.push_back((*iter));
            }
        }
        ++iter;
    }
    if (curList.size()>0) {
        std::sort(curList.begin(),curList.end(),sortByUseOder);
    }
    iter = ToolController::getInstance()->m_mateTools.begin();
    while (iter != ToolController::getInstance()->m_mateTools.end()) {
        if(ToolController::getInstance()->m_toolInfos.find((*iter))!=ToolController::getInstance()->m_toolInfos.end()){
            ToolInfo& info = ToolController::getInstance()->getToolInfoById((*iter));
            if (info.getCNT()>0 && info.is_gift){
                curList.push_back((*iter));
            }
        }
        ++iter;
    }
    
    string itemId = "";
    int needCount =MAIL_GIFT_ITEM_COL - curList.size() % MAIL_GIFT_ITEM_COL;
    while (needCount>0) {
        curList.push_back(-1);
        --needCount;
    }
    int count1 = curList.size() / MAIL_GIFT_ITEM_COL;
    int iIndex = 0;
    while (iIndex<count1) {
        itemId = "";
        itemId.append(CC_ITOA(curList.at(iIndex * MAIL_GIFT_ITEM_COL)));
        itemId.append(":");
        itemId.append(CC_ITOA(curList.at(iIndex * MAIL_GIFT_ITEM_COL + 1)));
        itemId.append(":");
        itemId.append(CC_ITOA(curList.at(iIndex * MAIL_GIFT_ITEM_COL + 2)));
        itemId.append(":");
        itemId.append(CC_ITOA(curList.at(iIndex * MAIL_GIFT_ITEM_COL + 3)));
        m_data->addObject(CCString::create(itemId));
        ++iIndex;
    }
}
void MailGiftItemView::onEnter() {
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailGiftItemView::onChooseBack), MSG_MAIL_GIFT_CHOOSE, NULL);
}
void MailGiftItemView::onExit() {
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MAIL_GIFT_CHOOSE);
    PopupBaseView::onExit();
}
SEL_CCControlHandler MailGiftItemView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}
void MailGiftItemView::onChooseBack(CCObject *ccObj){
    if(ccObj){
        CCInteger *inter = dynamic_cast<CCInteger*>(ccObj);
        if(inter){
            closeSelf();
        }else{
            CCArray *array = dynamic_cast<CCArray*>(ccObj);
            if(array && array->count()>1){
                closeSelf();
            }
        }
    }
}
bool MailGiftItemView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeList", CCNode*, this->m_nodeList);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblTitle", CCLabelIF*, this->m_lblTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgSpr", CCScale9Sprite*, this->m_bgSpr);
    return false;
}
bool MailGiftItemView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
//    m_startPoint = pTouch->getStartLocation();
    if(isTouchInside(m_nodeList, pTouch)){
        return false;
    }
    if(!isTouchInside(m_touchNode, pTouch)){
        closeSelf();
    }
    return true;
}
void MailGiftItemView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
//    CCPoint endPoint = pTouch->getLocation();
//    if(fabs(endPoint.x-m_startPoint.x)>10||fabs(endPoint.y-m_startPoint.y)>10){
//        return;
//    }
}
CCSize MailGiftItemView::tableCellSizeForIndex(CCTableView *table, ssize_t idx){
    if(idx >= m_data->count()){
        return CCSizeZero;
    }
    return CCSize(605, 145);
}
CCSize MailGiftItemView::cellSizeForTable(CCTableView *table){
    return CCSize(605, 145);
}
CCTableViewCell* MailGiftItemView::tableCellAtIndex(CCTableView *table, ssize_t idx){
    if(idx >= m_data->count()){
        return NULL;
    }
    
    string itemId = dynamic_cast<CCString*>(m_data->objectAtIndex(idx))->getCString();
    
    MailGiftItemCell* cell = (MailGiftItemCell*)table->dequeueCell();
    if(cell){
        cell->setData(itemId);
    }else{
        cell = MailGiftItemCell::create(itemId,m_nodeList,mPropID);
    }
    return cell;
}
ssize_t MailGiftItemView::numberOfCellsInTableView(CCTableView *table)
{
    int num = ceil(m_data->count() / 1.0f);
    return num;
}

//
// MailGiftItemCell
//
MailGiftItemCell* MailGiftItemCell::create(string itemIds, CCNode* touchNode,int propID){
    auto ret = new MailGiftItemCell();
    if (ret && ret->init(itemIds,touchNode,propID)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
bool MailGiftItemCell::init(string itemIds, CCNode* touchNode,int propID)
{
    if(!CCTableViewCell::init()){
        return false;
    }
    auto ccb = CCBLoadFile("MailGiftItemCell",this,this);
    setContentSize(ccb->getContentSize());
    m_touchNode = touchNode;
    setData(itemIds);
    mPropID = propID;
    return true;
}
void MailGiftItemCell::setData(string itemIds){
    std::vector<std::string> items;
    CCCommonUtils::splitString(itemIds, ":", items);
    
    int iIndex= 0;
    while (iIndex<items.size()) {
        if(!items[iIndex].empty()){
            mItemID[iIndex] = atoi(items[iIndex].c_str());
        }
        ++iIndex;
    }
    if(iIndex<MAIL_GIFT_ITEM_COL){
        mItemID[iIndex] = -1;
    }
    iIndex = 0;
    while (iIndex<MAIL_GIFT_ITEM_COL) {
        m_picNode[iIndex]->removeAllChildren();
        m_picNode[iIndex]->setVisible(true);
        if(mItemID[iIndex]!=-1 && ToolController::getInstance()->m_toolInfos.find(mItemID[iIndex])!=ToolController::getInstance()->m_toolInfos.end()){
            ToolInfo& info = ToolController::getInstance()->getToolInfoById(mItemID[iIndex]);
            m_numLabel[iIndex]->setString(CC_CMDITOA(info.getCNT()));
            this->m_itemNode[iIndex]->setVisible(true);
            MailGiftView::addToolIcon(mItemID[iIndex], m_picNode[iIndex]);
        }else{
            m_itemNode[iIndex]->setVisible(false);
        }
        ++iIndex;
    }
}
void MailGiftItemCell::updateSelectState(CCObject *ccObj){
    if(ccObj){
        CCInteger *ccInteger = dynamic_cast<CCInteger*>(ccObj);
        if(ccInteger){
            int chooseid = ccInteger->getValue();
            int index = 0;
            while (index < MAIL_GIFT_ITEM_COL) {
                if( mItemID[index] == chooseid){
                    m_selectSpr->setPosition(m_picBG[index]->getPosition());
                    m_selectSpr->setVisible(true);
                    return;
                }
                ++index;
            }
        }
    }
    m_selectSpr->setVisible(false);
}
void MailGiftItemCell::onEnter() {
    CCTableViewCell::onEnter();

    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
//    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailGiftItemCell::updateSelectState), MSG_MAIL_GIFT_CHOOSE, NULL);
}
void MailGiftItemCell::onExit() {
    setTouchEnabled(false);
    CCTableViewCell::onExit();
//    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MAIL_GIFT_CHOOSE);
}
SEL_CCControlHandler MailGiftItemCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}
bool MailGiftItemCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    m_startPoint = pTouch->getStartLocation();
    if (isTouchInside(m_touchNode,pTouch)){
        if(isTouchInside(this,pTouch)){
            return true;
        }
    }
    return false;
}

void MailGiftItemCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CCPoint endPoint = pTouch->getLocation();
    if(fabs(endPoint.x-m_startPoint.x)>10||fabs(endPoint.y-m_startPoint.y)>10){
        return;
    }
    int index = 0;
    while (index < MAIL_GIFT_ITEM_COL) {
        if (isTouchInside(m_picBG[index],pTouch) && mItemID[index] != -1){
            int itemid = mItemID[index];
            if(mPropID == ITEM_SEND_GIFT || mPropID == ITEM_SEND_GIFTALLIANCE){
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MAIL_GIFT_CHOOSE,CCInteger::create(itemid));
            }else if(mPropID == ITEM_SEND_GIFTMORE){
                PopupViewController::getInstance()->addPopupView(ToolNumSelectView::create(itemid,1));
            }
            return;
        }
        ++index;
    }
}
bool MailGiftItemCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBG1", CCSprite*, m_picBG[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBG2", CCSprite*, m_picBG[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBG3", CCSprite*, m_picBG[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBG4", CCSprite*, m_picBG[3])
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode1", CCNode*, m_picNode[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode2", CCNode*, m_picNode[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode3", CCNode*, m_picNode[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode4", CCNode*, m_picNode[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel1", CCLabelIF*, m_numLabel[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel2", CCLabelIF*, m_numLabel[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel3", CCLabelIF*, m_numLabel[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel4", CCLabelIF*, m_numLabel[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectSpr", CCSprite*, m_selectSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode1", CCNode*, m_itemNode[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode2", CCNode*, m_itemNode[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode3", CCNode*, m_itemNode[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode4", CCNode*, m_itemNode[3]);
    return false;
}


//
//MailGiftReadPopUpView
//

void MailGiftReadPopUpView::onEnter(){
    PopupBaseView::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MailGiftReadPopUpView::refreshContent), MIAL_GET_REWARD_REFRESH, NULL);
}
void MailGiftReadPopUpView::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MIAL_GET_REWARD_REFRESH);
    PopupBaseView::onExit();
}
MailGiftReadPopUpView* MailGiftReadPopUpView::create(MailInfo& info){
    MailGiftReadPopUpView* ret = new MailGiftReadPopUpView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MailGiftReadPopUpView::init(){
    if(!PopupBaseView::init()){
        return false;
    }
    CCLoadSprite::doResourceByCommonIndex(11, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
    });
    auto bg = CCBLoadFile("MailGiftReadView", this, this);
    this->setContentSize(bg->getContentSize());
    setTitleName(_lang("105513"));
    int preHeight = this->m_buildBG->getContentSize().height;
    changeBGHeight(m_buildBG);
    int dh = m_buildBG->getContentSize().height - preHeight;
    this->m_bg->setContentSize(CCSize(m_bg->getContentSize().width, m_bg->getContentSize().height + dh));
    
    m_listNode->setContentSize(CCSize(m_listNode->getContentSize().width, m_listNode->getContentSize().height + dh));
    m_listNode->setPositionY(m_listNode->getPositionY() - dh);
    m_contentText->setPositionY(m_contentText->getPositionY() + dh);
    m_rewardNode->setPositionY(m_rewardNode->getPositionY() + dh);
    
    this->m_nameText->setFntFile(getNBFont(NB_FONT_Bold));
    this->m_titleText->setFntFile(getNBFont(NB_FONT_Bold));
    //this->m_guideTxt->setFntFile(getNBFont(NB_FONT_Bold));
    this->m_nameText->setString(m_info.fromName.c_str());
    string nameExt = m_info.fromName;
    if(m_info.alliance!=""){
        nameExt = "("+m_info.alliance+")"+m_info.fromName;
        this->m_nameText->setString(nameExt);
    }
    this->m_titleText->setString(m_info.title.c_str());
    this->m_timeText->setString(CCCommonUtils::timeStampToDate(m_info.createTime).c_str());
    CCRichLabelTTF* ccrichlabel = CCRichLabelTTF::create("", "Helvetica", 22,CCSize(570,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
    
    ccrichlabel->setString(m_info.contents.c_str());
    ccrichlabel->setFontFillColor(m_contentText->getColor());
    float textH = ccrichlabel->getContentSize().height;
    if(textH > 120){
        
        int needH = 450 + textH;
        CCSize listSize = m_listNode->getContentSize();
        if(needH>listSize.height){
            CCScrollView *scrollView = CCScrollView::create(listSize);
            scrollView->setDirection(kCCScrollViewDirectionVertical);
            this->m_rewardNode->removeFromParentAndCleanup(false);
            scrollView->addChild(m_rewardNode);
            m_rewardNode->setPosition(ccp(0, 0));
//            ccrichlabel->removeFromParentAndCleanup(false);
            scrollView->addChild(ccrichlabel);
            ccrichlabel->setPosition(ccp(0, 450));
            m_listNode->addChild(scrollView);
            scrollView->setContentSize(CCSize(m_listNode->getContentSize().width,needH));
            scrollView->setContentOffset(CCPoint(0,listSize.height - needH));
        }else{
            int addH = textH - 120;
            float pos1 = m_rewardNode->getPositionY() - addH;
            m_rewardNode->setPositionY(pos1);
            ccrichlabel->setAnchorPoint(ccp(0,1));
            ccrichlabel->setPosition(m_contentText->getPosition());
            m_listNode->addChild(ccrichlabel);
        }
    }else{
        ccrichlabel->setAnchorPoint(ccp(0,1));
        m_listNode->addChild(ccrichlabel);
        CCPoint lblPos = m_contentText->getPosition();
        ccrichlabel->setPosition(ccp(lblPos.x,lblPos.y));
    }
    CCSprite* pic;
    if(m_info.pic=="0"||m_info.pic==""){
        string mpic = GlobalData::shared()->playerInfo.pic;
        mpic = mpic+".png";
        pic = CCLoadSprite::createSprite(mpic.c_str());
    }else{
        std::string head = m_info.pic;
        head.append(".png");
        pic = CCLoadSprite::createSprite(head.c_str());
    }
    CCCommonUtils::setSpriteMaxSize(pic, 92);
    pic->setPosition(ccp(10, 4));
    m_userHeadContainer->addChild(pic);
    std::vector<std::string> arSplit;
    CCCommonUtils::splitString(m_info.rewardId, ",", arSplit);
    if(arSplit.size()>2){
        int toolID = atoi(arSplit.at(1).c_str());
        int toolCount = atoi(arSplit.at(2).c_str());
        MailGiftView::addToolIcon(toolID,m_giftNode,m_rewardTitle,m_rewardDesc);
        if(toolCount>1){
            m_nodeCount->setVisible(true);
            m_numLabel->setString(CC_CMDITOA(toolCount));
        }else{
            m_nodeCount->setVisible(false);
        }
    }
    if(m_info.save==0){
        m_unSaveBtn->setVisible(true);
        m_addSaveBtn->setVisible(false);
        
    }else{
        m_unSaveBtn->setVisible(false);
        m_addSaveBtn->setVisible(true);
    }
    refreshContent();
    return true;
}
void MailGiftReadPopUpView::refreshContent(CCObject* p){
    if(p!=NULL && m_waitInterface){
        m_waitInterface->remove();
        m_waitInterface=NULL;
    }

    if(m_info.rewardId != "" && m_info.rewardStatus==0){
        setButtonTitle(m_rewardBtn, _lang("101313").c_str());
//        CCArray *giftNodeChildren = m_giftNode->getChildren();
//        CCObject* ccObj = NULL;
//        CCARRAY_FOREACH(giftNodeChildren, ccObj){
//            CCSprite *spr = dynamic_cast<CCSprite*>(ccObj);
//            if(spr){
//            }
//        }
//        CCScale9Sprite* ccScale9 = CCLoadSprite::createScale9Sprite("Mail_btn03.png");
//        m_rewardBtn->setBackgroundSprite(ccScale9);
//        m_rewardBtn->setPreferredSize(CCSize(170, 64));
//        m_rewardDesc->setColor({61,35,10});
//        m_rewardTitle->setColor({61,35,10});
    }else{
        setButtonTitle(m_rewardBtn, _lang("101386").c_str());
        auto& giftNodeChildren = m_giftNode->getChildren();
        for (auto child : giftNodeChildren)
        {
            CCSprite *spr = dynamic_cast<CCSprite*>(child);
            if(spr){
                spr->setColor({90,85,81});
            }

        }
        CCScale9Sprite* ccScale91 = CCLoadSprite::createScale9Sprite("Mail_btn05.png");
        m_rewardBtn->setBackgroundSpriteForState(ccScale91,CCControlStateNormal);
        CCScale9Sprite* ccScale92 = CCLoadSprite::createScale9Sprite("Mail_btn05.png");
        m_rewardBtn->setBackgroundSpriteForState(ccScale92,CCControlStateHighlighted);
        CCScale9Sprite* ccScale93 = CCLoadSprite::createScale9Sprite("Mail_btn05.png");
        m_rewardBtn->setBackgroundSpriteForState(ccScale93,CCControlStateSelected);
        m_rewardBtn->setPreferredSize(CCSize(230, 70));
        m_rewardDesc->setColor({90,85,81});
        m_rewardTitle->setColor({90,85,81});
    }
}
bool MailGiftReadPopUpView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalNode", CCNode*, this->m_totalNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_line", CCScale9Sprite*, this->m_line);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText", CCLabelIF*, this->m_nameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeText", CCLabelIF*, this->m_timeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_deleteBtn", CCControlButton*, this->m_deleteBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addSaveBtn", CCControlButton*, this->m_addSaveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_unSaveBtn", CCControlButton*, this->m_unSaveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_userHeadContainer", CCNode*, this->m_userHeadContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_contentText", CCLabelIF*, this->m_contentText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardTitle", CCLabelIF*, this->m_rewardTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardDesc", CCLabelIF*, this->m_rewardDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_giftNode", CCNode*, this->m_giftNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprContent", CCScale9Sprite*, this->m_sprContent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardBtn", CCControlButton*, this->m_rewardBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardNode", CCNode*, this->m_rewardNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, this->m_numLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeCount", CCNode*, this->m_nodeCount);
    return false;
}

SEL_CCControlHandler MailGiftReadPopUpView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDeleteClick", MailGiftReadPopUpView::onDeleteClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddSaveClick", MailGiftReadPopUpView::onAddSaveClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRewardClick", MailGiftReadPopUpView::onRewardClick);
    return NULL;
}
void MailGiftReadPopUpView::onDeleteClick(cocos2d::CCObject *pSender, CCControlEvent event){
    if(PopupViewController::getInstance()->getPlayingInAnim())
        return;
    if(&m_info==NULL){
        return;
    }
    if(m_info.rewardStatus==0&&m_info.rewardId!=""){
        CCCommonUtils::flyHint("", "", _lang("105512"));
        return;
    }
    if(m_info.save!=0){
        CCCommonUtils::flyHint("","",_lang("105575"));
        return;
    }
    YesNoDialog::showYesDialog(_lang("105570").c_str(),false,CCCallFunc::create(this, callfunc_selector(MailGiftReadPopUpView::onOkDeleteMail)),false);
}
void MailGiftReadPopUpView::onOkDeleteMail(){
    MailController::getInstance()->removeMail(m_info.uid, CC_ITOA(m_info.type));
    //    this->closeSelf();
    PopupViewController::getInstance()->goBackPopupView();
}
void MailGiftReadPopUpView::onAddSaveClick(cocos2d::CCObject *pSender, CCControlEvent event){
    if(PopupViewController::getInstance()->getPlayingInAnim())
        return;
    
    //    showRewardAnim();
    //    return;
    if(m_info.save ==1){
        MailCancelSaveCommand *cmd = new MailCancelSaveCommand(m_info.uid, m_info.type);
        cmd->sendAndRelease();
        m_info.save = 0;
        CCCommonUtils::flyHint("quest_icon_1.png", "", _lang("105576"));
        m_unSaveBtn->setVisible(true);
        m_addSaveBtn->setVisible(false);
    }else{
        if(MailController::getInstance()->isMailFull(2)){
            CCCommonUtils::flyText("full");
            return;
        }
        MailSaveCommand *cmd = new MailSaveCommand(m_info.uid, m_info.type);
        cmd->sendAndRelease();
        m_info.save = 1;
        CCCommonUtils::flyHint("quest_icon_1.png", "", _lang("105575"));
        m_unSaveBtn->setVisible(false);
        m_addSaveBtn->setVisible(true);
    }
}
void MailGiftReadPopUpView::onRewardClick(CCObject *pSender, CCControlEvent event){
    if(m_waitInterface)
        return;
    
    if(!m_info.rewardId.empty() && m_info.rewardStatus == 0){
        if(PopupViewController::getInstance()->getPlayingInAnim())
            return;
        m_waitInterface = GameController::getInstance()->showWaitInterface(m_rewardBtn);
        MailGetRewardCommand* cmd = new MailGetRewardCommand(m_info.uid, m_info.type);
        cmd->sendAndRelease();
    }else{
        PopupViewController::getInstance()->addPopupInViewWithAnim(MailWritePopUpView::create(m_info.fromName, "","",m_info.uid));
    }
}