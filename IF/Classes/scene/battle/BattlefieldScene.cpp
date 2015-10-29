//
//  BattlefieldScene.cpp
//  IF
//
//  Created by ganxiaohua on 14-6-18.
//
//

#include "BattlefieldScene.h"
#include "SceneController.h"
#include "UIComponent.h"
#include "SoundController.h"

bool BattlefieldScene::init(){
    bool bRet = false;
    if(CCLayer::init()){
        this->setTouchEnabled(true);
        bRet = true;
        if (CCCommonUtils::isIosAndroidPad())
        {
            this->setScale(1.25);
        }
        m_atkQiNodes = CCArray::create();
        m_atkGongNodes = CCArray::create();
        m_atkCheNodes = CCArray::create();
        m_atkBuNodes = CCArray::create();
        m_defGongNodes = CCArray::create();
        m_defCheNodes = CCArray::create();
        m_solParNodes = CCArray::create();
        m_warParNodes = CCArray::create();
        m_warBmNodes = CCArray::create();
        m_warSFiresNodes = CCArray::create();
        
        m_wallSmokeNodes = CCArray::create();
        m_wolfSmokeNodes = CCArray::create();

        CCLoadSprite::doResourceByPathIndex(IMPERIAL_PATH,1, true);
        CCLoadSprite::doResourceByPathIndex(IMPERIAL_PATH,28, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByPathIndex(IMPERIAL_PATH,1, false);
//            CCLoadSprite::doResourceByPathIndex(IMPERIAL_PATH,28, false);
        });
        
        cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Battle/Battle_soldier.plist");
        
//        CCBLoadFile("BattleSceneCCB",this,this);
        if (true || CCCommonUtils::isTestPlatformAndServer("guide_skip_2")) {
            CCBLoadFile("BattleSceneCCB_2",this,this, true);
        }else {
            CCBLoadFile("BattleSceneCCB",this,this);
        }
        
        m_defArmys = CCArray::create();
        m_attackArmys = CCArray::create();
        m_attackChe = CCArray::create();
        m_battleGen = NULL;
        m_battleZhuan = NULL;
        
        m_soldierNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("a010_0_N_move_0.png")->getTexture());
        this->addChild(m_soldierNode, 10);
        
        m_wallDesAni = CCNode::create();
        m_wallDesAni->setPosition(m_wallNode->getPosition());
        this->addChild(m_wallDesAni, 20);
        
        m_spainLayer = CCLayer::create();
        this->addChild(m_spainLayer, 25);
        
        m_jianNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("jian_0.png")->getTexture());
        this->addChild(m_jianNode, 30);
        
        m_warParLayer = CCLayer::create();
        this->addChild(m_warParLayer, 40);
        
        m_solParLayer = CCLayer::create();
        this->addChild(m_solParLayer, 50);
        
//        auto bg = CCLoadSprite::createScale9Sprite("goBack_btn.png");
//        CCLabelIF* lable = CCLabelIF::create("start", 25);
//        lable->setAnchorPoint(ccp(0.5, 0.5));
//        lable->setColor(ccRED);
//        CCControlButton* goBack = CCControlButton::create(lable,bg);
//        goBack->addTargetWithActionForControlEvents(this, cccontrol_selector(BattlefieldScene::goBack), CCControlEventTouchUpInside);
//        goBack->setPreferredSize(CCSize(228, 52));
//        goBack->setPosition(2040,1000);
//        this->addChild(goBack);
        
        initBattle();
        
    }
    return bRet;
}

void BattlefieldScene::initBattle()
{
    int zorder = 70;
    for (int i=0; i<m_atkBuNodes->count(); i++) { //攻击 步兵
        zorder += 10;
        auto node = dynamic_cast<CCNode*>(m_atkBuNodes->objectAtIndex(i));
        BattleArmy* chengBingL = BattleArmy::create(m_jianNode,m_soldierNode,node->getPosition(),0,0);
        chengBingL->setPosition(node->getPosition());
        this->addChild(chengBingL, zorder);
        m_attackArmys->addObject(chengBingL);
    }
    
    for (int i=0; i<m_atkQiNodes->count(); i++) { //攻击 骑兵
        zorder += 10;
        auto node = dynamic_cast<CCNode*>(m_atkQiNodes->objectAtIndex(i));
        BattleArmy* chengBingL = BattleArmy::create(m_jianNode,m_soldierNode,node->getPosition(),2,0);
        chengBingL->setPosition(node->getPosition());
        this->addChild(chengBingL, zorder);
        m_attackArmys->addObject(chengBingL);
    }
    
    for (int i=0; i<m_atkGongNodes->count(); i++) { //攻击 弓兵
        zorder += 10;
        auto node = dynamic_cast<CCNode*>(m_atkGongNodes->objectAtIndex(i));
        BattleArmy* chengBingL = BattleArmy::create(m_jianNode,m_soldierNode,node->getPosition(),4,0);
        chengBingL->setPosition(node->getPosition());
        this->addChild(chengBingL, zorder);
        m_attackArmys->addObject(chengBingL);
    }
    
    for (int i=0; i<m_atkCheNodes->count(); i++) { //攻击 车兵
//        zorder += 10;
        auto node = dynamic_cast<CCNode*>(m_atkCheNodes->objectAtIndex(i));
//        BattleArmy* chengBingL = BattleArmy::create(m_jianNode,m_soldierNode,node->getPosition(),3,0);
//        chengBingL->setPosition(node->getPosition());
//        this->addChild(chengBingL, zorder);
//        m_attackArmys->addObject(chengBingL);
        
        if (true) {
            auto animationObj = new IFSkeletonAnimation("Spine/Imperial/tsc.json","Imperial/Imperial_30.atlas");
            if (animationObj) {
                animationObj->setVisibleStop(false);
                animationObj->setPosition(node->getPosition());
                m_spainLayer->addChild(animationObj);
                spTrackEntry* entry = animationObj->setAnimation(0, "tsc_dj", true);
//                animationObj->timeScale = entry->endTime/8.0f;
                m_attackChe->addObject(animationObj);
                animationObj->setScale(0.9);
            }
        }
    }
    
    
    for (int i=0; i<m_defCheNodes->count(); i++) { //防守 车兵
        zorder += 10;
        auto node = dynamic_cast<CCNode*>(m_defCheNodes->objectAtIndex(i));
        BattleArmy* chengBingL = BattleArmy::create(m_jianNode,m_soldierNode,node->getPosition(),7,0);
        chengBingL->setPosition(node->getPosition());
        this->addChild(chengBingL, zorder);
        m_defArmys->addObject(chengBingL);
    }
    
    for (int i=0; i<m_defGongNodes->count(); i++) { //防守 弓兵
        zorder += 10;
        auto node = dynamic_cast<CCNode*>(m_defGongNodes->objectAtIndex(i));
        int key = i>2?99:0;
        BattleArmy* chengBingL = BattleArmy::create(m_jianNode,m_soldierNode,node->getPosition(),1,1,key);
        chengBingL->setPosition(node->getPosition());
        this->addChild(chengBingL, zorder);
        m_defArmys->addObject(chengBingL);
    }
    
    m_battleGen = BattleArmy::create(m_jianNode,m_soldierNode,m_atk_gen1->getPosition(),5, 0);
    m_battleGen->setPosition(m_atk_gen1->getPosition());
    this->addChild(m_battleGen, zorder+10);
    
    m_battleZhuan = BattleArmy::create(m_jianNode,m_soldierNode,m_def_gen1->getPosition(),6, 0);
    m_battleZhuan->setPosition(m_def_gen1->getPosition());
    this->addChild(m_battleZhuan, zorder+20);
    
    onShowWarParticle();
    
//    CCSequence* sc = CCSequence::create(CCMoveTo::create(5, ccp(686.7, -357.3)),CCCallFunc::create(this, callfunc_selector(BattlefieldScene::startAttack)),NULL);
//    m_battleZhuan->runAction(sc);
}

void BattlefieldScene::restart(){
    if(m_battleGen){
        m_battleGen->removeFromParentAndCleanup(true);
    }
    if(m_battleZhuan){
        m_battleZhuan->removeFromParentAndCleanup(true);
    }
    m_jianNode->removeAllChildrenWithCleanup(true);
    int num = m_attackArmys->count();
    for (int i=0; i<num; i++) {
        auto army = dynamic_cast<BattleArmy*>(m_attackArmys->objectAtIndex(i));
        if(army){
            army->destory();
            army->removeAllChildrenWithCleanup(true);
        }
    }
    num = m_defArmys->count();
    for (int i=0; i<num; i++) {
        auto army = dynamic_cast<BattleArmy*>(m_defArmys->objectAtIndex(i));
        if(army){
            army->destory();
            army->removeAllChildrenWithCleanup(true);
        }
    }
    m_attackArmys->removeAllObjects();
    m_defArmys->removeAllObjects();
    this->initBattle();
}

void BattlefieldScene::moveScreen(float _time)
{
    CCDictionary* dic = CCDictionary::create();
    dic->setObject(CCString::create("0.5"), "time");
    if (CCCommonUtils::isIosAndroidPad())
    {
        dic->setObject(CCString::create("2500"), "x");
    }
    else
        dic->setObject(CCString::create("2380"), "x");
    
    dic->setObject(CCString::create("610"), "y");
    dic->setObject(CCString::create("1.0"), "scale");
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MOVE_TO_POSITION,dic);
    
    this->scheduleOnce(schedule_selector(BattlefieldScene::guidSoldier), 0.5);//跑动停下来之后，弹出剧情面板
}
void BattlefieldScene::noticeToSoldier(float _time)//引导开始，传令兵跑动
{
    m_battleZhuan->playAnimation(ACTION_MOVE,0,NULL,0,1);
    this->scheduleOnce(schedule_selector(BattlefieldScene::moveScreen), 2);//跑动停下来之后, 拉伸镜头
//    this->scheduleOnce(schedule_selector(BattlefieldScene::guidSoldier), 2);//跑动停下来之后，弹出剧情面板
}
void BattlefieldScene::guidSoldier(float _time)
{//跑动停下来之后，弹出剧情面板
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("begin1"));
}
void BattlefieldScene::noticeToGen(cocos2d::CCObject *obj)
{
    noticeToGen(0.0);
}
void BattlefieldScene::noticeToGen(float _time)//引导将军开始攻击
{
    SoundController::sharedSound()->playEffects(Music_Sfx_guide_attack_city);
    m_battleGen->playAnimation(ACTION_ATTACK,0,CCCallFunc::create(this, callfunc_selector(BattlefieldScene::startAttack)),0,1);
    this->scheduleOnce(schedule_selector(BattlefieldScene::startPlayChe), 0.5);
}

void BattlefieldScene::startAttack()
{
//    m_battleGen->hideSoldier(0.5);
//    m_battleZhuan->hideSoldier(0.5);
    
//    m_battleZhuan->playAnimation(ACTION_STAND);
    
    CCDictionary* dic = CCDictionary::create();
//    dic->setObject(CCString::create("2"), "time");
//    dic->setObject(CCString::create("2010"), "x");
//    dic->setObject(CCString::create("770"), "y");
//    dic->setObject(CCString::create("1.0"), "scale");
    
    dic->setObject(CCString::create("2.5"), "time");
    dic->setObject(CCString::create("1860"), "x");
    dic->setObject(CCString::create("870"), "y");
    dic->setObject(CCString::create("1.0"), "scale");
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MOVE_TO_POSITION,dic);
    
    int num = m_attackArmys->count();
    for (int i=0; i<num; i++) {//进攻方 弓箭 和 车兵 攻击
        auto army = dynamic_cast<BattleArmy*>(m_attackArmys->objectAtIndex(i));
        int armType = army->getType();
        if(armType!=0 && armType!=2){
            army->playAnimation(ACTION_ATTACK);
        }
    }
    num = m_defArmys->count();
    for (int i=0; i<num; i++) {//防御方 弓箭 和 车兵 攻击
        auto army = dynamic_cast<BattleArmy*>(m_defArmys->objectAtIndex(i));
        if(army){
            army->playAnimation(ACTION_ATTACK);
        }
    }
    
//    this->schedule(schedule_selector(BattlefieldScene::playBoombParticle), 0.8);
    this->scheduleOnce(schedule_selector(BattlefieldScene::startPlayBoom), 1);
    
    this->scheduleOnce(schedule_selector(BattlefieldScene::moveToTheGate), 0.3);
    //this->moveToTheGate();
}

void BattlefieldScene::startPlayBoom(float _time)
{
    this->schedule(schedule_selector(BattlefieldScene::playBoombParticle), 0.8);
    this->scheduleOnce(schedule_selector(BattlefieldScene::playWallDestroy), 1.3);
}

void BattlefieldScene::moveToTheGate(float _time){
    int num = m_attackArmys->count();
    for (int i=0; i<num; i++) {//进攻方 步兵 和 骑兵 移动
        auto army = dynamic_cast<BattleArmy*>(m_attackArmys->objectAtIndex(i));
        int armType = army->getType();
        if(armType==0){
            army->playAnimation(ACTION_MOVE);
        }
    }
    onShowParticle();
    //移动快完了的时候 结束播放
    this->scheduleOnce(schedule_selector(BattlefieldScene::atBattleAttack), 6);
}

void BattlefieldScene::atBattleAttack(float _time)
{
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("begin2"));
//    int num = m_attackArmys->count();
//    for (int i=0; i<num; i++) {
//        auto army = dynamic_cast<BattleArmy*>(m_attackArmys->objectAtIndex(i));
//        if(army){
//            army->hideSoldier(1);
//        }
//    }
//    num = m_defArmys->count();
//    for (int i=0; i<num; i++) {
//        auto army = dynamic_cast<BattleArmy*>(m_defArmys->objectAtIndex(i));
//        if(army){
//            army->hideSoldier(1);
//        }
//    }
//    this->scheduleOnce(schedule_selector(BattlefieldScene::removeSelf), 1);
}
void BattlefieldScene::removeSelf(cocos2d::CCObject *obj)
{
    removeSelf(0.0);
}
void BattlefieldScene::removeSelf(float _time)
{
    int num = m_attackArmys->count();
    for (int i=0; i<num; i++) {
        auto army = dynamic_cast<BattleArmy*>(m_attackArmys->objectAtIndex(i));
        if(army){
            army->hideSoldier(0);
        }
    }
    num = m_defArmys->count();
    for (int i=0; i<num; i++) {
        auto army = dynamic_cast<BattleArmy*>(m_defArmys->objectAtIndex(i));
        if(army){
            army->hideSoldier(0);
        }
    }
    
    this->removeFromParent();
    SoundController::sharedSound()->playBGMusic(Music_M_city_1);
//    SoundController::sharedSound()->playBGMusic(Music_M_city_1_preview);
    UIComponent::getInstance()->setVisible(true);
}

void BattlefieldScene::onEnter(){
    CCLayer::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BattlefieldScene::goBack), GUIDE_BEGIN1, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BattlefieldScene::noticeToGen), GUIDE_BEGIN2, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BattlefieldScene::removeSelf), GUIDE_BATTLE_END, NULL);
    //添加背景音乐
    SoundController::sharedSound()->playBGMusic(Music_Begin_Music);
    UIComponent::getInstance()->setVisible(false);
}
void BattlefieldScene::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GUIDE_BEGIN1);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GUIDE_BEGIN2);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GUIDE_BATTLE_END);
    CCLayer::onExit();
}

bool BattlefieldScene::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    return true;
}

void BattlefieldScene::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void BattlefieldScene::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    
}
void BattlefieldScene::goBack(cocos2d::CCObject *obj)
{
    goBack(0.0);
}
void BattlefieldScene::goBack(float _time){
//    noticeToGen();
//    return ;
    
    if (true || CCCommonUtils::isTestPlatformAndServer("guide_skip_2")) {
        CCDictionary* dic = CCDictionary::create();
        dic->setObject(CCString::create("7.0"), "time");
        dic->setObject(CCString::create("2380"), "x");
        dic->setObject(CCString::create("610"), "y");
        dic->setObject(CCString::create("1.0"), "scale");
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MOVE_TO_POSITION,dic);
        this->scheduleOnce(schedule_selector(BattlefieldScene::noticeToSoldier), 7.1);
    }else {
        CCDictionary* dic = CCDictionary::create();
        dic->setObject(CCString::create("0.5"), "time");//0.5
        dic->setObject(CCString::create("2380"), "x");
        dic->setObject(CCString::create("610"), "y");
        dic->setObject(CCString::create("1.0"), "scale");//1.0
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MOVE_TO_POSITION,dic);
        
        this->scheduleOnce(schedule_selector(BattlefieldScene::noticeToSoldier), 0.6);
    }
//    restart();
}

SEL_MenuHandler BattlefieldScene::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

SEL_CCControlHandler BattlefieldScene::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

bool BattlefieldScene::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    if (pTarget == this && strncmp(pMemberVariableName, "m_atk_qi",8) == 0) {
        m_atkQiNodes->addObject(pNode);
        return true;
    }
    else if (pTarget == this && strncmp(pMemberVariableName, "m_atk_gong",10) == 0) {
        m_atkGongNodes->addObject(pNode);
        return true;
    }
    else if (pTarget == this && strncmp(pMemberVariableName, "m_atk_che",9) == 0) {
        m_atkCheNodes->addObject(pNode);
        return true;
    }
    else if (pTarget == this && strncmp(pMemberVariableName, "m_atk_bu",8) == 0) {
        m_atkBuNodes->addObject(pNode);
        return true;
    }
    else if (pTarget == this && strncmp(pMemberVariableName, "m_def_gong",10) == 0) {
        m_defGongNodes->addObject(pNode);
        return true;
    }
    else if (pTarget == this && strncmp(pMemberVariableName, "m_def_che",9) == 0) {
        m_defCheNodes->addObject(pNode);
        return true;
    }
    else if (pTarget == this && strncmp(pMemberVariableName, "m_sol_par",9) == 0) {
        m_solParNodes->addObject(pNode);
        return true;
    }
    else if (pTarget == this && strncmp(pMemberVariableName, "m_war_fire",10) == 0) {
        m_warParNodes->addObject(pNode);
        return true;
    }
    else if (pTarget == this && strncmp(pMemberVariableName, "m_war_s_fire",12) == 0) {
        m_warSFiresNodes->addObject(pNode);
        return true;
    }
    else if (pTarget == this && strncmp(pMemberVariableName, "m_war_bm",8) == 0) {
        m_warBmNodes->addObject(pNode);
        return true;
    }
    else if (pTarget == this && strncmp(pMemberVariableName, "m_wallSmoke",11) == 0) {
        m_wallSmokeNodes->addObject(pNode);
        return true;
    }
    else if (pTarget == this && strncmp(pMemberVariableName, "m_wolfSmoke",11) == 0) {
        m_wolfSmokeNodes->addObject(pNode);
        return true;
    }
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_atk_gen1", CCNode*, this->m_atk_gen1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_def_gen1", CCNode*, this->m_def_gen1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wallNode", CCNode*, this->m_wallNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desWallParNode", CCNode*, this->m_desWallParNode);
    return false;
}

void BattlefieldScene::addParticleToBatch(cocos2d::CCParticleSystemQuad *particle) {
    auto batchCount = m_parVec.size();
    while (batchCount--) {
        auto &batch = m_parVec[batchCount];
        if (batch) {
            auto batchBlend = batch->getBlendFunc();
            auto particleBlend = particle->getBlendFunc();
            if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
                batch->addChild(particle);
                return;
            }
        }
    }
    auto newBatch = ParticleController::createParticleBatch();
    m_solParLayer->addChild(newBatch);
    newBatch->addChild(particle);
    m_parVec.push_back(newBatch);
}

void BattlefieldScene::onHideParticle()
{
    auto batchCount = m_parVec.size();
    while (batchCount--) {
        m_solParLayer->removeChild(m_parVec[batchCount]);
    }
    m_parVec.clear();
}

void BattlefieldScene::onShowParticle()
{
//    for (int i=0; i<m_solParNodes->count(); i++) {
//        auto node = dynamic_cast<CCNode*>(m_solParNodes->objectAtIndex(i));
//        auto particle1 = ParticleController::createParticle("SoldierSmoke_1");
//        particle1->setPosition(node->getPosition());
//        addParticleToBatch(particle1);
//        auto particle2 = ParticleController::createParticle("SoldierSmoke_2");
//        particle2->setPosition(node->getPosition());
//        addParticleToBatch(particle2);
//    }
//    CCActionInterval * moveBy = CCMoveBy::create(5,ccp(-300,150));
//    m_solParLayer->runAction(moveBy);
}

void BattlefieldScene::addWarParticleToBatch(cocos2d::CCParticleSystemQuad *particle) {
    auto batchCount = m_warParVec.size();
    while (batchCount--) {
        auto &batch = m_warParVec[batchCount];
        if (batch) {
            auto batchBlend = batch->getBlendFunc();
            auto particleBlend = particle->getBlendFunc();
            if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
                batch->addChild(particle);
                return;
            }
        }
    }
    auto newBatch = ParticleController::createParticleBatch();
    m_warParLayer->addChild(newBatch);
    newBatch->addChild(particle);
    m_warParVec.push_back(newBatch);
}

void BattlefieldScene::onHideWarParticle()
{
    auto batchCount = m_warParVec.size();
    while (batchCount--) {
        m_warParLayer->removeChild(m_warParVec[batchCount]);
    }
    m_warParVec.clear();
}

void BattlefieldScene::onShowWarParticle()
{
    for (int i=0; i<m_warParNodes->count(); i++) {
        for (int j=1; j<=5; j++) {
            auto node = dynamic_cast<CCNode*>(m_warParNodes->objectAtIndex(i));
            auto particle1 = ParticleController::createParticle(CCString::createWithFormat("WarFire_%d",j)->getCString());
            particle1->setPosition(node->getPosition());
            addWarParticleToBatch(particle1);
        }
    }
    
    for (int i=0; i<m_warSFiresNodes->count(); i++) {
        for (int j=2; j<=4; j++) {
            auto node = dynamic_cast<CCNode*>(m_warSFiresNodes->objectAtIndex(i));
            auto particle1 = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",j)->getCString());
            particle1->setPosition(node->getPosition());
            addWarParticleToBatch(particle1);
        }
    }
    
    for (int i=0; i<m_wolfSmokeNodes->count(); i++) {
        for (int j=1; j<=2; j++) {
            auto node = dynamic_cast<CCNode*>(m_wolfSmokeNodes->objectAtIndex(i));
            auto particle1 = ParticleController::createParticle(CCString::createWithFormat("WarSmoke_%d",j)->getCString());
            particle1->setPosition(node->getPosition());
            addWarParticleToBatch(particle1);
        }
    }
}

void BattlefieldScene::playBoombParticle(float _time)
{
    if (m_bmIdx < m_warBmNodes->count()) {
        for (int j=1; j<=9; j++) {
            auto node = dynamic_cast<CCNode*>(m_warBmNodes->objectAtIndex(m_bmIdx));
            auto particle1 = ParticleController::createParticle(CCString::createWithFormat("MassDestruction_%d",j)->getCString());
            particle1->setPosition(node->getPosition());
            addWarParticleToBatch(particle1);
        }
        
        if (m_bmIdx==0 || m_bmIdx==2 || m_bmIdx==4) {
            for (int j=1; j<=4; j++) {
                auto node = dynamic_cast<CCNode*>(m_warBmNodes->objectAtIndex(m_bmIdx));
                auto particle1 = ParticleController::createParticle(CCString::createWithFormat("Collapse_L_%d",j)->getCString());
                particle1->setPosition(node->getPosition());
                addWarParticleToBatch(particle1);
            }
        }
        m_bmIdx++;
        if (m_bmIdx>=m_warBmNodes->count()) {
            this->unschedule(schedule_selector(BattlefieldScene::playBoombParticle));
        }
//        float dt = CCMathUtils::getRandom(0.5, 1);
//        this->scheduleOnce(schedule_selector(BattlefieldScene::playBoombParticle), dt);
    }
}

void BattlefieldScene::playWallDestroy(float _time)
{
    m_wallNode->removeAllChildren();
    
    auto effSpr0 = CCSprite::create();
    CCCommonUtils::makeOnceEffectSpr(effSpr0, "GatesBroken_%d.png", 4, 0, 0.1);
    effSpr0->setPosition(ccp(-191, -193));
    m_wallNode->addChild(effSpr0);
    
    for (int j=1; j<=8; j++) {
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("Collapse_%d",j)->getCString());
        particle1->setPosition(m_desWallParNode->getPosition());
        addWarParticleToBatch(particle1);
    }
    
    for (int j=1; j<=5; j++) {
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("Collapse_U_%d",j)->getCString());
        particle1->setPosition(m_desWallParNode->getPosition());
        addWarParticleToBatch(particle1);
    }
    
    for (int j=1; j<=4; j++) {
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("Collapse_L_%d",j)->getCString());
        particle1->setPosition(m_desWallParNode->getPosition());
        addWarParticleToBatch(particle1);
    }
    this->scheduleOnce(schedule_selector(BattlefieldScene::playWallFire), 0.5);
}

void BattlefieldScene::playWallFire(float _time)
{
    for (int j=1; j<=5; j++) {
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("WarFire_%d",j)->getCString());
        particle1->setPosition(m_desWallParNode->getPosition()+ccp(50,0));
        addWarParticleToBatch(particle1);
    }
    
    for (int i=0; i<m_wallSmokeNodes->count(); i++) {
        for (int j=2; j<=3; j++) {
            auto node = dynamic_cast<CCNode*>(m_wallSmokeNodes->objectAtIndex(i));
            auto particle1 = ParticleController::createParticle(CCString::createWithFormat("WarFire_%d",j)->getCString());
            particle1->setPosition(node->getPosition());
            addWarParticleToBatch(particle1);
        }
    }
    
    for (int j=1; j<=9; j++) {
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("MassDestruction_%d",j)->getCString());
        particle1->setPosition(ccp(1938, 1000));
        addWarParticleToBatch(particle1);
    }
}

void BattlefieldScene::startPlayChe(float _time)
{
    for (int m=0; m<m_attackChe->count(); m++) {
        IFSkeletonAnimation* animationObj = dynamic_cast<IFSkeletonAnimation*>(m_attackChe->objectAtIndex(m));
        if (animationObj) {
            spTrackEntry* entry = animationObj->setAnimation(0, "tsc_gj", true);
            animationObj->setTimeScale(entry->endTime/2.0f);
        }
    }
    this->scheduleOnce(schedule_selector(BattlefieldScene::playBigFireBall), 0.6);
}

void BattlefieldScene::playBigFireBall(float _time)
{
    auto startArray = CCPointArray::create(20);
    startArray->addControlPoint(ccp(2360, 480));
    startArray->addControlPoint(ccp(2480, 520));
    startArray->addControlPoint(ccp(2660, 570));
    startArray->addControlPoint(ccp(2820, 620));
    startArray->addControlPoint(ccp(2980, 670));
    
    auto endArray = CCPointArray::create(20);
    endArray->addControlPoint(ccp(1640, 860));
    endArray->addControlPoint(ccp(1750, 920));
    endArray->addControlPoint(ccp(1860, 980));
    endArray->addControlPoint(ccp(1980, 1050));
    endArray->addControlPoint(ccp(2070, 1080));
    
    vector<float> delayTimeVec;
    delayTimeVec.push_back(0);
    delayTimeVec.push_back(0.3);
    delayTimeVec.push_back(0);
    delayTimeVec.push_back(0.3);
    delayTimeVec.push_back(0);
    
    vector<float> moveTimeVec;
    moveTimeVec.push_back(2.0);
    moveTimeVec.push_back(2.1);
    moveTimeVec.push_back(2.2);
    moveTimeVec.push_back(2.1);
    moveTimeVec.push_back(2.0);
    
    int zorder = 3000;
    m_guideParticleNode = ParticleController::createParticleBatch();
    this->addChild(m_guideParticleNode, zorder-10);
    
    for (int i=0; i<5; i++)
    {
        auto tmpNode = CCNode::create();
        tmpNode->setPosition(startArray->getControlPointAtIndex(i));
        this->addChild(tmpNode, zorder+i);
        
        for (int j=1; j<=3; j++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("Fireball_%d",j)->getCString());
            particle->setPositionType(kCCPositionTypeRelative);
            
            if (j==1) {
                particle->setPosition(startArray->getControlPointAtIndex(i));
                m_guideParticleNode->addChild(particle);
                
                CCActionInterval * delay1 = CCDelayTime::create(delayTimeVec[i]);
                CCActionInterval * moveTo1 = CCMoveTo::create(moveTimeVec[i], endArray->getControlPointAtIndex(i));
                CCActionInterval * easeSineOut1 = CCEaseSineOut::create(moveTo1);
                auto func1 = CCCallFuncO::create(this, callfuncO_selector(BattlefieldScene::playEndBigBall1), particle);
                particle->runAction(CCSequence::create(delay1, easeSineOut1, func1, NULL));
                
            }else {
                tmpNode->addChild(particle);
            }
        }
        
//        ccBezierConfig  beziercofig;
//        beziercofig.controlPoint_1 = ccp(startArray->getControlPointAtIndex(i).x+200,startArray->getControlPointAtIndex(i).y+100);
//        beziercofig.controlPoint_2 = ccp(startArray->getControlPointAtIndex(i).x+200,endArray->getControlPointAtIndex(i).y+100);
//        beziercofig.endPosition = endArray->getControlPointAtIndex(i);
//        CCActionInterval *forward = CCBezierTo::create(moveTimeVec[i], beziercofig);
        
        CCActionInterval * delay = CCDelayTime::create(delayTimeVec[i]);
        CCActionInterval * moveTo = CCMoveTo::create(moveTimeVec[i], endArray->getControlPointAtIndex(i));
        CCActionInterval * easeSineOut = CCEaseSineOut::create(moveTo);
        auto func = CCCallFuncO::create(this, callfuncO_selector(BattlefieldScene::playEndBigBall), tmpNode);
        tmpNode->runAction(CCSequence::create(delay, easeSineOut, func, NULL));
    }
}

void BattlefieldScene::playEndBigBall(CCObject *obj)
{
    CCNode* node = dynamic_cast<CCNode*>(obj);
    if (node) {
        
        for (int j=1; j<=8; j++) {
            auto particle1 = ParticleController::createParticle(CCString::createWithFormat("Collapse_%d",j)->getCString());
            particle1->setPosition(node->getPosition());
            addWarParticleToBatch(particle1);
        }
        for (int j=1; j<=5; j++) {
            auto particle1 = ParticleController::createParticle(CCString::createWithFormat("Collapse_U_%d",j)->getCString());
            particle1->setPosition(node->getPosition());
            addWarParticleToBatch(particle1);
        }
        for (int j=1; j<=4; j++) {
            auto particle1 = ParticleController::createParticle(CCString::createWithFormat("Collapse_L_%d",j)->getCString());
            particle1->setPosition(node->getPosition());
            addWarParticleToBatch(particle1);
        }
        for (int j=1; j<=5; j++) {
            auto particle1 = ParticleController::createParticle(CCString::createWithFormat("WarFire_%d",j)->getCString());
            particle1->setPosition(node->getPosition()+ccp(50,0));
            addWarParticleToBatch(particle1);
        }
        
        node->stopAllActions();
        node->removeFromParent();
    }
}

void BattlefieldScene::playEndBigBall1(CCObject* obj)
{
    CCNode* node = dynamic_cast<CCNode*>(obj);
    if (node) {
        node->removeFromParent();
    }
}