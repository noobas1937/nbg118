//
//  HFViewport.cpp
//  HappyFarm
//
//  Created by wuyuqun on 13-9-7.
//
//

#include "HFViewport.h"
#include "ImperialScene.h"
#include "WorldController.h"
#include "UIComponent.h"

USING_NS_CC;

#define ZOOM_FACTOR 0.4
#define OPERATION_DAMPING 1.0
#define SCROLL_EASEOUT_RATE 1.8
#define ZOOM_THRESHOLD 1
#define LONG_CLICK_DURATION 0.9
#define SPEED_TRACKING_DURATION 0.05
#define ZOOM_BOUND_LIMIT_PERCENT 0.2
#define FLING_DURATION 0.4
#define SCROLL_DEACCEL_RATE  0.96f
#define SCROLL_DEACCEL_DIST  0.5f
#define FLINGACTION_TAG 10000
#define ZOOM_DEACCEL_RATE 0.95f
//#define ZOOM_DELTA        0.05f
#define UPDATE_TIMES 20
#define MOVE_INCH            1.0f/300.0f

#define HIGH_FPS	(1.0/30)
#define LOW_FPS		(1.0/15)

static float convertDistanceFromPointToInch(float pointDis)
{
    float factor = (Director::sharedDirector()->getOpenGLView()->getScaleX() + Director::sharedDirector()->getOpenGLView()->getScaleY() ) / 2;
    return pointDis * factor / CCDevice::getDPI();
}

void HFViewport::setViewPortTarget(cocos2d::CCNode* target)
{
    m_TargetNode = target;
}

// 取得ccb分辨率与当前屏幕分辨率之间的比
float HFViewport::getResolutionRate()
{
    return 1.0;
}

bool HFViewport::init()
{
    do
    {
        CC_BREAK_IF(!CCLayer::init());
        mTouchSlot = 0 == mTouchSlot? 2.5 * 2.5 : mTouchSlot * mTouchSlot;
        mCurZoomScale = getResolutionRate();
        mForceStopScroll = false;
        mMovable = true;
        mTouchable = true;
        mMapSize = CCSizeZero;
        mPreviewPos = CCPointZero;
        
        isMove = false;
        notMove = false;
        this->scheduleUpdate();
        triggerTime = 2;//fusheng 两秒
        return true;
    }
    while(0);
    return false;
}

void HFViewport::onEnter()
{
//    setSwallowsTouches(true);
    setTouchMode(Touch::DispatchMode::ALL_AT_ONCE);
    setTouchEnabled(true);
    CCNode::onEnter();
    //CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);
}
void HFViewport::update(float time)
{
    if (isBeginTouch&&isCanMoveAction) {
        
        touchSecond += time;
        
        if (touchSecond >= triggerTime) {
            if (!isUIMove) {
                isUIMove = true;
                //fusheng 移动UI
                UIComponent::getInstance()->moveOut();
            }
            
        }
    }
}
void HFViewport::onExit()
{
    setTouchEnabled(false);
    setTouchEnabled(false);
    CCNode::onExit();
}

void HFViewport::setSceneSize(int w, int h)
{
    MAP_WIDTH = w;
    MAP_HEIGHT = h;
}

float HFViewport::getMinZoom()
{
    CC_ASSERT(this->m_TargetNode);
    cocos2d::CCPoint maxScale(1,1);
    if( this->m_TargetNode->getContentSize().width > 0 )
    {
        maxScale.x = cocos2d::CCDirector::sharedDirector()->getWinSize().width / this->mWorldBound.size.width;
    }
    
    if( this->m_TargetNode->getContentSize().height > 0 )
    {
        maxScale.y = cocos2d::CCDirector::sharedDirector()->getWinSize().height / this->mWorldBound.size.height;
    }
    return MAX(maxScale.x , maxScale.y);
}

void HFViewport::alignWithTargetCenter()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CC_ASSERT(this->m_TargetNode);
    this->m_TargetNode->setScale(mCurZoomScale);
    this->m_TargetNode->setPosition((winSize.width-MAP_WIDTH)/2, (winSize.height-MAP_HEIGHT)/2);
}

void HFViewport::beginScroll(cocos2d::CCTouch* touch)
{
    mScrollStartPoint = touch->getLocation();
    mOperationStartPostition = this->m_TargetNode->getPosition();
    CCLOG("beginScroll mScrollStartPoint=(%f,%f), mOperationStartPostition=(%f,%f)", mScrollStartPoint.x, mScrollStartPoint.y, mOperationStartPostition.x, mOperationStartPostition.y);
    
    if (mTouchDelegate) {
        mTouchDelegate->onBeginScroll(m_TargetNode->convertTouchToNodeSpace(touch));
    }
    Director::getInstance()->setAnimationInterval(HIGH_FPS);
}

CCPoint HFViewport::getCurrentCenterGridPoint(){
    CCPoint mtargetPosition = m_TargetNode->getPosition();
    CCPoint currentCenter;
    CCPoint visibleTopRight(CCDirector::sharedDirector()->getWinSize().width, CCDirector::sharedDirector()->getWinSize().height);
    currentCenter.x = (visibleTopRight.x/2 - mtargetPosition.x)/m_TargetNode->getScale();
    currentCenter.y = (visibleTopRight.y/2 - mtargetPosition.y)/m_TargetNode->getScale();
    return currentCenter;
}
void HFViewport::centOnPoint(cocos2d::CCPoint curTouch, bool useTween){
    mIsProcessingScroll = true;
    CCPoint visibleTopRight(CCDirector::sharedDirector()->getWinSize().width, CCDirector::sharedDirector()->getWinSize().height);
    curTouch.x = visibleTopRight.x/2- curTouch.x*m_TargetNode->getScale();
    curTouch.y = visibleTopRight.y/2- curTouch.y*m_TargetNode->getScale();
    boundPos(curTouch);
    if(useTween){
        m_TargetNode->runAction(CCMoveTo::create(0.3, curTouch));
    }else{
        m_TargetNode->setPosition(curTouch);
    }
}

void HFViewport::stepScroll(cocos2d::CCTouch* touch)
{
    if (!mIsScrollBegan) {
        mIsScrollBegan = true;
        beginScroll(touch);
    }
    mIsProcessingScroll = true;
    CCPoint curTouch = touch->getLocation();
    CCPoint curDiff(curTouch.x - mScrollStartPoint.x, curTouch.y - mScrollStartPoint.y);
    
    if (curDiff.getLength()>20) {
        isMove = true;
    }
    
    if( this->mTouchMode == TouchMode_Scroll )
    {
        if (mTouchDelegate) {
            mTouchDelegate->onStepScroll(m_TargetNode->convertTouchToNodeSpace(touch), ccpMult(curTouch - mScrollStartPoint, 1 / this->m_TargetNode->getScale()));
        }
        curDiff = ccpMult(curDiff, OPERATION_DAMPING);
        CCPoint curPos = ccpAdd(mOperationStartPostition, curDiff);
        boundPos(curPos);
        CCLOG("%d,,,,,,,%d",(int)curPos.x, (int)curPos.y);
        CCLOGFUNCF("taget scale:%f", this->m_TargetNode->getScale());
        this->m_TargetNode->setPosition(curPos);
    }
}

void HFViewport::endScroll(cocos2d::CCTouch* touch)
{
    CCLOGFUNCF("speed: %f, %f", m_tScrollDistance.x, m_tScrollDistance.y);
    do
    {
        if (isAutoMove) {
            this->schedule(schedule_selector(HFViewport::intervalMove));
            this->mTouchMode = TouchMode_Fling;
        }
        else
        {
            Director::getInstance()->setAnimationInterval(LOW_FPS);
        }
    }
    while(0);
    mIsProcessingScroll = false;
    if (mTouchDelegate) {
        mTouchDelegate->onEndScroll(m_TargetNode->convertTouchToNodeSpace(touch));
    }
}

void HFViewport::intervalMove(float dt)
{
    if (limitSpeedX > 0 && fabsf(m_tScrollDistance.x) > limitSpeedX) {
        m_tScrollDistance.x = m_tScrollDistance.x > 0 ? limitSpeedX:-limitSpeedX;
    }
    if (limitSpeedY > 0 && fabsf(m_tScrollDistance.y) > limitSpeedY) {
        m_tScrollDistance.y = m_tScrollDistance.y > 0 ? limitSpeedY:-limitSpeedY;;
    }
    applyPos(m_tScrollDistance);
    m_tScrollDistance = ccpMult(m_tScrollDistance, SCROLL_DEACCEL_RATE);
    
    if (fabsf(m_tScrollDistance.x) <= SCROLL_DEACCEL_DIST &&
        fabsf(m_tScrollDistance.y) <= SCROLL_DEACCEL_DIST)
    {
        this->unschedule(schedule_selector(HFViewport::intervalMove));
        this->mTouchMode = TouchMode_None;
        if (mTouchDelegate) {
            mTouchDelegate->autoMoveEnd();
        }
        Director::getInstance()->setAnimationInterval(LOW_FPS);
    }
}

void HFViewport::applyPos(CCPoint position)
{
    CCPoint newPosition = ccpAdd(this->m_TargetNode->getPosition(), position);
    if (boundPos(newPosition)) {
        this->m_TargetNode->setPosition(newPosition);
        mWSZoomCenter = ccpAdd(mWSZoomCenter, position);
        mNSZoomCenter = this->m_TargetNode->convertToNodeSpace(mWSZoomCenter);
    }
}

// 移动场景到指定的坐标点
void HFViewport::scrollTo(const CCPoint& targetPoint, CCCallFunc* callbackFunc, bool Flying)
{
    unscheduleAll();
    mIsProcessingScroll = true;
    do
    {
        this->mTouchMode = TouchMode_Fling;
        CCPoint curPos = this->m_TargetNode->getPosition();
        CCPoint targetPos = targetPoint;
        CCSize winsize = CCDirector::sharedDirector()->getWinSize();
        if (targetPos.x >0.0f) {
            targetPos.x = curPos.x;
        }
        if (targetPos.x <winsize.width-MAP_WIDTH*this->m_TargetNode->getScaleX()){
            targetPos.x = winsize.width-MAP_WIDTH*this->m_TargetNode->getScaleX();
        }
        if (targetPos.y >0.0f) {
            targetPos.y = curPos.y;
        }
        if (targetPos.y <winsize.height-MAP_HEIGHT*this->m_TargetNode->getScaleY()){
            targetPos.y =winsize.height-MAP_HEIGHT*this->m_TargetNode->getScaleY();
        }
        if (Flying) {
            CCMoveTo* moveTo = CCMoveTo::create(0.4f, targetPos);
            CCEaseOut* ease = CCEaseOut::create(moveTo, 3);
            ease->setTag(FLINGACTION_TAG);
            this->m_TargetNode->runAction(CCRepeat::create(CCSequence::create(ease, callbackFunc, NULL), 1));
        }
        else{
            this->m_TargetNode->setPosition(targetPos);
        }
    }
    while(0);
    mIsProcessingScroll = false;
}

void HFViewport::changeSceneZoom(){
    CCPoint orgPt = m_TargetNode->getPosition();
    float orgS = m_TargetNode->getScale();
    float finalS = orgS - 0.2;
    float time = 0.2;
    CCPoint setPt = orgPt * (finalS / orgS);
    scrollTo(setPt, NULL);
    applyZoom(finalS);
    CCPoint finalPt = m_TargetNode->getPosition();
    finalS = m_TargetNode->getScale();
    m_TargetNode->setPosition(orgPt);
    m_TargetNode->setScale(orgS);
    CCSpawn *spawn =CCSpawn::createWithTwoActions(CCMoveTo::create(time, finalPt), CCScaleTo::create(time, finalS));
    m_TargetNode->runAction(CCSequence::create(spawn, NULL));
}

void HFViewport::scrollBy(const CCPoint& targetPoint, CCCallFunc* callbackFunc,float time)
{
    unscheduleAll();
    mIsProcessingScroll = true;
    do
    {
        this->mTouchMode = TouchMode_Fling;
        CCPoint curPos = this->m_TargetNode->getPosition();
        CCPoint targetPos = ccpSub(curPos,targetPoint);
        CCSize winsize = CCDirector::sharedDirector()->getWinSize();
        if (targetPos.x >0.0f) {
            targetPos.x = curPos.x;
        }
        if (targetPos.x <winsize.width-MAP_WIDTH*this->m_TargetNode->getScaleX()){
            targetPos.x = winsize.width-MAP_WIDTH*this->m_TargetNode->getScaleX();
        }
        if (targetPos.y >0.0f) {
            targetPos.y = curPos.y;
        }
        if (targetPos.y <winsize.height-MAP_HEIGHT*this->m_TargetNode->getScaleY()){
            targetPos.y =winsize.height-MAP_HEIGHT*this->m_TargetNode->getScaleY();
        }        if(time <0.0f)
        {
            this->m_TargetNode->setPosition(targetPos);
            return;
        }
        CCMoveBy* moveTo = CCMoveBy::create(time, targetPos);
        CCEaseOut* ease = CCEaseOut::create(moveTo, 3);
        ease->setTag(FLINGACTION_TAG);
        this->m_TargetNode->runAction(CCRepeat::create(CCSequence::create(ease, callbackFunc, NULL), 1));
        this->mTouchMode = TouchMode_Fling;
    }
    while(0);
    mIsProcessingScroll = false;
}

void HFViewport::scrollToSmooth(const CCPoint& targetPoint, CCCallFunc* callbackFunc,float time)
{
    unscheduleAll();
    mIsProcessingScroll = true;
    do
    {
        this->mTouchMode = TouchMode_Fling;
        CCPoint curPos = this->m_TargetNode->getPosition();
        CCPoint targetPos = targetPoint;
        CCSize winsize = CCDirector::sharedDirector()->getWinSize();
        if (targetPos.x >0.0f) {
            targetPos.x = curPos.x;
        }
        if (targetPos.x <winsize.width-MAP_WIDTH*this->m_TargetNode->getScaleX()){
            targetPos.x = winsize.width-MAP_WIDTH*this->m_TargetNode->getScaleX();
        }
        if (targetPos.y >0.0f) {
            targetPos.y = curPos.y;
        }
        if (targetPos.y <winsize.height-MAP_HEIGHT*this->m_TargetNode->getScaleY()){
            targetPos.y =winsize.height-MAP_HEIGHT*this->m_TargetNode->getScaleY();
        }
        CCMoveTo* move = CCMoveTo::create(time, targetPos);
        CCAction* action = CCSequence::create(move,callbackFunc,NULL);
        action->setTag(FLINGACTION_TAG);
        this->m_TargetNode->runAction(action);
    }
    while(0);
    mIsProcessingScroll = false;
}

void HFViewport::beginZoom(const CCPoint& point1, const CCPoint& point2)
{
    CC_ASSERT(this->m_TargetNode);
    mZoomFingersDistance = ccpDistance(point1, point2);
    if (mForceStopScroll || !mMovable) {
        mWSZoomCenter = ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2);
    }
    else {
        mWSZoomCenter = ccpMidpoint(point1, point2);
    }
    mNSZoomCenter = this->m_TargetNode->convertToNodeSpace(mWSZoomCenter);
    
    mZoomOldScale = this->m_TargetNode->getScale();
    mOperationStartPostition = this->m_TargetNode->getPosition();
    
    if (mTouchDelegate) {
        mTouchDelegate->onBeginZoom(point1, point2);
    }
}

void HFViewport::applyZoom(float newZoom)
{
    if(!allowZoom){
        return;
    }

    float oldScale = this->m_TargetNode->getScale();
    this->m_TargetNode->setScale(newZoom);
    
    CCPoint newCenter(mNSZoomCenter);
    
    newCenter = this->m_TargetNode->convertToWorldSpace(newCenter);
    
    CCPoint centerDif = ccpSub(mWSZoomCenter, newCenter);
    
    cocos2d::CCPoint pos = ccpAdd(m_TargetNode->getPosition(), centerDif);
//    CCLOGFUNCF("pos.x=%f, pos.y=%f, centerDif.x=%f, y=%f", pos.x, pos.y, centerDif.x, centerDif.y);
    if(boundPos(pos))
    {
        this->m_TargetNode->setPosition(pos);
        mCurZoomScale = newZoom;
//        CCLOGFUNCF("new mCurZoomScale=%f", mCurZoomScale);
        
    }
    else
    {
        this->m_TargetNode->setScale(oldScale);
        mCurZoomScale = oldScale;
//        CCLOGFUNCF("old mCurZoomScale=%f", mCurZoomScale);
    }
}

void HFViewport::stepZoom(const CCPoint& point1, const CCPoint& point2, float scale)
{
    if(!allowZoom){
        return;
    }
    mIsProcessingZoom = true;
    float curDistance = ccpDistance(point1, point2);
    
    float curScale = (curDistance) / mZoomFingersDistance * mZoomOldScale;
    
    if (scale==0.0f) {
        if( curScale > mZoomLimitationMax )
        {
            curScale = mZoomLimitationMax;
        }
        else
            if( curScale < mZoomLimitationMin )
            {
                curScale = mZoomLimitationMin;
            }
    }
    else{
        curScale = scale;
    }
    applyZoom(curScale);
    
    if (mTouchDelegate) {
        mTouchDelegate->onStepZoom(point1, point2);
    }
}

void HFViewport::updatePosition(CCPoint pos, bool animate, float time, bool bound) {
    //user interface to boundPos basically
    m_TargetNode->stopAllActions();
    if (bound) {
        boundPos(pos);
    }
	if(!animate){
        m_TargetNode->setPosition(pos);
    }else{
        m_TargetNode->runAction(CCMoveTo::create(time, pos));
    }
}

bool HFViewport::boundPos(cocos2d::CCPoint& pos)
{
    auto winsize = CCDirector::sharedDirector()->getWinSize();
    float scale = m_TargetNode->getScale();
    mCurZoomScale = scale;
    
    //Correct for anchor
    cocos2d::CCPoint anchor = ccp(m_TargetNode->getContentSize().width * m_TargetNode->getAnchorPoint().x,
                                  m_TargetNode->getContentSize().height * m_TargetNode->getAnchorPoint().y);
    
    anchor = ccpMult(anchor, (1.0f - scale));
    
    CCPoint visibleTopRight(winsize.width, winsize.height);
    CCPoint visibleBottomLeft = CCPointZero;
    visibleTopRight = ccpAdd(visibleTopRight, visibleBottomLeft);
    
    auto size = CCDirector::sharedDirector()->getWinSize();
    float fixW = size.width;
    float fixH = size.height;

    if (isForDiamond) {
        auto fixSize = mWorldBound.size + CCSize(fixW / 2,fixH / 2);
        auto fixOrigin = mWorldBound.origin + ccp(fixW / 2, fixH / 2);
        topRight = ccpAdd(ccpSub(ccpMult(fixSize, scale), visibleTopRight), anchor);
        bottomLeft = ccpSub(ccpAdd(ccpMult(fixOrigin, scale), visibleBottomLeft), anchor);
        
    } else {
        topRight = ccpAdd(ccpSub(ccpMult(this->mWorldBound.size, scale), visibleTopRight), anchor);
        bottomLeft = ccpSub(ccpAdd(ccpMult(this->mWorldBound.origin, scale), visibleBottomLeft), anchor);
    }
    
    
    if (bottomLeft.x < pos.x ) 
    {
        pos.x = bottomLeft.x;
    }
    else if (pos.x < -topRight.x)
    {
        pos.x = -topRight.x;
    }
    
    if (bottomLeft.y < pos.y )
    {
        pos.y = bottomLeft.y;
    }
    else if(pos.y < -topRight.y)
    {
        pos.y = -topRight.y;
    }
    
    if (isForDiamond) {
        auto isNeedToAdjust = [](const CCPoint &p1, const CCPoint &p2){
            int dx = (p2.x - p1.x);
            int dy = (p2.y - p1.y);
            dx = abs(dx);
            dy = abs(dy);
            if(dx > 2 * dy){
                return true;
            }
            return false;
        };
        
        auto adjust = [](CCPoint &p1, CCPoint &p2, bool isTop){
            int dx = (p2.x - p1.x);
            dx = abs(dx);

            if(isTop){
                p1.y = p2.y + dx / 2;
            }else{
                p1.y = p2.y - dx / 2;
            }
        };
        auto ptDown = ccp(-bottomLeft.x + mWorldBound.size.width / 2 * scale, -bottomLeft.y) * -1;
        auto ptUp = ccp(-bottomLeft.x + mWorldBound.size.width / 2 * scale, -bottomLeft.y + mWorldBound.size.height * scale) * -1;
        auto centerPt = (ptDown + ptUp) / 2;
        if(pos.y > centerPt.y){//left_up and left_down
            if(isNeedToAdjust(pos, ptDown)){
                adjust(pos, ptDown, false);
            }
        }else{
            if(isNeedToAdjust(pos, ptUp)){
                adjust(pos, ptUp, true);
            }
        }
        
        
//        auto pInside = [](const CCPoint& p1,const CCPoint& p2,const CCPoint& p3){
//            // (x1-x3)*(y2-y3)-(y1-y3)*(x2-x3)
//            // p3 is at left side to p1p2,then return true
//            return (p1.x-p3.x)*(p2.y-p3.y)-(p1.y-p3.y)*(p2.x-p3.x) < 0 ? false : true;
//        };
//        
//        float halfWidth = (topRight.x - bottomLeft.x)/2;
//        float halfHeight = (topRight.y - bottomLeft.y)/2;
//        
//        // caution: bottomLeft and topRight is reverse
//        // and pos is also reverse
//        // so when pos in the top left zone,it means the bottom right zone
//        auto centerPoint = -(bottomLeft + topRight)/2;
//        auto reversePos = centerPoint*2 - pos;
//        
////        float wh = halfWidth/halfHeight;
//        float hw = halfHeight/halfWidth;
//        auto pLeft = ccp(-topRight.x,-topRight.y+halfHeight);
//        auto pBottom = ccp(-topRight.x+halfWidth,-topRight.y);
//        auto pRight = ccp(-topRight.x+2*halfWidth, -topRight.y+halfHeight);
//        auto pTop = ccp(-topRight.x+halfWidth, -topRight.y+2*halfHeight);
//        
//        if (pos.x < centerPoint.x && pos.y < centerPoint.y) {
//            // top right corner
//            if (pInside(pTop,pRight,reversePos)) {
////                if (m_tScrollDistance.x == 0 || m_tScrollDistance.y/m_tScrollDistance.x > wh) {
////                    pos.x = -wh*(pos.y-pLeft.y)+pLeft.x;
////                } else {
//                    pos.y = -hw*(pos.x-pLeft.x)+pLeft.y;
////                }
//            }
//            
//        } else if (pos.x < centerPoint.x && pos.y > centerPoint.y) {
//            // bottom right corner
//            if (pInside(pRight,pBottom,reversePos)) {
////                if (m_tScrollDistance.x == 0 || -m_tScrollDistance.y/m_tScrollDistance.x > wh) {
////                    pos.x = wh*(pos.y-pTop.y)+pTop.x;
////                } else {
//                    pos.y = hw*(pos.x-pTop.x)+pTop.y;
////                }
//            }
//            
//        } else if (pos.x > centerPoint.x && pos.y > centerPoint.y) {
//            // bottom left corner
//            if (pInside(pBottom,pLeft,reversePos)) {
////                if (m_tScrollDistance.x == 0 || m_tScrollDistance.y/m_tScrollDistance.x > wh) {
////                    pos.x = -wh*(pos.y-pRight.y)+pRight.x;
////                } else {
//                    pos.y = -hw*(pos.x-pRight.x)+pRight.y;
////                }
//            }
//            
//        } else if (pos.x > centerPoint.x && pos.y < centerPoint.y) {
//            // top left corner
//            if (pInside(pLeft,pTop,reversePos)) {
////                if (m_tScrollDistance.x == 0 || -m_tScrollDistance.y/m_tScrollDistance.x > wh) {
////                    pos.x = wh*(pos.y-pBottom.y)+pBottom.x;
////                } else {
//                    pos.y = hw*(pos.x-pBottom.x)+pBottom.y;
////                }
//            }
//        }
    }
    
    return true;
}

void HFViewport::endZoom()
{
    float curScale = this->m_TargetNode->getScale();
    CCLOG("endZoom with scale: %f", curScale);
    float targetZoom =0.0;
    bool moreThanMax = false;
    bool lessThanMin = false;
    if( curScale >= mZoomLimitationMax * (1 - ZOOM_BOUND_LIMIT_PERCENT) ) {
        moreThanMax = true;
        targetZoom = mZoomLimitationMax * (1 - ZOOM_BOUND_LIMIT_PERCENT);
    }
    else if (curScale < mZoomLimitationMin * (1 + ZOOM_BOUND_LIMIT_PERCENT)) {
        lessThanMin = true;
        targetZoom = mZoomLimitationMin * (1 + ZOOM_BOUND_LIMIT_PERCENT);
    }
    else{
        targetZoom = curScale;
    }
    
    if (!isAutoZoom) {
        targetZoom = curScale;
    }
    
    if( targetZoom != curScale )
    {
        if(moreThanMax){
            //计算指数递减初始值 sum = 1 / (1 - q), q为比值
            zoomDelta = (this->m_TargetNode->getScale() - mZoomLimitationMax * (1 - ZOOM_BOUND_LIMIT_PERCENT)) / ((1/(1-ZOOM_DEACCEL_RATE))*0.95);
            this->mTouchMode = TouchMode_ZoomFling;
            this->schedule(schedule_selector(HFViewport::intervalZoom));
        }
        else if (lessThanMin) {
            //计算指数递减初始值 sum = 1 / (1 - q), q为比值
//            zoomDelta = (mZoomLimitationMin - this->m_TargetNode->getScale()) / ((1/(1-ZOOM_DEACCEL_RATE))*0.95);
            zoomDelta = (mZoomLimitationMin * (1 + ZOOM_BOUND_LIMIT_PERCENT) - this->m_TargetNode->getScale()) / ((1/(1-ZOOM_DEACCEL_RATE))*0.95);
            this->mTouchMode = TouchMode_ZoomFling;
            this->schedule(schedule_selector(HFViewport::intervalZoom));
        }
        else{
            applyZoom(targetZoom);
        }
    }
    
    mIsProcessingZoom = false;
    
    if (mTouchDelegate) {
        mTouchDelegate->onEndZoom(targetZoom);
    }
}

void HFViewport::intervalZoom(float dt){
    if(!allowZoom){
        return;
    }

    applyPos(m_tScrollDistance);
    m_tScrollDistance = ccpMult(m_tScrollDistance, SCROLL_DEACCEL_RATE);
    float curScale = this->m_TargetNode->getScale();
    float targetZoom = 0.0;
    if ((curScale == mZoomLimitationMin * (1 + ZOOM_BOUND_LIMIT_PERCENT) ||
         curScale == mZoomLimitationMax * (1 - ZOOM_BOUND_LIMIT_PERCENT)) &&
        (fabsf(m_tScrollDistance.x) <= SCROLL_DEACCEL_DIST &&
         fabsf(m_tScrollDistance.y) <= SCROLL_DEACCEL_DIST))
    {
        this->unschedule(schedule_selector(HFViewport::intervalZoom));
        this->mTouchMode = TouchMode_None;
    }
    
    if( curScale > mZoomLimitationMax * (1 - ZOOM_BOUND_LIMIT_PERCENT) )
    {
        targetZoom=curScale-zoomDelta;
        if(targetZoom<mZoomLimitationMax * (1 - ZOOM_BOUND_LIMIT_PERCENT)){
            targetZoom=mZoomLimitationMax * (1 - ZOOM_BOUND_LIMIT_PERCENT);
        }
        
        if( targetZoom != curScale )
        {
            if(this->mTouchMode==TouchMode_GOUND){
              applyZoom(curScale);   
            }else{
                applyZoom(targetZoom);
            }
            zoomDelta *= ZOOM_DEACCEL_RATE;
        }
    }
    else if (curScale <= mZoomLimitationMin * (1 + ZOOM_BOUND_LIMIT_PERCENT)) {
        targetZoom=curScale+zoomDelta;
        if(targetZoom > mZoomLimitationMin * (1 + ZOOM_BOUND_LIMIT_PERCENT)){
            targetZoom = mZoomLimitationMin * (1 + ZOOM_BOUND_LIMIT_PERCENT);
        }
        
        if( targetZoom != curScale )
        {
            if(this->mTouchMode==TouchMode_GOUND){
                applyZoom(curScale);
            }else{
                applyZoom(targetZoom);
            }
            zoomDelta *= ZOOM_DEACCEL_RATE;;
        }
    }
}

void HFViewport::unscheduleAll()
{
    if (this->m_TargetNode->getActionByTag(FLINGACTION_TAG) != NULL) {
        this->m_TargetNode->getActionByTag(FLINGACTION_TAG)->stop();
    }
    unschedule(schedule_selector(HFViewport::intervalMove));
    unschedule(schedule_selector(HFViewport::intervalZoom));
    unschedule(schedule_selector(HFViewport::zoomInOut));

    if (this->mTouchMode == TouchMode_Fling) {
        this->mTouchMode = TouchMode_None;
    }
    if (this->mTouchMode == TouchMode_ZoomFling) {
        this->mTouchMode = TouchMode_None;
    }
    if (!m_doubleClicked) {
        this->mTouchMode = TouchMode_None;
    }
}

void HFViewport::onTouchesBegan(const std::vector<Touch*>& pTouches, Event *pEvent)
{
    unscheduleAll();
    CC_ASSERT(this->m_TargetNode);
    //tao.yu 为解决batch不跟随自定义摄像机走的问题，增加此行代码
//    if (m_TargetNode->getTag() == IMPERIAL_SCENE_TOUCH_LAYER_TAG)
    {
        m_TargetNode->setCameraMask((unsigned short)CameraFlag::USER4, true);
        auto node3d = m_TargetNode->getChildByTag(JUST3D_NODE_TAG);
        if (node3d) {
            node3d->setCameraMask((unsigned short)CameraFlag::USER2, true);
        }
    }
//    CC_ASSERT(pTouches);
    
    if (mFingerMap.size() >= 2) {
        isMove = true;
        return;
    }
    
    if( this->mTouchMode == TouchMode_Fling )
    {
        this->m_TargetNode->stopActionByTag(FLINGACTION_TAG);
    }
    this->mTouchMode = TouchMode_None;
    
    
    for ( auto &item: pTouches )
    {
        CCTouch* curTouch = dynamic_cast<CCTouch*>(item);
        CC_ASSERT(curTouch);
        if (mFingerMap.size() <= 2) {
            mFingerMap[curTouch->getID()] = curTouch->getLocation();
        }
    }
//    
//    CCSetIterator iter = pTouches->begin();
//    
//    while( iter != pTouches->end() )
//    {
//        CCTouch* curTouch = dynamic_cast<CCTouch*>(*iter);
//        CC_ASSERT(curTouch);
//        if (mFingerMap.size() <= 2) {
//            mFingerMap[curTouch->getID()] = curTouch->getLocation();
//        }
//        iter++;
//    }
    
    switch(mFingerMap.size())
    {
        case 1:            
        {
            m_bTouchMoved = false;
            if (this->mTouchMode != TouchMode_ZoomFling) {
                this->mTouchMode = TouchMode_Scroll;
            }

            if (!mForceStopScroll && mMovable) {
                CCTouch* objTouch = dynamic_cast<CCTouch*>(*pTouches.begin());
                CC_ASSERT(objTouch);
                // let "beginScroll" fired by first step scroll , make single touch not fire this
                //                beginScroll(objTouch);
                if (mTouchDelegate) {
                    mTouchDelegate->onSingleTouchBegin(this->m_TargetNode->convertToNodeSpace(objTouch->getLocation()));
                    mTouchDelegate->onSingleTouchBegin(objTouch);
                }
            }
            
            CCTouch* ptouch =getAnyTouchObject(pTouches);
            mPreviewPos = ptouch->getLocation();
            
            if (!isBeginTouch) {
                isBeginTouch = true;
                touchSecond = 0.0;
            }

        }
            break;
        case 2:
            if (this->mTouchMode != TouchMode_ZoomFling) {
                this->mTouchMode = TouchMode_Zoom;
            }
            if(!allowZoom){
                return;
            }
            do
            {
                FingerMap::iterator iter = mFingerMap.begin();
                CC_BREAK_IF(iter == mFingerMap.end());
                CCPoint pos1 = (*iter).second;
                CC_BREAK_IF(++iter == mFingerMap.end());
                CCPoint pos2 = (*iter).second;
                beginZoom(pos1, pos2);
            }
            while(0);
            isMove = true;
            
            isBeginTouch = false;
            touchSecond = 0.0;
            break;
        default:
            isMove = true;
            CCLOG("begin:Touch not support with %d fingers", pTouches.size());
    }
}

Touch* HFViewport::getAnyTouchObject(const std::vector<Touch*>& pTouches)
{
    std::vector<Touch*>::const_iterator itor = pTouches.begin();
    for(;itor!=pTouches.end();itor++)
    {
        if(*itor)
        {
            return *itor;
        }
    }
    return NULL;
}

void HFViewport::onTouchesMoved(const std::vector<Touch*>& pTouches, Event *pEvent)
{
    if (notMove) {
        return;
    }
   
    CC_ASSERT(this->m_TargetNode);
    vector<CCTouch*> tmpTouches;
    auto tmpIt = pTouches.begin();
    while (tmpIt != pTouches.end()) {
        auto tmpTouch = dynamic_cast<CCTouch*>(*tmpIt);
        if (mFingerMap.find(tmpTouch->getID()) != mFingerMap.end()) {
            tmpTouches.push_back(tmpTouch);
        }
        ++tmpIt;
    }
    switch(tmpTouches.size())
    {
        case 1:
        {
            CCTouch* ptouch = *tmpTouches.begin();
            if (!m_bTouchMoved && !moveableCheck(ptouch)) {
                return;
            }
            
//            CC_BREAK_IF(this->mTouchMode != TouchMode_Scroll);
            
            if (this->mTouchMode != TouchMode_Scroll) {
                isBeginTouch = false;
                touchSecond = 0.0;
                break;
            }
            if (isBeginTouch)
            {
                CCTouch* ptouch =getAnyTouchObject(pTouches);
                auto newPos = ptouch->getLocation();
                
                if (ccpSub(newPos, mPreviewPos).length()>10) {
                    isBeginTouch = false;
                    touchSecond = 0.0;
                }
            }
            
            if (!mForceStopScroll && mMovable) {
                CCPoint newPoint, moveDistance;
                stepScroll(ptouch);
                m_tTouchTime = getNowTime();
                newPoint     = this->convertTouchToNodeSpace(ptouch);
                moveDistance = ccpSub(newPoint, m_tTouchPoint);
                
                if (!mIsProcessingScroll) {
                    m_tTouchPoint = CCPointZero;
                }
                
                float dis = 0.0f;
                dis = sqrtf(moveDistance.x*moveDistance.x + moveDistance.y*moveDistance.y);
                
                if (fabs(convertDistanceFromPointToInch(dis)) < MOVE_INCH )
                {
                    //CCLOG("Invalid movement, distance = [%f, %f], disInch = %f", moveDistance.x, moveDistance.y);
                    return;
                }
                if (!m_bTouchMoved)
                {
                    moveDistance = CCPointZero;
                }
                m_tTouchPoint = newPoint;
                m_bTouchMoved = true;
                m_tScrollDistance = moveDistance;
                
            }
        }
            break;
        case 2:
            CC_BREAK_IF(this->mTouchMode != TouchMode_Zoom);
            do
            {
                CCTouch* objTouch1 = *tmpTouches.begin();
                CCTouch* objTouch2 = tmpTouches.back();
                stepZoom(objTouch1->getLocation(), objTouch2->getLocation());
            }
            while(0);
            break;
        default:
            CCLOG("move:Touch not support with %d fingers", pTouches.size());
            mFingerMap.clear();
    }
}

void HFViewport::zoomInOut(float dt)
{
    float curScale = this->m_TargetNode->getScale();
    float targetZoom = 0.0;
    float resolutionScale = getResolutionRate();
    if(curScale < resolutionScale)
    {
        targetZoom=curScale+m_tAutoZoomDelta;
        if(targetZoom > resolutionScale){
            targetZoom=resolutionScale;
        }
    }
    else if (curScale > resolutionScale) {
        targetZoom=curScale-m_tAutoZoomDelta;
        if(targetZoom < resolutionScale){
            targetZoom=resolutionScale;
        }
    }
    else {
        targetZoom = resolutionScale;
    }
    
    if( targetZoom != curScale ) {
        applyZoom(targetZoom);
        m_tAutoZoomDelta *= ZOOM_DEACCEL_RATE;
    }
    if (fabsf(m_tAutoZoomDistance.x) > SCROLL_DEACCEL_DIST ||
        fabsf(m_tAutoZoomDistance.y) > SCROLL_DEACCEL_DIST) {
        applyPos(m_tAutoZoomDistance);
        m_tAutoZoomDistance = ccpMult(m_tAutoZoomDistance, SCROLL_DEACCEL_RATE);
    }
    
    if (curScale == resolutionScale
        && (fabsf(m_tAutoZoomDistance.x) <= SCROLL_DEACCEL_DIST &&
            fabsf(m_tAutoZoomDistance.y) <= SCROLL_DEACCEL_DIST)) {
            unschedule(schedule_selector(HFViewport::zoomInOut));
            this->mTouchMode = TouchMode_None;
            return;
        }
}

void HFViewport::doAutoZoom(const CCPoint& curPosition){
    m_doubleClicked = true;
    this->mTouchMode = TouchMode_ZoomFling;
    beginZoom(CCPointMake(curPosition.x - 50, curPosition.y),
              CCPointMake(curPosition.x + 50, curPosition.y));
    m_tAutoZoomDistance = curPosition;
    float curScale = this->m_TargetNode->getScale();
    float resolutionScale = getResolutionRate();
    //zoom
    if(curScale < resolutionScale) {
        //计算指数递减初始值 sum = 1 / (1 - q), q为比值
        m_tAutoZoomDelta = (resolutionScale - curScale) / ((1/(1-ZOOM_DEACCEL_RATE))*0.95);
    }
    else if (curScale > resolutionScale) {
        //计算指数递减初始值 sum = 1 / (1 - q), q为比值
        m_tAutoZoomDelta = (curScale - resolutionScale) / ((1/(1-ZOOM_DEACCEL_RATE))*0.95);
    }
    //scroll
    CCPoint targetPosition = ccp(0, 0);//CCPoint targetPosition = UIUtil::moveToCenterPostion(this->m_TargetNode->convertToNodeSpace(curPosition));
    CCPoint autoCenterDifAll = ccpSub(targetPosition, this->m_TargetNode->getPosition());
    //计算指数递减初始值 sum = 1 / (1 - q), q为比值
    if (mForceStopScroll || !mMovable) {
        m_tAutoZoomDistance = CCPointZero;
    }
    else {
        m_tAutoZoomDistance = autoCenterDifAll / ((1/(1-SCROLL_DEACCEL_RATE))*0.9);
    }
    this->schedule(schedule_selector(HFViewport::zoomInOut));
}

bool HFViewport::moveableCheck(CCTouch* ptouch){
    //原先是 4.0； 现在改为15.0
    if (fabsf(ptouch->getLocation().x - mPreviewPos.x) < 15.0f &&
        fabsf(ptouch->getLocation().y - mPreviewPos.y) < 15.0f) {
        return false;
    }
    
    return true;
}

void HFViewport::onTouchesEnded(const std::vector<Touch*>& pTouches, Event *pEvent)
{
    CC_ASSERT(this->m_TargetNode);
    m_doubleClicked = false;
    
    isBeginTouch = false;
    touchSecond = 0;
    
    if (isUIMove) {
        //fusheng移动回来
        UIComponent::getInstance()->moveIn();
        isUIMove = false;
    }
    
    switch(mFingerMap.size())
    {
        case 1:
        {
            if(isInAutoZoom)
            {
                endZoom();
                isInAutoZoom = false;
                mTouchMode = TouchMode_Scroll;
                
            }
            CCTouch* ptouch =getAnyTouchObject(pTouches);
            CCLOG("touchMoved posX:%f, posY:%f", ptouch->getLocation().x, ptouch->getLocation().y);
            
            if (!moveableCheck(ptouch)) {
                CCLOG("skip in touched moved!");
                // this is a single touch action
                if (mTouchDelegate) {
                    auto singleTouch = this->m_TargetNode->convertToNodeSpace(ptouch->getLocation());
                    CCLOG("fire single touch! x:%f,y:%f",singleTouch.x,singleTouch.y);
                    mTouchDelegate->onSingleTouchEnd(singleTouch);
                    mTouchDelegate->onSingleTouchEnd(ptouch);
                    mFingerMap.clear();
                }
                return;
            }
            
            CC_BREAK_IF(this->mTouchMode != TouchMode_Scroll);
            CCTouch* objTouch = dynamic_cast<CCTouch*>(*pTouches.begin());
            CC_ASSERT(objTouch);
            
            CCLOG(" x:%f,  y:%f", objTouch->getLocation().x, objTouch->getLocation().y);

            if (!mForceStopScroll) { 
                endScroll(objTouch);
            }
            mIsScrollBegan = false;
            long time = getNowTime();
            if (time - m_tTouchTime >= 80) {
                m_tScrollDistance = CCPointZero;
            }
            mFingerMap.clear();
        }
            break;
        case 2:{
            CCLOG("end zoom------- end %d, finger(s)", pTouches.size());
            for (auto touchItem: pTouches) {
                auto touchItemP = dynamic_cast<CCTouch*>(touchItem);
                mFingerMap.erase(touchItemP->getID());
            }
            if (mFingerMap.size()) {
                isInAutoZoom = true;
                mTouchMode = TouchMode_Zoom;
                mIsScrollBegan = false;
            }else
            {
                endZoom();
            }
        }
            break;
        default:
            if(isInAutoZoom)
            {
                endZoom();
                isInAutoZoom = false;
            }
            mIsScrollBegan = false;
            break;
    }
    if (pTouches.size() == 0)
    {
        m_bTouchMoved = false;
    }
}

void HFViewport::onTouchesCancelled(const std::vector<Touch*>& pTouches, Event *pEvent)
{
    onTouchesEnded(pTouches, pEvent);
//    if(mIsProcessingScroll)
//    {
//        mIsProcessingScroll = false;
//        if (mTouchDelegate) {
//            CCTouch* objTouch = dynamic_cast<CCTouch*>(*pTouches.begin());
//            mTouchDelegate->onEndScroll(m_TargetNode->convertTouchToNodeSpace(objTouch));
//        }
//    }
}

CCSize HFViewport::getMapSize()
{
    CCSize scaledMapSize = CCSize(mMapSize.width * mCurZoomScale, mMapSize.height * mCurZoomScale);
    return scaledMapSize;
}

void HFViewport::setMapSize(const CCSize mapSize)
{
    mMapSize = mapSize;
}

long HFViewport::getNowTime()
{
//    struct cc_timeval now;
//    CCTime::gettimeofdayCocos2d(&now, NULL);
    struct timeval now;
    gettimeofday(&now, nullptr);
    long millSecond=now.tv_sec * 1000 + now.tv_usec / 1000;
    return millSecond;
}
