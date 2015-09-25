//
//  ParticleController.h
//  IF
//
//  Created by 吴 凌江 on 13-8-30.
//
//

#ifndef __IF__ParticleController__
#define __IF__ParticleController__

#include "CommonInclude.h"

class ParticleController 
{
public:
    static CCParticleSystemQuad* createFightingParticle(const char *particleName,float duration = 1.0,CCPoint sourcePosition = CCPointZero,bool autoRemove = true);
    

    static CCParticleSystemQuad* createParticleForLua(const char *name);
    static CCParticleSystemQuad* createParticle(const string& name,CCPoint sourcePosition = CCPointZero,int maxParticles = 0, CCDictionary *rdict=NULL);
    static CCParticleBatchNode* createParticleBatch(CCParticleSystemQuad *particle = NULL);
    static void initParticle();
    
};

typedef std::map<string,ValueMap> ParticleDataMap;


class ParticleDataManager : public CCObject
{
public:
    ValueMap getParticleValue(const string& name);
    
private:
    ParticleDataMap m_DataCache;
public:
    static ParticleDataManager * shared();
    void purge();
    
private:
    ParticleDataManager();
    virtual ~ParticleDataManager();

};

#endif /* defined(__IF__ParticleController__) */
