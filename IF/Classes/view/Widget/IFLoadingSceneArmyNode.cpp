//
//  IFLoadingSceneArmyNode.cpp
//  IF
//
//  Created by 李锐奇 on 15/3/2.
//
//

#include "IFLoadingSceneArmyNode.h"

IFLoadingSceneArmyNode *IFLoadingSceneArmyNode::create(std::string atlas, std::string json, std::string aniName, float timeScale){
    IFLoadingSceneArmyNode *ret = new IFLoadingSceneArmyNode();
    if(ret && ret->init(atlas, json, aniName, timeScale)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool IFLoadingSceneArmyNode::init(std::string atlas, std::string json, std::string aniName, float timeScale)
{
    animationObj = new IFSkeletonAnimation(json.c_str(), atlas.c_str());
    animationObj->setVisibleStop(false);
    addChild(animationObj);
    animationObj->setAnimation(0, aniName.c_str(), true);
    animationObj->setTimeScale(timeScale);
    return true;
}

IFLoadingSceneArmyNode::IFLoadingSceneArmyNode()
{
}

IFLoadingSceneArmyNode::~IFLoadingSceneArmyNode()
{
    CC_SAFE_RELEASE_NULL(animationObj);
}