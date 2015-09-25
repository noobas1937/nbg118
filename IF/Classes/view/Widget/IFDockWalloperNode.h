//
//  IFDockWalloperNode.h
//  IF
//
//  Created by xxrdsg on 15-4-13.
//
//

#ifndef __IF__IFDockWalloperNode__
#define __IF__IFDockWalloperNode__

#include "CommonInclude.h"
#include "IFSkeletonNode.h"

typedef enum DockWalloperState
{
    leftAwayShip,
    upAwayShip,
    unLoad,
    downTowardsShip,
    rightTowardsShip
};

class IFDockWalloperNode : public IFSkeletonNode
{
public:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    static IFDockWalloperNode *create();
    IFDockWalloperNode();
    virtual ~IFDockWalloperNode();
    IFSkeletonAnimation* setDockWalloperState(int);
    void setDockWalloperState(CCObject*);
    void startLoop(CCObject*);
    
    int m_state;
    CCPoint p1;
    CCPoint p2;
    CCPoint p3;

    bool m_inLoop;
};

#endif /* defined(__IF__IFDockWalloperNode__) */
