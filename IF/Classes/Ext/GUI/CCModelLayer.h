//
//  CCModelLayer.h
//  ccgoe
//
//  Created by 谢文杰 on 12-11-27.
//
//

#ifndef __ccgoe__CCModelLayer__
#define __ccgoe__CCModelLayer__

#include "cocos-ext.h"
#include "cocosextV3.h"
NS_CC_EXT_BEGIN

class CCModelLayer: public Layer
{
public:
    CCModelLayer(bool bRestraintTouchInside=true)
    : m_bRestraintTouchInside(bRestraintTouchInside), m_bTouchPriority(1) {_ignoreAnchorPointForPosition = false;};
    virtual ~CCModelLayer(){};
    
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(CCModelLayer, create);
    
    virtual bool init();
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    //virtual void registerWithTouchDispatcher(void);
    
    void setTouchPriority(int priority);
    int getTouchPriority()
    { return m_bTouchPriority; }
    
protected:
    int m_bTouchPriority;
    
    CC_SYNTHESIZE(bool, m_bRestraintTouchInside, RestraintTouchInside);
};

class CCModelLayerColor: public CCLayerColor
{
public:
    CCModelLayerColor(bool bRestraintTouchInside=true)
    : m_bRestraintTouchInside(bRestraintTouchInside), m_bTouchPriority(1),m_function(nullptr),m_throughBegan(nullptr),m_throughEnd(nullptr),m_bTouchThrough(false) {};
    virtual ~CCModelLayerColor(){};
    
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(CCModelLayerColor, create);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

 //   virtual void registerWithTouchDispatcher(void);
    
    void setTouchPriority(int priority);
    void setTouchFunction(std::function<void(cocos2d::CCTouch *pTouch)> function);
    
    void setThroughBegan(std::function<bool(cocos2d::CCTouch *pTouch,cocos2d::CCEvent *pEvent)> function)
    {
        m_throughBegan = function;
    }
    void setThroughEnd(std::function<void(cocos2d::CCTouch *pTouch,cocos2d::CCEvent *pEvent)> function){
        m_throughEnd = function;
    }
    
    int getTouchPriority()
    { return m_bTouchPriority; }
    
protected:
    
    std::function<void(cocos2d::CCTouch *pTouch)> m_function;
    
    std::function<bool(cocos2d::CCTouch *pTouch,cocos2d::CCEvent *pEvent)> m_throughBegan;
    std::function<void(cocos2d::CCTouch *pTouch,cocos2d::CCEvent *pEvent)> m_throughEnd;
    bool m_bTouchThrough;
    int m_bTouchPriority;
    
    CC_SYNTHESIZE(bool, m_bRestraintTouchInside, RestraintTouchInside);
};

NS_CC_EXT_END

#endif /* defined(__ccgoe__CCModelLayer__) */
