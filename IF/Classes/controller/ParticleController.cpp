//
//  ParticleController.cpp
//  IF
//
//  Created by 吴 凌江 on 13-8-30.
//
//

#include "ParticleController.h"
#include "CCLoadSprite.h"
static ParticleDataManager* s_sharedParticleDataManager = NULL;
ParticleDataManager* ParticleDataManager::shared()
{
    if (!s_sharedParticleDataManager)
    {
        s_sharedParticleDataManager = new ParticleDataManager();
    }
    return s_sharedParticleDataManager;
}

void ParticleDataManager::purge()
{
    CC_SAFE_RELEASE_NULL(s_sharedParticleDataManager);
}

ParticleDataManager::ParticleDataManager()
{
    
}

ParticleDataManager::~ParticleDataManager()
{
    m_DataCache.clear();
}

ValueMap ParticleDataManager::getParticleValue(const string& name)
{
    char path[64];
    sprintf(path, "particle/%s.plist",name.c_str());
    ParticleDataMap::iterator it = m_DataCache.find(path);
    if (it != m_DataCache.end()) {
        return it->second;
    }
    auto m_sPlistFile = CCFileUtils::sharedFileUtils()->fullPathForFilename(path);
    ValueMap dict2 = FileUtils::getInstance()->getValueMapFromFile(m_sPlistFile.c_str());
    m_DataCache[path] = dict2;
    return dict2;
}

CCParticleSystemQuad* ParticleController::createFightingParticle(const char *particleName,float duration,CCPoint sourcePoint,bool autoRemove) {
    // init particle file path
    char path[64];
    sprintf(path, "particle/%s.plist",particleName);
    CCParticleSystemQuad *pRet = CCParticleSystemQuad::create(path);
    if(pRet) {
        pRet->setDuration(duration);
        pRet->setPosition(sourcePoint);
        pRet->setAutoRemoveOnFinish(autoRemove);
        return pRet;
    } else {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
};





class SelfParticle : public CCParticleSystemQuad {
public:
    void initTextureBySpriteFrame(CCSpriteFrame* frame);
    void setInit();
};

void SelfParticle::initTextureBySpriteFrame(CCSpriteFrame* frame) {
    
    _opacityModifyRGB = false;
    
    auto texture = frame->getTexture();
    auto rect = frame->getRect();
    
    this->setTextureWithRect(texture, rect);
}

void SelfParticle::setInit() {
    m_initTextureByPlist = false;
}


#define PARTICLE_PATH "World/Particle.plist"
//#define PARTICLE_PATH_1 "World/Particle_1.plist"
#define EXAMPLE_PARTICLE "Effect_0000.png"
//#define EXAMPLE_PARTICLE_1 "Effect_0249.png"

static bool _particle_init = false;
void ParticleController::initParticle() {
    if (_particle_init) {
        return;
    }
    cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PARTICLE_PATH);
//    cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PARTICLE_PATH_1);

    _particle_init = true;
}
CCParticleSystemQuad* ParticleController::createParticleForLua(const char* name){
    if (!_particle_init) {
        return nullptr;
    }
    std::string path = name;
    path += ".plist";
    auto pRet = new SelfParticle();
    
    if(pRet) {
        CCDictionary *dict = NULL;
        auto m_sPlistFile = CCFileUtils::sharedFileUtils()->fullPathForFilename(path.c_str());
        dict = CCDictionary::createWithContentsOfFileThreadSafe(m_sPlistFile.c_str());
        if(!dict){
            CC_SAFE_DELETE(pRet);
            return NULL;
        }
        
        ValueMap dict2 = FileUtils::getInstance()->getValueMapFromFile(path.c_str());
        auto textureName = dict->valueForKey("textureFileName")->getCString();
        auto spriteFrame = CCLoadSprite::loadResource(textureName);
        
        pRet->setInit();
        pRet->initWithDictionary(dict2, "");
        pRet->setPositionType(kCCPositionTypeRelative);
        pRet->initTextureBySpriteFrame(spriteFrame);
        
        dict->release();
        
        pRet->autorelease();
        
        return pRet;
    } else {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

CCParticleSystemQuad* ParticleController::createParticle(const string& name,CCPoint sourcePosition,int maxParticles, CCDictionary *rdict) {
    if (!_particle_init) {
        return nullptr;
    }
    char path[64];
    sprintf(path, "particle/%s.plist",name.c_str());
    
    
    auto pRet = new SelfParticle();
    
    if(pRet) {
        CCDictionary *dict = NULL;
 
//        if (rdict) {
//            dict = rdict;
//            dict->retain();
//        }
//        else {
            auto m_sPlistFile = CCFileUtils::sharedFileUtils()->fullPathForFilename(path);
        ValueMap dict2 = ParticleDataManager::shared()->getParticleValue(name);
        if (maxParticles>0) {
            dict2["maxParticles"] = maxParticles;
        }
        auto textureName = dict2["textureFileName"].asString();
        
        auto spriteFrame = CCLoadSprite::loadResource(textureName.c_str());
        
        pRet->setInit();
        pRet->initWithDictionary(dict2, "");
        pRet->setPositionType(kCCPositionTypeRelative);
        pRet->initTextureBySpriteFrame(spriteFrame);
        
//        dict->release();
        
        pRet->setPosition(sourcePosition);
        
        pRet->autorelease();
        return pRet;
    } else {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}



CCParticleBatchNode* ParticleController::createParticleBatch(CCParticleSystemQuad *particle) {
    if (!_particle_init) {
        return nullptr;
    }
//    std::string picName = "";
//    if(particle == NULL){
//        picName = EXAMPLE_PARTICLE;
//    }else{
//        GLuint name1 = particle->getTexture()->getName();
//        GLuint name2 = CCLoadSprite::loadResource(EXAMPLE_PARTICLE)->getTexture()->getName();
//        if(name1 == name2){
//            picName = EXAMPLE_PARTICLE;
//        }else{
//            picName = EXAMPLE_PARTICLE_1;
//        }
//    }
    auto ret = CCParticleBatchNode::createWithTexture(CCLoadSprite::loadResource(EXAMPLE_PARTICLE)->getTexture());
    if (ret) {
        return ret;
    } else {
        return nullptr;
    }
}
