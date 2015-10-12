//
//  AllianceAreaPopupView.cpp
//  IF
//
//  Created by wangdianzhen on 15/4/10.
//
//

#include "AllianceAreaPopupView.h"
#include "WorldMapView.h"
#include "YesNoDialog.h"
#include "WorldCommand.h"
#include "FunBuildController.h"
#include "CCFlagWaveSprite.h"
#include "DynamicTiledMap.h"

AllianceAreaPopupView* AllianceAreaPopupView::create(string aareaId,int tileIndex){
    AllianceAreaPopupView* pRet = new AllianceAreaPopupView(aareaId ,tileIndex);
    if (pRet && pRet->init()) {
        pRet->autorelease();
    }
    else{
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
    return pRet;
}
bool AllianceAreaPopupView::init(){
    if(!PopupBaseView::init()){
        return false;
    }
    setIsHDPanel(true);
    isMoving = false;
    this->setModelLayerOpacity(0);
    this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    m_picNode = CCNode::create();
    m_picNode->setContentSize(CCSize(_tile_width * 2, _tile_height * 2));
    m_picNode->setAnchorPoint(ccp(0, 0));
    CCPoint testPoint =PopupViewController::getInstance()->getTouchPoint() + ccp(-_tile_width, -_tile_height);
//    m_picNode->setPosition(PopupViewController::getInstance()->getTouchPoint() + ccp(-_tile_width, -_tile_height));
    
    CCPoint tilePoint = WorldController::getInstance()->getPointByIndex(m_tileIndex);
    CCPoint viewPoint = getInLayerPoint(tilePoint);
    viewPoint = viewPoint + ccp(-_tile_width * 1.5, -_tile_height);
    m_picNode->setPosition(viewPoint);
    
    m_cityNode = CCNode::create();
    
 
    int mapIndex = 3;
    while (mapIndex >= 0) {
        auto arr = WorldMapView::getCityPicArr(mapIndex, 0, false ,44996);
        int posX = _tile_width / 2;
        int posY = _tile_height / 2;
        if(mapIndex == 0 || mapIndex == 2){
            posX = _tile_width;
        }
        if(mapIndex == 3){
            posX = _tile_width + posX;
        }
        if(mapIndex == 1 || mapIndex == 3){
            posY = _tile_height;
        }
        if(mapIndex == 2){
            posY = _tile_height + posY;
        }
        CCObject *obj = NULL;
        CCARRAY_FOREACH(arr, obj){
            std::string picName = _dict(obj)->valueForKey("pic")->getCString();
            int addX = _dict(obj)->valueForKey("x")->intValue();
            int addY = _dict(obj)->valueForKey("y")->intValue();
            auto sprite = CCLoadSprite::createSprite(picName.c_str());
            sprite->setAnchorPoint(ccp(0, 0));
            if (CCCommonUtils::isIosAndroidPad()) {
                m_cityNode->addChild(sprite);
                sprite->setPosition(ccp(posX, posY) - ccp(_tile_width / 2, _tile_height / 2) + ccp(addX, addY));

            }
            else {
                m_picNode->addChild(sprite);
                sprite->setPosition(ccp(posX, posY) - ccp(_tile_width / 2, _tile_height / 2) + ccp(addX, addY));
            }
        }
        mapIndex--;
    }
    
    if (CCCommonUtils::isIosAndroidPad()) {
        m_picNode->addChild(m_cityNode);
        m_cityNode->setScale(WORLD_DEFAULT_HD_SCALE);
        m_cityNode->setPosition(ccp(-_tile_width * (WORLD_DEFAULT_HD_SCALE - 1),-_tile_height * (WORLD_DEFAULT_HD_SCALE - 1)));
    }
    
        CCPointArray* pointArr = CCPointArray::create(3);
        pointArr->addControlPoint(ccp(-70,59));
        pointArr->addControlPoint(ccp(-2,23));
        pointArr->addControlPoint(ccp(66,57));
    CCLoadSprite::doResourceByCommonIndex(507, true);
        for (int i = 0; i < 3; i++) {
            auto bannerSprite = CCFlagWaveSprite::create(CCLoadSprite::loadResource("1212_double.png"));
            if (CCCommonUtils::isIosAndroidPad()) {
                bannerSprite->setScale(0.25);
            }
            else
                bannerSprite->setScale(0.18);
            bannerSprite->setPosition(ccp(_tile_width, _tile_height)+pointArr->getControlPointAtIndex(i) + ccp(12, 20));
            m_picNode->addChild(bannerSprite);
        }
    CCLoadSprite::doResourceByCommonIndex(507, false);
    
    if (CCCommonUtils::isIosAndroidPad()) {
        m_cancelBtn = CCControlButton::create(CCLoadSprite::createScale9Sprite("btn_yellow.png"));
        m_cancelBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(AllianceAreaPopupView::onCancelBtnClick), CCControlEventTouchUpInside);
        m_cancelBtn->setPreferredSize(CCSize(160, 70));
        m_cancelBtn->setScaleX(2.1);
        m_cancelBtn->setScaleY(1.7);
        
        m_confirmBtn = CCControlButton::create(CCLoadSprite::createScale9Sprite("btn_gold.png"));
        m_confirmBtn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("btn_gold.png"),CCControlStateHighlighted);
        m_confirmBtn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("Btn_grey.png"),CCControlStateDisabled);
        m_confirmBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(AllianceAreaPopupView::onConfirmBtnClick), CCControlEventTouchUpInside);
        m_confirmBtn->setPreferredSize(CCSize(160, 70));
        m_confirmBtn->setScaleX(2.1);
        m_confirmBtn->setScaleY(1.7);
        
        auto bg = CCLoadSprite::createSprite("technology_11.png");
        m_picNode->addChild(bg);

        m_btnLabel = CCLabelIF::create(_lang("115302").c_str());
        m_btnLabel->setFontSize(32);
        m_btnLabel->setColor(ccc3(181, 162, 119));
        m_btnLabel1 = CCLabelIF::create(_lang("115021").c_str());
        m_btnLabel1->setFontSize(32);
        m_btnLabel1->setColor(ccc3(181, 162, 119));
        m_picNode->addChild(m_cancelBtn);
        m_cancelBtn->setPosition(_tile_width - m_cancelBtn->getContentSize().width * m_cancelBtn->getScaleX() / 2 - 15, 0);
        m_picNode->addChild(m_confirmBtn);
        m_confirmBtn->setPosition(_tile_width + m_confirmBtn->getContentSize().width * m_confirmBtn->getScaleX() / 2 + 15, m_cancelBtn->getPositionY());
        m_picNode->addChild(m_btnLabel);
        m_btnLabel->setPosition(m_confirmBtn->getPosition());
        m_picNode->addChild(m_btnLabel1);
        m_btnLabel1->setPosition(m_cancelBtn->getPosition());
        
        bg->setPosition(ccp(_tile_width, m_cancelBtn->getPositionY()));
        bg->setScaleY(8);
        bg->setScaleX(5);
    }
    else {
        m_cancelBtn = CCControlButton::create(CCLoadSprite::createScale9Sprite("btn_yellow.png"));
        CCCommonUtils::setButtonTitleColor(m_cancelBtn, ccc3(181, 162, 119));
        CCCommonUtils::setButtonTitle(m_cancelBtn, _lang("115021").c_str());
        m_cancelBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(AllianceAreaPopupView::onCancelBtnClick), CCControlEventTouchUpInside);
        m_cancelBtn->setPreferredSize(CCSize(160, 70));
        
        m_confirmBtn = CCControlButton::create(CCLoadSprite::createScale9Sprite("btn_gold.png"));
        m_confirmBtn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("btn_gold.png"),CCControlStateHighlighted);
        m_confirmBtn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("Btn_grey.png"),CCControlStateDisabled);
        m_confirmBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(AllianceAreaPopupView::onConfirmBtnClick), CCControlEventTouchUpInside);
        m_confirmBtn->setPreferredSize(CCSize(160, 70));
        CCCommonUtils::setButtonTitleColor(m_confirmBtn, ccc3(181, 162, 119));
        CCCommonUtils::setButtonTitle(m_confirmBtn, _lang("115302").c_str());
        
        auto bg = CCLoadSprite::createSprite("technology_11.png");
        m_picNode->addChild(bg);
        
        m_iconNode = CCNode::create();
        int num = WorldController::getInstance()->m_bAAreaCost;
        std::string picPath = "";
        //    picPath = "ui_gold.png";
        //
        //
        //    auto sprite = CCLoadSprite::createSprite(picPath.c_str());
        //    m_iconNode->addChild(sprite);
        //    CCCommonUtils::setSpriteMaxSize(sprite, 40);
        
        m_numText = CCLabelIF::create(CC_ITOA(num));
        //    m_btnLabel = CCLabelIF::create(_lang("115302").c_str());
        //    m_btnLabel->setFontSize(22);
        //    m_btnLabel->setColor(ccc3(255, 230, 116));
        m_picNode->addChild(m_cancelBtn);
        m_cancelBtn->setPosition(_tile_width - m_cancelBtn->getContentSize().width / 2 - 15, 0);
        m_picNode->addChild(m_confirmBtn);
        m_confirmBtn->setPosition(_tile_width + m_confirmBtn->getContentSize().width / 2 + 15, m_cancelBtn->getPositionY());
        
        //    m_numBG = CCLoadSprite::createSprite("num_text_bg.png");
        //    m_picNode->addChild(m_numBG);
        //    m_numBG->setScaleX(1.1);
        
        bg->setPosition(ccp(_tile_width, m_cancelBtn->getPositionY()));
        bg->setScaleY(4);
        bg->setScaleX(3);
        m_picNode->addChild(m_iconNode);
        m_iconNode->setPosition(m_confirmBtn->getPositionX() - 30, m_confirmBtn->getPositionY() - 15);
        //    m_picNode->addChild(m_numText);
        //    m_numText->setAnchorPoint(ccp(0, 0.5));
        //    m_numText->setPosition(m_confirmBtn->getPositionX(), m_confirmBtn->getPositionY() - 15);
        //    m_picNode->addChild(m_btnLabel);
        //    m_btnLabel->setPosition(m_confirmBtn->getPositionX(), m_confirmBtn->getPositionY() + 20);
        //    m_numBG->setPosition(ccp(m_confirmBtn->getPositionX(), m_confirmBtn->getPositionY() - 15));
    }
    
    this->addChild(m_picNode, 2);
    m_showNode = CCNode::create();
    this->addChild(m_showNode, 1);
    refreshShowNode(false);
    return true;
}
CCPoint AllianceAreaPopupView::getMovePoint(){
    auto size = CCDirector::sharedDirector()->getWinSize();
    int top = size.height - (_tile_height * 2 + 140);
    int down = 140 + DOWN_HEIGHT;
    int left = 0;
    int right = size.width - (_tile_width * 2);
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

void AllianceAreaPopupView::refreshShowNode(bool isDrag/* = true*/){
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
                                            , CCCallFuncN::create(this, callfuncN_selector(AllianceAreaPopupView::reDraw))
                                            , NULL
                                            );
            getMapNode()->runAction(seque);
        }
    }else{
        reDraw(NULL);
    }
}
void AllianceAreaPopupView::reDraw(CCNode* obj){
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
    auto centerPt = m_picNode->getPosition() + ccp(_tile_width, _tile_height / 2);
    float s = WorldMapView::instance()->m_map->getScale();
    centerPt = (WorldMapView::instance()->m_map->getPosition() - centerPt) / s;
    auto mapPt1 = WorldMapView::instance()->m_map->getTilePointByViewPoint(-centerPt);
    currentCenterPt = mapPt1;
    auto mapPt2 = mapPt1 + ccp(-1, 0);
    auto mapPt3 = mapPt1 + ccp(-1, -1);
    auto mapPt4 = mapPt1 + ccp(0, -1);
    auto pt1 = WorldMapView::instance()->m_map->getPosition() + WorldMapView::instance()->m_map->getViewPointByTilePoint(mapPt1) * s;
    auto pt2 = WorldMapView::instance()->m_map->getPosition() + WorldMapView::instance()->m_map->getViewPointByTilePoint(mapPt2) * s;
    auto pt3 = WorldMapView::instance()->m_map->getPosition() + WorldMapView::instance()->m_map->getViewPointByTilePoint(mapPt3) * s;
    auto pt4 = WorldMapView::instance()->m_map->getPosition() + WorldMapView::instance()->m_map->getViewPointByTilePoint(mapPt4) * s;
    bool canRelocateFlag1 = isCanRelocationTile(mapPt1);
    bool canRelocateFlag2 = isCanRelocationTile(mapPt2);
    bool canRelocateFlag3 = isCanRelocationTile(mapPt3);
    bool canRelocateFlag4 = isCanRelocationTile(mapPt4);
    
    std::vector<CCPoint> vector;
    vector.push_back(mapPt1);
    vector.push_back(mapPt2);
    vector.push_back(mapPt3);
    vector.push_back(mapPt4);
    
    if(canRelocateFlag1 && canRelocateFlag2 && canRelocateFlag3 && canRelocateFlag4){
        bool isInSame = isInRelicAndNormalBlock(vector);
        if(!isInSame){
            canRelocateFlag1 = canRelocateFlag2 = canRelocateFlag3 = canRelocateFlag4 = false;
        }
    }
    addPic(m_showNode, pt1, (canRelocateFlag1 ? "green_block.png" : "red_block.png"), 1);
    addPic(m_showNode, pt2, (canRelocateFlag2 ? "green_block.png" : "red_block.png"), 2);
    addPic(m_showNode, pt3, (canRelocateFlag3 ? "green_block.png" : "red_block.png"), 3);
    addPic(m_showNode, pt4, (canRelocateFlag4 ? "green_block.png" : "red_block.png"), 4);
    enableBtn(canRelocateFlag1 && canRelocateFlag2 && canRelocateFlag3 && canRelocateFlag4);
}
void AllianceAreaPopupView::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
    m_confirmBtn->setTouchPriority(0);
    m_cancelBtn->setTouchPriority(0);
}
void AllianceAreaPopupView::onExit(){
    PopupBaseView::onExit();
    setTouchEnabled(false);
}

bool AllianceAreaPopupView::onTouchBegan(CCTouch* pTouch,CCEvent* pEvent){
    if(isTouchInside(m_picNode, pTouch)){
        isMoving = true;
        movingStartPt = m_picNode->getPosition();
    }
    return true;
}
void AllianceAreaPopupView::onTouchMoved(CCTouch* pTouch,CCEvent* pEvent){
    if( !isMoving){
        return;
    }
    
    CCPoint addPt = pTouch->getLocation() - pTouch->getStartLocation();
    m_picNode->setPosition(movingStartPt + addPt);
    refreshShowNode();
}
void AllianceAreaPopupView::onTouchEnded(CCTouch* pTouch,CCEvent* pEvent){
    if(!isTouchInside(m_picNode, pTouch) && !isMoving && !isTouchInside(m_confirmBtn, pTouch)){
        this->closeSelf();
    }
    isMoving = false;
}

void AllianceAreaPopupView::onCancelBtnClick(CCObject* pSender, CCControlEvent event){
    this->closeSelf();
}
void AllianceAreaPopupView::onConfirmBtnClick(CCObject* pSender,CCControlEvent event){
        doBuy();
//    int gold = WorldController::getInstance()->m_bAAreaCost;
//    if(CCCommonUtils::isEnoughResourceByType(Gold, gold)){
//        YesNoDialog::showButtonAndGold( _lang("104919").c_str() , CCCallFunc::create(this, callfunc_selector(AllianceAreaPopupView::doBuy)), _lang("104906").c_str(), gold);
//    }else{
//        YesNoDialog::gotoPayTips();
//    }
}
void AllianceAreaPopupView::enableBtn(bool b){
    this->m_confirmBtn->setEnabled(b);
}
bool AllianceAreaPopupView::isCanRelocationTile(CCPoint &pt){
    int index = WorldController::getIndexByPoint(pt);
    if(!WorldController::getInstance()->isInMap(pt.x, pt.y)){
        return false;
    }
    if(WorldController::getInstance()->getCityInfos()[index].m_aArea_id != ""){
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
    if(WorldController::getInstance()->getCityInfos()[index].m_isInAArea){
        return false;
    }
    return true;
}
void AllianceAreaPopupView::doBuy(){
    unsigned int index = WorldController::getIndexByPoint(currentCenterPt);
    WorldAllianceAreaCreateCommand*  cmd = new WorldAllianceAreaCreateCommand(index,m_aareaId);
    cmd->sendAndRelease();
    
//    WorldAllianceAreaCancelCommand *cancelCmd = new WorldAllianceAreaCancelCommand(index,uuid);
//    cancelCmd->sendAndRelease();
    closeSelf();
    
}
float AllianceAreaPopupView::getMapScale(){
    float _scale = 1;
    if (WorldMapView::instance() && WorldMapView::instance()->m_map) {
        _scale = WorldMapView::instance()->m_map->getScale();
    }
    return _scale;
}
CCPoint AllianceAreaPopupView::getInLayerPoint(CCPoint & tilePoint){
    if (WorldMapView::instance() && WorldMapView::instance()->m_map) {
        return WorldMapView::instance()->m_map->getPosition() + WorldMapView::instance()->m_map->getViewPointByTilePoint(tilePoint) * getMapScale();
    }
    return CCPointZero;
    
}
CCNode* AllianceAreaPopupView::getMapNode(){
    if (WorldMapView::instance() && WorldMapView::instance()->m_map) {
        return WorldMapView::instance()->m_map;
    }
    return CCNode::create();
}
bool AllianceAreaPopupView::isInRelicAndNormalBlock(std::vector<CCPoint> &vector){
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