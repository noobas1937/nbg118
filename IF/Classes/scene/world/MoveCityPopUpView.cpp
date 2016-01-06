//
//  MoveCityPopUpView.cpp
//  IF
//
//  Created by 李锐奇 on 14-8-20.
//
//

#include "MoveCityPopUpView.h"
#include "WorldController.h"
#include "PopupViewController.h"
#include "WorldMapView.h"
#include "FunBuildController.h"
#include "YesNoDialog.h"
#include "AllianceManager.h"
#include "DynamicResourceController.h"
#include "WinPointsUseCommand.h"
#include "ShakeController.h"
#include "DynamicTiledMap.h"

MoveCityPopUpView *MoveCityPopUpView::create(MoveCityType cttype,int itemid){
    MoveCityPopUpView *ret = new MoveCityPopUpView();
    if(ret && ret->init(cttype,itemid)){
        ret->autorelease();
    }
    return ret;
}

bool MoveCityPopUpView::init(MoveCityType cttype, int itemid){
    if(!PopupBaseView::init()){
        return false;
    }
    setIsHDPanel(true);
    m_toolID = itemid;
    if(m_toolID>0){
        auto iter = ToolController::getInstance()->m_toolInfos.find(itemid);
        if(iter!=ToolController::getInstance()->m_toolInfos.end()){
            auto &tInfo = (*iter).second;
            if(tInfo.type==12){
                mCityType = MoveCity_Monster;
            }else{
                mCityType = (MoveCityType)(tInfo.type2 + 1);
            }
        }
    }else{
        mCityType = cttype;
    }
    
    setCleanFunction([](){
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
    });
    
    int tilecount = getTileCount();
    m_alMoveType = 0;
    m_tmpCnt = 0;
    m_moveCnt = 0;
    isMoving = false;
    this->setModelLayerOpacity(0);
    this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    isSendingRequest = false;
    m_picNode = CCNode::create();
    
    m_picNode->setContentSize(CCSize(_tile_width * tilecount, _tile_height * tilecount));
    m_picNode->setAnchorPoint(ccp(0, 0));

    m_picNode->setPosition(PopupViewController::getInstance()->getTouchPoint() + ccp(-_tile_width * tilecount * 0.5, -_tile_height * tilecount * 0.5));
    
    if (CCCommonUtils::isIosAndroidPad()) {
        m_cityNode = CCNode::create();
    }
    createCitySprite();
    createButton();
    
    m_showNode = CCNode::create();
    this->addChild(m_showNode, 1);
    refreshShowNode(false);
    return true;
}
int MoveCityPopUpView::getTileCount(){
    switch (mCityType) {
        case MoveCity_Castle:{
            return 2;
        }
        case MoveCity_Food:
        case MoveCity_Wood:
        case MoveCity_Iron:
        case MoveCity_Stone:
        case MoveCity_Gold:
        case MoveCity_Monster:{
            return 1;
        }
        default:
            break;
    }
    return 2;
}
void MoveCityPopUpView::createCitySprite(){
    string picName = "";
    switch (mCityType) {
        case MoveCity_Castle:{
            int level = FunBuildController::getInstance()->getMainCityLv();
            int mapIndex = 3;
            while (mapIndex >= 0) {
                auto arr = WorldMapView::getCityPicArr(mapIndex, level, GlobalData::shared()->playerInfo.officer == KINGDOM_KING_ID);
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
                m_cityNode->setPosition(-_tile_width * (WORLD_DEFAULT_HD_SCALE - 1), -_tile_height * (WORLD_DEFAULT_HD_SCALE - 1));
            }
            return;
        }
        case MoveCity_Food:{
            picName = "0020.png";
            break;
        }
        case MoveCity_Wood:{
            picName = "0017.png";
            break;
        }
        case MoveCity_Gold:{
            std::string iconName = CCCommonUtils::getPropById("100101", "resource");
            picName = (iconName == "")? "0021.png" : iconName + ".png";
            break;
        }
        case MoveCity_Iron:{
            picName = "0019.png";
            break;
        }
//        case MoveCity_Silver:{
//            picName = "0041.png";
//            break;
//        }
        case MoveCity_Stone:{
            picName = "0018.png";
            break;
        }
        case MoveCity_Monster:{
            picName = "item032.png";
            break;
        }
        default:
            break;
    }
    
    if(!picName.empty()){
        CCSprite* sprite = NULL;
        if (MoveCity_Monster == mCityType) {
           sprite = CCLoadSprite::createSprite(picName.c_str(), true, CCLoadSpriteType_GOODS);
        }else{
            sprite = CCLoadSprite::createSprite(picName.c_str());
        }
        sprite->setAnchorPoint(ccp(0, 0));
        if (CCCommonUtils::isIosAndroidPad()) {
            sprite->setScale(1.4);
        }
        m_picNode->addChild(sprite);
        CCSize sprSize = sprite->getContentSize();
        if (CCCommonUtils::isIosAndroidPad()) {
            sprSize = sprite->getContentSize() * sprite->getScale();
        }
        if(sprSize.width < _tile_width && sprSize.height < _tile_height){
            sprite->setPosition(CCPoint((_tile_width-sprSize.width)*0.5 ,(_tile_height-sprSize.height)*0.5));
        }else{
            sprite->setPosition(CCPointZero);
        }
    }
}
void MoveCityPopUpView::createButton(){
    if (CCCommonUtils::isIosAndroidPad()) {
        m_cancelBtn = CCControlButton::create(CCLoadSprite::createScale9Sprite("but_blue.png"));
        m_btnLabel1 = CCLabelIF::create(_lang("115021").c_str());
        m_btnLabel1->setFontSize(32);
        m_btnLabel1->setColor(ccc3(255, 255, 255));
        m_cancelBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(MoveCityPopUpView::onCancelClick), CCControlEventTouchUpInside);
        m_cancelBtn->setPreferredSize(CCSize(160, 70));
        m_cancelBtn->setScaleX(2.1);
        m_cancelBtn->setScaleY(1.7);
        
        m_confirmBtn = CCControlButton::create(CCLoadSprite::createScale9Sprite("btn_green3.png"));
        m_confirmBtn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("btn_green3.png"),CCControlStateHighlighted);
        m_confirmBtn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("Btn_grey.png"),CCControlStateDisabled);
        m_confirmBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(MoveCityPopUpView::onMoveCityClick), CCControlEventTouchUpInside);
        m_confirmBtn->setPreferredSize(CCSize(160, 70));
        m_confirmBtn->setScaleX(2.1);
        m_confirmBtn->setScaleY(1.7);
        
        auto bg = CCLoadSprite::createSprite("technology_11.png");
        m_picNode->addChild(bg);
        m_iconNode = CCNode::create();
        int num = getItemNum();
        std::string picPath = "";
        switch (mCityType) {
            case MoveCity_Food:
            case MoveCity_Wood:
            case MoveCity_Iron:
            case MoveCity_Stone:
            case MoveCity_Gold:
            case MoveCity_Monster:{
                num = 1;
                //            picPath = CCCommonUtils::getIcon(CC_ITOA(getMoveItemId()));
                CCCommonUtils::createGoodsIcon(getMoveItemId(), m_iconNode, CCSize(55,55));
                m_btnLabel = CCLabelIF::create(_lang("111525").c_str());
                break;
            }
            case MoveCity_Castle:
            default:{
                if(checkCanUseWinPoint()){
                    picPath = "icon_liansheng.png";
                    string skill_point = CCCommonUtils::getPropById(CC_ITOA(ITEM_SKILL_MOVE), "skill_point");
                    num = atoi(skill_point.c_str());
                }else if(num == 0){
                    picPath = "ui_gold.png";
                    num = CCCommonUtils::getGoldBuyItem(CC_ITOA(getMoveItemId()));
                }else{
                    num = 1;
                    picPath = CCCommonUtils::getIcon(CC_ITOA(getMoveItemId()));
                }
                auto sprite = CCLoadSprite::createSprite(picPath.c_str());
                m_iconNode->addChild(sprite);
                CCCommonUtils::setSpriteMaxSize(sprite, 55);
                m_btnLabel = CCLabelIF::create(_lang("108719").c_str());
                break;
            }
        }
        m_numText = CCLabelIF::create(CC_ITOA(num));
        
        m_btnLabel->setFontSize(32);
        m_btnLabel->setColor(ccc3(255, 230, 116));
        m_picNode->addChild(m_cancelBtn);
        float centerX = _tile_width * getTileCount() * 0.5;
        m_cancelBtn->setPosition(_tile_width - m_cancelBtn->getContentSize().width * m_cancelBtn->getScaleX() / 2 - 15, 0);
        m_picNode->addChild(m_confirmBtn);
        m_confirmBtn->setPosition(_tile_width + m_confirmBtn->getContentSize().width * m_confirmBtn->getScaleX() / 2 + 15, m_cancelBtn->getPositionY());
        m_picNode->addChild(m_btnLabel1);
        m_btnLabel1->setPosition(m_cancelBtn->getPosition());
        
        bg->setPosition(ccp(centerX, m_cancelBtn->getPositionY()));
        bg->setScaleY(8);
        bg->setScaleX(5);
        m_picNode->addChild(m_iconNode);
        m_iconNode->setPosition(m_confirmBtn->getPositionX() - 30, m_confirmBtn->getPositionY() - 15);
        m_picNode->addChild(m_numText);
        m_numText->setAnchorPoint(ccp(0, 0.5));
        m_numText->setPosition(m_confirmBtn->getPositionX(), m_confirmBtn->getPositionY() - 15);
        m_picNode->addChild(m_btnLabel);
        m_btnLabel->setPosition(m_confirmBtn->getPositionX(), m_confirmBtn->getPositionY() + 15);
        
        this->addChild(m_picNode, 2);
    }
    else {
        m_cancelBtn = CCControlButton::create(CCLoadSprite::createScale9Sprite("but_blue.png"));
        CCCommonUtils::setButtonTitleColor(m_cancelBtn, ccc3(255, 255, 255));
        CCCommonUtils::setButtonTitle(m_cancelBtn, _lang("115021").c_str());
        m_cancelBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(MoveCityPopUpView::onCancelClick), CCControlEventTouchUpInside);
        m_cancelBtn->setPreferredSize(CCSize(160, 70));
        //缩小按钮字体
        m_cancelBtn->setTitleTTFSizeForState(22, CCControlStateNormal);
        m_cancelBtn->setTitleTTFSizeForState(22, CCControlStateHighlighted);
        m_cancelBtn->setTitleTTFSizeForState(22, CCControlStateDisabled);
        
        m_confirmBtn = CCControlButton::create(CCLoadSprite::createScale9Sprite("btn_green3.png"));
        m_confirmBtn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("btn_green3.png"),CCControlStateHighlighted);
        m_confirmBtn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("Btn_grey.png"),CCControlStateDisabled);
        m_confirmBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(MoveCityPopUpView::onMoveCityClick), CCControlEventTouchUpInside);
        m_confirmBtn->setPreferredSize(CCSize(160, 70));
        
        auto bg = CCLoadSprite::createSprite("technology_11.png");
        m_picNode->addChild(bg);
        m_iconNode = CCNode::create();
        int num = getItemNum();
        std::string picPath = "";
        switch (mCityType) {
            case MoveCity_Food:
            case MoveCity_Wood:
            case MoveCity_Iron:
            case MoveCity_Stone:
            case MoveCity_Gold:
            case MoveCity_Monster:{
                num = 1;
                //            picPath = CCCommonUtils::getIcon(CC_ITOA(getMoveItemId()));
                CCCommonUtils::createGoodsIcon(getMoveItemId(), m_iconNode, CCSize(40,40));
                m_btnLabel = CCLabelIF::create(_lang("111525").c_str());
                break;
            }
            case MoveCity_Castle:
            default:{
                if(checkCanUseWinPoint()){
                    picPath = "icon_liansheng.png";
                    string skill_point = CCCommonUtils::getPropById(CC_ITOA(ITEM_SKILL_MOVE), "skill_point");
                    num = atoi(skill_point.c_str());
                }else if(num == 0){
                    picPath = "ui_gold.png";
                    num = CCCommonUtils::getGoldBuyItem(CC_ITOA(getMoveItemId()));
                }else{
                    num = 1;
                    picPath = CCCommonUtils::getIcon(CC_ITOA(getMoveItemId()));
                }
                auto sprite = CCLoadSprite::createSprite(picPath.c_str());
                m_iconNode->addChild(sprite);
                CCCommonUtils::setSpriteMaxSize(sprite, 40);
                m_btnLabel = CCLabelIF::create(_lang("108719").c_str());
                break;
            }
        }
        m_numText = CCLabelIF::create(CC_ITOA(num));
        
        m_btnLabel->setFontSize(22);
        m_btnLabel->setColor(ccc3(255, 230, 116));
        m_picNode->addChild(m_cancelBtn);
        float centerX = _tile_width * getTileCount() * 0.5;
        m_cancelBtn->setPosition(centerX - m_cancelBtn->getContentSize().width / 2 - 15, 0);
        m_picNode->addChild(m_confirmBtn);
        m_confirmBtn->setPosition(centerX + m_confirmBtn->getContentSize().width / 2 + 15, m_cancelBtn->getPositionY());
        
        bg->setPosition(ccp(centerX, m_cancelBtn->getPositionY()));
        bg->setScaleY(4);
        bg->setScaleX(3);
        m_picNode->addChild(m_iconNode);
        m_iconNode->setPosition(m_confirmBtn->getPositionX() - 30, m_confirmBtn->getPositionY() - 15);
        m_picNode->addChild(m_numText);
        m_numText->setAnchorPoint(ccp(0, 0.5));
        m_numText->setPosition(m_confirmBtn->getPositionX(), m_confirmBtn->getPositionY() - 15);
        m_picNode->addChild(m_btnLabel);
        m_btnLabel->setPosition(m_confirmBtn->getPositionX(), m_confirmBtn->getPositionY() + 15);
        
        this->addChild(m_picNode, 2);
    }
}

bool MoveCityPopUpView::checkCanUseWinPoint(){
    bool flag = false;
    string id = CCCommonUtils::getPropById(CC_ITOA(ITEM_SKILL_MOVE), "item_id");
    int itemId = atoi(id.c_str());
    string skill_point = CCCommonUtils::getPropById(CC_ITOA(ITEM_SKILL_MOVE), "skill_point");
    int point = atoi(skill_point.c_str());
    if(m_toolID==ITEM_SKILL_MOVE && GlobalData::shared()->playerInfo.winPoint >= point){
        flag = true;
    }
    return flag;
}

void MoveCityPopUpView::addMailUid(string mailid) {
    m_mailUid = AllianceManager::getInstance()->tmpMailUid;
}

void MoveCityPopUpView::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
    m_confirmBtn->setTouchPriority(0);
    m_cancelBtn->setTouchPriority(0);
    m_confirmBtn->setTouchPriority(Touch_Default);
    m_cancelBtn->setTouchPriority(Touch_Default);
}

void MoveCityPopUpView::onExit(){
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool MoveCityPopUpView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isSendingRequest){
        return false;
    }
    if(isTouchInside(m_picNode, pTouch)){
        isMoving = true;
        movingStartPt = m_picNode->getPosition();
    }
    return true;
}

void MoveCityPopUpView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    if(isSendingRequest || !isMoving){
        return;
    }
//    if (m_alMoveType == 2) {
//        return;
//    }
    CCPoint addPt = pTouch->getLocation() - pTouch->getStartLocation();
    if (m_alMoveType == 2) {
        if (ccpDistance(addPt, CCPointZero) > 1) {
            m_tmpCnt++;
            if (m_moveCnt < 1) {
                if (m_tmpCnt == 1) {
                    CCCommonUtils::flyHint("", "", _lang("115443"));
                }
            }
        }
        addPt = ccp(0, 0);
    }
    m_picNode->setPosition(movingStartPt + addPt);
    
    refreshShowNode();
}

void MoveCityPopUpView::reDraw(CCNode* obj){
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
    
    float s = WorldMapView::instance()->m_map->getScale();
    
    if(mCityType==MoveCity_Castle){
        auto centerPt = m_picNode->getPosition() + ccp(_tile_width, _tile_height / 2);
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
    }else{
        auto centerPt = m_picNode->getPosition()+ccp(_tile_width*0.5,_tile_height*0.5);
        centerPt = (WorldMapView::instance()->m_map->getPosition() - centerPt) / s;
        auto mapPt1 = WorldMapView::instance()->m_map->getTilePointByViewPoint(-centerPt);
        currentCenterPt = mapPt1;
        auto pt1 = WorldMapView::instance()->m_map->getPosition() + WorldMapView::instance()->m_map->getViewPointByTilePoint(mapPt1) * s;
        bool canRelocateFlag1 = isCanRelocationTile(mapPt1);
        std::vector<CCPoint> vector;
        vector.push_back(mapPt1);
        if(canRelocateFlag1){
            bool isInSame = isInRelicAndNormalBlock(vector);
            if(!isInSame){
                canRelocateFlag1 = false;
            }
        }
        addPic(m_showNode, pt1, (canRelocateFlag1 ? "green_block.png" : "red_block.png"), 1);
        enableBtn(canRelocateFlag1);
    }
}

CCPoint MoveCityPopUpView::getMovePoint(){
    auto size = CCDirector::sharedDirector()->getWinSize();
    int top = size.height - (_tile_height * getTileCount() + 140);
    int down = 140 + DOWN_HEIGHT;
    int left = 0;
    int right = size.width - (_tile_width * getTileCount());
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

void MoveCityPopUpView::refreshShowNode(bool isDrag/* = true*/){
    CCPoint pt = getMovePoint();
    if(!pt.equals(ccp(0, 0))){
        m_picNode->setPosition(m_picNode->getPosition() + pt);
        if(isDrag){
            float maxMove = 15;
            auto moveDis = sqrt(pow(pt.x, 2) + pow(pt.y, 2));
            if(moveDis > maxMove){
                pt = pt * (maxMove * 1.0f / moveDis);
            }

            WorldMapView::instance()->m_map->setPosition(WorldMapView::instance()->m_map->getPosition() + pt);
            reDraw(NULL);
        }else{
            auto seque = CCSequence::create(CCMoveTo::create(0.2, WorldMapView::instance()->m_map->getPosition() + pt)
                                            , CCCallFuncN::create(this, callfuncN_selector(MoveCityPopUpView::reDraw))
                                            , NULL
                                            );
            WorldMapView::instance()->m_map->runAction(seque);
        }
    }else{
        reDraw(NULL);
    }
}

void MoveCityPopUpView::enableBtn(bool b){
    this->m_confirmBtn->setEnabled(b);
}

bool MoveCityPopUpView::isInRelicAndNormalBlock(std::vector<CCPoint> &vector){
    int i = 0;
    bool isPartInRelic = false;
    bool isPartOutRelic = false;
    
    while (i < vector.size()) {
        CCPoint &pt = vector[i];
        int index = WorldController::getIndexByPoint(pt);
        std::string landType = WorldController::getInstance()->getCityInfos()[index].cityOriginType;
        if(landType == "neutrally"){
            isPartInRelic = true;
        }else{
            isPartOutRelic = true;
        }
        i++;
    }
    if(isPartInRelic && isPartOutRelic){
        return false;
    }
    return true;
}

bool MoveCityPopUpView::isCanRelocationTile(CCPoint &pt){
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
    if (m_alMoveType == 3) {
        if (!WorldController::getInstance()->getCityInfos()[index].m_isMyAArea) {
            return false;
        }
    }
    return true;
}

void MoveCityPopUpView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(!isTouchInside(m_picNode, pTouch) && !isMoving && !isTouchInside(m_confirmBtn, pTouch)){
        this->closeSelf();
    }
    isMoving = false;
    if (m_tmpCnt > 0) {
        m_moveCnt++;
        m_tmpCnt = 0;
    }
}

void MoveCityPopUpView::onCancelClick(CCObject *pSender, CCControlEvent event){
    this->closeSelf();
}

int MoveCityPopUpView::getItemNum(){
    switch (mCityType) {
        case MoveCity_Food:
        case MoveCity_Wood:
        case MoveCity_Gold:
        case MoveCity_Stone:
        case MoveCity_Iron:
        case MoveCity_Monster:{
            auto &tool = ToolController::getInstance()->getToolInfoById(getMoveItemId());
            return tool.getCNT();
        }
        case MoveCity_Castle:
        default:{
            int num = 0;
            auto &rewardTool = ToolController::getInstance()->getToolInfoById(getFreeMoveItemId());
            if (rewardTool.getCNT()>0) {
                num += rewardTool.getCNT();
            }
            
            auto &tool = ToolController::getInstance()->getToolInfoById(getMoveItemId());
            if (tool.getCNT()>0) {
                num += tool.getCNT();
            }
            return num;
        }
    }
    return 0;
}

void MoveCityPopUpView::onMoveCityClick(CCObject *pSender, CCControlEvent event){
    doClick(NULL);
}

void MoveCityPopUpView::doClick(CCObject* obj){
    unsigned int index = WorldController::getIndexByPoint(currentCenterPt);
    if(!WorldController::getInstance()->isCityCanSettle(index,getTileCount())){
        return;
    }
    if(mCityType == MoveCity_Castle){
        if(FunBuildController::getInstance()->getMainCityLv() <= _limit_cross_server){
            if(WorldController::getInstance()->isInFresherForbidenArea(index)
               || WorldController::getInstance()->isInFresherForbidenArea(index - 1)
               || WorldController::getInstance()->isInFresherForbidenArea(index - 1 - WorldController::getInstance()->_current_tile_count_x)
               || WorldController::getInstance()->isInFresherForbidenArea(index - WorldController::getInstance()->_current_tile_count_x)
               ){
                if(!GlobalData::shared()->playerInfo.isInSelfServer()){
                    CCCommonUtils::flyText(_lang("104948"));
                    return;
                }else{
                    if (m_alMoveType == 0) {
                        CCCommonUtils::flyText(_lang("108878"));
                        return;
                    }
                }
            }
        }
    }else{
        if(FunBuildController::getInstance()->getMainCityLv() <= _limit_cross_server){
            if(WorldController::getInstance()->isInFresherForbidenArea(index)){
                if(!GlobalData::shared()->playerInfo.isInSelfServer()){
                    CCCommonUtils::flyText(_lang("104948"));
                    return;
                }else{
                    if (m_alMoveType == 0) {
                        CCCommonUtils::flyText(_lang("108878"));
                        return;
                    }
                }
            }
        }
    }

    confirmMove();
}

void MoveCityPopUpView::confirmMove(){
    switch (mCityType) {
        case MoveCity_Food:
        case MoveCity_Wood:
        case MoveCity_Stone:
        case MoveCity_Gold:
        case MoveCity_Iron:
        case MoveCity_Monster:{
            if (getItemNum() > 0) {
                YesNoDialog::show( _lang("111524").c_str() , CCCallFunc::create(this, callfunc_selector(MoveCityPopUpView::useTool)));
                return;
            }
            break;
        }
        case MoveCity_Castle:
        default:{
            if(this->checkCanUseWinPoint()){
                unsigned int index = WorldController::getIndexByPoint(currentCenterPt);
                WinPointsUseCommand* cmd = new WinPointsUseCommand(CC_ITOA(m_toolID));
                cmd->putParam("point", CCInteger::create(index));
                cmd->sendAndRelease();
                PopupViewController::getInstance()->removePopupView(this);
            }else if (m_alMoveType > 0) {
                useTool();
            } else {
                if (getItemNum() > 0) {
                    YesNoDialog::show( _lang("108761").c_str() , CCCallFunc::create(this, callfunc_selector(MoveCityPopUpView::useTool)));
                    return;
                }
                int gold = CCCommonUtils::getGoldBuyItem(CC_ITOA(getMoveItemId()));
                if(CCCommonUtils::isEnoughResourceByType(Gold, gold)){
                    YesNoDialog::showButtonAndGold( _lang("104919").c_str() , CCCallFunc::create(this, callfunc_selector(MoveCityPopUpView::confirmBuy)), _lang("104906").c_str(), gold);
                }else{
                    YesNoDialog::gotoPayTips();
                }
            }
            break;
        }
    }
    
}

void MoveCityPopUpView::confirmBuy(){
    isSendingRequest = true;
    auto dict = CCDictionary::create();
    unsigned int index = WorldController::getIndexByPoint(currentCenterPt);
    dict->setObject(CCString::createWithFormat("%d",index), "point");
    if (m_mailUid != "") {
        dict->setObject(ccs(m_mailUid), "mailUid");
    }
    doWhenUseMoney(dict);
}
void MoveCityPopUpView::onUseTool()
{
    ToolController::getInstance()->buyTool(getMoveItemId(), 1, CCCallFunc::create(this, callfunc_selector(MoveCityPopUpView::useTool)),0,true);
}

void MoveCityPopUpView::useTool(){
    isSendingRequest = false;
    auto dict = CCDictionary::create();
    unsigned int index = WorldController::getIndexByPoint(currentCenterPt);
    dict->setObject(CCString::createWithFormat("%d",index), "point");
    
    switch (mCityType) {
        case MoveCity_Food:
        case MoveCity_Wood:
        case MoveCity_Stone:
        case MoveCity_Gold:
        case MoveCity_Iron:
        case MoveCity_Monster:{
            auto &tool = ToolController::getInstance()->getToolInfoById(getMoveItemId());
            if (tool.getCNT()>0) {
                dict->setObject(CCString::create(tool.uuid), "goodsId");
                doWhenHasItem(dict);
                return;
            }
            break;
        }
        case MoveCity_Castle:
        default:{
            if (m_alMoveType > 0) {
                auto& info = ToolController::getInstance()->getToolInfoById(ITEM_ALLIANCE_CITY_MOVE);
                if (info.getCNT() > 0) {
                    dict->setObject(CCString::create(info.uuid), "goodsId");
                    dict->setObject(CCString::create(CC_ITOA(m_alMoveType)), "alMoveType");
                    doWhenHasItem(dict);
                } else {
                    closeSelf();
                }
                return;
            }
            
            if (m_mailUid != "") {
                dict->setObject(ccs(m_mailUid), "mailUid");
            }
            
            auto &rewardTool = ToolController::getInstance()->getToolInfoById(getFreeMoveItemId());
            if (rewardTool.getCNT()>0) {
                dict->setObject(CCString::create(rewardTool.uuid), "goodsId");
                doWhenHasItem(dict);
                return;
            }
            
            auto &tool = ToolController::getInstance()->getToolInfoById(getMoveItemId());
            if (tool.getCNT()>0) {
                dict->setObject(CCString::create(tool.uuid), "goodsId");
                doWhenHasItem(dict);
                return;
            }
            break;
        }
    }
}

int MoveCityPopUpView::getFreeMoveItemId(){
    return ITEM_FREE_MOVE_CITY;
}

int MoveCityPopUpView::getMoveItemId(){
    switch (mCityType) {
        case MoveCity_Castle:
            return ITEM_MOVE_CITY;
        case MoveCity_Food:
        case MoveCity_Wood:
        case MoveCity_Iron:
        case MoveCity_Stone:
        case MoveCity_Gold:
        case MoveCity_Monster:
            return m_toolID;
        default:
            break;
    }
    return ITEM_MOVE_CITY;
}

void MoveCityPopUpView::doWhenHasItem(CCDictionary *dict){
    switch (mCityType) {
        case MoveCity_Food:
        case MoveCity_Iron:
        case MoveCity_Stone:
        case MoveCity_Gold:
        case MoveCity_Wood:
        case MoveCity_Monster:
            //todo
            WorldMapView::instance()->afterAddWorldItems(dict);
            break;
        case MoveCity_Castle:{
            WorldMapView::instance()->afterCityMove(dict);
            ShakeController::getInstance()->m_shakeDataVec.clear();
        default:
            break;
        }
    }
    
    this->closeSelf();
}

void MoveCityPopUpView::doWhenUseMoney(CCDictionary *dict){
    unsigned int index = dict->valueForKey("point")->intValue();
    auto cmd = new WorldCityMoveGoldCommand(index);  
    string mailUid = "";
    if (dict->objectForKey("mailUid")) {
        mailUid = dict->valueForKey("mailUid")->getCString();
        cmd->addMailUid(mailUid);
    }
    //    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(WorldMapView::onCityMoveCallback), NULL));
    cmd->sendAndRelease();
    ShakeController::getInstance()->m_shakeDataVec.clear();
    this->closeSelf();
}

void MoveCityPopUpView::setAlMoveType(int type)
{
    m_alMoveType = type;
    if (m_alMoveType > 0)
    {
        m_iconNode->removeAllChildrenWithCleanup(true);
        auto picPath = CCCommonUtils::getIcon(CC_ITOA(ITEM_ALLIANCE_CITY_MOVE));
        auto sprite = CCLoadSprite::createSprite(picPath.c_str());
        CCCommonUtils::setSpriteMaxSize(sprite, 40);
        m_iconNode->addChild(sprite);
        m_numText->setString("1");
        if (m_alMoveType == 2)
        {
            CCCommonUtils::flyHint("", "", _lang("115443"));
        }
    }
}
