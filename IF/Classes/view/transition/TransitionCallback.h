//
//  TransitionCallback.h
//  IF
//
//  Created by 邹 程 on 13-10-24.
//
//

#ifndef __IF__TransitionCallback__
#define __IF__TransitionCallback__

#include "CommonInclude.h"
#include "CCAniNode.h"
#include "WorldController.h"

class SceneTransition :
public CCAniNode
{
public:
    static SceneTransition *createTransition(int sceneId,bool shouldSave,bool shouldCleanUp, bool playTransition = false,int subSceneId=-1, MapType mapType = DEFAULT_MAP);
    static long getCurrentTime();
    void start();
    virtual void update(float time);
private:
    
    bool init(int sceneId,bool shouldSave,bool shouldCleanUp, bool playTransition,int subSceneId, MapType mapType = DEFAULT_MAP);
    virtual void onEnter();
    virtual void onExit();
    void onTransitionMiddleFinish();
    void doWhenDownloadCompelete();
    void playTransitionEnd(float t);
    void handleFinishMsg(CCObject* p);
    void doFinish();
    void forceTransitionFinish();
    
    CC_SYNTHESIZE_READONLY(bool, m_isSendedFinishMsg, IsSendedFinishMsg);
    CC_SYNTHESIZE_READONLY(bool, m_isTransitionMiddleFinished, IsTransitionMiddleFinished);
    
    int m_sceneId;
    bool m_shouldSave;
    bool m_shouldCleanUp;
    bool m_playTransition;
    int m_subSceneId;
    bool m_waitForResource;
    MapType m_mapType;
};


#endif /* defined(__IF__TransitionCallback__) */
