//
//  ServerListPopUpView.cpp
//  IF
//
//  Created by 李锐奇 on 14-10-13.
//
//

#include "ServerListPopUpView.h"
#include "GetAllServerListCommand.h"
#include "PopupViewController.h"
#include "SceneController.h"
#include "WorldMapView.h"
#include "UIComponent.h"
#include "SoundController.h"
#include "CCFlagWaveSprite.h"
#include "WorldBannerRankView.h"
ServerListPopUpView *ServerListPopUpView::create(){
    ServerListPopUpView *ret = new ServerListPopUpView();
    if(ret && ret->init()){
        ret->autorelease();
    }
    return ret;
}

void ServerListPopUpView::onEnter(){
    PopupBaseView::onEnter();
    WorldMapView::instance()->m_touchDelegateView->notMove = true;
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                                      callfuncO_selector(ServerListPopUpView::doWhenDataBack),
                                                                      MSG_SERVER_LIST_BACK
                                                                      , NULL);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
    if(WorldMapView::instance()){
        WorldMapView::instance()->m_touchDelegateView->notMove = true;
    }
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void ServerListPopUpView::onExit(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    setTouchEnabled(false);
    WorldMapView::instance()->m_touchDelegateView->notMove = false;
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SERVER_LIST_BACK);
    PopupBaseView::onExit();
}

bool ServerListPopUpView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_returnBtn, pTouch)){
        return true;
    }
//    else if(isTouchInside(m_bannerStaticsBtn, pTouch))
//    {
//        return true;
//    }
    return false;
}

void ServerListPopUpView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_returnBtn, pTouch)){
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        PopupViewController::getInstance()->goBackPopupView();
        UIComponent::getInstance()->showPopupView(UIPopupViewType_Minimap_ServerList);
    }
//    else if(isTouchInside(m_bannerStaticsBtn, pTouch))
//    {
//        // TODO:LH 控制国家统计面板的显示与隐藏
//        PopupViewController::getInstance()->addPopupInView(WorldBannerRankView::create(0));
//    }
}

bool ServerListPopUpView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        totalW = 0;
        totalH = 0;
        addCityNum = 0;
        m_jumpId = -1;
        m_serverToDraw = CCArray::create();
        this->setModelLayerDisplay(false);
        CCLoadSprite::doResourceByCommonIndex(10, true);
        CCLoadSprite::doResourceByCommonIndex(208, true);
        
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(10, false);
            CCLoadSprite::doResourceByCommonIndex(208, false);
        });
        
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        
        m_container = CCNode::create();
        this->addChild(m_container);
        
        m_bgBatch = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("BD_1.png")->getTexture(), 448);
        m_lineBatch = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("BD_1.png")->getTexture(), 448);
        m_iconBatch = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("BD_1.png")->getTexture(), 448);
        m_flagPoleBatch = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("flag_banner.png")->getTexture(), 448);
        m_flagBatch = CCNode::create();
        m_serverNameNode = CCNode::create();
        m_pointerNode = CCNode::create();
        
        m_container->addChild(m_bgBatch, 1);
        m_container->addChild(m_lineBatch, 2);
        m_container->addChild(m_flagPoleBatch, 3);
        m_container->addChild(m_flagBatch, 4);
        m_container->addChild(m_iconBatch, 5);
        m_container->addChild(m_serverNameNode, 6);
        m_container->addChild(m_pointerNode, 7);
        
        
        m_returnBtn = CCLoadSprite::createSprite("bnt_02.png");
        this->addChild(m_returnBtn);
        m_returnBtn->setPosition(ccp(m_returnBtn->getContentSize().width / 2, 60));
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_returnBtn->setScale(2.f);
            m_returnBtn->setPosition(ccp(m_returnBtn->getContentSize().width, 60+45));
        }
        
        
        auto btnSprite2 = CCLoadSprite::createSprite("fanhui.png");
        this->addChild(btnSprite2);
        if (CCCommonUtils::isIosAndroidPad())
        {
            btnSprite2->setScale(2.f);
        }
        btnSprite2->setPosition(m_returnBtn->getPosition());
        
        // TODO:LH 国家统计按钮在这里添加
//        m_bannerStaticsBtn = CCLoadSprite::createSprite("bnt_01.png");
//        this->addChild(m_bannerStaticsBtn);
//        m_bannerStaticsBtn->setPosition(ccp(640-m_bannerStaticsBtn->getContentSize().width / 2, 60));
//        
//        auto btnSprite3 = CCLoadSprite::createSprite("icon_tongji.png");
//        this->addChild(btnSprite3);
//        btnSprite3->setPosition(m_bannerStaticsBtn->getPosition());

        
        if(WorldController::getInstance()->m_serverList.size() == 0){
            xNum = 10;
            yNum = 10;
            showBG();
            WorldController::getInstance()->getServerList();
        }else{
            doWhenDataBack(NULL);
        }
        addTitle();
        ret = true;
    }
    return ret;
}

void ServerListPopUpView::addTitle(){
    auto size = CCDirector::sharedDirector()->getWinSize();
    auto pos = ccp(size.width / 2, size.height - 40);
    if (CCCommonUtils::isIosAndroidPad())
    {
        pos = ccp(size.width / 2, size.height - 80);
    }
    auto sprite1 = CCLoadSprite::createSprite("UI-tishikuang.png");
    this->addChild(sprite1);
    auto sprite2 = CCLoadSprite::createSprite("UI-tishikuang.png");
    this->addChild(sprite2);
    auto sprite3 = CCLoadSprite::createSprite("UI-tishikuang.png");
    this->addChild(sprite3);
    auto sprite4 = CCLoadSprite::createSprite("UI-tishikuang.png");
    this->addChild(sprite4);
    sprite1->setPosition(pos);
    sprite1->setFlipY(true);
    sprite1->setFlipX(true);
    sprite1->setAnchorPoint(ccp(1, 0));
    sprite2->setPosition(pos);
    sprite2->setFlipY(true);
    sprite2->setAnchorPoint(ccp(0, 0));
    sprite3->setPosition(pos);
    sprite3->setAnchorPoint(ccp(1, 1));
    sprite3->setFlipX(true);
    sprite4->setPosition(pos);
    sprite4->setAnchorPoint(ccp(0, 1));
    m_title = CCLabelIF::create();
    this->addChild(m_title);
    m_title->setPosition(pos);
    m_title->setFontSize(24);

    m_title->setColor(ccc3(220, 180, 90));
    if (CCCommonUtils::isIosAndroidPad())
    {
//        sprite1->setScale(2.f);
//        sprite2->setScale(2.f);
//        sprite3->setScale(2.f);
//        sprite4->setScale(2.f);
        m_title->setFontSize(48);
    }
    else
    {
        sprite1->setScale(0.45);
        sprite2->setScale(0.45);
        sprite3->setScale(0.45);
        sprite4->setScale(0.45);
    }
   
    m_title->setString(_lang("108746"));
}

bool ServerListPopUpView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    return false;
}

void ServerListPopUpView::showBG(){
    this->m_bgBatch->removeAllChildren();    
    int indexX = 0;
    int indexY = 0;
    auto frame = CCLoadSprite::createSprite("BD_1.png");
    int cellW = frame->getContentSize().width;
    int cellH = frame->getContentSize().height;
    int totalX = ceil(1.0 * CELL_W * xNum / cellW);
    int totalY = ceil(1.0 * CELL_H * yNum / cellH);

    while (indexX < totalX) {
        indexY = 0;
        while (indexY < totalY) {
            auto sprite = CCLoadSprite::createSprite("BD_1.png");
            m_bgBatch->addChild(sprite);
            sprite->setAnchorPoint(ccp(0, 0));
            sprite->setPosition(ccp(indexX * cellW, indexY * cellH));
            indexY++;
        }
        indexX++;
    }
}

void ServerListPopUpView::doWhenDataBack(CCObject* obj){
    int num = WorldController::getInstance()->m_serverList.size();
    int unitNum = ceil(num * 1.0 / CITY_NUM_PER_UNIT);
    int totalNum = unitNum * NUM_PER_UNIT;
    xNum = ceil(sqrt(totalNum)) + 2 * ADD_NUM;
    xNum = (xNum + 1) / 2 * 2;
    yNum = xNum;
    totalW = xNum * CELL_W;
    totalH = yNum * CELL_H;
    
    if(!m_viewPort){
        m_viewPort = HFViewport::create();
        m_viewPort->setViewPortTarget(m_container);
        m_viewPort->setSceneSize(totalW, totalH);
        m_viewPort->setZoomLimitationMin(0.5);
        m_viewPort->setZoomLimitationMax(1.25);
        m_container->setScale(1);
        m_viewPort->setWorldBound(CCRect(0, 0, totalW, totalH));
        m_viewPort->setTouchDelegate(this);
        this->addChild(m_viewPort);
        showBG();
    }
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_container->setScale(1.2f);
        m_viewPort->setZoomLimitationMin(0.8);
        m_viewPort->setZoomLimitationMax(1.5);
    }
    centerPt = ccp(int(xNum / 2 * 3 / 2), int(xNum / 2 * 3 / 2));
    m_serverToDraw->removeAllObjects();
    
    //数据回来后按照距离自己服的远近排序
    std::map<int, CCArray*> map;
    int selfIndexX = 0;
    int selfIndexY = 0;
    for(auto it = WorldController::getInstance()->m_serverList.begin(); it != WorldController::getInstance()->m_serverList.end(); it++){
        if(it->serverId == GlobalData::shared()->playerInfo.selfServerId){
            selfIndexX = it->x;
            selfIndexY = it->y;
            break;
        }
    }
    
    for(auto it = WorldController::getInstance()->m_serverList.begin(); it != WorldController::getInstance()->m_serverList.end(); it++){
        int currentX = it->x;
        int currentY = it->y;
        int dis = abs(currentX - selfIndexX) + abs(currentY - selfIndexY);
        CCArray *arr = NULL;
        if(map.find(dis) != map.end()){
            arr = map[dis];
        }else{
            arr = CCArray::create();
            map[dis] = arr;
        }
        arr->addObject(CCInteger::create(it->serverId));
    }
    
    for (auto it = map.begin(); it != map.end(); it++) {
        m_serverToDraw->addObjectsFromArray(it->second);
    }
    m_serverToDraw->reverseObjects();
    update(0.0);
}

void ServerListPopUpView::addLines(){
    //add city path
    for (auto it = m_lines.begin(); it != m_lines.end(); it++) {
        int pathIndex = it->first;
        auto centerPt = getPointByIndex(pathIndex);
        auto addPath = [](CCSpriteBatchNode *batch, CCPoint pt1, CCPoint pt2, std::map<int, int> &map, int xNum, int firstIndex){
            int secondPtIndex = getIndexByPt(pt2, xNum);
            if(map.find(secondPtIndex) == map.end()){
                return;
            }
            if(pt1.x > pt2.x || pt1.y > pt2.y){//过滤重复的线
                return;
            }
            int tag = firstIndex * 10000 + secondPtIndex;
            if(batch->getChildByTag(tag)){
                return;
            }
            int x = int(pt1.x);
            int y = int(pt1.y);
            
            int index = y % 3 * 3 + x % 3 + 1;
            auto aPt = ccp(0, 0.5);
            if(pt1.x == pt2.x){
                index += 9;
                aPt = ccp(0.5, 0);
            }
            auto sprite = CCLoadSprite::createSprite(CCString::createWithFormat("path_%d.png", index)->getCString());
            batch->addChild(sprite);
            sprite->setTag(tag);
            auto realPt = getRealPt(pt1);
            auto additionalPt = ccp(0, 0);
            if(index == 9){
                additionalPt = ccp(0, -52);
            }
            if(index == 8){
                additionalPt = ccp(0, -50);
            }
            if(index == 7){
                additionalPt = ccp(0, 24);
            }
            if(index == 1){
                additionalPt = ccp(0, 20);
            }
            if(index == 3){
                additionalPt = ccp(0, -64);
            }
            if(index == 4){
                additionalPt = ccp(0, -28);
            }
            if(index == 5){
                additionalPt = ccp(0, -28);
            }
            if(index == 10){
                additionalPt = ccp(-47, 0);
            }
            if(index == 11){
                additionalPt = ccp(65, 0);
            }
            if(index == 12){
                additionalPt = ccp(-10, 0);
            }
            if(index == 13){
                additionalPt = ccp(60, 0);
            }
            if(index == 14){
                additionalPt = ccp(-10, 0);
            }
            if(index == 15){
                additionalPt = ccp(44, 0);
            }
            if(index == 16){
                additionalPt = ccp(34, 0);
            }
            if(index == 17){
                additionalPt = ccp(-30, 0);
            }
            if(index == 18){
                additionalPt = ccp(-10, 0);
            }
            
            sprite->setPosition(realPt + additionalPt);
            sprite->setAnchorPoint(aPt);
        };
        CCLOG("%f, %f", centerPt.x, centerPt.y);
        addPath(m_lineBatch, centerPt, centerPt + ccp(0, 1), m_lines, xNum, pathIndex);
        addPath(m_lineBatch, centerPt, centerPt + ccp(1, 0), m_lines, xNum, pathIndex);
    }
}

void ServerListPopUpView::refreshServerList(int drawNum){
    int beforAddNum = addCityNum;
    auto beforeTotalLine = m_lines.size();

    while (m_serverToDraw->count() > 0) {
        int serverId = dynamic_cast<CCInteger*>(m_serverToDraw->objectAtIndex(m_serverToDraw->count() - 1))->getValue();
        m_serverToDraw->removeLastObject();
        for (auto it = WorldController::getInstance()->m_serverList.begin(); it != WorldController::getInstance()->m_serverList.end(); it++) {
            if(it->serverId == serverId){
                addServerCell(*it);
                break;
            }
        }
        if(addCityNum - beforAddNum >= drawNum){
            break;
        }
    }
    
    auto afterTotalLine = m_lines.size();
    if(beforeTotalLine != afterTotalLine){
        addLines();
    }
}

void ServerListPopUpView::update(float delta){
    //showAndHide
    int num = WorldController::getInstance()->m_serverList.size();
    if(addCityNum < num){
        refreshServerList();
    }
    auto arr = m_serverNameNode->getChildren();
    int index = 0;
    auto size = CCDirector::sharedDirector()->getWinSize();
    int addX = SHOW_DISTANCE;
    int addY = SHOW_DISTANCE;
    
    while (index < arr.size()) {
        auto node = dynamic_cast<CCNode*>(arr.at(index));
        if(node && node->getParent()){
            CCPoint p = node->getPosition();
            CCPoint p2 = m_serverNameNode->convertToWorldSpace(p);
            if(p2.x < -addX || p2.x > size.width + addX || p2.y < -addY || p2.y > size.height + addY){
                node->setVisible(false);
            }else{
                node->setVisible(true);
            }
        }
        index++;
    }
    
    auto arr2 = m_flagBatch->getChildren();
    index = 0;
    while (index < arr2.size()) {
        auto node = dynamic_cast<CCNode*>(arr2.at(index));
        if(node && node->getParent()){
            CCPoint p = node->getPosition();
            CCPoint p2 = m_flagBatch->convertToWorldSpace(p);
            if(p2.x < -addX || p2.x > size.width + addX || p2.y < -addY || p2.y > size.height + addY){
                node->setVisible(false);
            }else{
                node->setVisible(true);
            }
        }
        index++;
    }
}

void ServerListPopUpView::addServerCell(ServerListInfo &info){
    if(m_iconBatch->getChildByTag(info.serverId)){
        return;
    }
    auto pt = ccp(info.x, info.y);
    auto realPt = getRealPt(pt);
    auto size = CCDirector::sharedDirector()->getWinSize();

    if(info.serverId == GlobalData::shared()->playerInfo.selfServerId){
        m_viewPort->scrollTo(-realPt * m_container->getScale() + ccp(size.width / 2, size.height / 2), NULL, false);
    }

    int index = getIndexByPt(pt,  xNum);
    m_lines[index] = 1;
    
    auto sprite = CCLoadSprite::createSprite(CCString::createWithFormat("server_%d.png", info.cityIconIndex)->getCString());
    sprite->setTag(info.serverId);
    m_iconBatch->addChild(sprite);
    sprite->setPosition(realPt);
    
    auto nameBG = CCLoadSprite::createSprite("BD_servername.png");
    m_iconBatch->addChild(nameBG, 1);
    nameBG->setPosition(sprite->getPosition() + ccp(0, -70));
    
    auto nameBG1 = CCLoadSprite::createSprite("BD_servername.png");
    m_iconBatch->addChild(nameBG1, 1);
    nameBG1->setPosition(sprite->getPosition() + ccp(0, -70));
        
    int dx = 20;
    
    auto name = CCLabelIF::create(info.serverName.c_str());
    name->setAnchorPoint(ccp(0, 0.5));
    name->setAlignment(kCCTextAlignmentLeft);
    name->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
    name->setColor(ccc3(246, 237, 216));
    name->setFontSize(24);
    m_serverNameNode->addChild(name);
    name->setPosition(nameBG->getPositionX() - nameBG->getContentSize().width / 2 + dx, nameBG->getPositionY());
    
    std::string kingName = "";
    std::string banner = "";
    std::string titleStr = _lang("108758");
    titleStr = info.serverName;
    kingName = info.kingName;
    banner = info.banner;
    
    int posY = nameBG->getPositionY() - nameBG->getContentSize().height / 2 - 2;
    
    //    if(kingName != ""){
    
    auto kingNameBG = CCLoadSprite::createSprite("BD_servername.png");
    kingNameBG->setScaleY(1.2);
    kingNameBG->setOpacity(127);
    m_iconBatch->addChild(kingNameBG, 1);
    kingNameBG->setPosition(ccp(nameBG->getPositionX(), posY - kingNameBG->getContentSize().height / 2));
    
    std::string kingNameStr = kingName;
    if(kingNameStr == ""){
        kingNameStr = _lang("108714");
    }
    auto nameLabel = CCLabelIF::create(_lang_1("110025", kingNameStr.c_str()));
    nameLabel->setAnchorPoint(ccp(0, 0.5));
    nameLabel->setAlignment(kCCTextAlignmentLeft);
    nameLabel->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
    nameLabel->setColor(ccc3(253,228,149));
    nameLabel->setFontSize(24);
    m_serverNameNode->addChild(nameLabel);
    nameLabel->setPosition(nameBG->getPositionX() - nameBG->getContentSize().width / 2 + dx, kingNameBG->getPositionY());

    if(banner != "" && kingName != "" && CCCommonUtils::isShowFlagForChinese()){
        auto pole = CCLoadSprite::createSprite("flag_banner.png");
        m_flagPoleBatch->addChild(pole);
        if(info.cityIconIndex == 2 || info.cityIconIndex == 1){
            pole->setPosition(sprite->getPosition() + ccp(sprite->getContentSize().width / 2 - 15, 20));
        }else{
            pole->setPosition(sprite->getPosition() + ccp(sprite->getContentSize().width / 2 - 25, 20));
        }
        pole->setAnchorPoint(ccp(0, 1));

        //大陆地区，看到台湾的，都成了大陆的国旗
        if((banner=="TW" || banner=="HK") && CCCommonUtils::checkTaiWanFlag()){
            banner = "CN";
        }
        banner = CCCommonUtils::changeChinaFlag(banner);
        auto cf = CCLoadSprite::getSF((banner + ".png").c_str());
        if (!cf) {
            banner = "UN";
            CCLog("have no banner = %s",banner.c_str());
        }
        auto bannerSprite = CCFlagWaveSprite::create(CCLoadSprite::loadResource((banner + ".png").c_str()));
        m_flagBatch->addChild(bannerSprite);
        bannerSprite->setAnchorPoint(ccp(0, 1));
        float s = 0.25;
        bannerSprite->setScale(s);
        bannerSprite->setPosition(pole->getPosition() + ccp(0, 3));
    }
    
    if(info.serverId == GlobalData::shared()->playerInfo.selfServerId){
        auto selfPointer = CCLoadSprite::createSprite("world_occupy_self.png");
        selfPointer->setAnchorPoint(ccp(0.5, 0));
        selfPointer->setScale(0.7);
        selfPointer->setPosition(sprite->getPosition() + ccp(0, sprite->getContentSize().height / 2 + 5));
        m_pointerNode->addChild(selfPointer);
    }
    if(info.isHot){
        auto newSprite = CCLoadSprite::createSprite("BD_new.png");
        m_iconBatch->addChild(newSprite, 1);
        int x = name->getPositionX() + name->getContentSize().width * name->getOriginScaleX() + newSprite->getContentSize().width / 2 + 5;
        newSprite->setPosition(ccp(x, nameBG->getPositionY()));
        
        auto newLabel = CCLabelIF::create(_lang("108753").c_str());
        newLabel->setFontSize(18);
        m_serverNameNode->addChild(newLabel);
        newLabel->setPosition(newSprite->getPosition());
    }
    addCityNum++;
}

bool ServerListPopUpView::onBeginScroll(const cocos2d::CCPoint& worldLocation){
    
}

void ServerListPopUpView::onStepScroll(const cocos2d::CCPoint& worldLocation,const cocos2d::CCPoint& delta){

}

void ServerListPopUpView::onEndScroll(const cocos2d::CCPoint& worldLocation){
    
}

void ServerListPopUpView::onSingleTouchBegin(const cocos2d::CCPoint& worldLocation){
    
}

void ServerListPopUpView::onSingleTouchEnd(const cocos2d::CCPoint& worldLocation){
    int index = 0;
    int serverId = -1;
    CCPoint pt = ccp(0, 0);
    while (index < m_iconBatch->getChildrenCount()){
        auto node = dynamic_cast<CCNode*>(m_iconBatch->getChildren().at(index));
        auto size = node->getContentSize();
        CCRect rect = CCRect(node->getPositionX() - size.width / 2, node->getPositionY() - size.height / 2, size.width, size.height);
        if(rect.containsPoint(worldLocation)){
            serverId = node->getTag();
            pt = node->getPosition();
            break;
        }
        index++;
    }
    if(serverId != -1){
        SoundController::sharedSound()->playEffects(Music_Sfx_city_castle);
        m_jumpId = serverId;
        WorldMapView::instance()->addCover();
        CCCallFunc *fun = NULL;
        auto size = CCDirector::sharedDirector()->getWinSize();
        fun = CCCallFunc::create(this, callfunc_selector(ServerListPopUpView::gotoWorld));
        m_viewPort->scrollTo(-pt * m_container->getScale() + ccp(size.width / 2, size.height / 2), fun, true);
    }
}

void ServerListPopUpView::gotoWorld(){
    PopupViewController::getInstance()->removeAllPopupView();
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
        if(WorldMapView::instance() && WorldMapView::instance()->m_map){
            if(m_jumpId == GlobalData::shared()->playerInfo.currentServerId){
                WorldMapView::instance()->gotoTilePoint(WorldController::getInstance()->selfPoint, m_jumpId, true);
            }else{
                WorldMapView::instance()->gotoTilePoint(ccp(600, 600), m_jumpId, true);
            }
        }
    }else{
        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD);
    }
}

void ServerListPopUpView::onStepZoom(const cocos2d::CCPoint& point1, const cocos2d::CCPoint& point2){

}

void ServerListPopUpView::onBeginZoom(const CCPoint& point1,const CCPoint& point2){
    
}

void ServerListPopUpView::onEndZoom(float targetZoom){
    
}

CCPoint ServerListPopUpView::getRealPt(CCPoint pt){
    int x = int(pt.x);
    int y = int(pt.y);
    int index = y % 3 * 3 + x % 3;
    return ccp(x / 3 * CELL_W * 2, y / 3 * CELL_H * 2) + _index_to_point[index];
}

int ServerListPopUpView::getIndexByPt(CCPoint &pt, int xNum){
    return ceil(pt.y - 1) * (xNum * 3 / 2) + ceil(pt.x - 1);
}

CCPoint ServerListPopUpView::getPointByIndex(int index){
    return ccp(index % (xNum * 3 / 2) + 1, index / (xNum * 3 / 2) + 1);
}

//

ChangeServerCover *ChangeServerCover::create(int serverId){
    ChangeServerCover *ret = new ChangeServerCover();
    if(ret && ret->init(serverId)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ChangeServerCover::init(int serverId){
    bool ret = false;
    for (auto it = WorldController::getInstance()->m_serverList.begin(); it != WorldController::getInstance()->m_serverList.end(); it++) {
        if (it->serverId == serverId) {
                CCLoadSprite::doResourceByCommonIndex(10, true);
                setCleanFunction([](){
                    CCLoadSprite::doResourceByCommonIndex(10, false);
                });
                auto size = CCDirector::sharedDirector()->getWinSize();
                this->setContentSize(size);
                
                auto iconBG = CCLoadSprite::createSprite("loading_revolve.png");
                this->addChild(iconBG);
                iconBG->setPosition(ccp(size.width / 2, size.height / 2 + 100));
                iconBG->runAction(CCRotateTo::create(2.0, 200));
                
                int iconIndex = it->cityIconIndex;
                std::string picName = CCString::createWithFormat("server_%d.png", iconIndex)->getCString();
                auto icon = CCLoadSprite::createSprite(picName.c_str());
                this->addChild(icon);
                icon->setPosition(iconBG->getPosition());
                
                int index = 0;
                while (index < 2) {
                    auto sprite = CCLoadSprite::createScale9Sprite("UI_world_Resources.png");
                    sprite->setInsetBottom(47);
                    sprite->setInsetRight(47);
                    sprite->setInsetLeft(44);
                    sprite->setInsetTop(44);
                    sprite->setPreferredSize(CCSize(320, 90));
                    sprite->setPosition(iconBG->getPosition() - ccp(0, iconBG->getContentSize().height / 2 + sprite->getContentSize().height / 2 - 8));
                    this->addChild(sprite);
                    index++;
                }
                
                auto enterLabel = CCLabelIF::create(_lang("108756").c_str());
                this->addChild(enterLabel);
                enterLabel->setColor(ccc3(206,155,74));
                enterLabel->setFontSize(30);
                enterLabel->setPosition(iconBG->getPosition() - ccp(0, iconBG->getContentSize().height / 2 + 15));
                
                std::string serverName = it->serverName;
                auto nameLabel = CCLabelIF::create(serverName.c_str());
                nameLabel->setColor(ccc3(231,226,183));
                nameLabel->setFontSize(32);
                this->addChild(nameLabel);
                nameLabel->setPosition(enterLabel->getPosition() - ccp(0, 40));
                
                this->scheduleOnce(schedule_selector(ChangeServerCover::onTimeOut), 2.0);
                
                ret = true;
            
            break;
        }
    }
    return ret;
}

void ChangeServerCover::onEnter(){
    CCNode::onEnter();
}

void ChangeServerCover::onExit(){
    CCNode::onExit();
}

void ChangeServerCover::onTimeOut(float _time){
    if (this->getParent()) {
        this->removeFromParent();
    }
}