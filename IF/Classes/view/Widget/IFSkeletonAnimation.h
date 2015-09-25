//
//  IFSkeletonAnimation.h
//  IF
//
//  Created by lihua on 15/2/2.
//
//

#ifndef __IF__IFSkeletonAnimation__
#define __IF__IFSkeletonAnimation__

#include "CommonInclude.h"
using namespace spine;

class IFSkeletonAnimation : public SkeletonAnimation
{
public:
	IFSkeletonAnimation (const char* skeletonDataFile, const char* atlasFile, float scale = 1);
	virtual ~IFSkeletonAnimation();
    
    virtual void update(float deltaTime);
    
private:
    bool getNodeVisible(CCNode* node);
    
    CC_SYNTHESIZE(bool, m_visibleStop, VisibleStop);

};


#endif /* defined(__IF__IFSkeletonAnimation__) */
