//
//  IFDailyActiveNode.h
//  IF
//
//  Created by chenfubi on 15/5/8.
//
//

#ifndef __IF__IFDailyActiveNode__
#define __IF__IFDailyActiveNode__

#include "CommonInclude.h"
#include "IFSkeletonNode.h"

class IFDailyActiveNode : public IFSkeletonNode
{
public:
	virtual bool init();
	CREATE_FUNC(IFDailyActiveNode);
	IFDailyActiveNode();
	virtual ~IFDailyActiveNode();
    
public:
    IFSkeletonAnimation* setSpineState(string skeName, int state, bool repeat = true, CCCallFunc* callFunc = nullptr,int loopCnt = 1);
//    virtual void update(float deltaTime);
    float getloopOneTime();
    
private:
    void animationStateEvent(int trackIndex, int loopCount);
    
private:
    string m_skeName;
    string m_aniName;
    int m_state;
    int m_loopCnt;
    spTrackEntry* m_entry;
};


#endif /* defined(__IF__IFDailyActiveNode__) */