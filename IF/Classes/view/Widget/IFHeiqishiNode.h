//
//  IFHeiqishiNode.h
//  IF
//
//  Created by wangdianzhen on 15/3/18.
//
//

#ifndef __IF__IFHeiqishiNode__
#define __IF__IFHeiqishiNode__

#include "CommonInclude.h"
#include "IFSkeletonNode.h"
#include "WorldController.h"
class IFHeiqishiNode : public IFSkeletonNode
{
public:
    virtual bool init();
    static IFHeiqishiNode* create(MarchInfo& marchInfo);
    virtual ~IFHeiqishiNode();
    IFHeiqishiNode(MarchInfo& marchInfo);
    virtual void setScaleX(float fScaleX);
    virtual void setScale(float scale);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
public:
    IFSkeletonAnimation* setSpineState(string skeName, int state, int direction, bool repeat = true, CCCallFunc* callFunc = nullptr,int loopCnt = 1);
    float getloopOneTime();
    bool isSkeExit(string skeName);
    bool isSkeTextureExit();
    void onExit();
    std::string getAtlasPath();
    std::string getJsonPath(std::string jsonName);

    static int getExchangeDir(int direction);
    int getState();
    string getSkeName();
    int getDirection();
    CCSize getSpinSize();
    void onStartCityAttack();
    void onStopCityAttack();
    
private:
    void animationStateEvent(int trackIndex, int loopCount);
    void cityAttack(float _time);
    void playAttackAnimation();
    
private:
    MarchInfo& m_info;
    string m_skeName;
    string m_aniName;
    int m_state;
    int m_loopCnt;
    spTrackEntry* m_entry;
    int m_direction;
    CCSafeObject<CCSprite> m_touchSp;
 
};
#endif /* defined(__IF__IFHeiqishiNode__) */
