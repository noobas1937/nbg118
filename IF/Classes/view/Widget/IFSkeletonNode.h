//
//  IFSkeletonNode.h
//  IF
//
//  Created by lihua on 15/2/2.
//
//

#ifndef __IF__IFSkeletonNode__
#define __IF__IFSkeletonNode__

#include "IFSkeletonAnimation.h"
#include "CommonInclude.h"


class IFSkeletonNode : public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(IFSkeletonNode);
	IFSkeletonNode();
	virtual ~IFSkeletonNode();
    virtual void cleanup();
    IFSkeletonAnimation* animationObj;
protected:
    CCCallFunc* m_callBack;
};


#endif /* defined(__IF__IFSkeletonNode__) */
