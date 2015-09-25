//
//  AllianceFlagPar.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-20.
//
//

#include "AllianceFlagPar.h"
#include "ParticleController.h"

AllianceFlagPar* AllianceFlagPar::create(std::string icon){
    AllianceFlagPar* ret = new AllianceFlagPar(icon);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


void AllianceFlagPar::onEnter(){
    CCNode::onEnter();
    setSwallowsTouches(false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup, false);
}

void AllianceFlagPar::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool AllianceFlagPar::init(){
    if (!CCNode::init()) {
        return false;
    }
    CCSprite* flag = CCLoadSprite::createSprite(m_icon.c_str());
    this->addChild(flag,0,444);
    this->setContentSize(flag->getContentSize());
    
    createPar(m_icon);
    
//    m_partileNode = CCNode::create();
//    this->addChild(m_partileNode);
    
//    auto particle = ParticleController::createParticle("FlagGrail_1");
//    m_partileNode->addChild(particle);
//    
//    particle = ParticleController::createParticle("FlagGrail_2");
//    particle->setPositionY(30);
//    m_partileNode->addChild(particle);
//    
//    particle = ParticleController::createParticle("FlagGrail_3");
//    particle->setPositionY(30);
//    m_partileNode->addChild(particle);
//    
//    particle = ParticleController::createParticle("FlagGrail_4");
//    particle->setPositionY(30);
//    m_partileNode->addChild(particle);
    
    m_touchTime = 0;
    return true;
}

bool AllianceFlagPar::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    unsigned long current = clock()/1000;
    if((current-m_touchTime)>200){
        m_touchTime = current;
        return true;
    }
    return false;
}

void AllianceFlagPar::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    int gapTime = clock()/1000 - m_touchTime;
}

void AllianceFlagPar::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void AllianceFlagPar::createPar(std::string icon){
    if(icon == "Blood.png"){
        bloodPar();
    }
    if(icon == "Snow.png"){
        snowPar();
    }
    if(icon == "Warhammer.png"){
        warhammerPar();
    }
    if(icon == "Tree.png"){
        treePar();
    }
    if(icon == "Cure.png"){
        curePar();
    }
    if(icon == "Death.png"){
        deathPar();
    }
    if(icon == "Fire.png"){
        firePar();
    }
    if(icon == "Ocean.png"){
        oceanPar();
    }
    if(icon == "Sun.png"){
        sunPar();
    }
    if(icon == "Wolf.png"){
        wolfPar();
    }
    if(icon == "Angel.png"){
        m_partileNode = CCNode::create();
        this->addChild(m_partileNode);
        for (int i=1; i<5; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("FlagAngel_%d",i)->getCString());
            particle->setPosition(0, 0);
            m_partileNode->addChild(particle);
        }
    }
    if(icon == "Cattle.png"){
        m_partileNode = CCNode::create();
        this->addChild(m_partileNode);
        for (int i=1; i<6; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("FlagCattle_%d",i)->getCString());
            particle->setPosition(0, 0);
            m_partileNode->addChild(particle);
        }
    }
    if(icon == "FireWolf.png"){
        m_partileNode = CCNode::create();
        this->addChild(m_partileNode);
        
        auto particle = ParticleController::createParticle("FireWolf_1");
        particle->setPosition(-6, 18);
        m_partileNode->addChild(particle);
        
        particle = ParticleController::createParticle("FireWolf_2");
        particle->setPosition(-12, 22);
        m_partileNode->addChild(particle);
        
        particle = ParticleController::createParticle("FireWolf_3");
        particle->setPosition(6, 20);
        m_partileNode->addChild(particle);
    }
    if(icon == "Grail.png"){
        m_partileNode = CCNode::create();
        this->addChild(m_partileNode);
        for (int i=1; i<5; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("FlagGrail_%d",i)->getCString());
            particle->setPosition(0, 0);
            m_partileNode->addChild(particle);
        }
    }
    if(icon == "Knight.png"){
        m_partileNode = CCNode::create();
        this->addChild(m_partileNode);
        
        auto particle = ParticleController::createParticle("FlagDeath_4");
        particle->setPosition(3, 17);
        m_partileNode->addChild(particle);
        
        particle = ParticleController::createParticle("FlagDeath_4");
        particle->setPosition(-6, 21);
        m_partileNode->addChild(particle);
        
        particle = ParticleController::createParticle("FlagKnight_R");
        particle->setPosition(48, -17);
        m_partileNode->addChild(particle);
        
        particle = ParticleController::createParticle("FlagKnight_L");
        particle->setPosition(-50, -15);
        m_partileNode->addChild(particle);
        
        particle = ParticleController::createParticle("FlagKnight_1");
        particle->setPosition(0, 18);
        m_partileNode->addChild(particle);
        
        particle = ParticleController::createParticle("FlagKnight_2");
        particle->setPosition(0, -32);
        m_partileNode->addChild(particle);
        
        particle = ParticleController::createParticle("FlagKnight_3");
        particle->setPosition(0, 91);
        m_partileNode->addChild(particle);

        particle = ParticleController::createParticle("FlagKnight_4");
        particle->setPosition(-3, -40);
        m_partileNode->addChild(particle);
        
        particle = ParticleController::createParticle("FlagKnight_4");
        particle->setPosition(-40, -60);
        m_partileNode->addChild(particle);
        
        particle = ParticleController::createParticle("FlagKnight_4");
        particle->setPosition(-20, -54);
        m_partileNode->addChild(particle);
        
        particle = ParticleController::createParticle("FlagKnight_4");
        particle->setPosition(45, -40);
        m_partileNode->addChild(particle);
        
    }
}

FlagAniCCB* FlagAniCCB::create(std::string flag)
{
    FlagAniCCB *pRet = new FlagAniCCB();
    if (pRet && pRet->init(flag))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool FlagAniCCB::init(std::string flag)
{
    m_flag = flag;
    
    CCBLoadFile(m_flag.c_str(),this,this);
    this->getAnimationManager()->runAnimationsForSequenceNamed(m_flag.c_str());
    return true;
}

void FlagAniCCB::onEnter() {
    CCNode::onEnter();
}
void FlagAniCCB::onExit() {
    CCNode::onExit();
}

SEL_CCControlHandler FlagAniCCB::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool FlagAniCCB::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    return false;
}

void AllianceFlagPar::bloodPar(){
    m_partileNode = CCNode::create();
    
    auto particle = ParticleController::createParticle("FlagBlood_1");
    particle->setPosition(-18, -7);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagBlood_2");
    particle->setPosition(18, -7);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagBlood_3");
    particle->setPosition(-20, -9);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagBlood_3");
    particle->setPosition(20, -9);
    m_partileNode->addChild(particle);
    
    this->addChild(m_partileNode);
}

void AllianceFlagPar::snowPar(){
    m_partileNode = CCNode::create();
    
    auto particle = ParticleController::createParticle("FlagSnow_1");
    particle->setPosition(0, 0);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagSnow_2");
    particle->setPosition(0, 0);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagSnow_3");
    particle->setPosition(0, 30);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagSnow_4");
    particle->setPosition(0, 0);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagSnow_5");
    particle->setPosition(0, -50);
    m_partileNode->addChild(particle);
    
    this->addChild(m_partileNode);
}
void AllianceFlagPar::warhammerPar(){
    m_partileNode = CCNode::create();
    
    auto particle = ParticleController::createParticle("FlagWarhammer_1");
    particle->setPosition(0, 0);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagWarhammer_2");
    particle->setPosition(0, 0);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagWarhammer_3");
    particle->setPosition(0, 0);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagWarhammer_4");
    particle->setPosition(0, 0);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagWarhammer_5");
    particle->setPosition(0, 0);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagWarhammer_6");
    particle->setPosition(0, -50);
    m_partileNode->addChild(particle);
    
    this->addChild(m_partileNode);
}
void AllianceFlagPar::treePar(){
    m_partileNode = CCNode::create();
    
    auto particle = ParticleController::createParticle("FlagTree_1");
    particle->setPosition(0, 0);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagTree_2");
    particle->setPosition(0, 0);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagTree_3");
    particle->setPosition(0, 0);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagTree_4");
    particle->setPosition(0, 0);
    m_partileNode->addChild(particle);
    //ccb动画
//    FlagAniCCB* aniNode = FlagAniCCB::create("light");
//    this->addChild(aniNode);
    
    this->addChild(m_partileNode);
}
void AllianceFlagPar::curePar(){
    m_partileNode = CCNode::create();
    
    auto particle = ParticleController::createParticle("FlagCure_1");
    particle->setPosition(25, 37);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagCure_2");
    particle->setPosition(-25, 37);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagCure_3");
    particle->setPosition(0, 0);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagCure_4");
    particle->setPosition(0, 0);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagCure_5");
    particle->setPosition(0, 40);
    m_partileNode->addChild(particle);
    //ccb动画
    FlagAniCCB* aniNode = FlagAniCCB::create("cure");
    this->addChild(aniNode);
    
    this->addChild(m_partileNode);
}
void AllianceFlagPar::deathPar(){
    m_partileNode = CCNode::create();
    
    auto particle = ParticleController::createParticle("FlagDeath_1");
    particle->setPosition(0, 0);
//    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagDeath_2");
    particle->setPosition(0, 0);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagDeath_3");
    particle->setPosition(-5, -18);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagDeath_4");
    particle->setPosition(12, 2);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagDeath_4");
    particle->setPosition(-17, 2);
    m_partileNode->addChild(particle);
    
    this->addChild(m_partileNode);
}
void AllianceFlagPar::firePar(){
    m_partileNode = CCNode::create();
    
    auto particle = ParticleController::createParticle("FlagFire_1");
    particle->setPosition(0, 0);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagFire_2");
    particle->setPosition(-20, -5);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagFire_3");
    particle->setPosition(23, -5);
    m_partileNode->addChild(particle);
    
    this->addChild(m_partileNode);
}
void AllianceFlagPar::oceanPar(){
    m_partileNode = CCNode::create();
    
    auto particle = ParticleController::createParticle("FlagOcean_1");
    particle->setPosition(0, 0);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagOcean_2");
    particle->setPosition(0, -35);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagOcean_3");
    particle->setPosition(0, -35);
    m_partileNode->addChild(particle);
    
    this->addChild(m_partileNode);
}
void AllianceFlagPar::sunPar(){
    m_partileNode = CCNode::create();
    
    auto particle = ParticleController::createParticle("FlagSun_1");
    particle->setPosition(0, 30);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagSun_2");
    particle->setPosition(0, 16);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagSun_3");
    particle->setPosition(0, 16);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagSun_4");
    particle->setPosition(0, 16);
    m_partileNode->addChild(particle);
    
    this->addChild(m_partileNode);
}
void AllianceFlagPar::wolfPar(){
    m_partileNode = CCNode::create();
    
    auto particle = ParticleController::createParticle("FlagWolf_1");
    particle->setPosition(0, 30);
    m_partileNode->addChild(particle);
    particle = ParticleController::createParticle("FlagWolf_2");
    particle->setPosition(6, 18);
    m_partileNode->addChild(particle);
    //ccb动画
//    FlagAniCCB* aniNode = FlagAniCCB::create("wolf");
//    this->addChild(aniNode);
    
    this->addChild(m_partileNode);
}
