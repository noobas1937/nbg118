//
//  DynamicTiledMap.cpp
//  IF
//
//  Created by gj on 10/12/15.
//
//

#include "DynamicTiledMap.h"

#include "UIComponent.h"
#include "SceneController.h"
#include "WorldController.h"
#include "WorldMapView.h"
#include "IFMapMaskLayer.h"

#define WORLD_CONTROLLER   WorldController::getInstance()
#define WORLD_MAP_VIEW     WorldMapView::instance()

DynamicTiledMap* DynamicTiledMap::create(const char *tmxFile,const CCPoint& pos, int loopSize) {
    
    DynamicTiledMap *pRet = new DynamicTiledMap();
    pRet->isChangingServer = false;
    pRet->currentTilePoint = pos;
    pRet->lastDisplayTilePoint = pos;
    pRet->centerTilePoint = ccp((int)(WORLD_CONTROLLER->_current_tile_count_x/2),(int)(WORLD_CONTROLLER->_current_tile_count_y/2));
    pRet->centerViewPoint = ccp(WORLD_CONTROLLER->_current_map_width/2, WORLD_CONTROLLER->_current_map_height/2);
    CCPoint serverViewPoint =  WORLD_CONTROLLER->getServerViewPosByPos(WORLD_CONTROLLER->getServerPosById(GlobalData::shared()->playerInfo.currentServerId));
    if(WORLD_CONTROLLER->currentMapType == SERVERFIGHT_MAP){
        serverViewPoint = ccp(0, 0);
    }
    //todo liudi
    //        pRet->initWithTMXFile(tmxFile,pos,serverViewPoint, loopSize)
    if (pRet->initWithTMXFile(tmxFile,pos,serverViewPoint, loopSize))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

DynamicTiledMap::~DynamicTiledMap()
{
}

void DynamicTiledMap::setPosition(const cocos2d::CCPoint &position) {
    CCNode::setPosition(position);
    if(WORLD_MAP_VIEW->m_touchDelegateView){
        WORLD_MAP_VIEW->m_touchDelegateView->setMovable(true);
    }
    if (this->getTag() == WM_BETWEEN_SERVER_MAP_TAG || getTag() == WM_BG_TAG) {
        return;
    }
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    
    float mapScaleX = WORLD_MAP_VIEW->m_map->getScaleX();
    float mapScaleY = WORLD_MAP_VIEW->m_map->getScaleY();
    int oldCurrentServerId = GlobalData::shared()->playerInfo.currentServerId;
    int nowCurrentServerId = -1;
    if (WORLD_CONTROLLER->currentMapType == NORMAL_MAP) {
        nowCurrentServerId = WORLD_MAP_VIEW->m_map->getServerIdByViewPoint(ccp((winsize.width / 2 - position.x)/mapScaleX , (winsize.height / 2 - position.y)/mapScaleY));
    }
    
    CCPoint needMovePos = CCPointZero;
    if (oldCurrentServerId != nowCurrentServerId   && nowCurrentServerId != -1) {
        
        auto oldPos = WORLD_CONTROLLER->getServerPosById(oldCurrentServerId);
        auto newPos = WORLD_CONTROLLER->getServerPosById(nowCurrentServerId);
        auto disPos = newPos - oldPos;
        
        CCLOG("change to serverId =%d",nowCurrentServerId);
        needMovePos = WORLD_CONTROLLER->getChangeViewPosByServerPos(disPos,true);
        needMovePos.x =  needMovePos.x * mapScaleX;
        needMovePos.y =  needMovePos.y * mapScaleY;
        CCPoint shouldToPos = WORLD_MAP_VIEW->m_map->getPosition() + needMovePos;
        CCLOG("shouldGotoPoint:%f,%f",shouldToPos.x,shouldToPos.y);
        auto  gotoPoint = WORLD_MAP_VIEW->m_map->getTilePointByViewPoint(ccp((winsize.width / 2 - shouldToPos.x)/mapScaleX , (winsize.height / 2 - shouldToPos.y)/mapScaleY));
        if (gotoPoint.x < 0) {
            gotoPoint.x = 0;
        }
        if (gotoPoint.y < 0) {
            gotoPoint.y = 0;
        }
        if (gotoPoint.x > _tile_count_x - 1) {
            gotoPoint.x = _tile_count_x - 1;
        }
        if (gotoPoint.y > _tile_count_x - 1) {
            gotoPoint.y = _tile_count_x - 1;
        }
        PopupViewController::getInstance()->removeAllPopupView();
        WORLD_MAP_VIEW->changeServer(nowCurrentServerId);
        WORLD_MAP_VIEW->m_map->setPosition(shouldToPos);
        updateDynamicMap();
        
        WORLD_MAP_VIEW->onCloseTilePopup();
        
        if(WORLD_MAP_VIEW->m_touchDelegateView){
            WORLD_MAP_VIEW->m_touchDelegateView->setMovable(false);
        }
        CCLOG("testwdz:%f,%f",gotoPoint.x,gotoPoint.y);
        onShowMapMask();
        
        
        return;
    }
    if(WORLD_MAP_VIEW && SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
        WORLD_MAP_VIEW->showAndHideUnBatchLabel();
        WORLD_MAP_VIEW->showAndHideFieldMonster();
        WORLD_MAP_VIEW->onShowAndHideSpinLb();
    }
    bool needUpdate = false;
    if (isNeedUpdate()) {
        updateDynamicMap();
        needUpdate = true;
    }
    
    if (needUpdate && WORLD_CONTROLLER->currentMapType != SERVERFIGHT_MAP) {
        vector<CCPoint> viewPointVec;
        CCPoint lbPos = ccp((0 - position.x)/mapScaleX , (0 - position.y)/mapScaleY);
        CCPoint ltPos = ccp((0 - position.x)/mapScaleX , (winsize.height - position.y)/mapScaleY);
        CCPoint rtPos = ccp((winsize.width - position.x)/mapScaleX , (winsize.height - position.y)/mapScaleY);
        CCPoint rbPos = ccp((winsize.width - position.x)/mapScaleX , (0 - position.y)/mapScaleY);
        viewPointVec.push_back(lbPos);
        viewPointVec.push_back(ltPos);
        viewPointVec.push_back(rtPos);
        viewPointVec.push_back(rbPos);
        
        map<int ,CCPoint> ServerPointMap;
        int index = 0;
        while (index < viewPointVec.size()) {
            int tempServerId = WORLD_MAP_VIEW->m_map->getServerIdByViewPoint(viewPointVec.at(index));
            if (tempServerId != GlobalData::shared()->playerInfo.currentServerId) {
                ServerPointMap[tempServerId] = viewPointVec.at(index);
            }
            index++;
        }
        
        auto it = ServerPointMap.begin();
        for (; it!= ServerPointMap.end(); it++) {
            CCPoint tilePoint = WORLD_MAP_VIEW->m_map->getTilePointByViewPoint(it->second);
            if(WORLD_CONTROLLER->getisAsyEnd()){
                updataBoderMap(tilePoint, it->first);
            }
            else{
                auto tempVec = WORLD_CONTROLLER->m_helpCmdVec;
                if (tempVec.size() == 0) {
                    WORLD_CONTROLLER->m_helpCmdVec.push_back(make_pair(it->first, tilePoint));
                }
                else{
                    auto tempIt = WORLD_CONTROLLER->m_helpCmdVec.begin();
                    for (; tempIt != WORLD_CONTROLLER->m_helpCmdVec.end(); ) {
                        if (tempIt->first == it->first) {
                            WORLD_CONTROLLER->m_helpCmdVec.erase(tempIt);
                            break;
                        }
                        tempIt++;
                    }
                    WORLD_CONTROLLER->m_helpCmdVec.push_back(make_pair(it->first, tilePoint));
                }
                
            }
        }
    }
}
void DynamicTiledMap::onShowMapMask(){
    int nowCurrentServerId = GlobalData::shared()->playerInfo.currentServerId;
    for (int index = 0; index < 9; index++) {
        CCPoint changeServerPos = WORLD_CONTROLLER->getChangePointByTypeNum(index);
        CCPoint maskServerPos = WorldController::getServerPosById(nowCurrentServerId)  + changeServerPos;
        int serverId = WorldController::getServerIdByServerPoint(maskServerPos);
        IFMapMaskLayer * tempNode  = dynamic_cast<IFMapMaskLayer*>(WORLD_MAP_VIEW->m_mapMaskNode->getChildByTag(WM_MASK_TAG + index));
        if(tempNode){
            CCPoint movePoint = WORLD_CONTROLLER->getChangeViewPosByServerPos(changeServerPos,true);
            tempNode->setPosition(movePoint + ccp(0, _map_height /2)+ WORLD_CONTROLLER->getServerViewPosByPos(WorldController::getServerPosById(GlobalData::shared()->playerInfo.currentServerId)));
            if (WORLD_CONTROLLER->currentMapType == SERVERFIGHT_MAP) {
                tempNode->setVisible(false);
            }else{
                if(GlobalData::shared()->isCrossService){
                    if (serverId == GlobalData::shared()->playerInfo.crossFightSrcServerId) {
                        tempNode->setVisible(false);
                    }else{
                        tempNode->setVisible(true);
                    }
                }
                else{
                    if (serverId == GlobalData::shared()->playerInfo.selfServerId) {
                        tempNode->setVisible(false);
                    }else{
                        tempNode->setVisible(true);
                    }
                }
                
            }
            
        }
    }
}
bool DynamicTiledMap::isNeedUpdate() {
    // this assume anchorPoint is (0,0)
    if(!WORLD_MAP_VIEW || !WORLD_MAP_VIEW->m_touchDelegateView){
        return false;
    }
    WORLD_MAP_VIEW->updateDirection();
    
    auto newViewPoint = WORLD_MAP_VIEW->m_touchDelegateView->getCurrentCenterGridPoint();
    auto tilePoint = getTilePointByViewPoint(newViewPoint);
    
    auto testPoint = ccpSub(lastDisplayTilePoint, tilePoint);
    if ((int)(testPoint.x) == 0 && (int)(testPoint.y) == 0) {
        return false;
    }
    
    CCPoint displayPoint = tilePoint;
    displayPoint.x = displayPoint.x > 0 ? (displayPoint.x > WORLD_CONTROLLER->_current_tile_count_x - 1 ? WORLD_CONTROLLER->_current_tile_count_x - 1 : displayPoint.x) : 0;
    displayPoint.y = displayPoint.y > 0 ? (displayPoint.y > WORLD_CONTROLLER->_current_tile_count_y - 1 ? WORLD_CONTROLLER->_current_tile_count_y - 1 : displayPoint.y) : 0;
    
    UIComponent::getInstance()->m_xCoordText->setString(CC_ITOA((int)displayPoint.x));
    UIComponent::getInstance()->m_yCoordText->setString(CC_ITOA((int)displayPoint.y));
    UIComponent::getInstance()->m_zCoordText->setString(CC_ITOA(GlobalData::shared()->playerInfo.currentServerId));
    UIComponent::getInstance()->setSerchCoordState();
    
    lastDisplayTilePoint = tilePoint;
    
    static float xTest = 5;
    static float yTest = 10;
    auto updateTestPoint = ccpSub(currentTilePoint, tilePoint);
    float x = updateTestPoint.x-updateTestPoint.y;
    float y = -updateTestPoint.x-updateTestPoint.y;
    //    CCLOG("old point %f,%f",currentTilePoint.x,currentTilePoint.y);
    //    CCLOG("log newpoint%f,%f",tilePoint.x,tilePoint.y);
    if (fabsf(x) > xTest || fabs(y) > yTest) {
        currentTilePoint = tilePoint;
        return true;
    }
    return false;
}

CCPoint DynamicTiledMap::getTilePointByViewPoint(const cocos2d::CCPoint &viewPoint) {
    auto newViewPoint = viewPoint;
    auto disPoint = ccpSub(newViewPoint,WORLD_CONTROLLER->serverMap_centerViewPoint);
    float x = disPoint.x/(_big_mapWidth/2);
    float y = disPoint.y/(_big_mapHeight/2);
    
    int a = x + y;
    int b = y - x;
    
    if (a > 0) {
        a = (a+1)/2;
    } else if (a < 0) {
        a = (a-1)/2;
    }
    
    if (b > 0) {
        b = (b+1)/2;
    } else if (b < 0) {
        b = (b-1)/2;
    }
    int currentServerId = GlobalData::shared()->playerInfo.currentServerId;
    CCPoint currentServerPos = WorldController::getServerPosById(currentServerId);
    
    CCPoint serverTilePoint = ccp((int)(currentServerPos.x) + a,(int)(currentServerPos.y) + b);
    //    int serverId = WORLD_CONTROLLER->getServerIdByServerPoint(serverTilePoint);
    //elem map左下角0.0 原來的1201view为座標系
    auto childMapPos = WORLD_CONTROLLER->getServerViewPosByPos(serverTilePoint);
    return getTileMapPointByViewPoint(newViewPoint - childMapPos);//old fun
}

int DynamicTiledMap::getServerIdByViewPoint(const CCPoint &viewPoint){
    auto newViewPoint = viewPoint;
    auto disPoint = ccpSub(newViewPoint,WORLD_CONTROLLER->serverMap_centerViewPoint);
    double x = (double)(disPoint.x)/(_big_mapWidth/2);
    double y =(double)(disPoint.y)/(_big_mapHeight/2);
    
    int a = x + y;
    int b = y - x;
    
    if (a > 0) {
        a = (a+1)/2;
    } else if (a < 0) {
        a = (a-1)/2;
    }
    
    if (b > 0) {
        b = (b+1)/2;
    } else if (b < 0) {
        b = (b-1)/2;
    }
    auto &playerInfo = GlobalData::shared()->playerInfo;
    auto centerPoint = WORLD_CONTROLLER->getServerPosById(playerInfo.currentServerId);
    CCPoint serverTilePoint = ccp((int)(centerPoint.x) + a,(int)(centerPoint.y) + b);
    int serverId = WORLD_CONTROLLER->getServerIdByServerPoint(serverTilePoint);
    return serverId;
}

CCPoint DynamicTiledMap::getTileMapPointByViewPoint(const CCPoint &viewPoint){
    auto newViewPoint = viewPoint;
    auto disPoint = ccpSub(newViewPoint,centerViewPoint);
    
    float x = disPoint.x/_halfTileSize.width;
    float y = disPoint.y/_halfTileSize.height;
    
    int a = x - y;
    int b = -x - y;
    
    if (a > 0) {
        a = (a+1)/2;
    } else if (a < 0) {
        a = (a-1)/2;
    }
    
    if (b > 0) {
        b = (b+1)/2;
    } else if (b < 0) {
        b = (b-1)/2;
    }
    
    return ccp((int)(centerTilePoint.x) + a,(int)(centerTilePoint.y) + b);
}

CCPoint DynamicTiledMap::getViewPointByTilePoint(const cocos2d::CCPoint &tilePoint,int forceServerId/* = -1*/) {
    int tileIndex = WorldController::getIndexByPoint(tilePoint);
    int serverId = GlobalData::shared()->playerInfo.currentServerId;
    if(forceServerId != -1){
        serverId = forceServerId;
    }
    CCPoint tempPoint = tilePoint;
    bool needFixServer = false;
    if (tilePoint.x < 0) {
        tempPoint.x = 0;
        needFixServer = true;
    }
    if (tilePoint.y < 0) {
        tempPoint.y = 0;
        needFixServer = true;
    }
    if (tilePoint.x > WORLD_CONTROLLER->_current_tile_count_x - 1) {
        tempPoint.x = WORLD_CONTROLLER->_current_tile_count_x - 1;
        needFixServer = true;
    }
    if (tilePoint.y > WORLD_CONTROLLER->_current_tile_count_y - 1) {
        tempPoint.y = WORLD_CONTROLLER->_current_tile_count_y - 1;
        needFixServer = true;
    }
    if (needFixServer) {
        int tempCityIndex = WorldController::getIndexByPoint(tempPoint);
        if (WORLD_CONTROLLER->getCityInfos().find(tempCityIndex) != WORLD_CONTROLLER->getCityInfos().end() ) {
            serverId = WORLD_CONTROLLER->getCityInfos()[tempCityIndex].tileServerId;
        }
    }
    //    CCLOG("logwdz+++++++++++++%d",serverId);
    auto disPoint = ccpSub(tilePoint, centerTilePoint);
    CCPoint childMapPoint = ccp(centerViewPoint.x+(disPoint.x-disPoint.y)*_halfTileSize.width,centerViewPoint.y-(disPoint.x+disPoint.y)*_halfTileSize.height);
    CCPoint serverViewPoint =  WORLD_CONTROLLER->getServerViewPosByPos(WorldController::getServerPosById(serverId));
    return serverViewPoint + childMapPoint;
}

void DynamicTiledMap::updataBoderMap(CCPoint point,int forceServerId){
    if (!WORLD_MAP_VIEW) {
        return;
    }
    if(GlobalData::shared()->playerInfo.uid == ""){
        return;
    }
    unsigned int type = 0;
    if(!GuideController::share()->updateWorldInfo){
        return;
    }
    WORLD_MAP_VIEW->m_updateTimeStamp = time(NULL);
    if (point.x < 0) {
        point.x = 0;
    }
    if (point.y < 0) {
        point.y = 0;
    }
    if (point.x > _tile_count_x - 1) {
        point.x = _tile_count_x - 1;
    }
    if (point.y > _tile_count_x - 1) {
        point.y = _tile_count_x - 1;
    }
    auto cmd = new WorldCommand(point,WORLD_MAP_VIEW->m_updateTimeStamp,type,forceServerId);
    int cmdSid = cmd->getServerId();
    bool isNormalMap =    WORLD_CONTROLLER->currentMapType == NORMAL_MAP ? true :false;
    if(cmdSid != 0){
        if (isNormalMap && GlobalData::shared()->serverMax < cmdSid) {
            WORLD_CONTROLLER->setisAsyEnd(true);
            return;
        }
        cmd->sendAndRelease();
        isSendCmd = true;
        WORLD_MAP_VIEW->m_loadingIcon->setVisible(true);
    }
    else{
        WORLD_CONTROLLER->setisAsyEnd(true);
#if COCOS2D_DEBUG == 2
        cmd->sendAndRelease();
        isSendCmd = true;
        WORLD_MAP_VIEW->m_loadingIcon->setVisible(true);
        WORLD_CONTROLLER->setisAsyEnd(false);
#endif
    }
    
}

void DynamicTiledMap::updateDynamicMap(CCPoint point) {
    if (!WORLD_MAP_VIEW) {
        return;
    }
    if(GlobalData::shared()->playerInfo.uid == ""){
        return;
    }
    unsigned int type = 1; // 0 : normal 1: single point
    if (point.x < 0 || point.y < 0) {
        point = currentTilePoint;
        type = 0;
        CCPoint serverViewPoint =  WORLD_CONTROLLER->getServerViewPosByPos(WORLD_CONTROLLER->getServerPosById(GlobalData::shared()->playerInfo.currentServerId));
        if(WORLD_CONTROLLER->currentMapType == SERVERFIGHT_MAP){
            serverViewPoint = ccp(0, 0);
        }
        
        updateMap(currentTilePoint, serverViewPoint);
        auto node = this->getChildByTag(WM_BETWEEN_SERVER_MAP_TAG);
        if(node)
        {
            auto map = dynamic_cast<DynamicTiledMap*>(node->getChildByTag(WM_BETWEEN_SERVER_MAP_TAG));
            if (map)
            {
                map->updateMap(currentTilePoint, serverViewPoint);
                updateOctopus();
            }
        }
        
        auto bg_node = this->getChildByTag(WM_BG_TAG);
        if (bg_node)
        {
            auto map = dynamic_cast<DynamicTiledMap*>(bg_node->getChildByTag(WM_BG_TAG));
            if (map)
            {
                map->updateMap(currentTilePoint, serverViewPoint);
            }
        }
    }
    if(!GuideController::share()->updateWorldInfo){
        return;
    }
    // do other update request here
    WORLD_MAP_VIEW->m_updateTimeStamp = time(NULL);
    int limitX = WORLD_CONTROLLER->_current_tile_count_x;
    int limitY = WORLD_CONTROLLER->_current_tile_count_y;
    if (point.x < 0) {
        point.x = 0;
    }
    if (point.y < 0) {
        point.y = 0;
    }
    if (point.x > limitX - 1) {
        point.x = limitX - 1;
    }
    if (point.y > limitY - 1) {
        point.y = limitY - 1;
    }
    auto cmd = new WorldCommand(point,WORLD_MAP_VIEW->m_updateTimeStamp,type);
    int cmdSid = cmd->getServerId();
    bool isNormalMap =    WORLD_CONTROLLER->currentMapType == NORMAL_MAP ? true :false;
    if(cmdSid != 0){
        if (isNormalMap && GlobalData::shared()->serverMax < cmdSid) {
            WORLD_CONTROLLER->setisAsyEnd(true);
            return;
        }
        cmd->sendAndRelease();
        isSendCmd = true;
        if (type == 0) {
            WORLD_MAP_VIEW->m_loadingIcon->setVisible(true);
        }
    }
    else{
        WORLD_CONTROLLER->setisAsyEnd(true);
#if COCOS2D_DEBUG == 2
        cmd->sendAndRelease();
        isSendCmd = true;
        WORLD_MAP_VIEW->m_loadingIcon->setVisible(true);
        WORLD_CONTROLLER->setisAsyEnd(false);
#endif
    }
}

class OctopusNode
: public Node
{
public:
    static OctopusNode * create()
    {
        OctopusNode * ret = new (std::nothrow) OctopusNode();
        if (ret && ret->init())
        {
            ret->autorelease();
        }
        else
        {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
    
    Sprite* octopus;
    float show_time;
};
void DynamicTiledMap::updateOctopus()
{
    static const int SEA_MONSTER_AREA_X = 5;
    static const int SEA_MONSTER_AREA_Y = 10;
    static const int SEA_MONSTER_TAG = WM_BETWEEN_SERVER_MAP_TAG + 1;
    static const int SEA_MONSTER_GZ = -2;
    
    srand((unsigned int)time(0));
    int random_variable = rand() % 100;
    if (random_variable > 75) return;
    
    bool isX = abs(currentTilePoint.x - _tile_count_x) <= SEA_MONSTER_AREA_X;
    bool isY = abs(currentTilePoint.y - _tile_count_y) <= SEA_MONSTER_AREA_Y;
    if (!isX && !isY)
    {
        return;
    }
    
    int tx = _tile_count_x + 2;
    int ty = _tile_count_y + 2;
    float sx = 2 + rand() % 4;
    float sy = sx;
    float speedx = -10;
    float speedy = 5;
    float speedScale = 1.2;
    random_variable = rand() % 100;
    int offset_tile = 0;//(rand() % 2) * (random_variable > 50 ? 1 : -1);
    if (isX && !isY)
    {
        ty = currentTilePoint.y + offset_tile;
        
        if (random_variable > 50)
        {
            tx = _tile_count_x + 1;
            sx *= 1;
            sy *= -1;
            
            speedx *= speedScale;
            speedy *= -speedScale;
        }
        else
        {
            tx = _tile_count_x + 2;
            sx *= -1;
            sy *= 1;
            
            speedx *= -speedScale;
            speedy *= speedScale;
        }
    }
    else if (!isX && isY)
    {
        tx = currentTilePoint.x + offset_tile;
        
        if (random_variable > 50)
        {
            ty = _tile_count_y + 1;
            sx *= -1;
            sy *= -1;
            
            speedx *= -speedScale;
            speedy *= -speedScale;
        }
        else
        {
            ty = _tile_count_y + 2;
            sx *= 1;
            sy *= 1;
            
            speedx *= speedScale;
            speedy *= speedScale;
        }
    }
    
    auto fPos = WORLD_MAP_VIEW->m_map->getViewPointByTilePoint(Vec2(tx, ty));
    auto mSprite_pos = fPos;
    auto mSprite = OctopusNode::create();
    mSprite->setGlobalZOrder(-1.5);
    mSprite->setScaleX(sx);
    mSprite->setScaleY(sy);
    int random_offset_y = 128 / 2 + rand() % 30;
    mSprite->setPosition(mSprite_pos.x, mSprite_pos.y + random_offset_y);
    mSprite->setTag(SEA_MONSTER_TAG);
    WORLD_MAP_VIEW->m_layers[WM_BETWEEN_SERVER_MAP]->addChild(mSprite);
    
    mSprite->octopus = Sprite::createWithSpriteFrameName("anima_sea_monster_01.png");
    mSprite->octopus->setOpacity(0);
    mSprite->octopus->setGlobalZOrder(-1.5);
    mSprite->addChild(mSprite->octopus);
    auto spriteCache = SpriteFrameCache::getInstance();
    spriteCache->addSpriteFramesWithFile("World/World_5.plist");
    Vector<SpriteFrame*> vsp;
    for (int i = 1; i <= 8; i++)
    {
        String *string = String::createWithFormat("anima_sea_monster_0%d.png", i);
        SpriteFrame *spfr = spriteCache->getSpriteFrameByName(string->getCString());
        vsp.pushBack(spfr);
    }
    Animation *animation = Animation::createWithSpriteFrames(vsp, 0.1);
    Animate *animate = Animate::create(animation);
    auto *ac1 = RepeatForever::create(animate);
    mSprite->octopus->runAction(ac1);
    
    int total_time = 12.0 + rand() % 8;
    mSprite->show_time = total_time;
    
    mSprite->schedule([mSprite, speedx, speedy, total_time](float dt){
        if (mSprite->show_time < 0)
        {
            mSprite->removeFromParent();
            return;
        }
        
        mSprite->setPositionX(mSprite->getPositionX() + dt * speedx);
        mSprite->setPositionY(mSprite->getPositionY() + dt * speedy);
        
        if (mSprite->show_time <= 2.0)
        {
            int o = 255.0 * mSprite->show_time / 2.0;
            mSprite->octopus->setOpacity(o);
        }
        else if (mSprite->octopus->getOpacity() <= 245)
        {
            mSprite->octopus->setOpacity(mSprite->octopus->getOpacity() + 10);
        }
        mSprite->show_time -= dt;
    }, "move");
}
