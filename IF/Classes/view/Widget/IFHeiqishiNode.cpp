
//
//  IFHeiqishiNode.cpp
//  IF
//
//  Created by wangdianzhen on 15/3/18.
//
//

#include "IFHeiqishiNode.h"
#include "WorldMapView.h"
#include "DynamicResourceController.h"
#define tag_touchSp 110

bool IFHeiqishiNode::init()
{
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup, true);
    animationObj = NULL;
//    this->removeChildByTag(tag_touchSp);
//    m_touchSp = CCLoadSprite::createSprite("world_gold_progress_bg.png");
//    m_touchSp->setTag(tag_touchSp);
//    m_touchSp->setContentSize(this->getSpinSize());
//    m_touchSp->setOpacity(0);
//    this->addChild(m_touchSp);
    return true;
}

IFHeiqishiNode::IFHeiqishiNode(MarchInfo& marchInfo)
:m_skeName("")
,m_aniName("")
,m_entry(nullptr)
,m_state(0)
,m_direction(0)
,m_info(marchInfo)
{
}
int IFHeiqishiNode::getState(){
    return m_state;
}
string IFHeiqishiNode::getSkeName(){
    return m_skeName;
}
 IFHeiqishiNode* IFHeiqishiNode::create(MarchInfo& marchInfo){
     IFHeiqishiNode* pRet = new IFHeiqishiNode(marchInfo);
     if(pRet && pRet->init()){
         pRet->setAnchorPoint(ccp(0.5, 0.5));
         pRet->setContentSize(pRet->getSpinSize());
         pRet->autorelease();
     }
     else{
         CC_SAFE_DELETE(pRet);
     }
      return pRet;
}
int IFHeiqishiNode::getDirection(){
    return m_direction;
}
IFSkeletonAnimation* IFHeiqishiNode::setSpineState(string skeName, int state, int direction, bool repeat/*=true*/,CCCallFunc* callFunc/*=nullptr*/,int loopCnt /*= 1*/)
{
//    CCLOG("testwdz==================================spinName: %s",skeName.c_str());
    m_skeName = skeName;
    m_state = state;
    m_loopCnt = loopCnt;
    m_direction = direction;
    //MonsterDead
    switch (state)
    {
        case MonsterBreath:
            m_aniName = "yd_"+string(CC_ITOA(direction));//direction 0/1/2/3/4  225/180/45/90/270
            break;
        case MonsterAttack:
        {
            m_aniName = "gj_"+string(CC_ITOA(direction));
            break;
        }
        case MonsterDead:
            m_aniName = "sw_"+string(CC_ITOA(direction));
            break;
        default:
            break;
    }
    
    if(!animationObj)
    {
        std::string strJsonFileName = getJsonPath(m_skeName);
        std::string strAtlasFilename = getAtlasPath();
        
        if (!isSkeExit(m_skeName) || !isSkeTextureExit())
        {
            return nullptr;
        }
        
        animationObj = new IFSkeletonAnimation(strJsonFileName.c_str(), strAtlasFilename.c_str());
        addChild(animationObj);
    }
    else
    {
        animationObj->setToSetupPose();
    }
    
    m_entry = animationObj->setAnimation(0,m_aniName.c_str(), repeat);
    animationObj->setTimeScale(m_entry->endTime/1.0f);
    
    if(callFunc)
    {
        CC_SAFE_RELEASE_NULL(m_callBack);
        m_callBack = callFunc;
        CC_SAFE_RETAIN(m_callBack);
        animationObj->setCompleteListener([&] (int trackIndex, int loopCount)
        {
            CC_SAFE_RETAIN(this);
            animationStateEvent(trackIndex,loopCount);
            CC_SAFE_RELEASE(this);
        });
    }
    return animationObj;
}

std::string IFHeiqishiNode::getAtlasPath(){
    std::string localPath = CCFileUtils::sharedFileUtils()->getWritablePath() + DynamicResource_Folder;
    std::string localAtlasPath = localPath + "World_3.atlas";
    if(!CCFileUtils::sharedFileUtils()->isFileExist(localAtlasPath)){
        localAtlasPath = "World/World_3.atlas";
    }
    return localAtlasPath;
}

std::string IFHeiqishiNode::getJsonPath(std::string jsonName){
    std::string localPath = CCFileUtils::sharedFileUtils()->getWritablePath() + DynamicResource_Folder;
    std::string localAtlasPath = localPath + "World_3.atlas";
    std::string jsonPath = localPath + jsonName + ".json";
    if(!CCFileUtils::sharedFileUtils()->isFileExist(localAtlasPath)){
        jsonPath = CCString::createWithFormat("Spine/World/%s.json", jsonName.c_str())->getCString();
    }
    return jsonPath;
}

bool IFHeiqishiNode::isSkeExit(string skeName)
{
    std::string localPath = CCFileUtils::sharedFileUtils()->getWritablePath() + DynamicResource_Folder;
    std::string localAtlasPath = localPath + "World_3.atlas";
    if(CCFileUtils::sharedFileUtils()->isFileExist(localAtlasPath)){
        std::string jsonPath = localPath + skeName + ".json";
        if (!CCFileUtils::sharedFileUtils()->isFileExist(jsonPath))
        {
            return false;
        }
    }else{
        char strJsonFileName[256] = {0};
        char strAtlasFilename[256] = {0};
        sprintf(strJsonFileName, "Spine/World/%s.json", skeName.c_str());
        if (!CCFileUtils::sharedFileUtils()->isFileExist(strJsonFileName))
        {
            return false;
        }
    }
    
    return true;
}

bool IFHeiqishiNode::isSkeTextureExit(){
    std::string localPath = CCFileUtils::sharedFileUtils()->getWritablePath() + DynamicResource_Folder;
    std::string localAtlasPath = localPath + "World_3.atlas";
    if(CCFileUtils::sharedFileUtils()->isFileExist(localAtlasPath)){
        return true;
    }else{
        if (!CCFileUtils::sharedFileUtils()->isFileExist("World/World_3.atlas"))
        {
            return false;
        }
    }
    
    return true;
    
}

float IFHeiqishiNode::getloopOneTime()
{
    if(m_entry)
    {
        return m_entry->endTime;
    }
    return 1.5f;
}

void IFHeiqishiNode::animationStateEvent(int trackIndex, int loopCount)
{
    spTrackEntry* entry = spAnimationState_getCurrent(animationObj->getState(), trackIndex);
    if(m_callBack && entry)
    {
        if(loopCount == m_loopCnt)
        {
            m_callBack->execute();
            CC_SAFE_RELEASE_NULL(m_callBack);
        }
    }
}

void IFHeiqishiNode::onExit(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    setTouchEnabled(false);
    CCNode::onExit();
}

IFHeiqishiNode::~IFHeiqishiNode()
{
    CC_SAFE_RELEASE_NULL(animationObj);
}
 int IFHeiqishiNode::getExchangeDir(int oldDirection){
     int realDirection = 0;
     switch (oldDirection) { //向西 //need to fix by spin res
         case 0:
             realDirection = 1;
             break;
         case 1:
             realDirection = 0;
             break;
         case 2:
             realDirection = 4;
             break;
         case 3:
             realDirection = 0;
             break;
         case 4:
             realDirection = 1;
             break;
         case 5:
             realDirection = 2;
             break;
         case 6:
             realDirection = 3;
             break;
         case 7:
             realDirection = 2;
             break;
             
         default:
             break;
     }
     return realDirection;
}
CCSize IFHeiqishiNode::getSpinSize(){
    return CCSize(80, 80);//no exact size of spin node
}
void IFHeiqishiNode::onStartCityAttack(){
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(IFHeiqishiNode::cityAttack), this, 1.6, 2, 0.4, false);
}
void IFHeiqishiNode::onStopCityAttack(){
    if (m_info.uuid == "") {
        CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
        return;
    }
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    CCSpriteBatchNode* arrowMapNode = WorldMapView::instance()->m_mapArrowSpNode;
    if(arrowMapNode->getChildByTag(m_info.marchTag)){
        arrowMapNode->removeChildByTag(m_info.marchTag);
    }
}

void IFHeiqishiNode::cityAttack(float _time){
    if (m_info.uuid == "") {
        CCLOGFUNC("warn:info data has been deleted!");
        CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
        return;
    }
    int index = 0;
    while(index < 20){
        playAttackAnimation();
        index++;
    }
}

void IFHeiqishiNode::playAttackAnimation(){
    auto addPt = ccp(0, 0);
    if(m_info.targetType == CityTile || m_info.targetType == Trebuchet){
        addPt = ccp(0, _tile_height / 2);
    }
    CCPoint crossPt = WorldController::getInstance()->getCrossPointAtCity(m_info);
    CCPoint m_startPoint = (WorldMapView::instance()->m_map->getViewPointByTilePoint(WorldController::getPointByIndex(MarchArmy::getStartIndex(m_info))) + addPt) - crossPt
    + ccp(20 - rand() % 40, -rand() % 20);
    
    if(m_info.targetType == Throne){
        auto attCross = WorldController::getInstance()->getCrossPointAtCity(m_info, 1.1);
        m_startPoint = attCross - crossPt + ccp(20 - rand() % 40, -rand() % 20);
    }
    
    CCPoint m_endPoint = ccp(100 - rand() % 200, 80 - rand() % 160);
    
    CCSprite* sp1 = NULL;
    
    
    
    CCSpriteBatchNode* arrowMapNode = WorldMapView::instance()->m_mapArrowSpNode;
    if(arrowMapNode->getChildByTag(m_info.marchTag)){//if crash tag 最大 2 ^32
        sp1 = dynamic_cast<CCSprite*>(arrowMapNode->getChildByTag(m_info.marchTag));
    }
    else{
        sp1 =CCLoadSprite::createSprite("a010_0_N_attack_0.png");
        sp1->setTextureRect(CCRectZero);
        sp1->setPosition(this->getPosition());
        sp1->setTag(m_info.marchTag);
        arrowMapNode->addChild(sp1);
    }
    auto arrow = Arrow::create(sp1);
    arrow->attack(m_startPoint, m_endPoint, 2.1);
}
 void IFHeiqishiNode::setScaleX(float fScaleX){
     CCNode::setScaleX(fScaleX);
}
 void IFHeiqishiNode::setScale(float scale){
     if (this->getScaleX() < 0) {
         CCNode::setScaleX(-scale);
         CCNode::setScaleY(scale);
     }
     else{
         CCNode::setScale(scale);
     }
     
}

bool IFHeiqishiNode::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if( isTouchInside(this, pTouch)){
        return true;
    }
    return false;
    
}

void IFHeiqishiNode::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void IFHeiqishiNode::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
        WorldMapView::instance()->showTroopInfo(m_info.uuid);
}