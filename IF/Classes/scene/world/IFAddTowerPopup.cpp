//
//  IFAddTowerPopup.cpp
//  IF
//
//  Created by wangdianzhen on 15/6/8.
//
//

#include "IFAddTowerPopup.h"
#include "WorldMapView.h"
#include "YesNoDialog.h"
#include "WorldCommand.h"
#include "FunBuildController.h"
#include "DynamicTiledMap.h"

IFAddTowerPopup* IFAddTowerPopup::create(string uid,int tileIndex){
    IFAddTowerPopup* pRet = new IFAddTowerPopup(uid ,tileIndex);
    if (pRet && pRet->init()) {
        pRet->autorelease();
    }
    else{
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
    return pRet;
}
bool IFAddTowerPopup::init(){
    if(!PopupBaseView::init()){
        return false;
    }
    setIsHDPanel(true);
    m_parentBatchNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("1200.png")->getTexture(), 448);
    isMoving = false;
    this->setModelLayerOpacity(0);
    this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    m_picNode = CCNode::create();
    m_picNode->setContentSize(CCSize(_tile_width, _tile_height));
    m_picNode->setAnchorPoint(ccp(0, 0));
    
    CCPoint tilePoint = WorldController::getInstance()->getPointByIndex(m_tileIndex);
    CCPoint viewPoint = getInLayerPoint(tilePoint);
    viewPoint = viewPoint + ccp(-_tile_width * 0.5,-_tile_height * 0.5);

    
    CCLayerColor* testColor = CCLayerColor::create(ccc4(125, 125, 125, 100), 256, 140);
//    m_picNode->addChild(testColor);
    m_picNode->setPosition(viewPoint);
    
    auto citySp = CCLoadSprite::createSprite("allianceTower.png");
    citySp->setAnchorPoint(ccp(0, 0));
    if (CCCommonUtils::isIosAndroidPad()) {
        citySp->setScale(WORLD_DEFAULT_HD_SCALE);
        citySp->setPosition(ccp(35, 1));
    }
    else {
        citySp->setPosition(ccp(59, 29));
    }
    m_picNode->addChild(citySp);
    
    if (CCCommonUtils::isIosAndroidPad()) {
        m_cancelBtn = CCControlButton::create(CCLoadSprite::createScale9Sprite("btn_yellow.png"));
        m_btnLabel1 = CCLabelIF::create(_lang("115021").c_str());
        m_btnLabel1->setFontSize(32);
        m_btnLabel1->setColor(ccc3(181, 162, 119));
        m_cancelBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(IFAddTowerPopup::onCancelBtnClick), CCControlEventTouchUpInside);
        m_cancelBtn->setPreferredSize(CCSize(160, 70));
        m_cancelBtn->setScaleX(2.1);
        m_cancelBtn->setScaleY(1.7);
        
        m_confirmBtn = CCControlButton::create(CCLoadSprite::createScale9Sprite("btn_gold.png"));
        m_confirmBtn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("btn_gold.png"),CCControlStateHighlighted);
        m_confirmBtn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("Btn_grey.png"),CCControlStateDisabled);
        m_confirmBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(IFAddTowerPopup::onConfirmBtnClick), CCControlEventTouchUpInside);
        m_confirmBtn->setPreferredSize(CCSize(160, 70));
        m_confirmBtn->setScaleX(2.1);
        m_confirmBtn->setScaleY(1.7);
        m_btnLabel = CCLabelIF::create(_lang("115302").c_str());
        m_btnLabel->setFontSize(32);
        m_btnLabel->setColor(ccc3(181, 162, 119));
        
        auto bg = CCLoadSprite::createSprite("technology_11.png");
        m_picNode->addChild(bg);
        
        m_iconNode = CCNode::create();
        int num = WorldController::getInstance()->m_bAAreaCost;
        std::string picPath = "";
        m_numText = CCLabelIF::create(CC_ITOA(num));
        m_picNode->addChild(m_cancelBtn);
        m_cancelBtn->setPosition(-60, -50);
        m_picNode->addChild(m_confirmBtn);
        m_confirmBtn->setPosition(_tile_width + 60, m_cancelBtn->getPositionY());
        bg->setPosition(ccp(citySp->getPositionX() + _halfTileSize.width - 59, m_cancelBtn->getPositionY()));
        bg->setScaleY(8);
        bg->setScaleX(5);
        m_picNode->addChild(m_iconNode);
        m_iconNode->setPosition(m_confirmBtn->getPositionX() - 30, m_confirmBtn->getPositionY() - 15);
        m_picNode->addChild(m_btnLabel);
        m_btnLabel->setPosition(m_confirmBtn->getPosition());
        m_picNode->addChild(m_btnLabel1);
        m_btnLabel1->setPosition(m_cancelBtn->getPosition());
    }
    else {
        m_cancelBtn = CCControlButton::create(CCLoadSprite::createScale9Sprite("btn_yellow.png"));
        CCCommonUtils::setButtonTitleColor(m_cancelBtn, ccc3(181, 162, 119));
        CCCommonUtils::setButtonTitle(m_cancelBtn, _lang("115021").c_str());
        m_cancelBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(IFAddTowerPopup::onCancelBtnClick), CCControlEventTouchUpInside);
        m_cancelBtn->setPreferredSize(CCSize(160, 70));
        
        m_confirmBtn = CCControlButton::create(CCLoadSprite::createScale9Sprite("btn_gold.png"));
        m_confirmBtn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("btn_gold.png"),CCControlStateHighlighted);
        m_confirmBtn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("Btn_grey.png"),CCControlStateDisabled);
        m_confirmBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(IFAddTowerPopup::onConfirmBtnClick), CCControlEventTouchUpInside);
        m_confirmBtn->setPreferredSize(CCSize(160, 70));
        CCCommonUtils::setButtonTitleColor(m_confirmBtn, ccc3(181, 162, 119));
        CCCommonUtils::setButtonTitle(m_confirmBtn, _lang("115302").c_str());
        
        auto bg = CCLoadSprite::createSprite("technology_11.png");
        m_picNode->addChild(bg);
        
        m_iconNode = CCNode::create();
        int num = WorldController::getInstance()->m_bAAreaCost;
        std::string picPath = "";
        m_numText = CCLabelIF::create(CC_ITOA(num));
        m_picNode->addChild(m_cancelBtn);
        m_cancelBtn->setPosition(40, -50);
        m_picNode->addChild(m_confirmBtn);
        m_confirmBtn->setPosition(_tile_width - 40, m_cancelBtn->getPositionY());
        bg->setPosition(ccp(citySp->getPositionX() + _halfTileSize.width - 59, m_cancelBtn->getPositionY()));
        bg->setScaleY(4);
        bg->setScaleX(3);
        m_picNode->addChild(m_iconNode);
        m_iconNode->setPosition(m_confirmBtn->getPositionX() - 30, m_confirmBtn->getPositionY() - 15);
    }
    
    this->addChild(m_picNode, 2);
    m_showNode = CCNode::create();
    this->addChild(m_showNode, 1);
    refreshShowNode(false);
    
    m_batchNode = CCNode::create();
    this->addChild(m_batchNode, 0);
    m_batchNode->addChild(m_parentBatchNode);
    
    
    
    CCPoint startPoint =  ccp(0,0);
    
    
    int startX = -_tile_width*2;
    int startY = - _tile_height*2;
    
    for (int index = 0; index < 5; index ++) {
        auto sp = CCLoadSprite::createSprite("ArrowTower_blue.png");
        sp->setScale(-1);
        CCPoint pos = startPoint + ccp(startX + _halfTileSize.width * index, -_halfTileSize.height * index);
        sp->setPosition(pos);
        m_parentBatchNode->addChild(sp);
    }
    
    for (int index = 0; index < 5; index ++) {
        auto sp = CCLoadSprite::createSprite("ArrowTower_blue.png");
        sp->setScaleX(-1);
        CCPoint pos = startPoint + ccp(startX + _halfTileSize.width * index,_halfTileSize.height * index);
        sp->setPosition(pos);
        m_parentBatchNode->addChild(sp);
    }
    
    
    for (int index = 0; index < 5; index ++) {
        auto sp = CCLoadSprite::createSprite("ArrowTower_blue.png");
        CCPoint pos = startPoint + ccp(_halfTileSize.width * index, -startY - _halfTileSize.height * index);
        sp->setPosition(pos);
        m_parentBatchNode->addChild(sp);
    }
    
    for (int index = 0; index < 5; index ++) {
        auto sp = CCLoadSprite::createSprite("ArrowTower_blue.png");
        sp->setScaleY(-1);
        CCPoint pos = startPoint + ccp(_halfTileSize.width * index,startY + _halfTileSize.height * index);
        sp->setPosition(pos);
        m_parentBatchNode->addChild(sp);
    }

    
    
    
    return true;
}
CCPoint IFAddTowerPopup::getMovePoint(){
    auto size = CCDirector::sharedDirector()->getWinSize();
    int top = size.height - (_tile_height + 140);
    int down = 140 + DOWN_HEIGHT;
    int left = _tile_width / 2;
    int right = size.width - (_tile_width * 1.5);
    auto pos = m_picNode->getPosition();
    int dx = 0;
    int dy = 0;
    if(pos.x < left){
        dx = left - pos.x;
    }else if(pos.x > right){
        dx = right - pos.x;
    }
    if(pos.y < down){
        dy = down - pos.y;
    }else if(pos.y > top){
        dy = top - pos.y;
    }
    return ccp(dx, dy);
}

void IFAddTowerPopup::refreshShowNode(bool isDrag/* = true*/){
    CCPoint pt = getMovePoint();
    if(!pt.equals(ccp(0, 0))){
        m_picNode->setPosition(m_picNode->getPosition() + pt);
        if(isDrag){
            float maxMove = 15;
            auto moveDis = sqrt(pow(pt.x, 2) + pow(pt.y, 2));
            if(moveDis > maxMove){
                pt = pt * (maxMove * 1.0f / moveDis);
            }
            
            getMapNode()->setPosition(getMapNode()->getPosition() + pt);
            reDraw(NULL);
        }else{
            auto seque = CCSequence::create(CCMoveTo::create(0.2, getMapNode()->getPosition() + pt)
                                            , CCCallFuncN::create(this, callfuncN_selector(IFAddTowerPopup::reDraw))
                                            , NULL
                                            );
            getMapNode()->runAction(seque);
        }
    }else{
        reDraw(NULL);
    }
}
void IFAddTowerPopup::reDraw(CCNode* obj){
    auto addPic = [](CCNode *parent, CCPoint &pt, std::string picName, int tag){
        if(parent->getChildByTag(tag)){
            auto sprite = dynamic_cast<CCSprite*>(parent->getChildByTag(tag));
            sprite->setDisplayFrame(CCLoadSprite::loadResource(picName.c_str()));
        }else{
            auto sprite = CCLoadSprite::createSprite(picName.c_str());
            sprite->setPosition(pt);
            float s = WorldMapView::instance()->m_map->getScale();
            sprite->setOpacity(200);
            sprite->setScale(0.95 * s);
            CCSequence *sequene = CCSequence::create(CCFadeTo::create(1, 50), CCFadeTo::create(1, 200), NULL);
            sprite->runAction(CCRepeatForever::create(sequene));
            parent->addChild(sprite);
        }
    };
    WorldMapView::instance()->onShowTowerRange(true);
    m_showNode->removeAllChildren();
    auto centerPt = m_picNode->getPosition() + ccp(_tile_width / 2, _tile_height / 2);
    float s = WorldMapView::instance()->m_map->getScale();
    centerPt = (WorldMapView::instance()->m_map->getPosition() - centerPt) / s;
    auto mapPt1 = WorldMapView::instance()->m_map->getTilePointByViewPoint(-centerPt);
    currentCenterPt = mapPt1;
    auto pt1 = WorldMapView::instance()->m_map->getPosition() + WorldMapView::instance()->m_map->getViewPointByTilePoint(mapPt1) * s;
    bool canRelocateFlag1 = isCanRelocationTile(mapPt1);
    std::vector<CCPoint> vector;
    vector.push_back(mapPt1);
    if(canRelocateFlag1 ){
        bool isInSame = isInRelicAndNormalBlock(vector);
        if(!isInSame){
            canRelocateFlag1 =  false;
        }
    }
    addPic(m_showNode, pt1, (canRelocateFlag1 ? "green_block.png" : "red_block.png"), 1);
    enableBtn(canRelocateFlag1 );
    
    m_parentBatchNode->setPosition(pt1);
    
}
void IFAddTowerPopup::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
    m_confirmBtn->setTouchPriority(0);
    m_cancelBtn->setTouchPriority(0);
    WorldMapView::instance()->onShowTowerRange(true);
}
void IFAddTowerPopup::onExit(){
    PopupBaseView::onExit();
    setTouchEnabled(false);
    WorldMapView::instance()->onShowTowerRange(false);
}

bool IFAddTowerPopup::onTouchBegan(CCTouch* pTouch,CCEvent* pEvent){
    if(isTouchInside(m_picNode, pTouch)){
        isMoving = true;
        movingStartPt = m_picNode->getPosition();
    }
    return true;
}
void IFAddTowerPopup::onTouchMoved(CCTouch* pTouch,CCEvent* pEvent){
    if( !isMoving){
        return;
    }
    
    CCPoint addPt = pTouch->getLocation() - pTouch->getStartLocation();
    m_picNode->setPosition(movingStartPt + addPt);
    refreshShowNode();
}
void IFAddTowerPopup::onTouchEnded(CCTouch* pTouch,CCEvent* pEvent){
    if(!isTouchInside(m_picNode, pTouch) && !isMoving && !isTouchInside(m_confirmBtn, pTouch)){
        this->closeSelf();
    }
    isMoving = false;
}

void IFAddTowerPopup::onCancelBtnClick(CCObject* pSender, CCControlEvent event){
    this->closeSelf();
}
void IFAddTowerPopup::onConfirmBtnClick(CCObject* pSender,CCControlEvent event){
    doBuy();
}
void IFAddTowerPopup::enableBtn(bool b){
    this->m_confirmBtn->setEnabled(b);
}
bool IFAddTowerPopup::isCanRelocationTile(CCPoint &pt){
    int index = WorldController::getIndexByPoint(pt);
    if(!WorldController::getInstance()->isInMap(pt)){
        return false;
    }
    if(WorldController::getInstance()->getCityInfos()[index].cityType != OriginTile){
        return false;
    }
    if(WorldController::getInstance()->getCityInfos()[index].cityOriginType == "stop"){
        return false;
    }
    if(WorldController::getInstance()->getCityInfos()[index].cityOriginType == "neutrally"){
        return false;
    }
    if(WorldController::getInstance()->getCityInfos()[index].m_isInTowerRange && WorldController::getInstance()->getCityInfos()[index].m_bigRangeAid == GlobalData::shared()->playerInfo.allianceInfo.uid){
        return false;
    }
    if(!WorldController::getInstance()->getCityInfos()[index].m_isInAArea){
        return false;
    }
    if(!WorldController::getInstance()->getCityInfos()[index].m_isMyAArea){
        return false;
    }
    if(!WorldController::getInstance()->getCityInfos()[index].m_forceActive){
        return false;
    }
    
    return true;
}
void IFAddTowerPopup::doBuy(){
    unsigned int index = WorldController::getIndexByPoint(currentCenterPt);
    WorldTowerCreateCommand* cmd = new WorldTowerCreateCommand(index,m_uid);
    cmd->sendAndRelease();
    closeSelf();
}
float IFAddTowerPopup::getMapScale(){
    float _scale = 1;
    if (WorldMapView::instance() && WorldMapView::instance()->m_map) {
        _scale = WorldMapView::instance()->m_map->getScale();
    }
    return _scale;
}
CCPoint IFAddTowerPopup::getInLayerPoint(CCPoint & tilePoint){
    if (WorldMapView::instance() && WorldMapView::instance()->m_map) {
        return WorldMapView::instance()->m_map->getPosition() + WorldMapView::instance()->m_map->getViewPointByTilePoint(tilePoint) * getMapScale();
    }
    return CCPointZero;
    
}
CCNode* IFAddTowerPopup::getMapNode(){
    if (WorldMapView::instance() && WorldMapView::instance()->m_map) {
        return WorldMapView::instance()->m_map;
    }
    return CCNode::create();
}
bool IFAddTowerPopup::isInRelicAndNormalBlock(std::vector<CCPoint> &vector){
    int i = 0;
    std::string typeStr = "";
    while (i < vector.size()) {
        CCPoint &pt = vector[i];
        int index = WorldController::getIndexByPoint(pt);
        std::string landType = WorldController::getInstance()->getCityInfos()[index].cityOriginType;
        if(typeStr == ""){
            typeStr = landType;
        }
        if(landType == "neutrally" && typeStr != landType){
            return false;
        }
        i++;
    }
    return true;
}