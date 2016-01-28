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
#include "ParticleController.h"

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

void addParticles(Node* parent, const Vec2& particlePos)
{
    const char * particleFiles[] = {"WorldRipple_1", "WorldRipple_2", "WorldRipple_3"};
    
    for (int i = 0; i < 3; ++i)
    {
        auto particle = ParticleController::createParticle(particleFiles[i]);
        particle->setPosition(particlePos);
        parent->addChild(particle);
    }
}

cocos2d::Node* NBWorldUtils::createSeaMonsterAndWaitingAnimation(std::string monsterId)
{
    std::string monster = CCCommonUtils::getPropById(monsterId, "monster");
    std::string lv = CCCommonUtils::getPropById(monsterId, "level");
    
    Vec2 particlePos(0, 0);
    
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
        }
        else
        {
            startFrame = "skeleton1_waiting_0.png";
            frames = "skeleton1_waiting_%d.png";
            lastFrameIdx = 7;
        }
        
        particlePos.x = 10;
        particlePos.y = -20;
    }
    else if (monster == "rm")
    {
        startFrame = "hetong_waiting_0.png";
        frames = "hetong_waiting_%d.png";
        lastFrameIdx = 7;
        
        particlePos.y = -20;
    }
    
    auto octopus = cocos2d::Sprite::createWithSpriteFrameName(startFrame);
    auto *ac1 = NBWorldNPC::createAnimation("World/World_5.plist", frames, 0, lastFrameIdx);
    octopus->runAction(ac1);
    octopus->setTag(NB_WORLD_MONSTER_ANIM_TAG);
    
    auto ret = Node::create();
    ret->setAnchorPoint({.5, .5});
    ret->setCascadeOpacityEnabled(true);
    addParticles(ret, particlePos);
    ret->addChild(octopus);
    
    return ret;
}

cocos2d::Node* NBWorldUtils::createSeaMonsterAndAttackAnimation(std::string monsterId)
{
    std::string monster = CCCommonUtils::getPropById(monsterId, "monster");
    std::string lv = CCCommonUtils::getPropById(monsterId, "level");
    
    Vec2 particlePos(0, 0);
    
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
        }
        else
        {
            startFrame = "skeleton1_attack_0.png";
            frames = "skeleton1_attack_%d.png";
            lastFrameIdx = 8;
        }
        
        particlePos.x = 10;
        particlePos.y = -20;
    }
    else if (monster == "rm")
    {
        startFrame = "hetong_attack_0.png";
        frames = "hetong_attack_%d.png";
        lastFrameIdx = 7;
        
        particlePos.y = -20;
    }
    
    auto octopus = cocos2d::Sprite::createWithSpriteFrameName(startFrame);
    auto *ac1 = NBWorldNPC::createAnimation("World/World_5.plist", frames, 0, lastFrameIdx);
    octopus->runAction(ac1);
    octopus->setTag(NB_WORLD_MONSTER_ANIM_TAG);
    
    auto ret = Node::create();
    ret->setAnchorPoint({.5, .5});
    ret->setCascadeOpacityEnabled(true);
    addParticles(ret, particlePos);
    ret->addChild(octopus);
    
    return ret;
}
