//
//  IFSkeletonNode.cpp
//  IF
//
//  Created by lihua on 15/2/2.
//
//

#include "IFSkeletonNode.h"
#include "WorldMapView.h"
#include "IFSkeletonBatchLayer.h"

bool IFSkeletonNode::init()
{
	bool bRet = false;
	do
	{
		// TODO: auto generated code
		bRet = true;
	}while(0);
	return bRet;
}

IFSkeletonNode::IFSkeletonNode():
animationObj(nullptr),
m_callBack(nullptr)
{
	
}

void IFSkeletonNode::cleanup()
{
    CC_SAFE_RELEASE_NULL(m_callBack);
    CCNode::cleanup();
}

IFSkeletonNode::~IFSkeletonNode()
{
	CC_SAFE_RELEASE_NULL(m_callBack);
}

