//
//  IFDailyActiveNode.cpp
//  IF
//
//  Created by chenfubi on 15/5/8.
//
//

#include "IFDailyActiveNode.h"
#include "WorldMapView.h"

bool IFDailyActiveNode::init()
{
	animationObj = NULL;
	return true;
}

IFDailyActiveNode::IFDailyActiveNode()
:m_skeName("")
,m_aniName("")
,m_entry(nullptr)
{
}

IFSkeletonAnimation* IFDailyActiveNode::setSpineState(string skeName, int state, bool repeat/*=true*/,CCCallFunc* callFunc/*=nullptr*/,int loopCnt /*= 1*/)
{
    m_skeName = skeName;
    m_state = state;
    m_loopCnt = loopCnt;
    
    m_aniName = "animation";
    
    if(!animationObj)
    {
        char strJsonFileName[256] = {0};
        char strAtlasFilename[256] = {0};
        sprintf(strJsonFileName, "Spine/Imperial/DailyactiveViewCell.json");
        sprintf(strAtlasFilename, "Imperial/Imperial_30.atlas");
        
        if (!CCFileUtils::sharedFileUtils()->isFileExist(strJsonFileName) ||
            !CCFileUtils::sharedFileUtils()->isFileExist(strAtlasFilename))
        {
            return nullptr;
        }
        
        animationObj = new IFSkeletonAnimation(strJsonFileName, strAtlasFilename);
        addChild(animationObj);
    }
    else
    {
        animationObj->setToSetupPose();
    }
    
    m_entry = animationObj->setAnimation(0,m_aniName.c_str(), repeat);
    if(m_entry){
        animationObj->setTimeScale(m_entry->endTime/1.5f);
        if(callFunc)
        {
            CC_SAFE_RELEASE_NULL(m_callBack);
            m_callBack = callFunc;
            CC_SAFE_RETAIN(m_callBack);
            animationObj->setCompleteListener( [&] (int trackIndex, int loopCount)
            {
                CC_SAFE_RETAIN(this);
                animationStateEvent(trackIndex,loopCount);
                CC_SAFE_RELEASE(this);
            });
        }
    }
    
    return animationObj;
}

float IFDailyActiveNode::getloopOneTime()
{
    if(m_entry)
    {
        return m_entry->endTime;
    }
    return 1.5f;
}

void IFDailyActiveNode::animationStateEvent(int trackIndex, int loopCount)
{
    spTrackEntry* entry = spAnimationState_getCurrent(animationObj->getState(), trackIndex);
    if(m_callBack && entry)
    {
        if(loopCount == m_loopCnt)
        {
            m_callBack->execute();
        }
    }
}

IFDailyActiveNode::~IFDailyActiveNode()
{
	CC_SAFE_RELEASE_NULL(animationObj);
}