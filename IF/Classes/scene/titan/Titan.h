//
//  Titan.h
//  IF
//
//  Created by tao.yu on 15/8/25.
//
//

#ifndef __IF__Titan__
#define __IF__Titan__

#include <stdio.h>
#include "NBSprite3D.h"
#include "DrawNode3D.h"
USING_NS_CC;

class sAnimationInfo : public Ref
{
public:
    std::string animationName;
    int startFrame;
    int endFrame;
    std::vector<int> keyFrames;
};

class Titan : public Node
{
public:

    
    enum class eActState
    {
        Stand = 0,
        Walk,
        Cheer,
        Idle,
    };
    
    enum class eFaceDir
    {
        None = 0,
        Front = 1,
        Back,
    };
public:
    Titan();
    virtual ~Titan();
    static Titan* create(int tid);

    NBSprite3D* getSprite(){
        return _model;
    };
    
    void update(float dt);
    
    const Vec3 getRotation3D();
    void setRotation3D(const Vec3& rotation);
    
    void updateTitanState();
    
    bool onTouched(CCTouch* pTouch);
    
    void changeTitanState(eActState state);
    
    void initMovePoint(std::vector<Vec2>& movePoint);
    
//    void updateModel();
    
    void turnFront();
    
    void turnBack();
    
    void onRotationY(float dtAngle);

    
    void reset();
    
    void resetDisplay(int tid);

    
    void onRotateX(float dtAngle);
    void onRotateY(float dtAngle);
    void onRotateZ(float dtAngle);
    void onReset();

protected:
    
    bool initWithFile(int tid);
    
    void stand();
    void walk();
    void ilde();
    void cheer();

    sAnimationInfo* getAnimationByType(eActState e);
    
protected:
    NBSprite3D* _model;
    Vec2 _position;
    Vec3 _rotation;
    std::map<std::string,NBSprite3D*> _equipmentMap;
    eActState _curState;
    eActState _lastState;
    eFaceDir _curFaceDir;
    std::map<eActState,Animate3D*> _actionCache;
    
    std::vector<Vec2> _movePointVec;
    int _curMovePosIndex;
    DrawNode3D* _drawDebug;
    OBB _obbt;
    int _tid;
};

#endif /* defined(__IF__Titan__) */
