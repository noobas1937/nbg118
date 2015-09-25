//
//  IFLoadingSceneArmyNode.h
//  IF
//
//  Created by 李锐奇 on 15/3/2.
//
//

#ifndef __IF__IFLoadingSceneArmyNode__
#define __IF__IFLoadingSceneArmyNode__

#include "CommonInclude.h"
#include "IFSkeletonNode.h"

class IFLoadingSceneArmyNode : public IFSkeletonNode
{
public:
    virtual bool init(std::string atlas, std::string json, std::string aniName, float timeScale = 1.0);
    static IFLoadingSceneArmyNode *create(std::string atlas, std::string json, std::string aniName, float timeScale = 1.0);
    IFLoadingSceneArmyNode();
    virtual ~IFLoadingSceneArmyNode();
};

#endif /* defined(__IF__IFLoadingSceneArmyNode__) */
