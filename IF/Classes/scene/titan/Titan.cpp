//
//  Titan.cpp
//  IF
//
//  Created by tao.yu on 15/8/25.
//
//

#include "Titan.h"
#include "Particle3D/CCParticleSystem3D.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"
#include "DrawNode3D.h"
#include "CCCommonUtils.h"

const float sfChangeActionTime = 3.f;
const float sfMoveSpeed = 2;
const float sfRunSpeed = 6;
Titan::Titan()
:_curMovePosIndex(0)
,_tid(0)
{
    _curFaceDir = eFaceDir::None;
}

Titan::~Titan()
{
    
}


Titan* Titan::create(int tid)
{
    auto titan = new (std::nothrow) Titan();
    if (titan && titan->initWithFile(tid))
    {
        titan->autorelease();
        return titan;
    }
    CC_SAFE_DELETE(titan);
    return nullptr;
}

struct sModelPram{
    std::string modelName;
    std::string textureName;
    std::string textureShaderName;
    std::string vShaderFileName;
    std::string fShaderFileName;
    Vec3 color;
    float speed;
    float min;
    float max;
};

bool Titan::initWithFile(int tid)
{
    _tid = tid;
    int nModelScale = atoi(CCCommonUtils::getPropById(CC_ITOA(_tid), "scale").c_str());
    // 设置当前node的contentSize 用于点击事件
    setContentSize(Size(500,550));
    setAnchorPoint(Vec2(0.5,0));
    
    sModelPram bodyParam;
    bodyParam.modelName = "3d/" + CCCommonUtils::getPropById(CC_ITOA(tid), "model") + ".c3b";
    bodyParam.textureName = "3d/" + CCCommonUtils::getPropById(CC_ITOA(tid), "texture") + ".jpg";
//    bodyParam.textureShaderName = "3d/titan_1_shader.jpg";
//    bodyParam.vShaderFileName = "shaders/titan.vsh";
//    bodyParam.fShaderFileName = "shaders/titan.fsh";
//    bodyParam.color = Vec3(68, 208, 255);
//    bodyParam.speed = 2;
//    bodyParam.min = 0.4;
//    bodyParam.max = 3;

    _model = NBSprite3D::create(bodyParam.modelName);
    
    if (!_model) {
        return false;
    }
    addChild(_model);
    _model->setTexture(bodyParam.textureName);
    // shader
//    _model->setEmittingShaderDuration(bodyParam.speed, bodyParam.min, bodyParam.max);
//    _model->applyEmittingShader(bodyParam.color,bodyParam.textureShaderName);
    _model->setScale(nModelScale);
//    _model->setPosition3D(Vec3(0,0,0));
//    _model->setRotation3D(Vec3(0,0,0));
//    _drawDebug = DrawNode3D::create();
//    _model->addChild(_drawDebug);
//    _obbt = OBB(_model->getAABB());
    
    reset();
    if (true) {
        return true;
    }
    
    // left shoulder
    sModelPram shoulderLeftParam;
    shoulderLeftParam.modelName = "3d/shoulder_l_1.obj";
    shoulderLeftParam.textureName = "3d/shoulder_1.jpg";
    shoulderLeftParam.textureShaderName = "3d/shoulder_1_shader.jpg";
    shoulderLeftParam.vShaderFileName = "shaders/titan_part.vsh";
    shoulderLeftParam.fShaderFileName = "shaders/titan.fsh";
    shoulderLeftParam.color = Vec3(68, 208, 255);
    shoulderLeftParam.speed = 2.8;
    shoulderLeftParam.min = 3;
    shoulderLeftParam.max = 7;
    
    auto scapulaLeft = NBSprite3D::create(shoulderLeftParam.modelName);
    scapulaLeft->setTexture(shoulderLeftParam.textureName);
    // shader
    scapulaLeft->setEmittingShaderDuration(shoulderLeftParam.speed, shoulderLeftParam.min, shoulderLeftParam.max);
    scapulaLeft->applyEmittingShader(shoulderLeftParam.color,
                                shoulderLeftParam.textureShaderName,
                                shoulderLeftParam.vShaderFileName);
    
//    scapulaLeft->setRotation3D(Vec3(90,0,0));
    auto sNodeL = _model->getAttachNode("shoulder_l");
    sNodeL->addChild(scapulaLeft);
    _equipmentMap["shoulder_l"] = scapulaLeft;
    

    // right shoulder ---------------------
    sModelPram shoulderRightParam;
    shoulderRightParam.modelName = "3d/shoulder_r_1.obj";
    shoulderRightParam.textureName = "3d/shoulder_1.jpg";
    shoulderRightParam.textureShaderName = "3d/shoulder_1_shader.jpg";
    shoulderRightParam.vShaderFileName = "shaders/titan_part.vsh";
    shoulderRightParam.fShaderFileName = "shaders/titan.fsh";
    shoulderRightParam.color = Vec3(68, 208, 255);
    shoulderRightParam.speed = 2.8;
    shoulderRightParam.min = 3;
    shoulderRightParam.max = 7;
    
    auto scapulaRight = NBSprite3D::create(shoulderRightParam.modelName);
    scapulaRight->setTexture(shoulderRightParam.textureName);
    // shader
    scapulaRight->setEmittingShaderDuration(shoulderRightParam.speed, shoulderRightParam.min, shoulderRightParam.max);
    scapulaRight->applyEmittingShader(shoulderRightParam.color,
                                     shoulderRightParam.textureShaderName,
                                     shoulderRightParam.vShaderFileName);
    // attach
    auto sNodeR = _model->getAttachNode("shoulder_r");
    sNodeR->addChild(scapulaRight);
    _equipmentMap["shoulder_r"] = scapulaRight;
    
    
    // rune ---------------------
    sModelPram runeParam;
    runeParam.modelName = "3d/rune_1.obj";
    runeParam.textureName = "3d/rune_1.jpg";
    runeParam.textureShaderName = "3d/rune_1_shader.jpg";
    runeParam.vShaderFileName = "shaders/titan_part.vsh";
    runeParam.fShaderFileName = "shaders/titan.fsh";
    runeParam.color = Vec3(68, 208, 255);
    runeParam.speed = 2;
    runeParam.min = 0.4;
    runeParam.max = 3;
    
    auto rune = NBSprite3D::create(runeParam.modelName);
    rune->setTexture(runeParam.textureName);
    // shader
    rune->setEmittingShaderDuration(runeParam.speed, runeParam.min, runeParam.max);
    rune->applyEmittingShader(runeParam.color,runeParam.textureShaderName,runeParam.vShaderFileName);
    
    rune->setRotation3D(Vec3(0,-90,90));
    // attach
    auto runeNode = _model->getAttachNode("rune");
    runeNode->addChild(rune);
    _equipmentMap["rune"] = rune;
    
    // particle
//    auto rootps = PUParticleSystem3D::create("3d/particle3d/titan_body.pu", "3d/particle3d/stone.material");
//    rootps->startParticleSystem();
//    rootps->setScale(10);
//    auto handLeftNode = _model->getAttachNode("weapon_r");
//    handLeftNode->addChild(rootps);
//
//    auto rootps2 = PUParticleSystem3D::create("3d/Particle3D/scripts/mp_torch.pu", "3d/Particle3D/materials/pu_mediapack_01.material");
//    rootps2->startParticleSystem();
//    auto handrightNode = _model->getAttachNode("hand_r");
//    handrightNode->addChild(rootps2);
    
    
//    changeTitanState(eActState::Stand);

//    Director::getInstance()->getScheduler()->schedule(schedule_selector(Titan::update), this, 0, false);
    return true;

}

void Titan::resetDisplay(int tid)
{
    _tid = tid;
    int nModelScale = atoi(CCCommonUtils::getPropById(CC_ITOA(_tid), "scale").c_str());
    // 设置当前node的contentSize 用于点击事件
    setContentSize(Size(500*nModelScale,550*nModelScale));
    setAnchorPoint(Vec2(0.5,0));
    
    sModelPram bodyParam;
    bodyParam.modelName = "3d/" + CCCommonUtils::getPropById(CC_ITOA(tid), "model") + ".c3b";
    bodyParam.textureName = "3d/" + CCCommonUtils::getPropById(CC_ITOA(tid), "texture") + ".jpg";

    if (_model) {
        _model->stopAllActions();
        _model->removeFromParent();
    }
    
    _model = NBSprite3D::create(bodyParam.modelName);
    

    addChild(_model);
    _model->setTexture(bodyParam.textureName);

    _model->setScale(nModelScale);

    
    reset();
}

void Titan::update(float dt)
{
    if (!_model) {
        return;
    }
    if (!_model->getActionByTag(1)) {
        changeTitanState(_lastState);
    }
    
//    if (_curState == eActState::Walk || _curState == eActState::Run) {
//        float speed = sfMoveSpeed;
//        if (_curState == eActState::Run) {
//            speed = sfRunSpeed;
//        }
//        if (_curMovePosIndex < 3) {
//            turnFront();
//            // 从王座走到门口
//            float bx = _movePointVec[_curMovePosIndex].x;
//            float by = _movePointVec[_curMovePosIndex].y;
//            
//            float ex = _movePointVec[_curMovePosIndex+1].x;
//            float ey = _movePointVec[_curMovePosIndex+1].y;
//            
//            float dx = ex - bx;
//            float dy = ey - by;
//            
//            float nx = getPositionX() + speed;
//            float ny = getPositionY() + speed * dy / dx;
//            if (nx > ex || ny < ey) {
//                nx = ex;
//                ny = ey;
//                _curMovePosIndex++;
//            }
//            setPosition(nx,ny);
//        }
//        else if (_curMovePosIndex >= 3 && _curMovePosIndex < 6)
//        {
//            turnBack();
//            float bx = _movePointVec[6 - _curMovePosIndex].x;
//            float by = _movePointVec[6 - _curMovePosIndex].y;
//            
//            float ex = _movePointVec[6 - _curMovePosIndex - 1].x;
//            float ey = _movePointVec[6 - _curMovePosIndex - 1].y;
//            
//            float dx = ex - bx;
//            float dy = ey - by;
//            
//            float nx = getPositionX() - speed;
//            float ny = getPositionY() - speed * dy / dx;
//            
//            if (nx < ex || ny > ey) {
//                nx = ex;
//                ny = ey;
//                _curMovePosIndex++;
//            }
//            setPosition(nx,ny);
//        }
//        if (_curMovePosIndex >= 6) {
//            _curMovePosIndex = 0;
//            changeTitanState(eActState::Idle);
//            turnFront();
//        }
//    }
    
//    if (_drawDebug)
//    {
//        _drawDebug->clear();
//        Mat4 mat = _model->getNodeToWorldTransform();
//        mat.getRightVector(&_obbt._xAxis);
//        _obbt._xAxis.normalize();
//        
//        mat.getUpVector(&_obbt._yAxis);
//        _obbt._yAxis.normalize();
//        
//        mat.getForwardVector(&_obbt._zAxis);
//        _obbt._zAxis.normalize();
//        
//        _obbt._center = _model->getPosition3D();
//        
//        Vec3 corners[8] = {};
//        _obbt.getCorners(corners);
//        _drawDebug->drawCube(corners, Color4F(255,0,1,255));
//    }
}

const Vec3 Titan::getRotation3D(){
    return _model->getRotation3D();
}

void Titan::setRotation3D(const Vec3& rotation)
{
    _rotation = rotation;
    _model->setRotation3D(_rotation);
}

void Titan::updateTitanState()
{
    if (!_model) {
        return;
    }
//    _model->applyEmittingShader(, )
}

bool Titan::onTouched(CCTouch* pTouch)
{
    Vec2 touchPoint = this->convertToNodeSpace(pTouch->getLocation());
    // 下面的touch点转换是为了让点击区域在模型内
    float originX = -1 * getContentSize().width * getAnchorPoint().x;
    float originY = -1 * getContentSize().height * getAnchorPoint().y;
    touchPoint.x = touchPoint.x + originX;
    touchPoint.y = touchPoint.y + originY;
    
    Rect boundingBox(originX, originY, getContentSize().width, getContentSize().height);
//    Rect boundingBox = this->getBoundingBox();
    bool isTouched = boundingBox.containsPoint(touchPoint);
    if (!isTouched) {
        return false;
    }
    switch (_curState) {
        case eActState::Stand:
            changeTitanState(eActState::Walk);
            break;
//        case eActState::Walk:
//            changeTitanState(eActState::Idle);
            break;
        case eActState::Sleep:
            changeTitanState(eActState::Dream);
            break;
        default:
            break;
    }
    return true;
}

void Titan::initMovePoint(std::vector<Vec2> &movePoint)
{
    _movePointVec = movePoint;
//    std::vector<Vec2>::iterator it = movePoint.begin();
//    for (; it != movePoint.end(); it++) {
//        _movePointVec.push_back(it->second);
//    }
}

void Titan::changeTitanState(eActState state)
{
    _lastState = _curState;
    _curState = state;
    switch (state) {
        case eActState::Stand:
            stand();
            break;
        case eActState::Idle:
            ilde();
            break;
        case eActState::Attack:
            attack();
            break;
        case eActState::Walk:
            walk();
            break;
        case eActState::Sleep:
            sleep();
            break;
        case eActState::Dream:
            dream();
            break;
        default:
            break;
    }
}

sAnimationInfo* Titan::getAnimationByType(eActState e)
{
    sAnimationInfo* animInfo = new sAnimationInfo;
    
    std::string keyFrames = CCCommonUtils::getPropById(CC_ITOA(_tid), "keyFrames");
    vector<string> ks;
    CCCommonUtils::splitString(keyFrames, "|", ks);
    
    if (ks.size()>0) {
        if (ks.at(0) != "null") {
            for (int i = 0; i<ks.size(); i++) {
                
                animInfo->keyFrames.push_back(atoi(ks.at(i).c_str()));
            }
            
        }
    }
    
    
    std::string anims = CCCommonUtils::getPropById(CC_ITOA(_tid), "animations");
    vector<string> strArr;
    CCCommonUtils::splitString(anims, "|", strArr);
    
    std::string strInfo = strArr.at(int(e));
    if (strInfo.size() > 1) {
        vector<string> strArr1;
        CCCommonUtils::splitString(strInfo, ";", strArr1);
        if (strArr1.at(0) != "null") {
            animInfo->animationName = "3d/"+strArr1.at(0)+".c3b";
            animInfo->startFrame = atoi(strArr1.at(1).c_str());
            animInfo->endFrame = atoi(strArr1.at(2).c_str());
            return animInfo;
        }
    }
    

    

    animInfo->release();
    return nullptr;
}

void Titan::stand()
{
    std::map<eActState,Animate3D*>::iterator it = _actionCache.find(eActState::Stand);
    Animate3D* pAnim = nullptr;
    if (it == _actionCache.end()) {
        auto animInfo = getAnimationByType(eActState::Stand);
        if (animInfo) {
            auto animation3d = Animation3D::create(animInfo->animationName);
            if (animation3d) {
//                pAnim = Animate3D::create(animation3d);
                pAnim = Animate3D::createWithFrames(animation3d, animInfo->startFrame, animInfo->endFrame);
                pAnim->retain();
                _actionCache.insert(std::map<eActState,Animate3D*>::value_type(eActState::Stand,pAnim));
            }
        }
    }
    else
    {
        pAnim = it->second;
    }
    
    if (pAnim) {
        Action* act = RepeatForever::create(pAnim);
        act->setTag(1);
        _model->stopAllActions();
        _model->runAction(act);
    }
}

void Titan::ilde()
{
    std::map<eActState,Animate3D*>::iterator it = _actionCache.find(eActState::Idle);
    Animate3D* pAnim = nullptr;
    if (it == _actionCache.end()) {
        auto animInfo = getAnimationByType(eActState::Idle);
        if (animInfo) {
            auto animation3d = Animation3D::create(animInfo->animationName);
            if (animation3d) {
                pAnim = Animate3D::createWithFrames(animation3d, animInfo->startFrame, animInfo->endFrame);
                pAnim->retain();
                _actionCache.insert(std::map<eActState,Animate3D*>::value_type(eActState::Idle,pAnim));
            }
        }
    }
    else
    {
        pAnim = it->second;
    }
    if (pAnim) {
        Action* act = Repeat::create(pAnim,1);
        act->setTag(1);
        _model->stopAllActions();
        _model->runAction(act);
    }
}

void Titan::attack()
{
    if (true) {
        return;
    }
    std::map<eActState,Animate3D*>::iterator it = _actionCache.find(eActState::Attack);
    Animate3D* pAnim = nullptr;
    if (it == _actionCache.end()) {
        auto animInfo = getAnimationByType(eActState::Attack);
        if (animInfo) {
            auto animation3d = Animation3D::create(animInfo->animationName);
            if (animation3d) {
                pAnim = Animate3D::createWithFrames(animation3d, animInfo->startFrame, animInfo->endFrame);
                pAnim->retain();
                _actionCache.insert(std::map<eActState,Animate3D*>::value_type(eActState::Attack,pAnim));
            }
        }
    }
    else
    {
        pAnim = it->second;
    }
    
    if (pAnim) {
        Action* act = Repeat::create(pAnim,1);
        act->setTag(1);
        _model->stopAllActions();
        _model->runAction(act);
    }
}

void Titan::walk()
{
    std::map<eActState,Animate3D*>::iterator it = _actionCache.find(eActState::Walk);
    Animate3D* pAnim = nullptr;
    if (it == _actionCache.end()) {
        auto animInfo = getAnimationByType(eActState::Walk);
        if (animInfo) {
            auto animation3d = Animation3D::create(animInfo->animationName);
            if (animation3d) {
                pAnim = Animate3D::createWithFrames(animation3d, animInfo->startFrame, animInfo->endFrame);
                
                if (animInfo->keyFrames.size()!=0) {
                    ValueMap v;
                    
                    v.insert(std::make_pair("fly_Dust",Value("fly_Dust")));
//                    v.insert("action","fly_Dust");
                    for (int i=0; i<animInfo->keyFrames.size(); i++) {
                        pAnim->setKeyFrameUserInfo(animInfo->keyFrames.at(i),v);
                    }
                }
                pAnim->retain();
                _actionCache.insert(std::map<eActState,Animate3D*>::value_type(eActState::Walk,pAnim));
            }
        }
    }
    else
    {
        pAnim = it->second;
    }
    if (pAnim) {
        Action* act = Repeat::create(pAnim,1);
        act->setTag(1);
        _model->stopAllActions();
        _model->runAction(act);
    }
    else
    {
        changeTitanState(_lastState);
    }
}


void Titan::sleep()
{
    
}

void Titan::dream()
{
    
}

void Titan::turnFront()
{

    if (_curFaceDir != eFaceDir::Front) {
        _curFaceDir = eFaceDir::Front;
        //begin a by ljf
        //_model->setRotation3D(Vec3(0,180,0));
        //end a by ljf
        // 面朝正方向
        //_model->setRotation3D(Vec3(38, 39, -24));
        // 面朝左90度
//        _model->setRotation3D(Vec3(38, -51, 24));
        // 面朝左90度
//        _model->setRotation3D(Vec3(38, -6, 24));
    }
}

void Titan::turnBack()
{
    if (_curFaceDir != eFaceDir::Back) {
        _curFaceDir = eFaceDir::Back;
//        _model->setRotation3D(Vec3(-32, -141, -24));
    }
}

void Titan::onRotationY(float dtAngle)
{
    if (!_model) {
        return;
    }
    Vec3 r = _model->getRotation3D();
    _model->setRotation3D(Vec3(r.x, r.y+dtAngle, r.z));

}

void Titan::reset()
{
    if (!_model) {
        return;
    }
    _model->setPosition(Vec2(getContentSize().width*0.5,0));
    changeTitanState(eActState::Stand);
    _curMovePosIndex = 0;
    turnFront();

}

void Titan::onRotateX(float dtAngle)
{
    Vec3 r = _model->getRotation3D();
    float n = r.x+dtAngle*3;
    _model->setRotation3D(Vec3(n, r.y, r.z));
    CCLOG("Titan rotation----: %f , %f , %f ",n, r.y, r.z);
    
}
void Titan::onRotateY(float dtAngle)
{
    Vec3 r = _model->getRotation3D();
    float n = r.y+dtAngle*3;
    _model->setRotation3D(Vec3(r.x, n, r.z));
    CCLOG("Titan rotation----: %f , %f , %f ",r.x, n, r.z);
}
void Titan::onRotateZ(float dtAngle)
{
    Vec3 r = _model->getRotation3D();
    float n = r.z+dtAngle*3;
    _model->setRotation3D(Vec3(r.x, r.y, n));
    CCLOG("Titan rotation----: %f , %f , %f ",r.x, r.y, n);
}

void Titan::onReset()
{
    _model->setRotation3D(Vec3(0, 0, 0));

}