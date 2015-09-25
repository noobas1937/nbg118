//
//  IFSkeletonAnimation.cpp
//  IF
//
//  Created by lihua on 15/2/2.
//
//

#include "IFSkeletonAnimation.h"
#include "IFSkeletonDataManager.h"
#include "WorldMapView.h"

IFSkeletonAnimation::IFSkeletonAnimation (const char* skeletonDataFile, const char* atlasFile, float scale)
//: SkeletonAnimation( skeletonDataFile, atlasFile, scale) {
:SkeletonAnimation(GET_SKELETON_DATA2( skeletonDataFile, atlasFile, scale)){
    m_visibleStop = true;
}

void IFSkeletonAnimation::update(float deltaTime)
{
    if(m_visibleStop && getNodeVisible(this) == false)
    {
        return;
    }
    
    this->retain();
    SkeletonAnimation::update(deltaTime);
    this->release();
}

bool IFSkeletonAnimation::getNodeVisible(CCNode* node)
{return true;
    if(node)
    {
        if(dynamic_cast<WorldMapView*>(node) && WorldController::getInstance()->isInWorld)
        {
            return true;
        }
        if(node->isVisible())
        {
            return getNodeVisible(node->getParent());
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

IFSkeletonAnimation::~IFSkeletonAnimation()
{
	// TODO: auto generated code
}

