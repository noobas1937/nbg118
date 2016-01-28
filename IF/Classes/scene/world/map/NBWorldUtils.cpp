//
//  NBWorldUtils.cpp
//  DragonClans
//
//  Created by gj on 12/23/15.
//
//

#include "NBWorldUtils.hpp"
#include "CCCommonUtils.h"
#include "NBWorldNPC.hpp"
//#include "ParticleController.h"

//#define WORLD_MAP_VIEW WorldMapView::instance()

std::string NBWorldUtils::getMonsterBustImage(std::string monsterId)
{
    std::string monster = CCCommonUtils::getPropById(monsterId, "monster");
    std::string lv = CCCommonUtils::getPropById(monsterId, "level");
    if (monster == "Kulou")
    {
        return monster + lv + "_bust.png";
    }
    return monster + "_bust.png";
}

//void addParticles(Node* parent, const Vec2& particlePos, float rotation = 0)
//{
//    const char * particleFiles[] = {"WorldRipple_1", "WorldRipple_2", "WorldRipple_3"};
//    
//    for (int i = 0; i < 3; ++i)
//    {
//        auto particle = ParticleController::createParticle(particleFiles[i]);
//        particle->setPosition(particlePos);
//        particle->setRotation(rotation);
//        parent->addChild(particle);
//        
//        WORLD_MAP_VIEW->addParticleToBatch(particle,getBatchTag(CSProtectTag, index));
//        WORLD_MAP_VIEW->m_cityItem[index].push_back(particle);
//    }
//}

cocos2d::Node* NBWorldUtils::createSeaMonsterAndWaitingAnimation(std::string monsterId)
{
    std::string monster = CCCommonUtils::getPropById(monsterId, "monster");
    std::string lv = CCCommonUtils::getPropById(monsterId, "level");
    
    auto ret = Node::create();
    ret->setAnchorPoint({.5, .5});
    ret->setCascadeOpacityEnabled(true);
    
//    Vec2 particlePos(0, 0);
    
    const char * startFrame = "waiting_0.png";
    const char * frames = "waiting_%d.png";
    int lastFrameIdx = 11;
    if (monster == "Kulou")
    {
        if (lv == "1")
        {
            startFrame = "skeleton0_waiting_0.png";
            frames = "skeleton0_waiting_%d.png";
            lastFrameIdx = 7;
            
//            particlePos.x = -10;
//            particlePos.y = -20;
        }
        else
        {
            startFrame = "skeleton1_waiting_0.png";
            frames = "skeleton1_waiting_%d.png";
            lastFrameIdx = 7;
            
//            particlePos.x = 15;
//            particlePos.y = -20;
        }
        
//        addParticles(ret, particlePos, 30);
    }
    else if (monster == "rm")
    {
        startFrame = "hetong_waiting_0.png";
        frames = "hetong_waiting_%d.png";
        lastFrameIdx = 7;
        
//        particlePos.y = -20;
//        addParticles(ret, particlePos);
    }
    
    auto octopus = cocos2d::Sprite::createWithSpriteFrameName(startFrame);
    auto *ac1 = NBWorldNPC::createAnimation("World/World_5.plist", frames, 0, lastFrameIdx);
    octopus->runAction(ac1);
    octopus->setTag(NB_WORLD_MONSTER_ANIM_TAG);
    ret->addChild(octopus);
    
    return ret;
}

cocos2d::Node* NBWorldUtils::createSeaMonsterAndAttackAnimation(std::string monsterId)
{
    std::string monster = CCCommonUtils::getPropById(monsterId, "monster");
    std::string lv = CCCommonUtils::getPropById(monsterId, "level");
    
    auto ret = Node::create();
    ret->setAnchorPoint({.5, .5});
    ret->setCascadeOpacityEnabled(true);
    
//    Vec2 particlePos(0, 0);
    
    const char * startFrame = "attack_0.png";
    const char * frames = "attack_%d.png";
    int lastFrameIdx = 7;
    if (monster == "Kulou")
    {
        if (lv == "1")
        {
            startFrame = "skeleton0_attack_0.png";
            frames = "skeleton0_attack_%d.png";
            lastFrameIdx = 8;
            
//            particlePos.x = 0;
//            particlePos.y = -20;
        }
        else
        {
            startFrame = "skeleton1_attack_0.png";
            frames = "skeleton1_attack_%d.png";
            lastFrameIdx = 8;
            
//            particlePos.x = 10;
//            particlePos.y = -20;
        }
//        addParticles(ret, particlePos);
    }
    else if (monster == "rm")
    {
        startFrame = "hetong_attack_0.png";
        frames = "hetong_attack_%d.png";
        lastFrameIdx = 7;
        
//        particlePos.y = -20;
//        addParticles(ret, particlePos);
    }
    
    auto octopus = cocos2d::Sprite::createWithSpriteFrameName(startFrame);
    auto *ac1 = NBWorldNPC::createAnimation("World/World_5.plist", frames, 0, lastFrameIdx);
    octopus->runAction(ac1);
    octopus->setTag(NB_WORLD_MONSTER_ANIM_TAG);
    ret->addChild(octopus);
    
    return ret;
}
