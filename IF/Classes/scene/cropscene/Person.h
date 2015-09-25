//
//  Person.h
//  IF
//
//  Created by fubin on 14-5-5.
//
//

#ifndef IF_Person_h
#define IF_Person_h

#include "CommonInclude.h"
#include "CCLabelIF.h"
#include "ParticleController.h"

class Person: public CCNode
{
public:
    Person():m_ptArray(NULL),m_batchNode(NULL),m_type(-1),s_type(-1),canTalk(true),p_type(-1){};
    ~Person(){};
    bool initPerson(int x, int y, CCSpriteBatchNode* batchNode, int zOrder, CCPointArray* ptArray, float duration,int pType);
    static Person* create(int x, int y, CCSpriteBatchNode* batchNode, int zOrder, CCPointArray* ptArray, float duration,int pType=-1);
    
    virtual void onEnter();
    virtual void onExit();
    void onPersonDelete();
    void onSetType(int type);
    
    CCSafeObject<CCSprite> m_per1Spr;
protected:
    void changePersonState(CCObject *obj);
    void testSend(float dt);
    void openTalk(CCObject *obj);
    void stopSendTalk(CCObject *obj);
    
    CCPointArray* m_ptArray;
    CCSpriteBatchNode* m_batchNode;
    
    CCSafeObject<CCSprite> m_per2Spr;
    int m_type;
    int s_type;
    int p_type;
    bool canTalk;
private:
    
};

#endif
