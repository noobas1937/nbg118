//
//  HarvestSkillAni.cpp
//  IF
//
//  Created by ganxiaohua on 14-11-3.
//
//

#include "HarvestSkillAni.h"
#include "SoundController.h"
#include "ParticleController.h"

using namespace cocos2d;

HarvestSkillAni* HarvestSkillAni::create(int type)
{
    HarvestSkillAni *pRet = new HarvestSkillAni();
    if (pRet && pRet->initHarvestSkillAni(type))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool HarvestSkillAni::initHarvestSkillAni(int type)
{
    m_dropNodes = CCArray::create();
    m_dropSprs = CCArray::create();
    string ccbName = "";
    string picName = "";
    musicName = "";
    int sprCnt = 5;
    float dt = 0.1;
    if (type == R_GOLD) {
        picName = "ActivitiesCoin";
        sprCnt = 7;
        musicName = Music_Sfx_button_click_reward;
        dt = 0.05;
    }
    else if (type == R_WOOD) {
        picName = "Wood";
        ccbName = "HarvestWood";
        musicName = Music_Sfx_city_harvest_wood;
    }
    else if (type == R_FOOD) {
        picName = "Food";
        ccbName = "HarvestFood";
        musicName = Music_Sfx_city_harvest_farm;
    }
    else if (type == R_IRON) {
        picName = "Iron";
        ccbName = "HarvestIron";
        musicName = Music_Sfx_city_harvest_iron;
    }
    else if (type == R_STONE) {
        picName = "Stone";
        ccbName = "HarvestStone";
        musicName = Music_Sfx_city_harvest_mithril;
    }
    CCBLoadFile(ccbName.c_str(),this,this);
    
    if (picName == "") {
        return true;
    }
    
//    for (int i=0; i<m_dropNodes->count(); i++) {
//        CCNode* curNode = dynamic_cast<CCNode*>(m_dropNodes->objectAtIndex(i));
//        auto pic = CCSprite::create();
//        CCCommonUtils::makeEffectSpr(pic, picName+"_%d.png", sprCnt, dt, i%sprCnt);
//        CCCommonUtils::setSpriteMaxSize(pic, 70, true);
//        
//        //        auto pic = CCLoadSprite::createSprite(picName.c_str());
//        m_dropSprs->addObject(pic);
//        curNode->addChild(pic);
//    }
    double rand = GlobalData::shared()->getRand(0.2,2.2);
    this->scheduleOnce(schedule_selector(HarvestSkillAni::onPlayEnd), rand);
    return true;
}

void HarvestSkillAni::onEnter() {
    CCNode::onEnter();
}

void HarvestSkillAni::onExit() {
    CCNode::onExit();
}

void HarvestSkillAni::onPlayEnd(float _time)
{
//    for (int i=0; i<m_dropSprs->count(); i++) {
//        CCSprite* spr = dynamic_cast<CCSprite*>(m_dropSprs->objectAtIndex(i));
//        if (spr) {
//            spr->runAction(CCFadeOut::create(1));
//        }
//    }
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(HarvestSkillAni::onDelete));
    this->getAnimationManager()->runAnimationsForSequenceNamed("Harvest");
    this->schedule(schedule_selector(HarvestSkillAni::onPlayMusic), 0.2, 8, 0);
    for (int i=1; i<11; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("Harvest_%d",i)->getCString());//CityRockfall_ Harvest_
        //particle->setPosition(ccp(GlobalData::shared()->getRand(-200,50),GlobalData::shared()->getRand(-20,20)));
        this->addChild(particle);
    }
    //this->scheduleOnce(schedule_selector(HarvestSkillAni::onDelete), 1);
}

void HarvestSkillAni::onPlayMusic(float _time)
{
    if (musicName != "") {
        SoundController::sharedSound()->playEffects(musicName.c_str());
    }
}

void HarvestSkillAni::onDelete()
{
    m_dropSprs->removeAllObjects();
    m_dropNodes->removeAllObjects();
    this->removeFromParent();
}

#pragma mark -
#pragma mark CocosBuilder Part
SEL_MenuHandler HarvestSkillAni::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

SEL_CCControlHandler HarvestSkillAni::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

bool HarvestSkillAni::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    if (pTarget == this && strncmp(pMemberVariableName, "m_dropNode",10) == 0) {
        pNode->removeAllChildren();
        m_dropNodes->addObject(pNode);
        return true;
    }
    return false;
}