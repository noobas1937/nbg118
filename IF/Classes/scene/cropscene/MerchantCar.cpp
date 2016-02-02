//
//  MerchantCar.cpp
//  DragonClans
//
//  Created by fusheng on 16/2/2.
//
//

#include "MerchantCar.h"

using namespace cocos2d;
MerchantCar::MerchantCar(){}

//void MerchantCar::onEnter()
//{
//    Node::onEnter();
//}
//
//void MerchantCar::onExit()
//{
//    Node::onExit();
//}

MerchantCar* MerchantCar::getInstance()
{
    static MerchantCar* mc = MerchantCar::create();
    
    if (mc) {
        return mc;
    }
    
    return nullptr;
}

bool MerchantCar::init()
{
	if (!Node::init())
	{
		return false;
	}
	/* code */
    _model = NBSprite3D::create("3d/merchant/lvxingshangren_SKIN.c3b");
    
    if (!_model) {
        CCLog("fuck 3d create error");
        return false;
    }
    rotateRootNode = Node::create();
    
    rotateRootNode->setRotation3D(Vec3(38, 39, -24));
    

    rotateRootNode->setPositionZ(200);
    
    _model->setTexture("3d/merchant/merchant_texture.jpg");
    
    _model->setScale(8);
    
    rotateRootNode->addChild(_model);
    
//        _model->setGlobalZOrder(10000);
    
//    _model->setPositionZ(200);
    
    this->addChild(rotateRootNode);
    
//    playWalk();

	return true;
}


void MerchantCar::playWalk()
{
    if(_model)
    {

        auto anim_walk = Animation3D::create("3d/merchant/lvxingshangren_walk.c3b");

        if (anim_walk)
        {
            auto pAnim = Animate3D::create(anim_walk);
            if (pAnim)
            {
                auto act = RepeatForever::create(pAnim);
                _model->stopAllActions();
                _model->runAction(act);
                m_actionStatus = MerchantCarStatus::MERCHANTCARSTATUSWALK;
            }
        }
    }

}

void MerchantCar::playStand()
{
    if(_model)
    {
        
        auto anim_stand = Animation3D::create("3d/merchant/lvxingshangren_stand.c3b");
        
        if (anim_stand)
        {
            auto pAnim = Animate3D::create(anim_stand);
            if (pAnim)
            {
                auto act = RepeatForever::create(pAnim);
                _model->stopAllActions();
                _model->runAction(act);
                m_actionStatus = MerchantCarStatus::MERCHANTCARSTATUSSTAND;
            }
        }
    }
    
}