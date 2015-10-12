//
//  TerritoryBannerPopupView.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/8/19.
//
//

#include "TerritoryBannerPopupView.h"
#include "WorldMapView.h"
#include "YesNoDialog.h"
#include "WorldCommand.h"
#include "FunBuildController.h"
#include "CCFlagWaveSprite.h"
#include "DynamicTiledMap.h"

TerritoryBannerPopupView* TerritoryBannerPopupView::create(int tileIndex, std::string banner){
    TerritoryBannerPopupView* pRet = new TerritoryBannerPopupView(tileIndex,banner);
    if (pRet && pRet->init()) {
        pRet->autorelease();
    }
    else{
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
    return pRet;
}
bool TerritoryBannerPopupView::init(){
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
    
    std::string bannerStr = m_banner;
    bannerStr.append(".png");
    CCLoadSprite::doResourceByCommonIndex(208, true);
    auto bannerSprite = CCFlagWaveSprite::create(CCLoadSprite::loadResource(bannerStr.c_str()));
    if (bannerSprite) {
        bannerSprite->setAnchorPoint(ccp(0, 0));
        if (CCCommonUtils::isIosAndroidPad()) {
            bannerSprite->setScale(0.4);
        }
        else
            bannerSprite->setScale(0.2);
        bannerSprite->setPosition(citySp->getPosition() + citySp->getContentSize() * citySp->getScale() / 2  + ccp(10, 50));
        m_picNode->addChild(bannerSprite);
    }
    else {
        bannerSprite = CCFlagWaveSprite::create(CCLoadSprite::loadResource("UN.png"));
        bannerSprite->setAnchorPoint(ccp(0, 0));
        if (CCCommonUtils::isIosAndroidPad()) {
            bannerSprite->setScale(0.4);
        }
        else
            bannerSprite->setScale(0.2);
        bannerSprite->setPosition(citySp->getPosition() + citySp->getContentSize() * citySp->getScale() / 2  + ccp(10, 50));
        m_picNode->addChild(bannerSprite);
    }
    CCLoadSprite::doResourceByCommonIndex(208, false);

    
    if (CCCommonUtils::isIosAndroidPad()) {
        m_cancelBtn = CCControlButton::create(CCLoadSprite::createScale9Sprite("btn_yellow.png"));
        m_btnLabel1 = CCLabelIF::create(_lang("115021").c_str());
        m_btnLabel1->setFontSize(32);
        m_btnLabel1->setColor(ccc3(181, 162, 119));
        m_cancelBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(TerritoryBannerPopupView::onCancelBtnClick), CCControlEventTouchUpInside);
        m_cancelBtn->setPreferredSize(CCSize(160, 70));
        m_cancelBtn->setScaleX(2.1);
        m_cancelBtn->setScaleY(1.7);
        
        m_confirmBtn = CCControlButton::create(CCLoadSprite::createScale9Sprite("btn_gold.png"));
        m_confirmBtn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("btn_gold.png"),CCControlStateHighlighted);
        m_confirmBtn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("Btn_grey.png"),CCControlStateDisabled);
        m_confirmBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(TerritoryBannerPopupView::onConfirmBtnClick), CCControlEventTouchUpInside);
        m_confirmBtn->setPreferredSize(CCSize(160, 70));
        m_confirmBtn->setScaleX(2.1);
        m_confirmBtn->setScaleY(1.7);
        m_btnLabel = CCLabelIF::create(_lang("115302").c_str());
        m_btnLabel->setFontSize(32);
        m_btnLabel->setColor(ccc3(181, 162, 119));
        
        auto bg = CCLoadSprite::createSprite("technology_11.png");
        m_picNode->addChild(bg);
        
        m_iconNode = CCNode::create();
        int num = WorldController::getInstance()->m_territoryBannerCost;
        std::string picPath = "ui_gold.png";
        auto sprite = CCLoadSprite::createSprite(picPath.c_str());
        m_iconNode->addChild(sprite);
        CCCommonUtils::setSpriteMaxSize(sprite, 80);
        m_numText = CCLabelIF::create(CC_ITOA(num));
        if (num == 0) {
            m_picNode->addChild(m_cancelBtn);
            m_cancelBtn->setPosition(-60, -50);
            m_picNode->addChild(m_confirmBtn);
            m_confirmBtn->setPosition(_tile_width + 60, m_cancelBtn->getPositionY());
            m_picNode->addChild(m_btnLabel);
            m_btnLabel->setPosition(m_confirmBtn->getPosition());
            m_picNode->addChild(m_btnLabel1);
            m_btnLabel1->setPosition(m_cancelBtn->getPosition());
        }
        else {
            m_picNode->addChild(m_iconNode);
            m_iconNode->setPosition(m_confirmBtn->getPositionX() - 30, m_confirmBtn->getPositionY() - 15);
            m_picNode->addChild(m_numText);
            m_numText->setAnchorPoint(ccp(0, 0.5));
            m_numText->setPosition(m_confirmBtn->getPositionX(), m_confirmBtn->getPositionY() - 15);
            m_picNode->addChild(m_cancelBtn);
            m_cancelBtn->setPosition(-60, -50);
            m_picNode->addChild(m_confirmBtn);
            m_confirmBtn->setPosition(_tile_width + 60, m_cancelBtn->getPositionY());
            m_picNode->addChild(m_btnLabel);
            m_btnLabel->setPosition(m_confirmBtn->getPositionX(), m_confirmBtn->getPositionY() + 15);
            m_picNode->addChild(m_btnLabel1);
            m_btnLabel1->setPosition(m_cancelBtn->getPosition());
        }
        bg->setPosition(ccp(citySp->getPositionX() + _halfTileSize.width - 59, m_cancelBtn->getPositionY()));
        bg->setScaleY(8);
        bg->setScaleX(5);
        
    }
    else {
        m_cancelBtn = CCControlButton::create(CCLoadSprite::createScale9Sprite("btn_yellow.png"));
        CCCommonUtils::setButtonTitleColor(m_cancelBtn, ccc3(181, 162, 119));
        CCCommonUtils::setButtonTitle(m_cancelBtn, _lang("115021").c_str());
        m_cancelBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(TerritoryBannerPopupView::onCancelBtnClick), CCControlEventTouchUpInside);
        m_cancelBtn->setPreferredSize(CCSize(160, 70));
        
        m_confirmBtn = CCControlButton::create(CCLoadSprite::createScale9Sprite("btn_gold.png"));
        m_confirmBtn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("btn_gold.png"),CCControlStateHighlighted);
        m_confirmBtn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("Btn_grey.png"),CCControlStateDisabled);
        m_confirmBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(TerritoryBannerPopupView::onConfirmBtnClick), CCControlEventTouchUpInside);
        m_confirmBtn->setPreferredSize(CCSize(160, 70));
        m_btnLabel = CCLabelIF::create(_lang("115302").c_str());
        m_btnLabel->setFontSize(24);
        m_btnLabel->setColor(ccc3(181, 162, 119));
        
        auto bg = CCLoadSprite::createSprite("technology_11.png");
        m_picNode->addChild(bg);
        
        m_iconNode = CCNode::create();
        std::string picPath = "ui_gold.png";
        auto sprite = CCLoadSprite::createSprite(picPath.c_str());
        m_iconNode->addChild(sprite);
        CCCommonUtils::setSpriteMaxSize(sprite, 40);

        int cost = WorldController::getInstance()->m_territoryBannerCost;
        if (cost == 0) {
            m_btnLabel->setFontSize(32);
            m_numText = CCLabelIF::create(CC_ITOA(cost));
            m_picNode->addChild(m_cancelBtn);
            m_cancelBtn->setPosition(40, -50);
            m_picNode->addChild(m_confirmBtn);
            m_confirmBtn->setPosition(_tile_width - 40, m_cancelBtn->getPositionY());
            m_picNode->addChild(m_btnLabel);
            m_btnLabel->setPosition(m_confirmBtn->getPositionX(), m_confirmBtn->getPositionY());
        }
        else {
            m_numText = CCLabelIF::create(CC_ITOA(cost));
            m_picNode->addChild(m_cancelBtn);
            m_cancelBtn->setPosition(40, -50);
            m_picNode->addChild(m_confirmBtn);
            m_confirmBtn->setPosition(_tile_width - 40, m_cancelBtn->getPositionY());
            m_picNode->addChild(m_iconNode);
            m_iconNode->setPosition(m_confirmBtn->getPositionX() - 10, m_confirmBtn->getPositionY() - 10);
            m_picNode->addChild(m_numText);
            m_numText->setAnchorPoint(ccp(0, 0.5));
            m_numText->setPosition(m_confirmBtn->getPositionX() + 10, m_confirmBtn->getPositionY() - 10);
            m_picNode->addChild(m_btnLabel);
            m_btnLabel->setPosition(m_confirmBtn->getPositionX(), m_confirmBtn->getPositionY() + 10);
        }
        bg->setPosition(ccp(citySp->getPositionX() + _halfTileSize.width - 59, m_cancelBtn->getPositionY()));
        bg->setScaleY(4);
        bg->setScaleX(3);
    }
    
    this->addChild(m_picNode, 2);
    m_showNode = CCNode::create();
    this->addChild(m_showNode, 1);
    refreshShowNode(false);
    
    m_batchNode = CCNode::create();
    this->addChild(m_batchNode, 0);
    m_batchNode->addChild(m_parentBatchNode);
    
    return true;
}
CCPoint TerritoryBannerPopupView::getMovePoint(){
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

void TerritoryBannerPopupView::refreshShowNode(bool isDrag/* = true*/){
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
                                            , CCCallFuncN::create(this, callfuncN_selector(TerritoryBannerPopupView::reDraw))
                                            , NULL
                                            );
            getMapNode()->runAction(seque);
        }
    }else{
        reDraw(NULL);
    }
}
void TerritoryBannerPopupView::reDraw(CCNode* obj){
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
void TerritoryBannerPopupView::onEnter(){
    PopupBaseView::onEnter();
    setTouchEnabled(true);
  //  CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
    m_confirmBtn->setTouchPriority(0);
    m_cancelBtn->setTouchPriority(0);
}
void TerritoryBannerPopupView::onExit(){
    PopupBaseView::onExit();
    setTouchEnabled(false);
   // CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool TerritoryBannerPopupView::onTouchBegan(CCTouch* pTouch,CCEvent* pEvent){
    if(isTouchInside(m_picNode, pTouch)){
        isMoving = true;
        movingStartPt = m_picNode->getPosition();
    }
    return true;
}
void TerritoryBannerPopupView::onTouchMoved(CCTouch* pTouch,CCEvent* pEvent){
    if( !isMoving){
        return;
    }
    
    CCPoint addPt = pTouch->getLocation() - pTouch->getStartLocation();
    m_picNode->setPosition(movingStartPt + addPt);
    refreshShowNode();
}
void TerritoryBannerPopupView::onTouchEnded(CCTouch* pTouch,CCEvent* pEvent){
    if(!isTouchInside(m_picNode, pTouch) && !isMoving && !isTouchInside(m_confirmBtn, pTouch)){
        this->closeSelf();
    }
    isMoving = false;
}

void TerritoryBannerPopupView::onCancelBtnClick(CCObject* pSender, CCControlEvent event){
    this->closeSelf();
}
void TerritoryBannerPopupView::onConfirmBtnClick(CCObject* pSender,CCControlEvent event){
    doBuy();
}
void TerritoryBannerPopupView::enableBtn(bool b){
    this->m_confirmBtn->setEnabled(b);
}
bool TerritoryBannerPopupView::isCanRelocationTile(CCPoint &pt){
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
void TerritoryBannerPopupView::doBuy(){
    unsigned int index = WorldController::getIndexByPoint(currentCenterPt);
    WorldTerritoryBannerCreateCommand* cmd = new WorldTerritoryBannerCreateCommand(index,m_banner);
    cmd->sendAndRelease();
    closeSelf();
}
float TerritoryBannerPopupView::getMapScale(){
    float _scale = 1;
    if (WorldMapView::instance() && WorldMapView::instance()->m_map) {
        _scale = WorldMapView::instance()->m_map->getScale();
    }
    return _scale;
}
CCPoint TerritoryBannerPopupView::getInLayerPoint(CCPoint & tilePoint){
    if (WorldMapView::instance() && WorldMapView::instance()->m_map) {
        return WorldMapView::instance()->m_map->getPosition() + WorldMapView::instance()->m_map->getViewPointByTilePoint(tilePoint) * getMapScale();
    }
    return CCPointZero;
    
}
CCNode* TerritoryBannerPopupView::getMapNode(){
    if (WorldMapView::instance() && WorldMapView::instance()->m_map) {
        return WorldMapView::instance()->m_map;
    }
    return CCNode::create();
}
bool TerritoryBannerPopupView::isInRelicAndNormalBlock(std::vector<CCPoint> &vector){
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