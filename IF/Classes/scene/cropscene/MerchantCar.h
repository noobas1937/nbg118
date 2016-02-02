//
//  MerchantCar.h
//  DragonClans
//
//  Created by fusheng on 16/2/2.
//
//

#ifndef MerchantCar_h
#define MerchantCar_h



#include <stdio.h>
#include "cocos2d.h"
#include "CCLoadSprite.h"
#include "SpriteSheetAni.h"
#include "CCMathUtils.h"
#include "NBSprite3D.h"

USING_NS_CC;
using namespace std;

enum MerchantCarStatus
{
    MERCHANTCARSTATUSNONE,
    MERCHANTCARSTATUSSTAND,
    MERCHANTCARSTATUSWALK
    
};


class MerchantCar : public cocos2d::Node
{
public:
   
    virtual bool init();
    
//    virtual void onEnter();
//
//    virtual void onExit();
    // implement the "static create()" method manually
    CREATE_FUNC(MerchantCar);
    
    static MerchantCar* getInstance();
    
    void playStand();
    
    void playWalk();
    
private:
    MerchantCar();
    
    MerchantCarStatus m_actionStatus;
    
    NBSprite3D* _model;
    
    CCNode* rotateRootNode;
    
};

#endif /* MerchantCar_h */
