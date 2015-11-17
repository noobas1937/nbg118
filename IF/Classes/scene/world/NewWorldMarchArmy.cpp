//
//  NewWorldMarchArmy.cpp
//  IF
//
//  Created by 李锐奇 on 14-7-14.
//
//

#include "NewWorldMarchArmy.h"
#include "CCMathUtils.h"
#include "WorldMapView.h"
#include "SceneController.h"
#include "ParticleController.h"
#include "DynamicTiledMap.h"

class RockAni *RockAni::create(CCNode *parent){
    RockAni *ret = new RockAni();
    if(ret && ret->init(parent)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void RockAni::attack(CCPoint &startPt, CCPoint &endPt, float totalTime, unsigned int index, int rockIdx, bool useDelayTime){
    m_startPt = startPt;
    m_endPt = endPt;
    m_attIndex = index;
    m_rockIndex = rockIdx;
    float angle = CCMathUtils::getAngle(m_startPt, m_endPt);
    
    m_rock->setPosition(m_startPt);
    CCPoint gap = ccpSub(m_startPt, m_endPt);
    int len = MAX(abs((int)gap.x),abs((int)gap.y));
    float time = 1.0*len/250;
    if(time<0.2){
        time = 0.2;
    }
    if(time > 1.2){
        time = 1.2;
    }
    CCSequence* s1;
    float scale = 0.5;
    if(m_rockIndex == 4){
        scale = 1.2f;
    }
    ccBezierConfig  beziercofig;
    float cy = 160 * cos(angle);
    cy = fabsf(cy);
    float cx = 30 * sin(angle);
    beziercofig.controlPoint_1 = m_startPt + ccp(-cx, cy);
    beziercofig.controlPoint_2 = m_endPt + ccp(cx, cy);
    
    s1 = CCSequence::create(CCScaleTo::create(time*0.25, scale),CCScaleTo::create(time*0.75, scale,0.5*scale),NULL);
    
    m_rock->setScale(0.8*scale);
    beziercofig.endPosition = m_endPt;
    m_rock->setOpacity(0);
    CCActionInterval *forward = CCBezierTo::create(time, beziercofig);
    CCSequence* s3 = CCSequence::create(forward, NULL);
    CCSequence* s4 = CCSequence::create(CCFadeIn::create(time/18.0),CCDelayTime::create(15*time/18.0),CCFadeOut::create(2*time/18.0),NULL);
    CCSpawn* spawn = CCSpawn::create(s1,s3,s4,NULL);
    
    float delayTime = totalTime - time;
    if(delayTime < 0){
        delayTime = 0;
    }
    CCSequence *s5 = NULL;
    
    if(useDelayTime){
        s5 = CCSequence::create(
                                CCDelayTime::create(delayTime),
                                spawn,
                                CCCallFuncO::create(this, callfuncO_selector(RockAni::addParitcle), NULL),
                                CCRemoveSelf::create(),
                                NULL
                                );
    }else{
        s5 = CCSequence::create(
                                spawn,
                                CCCallFuncO::create(this, callfuncO_selector(RockAni::addParitcle), NULL),
                                CCRemoveSelf::create(),
                                NULL
                                );
    }
    
    m_rock->runAction(s5);
}

void RockAni::attack2(CCPoint &startPt, CCPoint &endPt, unsigned int index, int rockIdx,int direction){
    m_startPt = startPt;
    m_endPt = endPt;
    m_attIndex = index;
    m_rockIndex = rockIdx;
    
    if(m_rockIndex == 4){
        CCNode* parent = m_rock->getParent();
        m_rock->removeFromParent();
        m_rock = CCLoadSprite::createSprite("WarStone.png");
        parent->addChild(m_rock);
    }
    
    m_rock->setPosition(m_startPt);
    int len = ccpDistance(m_startPt, m_endPt);
    float time = len/350.f;
    if(time<0.2f){
        time = 0.2f;
    }
    if(time > .8f){
        time = .8f;
    }

    float delayTime = 1.5f;
    m_rock->setVisible(false);
    CCSequence *s5 = NULL;
    
    if(direction == 2 || direction == 4)
    {
        ccBezierConfig  beziercofig;
        beziercofig.controlPoint_1 = m_startPt + ccp(len/4, len/4);
        beziercofig.controlPoint_2 = m_endPt + ccp(-len/4, len/4);
        beziercofig.endPosition = m_endPt;
        CCActionInterval *forward = CCBezierTo::create(time, beziercofig);
        
        s5 = CCSequence::create(
                                CCDelayTime::create(delayTime),
                                CCCallFunc::create(this, callfunc_selector(RockAni::setSockVisible)),
                                forward,
                                CCCallFuncO::create(this, callfuncO_selector(RockAni::addParitcle), NULL),
                                CCRemoveSelf::create(),
                                NULL
                                );
        
        
    }
    else
    {
        CCMoveTo* moveTo = CCMoveTo::create(time, m_endPt);
        s5 = CCSequence::create(
                                CCDelayTime::create(delayTime),
                                CCCallFunc::create(this, callfunc_selector(RockAni::setSockVisible)),
                                moveTo,
                                CCCallFuncO::create(this, callfuncO_selector(RockAni::addParitcle), NULL),
                                CCRemoveSelf::create(),
                                NULL
                                );
    }
    m_rock->runAction(s5);
}

void RockAni::setSockVisible()
{
    m_rock->setVisible(true);
}
bool RockAni::init(CCNode *parent){
    std::string rockStr = "world_stone_che.png";
//    if(m_rockIndex == 4){
//        rockStr = "WarStone.png";
//    }
    m_rock = CCLoadSprite::createSprite(rockStr.c_str());
    m_rock->setScale(0.3f);
//    m_rock->setColor(Color3B::RED);
    parent->addChild(m_rock);
    return true;
}

RockAni::~RockAni(){
    
}

void RockAni::addParitcle(CCObject* obj){
    BatchTagType type = Rock;
    if(m_rockIndex == 2){
        type = Rock1;
    }else if(m_rockIndex == 3){
        type = Rock2;
    }else if(m_rockIndex == 4){
        type = Rock3;
    }
    WorldMapView::instance()->delBatchItem(type, m_attIndex);
    WorldMapView::instance()->addBatchItem(type, m_attIndex);
}

//----

Arrow *Arrow::create(CCNode *parent){
    Arrow *ret = new Arrow();
    if(ret && ret->init(parent)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void Arrow::attack(CCPoint &startPt, CCPoint &endPt, float totalTime){
    m_startPt = startPt;
    m_endPt = endPt;
    m_currentPt = m_startPt;
    float angle = CCMathUtils::getAngle(m_startPt, m_endPt);
    
    m_arrow->setPosition(m_startPt);
    CCPoint gap = ccpSub(m_startPt, m_endPt);
    int len = MAX(abs((int)gap.x),abs((int)gap.y));
    float time = 1.0*len/180;
    if(time<0.2){
        time = 0.2;
    }
    CCSequence* s1;
    float scale = 0.5;
    ccBezierConfig  beziercofig;
    float cy = 80 * cos(angle);
    cy = fabsf(cy);
    float cx = 20 * sin(angle);
    beziercofig.controlPoint_1 = m_startPt + ccp(-cx, cy);
    beziercofig.controlPoint_2 = m_endPt + ccp(cx, cy);
    
    s1 = CCSequence::create(CCScaleTo::create(time*0.25, scale),CCScaleTo::create(time*0.75, scale,0.5*scale),NULL);
    
    m_arrow->setScale(0.8*scale);
    beziercofig.endPosition = m_endPt;
    m_arrow->setOpacity(0);
    CCActionInterval *forward = CCBezierTo::create(time, beziercofig);
    CCSequence* s3 = CCSequence::create(forward, NULL);
    CCSequence* s4 = CCSequence::create(CCFadeIn::create(time/18.0),CCDelayTime::create(15*time/18.0),CCFadeOut::create(2*time/18.0),NULL);
    CCSpawn* spawn = CCSpawn::create(s1,s3,s4,NULL);
    
    float delayTime = totalTime - time;
    if(delayTime < 0){
        delayTime = 0;
    }
    delayTime = rand() % 20 / 100.0;
    CCSequence *s5 = CCSequence::create(
                                        CCDelayTime::create(delayTime),
                                        spawn,
                                        CCRemoveSelf::create(),
                                        NULL
                                        );
    m_arrow->runAction(s5);
}

void Arrow::update(float time){
    float angle = CCMathUtils::getAngle(m_currentPt, m_arrow->getPosition());
    m_currentPt = m_arrow->getPosition();
    m_arrow->setRotation(90 - angle);
}

bool Arrow::init(CCNode *parent){
    m_arrow = CCLoadSprite::createSprite("world_jian.png");
    parent->addChild(m_arrow);
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
    return true;
}

Arrow::~Arrow(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
}

//-----

Soldier *Soldier::create(CCSprite *parent, int type, float direction, CCPoint pt, MarchInfo &info, int row, int col){
    Soldier *ret = new Soldier(parent, type, direction, pt, info, row, col);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void Soldier::setDirection(float direction){
    m_direction = direction;
    resetAnimate(m_direction, m_currentState);
}

void Soldier::walk(){
    this->m_currentState = 0;
    resetAnimate(m_direction, m_currentState);
}

void Soldier::attack(){
    m_sprite->setOpacity(255);
    m_pos = m_sprite->getPosition();
    this->m_currentState = 1;
    resetAnimate(m_direction, m_currentState);
    if(m_type == GONG){
        playAttackAnimation(0);
        playAttackAnimation(0);
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(Soldier::playAttackAnimation), this, 1.3, 5, 0.0, false);
    }else if(m_type == CHE || m_type == TITAN){
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(Soldier::playRockAttackAnimation), this, 2.6, 3, 0.4, false);
    }
}

bool Soldier::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(m_sprite && isTouchInside(m_sprite, pTouch)){
        return true;
    }
    return false;
}

void Soldier::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void Soldier::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(m_sprite && isTouchInside(m_sprite, pTouch)){
        WorldMapView::instance()->showTroopInfo(m_info.uuid);
    }
}

void Soldier::dead(float _time){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    removeAllParticle();
    this->m_currentState = 2;
    resetAnimate(m_direction, m_currentState);
}

void Soldier::soldierDelete(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    m_sprite->stopAllActions();
    Director::getInstance()->getEventDispatcher()->removeEventListener(_touchListener);
 //   setTouchEnabled(false);
    removeAllParticle();
}

void Soldier::playDeadAni(float delayTime){
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(Soldier::dead), this, delayTime, 1, 0.0, false);
}

void Soldier::playAttackAnimation(float _time){
    CCPoint m_startPoint = m_sprite->getPosition(); //
    auto addPt = ccp(0, 0);
    if(m_info.targetType == CityTile || m_info.targetType == Trebuchet){
        addPt = ccp(0, _tile_height / 2);
    }
    CCPoint endPt = WorldMapView::instance()->m_map->getViewPointByTilePoint(WorldController::getPointByIndex(MarchArmy::getStartIndex(m_info)),m_info.serverId) + addPt;
    CCPoint crossPt = WorldController::getInstance()->getCrossPointAtCity(m_info);
    CCPoint m_endPoint = endPt - crossPt + ccp(40 - rand() % 80, 90 - rand() % 110);
    if(m_info.targetType == Throne){
        auto attCross = WorldController::getInstance()->getCrossPointAtCity(m_info, 1.1);
        m_endPoint = attCross - crossPt + ccp(40 - rand() % 80, 90 - rand() % 110);
    }
    auto arrow = Arrow::create(m_parent);
    arrow->attack(m_startPoint, m_endPoint, 2.1);
    
    CCPoint m_endPoint1 = endPt - crossPt + ccp(40 - rand() % 80, 60 - rand() % 80);
    if(m_info.targetType == Throne){
        auto attCross = WorldController::getInstance()->getCrossPointAtCity(m_info, 1.1);
        m_endPoint1 = attCross - crossPt + ccp(40 - rand() % 80, 60 - rand() % 80);
    }

    auto arrow1 = Arrow::create(m_parent);
    arrow1->attack(m_startPoint, m_endPoint1, 2.1);
    
}

void Soldier::playRockAttackAnimation(float _time){
    CCPoint m_startPoint = m_sprite->getPosition(); //
    auto addPt = ccp(0, 0);
    if(m_info.targetType == CityTile || m_info.targetType == Trebuchet){
        addPt = ccp(0, _tile_height / 2);
    }
    CCPoint endPt = WorldMapView::instance()->m_map->getViewPointByTilePoint(WorldController::getPointByIndex(MarchArmy::getStartIndex(m_info)),m_info.serverId) + addPt;
    CCPoint crossPt = WorldController::getInstance()->getCrossPointAtCity(m_info);
    CCPoint m_endPoint = endPt - crossPt + ccp(40 - rand() % 80, 50 - rand() % 70);
    if(m_info.targetType == Throne){
        auto attCross = WorldController::getInstance()->getCrossPointAtCity(m_info, 1.1);
        m_endPoint = attCross - crossPt + ccp(40 - rand() % 80, 50 - rand() % 70);
    }

    auto rock = RockAni::create(m_parent);
    rock->attack(m_startPoint, m_endPoint, 3.1, MarchArmy::getStartIndex(m_info), m_col + 1);
}

//void Soldier::playMagicAttackAnimation(float _time){
//    
//}
// tao.yu 下面的函数用来获取小兵的动画
CCAnimate *Soldier::getAnimate(float direction, int state){
    std::string stateStr = "";
    
    Vector<SpriteFrame*> arr; //= CCArray::create();
    int totalFrame = 8;
    
    if(state == eSoldierState::kDead){
        stateStr = "move";
    }else if(state == eSoldierState::kAttack){
        stateStr = "move";
    }else if(state == eSoldierState::kWalk){
        stateStr = "move";
    }else if(state == eSoldierState::kHurt){
        stateStr = "move";
    }else if(state == eSoldierState::kStand){
        stateStr = "move";
    }
    std::string str = "vikings_0_%s_%s_%d.png";
    
//    if(state == eSoldierState::kDead){
//        stateStr = "die";
//    }else if(state == eSoldierState::kAttack){
//        stateStr = "attack";
//    }else if(state == eSoldierState::kWalk){
//        stateStr = "move";
//    }else if(state == eSoldierState::kHurt){
//        stateStr = "hurt";
//    }else if(state == eSoldierState::kStand){
//        stateStr = "stand";
//    }
    if(m_type == TITAN){
        // tao.yu 这是一个很屎的代码，龙的等级小于等于3级用动画1，大于3用动画2
        str = "dragon_1_%s_%s_%d.png";
        if (GlobalData::shared()->titanInfo.level > 3) {
            str = "dragon_2_%s_%s_%d.png";
        }
        totalFrame = 9;
//        if(state == eSoldierState::kDead){
//            totalFrame = 6;
//        }else if(state == eSoldierState::kAttack){
//            totalFrame = 11;
//        }else if(state == eSoldierState::kWalk){
//            totalFrame = 7;
//        }else if(state == eSoldierState::kHurt){
//            totalFrame = 1;
//        }else if(state == eSoldierState::kStand){
//            totalFrame = 10;
//        }
    }
//    if(m_type == BU){
//        str = "a010_0_%s_%s_%d.png";
//        if(state == eSoldierState::kDead){
//            totalFrame = 6;
//        }else if(state == eSoldierState::kAttack){
//            totalFrame = 11;
//        }else if(state == eSoldierState::kWalk){
//            totalFrame = 7;
//        }else if(state == eSoldierState::kHurt){
//            totalFrame = 1;
//        }else if(state == eSoldierState::kStand){
//            totalFrame = 10;
//        }
//    }
//    else if(m_type == QI){
//        str = "a020_0_%s_%s_%d.png";
//        if(state == eSoldierState::kDead){
//            totalFrame = 5;
//        }else if(state == eSoldierState::kAttack){
//            totalFrame = 10;
//        }else if(state == eSoldierState::kWalk){
//            totalFrame = 8;
//        }else if(state == eSoldierState::kHurt){
//            totalFrame = 1;
//        }else if(state == eSoldierState::kStand){
//            totalFrame = 8;
//        }
//    }
//    else if (m_type == GONG)
//    {
//        str = "a060_0_%s_%s_%d.png";
//        if(state == eSoldierState::kDead){
//            totalFrame = 4;
//        }else if(state == eSoldierState::kAttack){
//            totalFrame = 10;
//        }else if(state == eSoldierState::kWalk){
//            totalFrame = 6;
//        }else if(state == eSoldierState::kHurt){
//            totalFrame = 1;
//        }else if(state == eSoldierState::kStand){
//            totalFrame = 7;
//        }
//    }
//    else if (m_type == CHE)
//    {
//        str = "zhanche_0_%s_%s_%d.png";
//        if(state == eSoldierState::kDead){
//            totalFrame = 1;
//        }else if(state == eSoldierState::kAttack){
//            totalFrame = 6;
//        }else if(state == eSoldierState::kWalk){
//            totalFrame = 4;
//        }else if(state == eSoldierState::kHurt){
//            totalFrame = 0;
//        }else if(state == eSoldierState::kStand){
//            totalFrame = 0;
//        }
//    }
//    else if (m_type == TITAN)
//    { //fusheng 泰坦
//        str = "a010_0_%s_%s_%d.png";
//    }
    
    std::string directionStr = "";
    bool isFlipX = false;
    
    direction += 360;
    direction = fmodf(direction, 360);
    
    float angle = 360 / 16;
    
    if((direction >= 360 - angle && direction <= 360) || (direction >= 0 && direction <= angle)){
        directionStr = "W";
    }else if(direction > angle && direction <= 90 - angle){
        directionStr = "SW";
    }else if(direction > 90 - angle && direction <= 90 + angle){
        directionStr = "S";
    }else if(direction > 90 + angle && direction <= 180 - angle){
        directionStr = "SW";
        isFlipX = true;
    }else if(direction > 180 - angle && direction <= 180 + angle){
        directionStr = "W";
        isFlipX = true;
    }else if(direction > 180 + angle && direction <= 270 - angle){
        directionStr = "NW";
        isFlipX = true;
    }else if(direction > 270 - angle && direction <= 270 + angle){
        directionStr = "N";
    }else if(direction > 270 + angle && direction <= 360 - angle){
        directionStr = "NW";
    }
    int spriteW = 0;
    int index = 0;
    while(index < totalFrame){
        int addTime = 1;
        if(index == totalFrame - 1 && state ==1){
            if(m_type == CHE){
                addTime = 15;
            }else if(m_type == GONG){
                addTime = 1;
            }
        }
        if(index == 0){
            auto sprite = CCLoadSprite::createSprite(CCString::createWithFormat(str.c_str(), directionStr.c_str(), stateStr.c_str(), index)->getCString());
            spriteW = sprite->getContentSize().width;
        }
        int i = 0;
        while(i < addTime){
            arr.pushBack(CCLoadSprite::loadResource(CCString::createWithFormat(str.c_str(), directionStr.c_str(), stateStr.c_str(), index)->getCString()));
            i++;
        }
        index++;
    }
    m_sprite->setFlipX(isFlipX);
    CCAnimation *animation = CCAnimation::createWithSpriteFrames(arr, 0.1f);
    if(m_currentState == 2){
        animation->setLoops(1);
    }else{
        animation->setLoops(std::numeric_limits<int>::max());
    }
    CCAnimate *animate = CCAnimate::create(animation);
    if(m_kingSprite1){
        float s = 0.6;//0.45;
        auto w = spriteW * m_sprite->getScale() * s;
        m_kingSprite1->setScale(w / m_kingSprite1->getContentSize().width);
        m_kingSprite2->setScale(m_kingSprite1->getScale());
    }
    return animate;
}

void Soldier::resetAnimate(float direction, int state){
    m_sprite->stopAllActions();
    if(m_sprite->isVisible()){
        m_sprite->setOpacity(255);
    }
    auto animate = getAnimate(direction, state);
    animate->setTag(1);
    if(m_currentState != 2){
        m_sprite->runAction(animate);
    }else{
        m_sprite->runAction(CCSequence::create(animate
                                               , CCCallFuncO::create(this, callfuncO_selector(Soldier::playDeadAniBack), NULL)
                                               , NULL
                                               ));
    }

    m_sprite->setPosition(m_pos);
    setKingSpritePos();
}

void Soldier::playDeadAniBack(CCObject* obj){
    m_sprite->runAction(CCSequence::create(CCFadeTo::create(2.2, 0),
                                           CCRemoveSelf::create(),
                                           NULL
                                           ));
    if(m_kingSprite1){
        m_kingSprite1->removeFromParent();
        m_kingSprite2->removeFromParent();
    }
}

bool Soldier::init(){
    if(m_info.officer == KINGDOM_KING_ID){
        m_kingSprite1 = CCLoadSprite::createSprite("KingsArmy_1.png");
        m_parent->addChild(m_kingSprite1, 0);
        m_kingSprite1->setVisible(false);
    }
   
    m_sprite = CCLoadSprite::createSprite("a060_0_S_move_0.png");
    m_sprite->setScale(1);
    m_parent->addChild(m_sprite, 1);
    m_sprite->setVisible(false);
    
    if(m_info.officer == KINGDOM_KING_ID){
        m_kingSprite2 = CCLoadSprite::createSprite("KingsArmy_2.png");
        m_kingSprite2->setAnchorPoint(ccp(0.5, 0));
        m_parent->addChild(m_kingSprite2, 2);
        m_kingSprite2->setVisible(false);
    }

    walk();

    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(Soldier::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(Soldier::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(Soldier::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(Soldier::onTouchCancelled, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, m_sprite);
    _touchListener = listener;
    //TODO::Touch
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup, true);
    return true;
}

Soldier::~Soldier(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
}

void Soldier::addWarParticleToBatch(cocos2d::CCParticleSystemQuad *particle){
    WorldMapView::instance()->addWalkParticleToBatch(particle, getParticleTag());
    //    m_parVec.push_back(particle);
}

int Soldier::getParticleTag(){
    return  m_info.marchTag * 1000 + m_type * 100 +  m_row * 10 + m_col;
}

void Soldier::addParitcle(){
//    return;
    if(!m_sprite || !m_sprite->getParent()){
        return;
    }

    CCPoint p = m_sprite->getPosition();
    CCPoint p2 = m_sprite->getParent()->convertToWorldSpace(p);
    auto size = CCDirector::sharedDirector()->getWinSize();
    if(p2.x < 0 || p2.x > size.width || p2.y < 0 || p2.y > size.height){
        removeAllParticle();
        m_currentParticleType = "";
        return;
    }
    std::string landType = getLandType();
    auto pt = ccp(10 * cos(m_direction / 180.0f * PI), 10 * sin(m_direction / 180.0f * PI));

    if(m_currentParticleType == landType){
        auto vec = WorldMapView::instance()->m_walkparticleVec.find(getParticleTag());
        if (vec != WorldMapView::instance()->m_walkparticleVec.end()) {
            for (auto &particle:vec->second) {
                particle->setPosition(m_sprite->getPosition() + m_sprite->getParent()->getPosition() + pt);
            }
        }
        return;
    }else{
        removeAllParticle();
        m_currentParticleType = landType;
        string tmpUIStart = m_currentParticleType + "_";
        int uiCount = 2;
        if(landType == "ArmySpray"){
            uiCount = 3;
        }
        //原来的粒子特效
//        for (int i=1; i<=uiCount; i++) {
//            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpUIStart.c_str(),i)->getCString());
//            particle->setPosition(m_sprite->getPosition() + m_sprite->getParent()->getPosition() + pt);
//            particle->setPositionType(kCCPositionTypeRelative);
//            addWarParticleToBatch(particle);
//        }
        //行军技能减速粒子特效
        if(m_info.slowDown){
            auto vec = WorldMapView::instance()->m_walkparticleVec.find(getParticleTag());
            if (vec != WorldMapView::instance()->m_walkparticleVec.end()) {
                for (auto &particle:vec->second) {
                    particle->setPosition(m_sprite->getPosition() + m_sprite->getParent()->getPosition() + pt);
                }
            }else{
                for (int i=1; i<=2; i++) {
                    auto particle = ParticleController::createParticle(CCString::createWithFormat("ArmyMud_%d",i)->getCString());//ArmyMud_ //Loading_
                    particle->setPosition(m_sprite->getPosition() + m_sprite->getParent()->getPosition() + pt);
                    particle->setPositionType(kCCPositionTypeRelative);
                    addWarParticleToBatch(particle);
                }
            }
        }
        //行军技能减速粒子特效
        addParitcle();
    }
}

std::string Soldier::getLandType(){
    if(!m_sprite || !m_sprite->getParent()){
        return "ArmySmoke";
    }
    auto pt = m_sprite->getPosition() + m_sprite->getParent()->getPosition();
    auto mapPt = WorldMapView::instance()->m_map->getTilePointByViewPoint(pt);
    unsigned int index = WorldController::getInstance()->getIndexByPoint(mapPt);
    auto city = WorldController::getInstance()->getCityInfos().find(index);
    if(city != WorldController::getInstance()->getCityInfos().end()){
        if(city->second.cityName == "108619"){
            return "ArmySpray";
        }
    }
    return "ArmySmoke";
}

void Soldier::removeAllParticle(){
    //    m_parVec.clear();
    WorldMapView::instance()->removeWalkParticle(getParticleTag());
}

void Soldier::update(float time){
    if(m_sprite->getParent() == NULL){
        return;
    }
    if(SceneController::getInstance()->currentSceneId != SCENE_ID_WORLD || !WorldMapView::instance() ||!WorldMapView::instance()->m_map){
        return;
    }
    
    if(m_sprite->isVisible()){
        setKingSpritePos();
        if(m_currentState == 2){
            CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
        }else{
            if(m_sprite->getOpacity() > 50){
                if(m_currentState == 0){
                    addParitcle();
                }else{
                    removeAllParticle();
                }
            }
        }
        return;
    }
    auto addStartPt = ccp(0, 0);
    auto addEndPt = ccp(0, 0);
    if(m_info.stateType == StateReturn){
        addEndPt = ccp(0, _tile_height / 2);
    }else{
        addStartPt = ccp(0, _tile_height / 2);
    }
    
    if(WorldController::getInstance()->isFourGrid(m_info.targetType)){
        if(m_info.stateType == StateMarch){
            addEndPt = ccp(0, _tile_height / 2);
        }else{
            addStartPt = ccp(0, _tile_height / 2);
        }
    }
    
    auto startPt = WorldMapView::instance()->m_map->getViewPointByTilePoint(WorldController::getPointByIndex(MarchArmy::getStartIndex(m_info)),m_info.serverId) + addStartPt;
    auto endPt = WorldMapView::instance()->m_map->getViewPointByTilePoint(WorldController::getPointByIndex(MarchArmy::getEndIndex(m_info)),m_info.serverId) + addEndPt;
    auto currentPt = m_sprite->getParent()->getPosition() + this->m_sprite->getPosition();
    
    float dis1 = ccpDistance(startPt, endPt);
    float dis2 = ccpDistance(currentPt, endPt) - ccpDistance(ccp(0, 0), ccp(35, 35));
    
    if(dis1 > dis2){
        m_sprite->setVisible(true);
        if(m_kingSprite1){
            m_kingSprite1->setVisible(true);
            m_kingSprite2->setVisible(true);
        }
        if(dis1 < dis2 + 20){
            m_sprite->setOpacity(0);
            m_sprite->runAction(CCFadeIn::create(1.2f));
        }else{
            m_sprite->setOpacity(255);
        }
        update(0.0f);
    }
}

void Soldier::setZorder(int zorder){
    m_sprite->setZOrder(zorder + 1000);
    if(m_kingSprite1){
        m_kingSprite1->setZOrder(zorder);
        m_kingSprite2->setZOrder(zorder + 2000);
    }
}

void Soldier::setPos(CCPoint &pt){
    m_pos = pt;
    m_sprite->setPosition(pt);
    resetAnimate(m_direction, m_currentState);
}

void Soldier::spreadTo(CCObject *arr){
    if(!arr){
        return;
    }
    auto _array = dynamic_cast<CCArray*>(arr);
    if(m_currentState == 2 && _array){
        _array->removeAllObjects();
        _array->release();
        _array = NULL;
        return;
    }
    if(_array->count() == 0){
        if(_array){
            _array->release();
            _array = NULL;
        }
        attack();
    }else{
        auto dict = dynamic_cast<CCDictionary*>(_array->objectAtIndex(0));
        int x = dict->valueForKey("x")->intValue();
        int y = dict->valueForKey("y")->intValue();
        _array->removeObjectAtIndex(0);
        CCPoint pt = ccp(x, y);
        
        int direction = CCMathUtils::getAngle(pt, m_sprite->getPosition());
        resetAnimate(direction, m_currentState);
        
        float speed = 60.0f;
        
        float moveTime = abs(int(ccpDistance(m_sprite->getPosition(), pt))) / speed;
        
        float maxSpreadTime = 6;
        if(moveTime > maxSpreadTime){
            moveTime = maxSpreadTime;
        }
        float delayTime = 0;
        if(_array->count() == 0 && m_type == GONG){
            delayTime = 0.7 - moveTime;
            if(delayTime < 0){
                delayTime = 0;
            }
        }
        if(moveTime <= 0){
            moveTime = 0.1;
        }
        m_sprite->runAction(CCSequence::create(
                                               CCMoveTo::create(moveTime, pt)
                                               , CCDelayTime::create(delayTime)
                                               , CCCallFuncO::create(this, callfuncO_selector(Soldier::spreadTo), _array)
                                               , NULL
                                               ));
        int frame = 10;
        m_sprite->setPosition(m_sprite->getPosition() + ccp((pt.x - m_sprite->getPositionX()) / (moveTime * frame), (pt.y - m_sprite->getPositionY()) / (moveTime * frame)));
        m_pos = ccp(x, y);
        setKingSpritePos();
    }
}

void Soldier::setKingSpritePos(){
    if(m_kingSprite1){
        m_kingSprite1->setPosition(m_sprite->getPosition() - ccp(0, 5));
        m_kingSprite2->setPosition(m_sprite->getPosition() - ccp(0, 15));
    }
}

CCPoint Soldier::getCurrentPos(){
    return m_sprite->getPosition();
}
//-----

Phalanx *Phalanx::create(int type, float direction, int num, CCSprite* node, CCPoint pt, MarchInfo &info){
    Phalanx *ret = new Phalanx(type, direction, num, node, pt, info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void Phalanx::walk(){
    std::map<int, Soldier*>::iterator it;
    for (it = m_soldiers.begin(); it != m_soldiers.end(); it++) {
        it->second->walk();
    }
}

void Phalanx::attack(){
    std::map<int, Soldier*>::iterator it;
    for (it = m_soldiers.begin(); it != m_soldiers.end(); it++) {
        it->second->attack();
    }
}

void Phalanx::spreadTo(CCArray *data){
    auto addPt = [](CCPoint &pt, CCArray *arr){
        CCDictionary *dict1 = CCDictionary::create();
        dict1->setObject(CCString::create(CC_ITOA(pt.x)), "x");
        dict1->setObject(CCString::create("move"), "status");
        dict1->setObject(CCString::create(CC_ITOA(pt.y)), "y");
        arr->addObject(dict1);
    };
    
    
    auto generatePoint = [](CCPoint &startPt, CCPoint &endPt, std::vector<CCPoint> &vector){
        CCPoint midPt1 = ccp(endPt.x, startPt.y);
        CCPoint midPt2 = ccp(endPt.x, startPt.y);
        bool flag = false;
        if(ccpDistance(startPt, midPt1) > ccpDistance(endPt, midPt1)){
            midPt1.y = endPt.y;
            midPt2.y = startPt.y;
            int sign = 1;
            if(startPt.x > endPt.x){
                sign = -1;
            }
            midPt1.x = startPt.x + abs(int(endPt.y - startPt.y)) * sign;
            midPt2.x = endPt.x - abs(int(endPt.y - startPt.y)) * sign;
        }else{
            midPt1.x = endPt.x;
            midPt2.x = startPt.x;
            
            int sign = 1;
            if(startPt.y > endPt.y){
                sign = -1;
            }
            flag = true;
            midPt1.y = startPt.y + abs(int(endPt.x - startPt.x)) * sign;
            midPt2.y = endPt.y - abs(int(endPt.x - startPt.x)) * sign;
        }
        
        if(rand() % 2 == 0 || flag){
            vector.push_back(midPt2);
        }else{
            vector.push_back(midPt1);
        }
        
        vector.push_back(ccp(endPt.x, endPt.y));
    };
    
    
    std::map<int, Soldier*>::iterator it;
    auto soldiersIndex = CCArray::create();
    int index = 0;
    while(index < m_soldiers.size()){
        soldiersIndex->addObject(CCInteger::create(index++));
    }
    for (it = m_soldiers.begin(); it != m_soldiers.end(); it++) {
        int row = it->first / m_col;
        int col = it->first % m_col;
        
        
        int dataIndex = rand() % data->count();
        CCDictionary *dict = dynamic_cast<CCDictionary*>(data->objectAtIndex(dataIndex));
        int x = dict->valueForKey("x")->intValue();
        int y = dict->valueForKey("y")->intValue();
        float direction = dict->valueForKey("direction")->floatValue();
        CCPoint pt = ccp(x, y);
        
        int index = rand() % soldiersIndex->count();
        int pos = dynamic_cast<CCInteger*>(soldiersIndex->objectAtIndex(index))->getValue();
        soldiersIndex->removeObjectAtIndex(index);
        auto pt0 = getSoldierPos(0, 0, m_direction, startPt);
        auto pt1 = getSoldierPos(0, 0, direction, startPt);
        
        
        CCPoint pt2 = ccp(-1, -1);
        if(m_type == CHE || m_type == GONG){
            pt2 = getSoldierPosByRowAndCol(row, col, direction, m_row, m_col, startPt, 80, 80);
        }else{
            int w = 80;
            int h = 80;
            if(WorldController::getInstance()->isFourGrid(m_info.targetType)){
                w = 150;
                h = 150;
            }
            pt2 = getSoldierPosByRowAndCol(0, pos, direction, 1, m_col * m_row, startPt, w, h);
        }
        
        
        auto finalPt = pt0 + (pt2 - pt1);
        
        CCPoint spreadToPt = pt - getSoldierPos(0, 0, m_direction, startPt) + finalPt;
        
        it->second->setDirection(direction);
        
        CCArray *arr = CCArray::create();
        arr->retain();
        
        if(m_type == CHE){
            pt2 = getSoldierPosByRowAndCol(row, col, direction, m_row, m_col, startPt, 130, 130);
        }else if(m_type == GONG){
            int numPerLine = (m_col * m_row) / 2;
            if (numPerLine == 0) numPerLine = 1;
            row = it->first / numPerLine;
            col = it->first % numPerLine;
            
            pt2 = getSoldierPosByRowAndCol(row, col, direction, 2, numPerLine, startPt, 80, 120);
        }
        auto finalPt1 = pt0 + (pt2 - pt1);
        CCPoint spreadToPt1 = pt - getSoldierPos(0, 0, m_direction, startPt) + finalPt1;
        
        if(m_type == CHE || m_type == GONG){
            addPt(spreadToPt, arr);
            addPt(spreadToPt1, arr);
        }else{
            CCPoint startPt = it->second->getCurrentPos();
            CCPoint midPt = (startPt + spreadToPt) / 2;
            std::vector<CCPoint> vector;
            vector.clear();
            
            generatePoint(startPt, midPt, vector);
            generatePoint(midPt, spreadToPt, vector);
            
            int i = 0;
            while(i < vector.size()){
                addPt(vector[i], arr);
                i++;
            }
            vector.clear();
        }
        it->second->spreadTo(arr);
    }
}

void Phalanx::setDirection(){
    
}

CCPoint Phalanx::getSoldierPosByRowAndCol(int row, int col, float direction, int rowCount, int colCount,CCPoint startPt, int totalW, int totalH){
    //    float gapX = totalW / rowCount;
    float gapY = totalH / colCount;
    
    float x = (row) * gapX;
    float startY = -(colCount - 1) * gapY / 2;
    float y = startY + (col) * gapY;
    return getSoldierPos(x, y, direction, startPt);
}

CCPoint Phalanx::getSoldierPos(int x, int y, float direction, CCPoint startPt){
    x = x + startPt.x;
    y = y + startPt.y;
    float len = ccpDistance(ccp(x, y), ccp(0, 0));
    float angle1 = asin(y * 1.0 / len) * 180 / PI;
    float angle = direction;
    float tmpX = cos((angle + angle1) / 180 * PI);
    float tmpY = sin((angle + angle1) / 180 * PI);
    
    tmpX = len * tmpX;
    tmpY = len * tmpY;
    //    CCLOG("x:%d, y: %d, tmpx:%f, tmpy:%f", x, y, tmpX, tmpY);
    x = tmpX;
    y = tmpY;
    return ccp(x, y);
}

bool Phalanx::init(){
    m_killed = 0;
    m_row = 1;
    m_col = 1;
    gapX = 50;
    std::vector<int> vector = {10000, 50000};
    //    std::vector<int> vector = {1, 2};
    auto &map = MarchArmy::getMap(m_info);
//    if(m_type == BU){
//        //1*4, 2 * 5, 3 * 6
//        m_row = 3;
//        m_col = 6;
//        int num = map[BU].total;
//        if(num < vector[0]){
//            m_row = 1;
//            m_col = 4;
//        }else if(num >= vector[0] && num <= vector[1]){
//            m_row = 2;
//            m_col = 5;
//        }
//        gapX = 30;
//    }else if(m_type == QI){
//        //1*3, 2 * 3, 2 * 5
//        m_row = 2;
//        m_col = 5;
//        int num = map[QI].total;
//        if(num < vector[0]){
//            m_row = 1;
//            m_col = 3;
//        }else if(num >= vector[0] && num <= vector[1]){
//            m_row = 2;
//            m_col = 3;
//        }
//        gapX = 30;
//    }else if(m_type == GONG){
//        //1*4, 2 * 5, 3 * 6
//        m_row = 3;
//        m_col = 6;
//        int num = map[GONG].total;
//        if(num < vector[0]){
//            m_row = 1;
//            m_col = 4;
//        }else if(num >= vector[0] && num <= vector[1]){
//            m_row = 2;
//            m_col = 5;
//        }
//        gapX = 30;
//    }else if(m_type == CHE){
//        //1*1, 1 * 2, 1 * 3
//        m_row = 1;
//        m_col = 3;
//        int num = map[CHE].total;
//        if(num < vector[0]){
//            m_row = 1;
//            m_col = 1;
//        }else if(num >= vector[0] && num <= vector[1]){
//            m_row = 1;
//            m_col = 2;
//        }
//        gapX = 50;
//    }else if(m_type == TITAN){ //fusheng 泰坦
//        //1*1, 1 * 1, 1 * 1
//        m_row = 1;
//        m_col = 1;
//        int num = map[TITAN].total;
//        if(num < vector[0]){
//            m_row = 1;
//            m_col = 1;
//        }else if(num >= vector[0] && num <= vector[1]){
//            m_row = 1;
//            m_col = 1;
//        }
//        gapX = 50;
//    }
    int i = 0;
    int j = 0;
    while(i < m_row){
        j = 0;
        while(j < m_col){
            CCPoint realPt = getSoldierPosByRowAndCol(i, j, m_direction, m_row, m_col, startPt);
            Soldier *soldier = Soldier::create(m_sprite, m_type, m_direction, realPt, m_info, i, j);
            soldier->setZorder(i * m_col + (m_row - j));
            m_soldiers[i * m_col + j] = soldier;
            soldier->retain();
            j++;
        }
        i++;
    }
    return true;
}

void Phalanx::killed(int num){
    m_killed = num;
    if(m_killed > 0){
        int totalNum = m_col * m_row;
        int killNum = ceil(m_killed * totalNum / m_soldierNum);
        if(m_killed != m_soldierNum && killNum == m_soldierNum){
            killNum = m_soldierNum - 1;
        }
        auto arr = CCArray::create();
        while(arr->count() < m_soldiers.size()){
            arr->addObject(CCInteger::create(arr->count()));
        }
        while(arr->count() > killNum){
            arr->removeObjectAtIndex(rand() % arr->count());
        }
        CCObject *obj;
        float totalTime = 14.0f;
        float delayTime = 0.0f;
        
        CCARRAY_FOREACH(arr, obj) {
            int index = dynamic_cast<CCInteger*>(obj)->getValue();
            auto soldier = m_soldiers.find(index);
            if(soldier != m_soldiers.end()){
                soldier->second->playDeadAni(delayTime);
                delayTime += totalTime / (totalNum + 1);
            }
        }
    }
}

int Phalanx::getW(){
    return (m_row) * gapX;
}

Phalanx::~Phalanx(){
    for(auto it : m_soldiers){
        it.second->soldierDelete();
        it.second->release();
    }
    m_soldiers.clear();
}

//---

MarchArmy *MarchArmy::create(MarchInfo &info, CCSprite *parent){
    MarchArmy *ret = new MarchArmy(info, parent);
    CCLOG("createArmy");
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void MarchArmy::walk(){
    m_state = 0;
    map<int, Phalanx*>::iterator it;
    for(it = m_phalanx.begin(); it != m_phalanx.end(); it++){
        it->second->walk();
    }
}

void MarchArmy::attck(){
    if(m_state == 1){
        return;
    }
    m_state = 1;
    vector<CCPoint> vectorGONG;
    vector<CCPoint> vectorCHE;
    vector<CCPoint> vectorBU;
    vector<CCPoint> vectorQI;
    vector<float> angleVector;
    // 步兵
    float buW = _tile_width / 5;
    float buH = _tile_height / 5;
    if(m_info.targetType == FieldMonster || m_info.targetType == ActBossTile){
        buW = buW * 0.6;
        buH = buH * 0.6;
    }else if(m_info.targetType == Throne){
        buW = _tile_width * 1.25;
        buH = _tile_height * 1.25;
    }else if(m_info.targetType == Trebuchet){
        buW = buW * 1.2;
        buH = buH * 1.2;
    }
    vectorBU.push_back(ccp(-buW, -buH));
    vectorBU.push_back(ccp(-buW, buH));
    vectorBU.push_back(ccp(buW, buH));
    vectorBU.push_back(ccp(buW, -buH));
    // 骑兵
    float qiW = _tile_width / 4;
    float qiH = _tile_height / 4;
    if(m_info.targetType == FieldMonster || m_info.targetType == ActBossTile){
        qiW = qiW * 0.5;
        qiH = qiH * 0.5;
    }else if(m_info.targetType == Throne){
        qiW = _tile_width * 1.25;
        qiH = _tile_height * 1.25;
    }
    
    vectorQI.push_back(ccp(-qiW, -qiH));
    vectorQI.push_back(ccp(-qiW, qiH));
    vectorQI.push_back(ccp(qiW, qiH));
    vectorQI.push_back(ccp(qiW, -qiH));
    
    angleVector.push_back(235);
    angleVector.push_back(130);
    angleVector.push_back(50);
    angleVector.push_back(310);
    
    auto addPt = ccp(0, 0);
    if(WorldController::getInstance()->isFourGrid(m_info.targetType)){
        addPt = ccp(0, _tile_height / 2);
    }
    
    CCPoint startPt = WorldMapView::instance()->m_map->getViewPointByTilePoint(WorldController::getPointByIndex(MarchArmy::getEndIndex(m_info)),m_info.serverId) + ccp(0, _tile_height / 2);;
    CCPoint endPt = WorldMapView::instance()->m_map->getViewPointByTilePoint(WorldController::getPointByIndex(MarchArmy::getStartIndex(m_info)),m_info.serverId) + addPt;
    
    CCPoint crossPt = WorldController::getInstance()->getCrossPointAtCity(m_info);
    
    float angle = CCMathUtils::getAngle(startPt, endPt);
    angle = 360 + angle;
    angle = fmodf(angle, 360);
    
    int buIndex = 0;
    int qiIndex = 1;
    
    float a = atan2f(1, 2) * 180 / PI;
    if((angle >= 0 && angle < a) || (angle >= 360 - a && angle < 360)){
        buIndex = 0;
        qiIndex = 1;
    }else if(angle >= a && angle < 180 - a){
        buIndex = 3;
        qiIndex = 0;
    }else if(angle >= 180 - a && angle < 270 - a){
        buIndex = 2;
        qiIndex = 3;
    }else if(angle >= 270 - a && angle < 360 - a){
        buIndex = 1;
        qiIndex = 2;
    }
    
    float d1 = angleVector[buIndex];
    float d2 = angleVector[qiIndex];
    
    CCPoint pt = endPt - crossPt;
    
    CCPoint buPt = pt + vectorBU[buIndex];
    CCPoint qiPt = pt + vectorQI[qiIndex];
    
    if(m_info.targetType == Throne){
        //根据线段的坐标点找到2个最近的线，冲锋到那2个位置
//        "515,21|279,73;211,124|143,200;204,464|401,535;401,535|629,566;709,560|877,524;970,477|1055,405;1062,240|976,198;976,198|738,732;738,732|605,11"
        std::string ptStr = "16,327|50,261;50,261|111,208;111,208|198,169;198,169|278,152;278,152|360,147;360,147|451,157;451,157|527,179;527,179|587,218;587,218|640,261;640,261|670,327;670,327|640,408;640,408|587,453;587,453|527,487;527,487|451,516;451,516|360,530;360,530|278,516;278,516|198,487;198,487|112,453;112,453|50,398;50,398|16,327";
        auto centerPt = ccp(_tile_width * 1.0f / 2 * _throne_w + 40, _tile_height * 1.0f / 2 * _throne_h);
        std::vector<CCPoint> allPt;
        std::vector<std::string> allPtStr;
        std::vector<std::string> startEndVector;
        std::vector<std::string> pointVector;
        int h = 529;
        CCCommonUtils::splitString(ptStr, ";", allPtStr);
        int i = 0;
        while (i < allPtStr.size()) {
            startEndVector.clear();
            CCCommonUtils::splitString(allPtStr[i], "|", startEndVector);
            int j = 0;
            while (j < startEndVector.size()) {
                pointVector.clear();
                CCCommonUtils::splitString(startEndVector[j], ",", pointVector);
                CCPoint pt = CCPoint(atoi(pointVector[0].c_str()), h - atoi(pointVector[1].c_str())) - centerPt;
                allPt.push_back(pt);
                j++;
            }
            i++;
        }
        allPtStr.clear();
        startEndVector.clear();
        pointVector.clear();
        
        int findNum = 0;
        int totalNum = 2;
        int distance = 0;
        while (allPtStr.size() % 2 == 0 && findNum < totalNum) {
            std::vector<CCPoint>::iterator it;
            distance = 0;
            float angle;
            CCPoint lineCenterPt;
            for (auto ptIt = allPt.begin(); ptIt != allPt.end(); ptIt++) {
                auto &ptStart = *ptIt;
                ptIt++;
                if(ptIt == allPt.end()){
                    break;
                }
                auto &ptEnd = *ptIt;
                auto tmpCenterPt = (ptStart + ptEnd) / 2 + endPt;
                int tmpDistance = ccpDistance(tmpCenterPt, crossPt);

                if(distance == 0 || tmpDistance < distance){
                    it = ptIt - 1;
                    distance = tmpDistance;
                    lineCenterPt = tmpCenterPt;
                    angle = CCMathUtils::getAngle(ptStart, ptEnd) + 90;
                }
            }
            if(findNum == 0){
                d1 = angle;
                buPt = lineCenterPt - crossPt;
            }else{
                d2 = angle;
                qiPt = lineCenterPt - crossPt;
            }
            
            allPt.erase(it);
            allPt.erase(it);
            findNum++;
        }
    }

    float totalSpeed = sqrt(pow(m_info.speedX, 2) + pow(m_info.speedY, 2));
    CCPoint gongPt = ccp(0, 0);
    CCPoint chePt = gongPt + -ccp(m_info.speedX * _tile_height / totalSpeed * 0.8, m_info.speedY * _tile_height / totalSpeed * 0.8);
    
    auto arr = CCArray::create();
    
    auto addToArr = [](CCPoint &pt, float d, CCArray *arr){
        CCDictionary *dict = CCDictionary::create();
        dict->setObject(CCString::create(CC_ITOA(pt.x)), "x");
        dict->setObject(CCString::create(CC_ITOA(pt.y)), "y");
        dict->setObject(CCString::create(CC_ITOA(d)), "direction");
        arr->addObject(dict);
    };
    addToArr(buPt, d1, arr);
    addToArr(qiPt, d2, arr);
    if(isHaveArmy(BU)){
        m_phalanx[BU]->spreadTo(arr);
    }
    if(isHaveArmy(QI)){
        m_phalanx[QI]->spreadTo(arr);
    }
    arr->removeAllObjects();
    addToArr(gongPt, CCMathUtils::getAngle(endPt, startPt), arr);
    if(isHaveArmy(GONG)){
        m_phalanx[GONG]->spreadTo(arr);
    }
    arr->removeAllObjects();
    addToArr(chePt, CCMathUtils::getAngle(endPt, startPt), arr);
    if(isHaveArmy(CHE)){
        m_phalanx[CHE]->spreadTo(arr);
    }
    arr->removeAllObjects();
    addToArr(chePt, CCMathUtils::getAngle(endPt, startPt), arr);
    if(isHaveArmy(TITAN)){
        m_phalanx[TITAN]->spreadTo(arr);
    }
    if(m_info.targetType == CityTile || m_info.targetType == ResourceTile || m_info.targetType == Trebuchet || m_info.targetType == Throne || m_info.targetType == Tile_allianceArea || m_info.targetType == tile_banner){
        cityAttack(0);
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(MarchArmy::cityAttack), this, 2.6, 3, 0.4, false);
    }
    beAttacked();
    WorldMapView::instance()->addBatchItem(WarFire, MarchArmy::getStartIndex(m_info));
    
}

void MarchArmy::armyDelete(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
}

void MarchArmy::cityAttack(float _time){
    int index = 0;
    while(index < 20){
        playAttackAnimation();
        index++;
    }
}


void MarchArmy::beAttacked(){
    for(auto it = m_phalanx.begin(); it != m_phalanx.end(); it++){
        int killed = m_info.marchSoldier[it->first].dead;
        it->second->killed(killed);
    }
}

void MarchArmy::playAttackAnimation(){
    auto addPt = ccp(0, 0);
    if(m_info.targetType == CityTile || m_info.targetType == Trebuchet || m_info.targetType == Tile_allianceArea || m_info.targetType == tile_banner){
        addPt = ccp(0, _tile_height / 2);
    }
    CCPoint crossPt = WorldController::getInstance()->getCrossPointAtCity(m_info);
    CCPoint m_startPoint = (WorldMapView::instance()->m_map->getViewPointByTilePoint(WorldController::getPointByIndex(getStartIndex(m_info)),m_info.serverId) + addPt) - crossPt
    + ccp(20 - rand() % 40, -rand() % 20);
    
    if(m_info.targetType == Throne){
        auto attCross = WorldController::getInstance()->getCrossPointAtCity(m_info, 1.1);
        m_startPoint = attCross - crossPt + ccp(20 - rand() % 40, -rand() % 20);
    }
    
    CCPoint m_endPoint = ccp(100 - rand() % 200, 80 - rand() % 160);

    auto arrow = Arrow::create(m_parent);
    arrow->attack(m_startPoint, m_endPoint, 2.1);
}


bool MarchArmy::init(){
    m_state = 0;
    initArmy();
    return true;
}

void MarchArmy::initArmy(){
    auto addStartPt = ccp(0, 0);
    auto addEndPt = ccp(0, 0);
    if(m_info.stateType == StateReturn){
        addEndPt = ccp(0, _tile_height / 2);
    }else{
        addStartPt = ccp(0, _tile_height / 2);
    }
    
    if(WorldController::getInstance()->isFourGrid(m_info.targetType)){
        if(m_info.stateType == StateMarch){
            addEndPt = ccp(0, _tile_height / 2);
        }else{
            addStartPt = ccp(0, _tile_height / 2);
        }
    }
    
    CCPoint startPt = WorldMapView::instance()->m_map->getViewPointByTilePoint(WorldController::getPointByIndex(getStartIndex(m_info)),m_info.serverId) + addStartPt;
    CCPoint endPt = WorldMapView::instance()->m_map->getViewPointByTilePoint(WorldController::getPointByIndex(getEndIndex(m_info)),m_info.serverId) + addEndPt;
    
    int direction = CCMathUtils::getAngle(endPt, startPt);
    gapX = 0;
    gapY = 0;
    gapX += 5;
    auto &map = getMap(m_info);
    if(isHaveArmy(BU)){
        addArmyByType(BU, direction, map[BU].total, m_parent, 0);
    }
    
    if(isHaveArmy(QI)){
        addArmyByType(QI, direction, map[QI].total, m_parent, 3);
    }
    if(isHaveArmy(GONG)){
        addArmyByType(GONG, direction, map[GONG].total, m_parent, 5);
    }
    
    if(isHaveArmy(CHE)){
        addArmyByType(CHE, direction, map[CHE].total, m_parent, 8);
    }
    if(isHaveArmy(TITAN)){//fusheng 泰坦
        addArmyByType(TITAN, direction, map[TITAN].total, m_parent, 8);
    }
}

bool MarchArmy::isHaveArmy(int type){
    auto &map = getMap(m_info);
    
    std::map<int, MarchSoldierInfo>::iterator it = map.find(type);
    if(it == map.end()){
        return false;
    }
    return true;
}

void MarchArmy::addArmyByType(int type, float direction, int num, CCSprite *sprite, int startLine){
    map<int, Phalanx*>::iterator it = m_phalanx.find(type);
    if(it != m_phalanx.end()){
        it->second->release();
        m_phalanx.erase(it);
    }
    Phalanx *phalanx = Phalanx::create(type, direction, num, sprite, ccp(gapX, gapY), m_info);
    phalanx->retain();
    gapX += phalanx->getW() + 20;
    m_phalanx[type] = phalanx;
}

MarchArmy::~MarchArmy(){
    map<int, Phalanx*>::iterator it;
    for(it = m_phalanx.begin(); it != m_phalanx.end(); it++){
        it->second->release();
    }
    m_phalanx.clear();
    WorldMapView::instance()->delBatchItem(Rock, getStartIndex(m_info));
    WorldMapView::instance()->delBatchItem(Rock1, getStartIndex(m_info));
    WorldMapView::instance()->delBatchItem(Rock2, getStartIndex(m_info));
    WorldMapView::instance()->delBatchItem(WarFire, getStartIndex(m_info));
    armyDelete();
}

int MarchArmy::getStartIndex(MarchInfo &info){
    if(info.stateType == StateOccupy){
        return info.endPointIndex;
    }
    return info.startPointIndex;
}

int MarchArmy::getEndIndex(MarchInfo &info){
    if(info.stateType == StateOccupy){
        return info.startPointIndex;
    }
    return info.endPointIndex;
}

std::map<int, MarchSoldierInfo> &MarchArmy::getMap(MarchInfo &info){
    if (info.action == MarchActionPrepareAttack) {
        return info.marchSoldier;
    }
    return info.walkSoldier;
}