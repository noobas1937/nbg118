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

DynamicTiledMap* DynamicTiledMap::create(const char *tmxFile,const CCPoint& pos, int loopSize) {
    
    DynamicTiledMap *pRet = new DynamicTiledMap();
    pRet->isChangingServer = false;
    pRet->currentTilePoint = pos;
    pRet->lastDisplayTilePoint = pos;
    pRet->centerTilePoint = ccp((int)(WorldController::getInstance()->_current_tile_count_x/2),(int)(WorldController::getInstance()->_current_tile_count_y/2));
    pRet->centerViewPoint = ccp(WorldController::getInstance()->_current_map_width/2, WorldController::getInstance()->_current_map_height/2);
    CCPoint serverViewPoint =  WorldController::getInstance()->getServerViewPosByPos(WorldController::getInstance()->getServerPosById(GlobalData::shared()->playerInfo.currentServerId));
    if(WorldController::getInstance()->currentMapType == SERVERFIGHT_MAP){
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

void DynamicTiledMap::setPosition(const cocos2d::CCPoint &position) {
    CCNode::setPosition(position);
    if(WorldMapView::instance()->m_touchDelegateView){
        WorldMapView::instance()->m_touchDelegateView->setMovable(true);
    }
    if (this->getTag() == WM_BETWEEN_SERVER_MAP_TAG || getTag() == WM_BG_TAG) {
        return;
    }
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    
    float mapScaleX = WorldMapView::instance()->m_map->getScaleX();
    float mapScaleY = WorldMapView::instance()->m_map->getScaleY();
    int oldCurrentServerId = GlobalData::shared()->playerInfo.currentServerId;
    int nowCurrentServerId = -1;
    if (WorldController::getInstance()->currentMapType == NORMAL_MAP) {
        nowCurrentServerId = WorldMapView::instance()->m_map->getServerIdByViewPoint(ccp((winsize.width / 2 - position.x)/mapScaleX , (winsize.height / 2 - position.y)/mapScaleY));
    }
    
    CCPoint needMovePos = CCPointZero;
    if (oldCurrentServerId != nowCurrentServerId   && nowCurrentServerId != -1) {
        
        auto oldPos = WorldController::getInstance()->getServerPosById(oldCurrentServerId);
        auto newPos = WorldController::getInstance()->getServerPosById(nowCurrentServerId);
        auto disPos = newPos - oldPos;
        
        CCLOG("change to serverId =%d",nowCurrentServerId);
        needMovePos = WorldController::getInstance()->getChangeViewPosByServerPos(disPos,true);
        needMovePos.x =  needMovePos.x * mapScaleX;
        needMovePos.y =  needMovePos.y * mapScaleY;
        CCPoint shouldToPos = WorldMapView::instance()->m_map->getPosition() + needMovePos;
        CCLOG("shouldGotoPoint:%f,%f",shouldToPos.x,shouldToPos.y);
        auto  gotoPoint = WorldMapView::instance()->m_map->getTilePointByViewPoint(ccp((winsize.width / 2 - shouldToPos.x)/mapScaleX , (winsize.height / 2 - shouldToPos.y)/mapScaleY));
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
        WorldMapView::instance()->changeServer(nowCurrentServerId);
        WorldMapView::instance()->m_map->setPosition(shouldToPos);
        updateDynamicMap();
        
        WorldMapView::instance()->onCloseTilePopup();
        
        if(WorldMapView::instance()->m_touchDelegateView){
            WorldMapView::instance()->m_touchDelegateView->setMovable(false);
        }
        CCLOG("testwdz:%f,%f",gotoPoint.x,gotoPoint.y);
        onShowMapMask();
        
        
        return;
    }
    if(WorldMapView::instance() && SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
        WorldMapView::instance()->showAndHideUnBatchLabel();
        WorldMapView::instance()->showAndHideFieldMonster();
        WorldMapView::instance()->onShowAndHideSpinLb();
    }
    bool needUpdate = false;
    if (isNeedUpdate()) {
        updateDynamicMap();
        needUpdate = true;
    }
    
    if (needUpdate && WorldController::getInstance()->currentMapType != SERVERFIGHT_MAP) {
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
            int tempServerId = WorldMapView::instance()->m_map->getServerIdByViewPoint(viewPointVec.at(index));
            if (tempServerId != GlobalData::shared()->playerInfo.currentServerId) {
                ServerPointMap[tempServerId] = viewPointVec.at(index);
            }
            index++;
        }
        
        auto it = ServerPointMap.begin();
        for (; it!= ServerPointMap.end(); it++) {
            CCPoint tilePoint = WorldMapView::instance()->m_map->getTilePointByViewPoint(it->second);
            if(WorldController::getInstance()->getisAsyEnd()){
                updataBoderMap(tilePoint, it->first);
            }
            else{
                auto tempVec = WorldController::getInstance()->m_helpCmdVec;
                if (tempVec.size() == 0) {
                    WorldController::getInstance()->m_helpCmdVec.push_back(make_pair(it->first, tilePoint));
                }
                else{
                    auto tempIt = WorldController::getInstance()->m_helpCmdVec.begin();
                    for (; tempIt != WorldController::getInstance()->m_helpCmdVec.end(); ) {
                        if (tempIt->first == it->first) {
                            WorldController::getInstance()->m_helpCmdVec.erase(tempIt);
                            break;
                        }
                        tempIt++;
                    }
                    WorldController::getInstance()->m_helpCmdVec.push_back(make_pair(it->first, tilePoint));
                }
                
            }
        }
    }
}
void DynamicTiledMap::onShowMapMask(){
    int nowCurrentServerId = GlobalData::shared()->playerInfo.currentServerId;
    for (int index = 0; index < 9; index++) {
        CCPoint changeServerPos = WorldController::getInstance()->getChangePointByTypeNum(index);
        CCPoint maskServerPos = WorldController::getServerPosById(nowCurrentServerId)  + changeServerPos;
        int serverId = WorldController::getServerIdByServerPoint(maskServerPos);
        IFMapMaskLayer * tempNode  = dynamic_cast<IFMapMaskLayer*>(WorldMapView::instance()->m_mapMaskNode->getChildByTag(WM_MASK_TAG + index));
        if(tempNode){
            CCPoint movePoint = WorldController::getInstance()->getChangeViewPosByServerPos(changeServerPos,true);
            tempNode->setPosition(movePoint + ccp(0, _map_height /2)+ WorldController::getInstance()->getServerViewPosByPos(WorldController::getServerPosById(GlobalData::shared()->playerInfo.currentServerId)));
            if (WorldController::getInstance()->currentMapType == SERVERFIGHT_MAP) {
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
    if(!WorldMapView::instance() || !WorldMapView::instance()->m_touchDelegateView){
        return false;
    }
    WorldMapView::instance()->updateDirection();
    
    auto newViewPoint = WorldMapView::instance()->m_touchDelegateView->getCurrentCenterGridPoint();
    auto tilePoint = getTilePointByViewPoint(newViewPoint);
    
    auto testPoint = ccpSub(lastDisplayTilePoint, tilePoint);
    if ((int)(testPoint.x) == 0 && (int)(testPoint.y) == 0) {
        return false;
    }
    
    CCPoint displayPoint = tilePoint;
    displayPoint.x = displayPoint.x > 0 ? (displayPoint.x > WorldController::getInstance()->_current_tile_count_x - 1 ? WorldController::getInstance()->_current_tile_count_x - 1 : displayPoint.x) : 0;
    displayPoint.y = displayPoint.y > 0 ? (displayPoint.y > WorldController::getInstance()->_current_tile_count_y - 1 ? WorldController::getInstance()->_current_tile_count_y - 1 : displayPoint.y) : 0;
    
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
    auto disPoint = ccpSub(newViewPoint,WorldController::getInstance()->serverMap_centerViewPoint);
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
    //    int serverId = WorldController::getInstance()->getServerIdByServerPoint(serverTilePoint);
    //elem map左下角0.0 原來的1201view为座標系
    auto childMapPos = WorldController::getInstance()->getServerViewPosByPos(serverTilePoint);
    return getTileMapPointByViewPoint(newViewPoint - childMapPos);//old fun
}

int DynamicTiledMap::getServerIdByViewPoint(const CCPoint &viewPoint){
    auto newViewPoint = viewPoint;
    auto disPoint = ccpSub(newViewPoint,WorldController::getInstance()->serverMap_centerViewPoint);
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
    auto centerPoint = WorldController::getInstance()->getServerPosById(playerInfo.currentServerId);
    CCPoint serverTilePoint = ccp((int)(centerPoint.x) + a,(int)(centerPoint.y) + b);
    int serverId = WorldController::getInstance()->getServerIdByServerPoint(serverTilePoint);
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
    if (tilePoint.x > WorldController::getInstance()->_current_tile_count_x - 1) {
        tempPoint.x = WorldController::getInstance()->_current_tile_count_x - 1;
        needFixServer = true;
    }
    if (tilePoint.y > WorldController::getInstance()->_current_tile_count_y - 1) {
        tempPoint.y = WorldController::getInstance()->_current_tile_count_y - 1;
        needFixServer = true;
    }
    if (needFixServer) {
        int tempCityIndex = WorldController::getIndexByPoint(tempPoint);
        if (WorldController::getInstance()->getCityInfos().find(tempCityIndex) != WorldController::getInstance()->getCityInfos().end() ) {
            serverId = WorldController::getInstance()->getCityInfos()[tempCityIndex].tileServerId;
        }
    }
    //    CCLOG("logwdz+++++++++++++%d",serverId);
    auto disPoint = ccpSub(tilePoint, centerTilePoint);
    CCPoint childMapPoint = ccp(centerViewPoint.x+(disPoint.x-disPoint.y)*_halfTileSize.width,centerViewPoint.y-(disPoint.x+disPoint.y)*_halfTileSize.height);
    CCPoint serverViewPoint =  WorldController::getInstance()->getServerViewPosByPos(WorldController::getServerPosById(serverId));
    return serverViewPoint + childMapPoint;
}
void DynamicTiledMap::updataBoderMap(CCPoint point,int forceServerId){
    if (!WorldMapView::instance()) {
        return;
    }
    if(GlobalData::shared()->playerInfo.uid == ""){
        return;
    }
    unsigned int type = 0;
    if(!GuideController::share()->updateWorldInfo){
        return;
    }
    WorldMapView::instance()->m_updateTimeStamp = time(NULL);
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
    auto cmd = new WorldCommand(point,WorldMapView::instance()->m_updateTimeStamp,type,forceServerId);
    int cmdSid = cmd->getServerId();
    bool isNormalMap =    WorldController::getInstance()->currentMapType == NORMAL_MAP ? true :false;
    if(cmdSid != 0){
        if (isNormalMap && GlobalData::shared()->serverMax < cmdSid) {
            WorldController::getInstance()->setisAsyEnd(true);
            return;
        }
        cmd->sendAndRelease();
        isSendCmd = true;
        WorldMapView::instance()->m_loadingIcon->setVisible(true);
    }
    else{
        WorldController::getInstance()->setisAsyEnd(true);
#if COCOS2D_DEBUG == 2
        cmd->sendAndRelease();
        isSendCmd = true;
        WorldMapView::instance()->m_loadingIcon->setVisible(true);
        WorldController::getInstance()->setisAsyEnd(false);
#endif
    }
    
}
void DynamicTiledMap::updateDynamicMap(CCPoint point) {
    if (!WorldMapView::instance()) {
        return;
    }
    if(GlobalData::shared()->playerInfo.uid == ""){
        return;
    }
    unsigned int type = 1; // 0 : normal 1: single point
    if (point.x < 0 || point.y < 0) {
        point = currentTilePoint;
        type = 0;
        CCPoint serverViewPoint =  WorldController::getInstance()->getServerViewPosByPos(WorldController::getInstance()->getServerPosById(GlobalData::shared()->playerInfo.currentServerId));
        if(WorldController::getInstance()->currentMapType == SERVERFIGHT_MAP){
            serverViewPoint = ccp(0, 0);
        }
        
        updateMap(currentTilePoint, serverViewPoint);
        auto node = this->getChildByTag(WM_BETWEEN_SERVER_MAP_TAG);
        if(node){
            auto map = dynamic_cast<DynamicTiledMap*>(node->getChildByTag(WM_BETWEEN_SERVER_MAP_TAG));
            if(map){
                map->updateMap(currentTilePoint, serverViewPoint);
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
    WorldMapView::instance()->m_updateTimeStamp = time(NULL);
    int limitX = WorldController::getInstance()->_current_tile_count_x;
    int limitY = WorldController::getInstance()->_current_tile_count_y;
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
    auto cmd = new WorldCommand(point,WorldMapView::instance()->m_updateTimeStamp,type);
    int cmdSid = cmd->getServerId();
    bool isNormalMap =    WorldController::getInstance()->currentMapType == NORMAL_MAP ? true :false;
    if(cmdSid != 0){
        if (isNormalMap && GlobalData::shared()->serverMax < cmdSid) {
            WorldController::getInstance()->setisAsyEnd(true);
            return;
        }
        cmd->sendAndRelease();
        isSendCmd = true;
        if (type == 0) {
            WorldMapView::instance()->m_loadingIcon->setVisible(true);
        }
    }
    else{
        WorldController::getInstance()->setisAsyEnd(true);
#if COCOS2D_DEBUG == 2
        cmd->sendAndRelease();
        isSendCmd = true;
        WorldMapView::instance()->m_loadingIcon->setVisible(true);
        WorldController::getInstance()->setisAsyEnd(false);
#endif
    }
}
