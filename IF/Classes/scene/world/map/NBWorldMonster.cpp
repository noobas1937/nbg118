//
//  NBWorldMonster.cpp
//  IF
//
//  Created by gj on 10/15/15.
//
//

#include "NBWorldMonster.hpp"
#include "WorldController.h"
#include "DynamicTiledMap.h"
#include "IFFieldMonsterNode.h"
#include "CCMathUtils.h"
#include "ParticleController.h"
#include "NBWorldNPC.hpp"

#define WORLD_MAP_VIEW WorldMapView::instance()
#define TILED_MAP WorldMapView::instance()->m_map

#pragma mark - WorldMapView

void WorldMapView::monsterDeathCB(CCObject* obj)
{
    CCArray* arr = dynamic_cast<CCArray*>(obj);
    if(arr)
    {
        int index = ((CCInteger*)arr->objectAtIndex(0))->getValue();
        IFFieldMonsterNode* monster = dynamic_cast<IFFieldMonsterNode*>(m_mapMonsterNode->getChildByTag(getBatchTag(MonsterDead, index)));
        
        if (monster)
        {
            string name = ((CCString*)arr->objectAtIndex(1))->getCString();
            int direction = ((CCInteger*)arr->objectAtIndex(2))->getValue();
            monster->setSpineState(name, MonsterDead, direction,false);
            
            auto city = m_cityInfo.find(index);
            if(city != m_cityInfo.end()){
                int curCityIdx = city->second.cityIndex;
                auto items = m_cityItem.find(curCityIdx);
                if (items != m_cityItem.end()) {
                    vector<CCSafeObject<CCNode> >::iterator it = m_cityItem[curCityIdx].begin();
                    for (; it<m_cityItem[curCityIdx].end(); it++) {
                        CCSprite* tmpSpr = dynamic_cast<CCSprite*>( it->getObject() );
                        if (tmpSpr) {
                            CCFadeOut* fadeOutt = CCFadeOut::create(1.5f);
                            tmpSpr->runAction(fadeOutt);
                        }
                    }
                }
            }
            
            CCDelayTime* delayTime = CCDelayTime::create(3.0f);
            CCFadeOut* fadeOut = CCFadeOut::create(2.0f);
            CCCallFuncO* callFunc = CCCallFuncO::create(this, callfuncO_selector(WorldMapView::refreshMonster), CCInteger::create(index));
            CCSequence* seq = CCSequence::create(delayTime, fadeOut, callFunc, nullptr);
            monster->runAction(seq);
        }
        
        CC_SAFE_RELEASE_NULL(arr);
    }
}

void WorldMapView::refreshMonster(CCObject *obj){
    CCInteger *in = dynamic_cast<CCInteger*>(obj);
    if(in){
        auto city = m_cityInfo.find(in->getValue());
        if(city != m_cityInfo.end()){
            if(city->second.fieldMonsterInfo.currentHp == 0 && city->second.cityType == MonsterTile){
                releaseCity(city->second);
                delBatchItem(MonsterDead, city->second.cityIndex);
                delBatchItem(MonsterDead1, city->second.cityIndex);
                
                city->second.cityIcon = -1;
                city->second.cityType = OriginTile;
            }else{
                createCity(city->second);
            }
        }
    }
}

void WorldMapView::addAttackParticle(CCObject *obj){
    CCDictionary* dict = dynamic_cast<CCDictionary*>(obj);
    std::string directionStr = dict->valueForKey("str")->getCString();
    int index = dict->valueForKey("index")->intValue();
    int x = dict->valueForKey("x")->intValue();
    int y = dict->valueForKey("y")->intValue();
    int count = dict->valueForKey("total")->intValue();
    if (count > 0 )
    {
        for (int i = 1; i <= count; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",directionStr.c_str(),i)->getCString());
            particle->setPosition(ccp(x, y));
            addParticleToBatch(particle,getBatchTag(MonsterAttackParticle, index));
            m_cityItem[index].push_back(particle);
        }
    }
    else {
        vector<string> particlesVec;
        CCCommonUtils::splitString(directionStr, "|", particlesVec);
        for (int m=0; m<particlesVec.size(); m++)
        {
            vector<string> partNameVec;
            CCCommonUtils::splitString(particlesVec[m], ";", partNameVec);
            if (partNameVec.size() == 2)
            {
                string partName = partNameVec[0];
                vector<string> partIdxVec;
                CCCommonUtils::splitString(partNameVec[1], ",", partIdxVec);
                for (int j=0; j<partIdxVec.size(); j++) {
                    auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%s",partName.c_str(),partIdxVec[j].c_str())->getCString());
                    particle->setPosition(ccp(x, y));
                    addParticleToBatch(particle,getBatchTag(MonsterAttackParticle, index));
                    m_cityItem[index].push_back(particle);
                }
                
            }
        }
    }
}

#pragma mark - NBWorldMonster

void NBWorldMonster::addFieldMonsterUnderNode(const WorldCityInfo& info, const Vec2& pos, unsigned int index)
{
//    WORLD_MAP_VIEW->addBatchItem(Shadow, index);
    if(info.fieldMonsterInfo.isHpChange){
        if(info.fieldMonsterInfo.currentHp == 0){
            createMonsterBatchItem(MonsterDead, index);
        }else{
            createMonsterBatchItem(MonsterAttack, index);
        }
    }else{
        createMonsterBatchItem(MonsterBreath, index);
        // createMonsterBatchItem(MonsterDead, index);
    }
    WORLD_MAP_VIEW->addBatchItem(MonsterProBG1, index);
    auto lv = CCLabelBatch::create(CCCommonUtils::getPropById(info.fieldMonsterInfo.monsterId, "level").c_str(), WORLD_MAP_VIEW->m_labelNode);
    lv->setScale(MapGlobalScale);
    lv->setColor(ccc3(255, 235, 180));
    lv->setPosition(ccp(pos.x - 45, pos.y-55));
    WORLD_MAP_VIEW->m_cityItem[index].push_back(lv);
    std::string nameStr = CCCommonUtils::getNameById(info.fieldMonsterInfo.monsterId);
    auto nameText = CCLabelIFTTF::create();
    WORLD_MAP_VIEW->m_unBatchLabelNode->addChild(nameText);
    nameText->setColor(ccc3(225, 174, 101));
    nameText->setFontSize(17);
    nameText->setAnchorPoint(ccp(0, 0.5));
    nameText->setPosition(ccp(-20, -55)+pos);
    nameText->setString(nameStr.c_str());
    WORLD_MAP_VIEW->m_cityItem[index].push_back(nameText);
}

void NBWorldMonster::addActBossTileUnderNode(const WorldCityInfo& info, const Vec2& pos, unsigned int index)
{
    if(info.parentCityIndex != info.cityIndex) return;
    
//    WORLD_MAP_VIEW->addBatchItem(Shadow, index);
    if(info.fieldMonsterInfo.isHpChange){
        if(info.fieldMonsterInfo.currentHp == 0){
            createMonsterBatchItem(MonsterDead, index);
        }else{
            createMonsterBatchItem(MonsterAttack, index);
        }
    }else{
        createMonsterBatchItem(MonsterBreath, index);
    }
    WORLD_MAP_VIEW->addBatchItem(MonsterProBG1, index);
    auto lv = CCLabelBatch::create(CCCommonUtils::getPropById(info.fieldMonsterInfo.monsterId, "level").c_str() , WORLD_MAP_VIEW->m_labelNode);
    lv->setScale(MapGlobalScale);
    lv->setColor(ccc3(255, 235, 180));
    lv->setPosition(ccp(pos.x - 45, pos.y-55));
    WORLD_MAP_VIEW->m_cityItem[index].push_back(lv);
    std::string nameStr = CCCommonUtils::getNameById(info.fieldMonsterInfo.monsterId);
    auto nameText = CCLabelIFTTF::create();
    WORLD_MAP_VIEW->m_unBatchLabelNode->addChild(nameText);
    nameText->setColor(ccc3(225, 174, 101));
    nameText->setFontSize(17);
    nameText->setAnchorPoint(ccp(0, 0.5));
    nameText->setPosition(ccp(-20, -55)+pos);
    nameText->setString(nameStr.c_str());
    WORLD_MAP_VIEW->m_cityItem[index].push_back(nameText);
}

void NBWorldMonster::createMonsterBatchItem(BatchTagType type, unsigned int index)
{
    unsigned int batch_tag = WORLD_MAP_VIEW->getBatchTag(type, index);
    if (WORLD_MAP_VIEW->m_batchNode->getChildByTag(batch_tag)) {
        return;
    }
    if (WORLD_MAP_VIEW->m_mapMonsterNode->getChildByTag(batch_tag)) {
        return;
    }
    
    auto &info = WORLD_MAP_VIEW->m_cityInfo[index];
    auto pos = TILED_MAP->getViewPointByTilePoint(WorldController::getPointByIndex(index), info.tileServerId);
    CCSprite* item = NULL;
    
    int zorder = 0;
    
    std::string monsterIcon = CCCommonUtils::getPropById(info.fieldMonsterInfo.monsterId, "monster");
    if (monsterIcon == ""){
        monsterIcon = "rm";
    }
    
    IFFieldMonsterNode* monsterNode = IFFieldMonsterNode::create();
    
    std::string monsterJsonName = CCCommonUtils::getPropById(info.fieldMonsterInfo.monsterId, "monster_update");
    if (monsterJsonName == "") {
        monsterJsonName = monsterIcon;
    }
    
    if(!monsterNode->isSkeExit(monsterJsonName))
    {
        monsterIcon = "rm";
        monsterJsonName = "rm";
    }
    
    int direction = 0;
    bool flipX = false;
    if(type == MonsterAttack || type == MonsterDead)
    {
        for(auto it = WorldController::getInstance()->m_marchInfo.begin(); it != WorldController::getInstance()->m_marchInfo.end(); it++){
            if(it->second.startPointIndex == info.cityIndex && (it->second.action == MarchActionAttack || it->second.action == MarchActionPrepareAttack)){
                auto startPt = TILED_MAP->getViewPointByTilePoint(WorldController::getPointByIndex(it->second.endPointIndex),info.tileServerId);
                auto endPt = TILED_MAP->getViewPointByTilePoint(WorldController::getPointByIndex(it->second.startPointIndex),info.tileServerId);
                int angle = CCMathUtils::getAngle(startPt, endPt);
                angle = (angle + 360) % 360;
                //                            if(angle > 180){
                //                                direction = 2;
                //                            }
                if((angle >= 90 && angle < 180) || (angle >= 180 && angle <= 270)){
                    flipX = true;
                }
                
                break;
            }
        }
    }else{
        flipX = (info.cityIndex % 2 == 1);
    }
    
    if (type == MonsterDead)
    {
        CCArray* arr = CCArray::create();
        arr->addObject(CCInteger::create(index));
        arr->addObject(CCString::create(monsterJsonName));
        arr->addObject(CCInteger::create(direction));
        CC_SAFE_RETAIN(arr);
        CCCallFuncO* callFunc = CCCallFuncO::create(WORLD_MAP_VIEW, callfuncO_selector(WorldMapView::monsterDeathCB), arr);
        monsterNode->setSpineState(monsterJsonName, MonsterAttack, direction,true,callFunc,9);
        
    }
    else if(type == MonsterAttack)
    {
        CCCallFuncO* callFunc = CCCallFuncO::create(WORLD_MAP_VIEW, callfuncO_selector(WorldMapView::refreshMonster), CCInteger::create(index));
        monsterNode->setSpineState(monsterJsonName, MonsterAttack, direction,true,callFunc,9);
    }
    else
    {
        monsterNode->setSpineState(monsterJsonName, MonsterBreath, direction,true);
    }
    
    if(flipX)
    {
        monsterNode->setScaleX(-1.0f);
    }
    else
    {
        monsterNode->setScaleX(1.0f);
    }
    
    int dy = -50;
    int dx = 0;
    if(monsterIcon == "rock"){
        dy = -30;
    }
    else if(monsterIcon == "Kulou"){
        dy = -25;
    }
    else if(monsterIcon == "Yeren"){
        dy = -30;
        if(!flipX)
        {
            dx = 10;
        }
    }
    else if(monsterIcon == "rm"){
        dy = -30;
        if(!flipX)
        {
            dx = 10;
        }
        else
        {
            dx = -10;
        }
    }
    else if(monsterIcon == "Shixianggui"){
        dy = -10;
        if(!flipX)
        {
            dx = -5;
        }
        else
        {
            dx = 5;
        }
    }//Griffin
    else if(monsterIcon == "Griffin"){
        dy = -50+20;
        if(!flipX)
        {
            dx = 15 - 30;
        }
        else
        {
            dx = -10 + 30;
        }
    }
    else if(monsterIcon == "scws"){
        dy = 0;
        if(!flipX) {
            dx = -10;
        }
        else {
            dx = 10;
        }
    }
    else if(monsterIcon.find("renzhe") != string::npos){
        dy = -10;
    }
    else if(monsterIcon == "meidusha"){
        dy = -33;
        if(!flipX) {
            dx = -40;
        }
        else {
            dx = 40;
        }
    }
    
    auto aniPos = pos + ccp(dx, dy);
    monsterNode->setPosition(aniPos);
    
    float durTime = 1.0f;
    // 添加粒子效果
    
    {
        CCSequence *par = NULL;
        if((type == MonsterDead || type == MonsterAttack)){
            std::string particleIcon = "rm";
            if(monsterIcon == "rock" || monsterIcon == "rm" || monsterIcon == "Griffin" || monsterIcon == "Shixianggui" || monsterIcon == "Lion" || monsterIcon == "scws"){
                particleIcon = monsterIcon;
            }
            auto dict = CCDictionary::create();
            int posX = monsterNode->getPositionX();
            int posY = monsterNode->getPositionY();
            std::string directionStr = "rm_LD_";
            if(direction == 0){
                if(!flipX){
                    directionStr = particleIcon + "_LD_";
                    posX -= 50;
                }else{
                    directionStr = particleIcon + "_RD_";
                    posX += 50;
                }
            }else if(direction == 2){
                if(!flipX){
                    directionStr = particleIcon + "_LU_";
                    posX -= 30;
                    posY += 30;
                }else{
                    directionStr = particleIcon + "_RU_";
                    posX += 45;
                    posY += 45;
                }
            }
            if(particleIcon == "rock"){
                directionStr = "rock_";
            }
            else if(particleIcon == "Lion")
            {
                directionStr = "RockGolem_";
                if(direction == 0)
                {
                    if(!flipX)
                    {
                        posX -= 40;
                        posY -= 60;
                    }
                    else
                    {
                        posX += 40;
                        //                                    posY -= 60;
                    }
                }
                else
                {
                    if(!flipX)
                    {
                        posX -= 35;
                        posY -= 30;
                    }
                    else
                    {
                        //                                    posX += 35;
                        //                                    posY -= 65;
                    }
                }
                
            }
            else if(particleIcon == "scws"){
                if( type == MonsterDead ) {
                    directionStr = "Crown_;1,2,3,4|FireBossDeath_;1,2,3,4,5,6,7,8,9";
                    if(!flipX){
                        posX += 60;
                        posY += 30;
                    }else{
                        posX -= 60;
                        posY += 30;
                    }
                }
                else if (type == MonsterAttack){
                    if(!flipX){
                        directionStr = "FireBossAttack_RD_";
                        posX += 100;
                        posY += 50;
                    }else{
                        directionStr = "FireBossAttack_LD_";
                        posX -= 100;
                        posY += 55;
                    }
                }
            }
            
            dict->setObject(CCString::create(directionStr), "str");
            dict->setObject(CCString::create(CC_ITOA(posX)), "x");
            dict->setObject(CCString::create(CC_ITOA(posY)), "y");
            dict->setObject(CCString::create(CC_ITOA(int(index))), "index");
            int total = 6;
            float delayTime1 = durTime-0.5;
            if(particleIcon == "rock"){
                total = 10;
            }else if(particleIcon == "rm"){
                total = 5;
            }else if(particleIcon == "Shixianggui"){
                total = 4;
            }
            else if(particleIcon == "Lion")
            {
                total = 10;
                delayTime1 = durTime- 0.2f;
            }
            else if(particleIcon == "scws")
            {
                if( type == MonsterDead ) {
                    total = 0;
                    delayTime1 = 2.2f;
                }
                else if (type == MonsterAttack){
                    total = 4;
                    //                                    delayTime1 = 1.2f;
                }
            }
            
            dict->setObject(CCString::create(CC_ITOA(total)), "total");
            
            float delayTime = durTime;
            par = CCSequence::create(CCDelayTime::create(delayTime1),
                                     CCCallFuncO::create(WORLD_MAP_VIEW, callfuncO_selector(WorldMapView::addAttackParticle), dict),
                                     CCDelayTime::create(delayTime),
                                     CCCallFuncO::create(WORLD_MAP_VIEW, callfuncO_selector(WorldMapView::addAttackParticle), dict),
                                     CCDelayTime::create(delayTime),
                                     CCCallFuncO::create(WORLD_MAP_VIEW, callfuncO_selector(WorldMapView::addAttackParticle), dict),
                                     CCDelayTime::create(delayTime),
                                     CCCallFuncO::create(WORLD_MAP_VIEW, callfuncO_selector(WorldMapView::addAttackParticle), dict),
                                     CCDelayTime::create(delayTime),
                                     CCCallFuncO::create(WORLD_MAP_VIEW, callfuncO_selector(WorldMapView::addAttackParticle), dict),
                                     CCDelayTime::create(delayTime),
                                     CCCallFuncO::create(WORLD_MAP_VIEW, callfuncO_selector(WorldMapView::addAttackParticle), dict),
                                     CCDelayTime::create(delayTime),
                                     CCCallFuncO::create(WORLD_MAP_VIEW, callfuncO_selector(WorldMapView::addAttackParticle), dict),
                                     CCDelayTime::create(delayTime),
                                     CCCallFuncO::create(WORLD_MAP_VIEW, callfuncO_selector(WorldMapView::addAttackParticle), dict),
                                     CCDelayTime::create(delayTime),
                                     CCCallFuncO::create(WORLD_MAP_VIEW, callfuncO_selector(WorldMapView::addAttackParticle), dict),
                                     NULL);
            monsterNode->runAction(par);
        }
        
        if(type != MonsterDead)
        {
            if (monsterIcon == "scws")
            {
                int posX = monsterNode->getPositionX();
                int posY = monsterNode->getPositionY();
                int dy = 20;
                int dx = -10;
                if(!flipX) {
                    dx = 10;
                }else {
                    dx = -10;
                }
                for (int i = 2; i <= 6; i++) {
                    auto particle = ParticleController::createParticle(CCString::createWithFormat("WeaponsFire_Wood_%d",i)->getCString());
                    particle->setPosition(ccp(posX+dx, posY+dy));
                    WORLD_MAP_VIEW->addParticleToBatch(particle, WORLD_MAP_VIEW->getBatchTag(MonsterAttackParticle, index));
                    WORLD_MAP_VIEW->m_cityItem[index].push_back(particle);
                }
            }else if (monsterIcon == "meidusha"){
                int posX = monsterNode->getPositionX();
                int posY = monsterNode->getPositionY();
                int dy = 10;
                int dx = -20;
                if(!flipX) {
                    dx = 30;
                }else {
                    dx = -30;
                }
                for (int i = 1; i <= 2; i++) {
                    auto particle = ParticleController::createParticle(CCString::createWithFormat("Medusa_%d",i)->getCString());
                    particle->setPosition(ccp(posX+dx, posY+dy));
                    WORLD_MAP_VIEW->addParticleToBatch(particle, WORLD_MAP_VIEW->getBatchTag(MonsterAttackParticle, index));
                    WORLD_MAP_VIEW->m_cityItem[index].push_back(particle);
                }
            }
        }
        
        zorder = info.cityIndex;
        monsterNode->setTag(WORLD_MAP_VIEW->getBatchTag(BatchTagType(type), index));
        WORLD_MAP_VIEW->m_cityItem[index].push_back(monsterNode);
        
        WORLD_MAP_VIEW->m_mapMonsterNode->addChild(monsterNode, zorder);
    }
    
    // guo.jiang todo
    if (monsterNode)
    {
        int tag = 1000000 * 100 + monsterNode->getTag();
        auto c = WORLD_MAP_VIEW->m_mapMonstersNode->getChildByTag(tag);
        if (c)
        {
            c->removeFromParent();
        }
        
        if (type == MonsterAttack)
        {
            monsterNode->setVisible(false);
        
            auto octopus = Sprite::createWithSpriteFrameName("attack_0.png");
            auto *ac1 = NBWorldNPC::createAnimation("World/World_5.plist", "attack_%d.png", 0, 7);
            octopus->runAction(ac1);
        
            octopus->setScaleX(monsterNode->getScaleX());
            octopus->setPosition(monsterNode->getPosition() + Vec2(octopus->getContentSize().width * 0, octopus->getContentSize().height / 4));
            octopus->setTag(tag);
            WORLD_MAP_VIEW->m_mapMonstersNode->addChild(octopus, monsterNode->getZOrder());
        }
        else if (type != MonsterDead)
        {
            monsterNode->setVisible(false);
            
            auto octopus = Sprite::createWithSpriteFrameName("waiting_0.png");
            auto *ac1 = NBWorldNPC::createAnimation("World/World_5.plist", "waiting_%d.png", 0, 7);
            octopus->runAction(ac1);
            
            octopus->setScaleX(monsterNode->getScaleX());
            octopus->setPosition(monsterNode->getPosition() + Vec2(octopus->getContentSize().width * 0, octopus->getContentSize().height / 4));
            octopus->setTag(tag);
            WORLD_MAP_VIEW->m_mapMonstersNode->addChild(octopus, monsterNode->getZOrder());
        }
    }
    
    return;

}