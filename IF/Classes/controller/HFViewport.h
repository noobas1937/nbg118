//
//  HFViewport.h
//  HappyFarm
//
//  Created by wuyuqun on 13-9-7.
//
//

#ifndef __HappyFarm__HFViewport__
#define __HappyFarm__HFViewport__

#include "cocos2d.h"
#include <map>

USING_NS_CC;

// 直接拖拽结束后用来计算拖拽速度的栈大小，然后计算出来的速度会用来进行fling操作
#define VIEWPORT_TOUCH_HISTORY_CAPACITY 12

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288 //pi
#endif

enum TouchMode
{
    TouchMode_None,     //地图静止
    TouchMode_Scroll,   //地图移动
	TouchMode_CustomScroll,
    TouchMode_Fling,    //地图自动移动(如移动后的自动移动效果)
    TouchMode_Zoom,     //地图缩放
    TouchMode_ZoomFling,//地图自动缩放(如缩放后的自动恢复，双击地图自动恢复并移动)
    TouchMode_GOUND,     //地表移动
};

typedef std::map<int, cocos2d::CCPoint> FingerMap;

// 所有的坐标是以HFViewport的target的view坐标
class ITouchDelegate
{
public:
    // 如果不想让移动，返回false即可
    virtual bool onBeginScroll(const cocos2d::CCPoint& worldLocation){return true;}
    virtual void onStepScroll(const cocos2d::CCPoint& worldLocation,const cocos2d::CCPoint& delta){}
    virtual void onEndScroll(const cocos2d::CCPoint& worldLocation){}
    virtual void onSingleTouchBegin(const cocos2d::CCPoint& worldLocaltion){}
    virtual void onSingleTouchEnd(const cocos2d::CCPoint& worldLocaltion){}
    virtual void onBeginZoom(const cocos2d::CCPoint& point1, const cocos2d::CCPoint& point2){}
    virtual void onStepZoom(const cocos2d::CCPoint& point1, const cocos2d::CCPoint& point2){}
    virtual void onEndZoom(float targetZoomScale){}
    virtual void autoMoveEnd(){};

    virtual void onSingleTouchBegin(cocos2d::CCTouch* curTouch){}
    virtual void onSingleTouchEnd(cocos2d::CCTouch* curTouch){}
};

class HFViewport:public cocos2d::CCLayer
{
public:
    virtual ~HFViewport(){};
    CREATE_FUNC(HFViewport);
    void setViewPortTarget(cocos2d::CCNode* target);
    virtual bool init();
    virtual void onTouchesBegan(const std::vector<Touch*>& pTouches, Event *pEvent);
    virtual void onTouchesMoved(const std::vector<Touch*>& pTouches, Event *pEvent);
    virtual void onTouchesEnded(const std::vector<Touch*>& pTouches, Event *pEvent);
    virtual void onTouchesCancelled(const std::vector<Touch*>& pTouches, Event *pEvent);

    float getMinZoom();
    void alignWithTargetCenter();
    void setTouchDelegate(ITouchDelegate* touchDelegate)
    {
        mTouchDelegate = touchDelegate;
    }
    // 场景当前的大小（乘了缩放率）
    CCSize getMapSize();
    void setMapSize(const CCSize mapSize);
    
    void centOnPoint(cocos2d::CCPoint curTouch, bool useTween = false);
    
    bool isAutoMove;
    bool isAutoZoom;
    
    cocos2d::CCPoint getCurrentCenterGridPoint();
    //用于自动缩放
    void intervalZoom(float dt);
    //用于自动移动
    void intervalMove(float dt);
    //用于双击缩放加移动
    void zoomInOut(float dt);
    //停止移动及缩放update
    void unscheduleAll();
    
    CCSize getZoomedPosition();
    
    virtual void onEnter();
    virtual void onExit();
    void changeSceneZoom();
    // 移动场景到指定的坐标点
    void scrollTo(const CCPoint& targetPoint, CCCallFunc* callbackFunc, bool Flying = false);
    void scrollBy(const CCPoint& targetPoint, CCCallFunc* callbackFunc,float time = 0.4f);
    void scrollToSmooth(const CCPoint& targetPoint, CCCallFunc* callbackFunc,float time = 0.8f);
    void doAutoZoom(const CCPoint& curPosition);
    
    void updatePosition(CCPoint pos, bool animate = false, float time = 1.0f, bool bound=true);
    void setSceneSize(int w, int h);
    bool isMove;
    bool notMove;
    CCPoint topRight;
    CCPoint bottomLeft;
    bool allowZoom;
    void applyZoom(float newZoom);
    CCPoint m_bgTouchPoint;
protected:
    inline void teardown()
    {
        // 多点
        mFingerMap.clear();
        mIsProcessingScroll = false;
        mIsProcessingZoom = false;
    }
    
    void beginScroll(cocos2d::CCTouch* touch);
    void stepScroll(cocos2d::CCTouch* touch);
    void endScroll(cocos2d::CCTouch* touch);
    
    void beginZoom(const cocos2d::CCPoint& point1, const cocos2d::CCPoint& point2);
    void stepZoom(const cocos2d::CCPoint& point1, const cocos2d::CCPoint& point2, float scale = 0.0f);
    void endZoom();
    void endZoomBackUpdate(float dt);
    bool boundPos(cocos2d::CCPoint& pos);
    
    void applyPos(CCPoint position);
    HFViewport():m_TargetNode(NULL),mTouchMode(TouchMode_None),mTouchDelegate(NULL),mIsScrollBegan(false),autoZoom(true),mForceStopScroll(false),isForDiamond(false),limitSpeedX(0),limitSpeedY(0),isAutoMove(true),isAutoZoom(true),isInAutoZoom(false),allowZoom(true)
    {
        teardown();
    };
        
    // 场景当前的缩放率
    CC_SYNTHESIZE(float, mCurZoomScale, CurZoomScale);
    CC_SYNTHESIZE(float, mTouchSlot, TouchSlot);
    // 停止处理屏幕滚动
    CC_SYNTHESIZE(bool, mForceStopScroll, ForceStopScroll);
    // 移动场景开关，和ForceStopScroll相比不会被任意开关－－zisong
    CC_SYNTHESIZE(bool, mMovable, Movable);
    // 触摸屏幕开关
    CC_SYNTHESIZE(bool, mTouchable, Touchable);
    CC_SYNTHESIZE(float, mZoomLimitationMin, ZoomLimitationMin);
    CC_SYNTHESIZE(float, mZoomLimitationMax, ZoomLimitationMax);
    CC_SYNTHESIZE(TouchMode, mTouchMode, ViewTouchMode);
    CC_SYNTHESIZE(FingerMap, mFingerMap, FingerMap);
    CC_SYNTHESIZE(cocos2d::CCPoint, mScrollStartPoint, ScrollStartPoint);
    CC_SYNTHESIZE(cocos2d::CCPoint, mOperationStartPostition, OperationStartPostition);
    CC_SYNTHESIZE(float, mZoomFingersDistance, ZoomFingersDistance);
    CC_SYNTHESIZE(cocos2d::CCPoint, mNSZoomCenter, NSZoomCenter);
    CC_SYNTHESIZE(cocos2d::CCPoint, mWSZoomCenter, WSZoomCenter);
    CC_SYNTHESIZE(float, mZoomOldScale, ZoomOldScale);
    CC_SYNTHESIZE(cocos2d::CCRect, mWorldBound, WorldBound);
    CC_SYNTHESIZE_READONLY(bool, mIsProcessingScroll, IsProcessingScroll);
    CC_SYNTHESIZE_READONLY(bool, mIsProcessingZoom, IsProcessingZoom);
    CC_SYNTHESIZE_READONLY(bool, mIsLongClickCheckingScheduled, IsLongClickCheckingScheduled);
    CC_SYNTHESIZE(bool, mIsScrollBegan, IsScrollBegan);
    CC_SYNTHESIZE_READONLY(ITouchDelegate*, mTouchDelegate, TouchDelegate);
    cocos2d::CCNode* m_TargetNode;
    
    CC_SYNTHESIZE(bool, autoZoom, IsAutoZoom);
    CCSize mMapSize;
    
private:
    // 取得ccb分辨率与当前屏幕分辨率之间的比
    float getResolutionRate();
    bool moveableCheck(CCTouch* ptouch);
    long getNowTime();
    CCPoint mPreviewPos;
    //用于地图移动Fling
    long    m_tTouchTime;
    CCPoint m_tTouchPoint;
    CCPoint m_tScrollDistance;
    bool m_bTouchMoved;
    //用于双击移动
    CCPoint m_tAutoZoomDistance;
    //用于双击缩放
    float m_tAutoZoomDelta;
    bool m_doubleClicked;
    //用于缩放
    float zoomDelta;
    //用于控制自动缩回时机
    bool isInAutoZoom;
    
    Touch* getAnyTouchObject(const std::vector<Touch*>& pTouches);
public:
    // used for diamond shape map
    bool isForDiamond;
    int limitSpeedX;
    int limitSpeedY;

private:
    int MAP_WIDTH;
    int MAP_HEIGHT;
};

#endif /* defined(__HappyFarm__HFViewport__) */
