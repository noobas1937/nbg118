//
//  IFFieldMonsterNode.h
//  IF
//
//  Created by lihua on 15/2/2.
//
//

#ifndef __IF__IFFieldMonsterNode__
#define __IF__IFFieldMonsterNode__

#include "CommonInclude.h"
#include "IFSkeletonNode.h"

class IFFieldMonsterNode : public IFSkeletonNode
{
public:
	virtual bool init();
	CREATE_FUNC(IFFieldMonsterNode);
	IFFieldMonsterNode();
	virtual ~IFFieldMonsterNode();
    
public:
    IFSkeletonAnimation* setSpineState(string skeName, int state, int direction, bool repeat = true, CCCallFunc* callFunc = nullptr,int loopCnt = 1);
//    virtual void update(float deltaTime);
    float getloopOneTime();
    bool isSkeExit(string skeName);
    bool isSkeTextureExit();
    std::string getAtlasPath();
    std::string getJsonPath(std::string jsonName);
private:
    void animationStateEvent(int trackIndex, int loopCount);
    
private:
    string m_skeName;
    string m_aniName;
    int m_state;
    int m_loopCnt;
    spTrackEntry* m_entry;
};


#endif /* defined(__IF__IFFieldMonsterNode__) */